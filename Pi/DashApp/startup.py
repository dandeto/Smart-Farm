"""
This is the starting point of the app. It's like main.py. Run this file when you want to run the app.
"""

from dash import dcc
from dash import html
import dash_bootstrap_components as dbc
from app import app, server
from routes import render_page_content

# import the callbacks from each page into one place
from pages.about.interaction import *
from pages.demo_iris.interaction import *
from pages.farm_overview.interaction import *
from layout.header.header_callbacks import *
# Uncomment below if using the docker image. The plan is to use that mostly for production.
# from environment.settings import APP_HOST, APP_PORT, APP_DEBUG, DEV_TOOLS_PROPS_CHECK, DEV_TOOLS_SILENCE_ROUTES, USE_RELOADER

"""
# start app
if __name__ == "__main__":
    app.run_server(
        host=APP_HOST,
        port=APP_PORT,
        debug=APP_DEBUG,
        dev_tools_props_check=DEV_TOOLS_PROPS_CHECK,
        dev_tools_silence_routes_logging=DEV_TOOLS_SILENCE_ROUTES, 
        use_reloader=USE_RELOADER,
    )
"""

# start app without config file
# ---- you would also want to do this if you're not testing with docker but the debugger
if __name__ == '__main__':
   app.run_server(
        host="127.0.0.1", 
        debug=True, 
        port=8085, 
        dev_tools_props_check=True, 
        dev_tools_silence_routes_logging=True, 
        use_reloader=False
   )

