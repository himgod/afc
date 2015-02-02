<?php
include("expired_check.php");
include("encoding_json.php");

if(isset($_POST["json"]))
{
	$json = json_decode($_POST["json"]);
}
else
{
	$json = "";
}
//echo $json;
if(isset($json->o))
{
	$o = $json->o;
}
else
{
	$o = "";
}

$meta=array("rc"=>"ok");

if($o == "wlan")
{
	$ret = ext_wireless("show_wireless_network_list");

	$data = array();
	$i = 0;
	$j = 0;
	if(isset($ret->wlans["wlan_num"]))
	{
		$wlan_num = $ret->wlans["wlan_num"];	
		if(($wlan_num > 0))
		{
			for($i = 0; $i < $wlan_num; $i++)
			{
				$data[$j] = "";
				$data[$j]["_id"] = $ret->value[$i]["_id"];
				$data[$j]["wlan"] = encoding_chinese($ret->value[$i]["name"]);
				$data[$j]["bytes"] = isset($ret->value[$i]["bytes"])?$ret->value[$i]["bytes"]:0;
				$data[$j]["num_sta"] = isset($ret->value[$i]["num_sta"])?$ret->value[$i]["num_sta"]:0;
				$data[$j]["o"] = "wlan";
				$data[$j]["type"] = "current";
				$j++;				
			}
		}
	}
	$result=array("data"=>$data, "meta"=>$meta);
	echo json_encode($result);
	return ;
}
else //if($o == "ap")
{
	$ret = ext_wtp("wtp_list");
	$hash = array();
	$i = 0;
	$j = 0;
	if(isset($ret->wtps["wtp_num"]) && $ret->wtps["wtp_num"] > 0)
	{
		for($i = 0; $i < $ret->wtps["wtp_num"]; $i++)
		{
			if(!isset($hash[$ret->value[$i]["mac"]]) || 1 != $hash[$ret->value[$i]["mac"]])
			{
				if($ret->value[$i]["adopted"])
				{
					$data[$j]["_id"] = $ret->value[$i]["_id"];
					$data[$j]["ap"] = $ret->value[$i]["mac"];
					$data[$j]["bytes"] = isset($ret->value[$i]["bytes"])?$ret->value[$i]["bytes"]:0;
					$data[$j]["num_sta"] = isset($ret->value[$i]["num_sta"])?$ret->value[$i]["num_sta"]:0;
					$data[$j]["type"] = "current";
					$data[$j]["o"] = "ap";
					$j++;
				}
				$hash[$ret->value[$i]["mac"]] = 1;
			}
		}
	}
	else
	{
		$data = array();
	}
   $result=array("data"=>$data, "meta"=>$meta);
   echo json_encode($result);
   return ;
}

