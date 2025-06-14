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

### Process Termination

After doing it's work all processes will terminate. There are 4 reasons why a process might terminate -

- Normal Exit: A process has done it's work, and then it tells the operating system to terminate itself. Like a compiler after compiling the program, will stop it's execution. This call is `exit` in MINIX 3 system.
- Error Exit: The process may discover a fatal error, and then it exits. For example a compiler is trying to compile a file that does not exist then it will simply exit.
- Fatal Error Exit: The process may have some program bug which can cause termination for the process. It may be referencing wrong memory, accessing illegal resources, or dividing by zero.
- Killed by other process: One process with the necessary authentication can execute the system call to terminate other processes.

### Process Tree

In an operating system when one process creates another process, the parent and child process become associated in some way. In MINIX 3, all the parent, children processes are part of process group. 

When a signal occurs (example - keyword signal), it will reach all the members of that process group. The process could decide whether to listen to the signal, ignore it or take the default action (exiting).

In MINIX 3, the boot image contains 2 servers **reincarnation server** and **init**. Reincaranation server restarts drivers and servers if they stop. Init uses the `/etc/rc` script to run all the drivers and servers that are not present in the boot image. All those processes become children process of reincarnation server, and when any of them stops it will be informed. Then it can start those drivers and servers again.

This is a tolerance mechanism for MINIX 3 when any driver or server crashes.

When init has done that, it will open the configuration file `/etc/ttytab` to see which terminals or virtual terminals exist. Init forks getty process for each one, getty execs login process, login process waits for keyboard input, after typing if the login is successful, then login execs a shell for that user. So, the shell is a child of init. After that any process created from shell will be a grandchildren of the init process.

This is an example of how process tree works.

### Process States

Every process can be in any of the 3 states -

1. Blocked
2. Ready
3. Running

A process may go into blocked state when it is waiting for something to happen e.g inputs from keyboard. Then the process will only be starting again when the input is received. After the input, the process goes into ready state which means the process is ready to be executed by the CPU.

In this time, if the CPU is idle then it will immediately start the process and the process will be in running state.

While a process is running scheduler may decide that other process also needs some CPU execution, it may send the process in ready state and give other process the CPU utilization, making that process in running state.

After some time if the scheduler decides, it will put the previous process back into running state.

### Implementation of Process

Process related information are stored in the process table. Each entry in the table is called process control block. Process control blocks contain information of the program counter, registers, stack pointer, open files, scheduling information and many other things.

The information in the process control block is useful for the time when the process goes from ready state to running state. With those data, the process can start just when it stopped without us realizing that it actually stopped.

## Race conditions

When two or more processes try to read/write to a shared data and the final output depends on who runs precisely when, is called race condition. Let's understand with a *printer spooler* example.

When a program wants to print a file, it enters the filename in a spooler directory. There is another program *print daemon* which peridically checks if there are any files that need to be printed, if there is then it will remove the filename from the spooling directory and print the file.

Let's take two process A and B who want to print a file. In the spooling directory there are multiple slots where filenames can be stored. There are two shared variables *in* and *out*. *in* variable tracks the slot which is empty, and *out* variable tracks the file which will be printed. Slot 0-3 are empty (files in those slots are already printed), and slot 4-6 are full. *in* variable is 7 and *out* is 4 right now.

Process A decides to print a file, so it reads the *in* and stores 7 to store the file. Suddenly CPU decides to switch to Process B because A has run enough. Process B starts and reads *in* and stores 7 in it's local variable. Then it stores the filename at 7 and then increases *in* to 8. After some time, process A starts again, and stores a file at 7, increase the *in* to 8 again. Process A will soon get the file printed but process B will wait but never get it's file printed because it has lost the file in the middle.

When a process tries to access any shared resource race conditions happen. In a process the section where the shared variable is accessed is called **critical section**. We need to take measure for that section to prevent the race condition problem.

We have to make sure that another process does not enter the critical section when one process is already executing something in that section. Put in other words, we need **mutual exclusion** which ensures no 2 processes can access the critical section at the same time.

The requirements to solve the mutual exclusion are -

1. No two processes can be inside their critical section at the same time
2. No assumptions can be made with the CPU speed
3. No process outside it's critical section may block other processes
4. No process have to wait forever to get inside it's critical section

### Race Condition Example

Two processes try to increment the count (shared variable). But different execution gives different final result for `count` because of race condition between the two processes.

```c
#include<stdio.h>
#include<pthread.h>

int counter = 0;

void* increment_counter() {
    for(int i=0; i<=4; i++) {
        counter += 1;
    }

    return NULL;
}

int main() {
    pthread_t thread_1, thread_2;

    pthread_create(&thread_1, NULL, (void*)increment_counter, NULL);
    pthread_create(&thread_2, NULL, (void*)increment_counter, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("counter=%d\n", counter);

    return 0;
}
```
### Mutual exclusion methods

