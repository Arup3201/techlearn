#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H

#define BD_NO_MASK 01
#define BD_NO_CHDIR 02
#define BD_NO_OPEN_FILE 03
#define BD_NO_STD 04
#define BD_MAX_CLOSE 8192

int become_daemon(int);

#endif // !BECOME_DAEMON_H
