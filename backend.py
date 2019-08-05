import sqlite3
from flask import Flask



app = Flask(__name__)

@app.route('/getdata/<idx>/<datax>', methods=['GET'])
def getdata(idx,datax):
        try:
            with sqlite3.connect("esp32.db") as con:
                cur = con.cursor()
                nm = "sensor"+idx
                cur.execute("INSERT INTO SENSOR (NAME,DATA)\
                    VALUES (?,?)",(nm,datax) )
                con.commit()
                msg = "Recorded"
                # cur.execute("SELECT ID, NAME, DATA from SENSOR")
                # for row in cursor:
                #     print ("ID = ", row[0])
                #     print ("NAME = ", row[1])
                #     print ("DATA = ", row[2], "\n")
                
        except:
            con.rollback()
            msg= "Error Record operation"
            #con.close()
        con.close()
        return datax,idx

app.run(host='0.0.0.0', port= 8090)
