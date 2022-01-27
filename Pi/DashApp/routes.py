
"""
This module connect between all the webpages on the Dash App
"""
# Core
from dash.dependencies import Input, Output
from dash import html

# Local
from app import app
# Page Paths
import utils.constants as uc

# Apps
from pages.about import view as view_about
from pages.farm_overview import view as view_farm_overview

@app.callback(
  Output("page-content", "children"), 
  Input("url", "pathname")
)
def render_page_content(pathname):
    if pathname == uc.about_path:
        return view_about.layout
    elif pathname == uc.farm_overview_path:
        return view_farm_overview.layout
    else:
        return ([
            html.H1("404: Not found", className="text-danger"),
            html.Hr(),
            html.P(f"The pathname {pathname} was not recognised..."),
        ])
