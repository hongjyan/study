#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.5.22
#======================================================================
g_debug=True
#g_debug=False

if g_debug:
    import sys
    sys.path.append('..')
    sys.path.append('../lib')
    from lib.boa_simulate import * 

lib_Base = boaimport('BU_Base_lib')

if g_debug:
    VERDICT_PASS = 1 
    VERDICT_FAIL = 0 
    def verdict(v):
        if v:
            print "PASS"
        else:
            print "FAIL"
                                                        

def run():
    if lib_Base.all_objects_should_be_available("NODE"):
        verdict(VERDICT_PASS)
    else:
        verdict(VERDICT_FAIL)


if __name__ == "__main__":
    run()
