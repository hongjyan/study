#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.9.20
# 2013.2.22 change set to SET in add sync by YHJ
#=====================================================================
import sys
import time
import os
import re
g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *


sys.path.append("../lib")  # not used in BOA

lib=boaimport('BU_BUS_Lib')
lib_delete_sgw=boaimport('delete_sgw_lib')

if g_debug:
        VERDICT_FAIL = 0
        VERDICT_PASS = 1
        def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
                if v == VERDICT_FAIL:
                        print 'FAIL'
                else:
                        print 'PASS'
    
        def cli(str):     #simulate funcation of BOA, issue command:str on MGW
                return os.popen(str).read()
    
        def log(str):     #simulate funcation of BOA, log anything what you want 
                print str
    
    
        #simulate funcation of BOA,BOA will run corresponding code between wait_trigger
                #and send_trigger when there is a test procedure named TpAutomationTrigger in 
                #test case,More question, please ask yikun.huang@nsn.com
        def wait_trigger(): 
                print "end call execution"
                print "start script execution"
                print '.'
                print '.'
                print '.'
    
        def send_trigger():
                print "end script execution"
                print "start call execution"
                print '*'
                print '*'
                print '*'
    
        def stop_tc_execution():       #simulate funcation of BOA, stop test case execution, if there is some critical error happened
                verdict(VERDICT_FAIL)
                sys.exit(-1)

def bus_cli(str):
    if g_debug:
        out = os.popen(str + " 2>&1").read()
        sys.stdout.write('\n'+str+ '\n' + out)
        return out 
    else:
        return cli(str)
       
def bus_fsclish(str):
    if g_debug:
        cmd = 'fsclish -c "' + str + '"' 
        out = os.popen(cmd + " 2>&1").read()
        sys.stdout.write('\n'+cmd + '\n' + out)
        return out 
    else:
        return fsclish(str)


def help():
    log("usage:  python delete_tdm_lib.sh [set|pgr|letgr|crct|cgr|ater|all] [index]")
    log("        for crct/ater, the index can be etpcm or apcm index.")
    log("example:")
    log("        python delete_tdm_lib.py --set    1,3-5,9-10")
    log("        python delete_tdm_lib.sh --pgr    0")
    log("        python delete_tdm_lib.sh --letgr  1")
    log("        python delete_tdm_lib.sh --crct   11001")
    log("        python delete_tdm_lib.sh --cgr    1")
    log("        python delete_tdm_lib.sh --ater   11001")
    log("        python delete_tdm_lib.sh --ater   10-20")
    log("        python delete_tdm_lib.sh --all ")

    
    
def remove_crct_by_pcm(pcm_index):
    if ( int(pcm_index) <= 10752 and int(pcm_index) >= 0 ):   #normal et pcm#
        if "" != bus_cli("fsclish -c \" show tdm pdh et-state et-index %s\"|awk '/unlocked/'" %(pcm_index)):
            bus_fsclish("set tdm pdh et-state admin-state locked et-index %s" %(pcm_index))
    elif ( int(pcm_index) <= 21752 and int(pcm_index) >= 11000): #ater part#
        if "" != bus_cli("fsclish -c \" show tdm ater-state apcm-index %s\" |awk '/unlocked/'" %(pcm_index)):
            bus_fsclish("set tdm ater-state admin-state locked apcm-index %s" %(pcm_index))
    else:
        log("invalid pcm index %s" %(pcm_index))

    #############get cgr number#####################
    cgrs = bus_cli("fsclish -c \"show tdm crct-state crct-pcm %s crct-tsl 1-31\"|awk '/CGR:/ {split($2,b,\":\");{print b[2]}}' | sort| uniq" %(pcm_index)).strip().split()
    for cgr in cgrs:
        i = 0
        tsls = bus_cli("fsclish -c \"show tdm crct-state crct-pcm %s crct-tsl 1-31\"|grep CGR:%s -A2|awk '/'%s-'/ {split($1,c,\"-\");{print c[2]}}'" %(pcm_index, cgr, pcm_index)).strip().split()
        for tsl in tsls:    
            if 0 == i:
                first_tsl = tsl
                last_tsl = tsl
            ############not continious with previous crct, so remove previous crcts first###
            if ( (int(tsl) - int(last_tsl) ) > 1):
                log("remove pcm %s tsl %s-%s from cgr %s" %(pcm_index, first_tsl, last_tsl, cgr))
                bus_cli("fsclish -c \"delete tdm crct cgr %s crct-pcm %s crct-tsl %s-%s\" |grep -v -i \"Executing\"" %(cgr, pcm_index, first_tsl, last_tsl))
                frist_tsl = tsl
            ###########continue crct, continue increase last_tsl
            else:
                i =+ 1
        
            last_tsl = tsl
        
        ##########remove the last circuit#######
        log("remove pcm %s tsl %s-%s from cgr %s" %(pcm_index, first_tsl, last_tsl, cgr))
        bus_cli("fsclish -c \"delete tdm crct cgr %s crct-pcm %s crct-tsl %s-%s\" |grep -v -i \"Executing\"" %(cgr, pcm_index, first_tsl,last_tsl))
    
    if  not len(cgrs):
        log("pcm:%s has no crct in circuit group" %(pcm_index))
    
    cgrs = bus_cli("fsclish -c \"show tdm crct-state crct-pcm %s crct-tsl 1-31\"|awk '/CGR:/ {split($2,b,\":\");{print b[2]}}' | sort| uniq" %(pcm_index)).strip().split()
    if len(cgrs):
        return False
    else:
        return True

