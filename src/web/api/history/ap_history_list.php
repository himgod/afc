<?php 

if (isset($_POST['time_type']))
	$tm_flag = $_POST['time_type'];
else
	$tm_flag = -1;
	

if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$bw = array();
$tm_dely = array();
$sig_intens = array();
$tm_stmp = array();

$ret = ext_history("get_ap_info", $tm_flag);
if(is_object($ret) && $ret->ap_info["ap_num"] > 0)
{	
	for ($i = 0; $i < $ret->ap_info["ap_num"]; $i++) {
		$sig_intens[$i] = isset($ret->value[$i]["SignalIntens"]) ? $ret->value[$i]["SignalIntens"] : 0;
		$bw[$i] = isset($ret->value[$i]["DownUpBW"]) ? $ret->value[$i]["DownUpBW"] : 0;
		$tm_dely[$i] = isset($ret->value[$i]["TmDelay"]) ? $ret->value[$i]["TmDelay"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;	
	}
	$data = array("TmDelay"=>$tm_dely, "DownUpBW"=>$bw, "SignalIntens"=>$sig_intens);
}
else 
{
	$meta["rc"] = "fail";
	$data = array("TmDelay"=>$tm_dely, "DownUpBW"=>$bw, "SignalIntens"=>$sig_intens);
}

$result = array("TmStmp"=>$tm_stmp, "data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>