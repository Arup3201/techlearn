# Generating rpc protocols using `rpcgen`

`rpcgen` provides a way to write distributes applications. Server is written in any language that observes procedure-calling conventions. Then the server program is linked to the server stub generated by `rpcgen` to create server executable. Same happens with the client program.


We will try to create remote procedure like the `printmsg` function. Look at the implementation at [printmsg.c](./printmsg.c).

RPC has it's own language which has extention of `.x`. In RPCL language, every program has a number associated with it, inside every program different versions are implemented and inside every version we have the procedure with their unque identifier.

Here we will try to define the `printmsg` as a remote procedure in `msg.x`. Let's determine the arguments and the return type of `printmsg` procedure.

The argument is a string and the return type is an integer.

With this we can write the remote version of `printmsg` -

```x
program PRINTMSGPROG {
    version PRINTMSGVERS {
        int PRINTMSG(string) = 1;
    } = 1;
} = 0x20000001;
```

The remote procedures are defined as part of remote programs. The code declares a remote program that contains only one remote procedure `PRINTMSG`.

New versions of the same program can be implemented by increasing the version number and implementing seperate functionalities. One version can contain multiple procedures.

**NOTE** that here we are using `string` which is a null terminated array of characters - as char* is ambigious whether it is pointing to a character or an array of characters.

If we compile the `msg.x` with `rpcgen` -

```sh
rpcgen msg.x
```

It will generate `msg.h`, `msg_clnt.c` and `msg_svc.c`.

First we have to implement the remote version of the procedure in C which will be called when the client makes a request to server.

The implementation of the remote procedure is [msg_proc.c](./msg_proc.c).

Now we can implement the remote version of the procedure `printmsg`. The full implementation is [rprintmsg.c](./)

