# Database - SQLite (Theory and Code)

This folder explores about one of the databases sqlite using C. We will talk about the theories of databases and some concepts to build one using C language.

The sqlite architecture follows the pattern -

1. Tokenizer
2. Parser
3. Code Generator
4. Virtual Machine
5. B-Tree
6. Pager
7. OS Interface

A query goes through a chain of components to retrieve the data. The sqlite working could be divided into 2 parts - front-end and back-end.

The frontend consists of the following elements -

1. Tokenizer
2. Parser
3. Code Generator

The input to the frontend is the sql query and the output is virtual machine byte code (a compiled program that can run in the database).

The backend consists of -

1. Virtual Machine
2. B-Tree
3. Pager
4. Pager
5. OS Interface

The frontend output (byte codes) will be the input to the virtual machine. The virtual machine will use this byte code to perform operations on one or more tables or indexes. The tables and indexes are stored in a data structure called B-Tree.

Each B-Tree consists of many nodes. Every node is of length one page. B-Tree can store these pages to disk or retrieve them back by issuing commands to pager.

The pager takes commands to read/write pages of data. It keeps track of the offset in the database file where the read/write will happen. It also keeps track of the recently accessed page and decides when to save them back to disk.

The OS Interface is the layer that differs for different operating systems. This interface was built for portability between different OS. It defines the way OS specific tasks like opening, writing, reading or closing files in a disk will happen. SQLite has VFSes object that provides methods for such along with some other OS specific things like getting the current time, randomly generating numbers. Currently SQLite only supports VFSes for unix and windows systems.


