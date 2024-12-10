# Networking in C

Socket Programming using C language.

## Socket Concept

`socket` needs 3 arguments to create a communication channel - `family`, `domain` and `protocol`.

A socket needs to know what protocol to use when implementing the communication style.

A socket must have a *namespace*. The socket "address" makes sense in the context of a *namespace*. Any namespace starts with `PF_` and socket address format of those namespace starts with `AF_`.

The rules of a protocol apply to the data passing between two programs, perhaps on different computers; most of these rules are handled by the operating system and you need not know about them. What you do need to know about protocols is this:

- In order to have communication between two sockets, they must specify the *same* protocol. 
- Each protocol is meaningful with particular style/namespace combinations and cannot be used with inappropriate combinations. For example, the *TCP* protocol fits only the byte stream style of communication and the Internet namespace.
- For each combination of style and namespace there is a default protocol, which you can request by specifying 0 as the protocol number. And that’s what you should normally do—use the default.


Read about the socket more in detail [here](https://www.gnu.org/software/libc/manual/html_node/Socket-Concepts.html).

## Communication Styles

GNU C library provides several different kinds of sockets, each with different characteristics.

Following are the supported socket types -
1. **SOCK_STREAM**: It operates over a communication with a particular remote socket and transmites data reliably as a stream of bytes.
2. **SOCK_DGRAM**: Each time you write to a socket using this style, one packet is created. Since *SOCK_DGRAM* does not have any connection, you need to specify the recipient address every time you are sending the packet. Some packets may not reach the destination, some may go multiple times or some may go in wrong order.

Read about the communication styles more in detail [here](https://www.gnu.org/software/libc/manual/html_node/Communication-Styles.html).


## Socket Addresses

The name of a socket is usually called the *address* of the socket.

When socket is created using the `socket` function, they do not contain any address. If you want to bind an address to the socket, you have to use `bind` function.

Usually this is needed when you are trying to create a server, and the clients need to connect with this server at the server address.

### Socket Address Formats

Depending on the namespace, the address formats will change, but `bind` and `getsockname` functions use the generic type `struct sockaddr*` to point to socket address.

So, after using the proper namespace supported address format - you need to type cast the address format to `struct sockaddr*`.

`struct sockaddr` type has 2 members - `short int sa_family` and `char sa_data[14]`.

For more details on socket address formats, read [here](https://www.gnu.org/software/libc/manual/html_node/Address-Formats.html).

### Setting the socket address using `bind`

Use the `bind` function to assign an address to a socket. The prototype for `bind` function is in the header file `sys/socket.h`.

**bind**: `int bind(int socket, struct sockaddr*, socklen_t length)`

The return value is `0` when succeeds and `-1` when fails.

For more details on socket address formats, read [here](https://www.gnu.org/software/libc/manual/html_node/Setting-Address.html).

**`address already in use` error**: While you are binding your socket to an address, that address won't be available if you do not close the socket properly. We will discuss about that in detail later, but for now to get rid of the *address already in use* error, we can do the following to re-use the same address by setting the socket options using `setsockopt`.

**setsockopt**: `int setsockopt(int socket, int level, int option_name, const void* option_name, socklen_t option_len)`

```c
int yes = 1;
if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))==-1) {
	perror("setsockopt");
	return 1;
}
```

For more detailed explanation about the close and shutdown, you may read [this](https://beej.us/guide/bgnet/html/#close-and-shutdownget-outta-my-face).

When you are trying to use `PF_LOCAL` which is the local namespace, and you are using files as the server address, then you have to delete those files after the connection has been closed.

You can do that using `unlink`.
```c
#include<unistd.h>

int make_named_socket(int argc, char* argv[]) {
	sock = make_named_socket(argv[1]);
	unlink(sock);
	return 0;
}
```

### Getting the name of the socket

I can use `getsockname` to get the address of the socket.

The definition for `getsockname` is defined as below -

`int getsockname(int socket, struct sockaddr* addr, socklen_t* length_ptr)`

It returns information about the address of the socket `socket` in the locations specified by the `addr` and `length_ptr` arguments.

## Local Namespace