def remove_cgr(cgr_index):
    pcms = bus_cli("fsclish -c \"show tdm circuitgroup crct cgr cgr-num %s\" |grep -v -i \"Executing\" |awk '/[0-9]-[0-9]/{split($1,a,\"-\"); {print a[1]};}'|sort |uniq" %(cgr_index)).strip().split()
    for pcm in pcms:
        remove_crct_by_pcm(pcm)
    ##added by Rene,2013.7.5,delete vmg circuitgroup start##
    out = "fsclish -c \"show vmgw mgw vid 0 \"|awk '/virtual media gateway name/ {print $6}'"
    vmgw_name = bus_cli(out).strip().split()[0]
    bus_fsclish("delete vmgw circuitgroup cgr %s vmn %s" %(cgr_index,vmgw_name))
    ##added by Rene,2013.7.5,delete vmg circuitgroup end##
    bus_fsclish("delete tdm circuitgroup cgr %s" %(cgr_index))
    
    if not bus_cli("fsclish -c \"show tdm circuitgroup cgr %s\"|grep \"group number\"" %(cgr_index)):
        return True
    else:
        return False      



def remove_ater_by_et(et_index):
    apcms = bus_cli("fsclish -c \"show tdm ater et-index %s\" |grep -v -i \"Executing\|NBR\" |awk '/'%s'/ {print $NF}'" %(et_index, et_index)).strip().split()
    
    for apcm in apcms:
        remove_crct_by_pcm(apcm)
    
    nbr = bus_cli("fsclish -c \"show tdm ater et-index %s\" |grep -v -i \"Executing\" |awk '/NBR/ {print $3}'" %(et_index))
    
    if not (len(apcms) and len(nbr)):
        log("%s not configurated as ater use" %(et_index))
        return False

    if "0" != nbr:
        bus_cli("fsclish -c \" set tdm ater nbr et-index %s thru-conn-num 0\" |grep -v -i \"Executing\"" %(et_index))
    
    tcpcms = bus_cli("fsclish -c \"show tdm ater et-index %s\"|grep -v -i \"Executing\|NBR\" |awk '/'%s'/ {print $2}'|sort -r" %(et_index,et_index)).strip().split()
    for tcpcm in tcpcms:
        log("delete tcpcm:%s of et:%s" %(tcpcm, et_index))
        bus_cli("fsclish -c \"delete tdm ater et-index %s tcpcm-index %s\" |grep -v -i \"Executing\"" %(et_index, tcpcm))
    
    apcms = bus_cli("fsclish -c \"show tdm ater et-index %s\" |grep -v -i \"Executing\|NBR\" |awk '/'%s'/ {print $NF}'" %(et_index, et_index)).strip().split(
)
    if not len(apcms):
        return True
    else:
        return False


