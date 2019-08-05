import sqlite3
import pandas as pd

conn = sqlite3.connect('esp32.db')
print ("Opened database successfully")

idx = 1
cursor = conn.execute("SELECT ID,NAME,DATA from SENSOR")
for row in cursor:
   print (row)
   print ("ID = ", row[0])
   print ("NAME = ", row[1])
   print ("DATA = ", row[2], "\n")
# cursor = conn.execute("SELECT * FROM SENSOR WHERE ID = ?",(idx,))
# for row in cursor:
#     print (row)
# rows = cursor.fetchall()
# print (rows[:])
conn.close()

