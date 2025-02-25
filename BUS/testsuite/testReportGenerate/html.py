#!/usr/local/bin/python2.5
#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
# This script is used to generate test report in html format according data
# which are gotten from BOA by SOAP interface.
# 2013.3.6 First version by Yang Hongjun 
#=====================================================================
import re
import os
import copy
import sys
import time
from xml.dom import minidom
from BeautifulSoup import BeautifulSoup, Tag, NavigableString

BU_tree = {\
'BU_Sanity':{
    '0System state checking':['BU_Q1_0_HAS_checking', 'BU_Q1_1_ClusterState_checking', 'BU_Q1_1_LDAP_checking', 'BU_Q1_1_SCLI_framework_checking', 'BU_Q1_1_CLANodes_checking',\
                                                 'BU_Q1_1_AllNodes_checking', 'BU_Q1_1_eSW_checking', 'BU_Q1_1_call_related_services_checking', \
                                                 'BU_Q1_1_Singaling_service_checking', 'BU_Q1_1_WebUI_service_checking', 'BU_Q1_1_available_RG_state_checking', \
                                                 'BU_Q1_1_available_RU_state_checking', 'BU_Q1_1_AHUB_reset', \
                                                 'BU_Q1_2_save_and_delete_snapshot'],
    '1call_sanity':{\
        '0IP Call':['Nb(IP)(G.711)-Nb(IP)(GSM_FR)', 'Nb(IP)(UMTS-AMR_122)-Nb(SIP-I)(GSM_EFR)',\
           'Iu(IP)(UMTS-AMR_122_795_59)-Iu(IP)(UMTS-AMR_122_795_59)', 'Iu(IP)(UMTS_AMR-WB_5)-Nb(SIP-I)(G711-Alaw)',\
           'Iu(IP)(UMTS_AMR_WB_3)-Nb(IP)(UMTS_AMR_WB_3)', 'Nb(SIP-I)(GSM_FR)-Nb(SIP-I)(UMTS-AMR_102)', \
           'Nb(SIP-I)(HR_AMR_590_475)-AoIP(HR_AMR_590_475)', 'AoIP(G.711)-AoIP(GSM_FR)', \
           'AoIP(FR_AMR-WB_mode-set_0_1_2)-Nb(IP)(GSM_EFR)', 'AoIP(GSM_EFR)-Iu(IP)(UMTS-AMR2_122)',\
           'Mb(HR_AMR_740_670_590_475)-Nb(IP)(FR_AMR_740_670_590_475)', 'Mb(G711)-Iu(IP)(UMTS-AMR2_475)', \
           'Mb(GSM_FR)-AoIP(GSM_HR)', 'Mb(GSM_EFR)-Nb(SIP-I)(UMTS-AMR2_122)', 'Nb(SIP-I)(GSM_EFR)-NbPia(G711)', \
           'AoTDM-Nb(SIP-I)(G711-Alaw)', 'Mb(UMTS-AMR_515)-Mb(UMTS-AMR2_122)'], 
        '1TDM call':['AoTDM-Iu(IP)(UMTS-AMR_122_795_59_475)', 'AoTDM-AoTDM', 'PSTN-Iu(IP)(UMTS-AMR_122_795_59_475)', \
            'AoTDM-NbPia(G711)', 'AoTDM-AoIP(HR_AMR_590_475)', 'AoTDM-Mb(UMTS_AMR-WB_0)','PSTN-Nb(IP)(UMTS-AMR2_122)',\
            'PSTN-Nb(SIP-I)(G711-Alaw)', 'PSTN-Mb(UMTS_AMR-WB_0)', 'PSTN-AoIP_(HR_AMR_590_475)', 'PSTN-NbPia(G711)', \
            'PSTN-PSTN', 'AoTDM-Nb(IP)(UMTS-AMR2_122)'],
        '2ATER call':['A_TDM(Alaw)-Ater(GSM-EFR)', 'Ater_(GSM-EFR)-Ater(GSM-EFR)', 'Ater_(GSM-HR-FR)-Ater(GSM-EFR-FR)',\
             'Ater_(GSM-HS2-EFR-DR)-Ater(GSM-HS2-EFR-DR)'],
        '3Other':['Iu4(IP)(GSM_EFR)-AoIP4(GSM_EFR)_HandOver', 'BU_Q3_1_Mb-Mb_after_HCLB_control_switchover', \
         'BU_Q3_1_AoTDM-AoTDM_after_HCLB_control_switchover', 'BU_Q3_1_Mb-Mb_after_SISU_control_switchover',\
         'BU_Q3_1_AoTDM-AoTDM_after_SISU_control_switchover', '2VMGW_testcase', 'Tone_testcase', \
         'Basic_Mb_LTE-Mb_LTE', 'MB(IP)_LTE-NB(SIP-I)_LTE_call']
    },

    '2SGW_sanity':['BU_SGW_SANITY_BU_IPTDM_01', 'BU_SGW_SANITY_BU_IPTDM_02', 'BU_SGW_SANITY_BU_IPTDM_03', 'BU_SGW_SANITY_BU_IPTDM_04',\
        'BU_SGW_SANITY_IPTDM_BSSAP', 'BU_SGW_SANITY_IPIP_RANAP', 'BU_SGW_SANITY_IPTDM_HSL_1984', 'BU_SGW_SANITY_IPTDM_HSL_128', \
        'BU_SGW_SANITY_IPTDM_SPMC']
},

'BU_extended':{
    '3Redundancy call':{\
        '0call after force switchover':['BU_Q3_2_AoTDM-AoTDM_after_OMU_force_switchover', 'BU_Q3_2_AoTDM-AoTDM_after_CM_force_switchover', \
                'BU_Q3_2_AoTDM-AoTDM_after_HCLB_force_switchover', 'BU_Q3_2_AoTDM-AoTDM_after_SISU_force_switchover', \
                'BU_Q3_2_AoTDM-AoTDM_after_TDMSNIUP_force_switchover', 'BU_Q3_2_Mb-Mb_after_OMU_force_switchover', 'BU_Q3_2_Mb-Mb_after_CM_force_switchover', \
                'BU_Q3_2_Mb-Mb_after_HCLB_force_switchover', 'BU_Q3_2_Mb-Mb_after_SISU_force_switchover', 'BU_Q3_2_Mb-Mb_after_IPNIU_force_switchover'], 
        '1call after restart':['BU_Q3_3_AoTDM-AoTDM_after_OMU_restart', 'BU_Q3_3_AoTDM-AoTDM_after_CM_restart', 'BU_Q3_3_AoTDM-AoTDM_after_HCLB_restart', \
                        'BU_Q3_3_AoTDM-AoTDM_after_SISU_restart', 'BU_Q3_3_AoTDM-AoTDM_after_DSPM_restart', 'BU_Q3_3_AoTDM-AoTDM_after_AHUB_restart', \
                        'BU_Q3_3_AoTDM-AoTDM_after_TDMMGU_restart', 'BU_Q3_3_AoTDM-AoTDM_after_TDMSNIUP_restart', 'BU_Q3_3_Mb-Mb_after_OMU_restart', \
                        'BU_Q3_3_Mb-Mb_after_CM_restart', 'BU_Q3_3_Mb-Mb_after_HCLB_restart', 'BU_Q3_3_Mb-Mb_after_SISU_restart', 'BU_Q3_3_Mb-Mb_after_DSPM_restart'\
                        'BU_Q3_3_Mb-Mb_after_AHUB_restart', 'BU_Q3_3_Mb-Mb_after_IPNIU_restart'],
        '2call during control switchover':['BU_Q4_1_AoTDM-AoTDM_during_OMU_control_switchover', 'BU_Q4_1_AoTDM-AoTDM_during_CM_control_switchover', \
                        'BU_Q4_1_AoTDM-AoTDM_during_HCLB_control_switchover', 'BU_Q4_1_AoTDM-AoTDM_during_SISU_control_switchover', 'BU_Q4_1_AoTDM-AoTDM_during_TDMSNIUP_control_switchover'\
                        'BU_Q4_1_Mb-Mb_during_OMU_control_switchover', 'BU_Q4_1_Mb-Mb_during_CM_control_switchover', 'BU_Q4_1_Mb-Mb_during_HCLB_control_switchover'\
                        'BU_Q4_1_Mb-Mb_during_SISU_control_switchover', 'BU_Q4_1_Mb-Mb_during_IPNIU_control_switchover'],
        '3call after RG handover':['BU_Q5_1_AoTDM-AoTDM_after_SISU_RG_handover', 'BU_Q5_1_AoTDM-AoTDM_after_DSPM_RG_handover', \
                        'BU_Q5_1_Mb-Mb_after_SISU_RG_handover', 'BU_Q5_1_Mb-Mb_after_DSPM_RG_handover']
    },
    '4Signalling after service state change':{\
        '0Signalling after force switchover':['BU_Q6_1_signaling_after_CM_forceSwitchover', 'BU_Q6_1_signaling_after_SGWNetMgr_forceSwitchover', \
                        'BU_Q6_1_signaling_after_TDMSNIUP_forceSwitchover', 'BU_Q6_1_signaling_after_TDMNIP_forceSwitchover_byreset'],
        '1Signalling after restart':['BU_Q6_1_signaling_after_SGWNetMgr_restart', 'BU_Q6_1_signaling_after_SS7_restart', \
                        'BU_Q6_1_signaling_after_TCU_restart', 'BU_Q6_1_signaling_after_TDMNIP_reset', 'BU_Q6_1_signaling_after_dspcore_reselect_byrestart', 'BU_Q6_1_signalling_after_AHUB_reset'],
        '2Signalling after lock':['BU_Q6_1_signaling_after_SGWNetMgr_lock', 'BU_Q6_1_signaling_after_SS7_lock', 'BU_Q6_1_signaling_after_TCU_lock',\
                        'BU_Q6_1_signaling_after_TDMNIP_lock', 'BU_Q6_1_signaling_after_dspcore_reselect_bylock']
    },
    '5Configuration add and delete':['BU_Q1_2_Mb4(GSM_EFR)-Mb4(GSM_EFR)_after_CP_delete_and_add', 'BU_Q1_2_AoTDM-AoTDM_after_TDM_delete_and_add',\
        'BU_Q1_2_AoTDM-AoTDM_after_CP_delete_and_add', 'BU_Q1_2_AoTDM-AoTDM_after_License_delete_and_add', \
        'BU_Q1_2_Mb4-Mb4_after_License_delete_and_add', 'BU_Q1_2_Mb4(GSM_EFR)-Mb4(GSM_EFR)_after_UP_delete_and_add', \
        'BU_Q1_2_Signalling_after_SGW_config_delete_and_add']
    }

}

    

