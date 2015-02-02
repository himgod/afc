#!/bin/bash

ins=`ps -ef | grep wid | grep -v grep | wc -l`
echo -e "current wireless daemon instance $ins \n"

if [ $ins -gt 0 ];
then
  #pids=`ps |grep wtpd |grep -v grep | awk '{print $1}'`
  #echo -e "all wtpd pids:\c"
  #echo -e "$pids \n"
  #npids=`echo $pids |wc -w` 
 
  #echo -e "kill off each pid\n"

  #i=1 
  #while [ $i -le $npids ];
  #do 
  #  echo -e "round $i:\c"
  #  pid=`echo $pids | awk '{print $1}'`
  #  pids=`echo $pids | awk -F"$pid" '{print $2}'`
  #  echo -e "$pid\n"
  #  kill -9 $pid
  #  i=$(($i+1))
  #done
  sudo killall wid
else 
   echo -e "nothing need be done.\n"   
fi

ulimit -c unlimited 
echo -e "start new wireless daemon instance.\n"
/opt/bin/wid 0 1 &
sleep 1
echo -e "load wireless config.\n"
php /opt/run/wcpss/load_wireless.php

