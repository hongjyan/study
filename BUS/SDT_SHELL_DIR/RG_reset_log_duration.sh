#!/bin/bash
#-----------------------------------------------------------------------------------------
# filename : RG startup Time measurement
#
# Initial version : Zhou, Tong (NSN - CN/Shanghai); Huang, Yikun (EXT-Other - CN/Hangzhou)
# Lite version : Zhu, Jie (EXT-School - CN/Hangzhou)
#-----------------------------------------------------------------------------------------

env_init()
{
  RU_TIME_COUNTER=0
  RU_TIME_SUM=0
}

tips()
{
  echo "Usage: sh monkey.sh --total-restart <MO Name>    //mesure the node startup time"
  echo "       sh monkey.sh --restart <MO Name>   	//measure the hot standby recovery unit startup time"
  echo "       sh monkey.sh --switch-over <MO Name>   	//measure the hot standby recovery unit swich over time"
  echo "       sh monkey.sh --help -h <MO Name>	//show this message"
  echo "current support RUs are:"
  #echo "OMUFU"
  #echo "HCLBFU"
  #echo "SGUNNSFFU"
  #echo "MGW_TDMSNIUPFU"
}

show_time()
{
   s_hh=`cat ${1}_start_time | awk '{print $3}' | awk -F "[:.]" '{print $1}'`
   s_mm=`cat ${1}_start_time | awk '{print $3}' | awk -F "[:.]" '{print $2}'`
   s_ss=`cat ${1}_start_time | awk '{print $3}' | awk -F "[:.]" '{print $3}'`
   s_uuuuuu=`cat ${1}_start_time | awk '{print $3}' | awk -F "[:.]" '{print $4}'`
   s_h=$((10#$s_hh))
   s_m=$((10#$s_mm))
   s_s=$((10#$s_ss))
   s_u=$((10#$s_uuuuuu))
   s_time=$(($s_h*3600*1000000 + $s_m*60*1000000 + $s_s*1000000 + $s_u))
   
   e0_hh=`cat ${2}_0_end_time | awk '{print $3}' | awk -F "[:.]" '{print $1}'`
   e0_mm=`cat ${2}_0_end_time | awk '{print $3}' | awk -F "[:.]" '{print $2}'`
   e0_ss=`cat ${2}_0_end_time | awk '{print $3}' | awk -F "[:.]" '{print $3}'`
   e0_uuuuuu=`cat ${2}_0_end_time | awk '{print $3}' | awk -F "[:.]" '{print $4}'`
   e0_h=$((10#$e0_hh))
   e0_m=$((10#$e0_mm))
   e0_s=$((10#$e0_ss))
   e0_u=$((10#$e0_uuuuuu))
   e0_time=$(($e0_h*3600*1000000 + $e0_m*60*1000000 + $e0_s*1000000 + $e0_u))
   
   e1_hh=`cat ${2}_1_end_time | awk '{print $3}' | awk -F "[:.]" '{print $1}'`
   e1_mm=`cat ${2}_1_end_time | awk '{print $3}' | awk -F "[:.]" '{print $2}'`
   e1_ss=`cat ${2}_1_end_time | awk '{print $3}' | awk -F "[:.]" '{print $3}'`
   e1_uuuuuu=`cat ${2}_1_end_time | awk '{print $3}' | awk -F "[:.]" '{print $4}'`
   e1_h=$((10#$e1_hh))
   e1_m=$((10#$e1_mm))
   e1_s=$((10#$e1_ss))
   e1_u=$((10#$e1_uuuuuu))
   e1_time=$(($e1_h*3600*1000000 + $e1_m*60*1000000 + $e1_s*1000000 + $e1_u))

   if [ $e0_time -lt $e1_time ]
   then
     e_time=$e0_time
   else
     e_time=$e1_time
   fi
   
   duration=$(($e_time - $s_time))
   d_m=$(($(($duration/60))/1000000))
   d_s=$(($(($duration - $d_m*60*1000000))/1000000))
   d_u=$(($(($duration - $d_m*60*1000000))-$d_s*1000000))

   echo "`date` Spended time of $1 is: $d_m mins, $d_s sec, $d_u usec($duration us)" | tee -a MO_operation_time_statistics.log
}

MO_operation()
{
  if [ ! -z $2 ]
  then
    local MO=$2
    local MO_name=`echo $MO | awk -F "[_]" '{print $2}'`
    if [ -z $MO_name ]
    then
      MO_name=`echo $MO | awk -F "[/]" '{print $2}'`
    fi
    echo $MO_name
    #local MO_name=`echo "$MO" | awk -F "[/_]" '{print $4}'`
    local MO_children_0=`fshascli --children $MO | awk -F "[/]" '{print "/" $2 "/" $3}' | sort -u | head -n 1`
    echo $MO_children_0
    local MO_children_1=`fshascli --children $MO | awk -F "[/]" '{print "/" $2 "/" $3}' | sort -u | tail -n 1`
    echo $MO_children_1
  else
    exit 21
  fi
  
  case $1 in
    "restart")
      local cmd="fshascli --$1 -n"
      local start_msg="INFO Resetting all RUs of the RG. RG=$2"
      local end_msg_0="INFO Startup of $MO_children_0 is complete"
      local end_msg_1="INFO Startup of $MO_children_1 is complete"
      local timeout=240
      ;;
    "switchover")
      local cmd="fshascli --$1 -n"
      local start_msg="A successful Recovery unit '$1' operation done by operator"
      local end_msg=""
      local timeout=5
      ;;
    *)
      echo "operation is incorrect, please refer to verbose fshascli --help"
      exit 20
      ;;
  esac
  
  
  
  tail -f /var/log/master-syslog > duck.log &
  # echo "$cmd $MO is performed"
  cmd_output=`$cmd $MO`
  echo -n $cmd_output
  if [[ ! $cmd_output =~ "successful" ]];
  then
    echo "$MO_name restart failed! Please check."
    killall tail >/dev/null 2>/dev/null
    killall grep >/dev/null 2>/dev/null
    exit 4
  fi
  
  
  echo "The RUs below will be measured their startup time:"
  echo $MO
  
  local MO_up_tag="false"
  
  until [ "$MO_up_tag" == "true" ] #[ $timeout -le 0 ] || [ "$MO_up_tag" == "true" ]
  do
    cat duck.log | grep "$start_msg" > ${MO_name}_start_time
    cat duck.log | grep "$end_msg_0" > ${MO_name}_0_end_time
    cat duck.log | grep "$end_msg_1" > ${MO_name}_1_end_time
    
    if [ -s ${MO_name}_0_end_time -a -s ${MO_name}_1_end_time ]
    then
      MO_up_tag="true"
      echo $MO_up_tag
    fi
    
    let timeout--
    echo -n "."
    sleep 1
  done
  
  if [ ${timeout} -le 0 ]
  then
    echo ""
    echo ""
    echo "Too long startup time exceed to 1 minutes, please check related RU by yourself!"
    exit 21
  fi
  
  echo ""
  echo ""
  show_time $MO_name $MO_name
 
}
main()
{

  env_init
  
  if [ $# -eq 0 ]
  then 
    tips
    exit 1
  fi
  
  sleep 1
  rm -f duck.log

  case $1 in
    #"--total-restart")
    #  MO_operation restart $2
    #  ;;
    "--restart")
      MO_operation restart $2
      #local cmd="fshascli -rn"
      #local sc="A successful Recovery unit 'restart' operation done by operator"
      #local ec="Startup of "
      #local ec="[Startup of * is complete]"
      #ru_measure $2 "$cmd" "$sc" "$ec"
      ;;
    #"--switch-over")
      #ru_swo $2
      #local cmd="fshascli -wn"
      #local sc="A successful Recovery unit 'switchover' operation done by operator" 
      #local ec="hot switchover marked complete"
    #  MO_operation switchover $2 #"$cmd" "$sc" "$ec"
    #  ;;
    *)
      tips
      exit 1
      ;;
  esac

  #handle with process
  killall -9 tail >/dev/null  #1>/dev/null 2>/dev/null
  killall -9 grep 1>/dev/null 2>/dev/null


  #echo ""
  #echo "---------------------------------------------"
  #echo "Debug Startup time lines:"
  #cat *_start_time.txt
  #cat *_end_time.txt
  rm -f *_time  
  rm -f duck.log 
}

main $@
