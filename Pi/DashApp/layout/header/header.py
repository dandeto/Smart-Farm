
""" This file is used for defining the header layout """

import dash_bootstrap_components as dbc
import utils.constants as uc

header = dbc.NavbarSimple(
    children=[
        dbc.NavItem(dbc.NavLink("About", href=uc.about_path)),
        dbc.DropdownMenu(
        children=[
            dbc.DropdownMenuItem("Extras", header=True),
            dbc.DropdownMenuItem("Farm Overview", href=uc.farm_overview_path),
            dbc.DropdownMenuItem("Iris Demo", href=uc.iris_path),
        ],
        nav=True,
        in_navbar=True,
        label="View",
        ),
    ],
    brand="SmartFarm",
    color="primary",
    dark=True,
)