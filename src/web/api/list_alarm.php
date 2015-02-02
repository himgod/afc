<?php
include("expired_check.php");
$json="";
$archived = "all";
$archiveValue = -1;
$_sort = 0;
//$lastId = 0;
$data=array();
if(!isset($meta))
{
	$meta=array();
	$meta["rc"] = "ok";
}
if(isset($_REQUEST["json"]))
{
	$json=json_decode($_REQUEST["json"]);
}
if(isset($json->_sort))
{
	$_sort = $json->_sort;
	if($_sort == "-time")
	{
		$_sort = -1;
	}
	else if($_sort == "time" || $_time == "+time")
	{
		$_sort = 1;
	}
}
if(isset($json->archived))
{
	$archived=$json->archived;
	if(true == $archived || "true" == $archived)
	{
		$archiveValue = 1;
	}
	else
	{
		$archiveValue = 0;
	}
}
$ret = ext_active("get_alert_with_filter", $archiveValue);
if(!is_object($ret))
{
	//$meta["rc"] = "error";
	//$meta["msg"] = "";
}
else if(!isset($ret->result) || 0 != $ret->result)
{	
	//$meta["rc"] = "error";
	//$meta["msg"] = "";
}
else if(isset($ret->count) && $ret->count > 0)
{
	if(isset($ret->value))
	{
		$data = $ret->value;
	}
	//if(isset($ret->lastId))
	//{
		//$lastId = $ret->lastId;
	//}	
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);
/*
$json_str = '{ "data" : [ { "_id" : "53b15432f5f929f8d6f424fd" , "ap" : "24:a4:3c:8c:07:c0" , "archived" : false , "datetime" : "2014-06-30T12:12:34Z" , "handled_admin_id" :  null  , "handled_datetime" : "2014-06-30T12:19:18Z" , "handled_time" : 1404130758 , "key" : "EVT_AP_DiscoveredPending" , "msg" : "AP[24:a4:3c:8c:07:c0] was discovered and waiting for adoption" , "time" : 1404130354053} , { "_id" : "53b20b4cf5f929f8def424fd" , "ap" : "24:a4:3c:8c:07:c0" , "archived" : true , "datetime" : "2014-07-01T01:13:48Z" , "handled_admin_id" : "538866ed48f029f8a0b8d52f" , "handled_time" : "2014-07-04T01:57:55Z" , "key" : "EVT_AP_Lost_Contact" , "msg" : "AP[24:a4:3c:8c:07:c0] was disconnected" , "time" : 1404177228439}] , "meta" : { "rc" : "ok"}}';

echo $json_str;
*/
?>
