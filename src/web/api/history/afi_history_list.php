<?php 

include("expired_check.php");    //notes the path
$json = "";
if(!isset($meta))
{
	$meta["rc"] = "ok";
}
if(isset($_REQUEST["json"]))
{
	$json = $_REQUEST["json"];
}

$ret = ext_history("get_afi_info");
if(is_object($ret) && $ret->ap_num["afi_num"] > 0)
{
	$data = $ret->value;
}
else
{
	$data = array();
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>