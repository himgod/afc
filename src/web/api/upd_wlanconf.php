<?php
include("expired_check.php");
include("encoding_json.php");
$json = json_decode($_POST['json']);
$wlanid = $json->_id;
$ssid = $json->name;
$security = $json->security;
$passphrase = $json->x_passphrase;
$wep_index = $json->wep_idx;
$wep_key = $json->x_wep;
$enabled = $json->enabled;
$radius_ip = $json->radius_ip_1;
$radius_port = $json->radius_port_1;
$radius_secret = $json->x_radius_secret_1;
$is_guest = $json->is_guest;
$vlan_enabled = $json->vlan_enabled;
$vlanid = $json->vlan;
$hide_ssid = $json->hide_ssid;
$wpa_mode = $json->wpa_mode;
$wpa_enc = $json->wpa_enc;
$usergroup_id = $json->usergroup_id;
$user_key = "";
if($ssid == "")
{
	$ssid = "Name";
}
$wep_index = $wep_index ? $wep_index:1;
$mode = array("auto"=>0, "wpa1"=>1, "wpa2"=>2);
if($wpa_mode == "")
{
	$wpa_mode = "auto";
}
//$wpa_mode = $mode[$wpa_mode];
$secur_arg = $mode[$wpa_mode];
if($security == "wep")
{
	$user_key = $wep_key;
	$secur_arg = $wep_index;
}
else if($security == "wpapsk")
{//wpa-p 
	$user_key = $passphrase;
}
else if($security == "wpaeap")
{
	$user_key = $radius_secret;
}
else
{//open
	$user_key = "";
	$secur_arg = 0;
}
//$type = array("open"=>0, "wep"=>1, "wpapsk"=>2, "wpaeap"=>3);
//$security=$type[$security];
if($vlan_enabled != true)
{
	$vlanid = 0;
}
//$enc = array("auto"=>0, "tkip"=>1, "ccmp"=>2);
if($wpa_enc == "")
{
	$wpa_enc = "auto";
}
//$wpa_enc = $enc[$wpa_enc];
$usergroup_id = 0; //$usergroup_id ? $usergroup_id:0;
$enabled = $enabled ? 1:0;
$is_guest = $is_guest ? 1:0;
$wlanid = $wlanid ? : 0;
//$wlanid = 1;//for test

$meta["rc"] = "ok";
$ret = ext_wireless("modify_wireless_network", $wlanid, $ssid, $user_key, $security, $secur_arg, 
	$enabled, $is_guest, $hide_ssid, $vlan_enabled, $vlanid, 
	$wpa_enc, $radius_ip, $radius_port, $usergroup_id);
	
if($ret != 0)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.UpdateWirelessNetworkFailed";
	$meta["name"] = $ssid;
	$meta["ret"] = $ret;
	$result = array("data"=>array(), "meta"=>$meta);	
	echo json_encode($result);
	return ;
}

if($wlanid > 0)
{
	$i = 0;
	$ret = ext_wireless("get_one_wireless_network", $wlanid);
	if(isset($ret->value))
	{
		$data[$i] = $ret->value;
		$data[$i]["name"] = encoding_chinese($data[$i]["name"]);
	}
	else
	{
		$meta["msg"] = "api.err.GetWirelessNetworkFailed";
	}
}
$result = array("data"=>$data, "meta"=>$meta);
//$result->data = $data;
//$result->meta = $meta;
echo json_encode($result);
?>
