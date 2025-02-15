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

