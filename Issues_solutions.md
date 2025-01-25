# Issues and Solutions

This is file where I document all the issues I have faced when working with different tech stacks till now.

## Python

### Issue 1: ModuleNotFound Error

I had some folders, all folders have their own _\_\_init\_\_.py_ file.

Project structure-

```
- Enegma
    - admin
    - base
    - demov2
    - procurement
    - user
        - persistence
        - route
            -- __init__.py
            -- user_route.py
        - service
            - __init__.py
            - user_service.py
        - test
        - main.py
    - report
        - ...
        - ...
    - workflow
        - ...
        - ...
```

Code inside `main.py`-

```python
from flask import Flask
from user.route.user_route import user_app

app = Flask(__name__)
app.register_blueprint(user_app, url_prefix="/user")

if __name__=='__main__':
    app.run(debug=True, port=6000)
```

**Error**

```
Traceback (most recent call last):
  File "F:\Work\Enegma\user\main.py", line 2, in <module>
    from user.route.user_route import user_app
ModuleNotFoundError: No module named 'user'
```

**Solution**

Don't use `user` instead directly use `route` to import the necessary variables or classes.

`route` is a package and you can treat it as a package outside of the folder.

If you are inside the directory and want to accesss any module then use `.module_name` to import.

_route/user_route.py_

```python
from service.user_service import UserService
```

_service/user_service.py_

```python
from persistence.session import UserSession
from persistence.static_models import User
```

_persistence/session.py_

```python
from .static_models import static_base
```

Don't run a module which is inside a package and it imports the module next to it.

```
- test
    - test_base.py
    - test_user_services.py
```

_test_user_services.py_

```python
from .test_base import TestBase
```

Then, doing the following will give you the error `Relative import without parent package` error.

```
python test/test_user_services.py
```

If you want to do so, you have run the following command instead-

```
python -m user.test.test_something
```

## Backend Development

### Issue 1: Postman `POST` error

More than an issue it is a very importannt and basic checkup you should do before checking your routes in `Flask` or `Django`.

Make sure you are sending a `POST` request when expected and `GET` request when expected.

Always check what kind of request you are making in `Postman`.

### Issue 2: jwt.exceptions.DecodeError: Invalid header padding

Make sure that the JWT token does not contain anything except the token code generated.

If you are getting the token in header from client side, and the header has `Authorization` value set to `Bearer "TOKEN_KEY"`, make sure to split the string and get the `TOKEN_KEY` WITHOUT the DOUBLE QUOTES.

### Issue 3: Token generation in the cookie and not getting the token in the application tab

_main.py_

```python
CORS(app, support_credentials=True)
```

_service/user_service.py_

```python
def login():
    # ...

    response = make_response(jsonify({...}), 200)
    response.set_cookie('token', 'GENERATED_TOKEN', samesite='None', secure=True, max_age=15) # 15 secs
    # ...

    return response
```

How the request should be like?

```js
fetch(url,
    {
        method: 'GET'/'POST'/...,
        credentials: 'include', // !!!IMPORTANT
        headers: {
            ...
        }
    }
)
```

### Issue 4: IO Socket CORS error

Change the socket instance initialization with `cors_allowed_origins="*"`.

```python
CORS(app)
socket = SocketIO(app, cors_allowed_origins="*")
```

## Frontend Development

### Issues 1: 504 (Outdated Optimize Dep) while using react-vite

Change the `vite.config.js` file -

```js
import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  optimizeDeps: {
    exclude: ["socket.io-client"],
  },
});
```

### Issue 2: CORS error because host is localhost and not 127.0.0.1

Change the `vite.config.js` file and make the host object point to 127.0.0.1

```js
export default defineConfig({
  server: {
    host: "127.0.0.1", // Or your desired host
  },
  //...
});
```

### Issue 3: Whenever there is an error in fetching, all states in the react application lose values

Try using `catch` block to catch the rejected promise instance otherwise it will go to the root application where all states are reset.

By using `catch` block it will prevent the flow of application to go back to the root to find any possible catch statement there. Which in turn will resolve this issue.

## Streamlit Deployment

### Issue 1: Dependency issues

Remove the following dependencies before deploying on streamlit:

```
twisted-iocpsupport==1.0.4
tensorflow-intel==2.15.0
pywin32==306
pywinpty==2.0.12
```

Replace

```
opencv-python
```

with

```
opencv-python-headless
```

If you are using nltk then make sure to download the 'popular' or any similar vocabulary by mentioning it inside deploy or any .py file used by deploy.

## VS Code

### Testing is not detected

Sometimes you will notice that your test cases are not being detected. In those cases, you first have to run the test manually in the powershell and after they run successfully, then only VS Code will detect those test files.

## Postgres

### SQLAlchemy create database case sensitive

```py
from sqlalchemy_utils import database_exists
engine = create_engine(f'postgresql://{env_config.PG_DB_USER}:{env_config.PG_DB_PWD}@{env_config.PG_DB_HOST}/{"Enegma"}')
if not database_exists(engine.url):
    create_database(db_name="Enegma")
```

Double quotes are for names of tables or fields. Sometimes You can omit them. The single quotes are for string constants. This is the SQL standard.

So when creating database make sure to use double quotes for the name in sqlalchemy:

```py
from sqlalchemy import create_engine, text
from base import env_config

def create_database(db_name):
    engine = create_engine(f'postgresql://{env_config.PG_DB_USER}:{env_config.PG_DB_PWD}@{env_config.PG_DB_HOST}/{env_config.BASE_DB}')

    # postgresql is case insensitive when used without quotes
    # To make sure we are saving the database with name mentioned in db_name parameter
    with engine.connect() as connection:
        connection.execute(text(f'CREATE DATABASE "{db_name}"'))
```

### SQLAlchemy `Table Already Exists`

First, cross checking that the `declarative_base` is used more than 1 time to create that table.

If not solved, put a logger for SQLAlchemy queries that are running -

```py
engine = create_engine(f'postgresql://{db_user}:{db_pwd}@{db_host}/{db_name}', echo=True)

logging.getLogger('sqlalchemy.engine').setLevel(logging.INFO)
```

If can't find any query that creates the table, then...

Check the model class itself.

Make sure every column has the type expected according to the data going into it. `str` type data goes to `VARCHAR`, `dict` type data goes to `JSON`, `date` type goes to `Date` and `datetime` type goes to `TIMESTAMP` etc.
