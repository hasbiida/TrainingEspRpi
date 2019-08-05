import dash
from dash.dependencies import Output, Input
import dash_core_components as dcc
import dash_html_components as html
import plotly
import random
import plotly.graph_objs as go
from collections import deque
import sqlite3
import pandas as pd
X = deque(maxlen=20)
X.append(1)
Y = deque(maxlen=20)
Y.append(1)

app = dash.Dash(__name__)
app.layout = html.Div(
    [   html.H1('Dash Near Realtime ESP32'),
        dcc.Graph(id='live-graph', animate=True),
        dcc.Interval(
            id='graph-update',
            n_intervals = 10
        ),
    ]
)

@app.callback(Output('live-graph', 'figure'),
        [Input('graph-update', 'n_intervals')])


def update_graph_scatter(n):
    dataSQL = [] #set an empty list 
    con = sqlite3.connect("esp32.db")
    cur = con.cursor()
    #cur.execute("SELECT ID, DATA FROM SENSOR WHERE ID = ?",(idx,))
    cur.execute("SELECT ID, DATA FROM SENSOR ORDER BY id DESC LIMIT 20")
    rows = cur.fetchall()
    for row in rows:
        dataSQL.append(list(row))
        labels = ['ID','DATA']
        df = pd.DataFrame.from_records(dataSQL, columns=labels)
        X = df['ID']
        Y = df['DATA']
    con.close()
    data = plotly.graph_objs.Scatter(
             x=list(X),
             y=list(Y),
             name='Scatter',
             mode= 'lines+markers'
             )

    return {'data': [data],'layout' : go.Layout(
                                  xaxis=dict(range=[min(X),max(X)]),
                                  yaxis=dict(range=[min(Y),max(Y)]),)}
 
if __name__ == '__main__':
    app.run_server(host='0.0.0.0', debug=True)