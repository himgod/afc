<?php
$aps_current = array();
$stas_current = array();
$users_life = array();
$data = array();
$meta["rc"] = "ok";

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
			if(isset($ret->value[$i]["adopted"]) && $ret->value[$i]["adopted"])
			{
				$aps_current[$j]["_id"] = $ret->value[$i]["_id"];
				$aps_current[$j]["ap"] = $ret->value[$i]["mac"];
				$aps_current[$j]["tx_bytes"] = isset($ret->value[$i]["tx_bytes"])?$ret->value[$i]["tx_bytes"]:0;
				$aps_current[$j]["rx_bytes"] = isset($ret->value[$i]["rx_bytes"])?$ret->value[$i]["rx_bytes"]:0;
				$j++;
			}
			$hash[$ret->value[$i]["mac"]] = 1;
		}
	}
}

$ret = ext_wtp("sta_list");
if(is_object($ret) && $ret->stas["sta_num"] > 0)
{
	for($i = 0; $i < $ret->stas["sta_num"]; $i++)
	{
		$stas_current[$i]["_id"] = $ret->value[$i]["_id"];
		$stas_current[$i]["mac"] = $ret->value[$i]["mac"];
		$stas_current[$i]["tx_bytes"] = isset($ret->value[$i]["tx_bytes"])?$ret->value[$i]["tx_bytes"]:0;
		$stas_current[$i]["rx_bytes"] = isset($ret->value[$i]["rx_bytes"])?$ret->value[$i]["rx_bytes"]:0;
	}
}
$data["stas_current"] = $stas_current;
$data["aps_current"] = $aps_current;
$data["users_life"] = $users_life;
$datas = array($data);
$result = array("data"=>$datas, "meta"=>$meta);
echo json_encode($result);
/*
echo '
{ 
	"data" : 
	[ 
		{ 
			"aps_current" : 
			[ 
				{ 
					"ap" : "24:a4:3c:8c:07:c0", 
					"tx_bytes" : "1000", 
					"rx_bytes" : "5000"  
				}
			],
			"stas_current" : 
			[ 
				{ 
					"mac" : "00:1f:64:00:00:ae", 
					"tx_bytes" : "100", 
					"rx_bytes" : "300" 
				}
			], 
			"users_life" : 
			[ 
				{ 
				"user" : "00:1f:64:00:01:02", 
				"tx_bytes" : "1000", 
				"rx_bytes" : "3000" , 
				"duration" : "100" 
				} 
			]
		}
	] , 
	"meta" : 
	{ 
		"rc" : "ok"
	}
}
';
*/
?>
