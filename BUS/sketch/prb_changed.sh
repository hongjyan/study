
paths=(`rpm -qal | grep "^/opt/nokiasiemens/SS*" | grep "bin$"`)
for path in ${paths[@]}
do
    if ls $path | grep -q .old ;then
	ls $files 
    fi
done
