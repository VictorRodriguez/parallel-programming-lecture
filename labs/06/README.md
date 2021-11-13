# Lab 06 instructions

## Objective

Make the students familiar with Inter process communication.

There are 2 types of process:

* Independent Processes – Processes that do not share data with other processes.
* Cooperating Processes – Processes that shares data with other processes.

The cooperating process requires Inter process communication (IPC) mechanism.
Inter-Process Communication is the mechanism by which cooperating process share
data and information.  The mechanism that will allow them to exchange data and
information are the following:

* Shared memory    
* Message Passing

(Both examples here)

# Requirements

* Linux machine, either a VM or a bare-metal host
* git send mail server installed and configured on your Linux machine

## Instructions

* Create a program that handles the coordination of 5 milk sellers.
* Each seller can sell a random number of bottles ( from 1 to 500 )
* The factory only can produce daily 1000 bottles
* Headquarter need to take care that sellers do not sell more than 1000 bottles
* Sellers shoudl be processes
* Syncronization shoudl be with SEMAPHORES
* Inter process comunication can be with Shared memory or Pipes (op to you)

## Please use your STUDENT-ID to create the commit

    ```
        $ git add milk_seller.c
        $ git commit -s -m <STUDENT-ID>-homework-02

    ```

# Time to do the homework:

One week from the moment the mail is sent to students

