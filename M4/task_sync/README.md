# Milestone 4 Task Synchronization Application
### This folder contains the Task Synchronization Applet for Milestone 4.

## How to Build and Run
Simply run 'make' in this directory to build the program. The output hex file is then stored in the "./Build" directory as "barrier.hex".

At the beginning of the program the user is asked to enter a number for the amount of threads to be made available (1-6), this number essentially controls how many tasks can run simultaneously. Enter a number 1-6, and the program will begin running.

## Basic Functionality
In this program is contained 6 "consumer" tasks. In order for a consumer task to run, it must consume a thread (this thread is returned when the task is finished). Once a consumer task finishes it's task, it must wait for all of the 5 other consumer tasks to finish before it is allowed to run again; and the consumer tasks must be allowed to occur in any order at any cycle. This is implemented via the 'Reusable Barrier' method discussed in Allen Downey's <em>Little Book of Semaphores</em>, where every consumer task is a turnstile for every other consumer task. 

The consumer tasks aren't actually producing or doing any work upon eachother, they are simply yeilding for a random number of seconds between 0 and 16 to simulate work being done.