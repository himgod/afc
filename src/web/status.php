<?php
include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
$cmd = "";
if(isset($_REQUEST["cmd"]))
{
	$cmd = $_REQUEST["cmd"];
}
//$cmd = isset($json->cmd) ? $json->cmd : "";

$meta["rc"] = "ok";
if($cmd == "upgrade")
{
	if(isset($_SESSION['upgrade_status']) && "" != $_SESSION['upgrade_status'])
	{		
		if((isset($_SESSION["starttime"]) && $_SESSION["starttime"] 
			&& isset($_SESSION["totalsecond"]) && $_SESSION["totalsecond"])
			&& (time()-$_SESSION["starttime"] > $_SESSION["totalsecond"]+3))
		{
			$_SESSION["upgrade_status"] = "";
			$_SESSION["starttime"] = 0;
			$_SESSION["totalsecond"] = 0;
			$meta["upgrade_status"] = "upgrade_failed";
		}
		$meta["totalsecond"] = isset($_SESSION["totalsecond"]) ? $_SESSION["totalsecond"] : 0;
		$meta["passedsecond"] = isset($_SESSION["starttime"]) ? (time() - $_SESSION["starttime"]):0;
		$meta["upgrade_status"] = $_SESSION['upgrade_status'];
		if(($meta["upgrade_status"] == "upgraded") || 
			($meta["upgrade_status"] == "download_failed") || 
			($meta["upgrade_status"] == "url_parse_failed") || 
			($meta["upgrade_status"] == "upgrade_failed"))
		{
			$_SESSION['upgrade_status'] = "";
		}
		else
		{
			$version = get_ac_version_info();
			$over = get_ac_upgrade_over();
			$full_version = $version["version"].".".$version["buildno"];
			if($_SESSION["new_version_no"] == $full_version && $over)
			{
				$meta["upgrade_status"] = "upgraded";
				$_SESSION['upgrade_status'] = "";
			}
		}
	}
}
else if($cmd == "restore")
{
	if(isset($_SESSION['restore_status']) && $_SESSION['restore_status'])
	{
		$meta["restore_status"] = $_SESSION['restore_status'];	
		if(($meta["restore_status"] == "restored") || 
			($meta["restore_status"] == "restored") ||
			($meta["restore_status"] == "restored"))
		{
			$_SESSION['restore_status'] = "";
		}
	}
}
//$meta["msg"] = "cmd ".$cmd." session ".$_SESSION["upgrade_status"];
$result = array("data"=>array(), "meta"=>$meta);
echo json_encode($result);

?>
