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