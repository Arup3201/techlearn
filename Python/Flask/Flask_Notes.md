# Flask Documentation

Flask is a Python Library to develop backend applications and API. It is very versatile, fast and broadly used for small applications.

I am listing down all the things I learnt about Flask in this documentation.
Following are the topics covered-

1. First application using Flask `001-First-Flask-App`
2. Registration using Flask and HTML `002-Flask-Register`
3. Database connection `003-Flask-Database`
4. Login Authentication `004-Flask-Authentication`
5. Login and Register using SQLAlchemy
6. Blueprints

## First application using Flask

Steps:

1. First I import the `Flask` class. The instance of this class will work as the WSGI application.
2. Next I create an instance of this class. First argument of this class is the name of the application/package. This is needed so that Flask knows where to look for resources such as templates and static files.
3. I use `route` decorator to tell flask what URL should be trigger the function.
4. Lastly the function returns the message to the web browser. The default format is HTML.

_hello.py_

```python
from flask import Flask

app = Flask(__name__)

@app.route("/")
def hello():
    return "<h1>Flask Application</h1>"
```

To run the flask application, I need to write `flask --app hello run`.

To run the application in debug mode, you can use `--debug` as an extra parameter `flask --app hello run --debug`.

Now head over to `http://127.0.0.1:5000/`, and you should see your hello world greeting.

> If another program is already using port `5000`, you’ll see `OSError: [Errno 98]` or `OSError: [WinError 10013]` when the server tries to start. See Address already in use for how to handle that.

## Registration using Flask and HTML

I want to build a registration form that will take new user data and send them to backend. In the backend flask will take care of this new user data. For example purpose I am just printing the posted data.

### Folder structure

To get started with the registration, I need a directory structure like-

```
- Flask-Register
    - templates
        - register.html
    - app.py
```

### HTML code for registration

_register.html_

```html
<!DOCTYPE html>
<html>
  <head>
    <title>Registration Page</title>
  </head>
  <body>
    <h1>Register Now</h1>
    <form action="/register" method="POST">
      <input type="text" name="username" />
      <input type="email" name="email" />
      <button type="submit">Submit</button>
    </form>
  </body>
</html>
```

`form` tag needs some attribute set-

1. `action`: The URL that the form data is sent to on submit. Generate it with url_for. It can be omitted if the same URL handles showing the form and processing the data.
1. `method="post"`: Submits the data as form data with the POST method. If not given, or explicitly set to `GET`, the data is submitted in the query string (`request.args`) with the GET method instead.
1. `enctype="multipart/form-data"`: When the form contains file inputs, it must have this encoding set, otherwise the files will not be uploaded and Flask won't see them.

### Flask code

_app.py_

```python
from flask import Flask, request, render_template

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("register.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method=="POST":
        username = request.form['username']
        email = request.form['email']
        print(f"User Name: {username}\nEmail Address: {email}")

    return render_template("register.html")
```

#### HTTP methods

Web applications use different HTTP methods when accessing URLs. By default, a route only answers to GET requests. You can use the `methods` argument of the **route()** decorator to handle different HTTP methods.

The example above keeps all methods for the route within one function, which can be useful if each part uses some common data.

I can also separate views for different methods into different functions. Flask provides a shortcut for decorating such routes with **get()**, **post()**, etc. for each common HTTP method.

#### Rendering Templates

Generating HTML from within Python is not fun, and actually pretty cumbersome because I have to do the HTML escaping on your own to keep the application secure. Because of that Flask configures the _Jinja2_ template engine automatically.

Templates can be used to generate any type of text file. For web applications, you’ll primarily be generating HTML pages, but you can also generate markdown, plain text for emails, and anything else.

To render a template I can use the **render_template()** method. All I have to do is provide the name of the template and the variables I want to pass to the template engine as keyword arguments. In the example above I did not pass any arguments to the template but later I will use them to make the whole process more easier for some of the repetative tasks.

#### The Request Object

The current request method is available by using the `method` attribute. To access form data (data transmitted in a `POST` or `PUT` request) you can use the `form` attribute.

By default `ImmutableMultiDict` is returned from `form` property. This can be changed by setting **parameter_storage_class** to a different type. This might be necessary if the order of the form data is important.

> What happens if the key does not exist in the form attribute? In that case a special **KeyError** is raised. I can catch it like a standard **KeyError** but if I don’t do that, a HTTP 400 Bad Request error page is shown instead. So for many situations I don’t have to deal with that problem.
> To access parameters submitted in the URL (`?key=value`) I can use the args attribute:

