#ifndef RLS_H
#define RLS_H

#define DIR_SIZE 8196

#define READDIR_PROG ((u_long) 0x20000001)
#define READDIR_VERS ((u_long) 1)
#define READDIR_PROC ((u_long) 1)

extern char** read_dir(char*);

#endif