#analysis SOAP_return files, and generate test result whose name is tr
def generate_testResult(tr, *fs):
    open(tr, 'w').close() #clear the content of previous tr
    TR_handler = open(tr, 'a+')
    TR_handler.seek(0)
    for f in fs:
        try:
            doc = minidom.parse(f)
        except:
            print "problem happened when using minidom parse %s, please check file and file content" %(f)
            continue 
        root = doc.documentElement
        execution_name = f.strip('.xml')
        mgw_version = root.getAttribute("mgw_version")
        il_version = root.getAttribute("il_version")
        fp_version = root.getAttribute("fp_version")
        prbs_version = root.getAttribute("prbs_version")
        esws_version = root.getAttribute("esws_version")
        url = root.getAttribute("url")
        TR_handler.write("execution=%s mgw_version=%s il_version=%s fp_version=%s prbs_version=%s esws_version=%s url=%s\n" \
        %(execution_name, mgw_version, il_version, fp_version, prbs_version, esws_version, url))


        tcs = root.getElementsByTagName("testcase")
        for tc in tcs:
            name = tc.getAttribute("name").split("Name: ")[1]
            verdict_result = tc.getElementsByTagName("verdict")[0].getAttribute("value")
            assc_result = tc.getElementsByTagName("assc")[0].getAttribute("value")
            tw_result = tc.getElementsByTagName("tw")[0].getAttribute("value")
            mgw_result = tc.getElementsByTagName("mgw")[0].getAttribute("value")
            script_result = tc.getElementsByTagName("script")[0].getAttribute("value")
            boa_result = tc.getElementsByTagName("boa")[0].getAttribute("value")
            nelmon_result = tc.getElementsByTagName("nelmon")[0].getAttribute("value")
            link = tc.getElementsByTagName("link")[0].getAttribute("value")
 #           if (tw_result=="NA" and boa_result=="PASS" and script_result=="PASS") or (assc_result=="PASS" and tw_result=="PASS" and mgw_result=="PASS" and\
 #           script_result=="NA" and boa_result=="PASS") or (assc_result=="PASS" and tw_result=="PASS" and mgw_result=="PASS" and script_result=="PASS" and \
 #           boa_result=="PASS") or (assc_result=="NA" and tw_result=="PASS" and boa_result=="PASS" and script_result=="NA") or (assc_result=="NA" and tw_result=="PASS"\
 #           and boa_result=="PASS" and script_result=="PASS"):
 #               verdict_result = "PASS" #to do, need XML add this info
 #           else:
 #               verdict_result == "FAIL"
            TR_handler.write("name=%s verdict=%s assc=%s tw=%s mgw=%s script=%s boa=%s nelmon=%s link=%s\n" %(name, verdict_result, assc_result, tw_result, mgw_result, script_result, boa_result, nelmon_result, link))
    TR_handler.close()




