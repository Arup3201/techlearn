# Dameon in C

A daemon is a long running computer program that runs on the background without any interaction from the user. It means that any process that is a daemon will not have any controlling terminal - `tty` (In unix and unix-like systems `tty` is the command to get the file name of the terminal connected to standard input).

Because daemons do not have any controlling terminall, they can't use `stdout` or `stderr` for any output as well as `stdin` for inputs. Popular examples of daemon are - nginx, prefix, httpd, cron, sshd, inetd. Many of them end with the letter `d` to denote them as daemon. Not having any controlling terminal is an important thing for daemon otherwise it could be used for many nefarious ways. 

In unix like systems the parent process of a daemon is `init` process but it is not always the case. `init` is the first process run by the kernel when it first starts. It has the process id 1. Every process running as a daemon is directly or indirectly running by the `init` process.

A daemon could be created by forking the parent process and creating the child process - then suddenly exiting the parent process. This will make `init` to adopt the child process. Otherwise `init` process could also directly launch the daemon.

A daemon that was created by "double-forking" (method of destroying the parent process and letting the `init` process take the control of the child process) the daemon must also dissociate the process the terminal (`tty`).

Usually outputs from the daemon are written as logs to the `/var/log` directory. And if it needs any input then it uses socket or signal.

## Some Background Theory

Before moving forward with the daemon we need to understand the concept of sessions, leaders and processes. In Unix every process has some id associated with it. They are `pid`. Then comes the parent process that called this proces - which also has an id called `ppid`. Then we have process groups that contains multiple processes. Every process group has some leader termed as process group leader. The `pid` of the process which is the process group leader is same as the `pgid` (process group id). 

There could be multiple process groups and every process group has it's own leader process. These process groups are part of a single session. Every session has it's own id called `sid`. There is a process which is the leader of this session. This process will have the same `pid` as the session id. This process will obviously be the process group leader as well.

The process group could run in the foreground or in the background. A session has a controlling terminal for it. The process group which is running on the foreground can access this terminal. The background process groups are prohibited from interacting with the terminal (no access to the `stdin`, `stdout`, and `stderr`).

Following are the attributes of process, process group and session.

**Process**:

- Process ID - `pid`
- Parent process ID - `ppid`
- Process Group ID - `pgid`
- Session ID - `sid`

**Process Group**:

- Process group ID
- Process group leader with `pgid = pid`
- Foreground or background process group

**Session**:

- Session ID
- Session leader with `sid = pid`

## Ways to create daemon

There are many ways to create daemon but here we gonna talk about the following 2 ways -

1. Programmatically creating daemon
2. Using `systemd` service

In the first method - we can use `unistd` to call the `daemon function that will create a traditional style BSD daemon. Another method uses "double fork" method.

## Creating a daemon with unistd

For creating the daemon we can include the `unistd.h` header which contains the `daemon` function. This function takes two arguments - `nochdir` and `noclose`.

If `nochdir` is 0 then it will change the current directory to '/' otherwise it will keep the current directory as it is. `noclose` will redirect the input, output and error standards to `/dev/null` if it is zero.

The following code will create a daemon that will stay in the memory for 60 secs.

```c
#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
	// directory - '/' which means no child directory
	int nochild = 0;
	
	// no input, output and error - means the file descriptor is closed
	int nofd = 0;

	if(daemon(nochild, nofd)) {
		perror("daemon");
		return 1;
	}

	// this daemon process stays for 60 secs
	sleep(60);

	return 0;
}
```

Now we have the code to create the daemon, let's create a daemon and see how it works on a process level.

First create the executable file from the `become_daemon.c` file using `gcc become_daemon.c -o become_daemon.out`. Next we will run the program `./become_daemon.out` which will create the daemon process.

We can look for this process using `pgrep` command which will show you the process ID of the daemon process.

```sh
> pgrep become_daemon

