#!/bin/bash

logcrm="debug.txt"
log_path="tmp_log"

get_available_nodes()
{
    local nodes=(`hascli -s /$1-* | xargs | awk '{gsub(/\//,"\n",$0); print $0}' | grep UNLOCKED | grep ENABLED | awk -F ":" '{print $1}'`)
    echo ${nodes[*]}
}


get_nodes_rpm_version()
{
    echo "get $1 rpm info detail"
    nodes=(`get_available_nodes "$1"`)
    echo $nodes
    if [ ! -z $nodes ]
    then
        for nd in ${nodes[*]}
        do
	    #ssh  -q -o "StrictHostKeyChecking no" $nd "rpm -qa" > rpm_info_${nd}.txt
        ssh  -q -o "StrictHostKeyChecking no" $nd "rpm -qa" > ./$LOG_PATH/rpm_info_${nd}.txt
        done
    fi
}

check_tcu_info()
{
    tcu_nodes=(`get_available_nodes "TCU"`)
    if [ ! -z $tcu_nodes ]
    then
        for nd in ${tcu_nodes[*]}
        do
	    echo "Check DSP LYNX Connectivity"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "linxls" > ./$LOG_PATH/DSP_LYNX_Connectivity_${nd}.txt

	    echo "Check DSP Signaling Link Table"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "/opt/nokiasiemens/SS_MGWDSPUtils/bin/linx_send_msg --name=\"0x00130005_0/sgw\" --signo=0x00021009 --timeout=100 --data=0001" > ./$LOG_PATH/DSP_Singaling_link_table_${nd}.txt

	    echo "Check DSP statistic after call!!!!!!Notice: Not Done yet!!!"
    	    #ssh  -q -o "StrictHostKeyChecking no" $nd "/opt/nokiasiemens/SS_MGWDSPUtils/bin/linx_rcmd 0x00130005_0 \"stat -r \" " > ./$LOG_PATH/DSP_statistic_after_call_${nd}.txt


	    echo "DSP node dump"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "node dump -w 0x100 /tmp/dsp_text_log.txt 0x00130005; cat /tmp/dsp_text_log.txt" > ./$LOG_PATH/DSP_text_log_${nd}.txt
        done
    fi
}


#echo "get IPNI1P rpm version detail"
#ip_nodes=(`get_available_nodes "IPNI1P"`)
#echo $ip_nodes
#if [ ! -z $ip_nodes ]
#then
#    for nd in ${ip_nodes[*]}
#    do
#	ssh  -q -o "StrictHostKeyChecking no" $nd "rpm -qa" > SS_MGW_version_${nd}.txt
#    done
#fi
#
#echo "get TCU rpm version detail"
#tcu_nodes=(`get_available_nodes "TCU"`)
#echo ${tcu_nodes[*]}
#if [ ! -z $tcu_nodes ]
#then
#    for nd in ${tcu_nodes[*]}
#    do
#	ssh  -q -o "StrictHostKeyChecking no" $nd "rpm -qa" > SS_MGW_version_${nd}.txt
#    done
#fi
#
#echo "get TDMNIP rpm version detail"
#tdm_nodes=(`get_available_nodes "TDMNIP"`)
#
#if [ ! -z $tdm_nodes ]
#then
#    for nd in ${tdm_nodes[*]}
#    do
#	ssh  -q -o "StrictHostKeyChecking no" $nd "rpm -qa" > SS_MGW_version_${nd}.txt
#    done
#fi