def update_testCase_result(src, soup):
    #print src
    localtime = time.localtime()
    updateTime = "%s_%s_%s_%s_%s" %(localtime[0], localtime[1], localtime[2], localtime[3], localtime[4])
    head = soup.h1
    #update head
    head.contents[0].replaceWith("BU test report %s" %updateTime)
    table_map = {"System state checking. URL:":["Base_service_check",], "BU call sanity test result. URL:":["call_sanity",], \
    "BU SGW sanity test result. URL:":["SGW_sanity",], "BU call redundancy test result. URL:":["call_redundancy",], \
    "Signalling after service state change. URL:":["SGW_redundancy",], \
    "Configuration add and delete. URL:":["call_configuration","SGW_configuration"]}

    if not re.search("fp_version", src):
        tc_name = re.search("name=(.*?) ", src).group(1).strip("HZ-").strip()
        verdict = re.search("verdict=(.*?) ", src).group(1).strip()
        assc = re.search("assc=(.*?) ", src).group(1).strip()
        tw = re.search("tw=(.*?) ", src).group(1).strip()
        mgw = re.search("mgw=(.*?) ", src).group(1).strip()
        script = re.search("script=(.*?) ", src).group(1).strip()
        boa = re.search("boa=(.*?) ", src).group(1).strip()
        nelmon = re.search("nelmon=(.*?) ", src).group(1).strip()
        link = re.search("link=(.*)", src).group(1).strip()
        
        try:
            tc = soup.find(text=tc_name) #node of text:test case name in soup
            #print tc
            tc.previous['href'] = link  #update link
            verdict_list = tc.parent.parent.findNextSiblings('td', limit=7) #verdict, tw, nelmon, assc, script, mgw, boa
            #print verdict_list
            #update verdict
            if "PASS" == verdict:
                tc.parent.parent['bgcolor'] = "green"
                verdict_list[0]['bgcolor'] = "green"
                verdict_list[0].contents[0].replaceWith("PASS")
            elif "WARNING" == verdict:
                tc.parent.parent['bgcolor'] = 'yellow'
                verdict_list[0]['bgcolor'] = 'yellow'
                verdict_list[0].contents[0].replaceWith("WARNING")
            elif "FAIL" == verdict:
                tc.parent.parent['bgcolor'] ="red"
                verdict_list[0]['bgcolor'] = "red"
                verdict_list[0].contents[0].replaceWith("FAIL")
        
            #update TW
            if "PASS" == tw:
                verdict_list[1]['bgcolor'] = "green"
                verdict_list[1].contents[0].replaceWith("PASS")
            elif "FAIL" == tw:
                verdict_list[1]['bgcolor'] = "red"
                verdict_list[1].contents[0].replaceWith("FAIL")
                    
            #update Nelmon
            if "PASS" == nelmon:
                verdict_list[2]['bgcolor'] = "green"
                verdict_list[2].contents[0].replaceWith("PASS")
            elif "FAIL" == nelmon:
                verdict_list[2]['bgcolor'] = "red"
                verdict_list[2].contents[0].replaceWith("FAIL")
    
            #update assc
            if "PASS" == assc:
                verdict_list[3]['bgcolor'] = "green"
                verdict_list[3].contents[0].replaceWith("PASS")
            elif "FAIL" == assc:
                verdict_list[3]['bgcolor'] = "red"
                verdict_list[3].contents[0].replaceWith("FAIL")
    
            #update script
            if "PASS" == script:
                verdict_list[4]['bgcolor'] = "green"
                verdict_list[4].contents[0].replaceWith("PASS")
            elif "FAIL" == script:
                verdict_list[4]['bgcolor'] = "red"
                verdict_list[4].contents[0].replaceWith("FAIL")
                        
            #update mgw
            if re.search("PASS", mgw):
                verdict_list[5]['bgcolor'] = "green"
                verdict_list[5].contents[0].replaceWith("PASS")
            elif re.search("FAIL", mgw):
                verdict_list[5]['bgcolor'] = "red"
                verdict_list[5].contents[0].replaceWith("FAIL")
            elif re.search("ALERT|CRITICAL", mgw):
                verdict_list[5]['bgcolor'] = "#800000"
                verdict_list[5].contents[0].replaceWith("CRITICAL")
                tc.parent.parent['bgcolor'] ="#800000"
            elif re.search("WARNING", mgw):
                verdict_list[5]['bgcolor'] = "yellow"
                verdict_list[5].contents[0].replaceWith("WARNING")
    
            #update boa
            if "PASS" == boa:
                verdict_list[6]['bgcolor'] = "green"
                verdict_list[6].contents[0].replaceWith("PASS")
            elif "FAIL" == boa:
                verdict_list[6]['bgcolor'] = "red"
                verdict_list[6].contents[0].replaceWith("FAIL")
        except:
            print "%s haven't been included in BU test cases, please contact with BU team" %tc_name
    else:
        execution_name = re.search("execution=(.*?) ", src).group(1).strip()
        mgw_version = re.search("mgw_version=(.*?) ", src).group(1).strip()
        il_version = re.search("il_version=(.*?) ", src).group(1).strip()
        fp_version = re.search("fp_version=(.*?) ", src).group(1).strip()
        prbs_version = re.search("prbs_version=(.*?) ", src).group(1).strip()
        esws_version = re.search("esws_version=(.*?) ", src).group(1).strip()
        url = re.search("url=(.*)", src).group(1).strip()

        #since there is "\n" at the end of every line, so need nextSibling 2 times
        #if mgw_version and il_version and fp_version and prbs is NA or empty, then update info. Otherwise, skip.
        #update mgw_version
        MGW = soup.find(text="release lable:")
        if MGW.parent.nextSibling.nextSibling.contents[0] == "NA" or \
        MGW.parent.nextSibling.nextSibling.contents[0] == "":
            MGW.parent.nextSibling.nextSibling.contents[0].replaceWith(mgw_version)  
        #update il_version
        IL = soup.find(text="IL version:")
        if IL.parent.nextSibling.nextSibling.contents[0] == "NA" or \
                IL.parent.nextSibling.nextSibling.contents[0] == "":
            IL.parent.nextSibling.nextSibling.contents[0].replaceWith(il_version)
        #update fp_version
        FP = soup.find(text="FP version:")
        if FP.parent.nextSibling.nextSibling.contents[0] == "NA" or \
                FP.parent.nextSibling.nextSibling.contents[0] == "":
            FP.parent.nextSibling.nextSibling.contents[0].replaceWith(fp_version)
        #updat prbs_version
        PRBS = soup.find(text= "PRBs version:")
        if PRBS.parent.nextSibling.nextSibling.a['href'] == "NA":
            PRBS.parent.nextSibling.nextSibling.a['href'] = prbs_version
            PRBS.parent.nextSibling.nextSibling.contents[0].contents[0].replaceWith(prbs_version)
        #updat esws_version
        ESW = soup.find(text= "ESWs version:")
        if ESW.parent.nextSibling.nextSibling.a['href'] == "NA":
            ESW.parent.nextSibling.nextSibling.a['href'] = esws_version
            ESW.parent.nextSibling.nextSibling.contents[0].contents[0].replaceWith(esws_version)
        #updat urls for executions
        for k in table_map.keys():
            n = 1
            for i in table_map[k]:
                if re.search(i, execution_name):
                    #print k
                    if soup.find(text=re.compile("%s.*" %k)) == None: #if update sanity test cases result, go to next execution
                        break
                    node =  soup.find(text=re.compile("%s.*" %k)).parent
                    temp_soup = BeautifulSoup()
                    tag = Tag(temp_soup, 'a')
                    text = NavigableString("%s" %url)
                    tag.insert(0,text)
                    tag['href'] = "%s" %url
                    node.insert(n, tag)
                    #print node
                    n = n + 1
                