**disabling interrupts**:

For mutual exclusion, the easiest method is to disable all interrups when a process enters it's critical region and enable them when they leave the critical region. With interrups disabled, CPU will not switch to other process.

This approach is not right because it is not a good idea to give user processes the power to turn interrupts off.

**lock variable**: 

The second approach is to use a *single shared variable (lock)*, initially 0. When a process wants to access it's critical section, it checks the lock is 1 or 0. If the lock is 0, the process sets it to 1, and enters the critical section. If the lock is 1, then it will wait till the other process leaves the critical section.

But this idea has a fatal problem, which is same as the spooler directory. If one process tries to enter the critical section, it reads the lock and sees that it is 0. But suddenly CPU switched to another process. The other process checks the lock, it is 0, enters the critical section. CPU switches to the previous process again. It resumes from where it left, and enters the critical section. Now, 2 processes are in their critical section at the same time.

**busy waiting**

Another approach to mutual exclusion is to wait untill the turn becomes equal to process no. It is called *busy waiting* as we are waiting till the turn comes for the current process.

Following is the pseudo-code for 2 processes:

```c
// Process 0
while(TRUE) {
    while(turn!=0);
    critical_section();
    turn=1;
    noncritical_section();
}

// Process 1
while(TRUE) {
    while(turn!=1);
    critical_section();
    turn=0;
    noncritical_section();
}
```

In busy waiting, the problem is CPU will be wasting it's time if something goes wrong. For example, process 0 sets turn to 1 and goes into the noncritical region, process 1 then goes into the critical region, sets the turn to 0 and goes into the noncritical region. Process 0 quickly finshes the while loop, sets the turn to 1, and enters the noncritical region. Then process 0 goes back to while loop and gets stuck because process 1 still has not finished it's noncritical region even though it's turn has already come. So, process 0 will have to wait till process 1 finishes.

### Peterson's algorithm

Using the concept of lock variable and busy waiting, peterson's algorithm was designed where the processes will call *enter_region* and *leave_region* before and after it's critical section. If there are 2 processes, process 0 has to call *enter_region* before entering the critical region and wait if it is not safe to enter the critical region. If it is safe to enter the critical section, it executes the critical section, and leaves the critical section by calling *leave_region*.

How does this solution works?

Process 0 enters the *enter_region*, sets the turn to 0, and tells that it is interested to enter the critical region. It checks whether it is not his turn or other process is not interested to enter to the critical section. If so, it will enter the critical section, and when leaving it will change it's interested state to not interested. At this time, if process 1 tries to enter the region, it turns the turn to 1, says that it is interested but when it checks it notices that other process is already in the critical section so it waits.

If both process enter the region at the same time, the last value will be the final value. Let's say process 1 enters last, makes the turn to 1. Process 0 will enter it's critical section immediately, but process 1 will have to wait.

Following is the code that implements peterson's algorithm:

```c
#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>

#define N 2
#define MAX 1000000

int count = 0;

int turn;
bool interested[N];

void enter_region(int process) {
    int other;
    other = 1 - process;

    interested[process] = true;
    turn = process;

    while(turn==process && interested[other]==true);
}

void leave_region(int process) {
    interested[process] = false;
}

void* increment_count(void *arg) {
    int *process = arg;
    for(int i=0; i<MAX; i++) {
        enter_region(*process);
        count += 1;
        leave_region(*process);
    }

    return NULL;
}

int main() {
    pthread_t thread_1, thread_2;

    int process_ids[2] = {0, 1}; // different memory for process IDs

    pthread_create(&thread_1, NULL, increment_count, &process_ids[0]);
    pthread_create(&thread_2, NULL, increment_count, &process_ids[1]);

    for(int i = 0; i<N; i++) interested[i] = false;

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("%d\n", count);

    return 0;
}
```

### Producer Consumer Problem 

The methods discussed above need busy waiting which is not a good way to utilize CPU. Also it causes priority inversion problem - where low priority process may run indefinitely while blocking high priority process.

`sleep` and `wakeup` are two system calls that can be used to prevent this problem. `sleep` call will block the process and it will only wakeup when another process calls the `wakeup` system call.

Let's understand it using the producer consumer problem. 

There are 2 processes - producer and consumer. There is a shared buffer of fixed size - producer puts information on the buffer, and the consumer takes it out.

Following program shows the flow of producer and consumer -

```c
#define N 100
#define count 0

#define FALSE 0
#define TRUE 1

void producer(void) {
	while(TRUE) {
		item = produce_item();

		if(count==N) sleep();

		insert_item(item);
		count = count + 1;

		if(count==1) wakeup(consumer);
	}
}

void consumer(void) {
	while(TRUE) {
		if(count==0) sleep();
		
		item = remove_item();
		count = count - 1;

		if(count==N-1) wakeup(producer);
	}
}
```