47844
```

We can use this process ID to look for more information using `ps` command.

```sh
> ps -p 2182 -o "user pid ppid sid tty command"

USER         PID    PPID     SID TT       COMMAND
arup       47844    2182   47844 ?        ./become_daemon.out
```

The command also helps you customize the things that you want to see - e.g. user, process id, parent process id, session id, terminal, command etc.

As a daemon we know that it will directly or indirectly be connected to `init` the process with id 1 or the first process that kernel starts. Here you can see the parent process id of the daemon id 2182. And if we take a look at it's parent then you will see -

```sh
> ps -p 2182 -o "user pid ppid sid tty command"

USER         PID    PPID     SID TT       COMMAND
arup        2182       1    2182 ?        /lib/systemd/systemd --user
```

Indeed the parent process is actually `systemd` which is running directly under `init` (process ID 1). 

We can further look at the daemon regarding which I/O it is using and some more things using `lsof` -

```sh
> lsof 47844

COMMAND     PID USER   FD   TYPE DEVICE SIZE/OFF    NODE NAME
become_da 47844 arup  cwd    DIR    8,3     4096       2 /
become_da 47844 arup  rtd    DIR    8,3     4096       2 /
become_da 47844 arup  txt    REG    8,3    16048  665464 /home/arup/learnings/clanguage/daemon/become_daemon.out
become_da 47844 arup  mem    REG    8,3  2220400 1050756 /usr/lib/x86_64-linux-gnu/libc.so.6
become_da 47844 arup  mem    REG    8,3   240936 1050752 /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
become_da 47844 arup    0u   CHR    1,3      0t0       5 /dev/null
become_da 47844 arup    1u   CHR    1,3      0t0       5 /dev/null
become_da 47844 arup    2u   CHR    1,3      0t0       5 /dev/null
```

The last 3 are using `/dev/null` as their stream which means they are closed.

## Ananysis of the daemon

1. Daemon is connected to `init` through `systemd`. Usually it will be directly under `init` but it depends on the way we implement.
2. It will not have any terminal associated with it (no `tty`).
3. It will not have any input or output stream as the `stdin`, `stdout` and `stderr` are closed.
4. The only files associated with it are some shared files, sockets and log files.

## Creating a daemon using "double fork"

This is a technique that is used to create traditional daemons. If you want to build modern daemons then along with this step there are some additional steps that are mentioned in the `man 7 daemon` page.

**NOTE**: `fork` call creates child processes. In case of failure it returns negative value otherwise it returns the id the child process. It returns 0 to notify it has returned to child process.

**NOTE**: `setsid` call creates a new session if the calling process is not a process group leader. The calling process is the leader of the new session.

The overall idea of "double fork" is using `fork` then `setsid` and finally a last `fork` to get a process that is running directly under `init` and has no controlling terminal `tty`.

The steps are as follows -

1. Call `fork` so that the process can run in the background.
2. Call `setsid` so that we move away from the shell's session to an independent session. 
3. Call `fork` again so that the process is not a process group leader in the new session. This will ensure the process does not get access to any terminal (`tty`).

In the first step after we `fork` and get the child process - we exit from the parent process. This makes the child process an orphan process. Orphan processes run under `init` with process ID 1. Aftere the last `fork` the child process creates a second child process, the first child process exits and the remaining child process will not be the process group leader. This final process will never have any controlling terminal. So, we got a daemon process now.

## Code to create a daemon with double fork

```c
#include <fcntl.h> // open, close
#include<unistd.h> // fork, setsid, _exit, 
#include<stdlib.h> // EXIT_SUCCESS
#include<sys/stat.h> // umask
#include "become_daemon.h"

