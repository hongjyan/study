#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.5.22
#======================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA
g_debug = True
if g_debug:
    from boa_simulate import *

lib_Base = boaimport('BU_Base_lib')

if g_debug:
    VERDICT_PASS = 1
    VERDICT_FAIL = 0
    def verdict(v):
        if v:
            print "PASS"
        else:
            print "FAIL"
            
    def log(str):
        return lib_Base.log(str)
 
    def cli(str):
        return lib_Base.cli(str)

def run():
    if lib_Base.all_objects_should_be_available("NODE"):
        verdict(VERDICT_PASS)
    else:
        verdict(VERDICT_FAIL)

run()
