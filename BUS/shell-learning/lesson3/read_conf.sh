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
echo $EAPON  #Õâ¸öÃ»ÓÐÊä³ö,ÇëÄ£·ÂAGE,Ìí¼Ó,ÈÃËü³É¹¦ÏÔÊ¾


#ÌÆÉ®Ã»ÓÐ¼ÓøÀ´,ÇëÌí¼Ó,Ê¹µÃÊäÈë²ÎÊýTSµÄÊ±ºòÄÜ¹»ÕýÈ·ÏÔÊ¾³öÀ´
