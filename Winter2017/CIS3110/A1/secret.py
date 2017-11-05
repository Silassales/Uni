

"""
Taken from https://pythonconquerstheuniverse.wordpress.com/2010/10/16/how-to-open-a-web-browser-from-python/
For documentation of the webbrowser module,
see http://docs.python.org/library/webbrowser.html
"""
import webbrowser
new = 2 # open in a new tab, if possible

# open a public URL, in this case, the webbrowser docs
url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
webbrowser.open(url,new=new)