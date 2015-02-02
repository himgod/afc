<?php
include("expired_check.php");
$json = json_decode($_POST['json']);
$wlanid = $json->_id;

$ret = ext_wireless("delete_wireless_network", $wlanid);

if($ret != 0)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.DeleteWirelessNetworkFailed";
	$meta["ret"] = $ret;
	$result = array("data"=>array(), "meta"=>$meta);	
	echo json_encode($result);
	return ;
}

$json_str = '{"data":[],"meta":{"rc":"ok"}}';
echo $json_str;
?>
