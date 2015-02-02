<?php
$json = "";
$_id = 0;
$cmd = "";
if(!isset($meta))
{
	$meta["rc"] = "ok";
}
if(isset($_REQUEST["json"]))
{
	$json=json_decode($_REQUEST["json"]);
}

if(isset($json->_id))
{
	$_id = $json->_id;
}
if(isset($json->cmd))
{
	$cmd = $json->cmd;
}
$json_str = '{"data":[],"meta":{"rc":"ok"}}';
if($cmd == "archive-alarm")
{
	$apmac = "";
	if(isset($json->apmac))
	{
		$apmac = $json->apmac;
	}
	$ret = ext_active("archive_alert_by_activeid_apmac", $_id, $apmac);// allow _id is 0 
	if(0 != $ret)
	{
		$meta["rc"] = "error";
		$meta["msg"] = "api.err.OperationFailed";
	}
}
else if($cmd == "archive-all-alarms")
{
	$ret = ext_active("archive_all_alert");
	if(0 != $ret)
	{
		$meta["rc"] = "error";
		$meta["msg"] = "api.err.OperationFailed";
	}
}
echo $json_str;
?>
