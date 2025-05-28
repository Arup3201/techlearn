from flask import Flask, request, render_template

def create_app():
    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:arup#123@localhost/flasklearning'

    from app.model import db, User
    db.init_app(app)
    with app.app_context():
        db.create_all()

    @app.route("/")
    @app.route("/login", methods=["GET", "POST"])
    def login():
        if request.method == "POST":
            username = request.form['username']
            password = request.form['password']
            user = db.session.execute(db.select(User).filter_by(username=username)).scalar()
            if password == user.password:
                return render_template("login_succs.html", username = user.username)
            else:
                return render_template("login_fail.html")

        return render_template("login.html")
    
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
    
    return app