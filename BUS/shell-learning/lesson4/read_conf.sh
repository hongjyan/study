source conf

case $1 in
	WK)
		AGE=$WK_AGE
		;;
	BJ)
		AGE=$BJ_AGE
		;;
	SS)
		AGE=$SS_AGE
		;;
	*)
		echo 'parameter needs'
		;;
esac



echo $AGE
echo $EAPON  #make it works by following the example of AGE


#make this script accept paramaters of TS, which are vars in conf file