grep_logs()
{
    echo "Grepping $logcrm from $log_path..."
    
    echo "Grepping hta log"
    cat $log_path/$logcrm | grep -i "hta" > ./$LOG_PATH/hta.txt
    perl tr_log.pl -f hta.txt
    
    echo "Grepping vmgHan log"
    cat $log_path/$logcrm |grep -i vmg > ./$LOG_PATH/VmgwHan.txt
    perl tr_log.pl -f VmgwHan.txt
    
    echo "Grepping bc2 log"
    cat $log_path/$logcrm |grep -i bc2 > ./$LOG_PATH/bc2.txt
    perl tr_log.pl -f ./$LOG_PATH/bc2.txt
    cat bc2.txt |grep -i E- > ./$LOG_PATH/bc2_E.txt 
    perl tr_log.pl -f ./$LOG_PATH/bc2_E.txt

    var="TDMPhlMgr"
    echo "Grepping $var log"
    cat $log_path/$logcrm |grep -i $var > ./$LOG_PATH/${var}.txt
    perl tr_log.pl -f ${var}.txt
    cat ${var}.txt |grep -i E- > ./$LOG_PATH/${var}_E.txt
    perl tr_log.pl -f  ./$LOG_PATH/${var}_E.txt

    var="TDMConnMgr"
    echo "Grepping $var log"
    cat $log_path/$logcrm |grep -i $var > ./$LOG_PATH/${var}.txt
    perl tr_log.pl -f ${var}.txt
    cat ${var}.txt |grep -i E- > ./$LOG_PATH/${var}_E.txt
    perl tr_log.pl -f ./$LOG_PATH/${var}_E.txt
    
    echo "Grepping rm2 log"
    cat $log_path/$logcrm |grep -i rm2 > ./$LOG_PATH/rm2.txt
    perl tr_log.pl -f rm2.txt
    
    echo "Grepping lemana log"
    cat $log_path/$logcrm |grep -i lem > ./$LOG_PATH/lemana.txt
    perl tr_log.pl -f lemana.txt
    
    echo "Grepping libgen log"
    cat $log_path/$logcrm |grep -i libgen > ./$LOG_PATH/libgen.txt
    perl tr_log.pl -f libgen.txt
    
    echo "Greppiing ipconnmgr log"
    cat $log_path/$logcrm |grep -i  IPConnMgr > ./$LOG_PATH/ipconnmgr.txt
    perl tr_log.pl -f ipconnmgr.txt
    
    #echo "Grepping cilprb log"
    #cat $log_path/$logcrm |grep -i cil >cilprb.txt
    #perl tr_log.pl -f cilprb.txt
    #
    #echo "Grepping as2prb log"
    #cat $log_path/$logcrm |grep -i as2 >as2prb.txt
    #perl tr_log.pl -f as2prb.txt
    #
    #echo "Grepping cmqprb log"
    #cat $log_path/$logcrm |grep -i cmq >cmqprb.txt
    #perl tr_log.pl -f cmqprb.txt
    #
    #
    #echo "Grepping tg3cal log"
    #cat $log_path/$logcrm |grep -i tg3 >tg3cal.txt
    #perl tr_log.pl -f tg3cal.txt
    #
    #
    #echo "Grepping anu log"
    #cat $log_path/$logcrm |grep -i anu >anulib.txt
    #perl tr_log.pl -f anulib.txt
    #
    #
    #echo "Grepping uha log"
    #cat $log_path/$logcrm |grep -i uha >uhaprb.txt
    #perl tr_log.pl -f uhaprb.txt
}

generate_env_info()
{
    echo "Generate env info"
    fsswcli -c > ./$LOG_PATH/build_version.txt
    fsclish -c "show functional-unit unit-info" > ./$LOG_PATH/function_unit_info.txt
    rpm -qa > ./$LOG_PATH/SS_MGW_version_x86.txt
}

main()
{
    grep_logs    
    generate_env_info

    echo "collecting DSP info ..."
    check_tcu_info

   # get_nodes_rpm_version "TCU"
   # 
   # case $1 in
   # ip)
   #     get_nodes_rpm_version "IPNI1P"
   # ;;
   # tdm)
   #     get_nodes_rpm_version "TDMNIP"
   # ;;
   # all)
   #     get_nodes_rpm_version "IPNI1P"
   #     get_nodes_rpm_version "TDMNIP"
   # ;;
   # none)
   # ;;
   # *)
   #     echo "unkown errors!"
   # ;;
   # esac

    echo "generate crm_log.tar"
    tar -cvf $1_crm_log.tar ./*.txt ./*.cap ./*.bin ./$log_path/*
    echo gzip $1_crm_log.tar
    gzip -9 -f $1_crm_log.tar
    cat lemana.txt |grep mac
    date=`date "+%y-%m-%d-%H-%M"`
    mkdir $date
    cp ./*.txt ./*.cap ./*tar.gz ./*.log $log_path/*.log $log_path/*.bin $log_path/*.txt ./$date
    echo -e "Directory \033[1;31m$date\033[0m created.Done"
    
    echo "Clear Temp Log"
    ./log_clean.sh $1
}

main $@
