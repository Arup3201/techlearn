# RPC Implementation in C

To implement RPC in C, we need three parts -

- server
- client
- protocol bw server and client

## Defining the Protocol

The easiest way to implement the protocol is by using `rpcgen` which is described [here](../rpc-programming).

Further explanations are given [here](https://users.cs.cf.ac.uk/Dave.Marshall/C/node34.html).

To make the protocol for server and client using `rpcgen` - you must identify the name of the procedure, arguments and their types, and the return type of the procedure.

`rpcgen` generates 4 files from one file (`rpcprog.x`) written in RPC language -

- `rpcprog_clnt.c` - client program
- `rpcprog_svc.c` - server program
- `rpcprog_xdr.c` - XDR filters which are external data types
- `rpcprog.h` - header file for the XDR filters

As client and server need to be the same machine, we need data types which are machine independent. For that we are using external data representation or XDR which is a data abstraction.

## Compiling and running the application 

As we are using `rpcgen` to build the protocol, the flow for building the server and client will look like the following -

1. Create a RPC file `rpcprog.x`.
2. Create the client and server programs `rpcprog.c` and `rpcsvc.c` respectively.
3. Generate the stubs and filters from `rpcprog.c` - stubs(`rpcprog_clnt.c`, `rpcprog_svc.c`) and filters(`rpcprog_xdr.c`, `rpcprog.h`). To comile a RPCL program - `rpcgen rpcprog.x`.
4.  Add `rpcprog.h` to both client and server programs.
5. Compile client `cc -c rpcprog.c` which generates `rpcprog.o`.
6. Compile client stub `cc -c rpcprog_clnt.c` which generates `rpcprog_clnt.o`.
7. Build the client executable `cc -o rpcprog rpcprog.o rpcprog_clnt.o rpcprog_xdr.o`.
8. Compile server `cc -c rpcsvc.c` which generates `rpcsvc.o`.
9. Compile server stub `cc -c rpcprog_svc.c` which will generate `rpcprog_svc.o`.
10. Build server executable `cc -o rpcsvc rpcsvc.o rpcprog_svc.o rpcprog_xdr.o`.

Finally you can just run the programs `rpcprog` and `rpcsvc` in the client and server respectively.

## Different Levels of Interface Routines

To interact with client and server, RPC provides multuple levels of application interfaces. The interfaces vary depending on the level of the control and complexity. Bases on this, the interfaces are divided into 2 categories - simplified and standard interface. 

### Simplified Interface

The simplified interface contains functions that most applications use. They are used to call remote procedure and specify the type of transport to use.

This contains functions like -

- `rpc_reg()`: registers the procedure as RPC program on all transports of the specified type
- `rpc_call()`: calls the remote procedure present at remote address from client
- `rpc_broadcast()`: broadcast a call message across all transports of specified type

### Standard Interface

The standard interface gives developer better control over how the server and client interact via RPC. It is divided into top, intermediate, expert and bottom.

#### Top Level Interface

Top level interface is still simple but here you have to create client handle before calling remote procedure or create server handle before receiving requests.

If you want your application to run on all transports then use this interface.

Following routines are present in this interface -

- `clnt_create()`: creates a generic client handle for all transports of a specified type, it also tells the client where the server is
- `clnt_create_timed()`: creates a client handle just like `clnt_create()` but also specify how much maximum time client has before it time outs the creation attempt for all types of transports.
- `svc_create()`: creates a server handle and specifies which procedure to dispatch when client makes a call to the server
- `clnt_call()`: calls the remote procedure to send request to server



