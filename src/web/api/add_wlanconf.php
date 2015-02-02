<?php
include("expired_check.php");
include("encoding_json.php");

$json = json_decode($_POST['json']);
$ssid = "";
if(isset($json->name))	$ssid = $json->name;
$security = "";
if(isset($json->security))	$security = $json->security;
$passphrase = "";
if(isset($json->x_passphrase))	$passphrase = $json->x_passphrase;
$wep_index = "";
if(isset($json->wep_idx))	$wep_index = $json->wep_idx;
$wep_key = "" ;
if(isset($json->x_wep))		$wep_key = $json->x_wep;
$enabled = "" ;
if(isset($json->enabled)) 	$enabled = $json->enabled;
$radius_ip = "" ;
if(isset($json->radius_ip_1))	$radius_ip = $json->radius_ip_1;
$radius_port = "" ;
if(isset($json->radius_port_1))	$radius_port = $json->radius_port_1;
$radius_secret = "" ;
if(isset($json->x_raiuds_secret_1))	$radius_secret = $json->x_radius_secret_1;
$is_guest = "" ;
if(isset($json->is_guest))	$is_guest = $json->is_guest;
$vlan_enabled = "" ;
if(isset($json->vlan_enabled))	$vlan_enabled = $json->vlan_enabled;
$vlanid = "" ;
if(isset($json->vlan))		$vlanid = $json->vlan;
$hide_ssid = "" ;
if(isset($json->hide_ssid))	$hide_ssid = $json->hide_ssid;
$wpa_mode = "" ;
if(isset($json->wpa_mode))	$wpa_mode = $json->wpa_mode;
$wpa_enc = "" ;
if(isset($json->wpa_enc))	$wpa_enc = $json->wpa_enc;
$usergroup_id = "" ;
if(isset($json->usergroup_id))	$usergroup_id = $json->usergroup_id;
$user_key = "";
$wlanid = 0;
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
$usergroup_id = $usergroup_id ? $usergroup_id:0;
$enabled = $enabled ? 1:0;
$is_guest = $is_guest ? 1:0;
$data = array();
$meta["rc"] = "ok";
$retval = 0;
$ret = ext_wireless("add_wireless_network", $ssid, $user_key, $security, $secur_arg, 
	$enabled, $is_guest, $hide_ssid, $vlan_enabled, $vlanid, 
	$wpa_enc, $radius_ip, $radius_port, $usergroup_id);
//ext_wireless("set_user_default_wireless_network", $ssid, $user_key);
$meta["rc"] = "ok";
if(isset($ret->value))
{
	$retval = $ret->value["ret"];
	$wlanid = $ret->value["wlanid"];
}
else
{
	$retval = $ret;
	$wlanid = 0;
}
if($retval < 0)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.AddWirelessNetworkFailed";
	$meta["name"] = $ssid;
	$meta["ret"] = $ret;
	$result = array("data"=>$data, "meta"=>$meta);	
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
		$data[$i]["vlan_enabled"] = $data[$i]["vlan_enabled"] ? true:false;
		$data[$i]["is_guest"] = $data[$i]["is_guest"] ? true:false;
		$data[$i]["enabled"] = $data[$i]["enabled"] ? true:false;
		$data[$i]["hide_ssid"] = $data[$i]["hide_ssid"] ? true:false; 
	}
	else
	{
		$meta["msg"] = "api.err.GetWirelessNetworkFailed";
	}
}
//$meta["msg"] = json_encode($data);// for debug 
$result = array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>
