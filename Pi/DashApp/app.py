
# Core
import dash
import dash_bootstrap_components as dbc
from flask_caching import Cache
import flask
from utils.external_assets import FONT_AWESOME #, CUSTOM_STYLE

# App Specific
from layout.layout import layout
# from utils.external_assets import FONT_AWESOME, CUSTOME_STYLE


# define flask app.server
server = flask.Flask(__name__)

# Load the Dash App
app = dash.Dash(__name__,
                server=server,
                suppress_callback_exceptions=True,
                external_stylesheets=[dbc.themes.BOOTSTRAP, FONT_AWESOME], # Potentially use: CUSTOM_STYLE too
                meta_tags=[
                    {"name": "viewport", "content": "width=device-width, initial-scale=1"}
                ])

cache = Cache(app.server, config={
    'CACHE_TYPE': 'filesystem',
    'CACHE_DIR': 'cache-directory'
})

# Optionally could use a redis cache
"""
cache = Cache(app.server, config={
    'CACHE_TYPE': 'redis',
    'CACHE_REDIS_URL': os.environ.get('REDIS_URL', 'redis://redis:6379')
})
"""

app.layout = layout
server = app.server     # runs server on gunicorn
