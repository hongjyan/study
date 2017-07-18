#!/usr/bin/env python
#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.10.12 
#=====================================================================

import sys
import os
import string
import re
import commands
import time
import logging
#import ldap
import shutil


#/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\#

class FSetupException(Exception):
    def __init__(self,msg=None):
        self.msg=msg

    def getMsg(self):
        if self.msg==None:
            return ""
        else:
            return str(self.msg)

    def __str__(self):
        return repr(self.msg)

class DataNotfound(FSetupException):
    def __str__(self):
        return "Value does not exist: "+self.getMsg()

class FileNotFound(FSetupException):
    def __str__(self):
        return "File does not exist: "+self.getMsg()

class PathNotFound(FSetupException):
    def __str__(self):
        return "Path does not exist: "+self.getMsg()

class ParseError(FSetupException):
    def __str__(self):
        return "Parsing error: "+self.getMsg()

class BindMountFailed(FSetupException):
    def __str__(self):
        return "Bind mount failed: "+self.getMsg()

class UmountFailed(FSetupException):
    def __str__(self):
        return "mount failed: "+self.getMsg()

class FailedtoStartLDAP(FSetupException):
    def __str__(self):
        return "ldap startup failed: "+self.getMsg()

class FailedtoStopLDAP(FSetupException):
    def __str__(self):
        return "ldap stop failed: "+self.getMsg()

class FailedtoPopulateLDAP(FSetupException):
    def __str__(self):
        return "ldap population failed: "+self.getMsg()

class Failedtolistlv(FSetupException):
    def __str__(self):
        return "Failed to list logical volume: "+self.getMsg()

class NoSpaceLeft(FSetupException):
    def __str__(self):
        return "No space left to create the logical volume "+self.getMsg()

class SWMInstallFailed(FSetupException):
    def __str__(self):
        return "SWM Install failed to create the SW volume "+self.getMsg()

class FailedtoCreatePartition(FSetupException):
    def __str__(self):
        return "Failed to create the partitioning and logical volume creation "+self.getMsg()

class FailedtoMount(FSetupException):
    def __str__(self):
        return "Failed to mount: "+self.getMsg()

class FailedtoConfigureStateVolume(FSetupException):
    def __str__(self):
        return "Failed to configure mstate "+self.getMsg()

class FailedtoConfigureIP(FSetupException):
    def __str__(self):
        return "Failed to configure IP "+self.getMsg()

class FailedtoAddfsLDAPRoot(FSetupException):
    def __str__(self):
        return "User addition failed "+self.getMsg()

class FailedtoDumpConfig(FSetupException):
    def __str__(self):
        return "Config dumping failed "+self.getMsg()

class FailedToSaveLDAPSecFragment(FSetupException):
    def __str__(self):
        return "Failed to save LDAP security fragment to its logical volume "+self.getMsg()

class HASCMFSetupFailed(FSetupException):
    def __str__(self):
        return "Failed to create files needed for HAS CMF to start up "+self.getMsg()

class FailedToCopySSHKeys2State(FSetupException):
    def __str__(self):
        return "Failed to copy ssh keys to state volume "+self.getMsg()

#/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\#

class FSetupVarParser:
    datacontent = {}
    def parsevars(C, filelocation):
        try:
            fs = open(filelocation, 'r')
        except:
            print "can not open %s" %(filelocation)
            raise Exception
        

        for line in fs.readlines():
            if line == '\n':
                continue
            if  line[0] != "#" :
		#print line
                C.datacontent[line.split("=")[0]]=line.split("=")[1].strip("\"").strip()
    parsevars = classmethod(parsevars)

    def getvalue(C, key):
        try:
            return C.datacontent[key]
        except:
            raise DataNotfound(str(key))
    getvalue = classmethod(getvalue)
