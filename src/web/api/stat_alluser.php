<?php
include("expired_check.php");
$json = "";
if(!isset($meta))
{
	$meta["rc"] = "ok";
}
if(isset($_REQUEST["json"]))
{
	$json = $_REQUEST["json"];
}
$ret = ext_wtp("sta_list");
if(is_object($ret) && $ret->stas["sta_num"] > 0)
{
	$data = $ret->value;
}
else
{
	$data = array();
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

/*
$json_str = '{ "data" : [ { "_id" : "53b21e2bf5f929f8e1f424fd" , "bytes" : 800676 , "duration" : 23530 , "first_seen" : 1404182057 , "hostname" : "MI2S-xiaomishouji" , "is_guest" : true , "last_seen" : 1404448180 , "mac" : "f8:a4:5f:85:98:af" , "name" : "11111" , "note" : "w" , "noted" : true , "oui" : "" , "rx_bytes" : 684116 , "stat" : { "_id" : "53b21e2b72b4cbf84b46c17b" , "bytes" : 800676 , "duration" : 23530 , "last_seen" : 1404448180 , "o" : "user" , "rx_bytes" : 684116 , "tx_bytes" : 116560 , "type" : "life" , "user" : "f8:a4:5f:85:98:af"} , "tx_bytes" : 116560 , "usergroup_id" : "538d4aba508629f8b5b6ebf9"}] , "meta" : { "rc" : "ok"}}';

echo $json_str;
*/
?>
