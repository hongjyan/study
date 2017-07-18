#!/usr/bin/expect -f 
spawn fsclish 
expect ">" 

send "shell hardware fabric-config switch-id Switch-1-8\r" 
expect "Fabric#" 

send "exit\r" 
exit 
