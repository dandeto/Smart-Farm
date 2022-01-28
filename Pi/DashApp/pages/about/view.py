import dash_bootstrap_components as dbc

layout = dbc.Button(
    [
        "About",
        dbc.Badge("4", color="light", text_color="primary", className="ms-1"),
    ],
    color="primary",
)