def add_statistic(soup):
    rowspan_list = soup.findAll('td', rowspan=True) #find all td with attribute rowspan
    for rowspan in rowspan_list:
        pass_count = 0 
        fail_count = 0
        rows = int(rowspan['rowspan'])
        tr_list = rowspan.parent.findNextSiblings('tr', limit=rows-1) #find the following rows tr after td with attribute rowspan
        #if rowspan's parent(tr) has td with attribute:bgcolor, meaning this tr is test caes tr, not test suite tr.
        if rowspan.parent.find('td', bgcolor=True):
            tr_list.insert(0, rowspan.parent) #add test case tr whose first td is used to name its test suite to tr_list also.

        for tr in tr_list:
           #print "tr is %s" %tr
            verdict = tr.findAll('td', bgcolor=True, limit=2)[1].contents[0]
            if verdict == 'PASS' or verdict == 'WARNING':
                pass_count = pass_count + 1
            else:
                fail_count = fail_count + 1
            #print "pass_count is %s, fail_count is %s" %(pass_count, fail_count)

        rowspan.find(text = re.compile("PASS.*")).replaceWith("PASS:" + str(pass_count)) #update pass count
        rowspan.find(text = re.compile("FAIL.*")).replaceWith("FAIL:" + str(fail_count)) #update fail count

    #find all lines with tag='th' and text="PASS"
    th_l = soup.findAll('th') # find all lines with tag:th
    th_list = []
    for i in th_l:
        if i.find(text=re.compile("PASS.*")) is not None:
            th_list.append(i)
    

    for th in th_list:
        pass_cnt = 0
        fail_cnt = 0
        table = th.parent.parent
        nested_rowspan_list = table.findAll('td', rowspan=True)
        for nrl in nested_rowspan_list:
            p_cnt = re.search("\d+", nrl.find(text=re.compile("PASS.*"))).group()
            f_cnt = re.search("\d+", nrl.find(text=re.compile("FAIL.*"))).group()
            pass_cnt = pass_cnt + int(p_cnt)
            fail_cnt = fail_cnt + int(f_cnt)
        th.find("font", text=re.compile("PASS.*")).replaceWith("PASS: " + str(pass_cnt)) #update pass count for level 2 above suite
        th.find("font", text=re.compile("FAIL.*")).replaceWith("FAIL: " + str(fail_cnt)) #update fail count for level 2 above suite




