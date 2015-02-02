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

genkey()
{
    echo "making genkey..."
    if [ -e ~/.ssh/id_dsa.pub ] ; 
        then 
            rm -rf ~/.ssh/id_dsa.pub ; 
            rm -rf ~/.ssh/id_dsa ; 
            echo "delete old id_dsa.pub" ; 
        fi 
        genkeystep 1>./genkey.out
    echo "making genkey...done"
	cat ./genkey.out
	rm -rf ./genkey.out
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
    if ! [ -e ~/.ssh/id_dsa.pub ] ; 
    then 
       genkey    
    fi
    cat ~/.ssh/id_dsa.pub > /opt/run/res/id_dsa.pub 
    echo "make pubkey...done"
}

cleankey()
{
    echo "clean pub key..."
    if [ -f ~/.ssh/id_dsa ] ; 
    then 
        rm -rf ~/.ssh/id_dsa ; 
    fi
    if [ -f ~/.ssh/id_dsa.pub ] ; 
    then 
        rm -rf ~/.ssh/id_dsa.pub ; 
    fi
    echo "clean pub key...done"
}

pubkey
