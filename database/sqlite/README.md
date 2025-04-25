# Database Design and Implementation

DBMS are part of operating sysem, so let's discuss some concepts in OS before coming to DBMS. An operating system is a bridge between user applications and hardware resources. OS gives access to hardware resources through it's system calls. User application uses these system calls to implement the services. 

System calls are akin to ordinary function calls in the operating system space. They are the programming interface for the operating system. To make application interface portable accross multiple operating systems, POSIX (Portable Operating System Interface for Unix) defines a set of application programming interfaces (APIs).

Applications don't directly call to these system calls instead they use the APIs just like functions which in turn invokes the system call to use the hardware services. For example, in Linux *libc* is a shared library that implements read, write, sleep etc APIs for C language applications.

An operating system distribution comes with *utilities*. Utilities are ready-made special standalone software programs that help users navigate the operating system more easily. Some utilities include compiler, assembler, loader, linker, debugger, database management system, shell, I/O operations etc. 

A database management system is the utility that uses persistent storage system. It is important for us to know about the storage hardware that is used by DBMS. 

## I/O Controller

I/O controllers are connect I/O devices with the host computer system. In modern compurer systems, the I/O controllers handle I/O operations on behalf of the main process. I/O controllers can carry out I/O operations without any intervention of the main process. The main processor starts the I/O operation by sending it the I/O command and the input data. The I/O controller then executes the I/O command in it's own speed, when it completes the command execution it sends the output and the status of the execution to processor.

Almost all modern I/O handlers have interrupt circuits. When the controller wants the attention of the processor, it issues an interrupt by raising signal on it's outgoing interrupt request line which is connected to the interrupt pin of the processor. On receiving the interrupt, the processors stops it's current process execution and starts a new program which handles the interrupt. The interrupt handler collects the status information and output data from the controller. 
