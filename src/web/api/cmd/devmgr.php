<?php
include($_SERVER["DOCUMENT_ROOT"]."/api/expired_check.php");
$json=json_decode($_REQUEST['json']);
$_id = 0;
if (isset($json->_id))
{
	$_id = $json->_id;
}
$data = array();
//$json_str = '{"data":[],"meta":{"rc":"ok"}}';
switch($json->cmd)
{
	case "adopt":
		$name = "";//$json->name;
		$model = "";
		if(isset($json->macs))
		{
			if(count($json->macs) > 0)
			{
				$macs = $json->macs;
				if(isset($json->models))
				{
					$models = $json->models;
				}
				$admin="";
				if(isset($_SESSION["username"]))
				{
					$loginip = isset($_SESSION["loginip"]) ? $_SESSION["loginip"] : "";
					$admin = $_SESSION["username"].":".$loginip;
				}
				for($i = 0; $i < count($macs); $i++)
				{				
					$model = "";
					if(isset($models[$i]))
					{
						$model = $models[$i];
					}
					$ret = ext_wtp("adopt_new_wtp", $name, $macs[$i], $model, $admin);
				}
			}
		}
		else
		{
			$mac = "";
			if(isset($json->mac))
			{
				$mac = $json->mac;
			}
			if(isset($json->model))
			{
				$model = $json->model;
			}
			$admin = "";
			if(isset($_SESSION["username"]))
			{
				$admin = $_SESSION["username"].":".$_SESSION["loginip"];
			}
			$ret = ext_wtp("adopt_new_wtp", $name, $mac, $model, $admin);
			if(!(is_object($ret) && isset($ret->value) && isset($ret->value["result"])) ||
				((0 != $ret->value["result"]) || (!isset($ret->value["wtpid"])) || (0 == $ret->value["wtpid"])))		
			{
				$meta["rc"] = "error";
				$meta["msg"] = "api.err.OperationFailed";
			}
		}
		break;
	case "adv-adopt":
	case "set-vwire-uplink":
	case "unset-vwire-uplink":
	case "scan-uplink":
		break;
	case "locate":
		/*do nothing*/	
		$switch_value = (isset($json->switch_value))?$json->switch_value:"on";/* {cmd: "locate", switch_value:"on"/*off* /} */
		$mac = "";
		$ip = "";
		if (isset($json->mac))
		{
			$mac = $json->mac;
		}
		if (isset($json->ip))
		{
			$ip = $json->ip;
		}
		//$apverinfo = get_ap_version_info();
		
		$cmd = $_SERVER["DOCUMENT_ROOT"]."/api/cmd/led_switch.sh ".$ip." ".$switch_value;
		$msg = system($cmd);
		$ledhash = isset($_SESSION["led_switch_values"]) ? $_SESSION["led_switch_values"]:array();
		$ledhash[$mac] = ($switch_value == "on") ? "locating":"located";
		$_SESSION["led_switch_values"] = $ledhash;
		//$meta["msg"] = "upgrade msg: ".$msg;
		break;
	case "set-locate":
	case "unset-locate":
		break;
	case "upgrade":
		$mac = "";
		$ip = "";
		if (isset($json->mac))
		{
			$mac = $json->mac;
		}
		if (isset($json->ip))
		{
			$ip = $json->ip;
		}
		//$apverinfo = get_ap_version_info();
		
		$cmd = $_SERVER["DOCUMENT_ROOT"]."/api/cmd/testupgrade_afi.sh ".$ip;
		$msg = system($cmd);
		//$meta["msg"] = "upgrade msg: ".$msg;
		break;
	case "restart":
		if(isset($_id) && ($_id > 0))
		{
			$mac = "";
			$admin = "";
			if(isset($json->mac))
			{
				$mac = $json->mac;
			}
			if(isset($_SESSION["username"]))
			{
				$admin = $_SESSION["username"].":".$_SESSION["loginip"];
			}
			$ret = ext_wtp("restart_wtp", $_id, $mac, $admin);
			if(0 != $ret)
			{
				$meta["rc"] = "error";
				$meta["msg"] = "api.err.OperationFailed";
				$meta["ret"] = "ret ".$ret;
			}
		}
		break;
	case "delete-ap":
		if(isset($_id))
		{
			$mac = "";
			$admin = "";
			if(isset($json->mac))
			{
				$mac = $json->mac;
			}
			if(isset($_SESSION["username"]))
			{
				$admin = $_SESSION["username"].":".$_SESSION["loginip"];
			}

			$ret = ext_wtp("delete_wtp", $_id, $mac, $admin);
			if(0 != $ret)
			{
				$meta["rc"] = "error";
				$meta["msg"] = "api.err.OperationFailed";
			}
		}
		else
		{
			$meta["rc"] = "error";
			$meta["msg"] = "Bad parameter _id ".$_id;
		}
		break;
	default:
		break;
}

$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);
//echo $json_str;
?>
