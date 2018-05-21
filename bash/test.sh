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

#2018/4/3. get all functions from script then execute one by one.
source common.source
functionOne() {
   echo "i am functionOne"
   return
}
another() {
   echo "i am another"
   return
}
yetANOTHERfunction() {
   echo "i am yetANOTHERfunction"
   return
}
print_arg() {
    for arg; do
        echo -e $arg
    done
}
IFS=$'\n'
declare -a funcs
for f in $(declare -F); do
   declare -f pTemp
   pTemp="${f:11}"
   funcs+=($pTemp)
#   echo "${f:11}"
done
for p in ${funcs[*]}; do
    $p $@
done
echo -e "-----------------------------------"

#2018/4/3. Test if callee function can shift caller's argument
callee() {
    for arg;
        do printf "%s " $arg
    done
    printf "\n"
    shift
}
caller() {
    callee $@
    for arg;
        do printf "%s " $arg
    done
    printf "\n"
}
caller 1 2 3 4
echo -e "-----------------------------------"

#2018/4/8. How to pass an associative array as parameter to a function
print_array() {
    eval declare -A func_assoc_array=${1#*=} #eval is mandatory, otherwise the content after = would be taken as literal string. eval differs but root cause still vague for me. 
    declare -p func_assoc_array
}

declare -A assoc_array=([key1]="value1" [key2]="value2")
#proof that array was successfully created
declare -p assoc_array
#customer code to iterate associative array
for key in ${!assoc_array[*]}; do
    echo $key=${assoc_array[$key]}
done

print_array "$(declare -p assoc_array)"
echo -e "-----------------------------------"
#2018/4/27
funcname() {
    echo ${FUNCNAME[0]}
    echo $0
}
funcname
echo -e "-----------------------------------"
echo $(pwd)
echo $(dirname $0)
echo -e "-----------------------------------"
#2018/5/17
#The content in [[ ]] is taken as string.
# [[ echo game | grep game ]] && echo found #error
[[ `echo game | grep game` ]] && echo found 1 #The output of command take as arg in [[ `cmd` ]], so [ -n game] && echo found
[[ `echo game | grep notexist` ]] && echo found 2 #when command execute failed, return value is an empty string. [[ -n '' ]] && echo found
# $(cmd) is the same as `cmd`
[[ $(echo game | grep game) ]] && echo found 3
#if take the return code of command, not output of command
if echo game | grep -q game;then echo found 4; fi
#take the output of command, then take is as command to execute, so shell will try to run "game" which is the output of cmd.
#if `echo game | grep game`; then echo found; fi
#if $(echo game | grep game); then echo found; fi
echo -e "-----------------------------------"
