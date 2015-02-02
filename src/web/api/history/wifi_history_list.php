<?php 

if (isset($_POST['time_type']))
	$tm_flag = $_POST['time_type'];
else
	$tm_flag = -1;


if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$sig_noise_rate = array();
$sig_inter = array();
$chan_utilize = array();
$tm_stmp = array();

$ret = ext_history("get_wifi_info", $tm_flag);
if(is_object($ret) && $ret->wifi_info["wifi_num"] > 0)
{
	for ($i = 0; $i < $ret->wifi_info["wifi_num"]; $i++) {
		$sig_noise_rate[$i] = isset($ret->value[$i]["SignalNoiseRate"]) ? $ret->value[$i]["SignalNoiseRate"] : 0;
		$sig_inter[$i] = isset($ret->value[$i]["SignalInter"]) ? $ret->value[$i]["SignalInter"] : 0;
		$chan_utilize[$i] = isset($ret->value[$i]["ChanUtilize"]) ? $ret->value[$i]["ChanUtilize"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;
	}
	$data = array("SignalNoiseRate"=>$sig_noise_rate, "SignalInter"=>$sig_inter, "ChanUtilize"=>$chan_utilize);
}
else
{
	$meta["rc"] = "fail";
	$data = array("SignalNoiseRate"=>$sig_noise_rate, "SignalInter"=>$sig_inter, "ChanUtilize"=>$chan_utilize);
}
$result = array("TmStmp"=>$tm_stmp, "data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>