def help():
   print '''This script use lib:BeautifulSoupi3.0, so you need install BeautifulSoup3.0 in your python\n'''
   print '''Usage:\n'''
   print '''python html.py --sanity|--extended cluster_name tag \n\
   html.py will analysit xml files of /home/BU_testing/test-reports/$tag, \n
   use --sanity to generate sanity test cases report, use --extended to generate extended test cases report.\n
   BU_sanityTestReport_$tag.html or BU_extendedTestReport_$tag.html under /home/BU_testing/test-reports/$cluster_name/$tag/ directory\n'''

#check if all test cases of test suite pass.
def testSuite_OK(soup, name):
    node_name = soup.find(text=re.compile("%s:.*" %name)).parent
    node_fail = node_name.findAll('font')[1].contents[0]
    fail_cnt = int(node_fail.split("FAIL:")[1].strip())
    if 0 == fail_cnt:
        return True
    else:
        return False
    

def main():
    if len(sys.argv) != 4 or (sys.argv[1] != "--extended" and sys.argv[1] != "--sanity"):
        help()
        sys.exit(-1)


    directory = "/home/BU_testing/test-reports/%s/%s/" %(sys.argv[2], sys.argv[3])

    if sys.argv[1] == "--sanity":
        testReport_name = "%sBU_sanityTestReport_%s_%s.html" %(directory, sys.argv[2], sys.argv[3])
        testResult_name = "%ssanityTestResult_%s_%s.txt" %(directory, sys.argv[2], sys.argv[3])
        html_model = "model_sanity.html"
    else:
        testReport_name = "%sBU_extendedTestReport_%s_%s.html" %(directory, sys.argv[2], sys.argv[3])
        testResult_name = "%sextendedTestResult_%s_%s.txt" %(directory, sys.argv[2], sys.argv[3])
        html_model = "model_extended.html"

    os.popen("mkdir %s" %directory)
    os.popen("cp -R /home/BU_testing/script/%s %s" %(html_model, testReport_name))
    handler = open(testReport_name, 'r')
    data = handler.read()
    handler.close()
    soup = BeautifulSoup(data)

    xmls = []
    for l in os.popen("ls %s" %directory).readlines():
        if re.search("\.xml", l):
            xmls.append(directory + l.strip("\n"))

    generate_testResult(testResult_name, *xmls)
    f_handler = open(testResult_name, 'r')
    for i in f_handler.readlines():
        update_testCase_result(i, soup)
    f_handler.close()
        
    add_statistic(soup)
    new_handler = open(testReport_name,'w')
    new_handler.write(soup.__str__())
    new_handler.close()
    if sys.argv[1] == "--sanity":
        if testSuite_OK(soup, "System state checking") and testSuite_OK(soup, "BU call sanity") \
        and testSuite_OK(soup, "BU SGW sanity cases"):
            print "Congratulation!!! Test report %s generated successfully" %testReport_name
            print "All BU sanity test cases pass"
            return True
        else:
            print "Congratulation!!! Test report %s generated successfully" %testReport_name
            print "Not all BU sanity test cases pass!!!!"
            return False

if __name__ == "__main__":
    main()
