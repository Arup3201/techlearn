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