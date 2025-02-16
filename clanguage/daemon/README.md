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
