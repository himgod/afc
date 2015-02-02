<?php 

if (isset($_POST['time_type']))
	$tm_flag = $_POST['time_type'];
else
	$tm_flag = -1;


if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$access_rate = array();
$retrans_rate = array();
$sig_intens = array();
$negotiate_rate = array();
$tm_stmp = array();

$ret = ext_history("get_terminal_info", $tm_flag);
if(is_object($ret) && $ret->terminal_info["term_num"] > 0)
{	
	for ($i = 0; $i < $ret->terminal_info["term_num"]; $i++) {
		$sig_intens[$i] = isset($ret->value[$i]["SignalIntens"]) ? $ret->value[$i]["SignalIntens"] : 0;
		$access_rate[$i] = isset($ret->value[$i]["AccessRate"]) ? $ret->value[$i]["AccessRate"] : 0;
		$negotiate_rate[$i] = isset($ret->value[$i]["NegotiateRate"]) ? $ret->value[$i]["NegotiateRate"] : 0;
		$retrans_rate[$i] = isset($ret->value[$i]["RetransRate"]) ? $ret->value[$i]["RetransRate"] : 0;
		$tm_stmp[$i] = isset($ret->value[$i]["TmStmp"]) ? $ret->value[$i]["TmStmp"] : 0;	
	}
	$data = array("SignalIntens"=>$sig_intens, "RetransRate"=>$retrans_rate, "AccessRate"=>$access_rate, "NegotiateRate"=>$negotiate_rate);
}
else 
{
	$meta["rc"] = "fail";
	$data = array("SignalIntens"=>$sig_intens, "RetransRate"=>$retrans_rate, "AccessRate"=>$access_rate, "NegotiateRate"=>$negotiate_rate);
}


$result = array("TmStmp"=>$tm_stmp, "data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>