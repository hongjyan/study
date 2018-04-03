#!/usr/bin/bash
game='fun'
eclname='game'
eclpath="${!eclname}" #!var, take the value of variable as new variable name 
echo ${eclpath}
echo -e "-----------------------------------\n\n"

#2018/3/13
var="have fun"
echo "= should be used with \"test\" command for POSIX conformance."
echo "STRING1 = STRING2
	the strings are equal"
test "$var" = "have fun" && echo have fun
test "$var" = "have\sfun" && echo have\\sfun
echo -e "-----------------------------------\n\n"


echo "== is not defined by POSIX, it's an extension of bash, derived from ksh, should not use == when want portability. =~ is recomended in bash."
echo "string1 == string2
    True if the strings are equal. When used with the [[ command, this performs pattern matching"
set +x
#character matching
[[ $var == "have fun" ]] && echo var matches "have fun"
bar="hav"
[[ $bar == "have fun" ]] && echo bar match \"have fun\"

#pattern matching
[[ $var == have\sfun ]] && echo have\\sfun #\s\t\n can not use in bash regex, refer to "man 7 regex" to know more. they should be expressed as [[:class:]], class is one of "alnum   digit   punct alpha   graph   space blank   lower   upper cntrl   print   xdigit".
[[ $var == have[[:space:]]+fun ]] && echo == have[[:space:]]+fun #should work, but not.
[[ $var =~ have[[:space:]]+fun ]] && echo =~ have[[:space:]]+fun #work, recommend to use =~ when do matching
[[ $var == have[[:space:]]*fun ]] && echo == have[[:space:]]*fun # * work in ==, but + won't, don't know why.
[[ $var =~ have[[:space:]]*fun ]] && echo =~ have[[:space:]]*fun #ok
[[ $var == have.*fun ]] && echo == have.*fun #. not ok in ==
[[ $var =~ have.*fun ]] && echo =~ have.*fun #. ok in =~
[[ $var == have* ]] && echo == have* #* ok in ==
[[ $var =~ have* ]] && echo =~ have* #* ok in =~

#wrong usage. will be interpreted as "[[ -n have fun==havesfun ]]"
[[ $var==have\sfun ]] && echo  have\\sfun    
set +x
echo -e "-----------------------------------"
#2018/4/3
source common.source
functionOne() {
   return
}
another() {
   return
}
yetANOTHERfunction() {
   return
}
IFS=$'\n'
for f in $(declare -F); do
   echo "${f:11}"
done
func1
echo -e "-----------------------------------"
