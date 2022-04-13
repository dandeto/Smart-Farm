from dash import html
from dash import dcc
from dash import dash_table
import dash_bootstrap_components as dbc

from pages.farm_overview.data import df, dataCols

selectionMenu = dbc.Card(
    [
        html.Div(
            [
                dbc.Label("Data Selection"),
                dcc.Dropdown(
                    id="menu-y-variable",
                    options=[
                        {"label": col, "value": col} for col in dataCols
                    ],
                    value="capacitance",
                ),
            ],
            className="mb-3",
        ),
    ],
    body=True,
)


layout = dbc.Container(
    [
        html.H1("Smart Farm Overview"),
        html.Hr(),
        html.H3("A look at your farm."),
        dbc.Row(
            [
                dbc.Col(selectionMenu, md=3),
                dbc.Col(dcc.Graph(id="graph-data-vs-time"), md=9),
            ],
            align="center",
        ),
        html.Hr(),
        dbc.Row(
            [
                dbc.Col(id="table-data-vs-time", width={"size": 6}),
            ],
            align="center",
        )
    ],
    fluid=True,
)