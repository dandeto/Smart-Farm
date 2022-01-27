""" This file is used for defining the layout """
from dash import html
from dash import dcc
from layout.header.header import header

content = html.Div(id='page-content')
layout= html.Div([dcc.Location(id="url", refresh=False), header, content])