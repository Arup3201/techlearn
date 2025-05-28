# Fast API

First let's create a virtual environment to start working with fastapi.

```sh
python3 -m venv .virtual
```

After the virtual environment is created, start the virtual environment using 

```sh
source .virtual/bin/activate
```

Install `fastapi`using pip.

```sh
pip install "fastapi[standard]"
```

**VERSIONS**
> fastapi 0.115.12

## Example with fastapi

Let's create `main.py` to use fastapi.

```py
from fastapi import FastAPI
from typing import Union

app = FastAPI()

@app.get('/')
def read_root():
    return {'Hello': 'World'}

@app.get('/items/{item_id}')
def read_item(item_id: int, query: Union[str, None] = None):
    return {'item_id': item_id, 'query': query}
```

Run the server using 

```sh
fastapi dev main.py
```

You will get the response `{"Hello": "World"}`, and if you go to  `localhost:8000/items/abc?query=somequery` you will receive the `{"item_id": "abc", "query": "somequery"}`.

## Receive a body from a `PUT` request

To receive a body from a `PUT` request, we need `pydantic` to define the model, and then receive the `Item` type value from the request body.

```py
@app.put('/items/{item_id}')
def update_item(item_id: int, item: Item):
    return {'name': item.name, 'item': item_id}
```

## Recap Example

### Step 1: Import FastAPI

```py
from fastapi import FastAPI
```

`FastAPI` is a Python class that provides all the functionality for your API.

### Step 2: create FastAPI "instance"

```py
app = FastAPI()
```

Here the app variable will be an "instance" of the class FastAPI. This will be the main point of interaction to create all your API.

### Step 3: create *path operation*

***Path***

"Path" here refers to the last part of the URL starting from the first /.

So, in a URL like:

```sh
https://example.com/items/foo
```

...the path would be:

```sh
/items/foo
```

While building an API, the "path" is the main way to separate "concerns" and "resources".

**Operation**

"Operation" here refers to one of the HTTP "methods".

One of:
- `POST`
- `GET`
- `PUT`
- `DELETE`

...and the more exotic ones:
- `OPTIONS`
- `HEAD`
- `PATCH`
- `TRACE`

In the HTTP protocol, you can communicate to each path using one (or more) of these "methods".

**Define a path operation decorator**

```py
@app.get("/")
```

The `@app.get("/")` tells FastAPI that the function right below is in charge of handling requests that go to:

- the path `/`
- using a `get` operation

### Step 4: define the path operation function

This is our "path operation function":

```py
def read_root():
```

- path: is /.
- operation: is get.
- function: is the function below the "decorator" (below `@app.get("/")`).

This is a Python function.

It will be called by FastAPI whenever it receives a request to the URL "/" using a `GET` operation.

### Step 5: return content

```py
    return {"message": "Hello World"}
```

You can return a `dict`, `list`, singular values as `str`, `int`, etc.

You can also return Pydantic models (you'll see more about that later).

There are many other objects and models that will be automatically converted to JSON (including ORMs, etc). Try using your favorite ones, it's highly probable that they are already supported.

## Docs

To see the docs, you can go to `localhost:8000/docs`. Alternatively you can also go to `localhost:8000/redocs`.

In both the cases, you can try out the APIs and check their responses.

## OpenAPI

FastAPI generates a "schema" with all your API using the `OpenAPI` standard for defining APIs.