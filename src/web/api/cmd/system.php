<?php
include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
$json_backup='{"data":[{"url":"/dl_backup/1.0.0.conf"}],"meta":{"rc":"ok"}}';
$json=json_decode($_POST['json']);
$json_str = '{"data": [], "meta": {"rc": "ok"}}';

if($json->cmd == "backup")
{
	echo $json_backup;
}
else if($json->cmd == "support")
{
	echo $json_str;
}
else if($json->cmd == "upgrade")
{
	$meta["rc"] = "ok";
	if(isset($_SESSION["ac_version_url"]) && $_SESSION["ac_version_url"] != "")
	{
		if((!isset($_SESSION["upgrade_status"]) || "" == $_SESSION["upgrade_status"]))
		{
			exec("sudo echo 0 > /opt/run/upgraded 2>/dev/null");
			$_SESSION["upgrade_status"] = "downloading";
			$new_version_no = isset($_SESSION["new_version_no"]) ? $_SESSION["new_version_no"] : "";
			$ac_version_file = "/tmp/ybuilder".$new_version_no."_".$_SERVER["REMOTE_ADDR"];
			$cmd = 'wget -q -O '.$ac_version_file.' "'.$_SESSION["ac_version_url"].'" >/dev/null 2>/dev/null ; echo $?';
			$tmpfile = "/tmp/tmp_".$_SERVER["REMOTE_ADDR"];
			
			$ret = exec($cmd);
			if("0" == $ret)
			{
				$cmd = "chmod +x ".$ac_version_file." 2>/dev/null ;";
				exec($cmd);
				$shellcmd = "'sudo ".$ac_version_file." > ".$tmpfile." 2>/dev/null &'";
				//$ret = system($cmd);
				$tmpshell = "/tmp/tmpshell_".$_SERVER["REMOTE_ADDR"];
				exec("echo ".$shellcmd." > ".$tmpshell." 2>/dev/null");
				$cmd = "sudo at now +1 minutes -f ".$tmpshell." >/dev/null 2>/dev/null;echo $?";
				$ret = exec($cmd);
				$_SESSION["totalsecond"] = (66-(int)date("s"));
				$_SESSION["starttime"] = time();
				$_SESSION["upgrade_status"] = "downloaded";
				if("0" == $ret)
				{
					//$_SESSION["upgrade_status"] = "upgraded";
				}
				else
				{
					$meta["rc"] = "error";
					$meta["msg"] = "api.err.UpgradeLoadFailed";
					$_SESSION["upgrade_status"] = "";//"upgrade_failed";
				}
			}
			else
			{
				$meta["rc"] = "error";
				$meta["msg"] = "api.err.UpgradeDownloadFailed";
				$_SESSION["upgrade_status"] = "";//"download_failed";
			}
		}
		else
		{
			if(isset($_SESSION["starttime"]) && $_SESSION["starttime"] 
				&& isset($_SESSION["totalsecond"]) && $_SESSION["totalsecond"])
			if(time()-$_SESSION["starttime"] > $_SESSION["totalsecond"]+3)
			{
				$_SESSION["upgrade_status"] = "";
				$_SESSION["starttime"] = 0;
				$_SESSION["totalsecond"] = 0;
			}
		}
	}
	else
	{
		$meta["rc"] = "error";
		$meta["msg"] = "api.err.UpgradeURLParseFailed";
		$_SESSION["upgrade_status"] = "";//"url_parse_failed";
	}
	$meta["upgrade_status"] = $_SESSION["upgrade_status"];
	$meta["totalsecond"] = isset($_SESSION["totalsecond"]) ? $_SESSION["totalsecond"] : 0;
	$meta["passedsecond"] = isset($_SESSION["starttime"]) ? (time() - $_SESSION["starttime"]):0;
	$result = array("data"=>array(), "meta"=>$meta);
	echo json_encode($result);
}
else if($json->cmd == "restore")
{
	$_SESSION['shutdown'] = true;
	$_SESSION['shutdown_time'] = time();
	echo $json_str;
}
else if("set-installed" == $json->cmd)
{
	echo $json_str;
}
else
{
	echo $json_str;
}
?>
