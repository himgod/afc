<?php 

if (isset($_POST['time_type']))
	$tm_flag = $_POST['time_type'];
else
	$tm_flag = -1;
	
if (isset($_POST['wireless_type'])) 
	$wireless_str = $_POST['wireless_type'];
else
	$wireless_str = "";

if(!isset($meta))
{
	$meta["rc"] = "ok";
}
$uplink_bytes = array();
$uplink_packs = array();
$downlink_bytes = array();
$downlink_packs = array();
$online_usr_cnt = array();
$tm_stmp = array();

$ret = ext_history("get_wireless_info", $tm_flag, $wireless_str);
if(is_object($ret) && $ret->wireless_info["wireless_num"] > 0)
{
	for ($i = 0; $i < $ret->wireless_info["wireless_num"]; $i++) {
		$uplink_bytes[$i] = isset($ret->value[$i]["ULnkByteNum"]) ? $ret->value[$i]["ULnkByteNum"] : 0;
		$downlink_bytes[$i] = isset($ret->value[$i]["DLnkByteNum"]) ? $ret->value[$i]["DLnkByteNum"] : 0;
		$uplink_packs[$i] = isset($ret->value[$i]["ULnkPackNum"]) ? $ret->value[$i]["ULnkPackNum"] : 0;
		$downlink_packs[$i] = isset($ret->value[$i]["DLnkPackNum"]) ? $ret->value[$i]["DLnkPackNum"] : 0;
		$online_usr_cnt[$i] = isset($ret->value[$i]["OnUsrCnt"]) ? $ret->value[$i]["OnUsrCnt"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;
	}
	$data = array("ULnkByteNum"=>$uplink_bytes, "ULnkPackNum"=>$uplink_packs, "DLnkByteNum"=>$downlink_bytes, "DLnkPackNum"=>$downlink_packs, "OnUsrCnt"=>$online_usr_cnt);
}
else
{	
	$meta["rc"] = "fail";
	$data = array("ULnkByteNum"=>$uplink_bytes, "ULnkPackNum"=>$uplink_packs, "DLnkByteNum"=>$downlink_bytes, "DLnkPackNum"=>$downlink_packs, "OnUsrCnt"=>$online_usr_cnt);
}
$result = array("TmStmp"=>$tm_stmp, "data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>