**NOTE** I could not run the code in ubuntu as it is showing me error related to libraries like rpc not found. So, I installed `libtirpc-dev` using `sudo apt install libtirpc-dev` but it still was missing some functions like `clnt_create`, `clnt_pcreateerror` etc. After searching for the problem I got to know that `ntripc` folder has `rpc` inside it so I had to manually link all the necessary libraries from the `/urs/include/ntirpc` to `/usr/include` folder using the command `sudo ln -s /usr/include/ntirpc/[header path] /usr/include/[rpc or nothing]`. It still did not work, so I am now following this youtube video (RPC programming in C in ubuntu OS)[https://www.youtube.com/watch?v=HbBxO5RXNhU].

**NOTE** I finally found the solution to my problem. I had to install `libtirpc` instead. It contains `tirpc` and contains all the necessary functions that I am using here.

To install `libtirpc` you can clone the repo `git@github.com:alisw/libtirpc.git`. After cloning, you can start creating config by `sh autogen.sh`. Before you run this make sure you have `libtool`, `libkrb5-dev`, `python-dev-is-python3` and `gssapi`.

After you create the `configure` file, run it `./configure` and then you can make the install file by `make` and finally install using `sudo make install`.

While generating the executable files for client and server, make sure to include libraries `-lnsl` and `-ltirpc` otherwise it will throw error saying some functions are undefined.

## Remote Procedure in C

The remote procedure implementation of the `printmsg` function will differ in some ways for RPC. Let's first look at the function definition -

```c
int* printmsg_1_svc(char**, struct svc_req*);
```

First of all the function is coming from `msg.h` which defines the server side remote procedure as `printmsg_1_svc` which comes from the procedure name and version defined in the `msg.h`. `rpcgen` generated remote procedure names follow the same rule. The procedure name is converted to all lowercase and then appended with the version using `_`, for server call `svc` is added at the end.

Next, you can see that the function takes 2 arguments. One is a pointer to a string and the other is a pointer to `struct svc_req` type which has information about the context invocation: the program, version and procedure numbers, raw and canonical credentials along with an `SVCXPRT` structure pointer which contains information about the transport.

```c
static int result; // must be static
```

Also the procedure returns a pointer to the result instead of a direct result. It is important that the result is a static value otherwise when the remote procedure call ends, the references to the result will be invalid.

Remote procedures are always called with one argument if `-N` option is not provided - if more than one argument is needed then it need to be in a struct and we need to pass the pointer to that struct.

Look at the code [here](./rpcgen-manual/msg_proc.c).

## Remote Procedure Calling From Client

In the implementation of the `rprintmsg.c` where we actually write the code to call the remote procedure from client. In this function there are few things to note.

```c
clnt = (CLIENT*)clnt_create(server, PRINTMSGPROG, PRINTMSGVERS, "visible");
```

`clnt_create` function is used to create the client handle that you can pass to any stub routine that calls the remote procedure. It takes three parameters - the host where the remote procedure is, program number and version number of the remote program, and finally the protocol of the transport layer the client will use to call the remote procedure. `visible` value for the protocol indicates that any transport noted as visible by `/etc/netconfig` can be used.

```c
clnt_pcreateerror(server);
```

If `clnt_create` fails to create the client handle then it returns null. Then `clnt_pcreateerror` will print the error message along with the host name pointed to by `server`.

We already saw from the server side remote procedure implementation that the remote procedure is going to return a pointer to the result. But, there can be two main ways the remote call can go wrong.

```c
if(result == (int*)NULL) {
    /* failed to call the procedure
     * print error and die
     */
    clnt_perror(clnt, server);
    return 1;
}
```

One is when it returns null, which means the remote call failed - in this case we print the message to the standard error indicating why remote procedure failed.

Another way depends on the implementation of the remote procedure and which output the user labels as error.

In this case if the output is 0 then we show error otherwise the remote procedure call was a success.

You also have to make sure that you include `libnsl` library which contains the networking functions for RPC and XDR.

There are no files related to XDR as we are using data types which are already implemented in the `libnsl`. If we used any user-defined data types then the XDR would have been created.

Look at the code [here](./rpcgen-manual/rprintmsg.c).

## Other files created by rpcgen

Other files created by `rpcgen` are -

- `msg.h`
- `msg_clnt.c`
- `msg_svc.c`

`msg.h` header file defines `PRINTMSGPROG`, `PRINTMSGVERS` and `PRINTMSG` which are used in other modules. This header file **must** be included in the client and server.

`rpcgen` also created client stub file which contains only one routine `printmsg_1` from `rprintmsg.c`. If the file for RPCL is `proc.x` then the client stub file will be `proc_clnt.c`.

Similarly it also created the server stub file which calls the `printmsg_1_svc` from `msg_proc.c`.

### Series of commands to execute

```sh
gcc -c msg_proc.c msg_svc.c
gcc -o server msg_proc.o msg_svc.o -lnsl -ltirpc

gcc -c rprintmsg.c msg_clnt.c
gcc -o client rprintmsg.o msg_clnt.o -lnsl -ltirpc
```

## Passing Complex Data Structures

It also possible for you to create your own data type and not just be restricted to the data types declared in the `libnsl`. You can implement your own data type in the `.x` file. `rpcgen` will generate XDR routines along with the stib files. This routine is used to convert the local data type in host platform to XDR format and vice versa.

To demonstrate the use of data structures in RPC - we will look at the directory listing service where user can remotely list the directory files and sub-folders inside it.

For that we will create the `dir.x` file where we will create the data structures. There are mainly 4 types we are working with -

1. A `nametype` which holds the name of the files and folders. It is a `string` with maximum length of 255. Just like C, we can change the name of the type using `typedef` with the following syntax -
```c
const MAXDIRLEN = 255;
typedef string nametype<MAXDIRLEN>;
```
2. We have another type that is `namenode` which has information about the directory and link to the next sub-directory. `namenode` is a structure defined using `struct` just like in C -
```c
struct namenode {
	nametype name, 
	namelist next;
};
```

Then the `struct namenode` is `typedef`ed into just `namenode`. Also we have another type - that is pointer to the `namenode` type called `namelist`.

3. Finally we have another type which is created using `union` in RPCL. Here the union looks different than how it is in C.
```c
union readdir_res switch(int error) {
    case 0:
        return namelist list;
    default:
        void;
}
```
This syntax is going to create a structure in C which looks like the following -
```c
struct readdir_res {
    int error;
    union {
        namelist list;
    } readdir_res_u;
};
typedef struct readdir_res readdir_res;
```
Notice that the union component has same name as the type with additional `_u` appended at the end.

Look at the full code [here](./rpcgen-xdr/dir.x).

And finally we define the program in RPC where the procedure takes the directory name of type `nametype` and returns type `readdir_res`.

Now we can use `rpcgen` to produce the following files -
1. `msg.h` header file that declares the program, version and procedure.
2. `msg_clnt.c` and `msg_svc.c` which are stub and skeleton files for client and server.
3. `msg_xdr.c` which defines the XDR routine for the complex data structure.

If there is any data type used in the `.x` file then `rpcgen` tries to find it in the `libnsl` where the xdr routines are defined with `xdr_` prefix (e.g. `xdr_int`).

If there is a data type defined in the `.x` file then `rpcgen` generates routines for that data type in `xdr_*.c` file.

You can also omit the data type definition - a data type that is not supported by `libnsl`. Then you can customize your own xdr_ routine.

### Remote Procedure Implementation

In the implementation of the remote procedure, we are traversing the directory mentioned by the client. The directory name is of the type `nametype`.

First we create a directory stream which is an object to iterate over the directory. To create a directory stream we need `opendir` from `dirnet.h`.

The directory stream object is a structure called `DIR`. Also to store any directory information we have `struct dirnet`.

If the `opendir` does not fail, we proceed with traversing the directory. But before that we make sure that we clean the previous result if the remote procedure has already been called. We use `xdr_free` to free the memory allocated to the XDR data type.

Then we initaliz a pointer `nlp` which points to the `res.readdir_res_u.list` at first. Next we iterate over every directory inside the main directory untill it is null.

In every iteration, we get the sub-directory object, create a `namenode` using `malloc`, check for error and finally set the `nl->name` from `d->d_name`, and set the value `nlp` to `&(nl->next)` so that we can link the next directory to the current one.

Look at the full code [here](./rpcgen-xdr/readdir_proc.c).

In the client implementation where we call the remote procedure, we traverse the directory in a loop like this -

```c
for(nl=result->readdir_res_u.list; nl != NULL; nl=nl->next) {
    printf("%s\n", nl->name);
}
```

Look at the client implementation [here](./rpcgen-xdr/rls.c).

### Compile and output executables

Follow the commands to get the executables for server and client -

```sh
gcc -c dir_xdr.c

gcc -c readdir_proc.c dir_svc.c -include /usr/include/errno.h
gcc -o server readdir_dir.o dir_svc.o dir_xdr.o -lnsl -ltirpc

gcc -c rls.c dir_clnt.c -include /usr/include/errno.h
gcc -o rls.o dir_clnt.o dir_xdr.o -lnsl -ltirpc

sudo ./server
sudo ./client localhost /usr/share/lib
```

NOTE: Make sure to include the `errno.h` as `errno` is used in the code. Also to avoid conflicting names we will use `error` in the structure `readdir_res` instead.