```python
searchword = request.args.get('key', '')
```

> It is recommended to access URL parameters with get or by catching the **KeyError** because users might change the URL and presenting them a 400 bad request page in that case is not user friendly.

## Database connection

I am connecting with MySQL database in this example. To do that first I need to install the `mysql-connector-python` library. So, first I will install it using `pip`.

For starters I will make the database using MySQL workbench to make things simpler (If I already have a database with table then I won't have to write code for that in Python).

In this example, I am creating `flasklearning` database in MySQL with `user` table. Following is the mysql code for that-

_schema.sql_

```sql
DROP SCHEMA IF EXISTS flasklearning;
CREATE SCHEMA IF NOT EXISTS flasklearning;

USE flasklearning;

CREATE TABLE user (
	user_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    user_name VARCHAR(100) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    pass VARCHAR(1000) NOT NULL
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8mb4;
```

Then, I copy pasted the mysql connector code from [official documentation](https://dev.mysql.com/doc/connector-python/en/connector-python-example-connecting.html).

Code is used to connect with the MySQL workbench-

_connect_to_mysql.py_

```python
import logging
import time
import mysql.connector

# Set up logger
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")

# Log to console
handler = logging.StreamHandler()
handler.setFormatter(formatter)
logger.addHandler(handler)

# Also log to a file
file_handler = logging.FileHandler("cpy-errors.log")
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

def connect_to_mysql(config, attempts=3, delay=2):
    attempt = 1
    # Implement a reconnection routine
    while attempt < attempts + 1:
        try:
            return mysql.connector.connect(**config)
        except (mysql.connector.Error, IOError) as err:
            if (attempts is attempt):
                # Attempts to reconnect failed; returning None
                logger.info("Failed to connect, exiting without a connection: %s", err)
                return None
            logger.info(
                "Connection failed: %s. Retrying (%d/%d)...",
                err,
                attempt,
                attempts-1,
            )
            # progressive reconnect delay
            time.sleep(delay ** attempt)
            attempt += 1
    return None
```

For explanation about the code, refer to the official documentation.

Next, I am creating the flask application which will send the user name, email and password to the server. The data coming from client will then be used for writing query that will insert these values in the database table `user`.

_app.py_

```python
from flask import Flask, request, render_template
from connect_to_mysql import connect_to_mysql

app = Flask(__name__)

config = {
    "host": 'localhost',
    "user": 'root',
    "password": 'arup#123',
    "database": 'flasklearning'
}

@app.route("/")
@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method=="POST":

        username = request.form['username']
        email = request.form['email']
        password = request.form['password']

        cnx = connect_to_mysql(config)
        if cnx and cnx.is_connected():
            with cnx.cursor() as cursor:
                cursor.execute('INSERT INTO user(user_name, email, pass) VALUES(%s, %s, %s)', (username, email, password))
                cnx.commit()

            cnx.close()
        else:
            print("Failed to connect...")
            return render_template("reg_fail.html")

        return render_template("reg_succs.html")

    return render_template("register.html")
```

The code is almost similar to previous example but with some additional code like the `config` variable which stores the keyword arguments to connect with the mysql database. It will be passed to `connect_to_mysql` function.

Next comes the main part where the insertion of user data is performed-

```python
cnx = connect_to_mysql(config)
if cnx and cnx.is_connected():
    with cnx.cursor() as cursor:
        cursor.execute('INSERT INTO user(user_name, email, pass) VALUES(%s, %s, %s)', (username, email, password))
        cnx.commit()

    cnx.close()
```

`cnx` is the connection object that is used to connect with mysql database. Then, with `cnx` I can get the `cursor` for executing the mysql query. Although I have to be careful that connection is valid, for that I used **is_connected()** method. The cursor need to be closed as well so I used the `with` statement in Python to take care of that automatically.

The **execute()** method is used to execute the query, it takes 2 arguments - one is the query statement with `%s` as the placeholder and another argument is the variables that will be placed as the value of those placeholders.

Lastly close the connection using **close()** method.

> I used 2 additional html files for showing users that the registration is successful or failed.

## Login Authentication

I am trying to validate users of my website and only let them go to the main page if they are registered users.

To achieve this, I am will almost do the same thing as before, just the query will change.

Instead of `INSERT`, I will `SELECT` the users whose `username` and `password` match with `user_name` and `pass` present in the database.

As the database only allows unique `user_name` I will get the one result if I get any.

Code for achieving all this-

_app.py_

```python
from flask import Flask, request, render_template
from connect_to_mysql import connect_to_mysql

app = Flask(__name__)

config = {
    "host": "localhost",
    "user": "root",
    "password": "arup#123",
    "database": "flasklearning"
}

@app.route("/")
@app.route("/login", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        username = request.form['username']
        password = request.form['password']

        cnx = connect_to_mysql(config)
        if cnx and cnx.is_connected():
            row = None
            with cnx.cursor() as cursor:
                cursor.execute('SELECT * FROM user WHERE user_name=%s AND pass=%s', (username, password, ))

                row = cursor.fetchone()
                if not row:
                    return render_template("login_fail.html")

            cnx.close()
            return render_template("login_succs.html", username = row[1])
    return render_template("login.html")
```

As you can see the query this time is `cursor.execute('SELECT * FROM user WHERE user_name=%s AND pass=%s', (username, password, ))` and I am also checking whether there exists any row that has same `user_name` and `pass`. If such row exists then send the login successful message otherwise send the login failure message.

## Login and Register using SQLAlchemy

The folder structure for this small project-

```
- run.py
- app
    - __init__.py
    - model.py
    - templates
        - login.html
        - login_fail.html
        - login_succs.html
```

### What is SQLAlchemy?

SQLAlchemy is the Python SQL toolkit and Object Relational Mapper that gives application developers the full power and flexibility of SQL.

It provides a full suite of well known enterprise-level persistence patterns, designed for efficient and high-performing database access, adapted into a simple and Pythonic domain language.

### What is Flask-SQLAlchemy

Flask-SQLAlchemy simplifies using SQLAlchemy by automatically handling creating, using, and cleaning up the SQLAlchemy objects you’d normally work with. While it adds a few useful features, it still works like SQLAlchemy.

```
pip install -U Flask-SQLAlchemy
```

### Initialize the Extension

First create the `db` object using the `SQLAlchemy` constructor.

Pass a subclass of either `DeclarativeBase` or `DeclarativeBaseNoMeta` to the constructor.

_model.py_

```python
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.orm import DeclarativeBase

class Base(DeclarativeBase):
  pass

db = SQLAlchemy(model_class=Base)
```

Once constructed, the `db` object gives you access to the `db.Model` class to define models, and the `db.session` to execute queries.

### Configure the Extension

The next step is to connect the extension to your Flask app. The only required Flask app config is the **SQLALCHEMY_DATABASE_URI** key. That is a connection string that tells SQLAlchemy what database to connect to.

Create your Flask application object, load any config, and then initialize the `SQLAlchemy` extension class with the application by calling `db.init_app`. This example connects to a MySQL database, which is already created.

_\_\_init\_\_.py_

```python
def create_app():
    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:arup#123@localhost/flasklearning'

    from app.model import db
    db.init_app(app)

    # ...
```

### Define model

Subclass `db.Model` to define a model class. The model will generate a table name by converting the `CamelCase` class name to `snake_case`.

_model.py_

```python
from sqlalchemy.orm import Mapped, mapped_column

# ...

class User(db.Model):
    id: Mapped[int] = mapped_column(primary_key=True)
    username: Mapped[str]
    email: Mapped[str] = mapped_column(unique=True)
    password: Mapped[str]
```

### Create the Tables

After all models and tables are defined, call SQLAlchemy.create_all() to create the table schema in the database. This requires an application context. Since you’re not in a request at this point, create one manually.

_\_\_init\_\_.py_

```python
def create_app():
    # ...

    from app.model import db
    with app.app_context():
        db.create_all()

    # ...
```

If you define models in other modules, you must import them before calling create_all, otherwise SQLAlchemy will not know about them.

> **create_all** does not update tables if they are already in the database. If you change a model’s columns, use a migration library like Alembic with Flask-Alembic or Flask-Migrate to generate migrations that update the database schema.

### Add register and login route

#### Selecting user by filtering for login

Queries are executed through `db.session.execute()`. They can be constructed using **select()**. Executing a select returns a `Result` object that has many methods for working with the returned rows.

To learn more ways of selecting, refer to this [documentation](https://docs.sqlalchemy.org/en/20/orm/queryguide/index.html).

```python
def create_app():
    # ...
    from app.model import db, User
    # ...

    @app.route("/")
    @app.route("/login", methods=["GET", "POST"])
    def index():
        if request.method == "POST":
            username = request.form['username']
            password = request.form['password']
            user = db.session.execute(db.select(User).filter_by(username=username)).scalar()
            if password == user.password:
                return render_template("login_succs.html", username = user.username)
            else:
                return render_template("login_fail.html")

        return render_template("login.html")

    # ...
```

#### Adding user in the databse for registration

Generate the data by sending user data to `User` class and making an instance of the class.

To insert data, pass the model object to `db.session.add()` and after doing modification commit it in the database using `db.session.commit()`. In case of `IngtigityError`, use `try-except` block to take care of it.

```python
def create_app():
    # ...

    @app.route("/register", methods=["GET", "POST"])
    def register():
        if request.method=="POST":
            user = User(username=request.form['username'], email=request.form['email'], password=request.form['password'])
            db.session.add(user)
            try:
                db.session.commit()
                return render_template("reg_succs.html")
            except:
                return render_template("reg_fail.html")

        return render_template("register.html")

    # ...
```

### More Notes!

#### Connection URL Format

A basic database connection URL uses the following format. Username, password, host, and port are optional depending on the database type and configuration.

```
dialect://username:password@host:port/database
```

Here are some example connection strings:

```
# SQLite, relative to Flask instance path
sqlite:///project.db

# PostgreSQL
postgresql://scott:tiger@localhost/project

# MySQL / MariaDB
mysql://scott:tiger@localhost/project
```

SQLite does not use a user or host, so its URLs always start with _three_ slashes instead of two. The dbname value is a file path. Absolute paths start with a _fourth_ slash (on Linux or Mac). Relative paths are relative to the Flask application’s instance_path.

#### Application Factories

If you are already using packages and blueprints for your application ([Modular Applications with Blueprints](https://flask.palletsprojects.com/en/3.0.x/blueprints/)) there are a couple of really nice ways to further improve the experience. A common pattern is creating the application object when the blueprint is imported. But if you move the creation of this object into a function, you can then create multiple instances of this app later.

So why would you want to do this?

- Testing. You can have instances of the application with different settings to test every case.
- Multiple instances. Imagine you want to run different versions of the same application. Of course you could have multiple instances with different configs set up in your webserver, but if you use factories, you can have multiple instances of the same application running in the same application process which can be handy.

So how would you then actually implement that?
**Basic Factories**
The idea is to set up the application in a function. Like this:

```python
def create_app(config_filename):
    app = Flask(__name__)
    app.config.from_pyfile(config_filename)

    # ...

    return app
```

> The downside is that you cannot use the application object in the blueprints at import time. You can however use it from within a request. How do you get access to the application with the config? Use `current_app`.

**Factories & Extensions**
It’s preferable to create your extensions and app factories so that the extension object does not initially get bound to the application.
Using Flask-SQLAlchemy, as an example, you should not do something along those lines:

```python
def create_app(config_filename):
    app = Flask(__name__)
    app.config.from_pyfile(config_filename)

    db = SQLAlchemy(app)
```

But, rather, in model.py (or equivalent):

```python
db = SQLAlchemy()
```

and in your application.py (or equivalent):

```python
def create_app(config_filename):
app = Flask(**name**)
app.config.from_pyfile(config_filename)

    from yourapplication.model import db
    db.init_app(app)

```

Using this design pattern, no application-specific state is stored on the extension object, so one extension object can be used for multiple apps.

## Blueprints

Flask uses a concept of blueprints for making application components and supporting common patterns within an application or across applications. Blueprints can greatly simplify how large applications work and provide a central means for Flask extensions to register operations on applications. A **Blueprint** object works similarly to a **Flask** application object, but it is not actually an application. Rather it is a blueprint of how to construct or extend an application.

### Why blueprints?

Blueprints in Flask are intended for these cases:

- Factor an application into a set of blueprints. This is ideal for larger applications; a project could instantiate an application object, initialize several extensions, and register a collection of blueprints.
- Register a blueprint on an application at a URL prefix and/or subdomain. Parameters in the URL prefix/subdomain become common view arguments (with defaults) across all view functions in the blueprint.
- Register a blueprint multiple times on an application with different URL rules.
- Provide template filters, static files, templates, and other utilities through blueprints. A blueprint does not have to implement applications or view functions.
- Register a blueprint on an application for any of these cases when initializing a Flask extension.

Blueprints provide separation at the Flask level, share application config, and can change an application object as necessary with being registered.

> The downside is that you cannot unregister a blueprint once an application was created without having to destroy the whole application object.

### The basic concept of blueprints

The basic concept of blueprints is that they record operations to execute when registered on an application. Flask associates view functions with blueprints when dispatching requests and generating URLs from one endpoint to another.

### Authentication Blueprint

```

```