int become_daemon(int flags) {
	switch(fork()) {
		case -1: return -1; // fork failed so return
		case 0: break; // it is the child process so process to next code block
		default: _exit(EXIT_SUCCESS); // it is the parent
	}

	if(setsid() == -1) { // open a new session and make the current child process it's process group leader
		return -1;
	}

	switch(fork()) {
		case -1: return -1; // fork failure
		case 0: break; // child process 
		default: _exit(EXIT_SUCCESS); // exit the first child process
	}

	// after the parent process of the second child process is exited. the current process is now an orphan process.
	// init will become the parent process of this orphan process
	
	// according to the flag we will
	// change the directory to /
	// clear file creation mode mask
	// close all open files
	// set stdin, stdout and stderr to /dev/null
	
	if(!(flags & BD_NO_MASK)) umask(0);

	if(!(flags & BD_NO_CHDIR)) chdir("/");

	if(!(flags & BD_NO_OPEN_FILE)) {
		int maxfd;
		maxfd = sysconf(_SC_OPEN_MAX);

		if(maxfd == -1) {
			maxfd = BD_MAX_CLOSE;
		}

		int fd = maxfd;
		while(fd >= 0) {
			close(fd);
			fd --;
		}
	}

	if(!(flags & BD_NO_STD)) {
		close(STDIN_FILENO);

		int fd = open("/dev/null", O_RDWR);
		if(fd != STDIN_FILENO) {
			return -1;
		}
		if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDIN_FILENO) {
			return -2;
		}
		if(dup2(STDIN_FILENO, STDERR_FILENO) != STDIN_FILENO) {
			return -3;
		}
	}

	return 0;
}
```

The code does the following -

1. `fork` the current process to create a child process with the current process as it's parent.
2. In the parent process (`fork()` returns some +ve value) we exit successfully.
3. For the child process (`fork()` returns 0) we proceed.
4. Child process is directly under the shell process, so we move to a new sessio using `setsid`.
5. In the child process inside new session, we create another child process and then exit the current child process. This makes the child process an orphan process without any terminal.
6. Now this orphan process is under the `init` process and becomes a daemon.
7. Next steps are for setting the file mask mode to 0. Setting the current directory to root. Then closing all the opened file descriptors and finally setting the `stdin`, `stdout` and `stderr` to `/dev/null`.


## Additional files

There are some additional code that you need to run the following code. We have the `become_daemon` function but we also need an header file `become_daemon.h` to include in other `.c` files where you want to create a daemon.

```c
#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H

#define BD_NO_MASK 01
#define BD_NO_CHDIR 02
#define BD_NO_OPEN_FILE 03
#define BD_NO_STD 04
#define BD_MAX_CLOSE 8192

int become_daemon(int);

#endif // !BECOME_DAEMON_H
```

And the `main.c` file which will use this `become_daemon` function to create the daemon.

```c
#include<stdlib.h>
#include<syslog.h>
#include<unistd.h>

#include "become_daemon.h"

int main(int argc, char* argv[]) {
	char* LOGNAME = "BECOME_DAEMON";

	int ret = become_daemon(1);
	if(ret) {
		syslog(LOG_USER | LOG_ERR, "become_daemon call failed!");
		closelog();
		return EXIT_FAILURE;
	}

	openlog(LOGNAME, LOG_PID, LOG_USER);
	syslog(LOG_USER | LOG_INFO, "starting");

	while(1) {
		sleep(60);
		syslog(LOG_USER | LOG_INFO, "running");
	}

	return 0;
}
```

Remember that daemons do not interact with any terminal, so they do not have access to input and output streams. That is the reason we are using `syslog` to create logs and show outputs in the form of a log. The outputs will availabel in the `/var/log/syslog` file.

Now we have got our daemon. Let's start the daemon now!

```sh
gcc main.c become_daemon.c -o main.out
./main.out

ps xao # shows all the processes that do not have any tty
ps 7735 # check your daemon process ID, you will see that it is directly running under init
```

If you do not see the entry of your daemon in the list of processes then most probably it failed. You can verify it by opeing `/var/log/syslog` file and searching for `main.out`. If the message says `become_daemon call failed!` then there is some error.


