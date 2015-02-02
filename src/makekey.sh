#!/bin/bash

genkeystep()
{
	expect <<<EOF
	spawn ssh-keygen -t dsa

	expect "id_dsa):" 
	send "\r"

	expect "Overwrite (y/n)?"
	send "y\r"

	expect "passphrase):"
	send "\r"

	expect "again:" 
	send "\r"

	expect eof
	EOF
}

MYID=`id -u`
NAME=`id -un`
if [ $MYID -eq 0 ] 
then
    HOME=/root
else
    HOME=/home/$NAME
fi
echo "my home is $HOME"
genkey()
{
    echo "making genkey..."
    if [ -e $HOME/.ssh/id_dsa.pub ] ; 
    then 
            rm -rf $HOME/.ssh/id_dsa.pub ; 
            rm -rf $HOME/.ssh/id_dsa ; 
            echo "delete old id_dsa.pub" ; 
    fi 
    ./genkey.sh 1> $HOME/genkey.out
    echo "making genkey...done"
    cat $HOME/genkey.out
    rm -rf $HOME/genkey.out
}
pubkey()
{ 
    cleankey
    echo "make pubkey..."
    if ! [ -d /opt/run/res ] ; 
    then 
        mkdir -p /opt/run/res ; 
    fi
    sudo chmod a+rwx /opt/run/res ; 
    if ! [ -e $HOME/.ssh/id_dsa.pub ] ; 
    then 
       genkey    
    fi
    cat $HOME/.ssh/id_dsa.pub >> /opt/run/res/id_dsa.pub 
    echo "make pubkey...done"
}

cleankey()
{
    echo "clean pub key..."
    if [ -f $HOME/.ssh/id_dsa ] ; 
    then 
        rm -rf $HOME/.ssh/id_dsa ; 
    fi
    if [ -f $HOME/.ssh/id_dsa.pub ] ; 
    then 
        rm -rf $HOME/.ssh/id_dsa.pub ; 
    fi
    echo "clean pub key...done"
}

pubkey
