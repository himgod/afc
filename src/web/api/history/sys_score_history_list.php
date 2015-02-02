<?php 

if (isset($_POST['data_type']))
	$tm_flag = $_POST['data_type'];
else
	$tm_flag = -1;	


if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$sig_intens = array();
$sig_inter = array();
$healthy_score = array();
$uplink_bytes = array();
$downlink_bytes = array();
$online_usr_nums = array();
$usr_acc_rate = array();
$cover_area = array();
$tm_stmp = array();


$ret = ext_wtp("ext_history_sys_score_info");
if(is_object($ret) && $ret->sys_score_info["sys_score_num"] > 0)
{
	for ($i = 0; $i < $ret->sys_score_info["sys_score_num"]; $i++) {
		$sig_intens[$i] = isset($ret->value[$i]["SignalInten"]) ? $ret->value[$i]["SignalInten"] : 0;
		$sig_inter[$i] = isset($ret->value[$i]["SignalInter"]) ? $ret->value[$i]["SignalInter"] : 0;
		$healthy_score[$i] = isset($ret->value[$i]["Score"]) ? $ret->value[$i]["healthy_score"] : 0;
		$online_usr_nums[$i] = isset($ret->value[$i]["OnLnUsrNum"]) ? $ret->value[$i]["OnLnUsrNum"] : 0;
		$usr_acc_rate[$i] = isset($ret->value[$i]["UsrACCRate"]) ? $ret->value[$i]["UsrACCRate"] : 0;
		$uplink_bytes[$i] = isset($ret->value[$i]["ULnkByteNum"]) ? $ret->value[$i]["ULnkByteNum"] : 0;
		$downlink_bytes[$i] = isset($ret->value[$i]["DLnkByteNum"]) ? $ret->value[$i]["DLnkByteNum"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;
	}
}
else
{
	$data = array();
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>