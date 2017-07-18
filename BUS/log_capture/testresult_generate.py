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
BU_tree = { 'call_sanity':{\
'IP Call':['Nb(IP)(G.711)-Nb(IP)(GSM_FR)', 'Nb(IP)(UMTS-AMR_122)-Nb(SIP-I)(GSM_EFR)',\
           'Iu(IP)(UMTS-AMR_122_795_59)-Iu(IP)(UMTS-AMR_122_795_59)', 'Iu(IP)(UMTS_AMR-WB_5)-Nb(SIP-I)(G711-Alaw)',\
           'Iu(IP)(UMTS_AMR_WB_3)-Nb(IP)(UMTS_AMR_WB_3)', 'Nb(SIP-I)(GSM_FR)-Nb(SIP-I)(UMTS-AMR_102)', \
           'Nb(SIP-I)(HR_AMR_590_475)-AoIP(HR_AMR_590_475)', 'AoIP(G.711)-AoIP(GSM_FR)', \
           'AoIP(FR_AMR-WB_mode-set_0_1_2)-Nb(IP)(GSM_EFR)', 'AoIP(GSM_EFR)-Iu(IP)(UMTS-AMR2_122)',\
           'Mb(HR_AMR_740_670_590_475)-Nb(IP)(FR_AMR_740_670_590_475)', 'Mb(G711)-Iu(IP)(UMTS-AMR2_475)', \
           'Mb(GSM_FR)-AoIP(GSM_HR)', 'Mb(GSM_EFR)-Nb(SIP-I)(UMTS-AMR2_122)', 'Nb(SIP-I)(GSM_EFR)-NbPia(G711)', \
           'AoTDM-Nb(SIP-I)(G711-Alaw)', 'Mb(UMTS-AMR_515)-Mb(UMTS-AMR2_122)'], 
'TDM call':['AoTDM-Iu(IP)(UMTS-AMR_122_795_59_475)', 'AoTDM-AoTDM', 'PSTN-Iu(IP)(UMTS-AMR_122_795_59_475)', \
            'AoTDM-NbPia(G711)', 'AoTDM-AoIP(HR_AMR_590_475)', 'AoTDM-Mb(UMTS_AMR-WB_0)','PSTN-Nb(IP)(UMTS-AMR2_122)',\
            'PSTN-Nb(SIP-I)(G711-Alaw)', 'PSTN-Mb(UMTS_AMR-WB_0)', 'PSTN-AoIP_(HR_AMR_590_475)', 'PSTN-Nbpia(G711)', \
            'PSTN-PSTN', 'AoTDM-Nb(IP)(UMTS-AMR2_122)'],
'ATER call':['A_TDM(Alaw)-Ater(GSM-EFR)', 'Ater_(GSM-EFR)-Ater(GSM-EFR)', 'Ater_(GSM-HR-FR)-Ater(GSM-EFR-FR)',\
             'Ater_(GSM-HS2-EFR-DR)-Ater(GSM-HS2-EFR-DR)'],
'Other':['Iu4(IP)(GSM_EFR)-AoIP4(GSM_EFR)_HandOver', 'BU_Q3_1_Mb-Mb_after_HCLB_control_switchover', \
         'BU_Q3_1_AoTDM-AoTDM_after_HCLB_control_switchover', 'BU_Q3_1_Mb-Mb_after_SISU_control_switchover',\
         'BU_Q3_1_AoTDM-AoTDM_after_SISU_control_switchover', '2VMGW_testcase']
         }
}

l = [] # l is global variable to recode path of struct which is used in funcation:valuepath_of_path
path = [] #path records path of struct
upperTrunkIndex = 1  #TrunkIndex range is 1~100. 1 is the first trunk
currentTrunkIndex = 2 
NodeIndex = 100  #100 above is range for node

#test_tree = { 'game':{'a':1, 'b':2}, 'fun':[3,4,5] }
    