The problem happens because there is no constraint when accessing `count`. Let's take one scenario - buffer is empty, consumer reads `count` and notice that it is 0. Suddenly scheduler switches to producer. Producer notices that `count` is 0, puts one item in the buffer, increases `count`, and then `wakeup` consumer. Consumer is already woke up so the signal just gets lost. The execution comes to consumer, resuming it's previous flow it goes to `sleep` because `count` is 0. Sonner or later producers fills the buffer, and goes to sleep as well. Now, both will sleep forever.

This problem can be solved if we can somehow manage the wakeup call that was lost. Due to this, the consumer failed to wakeup and both slept forever. So, we will keep a *wakeup waiting bit*. When a wakeup call is sent to a process that is still awake, this bit is set. Later when the process goes for sleep, we turn this bit off but the process stays awake. But the problem can easily grow, and we will need to include more of those waiting bits if more than 2 processes get involved.

That is why *semaphores* are introduced.

### Semaphores 

---

A useful way to think of a semaphore as used in a real-world system is as a record of how many units of a particular resource are available, coupled with operations to adjust that record safely (i.e., to avoid race conditions) as units are acquired or become free, and, if necessary, wait until a unit of the resource becomes available.

Semaphores that allow an arbitrary resource count are called counting semaphores, while semaphores that are restricted to the values 0 and 1 (or locked/unlocked, unavailable/available) are called binary semaphores and are used to implement locks.

**Library analogy**: 

Suppose a physical library has ten identical study rooms, to be used by one student at a time. Students must request a room from the front desk. If no rooms are free, students wait at the desk until someone relinquishes a room. When a student has finished using a room, the student must return to the desk and indicate that the room is free.

In this scenario, the front desk count-holder represents a counting semaphore, the rooms are the resource, and the students represent processes/threads. The value of the semaphore in this scenario is initially 10, with all rooms empty. When a student requests a room, they are granted access, and the value of the semaphore is changed to 9. After the next student comes, it drops to 8, then 7, and so on. If someone requests a room and the current value of the semaphore is 0,[2] they are forced to wait until a room is freed (when the count is increased from 0). If one of the rooms was released, but there are several students waiting, then any method can be used to select the one who will occupy the room (like FIFO or randomly picking one). And of course, a student must inform the clerk about releasing their room only after really leaving it.

> Taken from Wikipedia (https://en.wikipedia.org/wiki/Semaphore_(programming)

---

A semaphore that has value 0 indicates that no wakeups are saved, or some positive value if one or more wakeups are pending. Semaphore has two operations - `down` and `up`. `down` operation decrements the semaphore value if it is more than 0, but if it is 0 then the process goes to sleep without completing the down. `up` operation will increment the semaphore value by 1. If there were processes that were sleeping on semaphore, due to `up` operation they will wakeup and perform their pending `down` operation - resulting at no change in semaphore value. In this case, due to `up` there is no change in semaphore but one process woke up.

Decrementing the semaphore and going to sleep if value is 0 or incrementing the semaphore and waking up sleeping process - both are *single indivisible atomic* operations.

To solve the producer-consumer problem using semaphore, we use `mutex` for achieving mutual exclusion, `full` to keep track of buffer items and `empty` to keep track of empty buffer slots.

```c
#include<stdio.h>
#define TRUE 1
#define FALSE 0
#define N 10 // buffer size

typedef int semaphore;

semaphore full=0; // counts full buffer slots
semaphore empty=N; // counts empty buffer slots
semaphore mutex=1; // controls access to critical region

void producer() {
	int item;

	while(TRUE) {
		item=produce_item();
		down(&empty);
		down(&mutex);
		insert_item(item); // critical section
		up(&mutex);
		up(&full);
	}
}

void consumer() {
	int item;

	while(TRUE) {
		down(&full);
		down(&mutex);
		item=remove_item();
		up(&mutex);
		up(&empty);
		consume_item(item);
	}
}

int main() {
	
	return 0;
}
```

`mutex` is used as **binary semaphore** , but `full` and `empty` are used for synchronization. In this case, they are used to ensure that producer stops running when buffer is full, and consumer stops running when buffer is empty.

## References

1. [Operating Systems - Design and Implementation (MINIX 3)](https://csc-knu.github.io/sys-prog/books/Andrew%20S.%20Tanenbaum%20-%20Operating%20Systems.%20Design%20and%20Implementation.pdf)
2. [Operating System Concepts](https://os.ecci.ucr.ac.cr/slides/Abraham-Silberschatz-Operating-System-Concepts-10th-2018.pdf)