def remove_ater_by_apcm(apcm_index):
    etpcm = bus_cli("fsclish -c \"show tdm ater apcm-index %s \"|grep -v -i \"Executing\|NBR\"|awk '/'%s'/ {print $2}'" %(apcm_index, apcm_index))
    if "" == etpcm:
        log("apcm:%s non-existent" %(apcm_index))
        return
    
    remove_crct_by_pcm(apcm_index)

    tcpcm = bus_cli("fsclish -c \"show tdm ater et-index %s\"|grep -v -i \"Executing\" |awk '/'%s'/ {print $2}'" %(etpcm, apcm_index))
    log("start to delete apcm:%s" %(apcm_index))
    bus_cli("fsclish -c \"delete tdm ater et-index %s tcpcm-index %s\" |grep -v -i \"Executing\"" %(etpcm, tcpcm))

    if "" == bus_cli("fsclish -c \"show tdm ater apcm-index %s\"|grep %s:" %(apcm_index, apcm_index)).strip():
        return True
    else:
        return False


def remover_ater(index):
    if (index <= 10752 and index >= 0):
        return remove_ater_by_et(index)
    else:
        return remove_ater_by_apcm(index)


def remove_letgr(letgr_index):
    if "" == bus_cli("fsclish -c \"show tdm pdh letgr letgr-index %s\" |grep -i \"LETGR\"" %(letgr_index)):
        log("LETGR:%s doesn't exist" %(letgr_index))
    
    if "" != bus_cli("fsclish -c \"show tdm pdh letgr-state letgr-index %s\" |awk '/unlocked/'" %(letgr_index)):
        bus_fsclish(" set tdm pdh letgr-state admin-state locked letgr-index %s" %(letgr_index))

    log("start to delete LETGR:%s" %(letgr_index))

    a_pcms = bus_cli("fsclish -c \" delete tdm pdh letgr letgr-index %s\" |awk '/circuit/ { split($4,a,\"-\");{print a[2]}}'" %(letgr_index)).strip().split()
    for pcm_index in a_pcms:
        log("net %s has circuits in circuit group" %(pcm_index))
        remove_crct_by_pcm(pcm_index)
    
    ater_pcms = bus_cli("fsclish -c \" delete tdm pdh letgr letgr-index %s\" |awk '/ater/ { split($4,b,\"-\");{print b[2]}}'" %(letgr_index)).strip().split()
    for pcm_index in ater_pcms:
        log("net %s in ater use" %(pcm_index))
        remove_ater_by_et(pcm_index)

    if "" != bus_cli("fsclish -c \"show tdm pdh letgr letgr-index %s\" |grep -i \"LETGR\"" %(letgr_index)):
        bus_cli("fsclish -c \" delete tdm pdh letgr letgr-index %s\" |grep -v \"Executing\"" %(letgr_index))

    log("delete LETGR %s finished" %(letgr_index))

    if "" == bus_cli("fsclish -c \"show tdm pdh letgr letgr-index %s\" |grep -i \"LETGR\"" %(letgr_index)):
        return True
    else:
        return False



def remove_pgr(pgr_index):
    if "" == bus_cli("fsclish -c \"show tdm sdh pgr pgr-index %s\" |grep \"protection group\"" %(pgr_index)):
        log("PGR %s does not exist" %(pgr_index))
        return False
    
    letgr_index = pgr_index
    remove_letgr(letgr_index)
    log("start to delete PGR %s" %(pgr_index))
    bus_cli("fsclish -c \" delete tdm sdh pgr pgr-index %s\" |grep -v \"Executing\"" %(pgr_index))
    log("\ndelete PGR %s finished" %(pgr_index))

    if "" == bus_cli("fsclish -c \"show tdm sdh pgr pgr-index %s\" |grep \"protection group\"" %(pgr_index)):
        return True
    else:
        return False



