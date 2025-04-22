# Core Concepts in OS

The interface between operating system and user programs is defined the set of extended instructions provided by the operating system. This extended instructions are **system calls**. These calls vary from operating system to operating system, here we are going to look at the MINIX 3 system calls.

The MINIX 3 system calls fall broadly into 2 categories - those dealing with process and those dealing with file system.

## Process 

A process is a program in execution. A process is assigned an address space, a list of memory locations, which the process can read and write. The address space contains the executable program, program data, and it's stack. Also associated with each process are some set of registers - program counter, stack pointer and some other registers to keep the process running.

In multi-programming systems, it stops one process while restarts another process if the CPU time taken from one process is more. But when the process is restarted, it should be started from the same state when it was stopped. For that it needs to store the information related to the process somewhere during the suspension. For example, one process might read a file and then it switches to another process. Here the process has opened a lot of files and associated with it is the pointer for the current position.

In operating systems, all the information apart from the contents of it's own address space are stored in an operating system table called **process table**. It is an array of structures. Each structure is an information for process in existance. 

Thus a process consists of it's address space also called **core image**, and it's process table entry which contains the registers and other things.

The key process management system calls are those to start and stop the process.
