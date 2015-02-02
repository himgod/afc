#!/bin/bash

afi=$1
cmd="chmod +x /sbin/upgrade-monitor; /etc/init.d/cron start; chmod +x /sbin/start-upgrade; /sbin/start-upgrade";
echo -e "upgrade AFi $afi\n"
sshpass -p autelan611 ssh  -o stricthostkeychecking=no -l root $afi $cmd

