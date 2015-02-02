#! /usr/bin/expect
        
spawn ssh-keygen -t dsa

expect "id_dsa):"
send "\r"

expect "passphrase):"
send "\r"

expect "again:"
send "\r"

expect eof
