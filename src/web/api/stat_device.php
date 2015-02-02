<?php
include("expired_check.php");
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");

$data=array();
$meta["rc"] = "ok";

$json = json_decode($_REQUEST['json']);

if(isset($json->state))
{
	$state = $json->state;
}
if(isset($json->default))
{
	$default = $json->default;
}
$ret = ext_wtp("wtp_list");
$hash = array();
$apverinfo = get_ap_version_info();

if(isset($ret->wtps["wtp_num"]) && $ret->wtps["wtp_num"] > 0)
{
	$ledhash = isset($_SESSION["led_switch_values"]) ? $_SESSION["led_switch_values"] : array();
	for($i = 0; $i < $ret->wtps["wtp_num"]; $i++)
	{
		if(!isset($hash[$ret->value[$i]["mac"]]) || 1 != $hash[$ret->value[$i]["mac"]])
		{
			if((!isset($state) || $state == $ret->value[$i]["state"]) && 
				((!isset($default) || $default == $ret->value[$i]["default"])))
			{
				$data[$i] = $ret->value[$i];
				$data[$i]["uplink_table"] = array();
				if(isset($data[$i]["version"]) && isset($apverinfo["version"]) 
					&& "v".$data[$i]["version"] != $apverinfo["version"])
				{
					$data[$i]["upgradable"] = $apverinfo["version"];
				}
			}
			if(true != $data[$i]["adopted"])
			{
				if(isset($ledhash[$data[$i]["mac"]]))
				{
					$data[$i]["locate_state"] = $ledhash[$data[$i]["mac"]];
				}
				else
				{
					$data[$i]["locate_state"] = "unlocated";
				}
			}
			
			
			//$data[$i]["adopted"] = true;
			$hash[$ret->value[$i]["mac"]] = 1;/* if adopted and discovery items both exist ignore the discovery item */
		}
	}
	
	$_SESSION["led_switch_values"] = $ledhash;
}

//$meta["msg"] = json_encode($data);
$result=array("data"=>$data, "meta"=>$meta);

echo json_encode($result);
?>
