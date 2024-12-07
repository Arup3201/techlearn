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

