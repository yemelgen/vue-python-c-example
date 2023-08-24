# vue-python-c-example

Introduction
------------
This is simple solution that shows how to use binary code in your web applications. You can use it as a template or just as an idea in your projects.

Installation
------------
First, you need to set up python environment. Make sure that you use Python 3.6 or higher:
```console
python3 -m venv venv
source venv/bin/activate
pip install -r backend/requirements.txt
```

Then, build binaries:
```console

make -C core
mv core/libsecret.x64.so backend
```

Lastly, launch the backend:
```console
python backend/main.py
```

Now, you should be able to use the frontend:
```console
google-chrome frontend/index.html
```
Enjoy :)
