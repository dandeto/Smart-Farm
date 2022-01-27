# DASH App => Smart Farm Viewer

#### Quick-Links
* **[Architecture Notes]()**

## Purpose
The purpos of this app is to visualize smart farm data.

## Usage
It will be used by a farm owner or manager to look at their field sensors data and judge how the soil is doing. This will allow them to make better decisions about how to keep the field and areas of it healthy.

### Prerequisits to run or develop
In order to get a dash app to run, you must install some python components via pip.

From the root folder run:
* `docker-compose build`
* `docker-compose up -d`
* `docker-compose logs -f` + `ctrl+c` to exit if good.
* `docker-compose down`

Source Management Help
https://github.com/jazzband/pip-tools


### Instructions on how to get the Prerequisits
- pip install dash
- pip install jupyter-dash
- pip install pandas
- pip install PySerial
- pip install dash-bootstrap-components


### Downloading the App

## Maintaining and Developing the App
Need help on UI? 
- [Dash Bootstrap Component Examples](https://github.com/facultyai/dash-bootstrap-components/tree/main/examples/python/advanced-component-usage)
- [Dash Bootstrap Compoenente Docs](https://dash-bootstrap-components.opensource.faculty.ai/)

### Architecture Notes:

#### MVC Folders:
Often separate pages will have their functionality separated out in a Model-View-Controller (MVC) fashion. In this app there is a folder for the page, then there is a data.py, interaction.py, and view.py file inside it.

The advantage of this is three-fold:
1. There is a very clear distiction of file responisbility, thus you know where to edit code.
2. You can test the data.py file on it's own without all the dash app interaction and view code failing on you.
3. Because of the separation of concerns, it is easier to have multiple people work on separate parts of the app without creating merge conflicts.

### Helpful Code Snippets

#### General Tips:
- an empty figure can be returned as an empty dictionary {}, this is better than returning values that are non-figures like -1, if a figure is expected. 

#### Determining Which Callback Fired - Basic

There is a multi input callback, so how do you know which input was triggered?
Use the following code in the callback function:
```python
ctx = callback_context
updated = ctx.triggered[0]['prop_id'].split('.')[0]
print(f"Updated Input: {updated}")
```

#### Determining Which Callback Fired - Pattern Matching

There is a multi input callback for pattern matching, so how do you know which input was triggered?
Use the following code in the callback function:
```python
ctxDict = json.loads(callback_context.triggered[0]['prop_id'].split('.')[0])
updated = ctxDict.get('type')
print(f"Updated Input: {updated}")
```


### EndNotes

#### Maintainers:
- Current Mainatiners:
    - github/robotcorner
    - github/dandeto
- Previous Maintainers:
    - 

#### Timeline:
- This project started in September 2021.


## Installing Requirements

## pip install

You are probably familiar with the pip install command already.  You run pip install <package name> and it will install that package.  Further, you can run pip install <package name>==<version number> to install a specific version of a package.

However, instead of installing the packages you need one by one, you should use your requirements.txt file to install the packages. This has two benefits:

    You don’t have to manually type pip install 10 times to get all of your packages installed
    You don’t have to worry about getting the right version installed

By default, pip will just install the latest version of each package. But this might not be the behavior you want.  Requirements.txt will install the specific versions you requested.

To install your packages using requirements.txt, it is super easy.

    Open a terminal or command prompt
    Navigate to the folder with your requirements.txt
    run: pip install -r requirements.txt
    You are done installing dependencies


If you are using a virtual environment, and you want to create your requirements.txt, it is as simple as running this command:

pip freeze > requirements.txt

The command above will list all of the installed packages, and output them to the requirements.txt file.