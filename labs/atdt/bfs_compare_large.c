#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

typedef struct {
    int *neighbors;
    int degree;
    int capacity;
} Node;

typedef struct {
    int *data;
    int size;
    int capacity;
} Frontier;

static double now_sec(void) {
#ifdef _OPENMP
    return omp_get_wtime();
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
#endif
}

static void frontier_init(Frontier *f, int capacity) {
    f->data = (int *)malloc((size_t)capacity * sizeof(int));
    if (!f->data) {
        fprintf(stderr, "malloc failed in frontier_init\n");
        exit(1);
    }
    f->size = 0;
    f->capacity = capacity;
}

static void frontier_reset(Frontier *f) {
    f->size = 0;
}

static void frontier_free(Frontier *f) {
    free(f->data);
    f->data = NULL;
    f->size = 0;
    f->capacity = 0;
}

static void frontier_push(Frontier *f, int value) {
    if (f->size >= f->capacity) {
        fprintf(stderr, "frontier overflow: size=%d capacity=%d\n",
                f->size, f->capacity);
        exit(1);
    }
    f->data[f->size++] = value;
}

static void graph_init(Node *graph, int n) {
    for (int i = 0; i < n; i++) {
        graph[i].neighbors = NULL;
        graph[i].degree = 0;
        graph[i].capacity = 0;
    }
}

static void add_edge(Node *graph, int u, int v) {
    if (graph[u].degree >= graph[u].capacity) {
        int new_cap = (graph[u].capacity == 0) ? 8 : graph[u].capacity * 2;
        int *new_neighbors =
            (int *)realloc(graph[u].neighbors, (size_t)new_cap * sizeof(int));
        if (!new_neighbors) {
            fprintf(stderr, "realloc failed in add_edge\n");
            exit(1);
        }
        graph[u].neighbors = new_neighbors;
        graph[u].capacity = new_cap;
    }
    graph[u].neighbors[graph[u].degree++] = v;
}

static void graph_free(Node *graph, int n) {
    for (int i = 0; i < n; i++) {
        free(graph[i].neighbors);
    }
}

static int node_id(int level, int index_in_level, int width) {
    return level * width + index_in_level;
}

/*
 * Build a layered directed graph:
 *   - levels levels
 *   - width nodes per level
 *   - each node connects to edges_per_node random nodes in next level
 *
 * Total nodes = levels * width
 * Source = node(0,0)
 */
static void build_layered_graph(Node *graph,
                                int levels,
                                int width,
                                int edges_per_node,
                                unsigned int seed) {
    int total_nodes = levels * width;
    graph_init(graph, total_nodes);

    srand(seed);

    for (int l = 0; l < levels - 1; l++) {
        for (int i = 0; i < width; i++) {
            int u = node_id(l, i, width);

            for (int e = 0; e < edges_per_node; e++) {
                int next_idx = rand() % width;
                int v = node_id(l + 1, next_idx, width);
                add_edge(graph, u, v);
            }
        }
    }
}

static double bfs_serial(Node *graph, int n, int source, int *level) {
    Frontier current, next;
    int *visited = (int *)calloc((size_t)n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "calloc failed in bfs_serial\n");
        exit(1);
    }

    frontier_init(&current, n);
    frontier_init(&next, n);

    for (int i = 0; i < n; i++) {
        level[i] = -1;
    }

    visited[source] = 1;
    level[source] = 0;
    frontier_push(&current, source);

    int current_level = 0;
    double t0 = now_sec();

    while (current.size > 0) {
        frontier_reset(&next);

        for (int i = 0; i < current.size; i++) {
            int u = current.data[i];

            for (int j = 0; j < graph[u].degree; j++) {
                int v = graph[u].neighbors[j];
                if (!visited[v]) {
                    visited[v] = 1;
                    level[v] = current_level + 1;
                    frontier_push(&next, v);
                }
            }
        }

        Frontier tmp = current;
        current = next;
        next = tmp;
        current_level++;
    }

    double t1 = now_sec();

    free(visited);
    frontier_free(&current);
    frontier_free(&next);

    return t1 - t0;
}

static double bfs_parallel(Node *graph, int n, int source, int *level) {
    Frontier current, next;
    Frontier *locals = NULL;
    int *visited = (int *)calloc((size_t)n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "calloc failed in bfs_parallel\n");
        exit(1);
    }

#ifdef _OPENMP
    int num_threads = omp_get_max_threads();
#else
    int num_threads = 1;
