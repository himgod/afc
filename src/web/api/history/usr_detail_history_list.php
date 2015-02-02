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
$ret = ext_wtp("ext_history_usr_detail");
if(is_object($ret) && $ret->usr_detail["usr_num"] > 0)
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