#analysis SOAP_return files, and generate test result whose name is tr
def get_testResult(tr, *fs): #fs is list of SOAP return file
    result_sequence = ["name", "verdict", "assc_result", "tw_result", "mgw_result", "script_result", "boa_result", "nelmon_result", "link"]
    result = ""
    open(tr, 'w').close()  #clear tr previous content
    TR_handler = open(tr, 'a+')
    TR_handler.seek(0)
    pattern = re.compile("<boa:TC" + "( \w*=\".*\")")
    for f in fs:
        f_handler = open(f)
        for In in f_handler:
            match = re.search(pattern, In)
            if match:   #test case result
                #re-sequence match.group(1) according to sequence of result_sequence
                for i in range(len(result_sequence)):
                    p = re.compile(result_sequence[i] + "=\"(.*?)\"")
                    result = result + result_sequence[i] + "=" + re.search(p, match.group(1)).group(1) + " "
                result.rstrip("name=")  # strip "name=" where is at the begining of every test case result
                TR_handler.write(result + "\n") # write result to test result
                result = ""  #clear result for next test case result
            else:  #none test case line of SOAP return file
                pass   
        f_handler.close() # close SOAP return file hanlder
    TR_handler.close() #Test result file write finished, close it.
    return tr  #return test result file name
  

#Get path of one element in a struct
def valuepath_of_struct(v, s): #l is default variable using as static variable
    global l, path

    if len(path):  #if path isn't empty, no need to recuse anymore, return value path
        return path

#    print "s is ", s
#    print "l is %s" %l
    # if dictionary 
    if type(s) == type({}):
        for k in s.keys():
            l.append(k)
            valuepath_of_struct(v, s[k])


    #if list or tuple
    if type(s) == type(()) or type(s) == type([]):
        for i in range(len(s)):
            l.append(i)
            valuepath_of_struct(v, s[i])
    
    
    if not ( type(s) == type({}) or type(s) == type(()) or type(s) == type([]) ): #if s is singleton
        if str(s) == str(v): # if found v.
                path = copy.deepcopy(l)
                print("%s's path is %s\n" %(s, path))
                #print "pop found singleton element's upper path" 
                l.pop()
                return path
        else:
            #print "pop non-found singleton element upper path"
            l.pop()
    else: #if s isn't a sigleton, then pop upper path unless path is root path.
        try:
            #print "pop non-singleton element upper path"
            l.pop()
        except:
            print "s should be original tree here, exception will happen here"


    if len(path):  # if v is found, will go to upper funcation's left code(here), then return 
        return path

    return #return None if v isn't founded in s.
        

#change element's value in a struct. v is new value, pl is the list of element's path, s is struct
def change_elementValue_byPath(v, pl, s):
    if 1 != len(pl):
        return change_elementValue_byPath(v, pl[1:], s[pl[0]])
    else:
        s[pl[0]] = v
        return True

#create test result tree according info from f(testResult.txt) and org_tree(BU_tree)
def create_testResult_tree(f, org_tree):
    #Traverse tree and replace test case name if name of test case of tree can be found from test result.
    global l, path
    TR_tree = copy.deepcopy(org_tree)
    f_handler = open(f, 'r')
    for In in f_handler:
        print re.search("name=(.*?)", In).group(1)
        TS_name = re.search("name=(.*?) verdict", In).group(1).strip().strip('HZ-') # get test case name
        valuepath = valuepath_of_struct(TS_name, org_tree)
        if valuepath is None:
            print "%s is not in BU test suite" %TS_name
        else:
            Inn = TS_name + "       verdict=" + In.partition(" verdict=")[2].strip()
            change_elementValue_byPath(Inn.strip('\n'), valuepath, TR_tree)
        #make l and path empty if go to next In
        l = []
        path = []

    f_handler.close()
    return TR_tree    

