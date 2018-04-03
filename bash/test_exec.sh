#!/usr/bin/bash

exec ls
echo "will come here"
exec 200>"execfile"
{
    date
    ls -l ~
    exec 200>&-
}
exec notexist  #if command doesn't exist, the current shell, not only the exec command, exits in error
echo "won't come here"
