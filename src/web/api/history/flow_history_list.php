<?php 


if (isset($_POST['time_type']))
	$tm_flag = $_POST['time_type'];
else
	$tm_flag = -1;	


if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$uplink_bytes = array();
$uplink_packs = array();
$downlink_bytes = array();
$downlink_packs = array();
$tm_stmp = array();


$ret = ext_history("get_flow_info", $tm_flag);
if(is_object($ret) && $ret->flow_info["flow_num"] > 0)
{
	for ($i = 0; $i < $ret->flow_info["flow_num"]; $i++) {
		$uplink_bytes[$i] = isset($ret->value[$i]["ULnkByteNum"]) ? $ret->value[$i]["ULnkByteNum"] : 0;
		$uplink_packs[$i] = isset($ret->value[$i]["ULnkPackNum"]) ? $ret->value[$i]["ULnkPackNum"] : 0;
		$downlink_bytes[$i] = isset($ret->value[$i]["DLnkByteNum"]) ? $ret->value[$i]["DLnkByteNum"] : 0;
		$downlink_packs[$i] = isset($ret->value[$i]["DLnkPackNum"]) ? $ret->value[$i]["DLnkPackNum"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;
	}
	$data = array("ULnkByteNum"=>$uplink_bytes, "ULnkPackNum"=>$uplink_packs, "DLnkByteNum"=>$downlink_bytes, "DLnkPackNum"=>$downlink_packs);
}
else
{
	$meta["rc"] = "fail";
	$data = array("ULnkByteNum"=>$uplink_bytes, "ULnkPackNum"=>$uplink_packs, "DLnkByteNum"=>$downlink_bytes, "DLnkPackNum"=>$downlink_packs);
}
$result = array("TmStmp"=>$tm_stmp, "data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>

