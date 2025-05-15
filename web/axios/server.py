from flask import Flask

app = Flask(__name__)

@app.route('/health')
def health():
    return {'message': 'server running...'}

@app.route('/users')
def get_users():
    return [
        {'id': '123', 'name': 'user 1'},
        {'id': '124', 'name': 'user 2'},
        {'id': '125', 'name': 'user 3'}
    ]

if __name__=='__main__':
    app.run('127.0.0.1', port=8080)