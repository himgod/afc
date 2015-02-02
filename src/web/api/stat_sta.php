<?php
include("expired_check.php");
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");

$data = array();
$meta["rc"] = "ok";
$json = "";

if (isset($_REQUEST["json"]))
{
	$json = json_decode($_REQUEST['json']);
}

$ret = ext_wtp("sta_list");
$hash = array();

if (is_object($ret) && isset($ret->stas["sta_num"]) && $ret->stas["sta_num"] > 0)
{
	$j = 0;
	for ($i = 0; $i < $ret->stas["sta_num"]; $i++)
	{
		if (!isset($hash[$ret->value[$i]["mac"]]))
		{
			$data[$j] = $ret->value[$i];
			$data[$j]["uplink_table"] = array();

			$hash[$ret->value[$i]["mac"]] = $j;
			$j++;
		}
		else
		{
			if (!isset($ret->value[$i]["roam"]) || 1 != $ret->value[$i]["roam"])
			{
				$data[$hash[$ret->value[$i]["mac"]]] = $ret->value[$i];
				$data[$hash[$ret->value[$i]["mac"]]]["uplink_table"] = array();
			}
		}
	}
}

$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

/*
$json_str = '{ "_id" : "53b21e2bf5f929f8e1f424fd" , "_rx_bytes" : 2112 , "_rx_crypts" : 0 , "_rx_dropped" : 0 , "_rx_errors" : 0 , "_rx_frags" : 0 , "_rx_packets" : 7 , "_tx_bytes" : 310 , "_tx_dropped" : 0 , "_tx_errors" : 0 , "_tx_packets" : 2 , "_tx_retries" : 0 , "_uptime" : 44 , "ap_mac" : "24:a4:3c:8c:07:c0" , "assoc_time" : 1404381420 , "auth_time" : 4294967152 , "authorized" : true , "bssid" : "24:a4:3c:8d:07:c0" , "bytes" : 14168 , "bytes.d" : 0 , "bytes.r" : 0 , "ccq" : 1000 , "channel" : 11 , "dhcpend_time" : 0 , "dhcpstart_time" : 4927 , "essid" : "testssid" , "first_seen" : 1404182057 , "hostname" : "MI2S-xiaomishouji" , "idletime" : 22 , "is_11a" :  null  , "is_11ac" :  null  , "is_11b" :  null  , "is_11n" : true , "is_guest" : true , "last_seen" : 1404381723 , "mac" : "f8:a4:5f:85:98:af" , "map_id" : "5388665748f029f89fb8d52f" , "name" : "f8:a4:5f:85:98:af" , "noise" : -97 , "note" : "w" , "oui" : "" , "powersave_enabled" : true , "qos_policy_applied" : true , "radio" : "ng" , "rssi" : 50 , "rx_bytes" : 12308 , "rx_bytes.d" : 0 , "rx_bytes.r" : 0 , "rx_crypts" : 0 , "rx_crypts.d" : 0 , "rx_crypts.r" : 0 , "rx_dropped" : 0 , "rx_dropped.d" : 0 , "rx_dropped.r" : 0 , "rx_errors" : 0 , "rx_errors.d" : 0 , "rx_errors.r" : 0 , "rx_frags" : 0 , "rx_frags.d" : 0 , "rx_frags.r" : 0 , "rx_mcast" : 2 , "rx_packets" : 41 , "rx_packets.d" : 0 , "rx_packets.r" : 0 , "rx_rate" : 6000 , "rx_retries" : 6 , "signal" : -47 , "state" : 15 , "state_ht" : true , "state_pwrmgt" : false , "t" : "sta" , "tx_bytes" : 1860 , "tx_bytes.d" : 0 , "tx_bytes.r" : 0 , "tx_dropped" : 0 , "tx_dropped.d" : 0 , "tx_dropped.r" : 0 , "tx_errors" : 0 , "tx_errors.d" : 0 , "tx_errors.r" : 0 , "tx_packets" : 12 , "tx_packets.d" : 0 , "tx_packets.r" : 0 , "tx_power" : 40 , "tx_rate" : 0 , "tx_retries" : 0 , "tx_retries.d" : 0 , "tx_retries.r" : 0 , "uptime" : 303 , "user_id" : "53b21e2bf5f929f8e1f424fd" , "usergroup_id" : "538d4aba508629f8b5b6ebf9"}';
$sta1 = json_decode($json_str);
$sta1 = get_object_vars($sta1);
$sta2 = $sta1;
$sta2["mac"] = "00:1f:64:08:09:0a";
$sta2["name"]="teststa2";
$sta2["hostname"]="whoknowme";
$sta2["is_guest"] = false;
$meta->rc="ok";
$result->data=array($sta1, $sta2);
$result->meta=$meta;
echo json_encode($result);*/

?>
