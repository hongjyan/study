#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.28
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import time
import os
import re
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

def esw_OK():
    needed_amount = cli("cat eswVersion.txt |grep -i \"needed\"|wc -l")
    not_needed_amount = cli("cat eswVersion.txt |grep -i \"not needed\"|wc -l")
    unknown_error_amount = cli("cat eswVersion.txt |grep -i \"error\|unknown\"|wc -l")
    if needed_amount == not_needed_amount and 0 == int(unknown_error_amount):
        return True
    else:
        return False

def main():
    repeat = 0
    cli("cat /dev/null>eswVersion.txt")
    cli("sanity_test -t embedded>eswVersion.txt&")
    while repeat <= 90: #15 minutes time limit for this script
        out = cli("grep \"Commissioning completed\" eswVersion.txt") 
        if not re.search("Commissioning completed", out):
            time.sleep(10)
            repeat += 1
            continue
        else:
            log("ESW checking completed")
            break  # commisssion finished, break
    
    if 91 == repeat:
        log("ESW checking haven't finished in 15 minutes, terminate script execution, please check result at /root/eswVersion manually")
        verdict(VERDICT_FAIL)
    else:
        if esw_OK():
            verdict(VERDICT_PASS)
        else:
            verdict(VERDICT_FAIL)    

main()
