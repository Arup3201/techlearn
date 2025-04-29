# Database Design and Implementation

DBMS are part of operating sysem, so let's discuss some concepts in OS before coming to DBMS. An operating system is a bridge between user applications and hardware resources. OS gives access to hardware resources through it's system calls. User application uses these system calls to implement the services. 

System calls are akin to ordinary function calls in the operating system space. They are the programming interface for the operating system. To make application interface portable accross multiple operating systems, POSIX (Portable Operating System Interface for Unix) defines a set of application programming interfaces (APIs).

Applications don't directly call to these system calls instead they use the APIs just like functions which in turn invokes the system call to use the hardware services. For example, in Linux *libc* is a shared library that implements read, write, sleep etc APIs for C language applications.

An operating system distribution comes with *utilities*. Utilities are ready-made special standalone software programs that help users navigate the operating system more easily. Some utilities include compiler, assembler, loader, linker, debugger, database management system, shell, I/O operations etc. 

A database management system is the utility that uses persistent storage system. It is important for us to know about the storage hardware that is used by DBMS. 

## I/O Controller

I/O controllers are connect I/O devices with the host computer system. In modern compurer systems, the I/O controllers handle I/O operations on behalf of the main process. I/O controllers can carry out I/O operations without any intervention of the main process. The main processor starts the I/O operation by sending it the I/O command and the input data. The I/O controller then executes the I/O command in it's own speed, when it completes the command execution it sends the output and the status of the execution to processor.

Almost all modern I/O handlers have interrupt circuits. When the controller wants the attention of the processor, it issues an interrupt by raising signal on it's outgoing interrupt request line which is connected to the interrupt pin of the processor. On receiving the interrupt, the processors stops it's current process execution and starts a new program which handles the interrupt. The interrupt handler collects the status information and output data from the controller. 

## Disk

Disk is a persistent storage system that is widely used. Disk is an array of fixed sized blocks. A block is a sequence of bytes and typically stores 1024, 2048 or 4096 bytes of data.

## Introduction to Database System

**Data Item**

A *data item* carries or holds a piece of information, and the information represents the state of some physical or logical entity. The information is encoded in the form of a value of the data item. That is the meaning we assign to the value is the information.

The *granularity* of a data item defines how much information it can store. A *database* is a single repository that contains many persistent data items. A data item does not reside alone in a database, it resides along with other related data items. Thus a database contains a collection of data items. The relationship on data items are defined by some integrity constraints.

The state of a database is consistent if the all the integrity constraints on the data items are satsified. Users apply different database operations to retrive information or store it or modify it. A database resides in one or multiple native files and they are called database files.

A data model is an abstract model that organizes elements of data and standardizes how they relate to one another and to the properties of real-world entities.

The one we mostly talk about is the relational data model.

## Relational Data Model

Relational data model has 2 things - relationship and entity. An entity is an abstract object of some sort that describes a real world entity (physical or logical). A relationship is an association between two or more entities.


