#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.5.24
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA
g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    VERDICT_PASS = 1
    VERDICT_FAIL = 0
    def verdict(V):
        if V:
            print "PASS"
        else:
            print "FAIL"
            
    def log(str):
        return lib_MO.log(str)
 
    def cli(str):
        return lib_MO.cli(str)
 
    def stop_tc_execution():
        return lib_MO.stop_tc_execution()
 
    def wait_trigger():
        return lib_MO.wait_trigger()
 
    def send_trigger():
         return lib_MO.send_trigger()


CLASS_CLA0_LDAP = lib_MO.class_MOState("/CLA-0/FSDirectoryServer")
CLASS_CLA1_LDAP= lib_MO.class_MOState("/CLA-1/FSDirectoryServer")
CLA0_LDAP = CLASS_CLA0_LDAP.MO_is_OK()
CLA1_LDAP = CLASS_CLA1_LDAP.MO_is_OK()
if (CLA0_LDAP[0] and CLA0_LDAP[1] == "ACTIVE" and CLA1_LDAP[0] and CLA1_LDAP[1] == "COLDSTANDBY")\
 or (CLA0_LDAP[0] and CLA0_LDAP[1] == "COLDSTANDBY" and CLA1_LDAP[0] and CLA1_LDAP[1] == "ACTIVE"):
    verdict(VERDICT_PASS)
else:
    verdict(VERDICT_FAIL)
