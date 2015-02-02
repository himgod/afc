#!/usr/bin/php
<?php

if($argc > 1 && isset($argv[1]))
{
	$outputfile=$argv[1];
}
else
{
	$outputfile = "/opt/run/wcpss/afi.iplist";
}

echo "output file is set to $outputfile\n";

if($argv[0] == $outputfile)
{
	echo "Bad input parameter!\n";
	return;
}
system(">$outputfile");
$ret=ext_wtp("get_wtp_ip_list");
if(isset($ret))
{   
    if(isset($ret->wtps["wtp_num"]) && $ret->wtps["wtp_num"] > 0)
    {   
        $i = 0;
        for($i = 0; $i < ($ret->wtps["wtp_num"]); $i++)
        {   
            echo "wtp".$ret->value[$i]["_id"].": ";
            $ip = $ret->value[$i]["ip_port"];
            echo $ip."\n";
            if(!isset($flag[$ip]) || (1 != $flag[$ip]))
            {   
                $cmd = "echo $ip>>$outputfile";
                system($cmd);
				echo "append \"$ip\" to $outputfile\n";
                $flag[$ip]=1;
            }
			else
			{
				echo "\"$ip\" already exists ignore it!\n";
			}
        }
    }

}
else
{
	echo "ret not set";
}

?>
