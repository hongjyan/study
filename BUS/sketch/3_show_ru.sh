hwcli -q -o off -e '^[^d]' | awk 'NR!=1 {print "/"$1}' | grep -v "LMP-*" | grep -v "SE-*" > /tmp/nnd.tmp;
cat /tmp/nnd.tmp |  xargs hascli --children | awk -F/ 'NF==3' | sort | uniq | while read i; do fshascli -s -a -o -U -R -P $i | sed 's/()/ -/g' |tr '\n():' ' ' | awk '{printf("%-35s%-12s%-10s%-10s%-15s%s\n",$1,$3,$5,$7,$11,$9)}'; done;