#create Root HTML
def createRootHtml():
    import time
    localtime = time.localtime()
    testReport_name = "BU_testReport_%s_%s_%s_%s_%s.html" %(localtime[0], localtime[1], localtime[2], localtime[3], localtime[4])
    #APP_label =  "MGW:" + os.popen("fsswcli -c").read().strip('\n').strip()
    #FP_label = "FP:" + os.popen("rpm -qa|grep -i lynx|grep CFM|awk -F - '{split($3, a, \".x86\");{print a[1]}}'").strip('\n').strip()
    #IL_label = "IL:" + os.popen("rpm -qa|grep -i SS_ILDmxMsg|awk -F - '{split($3, a, \".x86\");{print a[1]}}'").strip('\n').strip()
    html_handler = open(testReport_name, 'a+')
    html_handler.write('''<html> 
        <h1> 
<div align="center"><font color="green">%s</font></div>
<div align="center"><font color="green">%s</font></div>
<div align="center"><font color="green">%s</font></div>
        </h1> 
<body> 


<link rel="STYLESHEET" type="text/css" href="dhtmlxtree.css">
<script  src="./codebase/dhtmlxcommon.js"></script>
<script  src="./codebase/dhtmlxtree.js"></script>
 
 
 
<table width="100%">
    <tr>
        <td valign="top">
            <div id="treeboxbox_tree0" style="width:1200px; height:5000px;border :1px solid Silver;"></div><br>
        </td>
    </tr>
</table>
 
<script>
tree = new dhtmlXTreeObject("treeboxbox_tree0", "150%", "150%", 0);
//tree.setSkin('dhx_skyblue');
tree.setImagePath("./codebase/imgs/");
tree.enableTreeLines(true);
//tree.setImageArrays("plus", "plus2.gif", "plus3.gif", "plus4.gif", "plus.gif", "plus5.gif");
//tree.setImageArrays("minus", "minus2.gif", "minus3.gif", "minus4.gif", "minus.gif", "minus5.gif");
//tree.setStdImages("book.gif", "books_open.gif", "books_close.gif");

tree.insertNewItem(0,1,"root");
//tree.insertNewItem(1,2,"New Node 2");
//tree.insertNewItem(1,3,"New Node 3");
//tree.insertNewItem(1,4,"New Node 4");
//tree.insertNewItem(2,5,"New Node 5");
//tree.insertNewItem(2,6,"New Node 6");
tree.setOnClickHandler(doOnClick);
function doOnClick(nodeId)
{
var myUrl = tree.getUserData(nodeId,"url")
if(null != myUrl && myUrl !="#")
        {
                window.open(myUrl);
  }

}

</script>


        </body> 
</html> ''')
    html_handler.close()
    return testReport_name

#analysit item and add  item to a DHTML file
def add_item(upperTrunkIndex, NodeIndex, item, report_name):
    show_info =  item.partition("link=")[0]
    link = item.partition("link=")[2]
    verdict_result = ""
    color = "gray"

    try:
        verdict_result = re.search("verdict=(.*?) ", item).group(1)
    except:
        print "Attention!!!!!, %s haven't been added into execution" %item
    if NodeIndex < 100:
        color = "Black"
    elif verdict_result == "" or verdict_result == "NA":
        pass
    elif verdict_result == "PASS":
        color = "green"
    elif verdict_result == "WARNING":
        color = "yellow"
    else:
        color = "red"

    LastTreeNode= os.popen("sed -n '/^tree.insertNewItem\|^tree.setUserData/h;${g;p}' %s" %report_name).read().strip().strip('\n')
    cmd_item = "sed -i '\#%s#a\\tree.insertNewItem(%s,%s,\"%s\");'  %s" %(LastTreeNode, str(upperTrunkIndex), str(NodeIndex), show_info, report_name)  # there can't be any " in item. 
    cmd_link = "sed -i '\#%s#a\\tree.setUserData(%s,\"url\",\"%s\");'  %s" %(LastTreeNode, str(NodeIndex), link, report_name)#link must in "", otherwise, it can't work in DHTML
    cmd_color = "sed -i '\#%s#a\\tree.setItemColor(%s,\"%s\");'  %s" %(LastTreeNode, str(NodeIndex), color, report_name)
    if (100 <= NodeIndex):  #only set singleton name as a link
        os.popen(cmd_link)  #cmd_link should be executed before cmd_item
    os.popen(cmd_color)
    os.popen(cmd_item)