/*
echo '{ 
  "data" : [ 
						{ "_id" : "53b21ddf72b4cbf84b46c177" , 
						  "ap" : "24:a4:3c:8c:07:c0" , 
						  "bytes" : 252540 , 
						  "guest-ng-num_sta" : 1 , 
						  "guest-ng-rx_bytes" : 20100 , 
						  "guest-ng-rx_packets" : 66 , 
						  "guest-ng-time_delta" : 1404182829 , 
						  "guest-ng-tx_bytes" : 232440 , 
						  "guest-ng-tx_dropped" : 43 , 
						  "guest-ng-tx_packets" : 857 , 
						  "guest-num_sta" : 1 , 
						  "guest-rx_bytes" : 20100 , 
						  "guest-rx_packets" : 66 , 
						  "guest-time_delta" : 1404182829 , 
						  "guest-tx_bytes" : 232440 , 
						  "guest-tx_dropped" : 43 , 
						  "guest-tx_packets" : 857 , 
						  "ng-num_sta" : 1 , 
						  "ng-rx_bytes" : 20100 , 
						  "ng-rx_packets" : 66 , 
						  "ng-time_delta" : 1404182829 , 
						  "ng-tx_bytes" : 232440 , 
						  "ng-tx_dropped" : 43 , 
						  "ng-tx_packets" : 857 , 
						  "num_sta" : 1 , 
						  "o" : "ap" , 
						  "rx_bytes" : 20100 , 
						  "rx_packets" : 66 , 
						  "time_delta" : 1404182829 , 
						  "tx_bytes" : 232440 , 
						  "tx_dropped" : 43 , 
						  "tx_packets" : 857 , 
						  "type" : "current" , 
						  "uplink-rx_bytes" : 247794 , 
						  "uplink-rx_packets" : 2675 , 
						  "uplink-time_delta" : 1404182829 , 
						  "uplink-tx_bytes" : 789406 , 
						  "uplink-tx_packets" : 1085
						} , 
						{ "_id" : "53b1583872b4cbf84b46c149" , 
							"bytes" : 6012766 , 
							"guest-ng-num_sta" : 1 , 
							"guest-ng-rx_bytes" : 20100 , 
							"guest-ng-rx_packets" : 66 , 
							"guest-ng-time_delta" : 7020754138 , 
							"guest-ng-tx_bytes" : 5992666 , 
							"guest-ng-tx_dropped" : 6431 , 
							"guest-ng-tx_packets" : 22759 , 
							"guest-num_sta" : 1 , 
							"guest-rx_bytes" : 20100 , 
							"guest-rx_packets" : 66 , 
							"guest-time_delta" : 7020754138 , 
							"guest-tx_bytes" : 5992666 , 
							"guest-tx_dropped" : 6431 , 
							"guest-tx_packets" : 22759 , 
							"ng-num_sta" : 1 , 
							"ng-rx_bytes" : 20100 , 
							"ng-rx_packets" : 66 , 
							"ng-time_delta" : 7020754138 , 
							"ng-tx_bytes" : 5992666 , 
							"ng-tx_dropped" : 6431 , 
							"ng-tx_packets" : 22759 , 
							"num_sta" : 1 , 
							"o" : "wlan" , 
							"rx_bytes" : 20100 , 
							"rx_packets" : 66 , 
							"time_delta" : 7020754138 , 
							"tx_bytes" : 5992666 , 
							"tx_dropped" : 6431 , 
							"tx_packets" : 22759 , 
							"type" : "current" , 
							"wlan" : "testssid"
						} , 
						{ "_id" : "53b1583872b4cbf84b46c14a" , 
						  "bytes" : 6012766 , 
						  "guest-ng-rx_bytes" : 20100 , 
						  "guest-ng-rx_packets" : 66 , 
						  "guest-ng-time_delta" : 7020754138 , 
						  "guest-ng-tx_bytes" : 5992666 , 
						  "guest-ng-tx_dropped" : 6431 , 
						  "guest-ng-tx_packets" : 22759 , 
						  "guest-rx_bytes" : 20100 , 
						  "guest-rx_packets" : 66 , 
						  "guest-time_delta" : 7020754138 , 
						  "guest-tx_bytes" : 5992666 , 
						  "guest-tx_dropped" : 6431 , 
						  "guest-tx_packets" : 22759 , 
						  "map" : "5388665748f029f89fb8d52f" , 
						  "ng-rx_bytes" : 20100 , 
						  "ng-rx_packets" : 66 , 
						  "ng-time_delta" : 7020754138 , 
						  "ng-tx_bytes" : 5992666 , 
						  "ng-tx_dropped" : 6431 , 
						  "ng-tx_packets" : 22759 , 
						  "o" : "map" , 
						  "rx_bytes" : 20100 , 
						  "rx_packets" : 66 , 
						  "time_delta" : 7020754138 , 
						  "tx_bytes" : 5992666 , 
						  "tx_dropped" : 6431 , 
						  "tx_packets" : 22759 , 
						  "type" : "current"
						  }
						 ] , 
	 "meta" : { "rc" : "ok"}
}';*/
?>
