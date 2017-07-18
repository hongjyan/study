#!/bin/bash

#LOG_PATH=`date "+%y-%m-%d-%H-%M"_log`
LOG_PATH="tmp_log"
PID_PATH="pid"
BOND1="bond1"
FI="fi0"

get_available_nodes()
{
    local ip_nodes=(`hascli -s /$1-* | xargs | awk '{gsub(/\//,"\n",$0); print $0}' | grep UNLOCKED | grep ENABLED | awk -F ":" '{print $1}'`)
    echo ${ip_nodes[*]}
}

ip_log_start()
{
    echo "ip log start .."
    local ip_nodes=(`get_available_nodes "IPNI1P"`)
    if [ ! -z $ip_nodes ]
    then
	for nd in ${ip_nodes[*]}
	do
	    echo "--> capture all packets of sfp1@$nd <--"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i eth_sfp1 -xx -nn -s 0 -w $nd@eth_sfp1.cap" &
	    #echo "--> capture FLIP packets of $FI@$nd <--"
    	    #ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i $FI ether proto 0x8901 -nn -xx -s 0 -w $nd@$FI.cap" &
	    echo "--> capture FLIP packets of $BOND1@$nd <--"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump  -n -e -vv -ttt -i $BOND1 ether proto 0x8901 -w $nd@$BOND1.cap" &
    	    echo "--> MOnster messages on $nd <--"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "monster -f 0a25 > ipn_msg_mon.log" &
	done
    fi
}

tdm_log_start()
{
    echo "tdm log start .."
    local tdm_nodes=(`get_available_nodes "TDMNIP"`)
    echo $tdm_nodes
    if [ ! -z $tdm_nodes ]
    then
	for nd in ${tdm_nodes[*]}
	do
	    #echo "--> capture FLIP packets of $FI@$nd <--"
	    #ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i $FI ether proto 0x8901 -nn -xx -s 0 -w $nd@$FI.cap" &
	    echo "--> capture FLIP packets of $BOND1@$nd <--"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i $BOND1 ether proto 0x8901 -nn -xx -s 0 -w $nd@$BOND1.cap" &
	    echo "--> Monster messages on TDMNIP-0 <--"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "monster -f 0a25 > ${nd}_msg_mon.log &" &
	done
    fi
}

tcu_log_start()
{
    echo "tcu log start .."
    local tcu_nodes=(`get_available_nodes "TCU"`)
    if [ ! -z $tcu_nodes ]
    then
	for nd in ${tcu_nodes[*]}
	do
	    #echo "--> capture FLIP packets of $FI@$nd/1 <--"
    	    #ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i $FI ether proto 0x8901 -nn -xx -s 0 -w $nd@$FI.cap" &
	    echo "--> capture FLIP packets of $BOND1@$nd/1 <--"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -i $BOND1 ether proto 0x8901 -nn -xx -s 0 -w $nd@$BOND1.cap" &
    	    echo "--> Get all the dsp cores' status <--"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "node status all" > ./$LOG_PATH/${nd}_node_status_all.txt
    	    echo "--> Get all the dsp statistic <--"
    	    ssh  -q -o "StrictHostKeyChecking no" $nd "/opt/nokiasiemens/SS_MGWDSPUtils/bin/linx_rcmd 0x00130005_0 \"stat -r\"" > ./$LOG_PATH/DSP_Statistic_before_call_${nd}.txt
	done
    fi
}

common_log_start()
{
    echo "--> Monster CRM messages in log mode <--"
#    monster -f 0744,05f1,04b4,05fa,05ec,0646,0656,05f3,05e4 > ./$LOG_PATH/crm_msg_bin_macro.txt &
#    echo $! > ./$PID_PATH/debug_log.pid

    echo "--> monster @CLA-0 <--"
    monster -f 0744,05f1,04b4,05fa,05ec,0646,0656,05f3,05e4,0683,0612,04a8,05fb,063a,06b0,0909,0a21,048e,05f2,0175,0574,09d0,09ce,0a26,0a27,09cf,00db,0680,06b1,041d,0399,0310,06a9 -b ./$LOG_PATH/CLA-0.bin &
    echo $! > ./$PID_PATH/debug_bin_log.pid

    echo "--> monster @CLA-1 <--"
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "rm -rf *.bin"
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "monster -f 0744,05f1,04b4,05fa,05ec,0646,0656,05f3,05e4,0683,0612,04a8,05fb,063a,06b0,0909,0a21,048e,05f2,0175,0574,09d0,09ce,0a26,0a27,09cf,00db,0680,06b1,041d,0399,0310,06a9 -b ./CLA-1.bin " &


    local isu_nodes=(`get_available_nodes "ISU"`)
    if [ ! -z $isu_nodes ]
    then
        for nd in ${isu_nodes[*]}
        do
            echo "--> monster @$nd <--"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf $nd.bin"
	    #echo $?
            ssh  -q -o "StrictHostKeyChecking no" $nd "monster -f 0744,05f1,04b4,05fa,05ec,0646,0656,05f3,05e4,0683,0612,04a8,05fb,063a,06b0,0909,0a21,048e,05f2,0175,0574,09d0,09ce,0a26,0a27,09cf,00db,0680,06b1,041d,0399,0310,06a9 -b ./$nd.bin " &
        done
    fi

    
    echo "--> Tail debug level syslog <--"
    tail -f /srv/Log/log/debug  > ./$LOG_PATH/debug.txt &
    echo $! > ./$PID_PATH/msg_txt.pid


    echo "-->capture h248 message @ fi0<--"
    tcpdump -n -e -vv -ttt -i fi0 -w CLA-0-fi0.cap &
    echo $! > ./$PID_PATH/h248_mon_0.pid


    echo "-->capture h248 message @ fi1<--"
    tcpdump -n -e -vv -ttt -i fi1 -w CLA-0-fi1.cap &
    echo $! > ./$PID_PATH/h248_mon_1.pid

    echo "--> capture h248 message on CLA-1 <--"
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "rm -rf CLA-1-fi*.cap"
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "tcpdump -n -e -vv -ttt -i fi0 -w CLA-1-fi0.cap " &
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "tcpdump -n -e -vv -ttt -i fi1 -w CLA-1-fi1.cap " &
 

    local isu_nodes=(`get_available_nodes "ISU"`)
    if [ ! -z $isu_nodes ]
    then
        for nd in ${isu_nodes[*]}
        do
            echo "--> capture h248 message on  @$nd <--"
            ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf $nd-fi*.cap"
            ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -n -e -vv -ttt -i fi0 -w $nd-fi0.cap " &
            ssh  -q -o "StrictHostKeyChecking no" $nd "tcpdump -n -e -vv -ttt -i fi1 -w $nd-fi1.cap " &
        done
    fi    

}

tips()
{
    echo "usage: sh call_log.sh all"
    echo "	 sh call_log.sh ip"
    echo "	 sh call_log.sh tdm"
    echo "	 sh call_log.sh none"
}

main()
{
    mkdir ${LOG_PATH}
    mkdir ${PID_PATH}
    case $1 in
    ip)
	common_log_start
	ip_log_start
	tcu_log_start
    ;;
    tdm)
	common_log_start
	tdm_log_start
	tcu_log_start
    ;;
    all)
	common_log_start
	ip_log_start
	tdm_log_start
	tcu_log_start
    ;;
    none)
	common_log_start
	tcu_log_start
    ;;
    *)
	tips
    	rm -rf ${LOG_PATH}
    	rm -rf ${PID_PATH}
    ;;
    esac
}

main $@

