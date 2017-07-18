
class FSetupVarParser:
    datacontent = {}
    def parsevars(C, filelocation):
        try:
            for line in open(filelocation,'r').readlines():
                if  line[0] != "#" :
                    left = string.split(line)
                    if left[0].split("=")[0] != "set":
                        C.datacontent[left[0].split("=")[0]]=left[0].split("=")[1].strip("\"")
                        #print datacontent
        except:
            raise FileNotFound(str(filelocation))
    parsevars = classmethod(parsevars)

    def getvalue(C, key):
        try:
            return C.datacontent[key]
        except:
            raise DataNotfound(str(key))
    getvalue = classmethod(getvalue)

