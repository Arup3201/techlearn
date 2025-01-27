# Learning Notes About API

There are 2 primary models to implement API -

- RPC
- REST

RPC focuses on functions or actions, while REST focuses on resources or objects.

## RPC

RPC stands for Remote Procedure Call. In distrubuted computing, a remote procedure call will run a procedure or subroutine (written on your machine) on another address space - another machine that is connected to the same network.

Many modern programming languages - C/C++, Java, Python, Golang, PHP and Javascript has procedures or functions. They are also languages built for producing and consuming APIs.

So, it is not surprising that procedures have been one of the dormant models for builing distributed APIs in the form of [Remote Procedure Call](https://en.wikipedia.org/wiki/Remote_procedure_call) (RPC).

RPC delivers ease of programming the client-server side along with efficiency of execution.

Calling a remote procedure is syntactically same as calling a procedure in a programming language, and learning the remote procedures of an API is same as learning a new programming library.

RPC implementations also tend to be efficient — the data that is passed between the client and the server is usually encoded in binary formats, and the RPC style encourages relatively small messages (although some care has to be taken to avoid overly chatty interactions).

### How RPC Works?

RPC works between server and client. Client makes the request and server process the request and then send back the response.
RPC is a remote function call from client. Client calls the remote function with some arguments and blocks the thread from processing. In the server side, when the server gets the request it calls the service asked by the client and returns the reply after executing the request.

Following diagram shows the flow of RPC from client to server and then back to client.

[rpc diagram](./images/rpc-diagram.png)

A remote procedure is identified by three things - 

1. the program number
2. the version of the RPC
3. the procedure number

The program number identifies a group of related procedure. A program can have multiple versions and each version contains a set a related procedures. Every procedure is identified by a procedure number.

### RPC Principles

There are some principles that you have to follow while implementing RPC. They are as follows -

1. Remote Invocation: An RPC call is made by a client to a function on the remote server as if it were called locally to the client.
2. Passing Parameters: The client typically sends parameters to a server function, much the same as a local function.
3. Stubs: Function stubs exist on both the client and the server. On the client side, it makes the function call. On the server, it invokes the actual function.

> [**Stub**](<https://en.wikipedia.org/wiki/Stub_(distributed_computing)>): In distributed computing, a stub is a program that acts as a temporary replacement for a remote service or object. It allows the client application to access a service as if it were local, while hiding the details of the underlying network communication. This can simplify the development process, as the client application does not need to be aware of the complexities of distributed computing.


### RPC Application Development

Let's consider a situation where you want to access a database which is present in a remote machine. One option is to use an UNIX remote shell to access the remote machine. But you will encounter 2 problms -

- the command will be slow to execute
- you may have to login to the system to access the database

In this the RPC can help you by -

- setting up a server that responds to the client queries
- retrieves information based on the queries which is much faster

To develop RPC you need 3 things -

- the server
- the client
- the communication protocol between client and server

The implementation of the server and client can be found [here](../clanguage/rpc).

### Example of RPC

Let's build a JSON-RPC which can add two numbers. A client will call the procedure `addNumbers` with two parameters, and helps you use a procedure that is on the server, as if it is a local procedure.

Before jumping to the implementation, following libraries are needed in npm.

```
jayson
cors
body-parse
connect
```

You can install them using `npm i`.

We will create JSON-RPC server using the npm-package jayson. Here is the server implementation.

```js
const jayson = require("jayson");
const cors = require("cors");
const jsonParser = require("body-parser").json;
const connect = require("connect");

app = connect();

// JSON-RPC methods
const methods = {
  addNumbers: (args, cb) => {
    cb(null, args[0] + args[1]);
  },
};

// Create the server
const server = new jayson.Server(methods);

app.use(cors({ methods: "POST" })); // allow CORS for POST
app.use(jsonParser()); // enable the server to parse JSON data
app.use(server.middleware()); // enable CORS for jayson server

app.listen(3000, () => {
  console.log("JSON-RPC is listening at port 3000");
});
```

In the code above, `methods` contains all the functionality we would like to implement on the server.

For this example, we have a simple `addNumbers` function which takes in two arguments. First is `args`, which is an array of two numbers and the second is `cb`, which is an error-first callback function. Basically, it adds the two numbers and passes the result through a callback function.

A lot of extra code is added to avoid CORS error, enable jayson to parse JSON.

We also need client who will use this method remotely. Here we will create a simple HTML file for creating the client.

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>RPC Example - JSON-RPC</title>
  </head>
  <body>
    <h1 id="result"></h1>
  </body>
  <script>
    async function addNumbers(a, b) {
      const response = await fetch("http://localhost:3000", {
        method: "POST",

        body: JSON.stringify({
          jsonrpc: "2.0",
          params: [a, b],
          method: "addNumbers",
          id: 1,
        }),
        headers: {
          "Content-Type": "application/json",
        },
      });

      const result = await response.json();
      return result.result;
    }

    (async () => {
      const result = await addNumbers(2, 3);
      const resultElm = document.getElementById("result");
      resultElm.innerText = result;
    })();
  </script>
</html>
```

`addNumbers` function is the stub which implements the remote method calling.

The function simply fetches the API data. Only difference than usual fetch is the `body` used in the fetch.

Let's take a closer look at the `body`.

`jsonrpc` is a required property - it identifies the version of the JSON-RPC protocol that is being used. Without it, we’ll get an error from the server.

The next property `method` contains the method name of the functionality we have implemented on the server. In this case `addNumbers` which we have defined in the `methods` variable on the server.

Then we have the `params` property which accepts the arguements of the method we need to pass in for the server method to accept.

Finally, we have the `id` property which is actually an optional. The `id` property is used to correlate the response to the request.

In response you will get a response after doing `response.json()`, the response looks like this.

```
{
    "jsonrpc": "2.0",
    "id": 1,
    "result": 5
}
```

We are using a HTML element to show the result of the addition after calling the remote procedure `addNumbers`.

### Problems with RPC

But what is the problem in RPC that we opt for some other model?

There are two main problems in software development -

1. Software is hard to change
2. Systems are difficult to integrate

Technical and business assumptions pass through procedures. This is largely true of remote procedures as well as local procedures. That is why softwares remain difficult to change.

## REST

REST stands for REpresentational State Transfer.

HTTP is the only commercially important implementation of REST API. REST is a bit confusing in different places, so we can avoid confusing by bringing HTTP to understand REST.

HTTP is the complete inverse of RPC. For RPC, the addressable units are procedures and the entities are hidden behind the procedures. In HTTP, the entities are the addressable unit, and the behaviours are hidden behind the entities as side effects of creating, updating or deleting them.
