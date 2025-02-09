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

Lets talk about the two mostly used routines that are used for simplified interface applications - `rpc_call` and `rpc_reg`.

`rpc_call` takes 9 arguments to call the remote procedure from client side. The 9 arguments are -
1. *host*: name of the server host - type `char*`
2. *prognum*: program number of the RPC program - type `u_long`. If you write any `.x` file - it will be the program number you assign to a `program`.
3. *vernum*: version of the RPC program - type `u_long`. 
4. *procnum*: procedure number of the RPC procedure - type `u_long`.
5. *inproc*: xdr filter to encode inputs args to the remote procedure - type `xdrproc_t`.
6. *in*: pointer to arguments for the remote procedure - type `char*`.
7. *outproc*: xdr filter to decode remote procedure result - type `xdrproc_t`.
8. *out*: address to store result of the remote procedure - type `char*`.
9. *nettype*: mention the transport type using the last argument - type `char*`.

`rpc_call` will call the remote procedure specified by the `prognum`, `version` and `procnum` on the host. When it calls the remote procedure, it passes the arguments in a address `in` and also provides a XDR filter `inproc` to encode this argument when the remote procedure receives it. It stores the result of the remote procedure on the address `out` which is filtered using the XDR filter `ourproc`.

When `rpc_call` is executed, it blocks the flow until it receives the response from the server. The server may return in success or failure when it replies. In case of success the server returns 0 which is defined as `RPC_SUCCESS` and in case of failure it returns non-zero value which can be castes to an enumerated type `clnt_stat`. This error function provided by rpc prints an error message showing why an RPC failed.

Example program is [here](./simplified-interface/clnt.c).

Since RPC uses data types - some may not be supported by all machines. So we use XDR filters that helps the client and server communicate using data types that are supported on each machine. That is why before remote procedure gets the input arguments - it is encoded using the XDR filter. Same is true for the result coming from the remote procedure. In the example, the output is filtered using the type `xdr_u_long` which is used for unsigned long type data. As there is no input to the remote procedure - xdr filter void for that.

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

