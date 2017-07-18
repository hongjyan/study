 #!/bin/bash
#-------------------------------------------------------
# filename : SDT MO operation avg time logger
#
# Initial version : Zhu, Jie (EXT-School - CN/Hnagzhou)
#-------------------------------------------------------
# example sh avg_time_loger.sh  [time statistic log file] \\[FU name]

if [ ! -e $1 ]
then
  echo "No such a time statistic log file, please exec monkey_new.sh"
  exit 1
fi

#local FU=$1
logfile=$1
loop_limit=20

FU_name=`cat $logfile | tail -n 1 | awk '{print $10}' | awk -F "[-]" '{print $1}'`
#echo "$FU_name"
FU_name_index=`cat $logfile | tail -n 1 | awk '{print $10}'`
#echo "$FU_name_index"
FU_operation_count=`cat $logfile | tail -n $loop_limit | grep $FU_name | wc -l`
#echo "$FU_operation_count"
FU_operation_duration=`cat $logfile | tail -n $FU_operation_count | grep $FU_name | awk -F "[(]" '{print $2}' | awk '{print $1}' | awk '{sum+=$1} END {print sum}'`
#echo "$FU_operation_duration"
FU_operation_avg_duration=$(($(($FU_operation_duration/$FU_operation_count))))
#echo "$FU_operation_avg_duration"

d_mm=$(($(($FU_operation_avg_duration/60))/1000000))
d_ss=$(($(($FU_operation_avg_duration - $d_mm*60*1000000))/1000000))
d_uu=$(($(($FU_operation_avg_duration - $d_mm*60*1000000)) - $d_ss*1000000))
echo "`date` Average time of $FU_name is: $d_mm mins, $d_ss sec, $d_uu usec, $FU_operation_count times included" | tee -a MO_operation_avg_time.log

#if [ RU_TIME_COUNTER -eq 0 -o RU_TIME_SUM -eq 0]
#then
#  echo "Time statistic logical error, please execute time statistic sh first!"
#  exit 1
#fi

#


  