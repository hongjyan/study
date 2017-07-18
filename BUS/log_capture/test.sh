#!/bin/bash
a="ISU-0"

test()
{
    echo $a
    scp root@$a:/root/game  ./$a.bin
}

test
