<?php
include("expired_check.php");
$json_str = '{"data":[{"_id":"53b155c6f5f929f8d8f424fd","adopted":true,"cfgversion":"856eb8e3390d844c","config_network":{"ip":"192.168.1.20","type":"dhcp"},"has_eth1":false,"has_poe_passthrough":false,"inform_ip":"192.168.45.135","inform_url":"http://192.168.45.135:8080/inform","ip":"192.168.45.149","last_seen":1404872381,"locked":true,"mac":"24:a4:3c:8c:07:c0","map_id":"5388665748f029f89fb8d52f","model":"BZ2","name":"abcd","radio_table":[{"antenna_gain":6,"builtin_antenna":true,"channel":0,"max_txpower":20,"name":"wifi0","radio":"ng"}],"serial":"24A43C8C07C0","version":"2.4.6.2178","vwire_table":[],"x":430.30225080385856,"x_authkey":"52ed781610b6f0033c41f1e5637fe0d9","x_fingerprint":"96:71:8f:4b:6b:eb:e4:be:47:5b:ad:56:d9:1b:d0:a6","x_vwirekey":"830a8681cddf9f7d04e2ce2445ea3696","y":367.0225080385852}],"meta":{"rc":"ok"}}';
$json_for_radio='{"data":[{"_id":"53b155c6f5f929f8d8f424fd","adopted":true,"cfgversion":"856eb8e3390d844c","config_network":{"ip":"192.168.1.20","type":"dhcp"},"has_eth1":false,"has_poe_passthrough":false,"inform_ip":"192.168.45.135","inform_url":"http://192.168.45.135:8080/inform","ip":"192.168.45.149","last_seen":1404872734,"locked":true,"mac":"24:a4:3c:8c:07:c0","map_id":"5388665748f029f89fb8d52f","model":"BZ2","name":"abcd","radio_table":[{"antenna_gain":"6","builtin_antenna":true,"channel":"0","ht":"20","max_txpower":20,"mode":"all","radio":"ng","tx_power":"auto","tx_power_mode":"auto"}],"serial":"24A43C8C07C0","version":"2.4.6.2178","vwire_table":[],"x":430.30225080385856,"x_authkey":"52ed781610b6f0033c41f1e5637fe0d9","x_fingerprint":"96:71:8f:4b:6b:eb:e4:be:47:5b:ad:56:d9:1b:d0:a6","x_vwirekey":"830a8681cddf9f7d04e2ce2445ea3696","y":367.0225080385852}],"meta":{"rc":"ok"}}';
$json_for_network = '{"data":[{"_id":"53b155c6f5f929f8d8f424fd","adopted":true,"cfgversion":"0a2a8ea9d1a2c648","config_network":{"type":"dhcp"},"has_eth1":false,"has_poe_passthrough":false,"inform_ip":"192.168.45.135","inform_url":"http://192.168.45.135:8080/inform","ip":"192.168.45.149","last_seen":1404873051,"locked":true,"mac":"24:a4:3c:8c:07:c0","map_id":"5388665748f029f89fb8d52f","model":"BZ2","name":"abcd","radio_table":[{"antenna_gain":"6","builtin_antenna":true,"channel":"0","ht":"20","max_txpower":20,"mode":"all","name":"wifi0","radio":"ng","tx_power":"auto","tx_power_mode":"auto"}],"serial":"24A43C8C07C0","version":"2.4.6.2178","vwire_table":[],"x":430.30225080385856,"x_authkey":"52ed781610b6f0033c41f1e5637fe0d9","x_fingerprint":"96:71:8f:4b:6b:eb:e4:be:47:5b:ad:56:d9:1b:d0:a6","x_vwirekey":"830a8681cddf9f7d04e2ce2445ea3696","y":367.0225080385852}],"meta":{"rc":"ok"}}';
$json_for_wlan='{"data":[{"_id":"53b155c6f5f929f8d8f424fd","adopted":true,"cfgversion":"c924a935ecb9d45a","config_network":{"type":"dhcp"},"has_eth1":false,"has_poe_passthrough":false,"inform_ip":"192.168.45.135","inform_url":"http://192.168.45.135:8080/inform","ip":"192.168.45.149","last_seen":1404873247,"locked":true,"mac":"24:a4:3c:8c:07:c0","map_id":"5388665748f029f89fb8d52f","model":"BZ2","name":"abcd","radio_table":[{"antenna_gain":"6","builtin_antenna":true,"channel":"0","ht":"20","max_txpower":20,"mode":"all","name":"wifi0","radio":"ng","tx_power":"auto","tx_power_mode":"auto"}],"serial":"24A43C8C07C0","version":"2.4.6.2178","vwire_table":[],"wlan_overrides":[],"x":430.30225080385856,"x_authkey":"52ed781610b6f0033c41f1e5637fe0d9","x_fingerprint":"96:71:8f:4b:6b:eb:e4:be:47:5b:ad:56:d9:1b:d0:a6","x_vwirekey":"830a8681cddf9f7d04e2ce2445ea3696","y":367.0225080385852}],"meta":{"rc":"ok"}}';
$json_for_wlan2='{"data":[{"_id":"53b155c6f5f929f8d8f424fd","adopted":true,"cfgversion":"3e07b95970868db6","config_network":{"type":"dhcp"},"has_eth1":false,"has_poe_passthrough":false,"inform_ip":"192.168.45.135","inform_url":"http://192.168.45.135:8080/inform","ip":"192.168.45.149","last_seen":1404873492,"locked":true,"mac":"24:a4:3c:8c:07:c0","map_id":"5388665748f029f89fb8d52f","model":"BZ2","name":"abcd","radio_table":[{"antenna_gain":"6","builtin_antenna":true,"channel":"0","ht":"20","max_txpower":20,"mode":"all","name":"wifi0","radio":"ng","tx_power":"auto","tx_power_mode":"auto"}],"serial":"24A43C8C07C0","version":"2.4.6.2178","vwire_table":[],"wlan_overrides":[{"enabled":true,"name":"testssida","radio":"ng","security":"wpapsk","vlan":"2","vlan_enabled":true,"wlan_id":"538866ed48f029f8a1b8d52f","x_passphrase":"12345678"}],"x":430.30225080385856,"x_authkey":"52ed781610b6f0033c41f1e5637fe0d9","x_fingerprint":"96:71:8f:4b:6b:eb:e4:be:47:5b:ad:56:d9:1b:d0:a6","x_vwirekey":"830a8681cddf9f7d04e2ce2445ea3696","y":367.0225080385852}],"meta":{"rc":"ok"}}';

