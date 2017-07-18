import imp
def boaimport(name):
    fp, pathname, description = imp.find_module(name)
    return imp.load_module(name, fp, pathname, description)
