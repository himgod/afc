<?php
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
$model= "";
$mac  = "";
$sn   = "";
$key  = "";
$trigger = "";
$ip = "";

function ret_for_pubkey($keyinfo)
{
	$ret_pubkey = "AC got info \"".$keyinfo."\"!\n";
	return $ret_pubkey;
}
if(isset($_REQUEST["product"]))
{
	$model = $_REQUEST["product"];
}

if(isset($_REQUEST["mac"]))
{
	$mac = $_REQUEST["mac"];
	$mac = strtolower($mac);
}

if(isset($_REQUEST["myip"]))
{
	$ip = $_REQUEST["myip"];
}

if(isset($_REQUEST["sn"]))
{
	$sn = $_REQUEST["sn"];
}

if(isset($_REQUEST["key"]))
{
	$key = $_REQUEST["key"];
}

if(isset($_REQUEST["trigger"]))
{
	$trigger = $_REQUEST["trigger"];
}
if($key == "")
{
	echo "AC got information model \"".$model."\" mac \"".$mac."\" ip \"".$ip."\" sn \"".$sn."\" key \"".$key."\" trigger \"".$trigger."\"\n";
}
else if($key == "workos_sysupgrade" || $key == "minios_sysupgrade")
{
	$versioninfo = get_ap_version_info();
	$version = $versioninfo["version"];
	$imgFile = $versioninfo["imgFile"];
	$imgUrl = "http://".$_SERVER['SERVER_NAME'].":".$_SERVER['SERVER_PORT']."/img/".$version."/".$imgFile."\n";
	if($trigger != "" && strncmp($trigger, "Admin:", 6))
	{
		$trigger = "by ".$trigger;
	}
	$ret = ext_active("add_upgrade_active_to_db", $mac, $ip, $trigger, $version, $key);
	echo $imgUrl;
}
else if($key == "get_pubkey_fail")
{
	$ret = ext_active("ap_down_pubkey_step_info", $mac, $ip, $key);
	echo ret_for_pubkey($key);
}
else if($key == "save_pubkey_fail")
{
	$ret = ext_active("ap_down_pubkey_step_info", $mac, $ip, $key);
	echo ret_for_pubkey($key);
}
else if($key == "get_save_pubkey_success")
{
	$ret = ext_active("ap_down_pubkey_step_info", $mac, $ip, $key);
	echo ret_for_pubkey($key);
//	echo $_SERVER["REMOTE_ADDR"]."\n";
}
else
{	
	/*sysupgrade_start_successfull  //after upgrade and reboot
	sysupgrede_successfull_from_minios*/
	$versioninfo = get_ap_version_info();
	$version = $versioninfo["version"];
	//$imgFile = $versioninfo["imgFile"];
	if(($key == "sysupgrade_start_successfull" || $key == "sysupgrede_successfull_from_minios"))
	{
		if(isset($_REQUEST["version"]))
		{
			$apversion = $_REQUEST["version"];
			if($version != "v".$apversion )
			{
				$key = "current version is ".$apversion;
			}
		}
		else
		{
			$key = "can't get afi's version info";
		}
	}
	if($trigger != "" && strncmp($trigger, "Admin:", 6))
	{
		$trigger = "by ".$trigger;
	}
	$ret = ext_active("add_upgrade_active_to_db", $mac, $ip, $trigger, $version, $key);
	echo "AC got information model \"".$model."\" mac \"".$mac."\" ip \"".$ip."\"sn \"".$sn."\" key \"".$key."\" trigger \"".$trigger."\"\n";
}
?>
