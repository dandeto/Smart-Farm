import dash_bootstrap_components as dbc
from dash import dcc
from dash import html

from pages.demo_iris.data import dataframe
from components.table import make_dash_table

controls = dbc.Card(
    [
        html.Div(
            [
                dbc.Label("X variable"),
                dcc.Dropdown(
                    id="x-variable",
                    options=[
                        {"label": col, "value": col} for col in dataframe().columns
                    ],
                    value="sepal length (cm)",
                ),
            ],
            className="mb-3",
        ),
        html.Div(
            [
                dbc.Label("Y variable"),
                dcc.Dropdown(
                    id="y-variable",
                    options=[
                        {"label": col, "value": col} for col in dataframe().columns
                    ],
                    value="sepal width (cm)",
                ),
            ],
            className="mb-3",
        ),
        html.Div(
            [
                dbc.Label("Cluster count"),
                dbc.Input(id="cluster-count", type="number", value=3),
            ],
            className="mb-3",
        ),
    ],
    body=True,
)

layout = dbc.Container(
    [
        html.H1("Iris k-means clustering"),
        html.Hr(),
        dbc.Row(
            [
                dbc.Col(controls, md=4),
                dbc.Col(dcc.Graph(id="cluster-graph"), md=8),
            ],
            align="center",
        ),
        html.Hr(),
        dbc.Row(
            dbc.Col(make_dash_table(dataframe()), width={"size": 8, "offset": 3}),
            align="center",
        )
    ],
    fluid=True,
)