def remove_set(set_index):
    if "" == bus_cli("fsclish -c \"show tdm sdh hierarchy set-index %s\" |awk '/SET-/ { split($1,a,\"-\");{print a[2]}}'" %(set_index)).strip():
        log("set %s does not exist" %(set_index))
        return False
    else:
        peer_index = bus_cli("fsclish -c \"show tdm sdh hierarchy set-index %s\" |awk '/peer/ { split($2,a,\"-\");{print a[2]}}'" %(set_index)).strip('\n')
        pgr_index = bus_cli("fsclish -c \"show tdm sdh hierarchy set-index %s\" |awk '/prot/ { print $5}'" %(set_index)).strip('\n')

    bus_cli("fsclish -c \"set tdm sdh set-state admin-state locked set-index %s\" |grep -v \"Executing\"" %(set_index))
    bus_cli("fsclish -c \"set tdm sdh set-state admin-state locked set-index %s\"|grep -v \"Executing\"" %(peer_index))

    time.sleep(1)
    
    if "N/A" != pgr_index:
        remove_pgr(pgr_index)

    log("start to delete SET %s" %(set_index))
    
    if "" != bus_cli("fsclish -c \"delete tdm sdh set set-index %s\" |grep \"synchronization\"" %(set_index)).strip():
        bus_cli("fsclish -c \"delete mgw synchronization inputreference port SET-%s\"" %(set_index))
        bus_cli("fsclish -c \"delete tdm sdh set set-index %s\"" %(set_index))

    log("\ndelete SET-%s finished" %(set_index))

    if "" == bus_cli("fsclish -c \"show tdm sdh hierarchy set-index %s\" |awk '/SET-/ { split($1,a,\"-\");{print a[2]}}'" %(set_index)).strip():
        return True
    else:
        return False

def delete_clock():
    set_index = bus_cli("fsclish -c \"show mgw synchronization inputreference\"|awk '/SET/ {print $1}'")
    if "" == set_index:
        return True
    else:
        bus_fsclish("delete mgw synchronization inputreference port %s" %(set_index))

        out = bus_fsclish("show mgw synchronization inputreference")
        if re.search("SET", out):
            return False
        else:
            return True



def remove_all_TDM_resource():
    flag =delete_clock()

    if not lib_delete_sgw.clear_all():
        log("SGW configuration delete failed, all TDM resource remove terminated")
        return False
    
    sets = bus_cli("fsclish -c \"show tdm sdh hierarchy\" |grep -v -i \"peer\" |awk '/SET-/ {split($1,a,\"-\"); {print a[2]} }'").strip().split()
    for set in sets:
        if not remove_set(set):  #remove_set->remove_pgr->remove_letgr->remove_ater_by_et->remove_crct_by_pcm
            flag = False         #                                    ->remove_crct_by_pcm                

    cgrs = bus_cli("fsclish -c \"show tdm circuitgroup all\" |awk '/group number/ {print $5}'").strip().split()
    sgw_cgr = bus_cli("fsclish -c \"show tdm circuitgroup ncgr sgwrm_circuit\"|grep \"group number\"|awk '{print $5}'").strip()
    if "" != sgw_cgr:
        cgrs.remove(sgw_cgr)  #SGW circuit group can't be deleted
    for cgr in cgrs:
        if not remove_cgr(cgr): #remove_cgr->remove_crct_by_pcm
            flag = False
    return flag


def main():
    options = { '--set':lambda index:remove_set(index),'--SET':lambda index:remove_set(index),\
            '--letgr':lambda index:remove_letgr(index),'--LETGR':lambda index:remove_letgr(index),\
            '--pgr':lambda index:remove_pgr(index),'--PGR':lambda index:remove_pgr(index),\
            '--cgr':lambda index:remove_cgr(index),'--CGR':lambda index:remove_cgr(index),\
            '--crct':lambda index:remove_crct(index),'--CRCT':lambda index:remove_crct(index),\
            '--ater':lambda index:remove_ater(index),'--ATER':lambda index:remove_ater(index),\
           }


    if ( (2 == len(sys.argv)) and ("--all" == sys.argv[1] or "--ALL" == sys.argv[1]) ):
        print remove_all_TDM_resource()
        sys.exit(0)
    elif ( (3 == len(sys.argv)) and ("" != bus_cli("echo %s|awk '/^[0-9]/ {printf $0}'" %(sys.argv[2])).strip()) ):
        if options.has_key(sys.argv[1]):
            idx_arr = sys.argv[2]
            indexs = bus_cli("echo %s|awk '{count=split($0, a, \",\");for(i=1;i<=count;i++){count2=split(a[i], b, \"-\");if(1==count2) b[2]=b[1];for(j=b[1];j<=b[2];j++) print j;};}'" %(sys.argv[2])).strip().split()
            for index in indexs:
                options[sys.argv[1]](index)
        else:
            help()
            sys.exit(-1)
    else:
            help()
            sys.exit(-1)

    


if __name__ == "__main__":
    main()
