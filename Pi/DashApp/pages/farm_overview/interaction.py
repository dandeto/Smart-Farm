from dash.dependencies import Input, Output
import plotly.express as px         # easier, quicker, less control
from dash.dependencies import Input, Output
import pages.farm_overview.data as d
from app import app
import dash_bootstrap_components as dbc
import pandas as pd

@app.callback(
    Output("graph-data-vs-time", "figure"), 
    [
        Input("menu-y-variable", "value")
    ],
)
def make_graph(menuY):
    d.df["recieverId"] = d.df["recieverId"].astype(str)
    d.df[menuY] = d.df[menuY].apply(pd.to_numeric, errors="coerce")
    fig = px.line(data_frame=d.df, x='time', y=menuY, color='recieverId', template="plotly_white")
    dynamicTitle = f"Time vs {menuY}"
    fig.update_layout(title=dynamicTitle)
    return fig


@app.callback(
    Output("table-data-vs-time", "children"), 
    [
        Input("menu-y-variable", "value"),
    ],
)
def make_table(menuY):
    numRows = 100
    # table = d.df['time', menuY].head(numRows)
    tableDf = d.df.filter(items=['time', 'recieverId', menuY])
    tableDf = tableDf.head(numRows)
    return dbc.Table.from_dataframe(tableDf, striped=True, bordered=True, hover=True)
"""
@app.callback(
    Output("menu-y-variable", "options"), [
        Input('menu-y-variable', "value")
    ]
)
def filterDfCol(v):
    return [
        {"label": col, "value": col, "disabled": col == v}
        for col in d.dataCols
    ]
"""