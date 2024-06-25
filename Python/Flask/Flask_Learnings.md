# Flask Documentation

Flask is a Python Library to develop backend applications and API. It is very versatile, fast and broadly used for small applications.

I am listing down all the things I learnt about Flask in this documentation.
Following are the topics covered-

1. First application using Flask
2. Registration using Flask and HTML
3. Database connection

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
