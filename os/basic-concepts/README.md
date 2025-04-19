# Core Concepts in OS

The interface between operating system and user programs is defined the set of extended instructions provided by the operating system. This extended instructions are **system calls**. These calls vary from operating system to operating system, here we are going to look at the MINIX 3 system calls.

The MINIX 3 system calls fall broadly into 2 categories - those dealing with process and those dealing with file system.

## Process 

A process is a program in execution. A process is assigned an address space, a list of memory locations, which the process can read and write. The address space contains the executable program, program data, and it's stack. Also associated with each process are some set of registers - program counter, stack pointer and some other registers to keep the process running.


