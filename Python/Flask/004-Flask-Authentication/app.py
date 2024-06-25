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