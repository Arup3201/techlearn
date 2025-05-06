# Operating System

## Processes

Process is an abstraction of a running program.

The difference between process and program is subtle so let's understand with an example -

A computer scientist is baking a cake for his daughter. For that he needs ingredients to cook the cake. Here the program is the algorithm or recipie he will follow to bake the cake, for that he needs some input which is ingredients here. The computer scientist is the processor in this example as he is the one performing those tasks. Process is the activity of cooking which involves the recipie, inputs and output together.

Now we can also understand the context switching between process with the same example -

If the computer scientist's son comes to him crying because a bee has bitten him, then the computer scientist will go to remove the bee sting. Here he will switch the context from cooking to more higher priority task of removing the bee sting. For this, he also needs to remember where he left off in his cake baking activity. When he comes back to baking, he will start off from where he left.

The key idea here is that a process is an activity of some kind, it has inputs, outputs and state. Multiple process can share the same processor and get their job done with context switching.

### Process Creation

Process can be created in 4 situations -

1. When an operating system is booted it starts some processes. Some process run on the foregroung. They are process that users directly interact with and performs their task. Some are background tasks that are always running but only triggered when something happens. For example, process may be designed to serve web pages when a page request comes to the host machine. When a request comes they wake up to serve web pages from the host machine. Such kind of processes that run on the backgroung and help in serving web pages, printers and so on are called **daemon**.

2. Process can be created by the already existing processes as well. A running program can issue system call to create other processes. When one work can be formulated by several related but independent works, multiple processes can be called by one process. For example, `make` command will tell C compiler to compile the source file to object code, and then invoke the `install` program to copy to destination, set ownerships and permissions.

3. Many interactive operating system allow it's user to start a new process.

4. In mainframe systems where users request batch jobs, when the OS decides that the resources are available it creates a new process and runs the next job in input queue.
