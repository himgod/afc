<?php 

//get country's code
function get_country_code()
{
	$country_code = 156;
	$ret = ext_wtp("get_country_code");
	if (isset($ret->value["CountryCode"])) {
		$country_code = $ret->value["CountryCode"];
	}
	else {
		error_log("function: get_country_code: get country code error!");
	} 
	return $country_code;
}

//get ap's information
function get_aps_info()
{
	$WTP = array();
	$ret = ext_wtp("get_aps_list");
	//$cnt = count($ret->value);
	//echo "cnt= $cnt<br>";
	for ($i = 0; $i < count($ret->value); $i++) {
		if (isset($ret->value[$i]["RadioMac"]))
			$WTP[$i]["RadioMac"] = $ret->value[$i]["RadioMac"];
		if (isset($ret->value[$i]["Channel"]))
			$WTP[$i]["Channel"] = $ret->value[$i]["Channel"];
		if (isset($ret->value[$i]["RadioType"]))
			$WTP[$i]["RadioType"] = $ret->value[$i]["RadioType"];
		if (isset($ret->value[$i]["TxPower"]))
			$WTP[$i]["TxPower"] = $ret->value[$i]["TxPower"];
		//Neighbors' list
		for ($j = 0; isset($ret->value[$i]["Neighbors"]) &&
			$j < count($ret->value[$i]["Neighbors"]); $j++) {    //neighbor list
			if (isset($ret->value[$i]["Neighbors"][$j]["NeighborRadioMac"])) {
				$WTP[$i]["Neighbors"][$j]["NeighborRadioMac"] = $ret->value[$i]["Neighbors"][$j]["NeighborRadioMac"];
			}
			if (isset($ret->value[$i]["Neighbors"][$j]["NeighborRadioRssi"])) {
				$WTP[$i]["Neighbors"][$j]["NeighborRadioRssi"] = $ret->value[$i]["Neighbors"][$j]["NeighborRadioRssi"];
			}
			if (isset($ret->value[$i]["Neighbors"][$j]["NeighborFlag"])) {    //check if the ap belongs to local AC
				$WTP[$i]["Neighbors"][$j]["NeighborFlag"] = $ret->value[$i]["Neighbors"][$j]["NeighborFlag"];
			}
			if (isset($ret->value[$i]["Neighbors"][$j]["NeighborRadioChannel"])) {    //not local AC
				$WTP[$i]["Neighbors"][$j]["NeighborRadioChannel"] = $ret->value[$i]["Neighbors"][$j]["NeighborRadioChannel"];
			}
		}
	}
	return $WTP;
}

//set radio's channel
function set_radio_channel($RadioMac, $RadioChannel)
{
	$ret = ext_wtp("dynamic_select_radio_channel", $RadioMac, $RadioChannel);
	if ($ret < 0) {
		error_log("set_radio_channel error!");
	}
}
?>