source conf

if [ "$1" = "WK" ]
then
	AGE=$WK_AGE
elif [ "$1" = "BJ" ]
then
	AGE=$BJ_AGE
elif [ "$1" = "SS" ]
then
	AGE=$SS_AGE
else
	echo "No parameter given"
fi

echo $AGE
echo $EAPON  #这个没有输出,请模仿AGE,添加,让它成功显示


#唐僧没有加�,请添加,使得输入参数TS的时候能够正确显示出来
