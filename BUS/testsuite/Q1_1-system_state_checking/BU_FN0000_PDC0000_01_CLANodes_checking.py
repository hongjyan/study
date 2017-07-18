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
    mos = lib_Base.GetAvailableNodesState(['/CLA-0', '/CLA-1'])
    for mo in mos:
        if not mo.MO_is_OK():
	        verdict(VERDICT_FAIL)
    verdict(VERDICT_PASS)
run()

