#!/bin/bash

afi=$1
switch=$2
cmd="led_control.sh detect $switch";
echo -e "set AFi $afi led $switch\n"
sshpass -p autelan611 ssh  -o stricthostkeychecking=no -l root $afi $cmd

