#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.4
# 2013.1.7 iwf configuration supported. 
# 2013.1.10 optimize parse_action_selection by yhj
# 2013.1.22 Optimize parse_action_selection, error tip will be shown if user doesn't input inter number.
# 2013.1.24 fix bug for parse_action_selection.
#=====================================================================
import sys
import os 
import getopt
from setOAM import set_oam
from setCP import set_cp
from setUP import set_up
from setLicense import set_license
from setVMGW import set_vmgw
from setTDM import set_tdm
from setSGW import set_sgw
from setSnapshot import set_snapshot
from setIWF import set_iwf

g_action_mode='setup'
g_action_modes=['setup','teardown', 'testcase']

def action_menu():
    print '----Select your Action----'
    print '1.Setup Mode'
    print '2.Teardown Mode'
    print '3.TestCase Mode'
    print '0.Quit'


def testcase_menu():
    print '----Select your Action----'
    print '             (Multi Select)'
    print '1.Test SGW'
    print '2.Test TDM'
    print '3.Test VMGW'
    print '4.Test UP'
    print '5.Test CP'
    print '6.Test license'
    print '7.Test OAM'
    print '8.Test IWF'
    print '9.Save Sanpshot'
    print 'a.Return Top Menu'
    print '0.Quit'

def setup_menu():
    print '----Select your Action----'
    print '             (Multi Select)'
    print '1.Setup OAM'
    print '2.Setup License'
    print '3.Setup CP'
    print '4.Add UP'
    print '5.Add VMGW'
    print '6.Setup TDM'
    print '7.Setup Signaling'
    print '8.Setup iwf'
    print '9.Save Snapshot'
    print 'a.Return Top Menu'
    print '0.Quit'

def teardown_menu():
    print '----Select your Action----'
    print '             (Multi Select)'
    print '1.Remove Signaling'
    print '2.Remove TDM'
    print '3.Remove VMGW'
    print '4.Remove UP'
    print '5.Remove CP'
    print '6.Remove License'
    print '7.Remove OAM'
    print '8.Remove iwf'
    print 'a.Return Top Menu'
    print '0.Quit'


def quit():
    sys.exit(0)

def toplevel():
    global g_action_mode 
    g_action_mode = 'toplevel'


def error_tips():
    print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
    print '!!Invalid Iput! plz type the HEX number!!'
    print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'


def error2_tips():
    print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'
    print '!!Invalid Iput! plz type 0/1/2/3!!!'
    print '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'

def ishex(s): #decide every character in s is the character of hex
    flag = True
    for c in s:
        if not ((48 <= ord(c) and 57 >= ord(c)) or (97 <= ord(c) and 102 >= ord(c))):
            flag = False
    return flag


def parse_action_selection(x):
    try:
        if (int(x) >= 0 and int(x) <=3  and len(x) == 1):
            i=int(x)
            count = len(g_action_modes)+1
            if i==0:
                quit()
            global g_action_mode
            g_action_mode = g_action_modes[i-1]
            print g_action_mode + ' mode is selected.'
            return True
        else:
            error2_tips()
            return False
    except ValueError, e: # if int(x) fail, come here.
        error2_tips(), e

def parse_setup_selection(x):
    x=x.lower()
    if ishex(x):
        ii = list(set(x))
        ii.sort()
        for i in ii:
            cmd = {
                    '1': lambda: set_oam('up'),
                '2': lambda: set_license('up'),
                '3': lambda: set_cp('up'),
                '4': lambda: set_up('up'),
                '5': lambda: set_vmgw('up'),
                '6': lambda: set_tdm('up'),
                '7': lambda: set_sgw('up'),
                '8': lambda: set_iwf('up'),
                '9': lambda: set_snapshot('up'),
                'a': lambda: toplevel(),
                '0': lambda: quit(),}
            cmd[i]()

    else:
        print "******"
        error_tips()

def parse_teardown_selection(x):
    x=x.lower()
    if ishex(x):
        ii = list(set(x))
        ii.sort()
        for i in ii:
            cmd = {
                '1': lambda: set_sgw('down'),
                '2': lambda: set_tdm('down'),
                '3': lambda: set_vmgw('down'),
                '4': lambda: set_up('down'),
                '5': lambda: set_cp('down'),
                '6': lambda: set_license('down'),
                '7': lambda: set_oam('down'),
                '8': lambda: set_iwf('down'),
                'a': lambda: toplevel(),
                '0': lambda: quit(),}
            cmd[i]()

    else:
        error_tips()

def parse_testcase_selection(x):
    x=x.lower()
    if ishex(x):
        ii = list(set(x))
        ii.sort()
        for i in ii:
            cmd = {
                    '1': lambda: set_sgw('both'),
                '2': lambda: set_tdm('both'),
                '3': lambda: set_vmgw('both'),
                '4': lambda: set_up('both'),
                '5': lambda: set_cp('both'),
                '6': lambda: set_license('both'),
                '7': lambda: set_oam('both'),
                '8': lambda: set_iwf('both'),
                '9': lambda: set_snapshot('both'),
                'a': lambda: toplevel(),
                '0': lambda: quit(),}
            cmd[i]()

    else:
        error_tips()

def help():
    print '   welcome to take BUS (Bring Up Scripts)' 
    print '                     have fun to the ride\n' 
    print './bus --help, get this help information\n' 
    print './bus --up, setup all configuration\n'  
    print './bus --down, teardown all configuration\n'  
    print './bus --both, setup all configuration and then teardown all configuration\n' 
    print './bus, go to interact mode, you can select "OAM" or/and "CP" or/and "UP" or/and "License" or/and "VMGW" or/and "TDM" or/and "SGW" or/and "Snapshot" to setup or teardown or setup&teardown' 


def main():
    if 2 == len(sys.argv):
        if "--up" == sys.argv[1]:
            for i in range(1,10):
                i = str(i)
                parse_setup_selection(i)
            print "All configuration done successfully"
        elif "--down" == sys.argv[1]:
            for i in range(1,9):
                i = str(i)
                parse_teardown_selection(i)
        elif "--both" == sys.argv[1]:
            for i in range(1,10):
                i = str(i)
                parse_testcase_selection(i)
        else: # if sys.argv length is 2, and opition isn't any of --up/--down/--both, then help and exit
            help()
            sys.exit(-1)
    
    elif 1 == len(sys.argv): # if no opition, then go to interact mode
        while True:
            action_menu() 
            n=raw_input("select mode: ")
            if parse_action_selection(n):
                while True:
                    if g_action_mode == 'setup':
                        setup_menu() 
                        n=raw_input("input: ")
                        parse_setup_selection(n)
                    elif g_action_mode == 'teardown':
                        teardown_menu() 
                        n=raw_input("input: ")
                        parse_teardown_selection(n)
                    elif g_action_mode == 'testcase':
                        testcase_menu() 
                        n=raw_input("input: ")
                        parse_testcase_selection(n)
                    else:
                                break
    else: # if sys.argv length is more than 2, help and exit
        help()
        sys.exit(-1)


if __name__ == '__main__':
    main()    