#endif

    locals = (Frontier *)malloc((size_t)num_threads * sizeof(Frontier));
    if (!locals) {
        fprintf(stderr, "malloc failed for locals\n");
        exit(1);
    }

    frontier_init(&current, n);
    frontier_init(&next, n);

    for (int t = 0; t < num_threads; t++) {
        frontier_init(&locals[t], n);
    }

    for (int i = 0; i < n; i++) {
        level[i] = -1;
    }

    visited[source] = 1;
    level[source] = 0;
    frontier_push(&current, source);

    int current_level = 0;
    double t0 = now_sec();

    while (current.size > 0) {
        frontier_reset(&next);
        for (int t = 0; t < num_threads; t++) {
            frontier_reset(&locals[t]);
        }

#ifdef _OPENMP
        #pragma omp parallel
        {
            int tid = omp_get_thread_num();
            Frontier *my_frontier = &locals[tid];

            #pragma omp for schedule(dynamic, 256)
            for (int i = 0; i < current.size; i++) {
                int u = current.data[i];

                for (int j = 0; j < graph[u].degree; j++) {
                    int v = graph[u].neighbors[j];

                    /* first thread that flips visited[v] from 0 to 1 wins */
                    if (__sync_bool_compare_and_swap(&visited[v], 0, 1)) {
                        level[v] = current_level + 1;
                        frontier_push(my_frontier, v);
                    }
                }
            }
        }
#else
        Frontier *my_frontier = &locals[0];
        for (int i = 0; i < current.size; i++) {
            int u = current.data[i];

            for (int j = 0; j < graph[u].degree; j++) {
                int v = graph[u].neighbors[j];
                if (!visited[v]) {
                    visited[v] = 1;
                    level[v] = current_level + 1;
                    frontier_push(my_frontier, v);
                }
            }
        }
#endif

        /* merge per-thread local frontiers into next frontier */
        for (int t = 0; t < num_threads; t++) {
            Frontier *lf = &locals[t];
            if (next.size + lf->size > next.capacity) {
                fprintf(stderr, "next frontier overflow during merge\n");
                exit(1);
            }
            memcpy(&next.data[next.size], lf->data, (size_t)lf->size * sizeof(int));
            next.size += lf->size;
        }

        Frontier tmp = current;
        current = next;
        next = tmp;
        current_level++;
    }

    double t1 = now_sec();

    free(visited);
    frontier_free(&current);
    frontier_free(&next);

    for (int t = 0; t < num_threads; t++) {
        frontier_free(&locals[t]);
    }
    free(locals);

    return t1 - t0;
}

static int compare_levels(const int *a, const int *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

static long count_reached(const int *level, int n) {
    long reached = 0;
    for (int i = 0; i < n; i++) {
        if (level[i] >= 0) {
            reached++;
        }
    }
    return reached;
}

int main(int argc, char **argv) {
    int levels = 12;
    int width = 20000;
    int edges_per_node = 8;
    int repeats = 3;
    unsigned int seed = 12345;

    if (argc > 1) levels = atoi(argv[1]);
    if (argc > 2) width = atoi(argv[2]);
    if (argc > 3) edges_per_node = atoi(argv[3]);
    if (argc > 4) repeats = atoi(argv[4]);

    if (levels < 2 || width < 1 || edges_per_node < 1 || repeats < 1) {
        fprintf(stderr, "Usage: %s [levels] [width] [edges_per_node] [repeats]\n",
                argv[0]);
        return 1;
    }

    int n = levels * width;
    int source = 0;

    printf("Building layered graph...\n");
    printf("  levels         = %d\n", levels);
    printf("  width          = %d\n", width);
    printf("  edges/node     = %d\n", edges_per_node);
    printf("  total nodes    = %d\n", n);
    printf("  approx edges   = %lld\n",
           (long long)(levels - 1) * (long long)width * (long long)edges_per_node);
    printf("  repeats        = %d\n", repeats);

#ifdef _OPENMP
    printf("  OpenMP threads = %d\n", omp_get_max_threads());
#else
    printf("  OpenMP threads = 1 (no OpenMP)\n");
#endif
    printf("\n");

    Node *graph = (Node *)malloc((size_t)n * sizeof(Node));
    int *level_serial = (int *)malloc((size_t)n * sizeof(int));
    int *level_parallel = (int *)malloc((size_t)n * sizeof(int));

    if (!graph || !level_serial || !level_parallel) {
        fprintf(stderr, "malloc failed in main\n");
        free(graph);
        free(level_serial);
        free(level_parallel);
        return 1;
    }

    build_layered_graph(graph, levels, width, edges_per_node, seed);

    double total_serial = 0.0;
    double total_parallel = 0.0;

    for (int r = 0; r < repeats; r++) {
        double ts = bfs_serial(graph, n, source, level_serial);
        double tp = bfs_parallel(graph, n, source, level_parallel);

        total_serial += ts;
        total_parallel += tp;

        if (!compare_levels(level_serial, level_parallel, n)) {
            fprintf(stderr, "Mismatch detected on repeat %d\n", r);
            graph_free(graph, n);
            free(graph);
            free(level_serial);
            free(level_parallel);
            return 2;
        }
    }

    double avg_serial = total_serial / repeats;
    double avg_parallel = total_parallel / repeats;
    long reached = count_reached(level_serial, n);

    printf("Results match: YES\n");
    printf("Reached nodes     : %ld / %d\n", reached, n);
    printf("Avg serial time   : %.6f seconds\n", avg_serial);
    printf("Avg parallel time : %.6f seconds\n", avg_parallel);

    if (avg_parallel > 0.0) {
        printf("Speedup           : %.3fx\n", avg_serial / avg_parallel);
    }

    graph_free(graph, n);
    free(graph);
    free(level_serial);
    free(level_parallel);

    return 0;
}
