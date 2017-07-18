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

def main():
    if lib_Base.check_single_obj('/HTTPDPlat') and lib_Base.check_single_obj('/TomcatPlat') \
    and lib_Base.check_single_obj('/Ne3sAgent'):
        verdict(VERDICT_PASS)
    else:
        verdict(VERDICT_FAIL)

main()