$json = json_decode($_POST['json']);

$admin=" ";
if(isset($_SESSION["username"]))
{
	$loginip = isset($_SESSION["loginip"]) ? $_SESSION["loginip"] : "";
	$admin = $_SESSION["username"].":".$loginip;
}

if (isset($json->name) && "" != $json->name)
{
	$_id = $json->_id;
	$name = $json->name;
	$ret = ext_wtp("set_wtp_name", $_id, $name);
	echo $json_str;
}
else if (isset($json->radio_table))
{
	$i = 0;
	$_id = $json->_id;
	$radio_table = $json->radio_table;
	$len = count($radio_table);
	error_log("id = $_id len $len  radio_table = ".json_encode($radio_table)."\n");
	
	for ($i = 0; $i < $len; $i++)
	{
		$radio = isset($radio_table[$i]->radio) ? $radio_table[$i]->radio : "";
		$channel = isset($radio_table[$i]->channel) ? $radio_table[$i]->channel : "";
		$tx_power = $radio_table[$i]->tx_power;
		$tx_power_mode = $radio_table[$i]->tx_power_mode;
		//$tx_power = isset($radio_table[$i]->tx_power) ? $radio_table[$i]->tx_power : "";
		//$tx_power_mode = isset($radio_table[$i]->tx_power_mode) ? $radio_table[$i]->tx_power_mode : "";
		$ht = isset($radio_table[$i]->ht) ? $radio_table[$i]->ht : "";
		$mode = isset($radio_table[$i]->mode) ? $radio_table[$i]->mode : "";
		
		if ("ng" == $radio)
		{
			$rid = 0;
		}
		else if ("na" == $radio)
		{
			$rid = 1;
		}
		else
		{
			continue;
		}
		
		if (isset($radio_table[$i]->channel))
		{
			$ret = ext_wtp("set_radio_channel", $_id, $rid, $radio_table[$i]->channel);
		}
		
		if ((isset($radio_table[$i]->tx_power)) || (isset($radio_table[$i]->tx_power_mode)))
		{
			if (("high" == ($tx_power_mode)) || ("medium" == ($tx_power_mode))  || ("low" == ($tx_power_mode)) || ("auto" == ($tx_power_mode)))
			{
				$tx_power = "100";
			}

			$ret = ext_wtp("set_radio_txpower", $_id, $rid, $tx_power_mode, $tx_power);
		}
		
		if ("" != ($ht))
		{
			$ret = ext_wtp("set_radio_cwmode", $_id, $rid, $ht);
		}
		
		if ("" != ($mode))
		{
			$ret = ext_wtp("set_radio_mode", $_id, $rid, $mode);
		}
		
		error_log("i = ".$i." radio = $radio channel = $channel tx_power = $tx_power tx_power_mode = $tx_power_mode ht = $ht mode = $mode\n");
	}	
	echo $json_for_radio;
}
else if (isset($json->config_network))
{
	$_id = $json->_id;
	$config_network = $json->config_network;
	error_log("id = $_id  config_network = ".json_encode($config_network)."\n");

	if ("static" == $config_network->type)
	{
		$ip = $config_network->ip;
		$netmask = $config_network->netmask;
		$gateway = $config_network->gateway;
		$dns1 = $config_network->dns1;
		$dns2 = $config_network->dns2;

		if ((isset($config_network->ip))
			|| (isset($config_network->netmask))
			|| (isset($config_network->gateway))
			|| (isset($config_network->dns1))
			|| (isset($config_network->dns2)))
		{
			$ret = ext_wtp("set_wtp_ip_mask", $_id, $config_network->type, $ip, $netmask, $gateway, $dns1, $dns2);
		}
		
		error_log("id = $_id type = $config_network->type ip = $ip netmask = $netmask gateway = $gateway dns1 = $dns1 dns2 = $dns2\n");
	}
	
	echo $json_for_network;
}
else if (isset($json->wlan_overrides))
{
	$_id = $json->_id;
	
	if ($json->wlan_overrides[0])
	{
		$wlan_overrides = $json->wlan_overrides[0];
	}
	else
	{
		$wlan_overrides = $json->wlan_overrides[1];
	}
	
	error_log("id = $_id  wlan_overrides = ".json_encode($wlan_overrides)."\n");
	
	//wlan_overrides = [{"wlan_id":1,"radio":"ng","enabled":true,"name":"Name241","vlan_enabled":true,"vlan":"100","security":"wpapsk","x_passphrase":"12345678"}]
	
	$radio = $wlan_overrides->radio;
	$wlan_id = $wlan_overrides->wlan_id;
	$name = $wlan_overrides->name;
	$enabled = $wlan_overrides->enabled;
	$vlan_enabled = $wlan_overrides->vlan_enabled;
	$vlan = $wlan_overrides->vlan;
	$security = $wlan_overrides->security;
	$x_passphrase = $wlan_overrides->x_passphrase;
	
	if ("ng" == $radio)
	{
		$rid = 0;
	}
	else if ("na" == $radio)
	{
		$rid = 1;
	}
	else
	{
	}
	
	if ((isset($wlan_overrides->name))
		|| (isset($wlan_overrides->enabled))
		|| (isset($wlan_overrides->vlan_enabled))
		|| (isset($wlan_overrides->vlan))
		|| (isset($wlan_overrides->security))
		|| (isset($wlan_overrides->x_passphrase)))
	{
		$ret = ext_wtp("set_radio_wlan_overrides", $_id, $rid, $wlan_id, $name, $enabled, $vlan_enabled, $vlan, $security, $x_passphrase);
	}
	
	if ($json->wlan_overrides[0])
	{
		echo $json_for_wlan2;
	}
	else
	{
		echo $json_for_wlan;
	}	
}
else if (isset($json->mapname))
{
	$mapname = $json->mapname;
	$left = isset($json->mapleft) ? $json->mapleft : 0;
	$top = isset($json->maptop) ? $json->maptop : 0;
	$lock = (isset($json->maplock) && ($json->maplock == "true")) ? 1 : 0;
	$afiid = isset($json->afiid) ? $json->afiid : 0;
    error_log("$afiid $mapname $left $top $lock $admin");
	$ret = ext_wtp("set_afi_map_config", $afiid, $mapname, $left, $top, $lock, $admin);

	$result = array("data"=>array(), "meta"=>array("rc"=>"ok"));
	echo json_encode($result);
}

?>
