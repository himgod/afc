<?php
include("expired_check.php");
//include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
include("../publichead.php");
//require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
//include("../message.php");
$json = isset($_REQUEST["json"]) ? json_decode($_REQUEST["json"]) : "";
if(isset($json->server) && "" != $json->server)
{
	if(!strncmp($json->server, "http://", 7))
	{
		$server = str_replace("http://", "", $json->server);
	}
	else
	{
		$server = $json->server;
	}
}
if(isset($json->update) && $json->update)
{
	$update = true;	
}

$version = get_ac_version_info();
$full_version = $version["version"].".".$version["buildno"];
$update_available = false;
$upgrade_status = "";
$version_url = "";
if(isset($_SESSION["upgrade_status"]) && "" != $_SESSION["upgrade_status"])
{
	$new_version_no = isset($_SESSION["new_version_no"]) ? $_SESSION["new_version_no"] : "";
	$upgrade_status = $_SESSION["upgrade_status"];
	if((isset($_SESSION["starttime"]) && $_SESSION["starttime"] 
			&& isset($_SESSION["totalsecond"]) && $_SESSION["totalsecond"])
			&& (time()-$_SESSION["starttime"] > $_SESSION["totalsecond"]+3))
	{
		$_SESSION["upgrade_status"] = "";
		$_SESSION["starttime"] = 0;
		$_SESSION["totalsecond"] = 0;
	}
}
else
{
	$new_version_file = "/tmp/new_version_".$_SERVER["REMOTE_ADDR"];
	$version_server = (isset($server) && "" != $server) ? $server : get_afc_version_server_addr();

	if(($version_server != "") &&(!file_exists($new_version_file) 
		|| (!isset($_SESSION["check_ac_version"]) || true != $_SESSION["check_ac_version"])
		|| (!isset($_SESSION["version_server"]) || ($_SESSION["version_server"] != $version_server))
		|| (!isset($_SESSION["old_version"]) || $_SESSION["old_version"] != $full_version)
		|| (isset($update) && true == $update)))
	{	/* $server is "" just for refresh new version info from broswer */
		$_SESSION["version_server"] = $version_server;
		$ret = exec('wget -tries=2 -T 3 -q -O '.$new_version_file.' "http://'.$version_server.'/img/afc_version" >/dev/null 2>/dev/null &');
		$_SESSION["check_ac_version"] = true;
		$_SESSION["old_version"] = $full_version;
	}
	$new_version_no = "";
	$update_available = "";
	if(file_exists($new_version_file))
	{
		$new_version = get_ac_new_version_info($new_version_file);
		$new_version_no = $new_version["version"];
		$_SESSION["ac_version_url"] = "";
		$_SESSION["new_version_no"] = "";
		if(version_is_newer($new_version_no, $full_version))
		{
			$update_available = true;
			$version_url = $new_version["url"];
			$_SESSION["ac_version_url"] = $new_version["url"];
			$_SESSION["new_version_no"] = $new_version_no;	
		}
	}
}

$data = array();
$meta["rc"] = "ok";
$data["update_available"] = $update_available;
$data["update_downloaded"] = $update_available;
$data["upgrade_status"] = $upgrade_status;
$data["update_version"] = $new_version_no;
$data["version"] = $full_version;
$data["url"] = $version_url;

$result = array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>
