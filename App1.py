from flask import Flask

app = Flask(__name__)

@app.route('/getdata/<data>', methods=['GET'])
def getdata(data):
        return "Rpi send :",data

app.run(host='0.0.0.0', port= 8090)
