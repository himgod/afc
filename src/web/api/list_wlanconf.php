<?php
include("expired_check.php");
include("encoding_json.php");

//$json = json_decode($_POST['json']);

$ret = ext_wireless("show_wireless_network_list");

$data = array();
$meta["rc"] = "ok"; //define and init , although done inside the include file 
//$ret = 1;
if(isset($ret->wlans["wlan_num"]))
{
	$wlan_num = $ret->wlans["wlan_num"];	
	if(($wlan_num > 0))
	{
		for($i = 0; $i < $wlan_num; $i++)
		{
			$data[$i] = $ret->value[$i];
			//$data[$i]["name"] = encoding_chinese($data[$i]["name"]);
			$data[$i]["vlan_enabled"] = $data[$i]["vlan_enabled"] ? true:false;
			$data[$i]["is_guest"] = $data[$i]["is_guest"] ? true:false;
			$data[$i]["enabled"] = $data[$i]["enabled"] ? true:false;
			$data[$i]["hide_ssid"] = $data[$i]["hide_ssid"] ? true:false; 
		}
	}
	else
	{
		//$meta["msg"] = "wlan num ".$wlan_num;
	}
}
else
{
	$meta["msg"] = "api.err.GetWirelessNetworkFailed";
	//$meta["ret"] = json_encode($ret);
}
//$meta["dataDecode"] = true;
//$meta["metaDecode"] = true;
//$meta["msg"] = encoding_chinese("来两中国字试试");
//$meta["msg"] = json_encode($data);// for debug 
$result = array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

return ;

?>
