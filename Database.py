#!/usr/bin/python

import sqlite3

conn = sqlite3.connect('esp32.db')

print ("Opened database successfully")


conn.execute('''CREATE TABLE SENSOR
         (ID INTEGER     PRIMARY KEY AUTOINCREMENT,
         NAME           TEXT    NOT NULL,
         DATA           INT     NOT NULL);''')

print ("Table created successfully")
conn.close()