#get how many test case pass or failed for trunks
def get_statistic(f):
    handler = open(f, 'r')
    sd = {} #statistic dict, if element's key is an trunk index, then value is a list such as 1:[10, 4], 10 is pass               #test cases number, 4 is failed or NA test cases number ,if element's key is an node index, 
            #then value always be [1,0]
    sl = handler.readlines()
    length = len(sl)
    for i in range(length):
        l = sl[length-i-1]
        if l.startswith("tree.insertNewItem"):
            up_index = l.partition("(")[2].split(",")[0]
            cur_index = l.partition("(")[2].split(",")[1]
            try:
                verdict = l.partition("(")[2].split(",")[2].split()[1].strip("verdict=")
            except:
                print "Case %s haven't in execution or it is an trunk name, set verdict as NA" %(l.strip("\n"))
                verdict = "NA"

            if not sd.has_key(cur_index):
                sd[cur_index] = [1,0]   # if cur_index occures frist time, then set 1 as its value 
            else:
                pass                # if cur_index already occured as others's up_index, then keep its value unchanged
            if not sd.has_key(up_index): #Make sd[up_index] = sd[up_index] + sd[cur_index] legal
               sd[up_index] = [0,0]

            #if cur_index is a trunk index, then verdict is "NA", but cur_index all statistic(inclu                                #ding pass or fail) should be added into up_index statistic
            if 100 > int(cur_index):    
                sd[up_index][0] = sd[up_index][0] + sd[cur_index][0]
                sd[up_index][1] = sd[up_index][1] + sd[cur_index][1]
            #for cur_index is a node index, we should judge if node is pass or not, 
            #then add corresponding data to up_index
            else:    
                if verdict == "PASS":
                    sd[up_index][0] = sd[up_index][0] + sd[cur_index][0]
                else:
                    sd[up_index][1] = sd[up_index][1] + sd[cur_index][0]
            #print "l is %s" %l
            #print "up_index is %s, cur_index is %s" %(sd[up_index], sd[cur_index]) 
    handler.close()
    return sd

#add info into html to indicate how many test case pass and failed
def set_statistic_toFile(f):
    sd = get_statistic(f)
    handler = open(f, 'r')
    for l in handler.readlines():
        if l.startswith("tree.insertNewItem"):
            cur_index = l.partition("(")[2].split(",")[1]
            if 100 > int(cur_index) and int(cur_index) != 0: 
                cmd = "sed -n '\#^tree.insertNewItem(\w,%s,.*#h;${g;p}' %s" %(cur_index, f)
                item = os.popen(cmd).read();
                oldTrunkName = item.split("\"")[1]
                newTrunkName = item.split("\"")[1] + " PASS:%s, FAIL:%s" %(sd[cur_index][0], sd[cur_index][1])
                new_item = re.sub(oldTrunkName, newTrunkName, item)
                cmd2 = "sed -i 's/%s/%s/'  %s" %(item.strip(";\n"), new_item.strip(";\n"), f) #if there is ;\n
                #then sed will be abnormal.
                os.popen(cmd2) # replace trunk item content in file
    handler.close()             
    
#use every element in struct as a node and add this node to html file. Hierachy follow hierachy of struct
def create_html(filename, struct): #struct is a dictionary here, it is BU test case tree.
    global upperTrunkIndex, NodeIndex, currentTrunkIndex

    if type(struct) == type({}):
        for k in struct.keys():
            add_item(upperTrunkIndex, currentTrunkIndex, k, filename)
            upperTrunkIndex = upperTrunkIndex + 1 #upperTrunkIndex increase for next sub-trunk
            currentTrunkIndex = currentTrunkIndex + 1 # currentTrunkIndex is unique, also should increase for next sub-sub-trunk
            create_html(filename, struct[k])
            upperTrunkIndex = upperTrunkIndex - 1 # upperTrunkIndex decreasE after all singleton element of a struct added finished
            
    if type(struct) == type(()) or type(struct) == type([]):
        for i in struct:
            create_html(filename, i)
            

    if type(struct) == type(""):
        add_item(currentTrunkIndex-1, NodeIndex, struct, filename) # -1 should be there since +1 above after a trunk added before node of this trunk added.
        NodeIndex = NodeIndex + 1 #current_index increase everytime when a new item added.
        
        
            
    
    

                
        
def main():
    tr = get_testResult("testResult.txt", "SOAP_return.txt")
    TR_tree = create_testResult_tree(tr, BU_tree)
    #print TR_tree
    testReportName = createRootHtml()
    create_html(testReportName, TR_tree)
    #print get_statistic(testReportName)
    set_statistic_toFile(testReportName)

if __name__ == "__main__":
    main()
