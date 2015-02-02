<?php

include ($_SERVER['DOCUMENT_ROOT']."/plugins/plugin_common.php");

function Ap_Channel_Check_2G($channel)
{
	if (1 == $channel || 6 == $channel || 11 == $channel)
		return $channel;
	else if((abs($channel-1)<=2))    //channel may be 1, 2, 3 return 1;
		return 1;
	else if((abs(6-$channel)<=2))    //channel may be 4, 5, 6, 7, 8 return 6;
		return 6;
	else if((abs(11-$channel)<=2))    //channel may be 9, 10, 11, 12 return 11;
		return 11;	
	else
		return $channel;     //other channel unknown	
}

//for code: Israel
function Ap_Channel_Check_2G_Israel($channel)
{
	if (3 == $channel || 8 == $channel || 13 == $channel)
		return $channel;
	else if((abs($channel-3)<=2))    //channel may be 1, 2, 3 return 1;
		return 3;
	else if((abs(8-$channel)<=2))    //channel may be 4, 5, 6, 7, 8 return 6;
		return 8;
	else if((abs(13-$channel)<=2))    //channel may be 9, 10, 11, 12 return 11;
		return 13;	
	else
		return $channel;     //other channel unknown	
}

//for 5.8G
function Ap_Channel_Check_5G($channel)
{
	$valid_channel = array(36, 40, 44, 48, 52, 56, 60, 64, 
		100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 
		149, 153, 157, 161, 165
	);
	for ($i = 0; $i < count($valid_channel); $i++)
		if ($channel == $valid_channel[$i])
			return $channel;
	for ($i = 0; $i < count($valid_channel); $i++)
		if (abs($channel - $valid_channel[$i]) <= 4)
			return $valid_channel[$i];
	if ($i >= count($valid_channel))
		return $channel;
	return $channel;
	/*
	if (149 == $channel || 153 == $channel || 157 == $channel || 161 == $channel || 165 == $channel)
		return $channel;
	else if((abs($channel-149)<=2))    //channel may be 150, 151, return 149;
		return 149;
	else if((abs(153-$channel)<=2))    //channel may be 151, 152, 154, 155 return 151;
		return 153;
	else if((abs(157-$channel)<=2))    //channel may be 155, 156, 158, 159 return 157;
		return 157;	
	else if((abs(161-$channel)<=2))    //channel may be 159, 160, 162, 163 return 161;
		return 161;
	else if((abs(165-$channel)<=2))    //channel may be 163, 164, 166, 167 return 165;
		return 165;
	else
		return $channel;     //other channel unknown
	*/
}


function Ap_Channel_Check($channel, $flag)
{
	switch($flag)
	{
		case 0:   /*2.4G*/
			return Ap_Channel_Check_2G($channel);
		break;
		
		case 1: //5.8G
			return Ap_Channel_Check_5G($channel);
		break;

		default:
			error_log("Not support channel flag!");
			break;
	}
}

function get_neighbor_index($WTP, $Neighbor_RadioMac)
{	
	if (isset($WTP) == false) {
		error_log("get_neighbor_index: Error parameter!");
		return -2;    //error
	}

	$cnt = count($WTP);
	for ($neighbor_idx = 0; $neighbor_idx < $cnt; $neighbor_idx++) {
		if ( 0 == strcasecmp($WTP[$neighbor_idx]["RadioMac"], $Neighbor_RadioMac) )
			return $neighbor_idx;
	}
	return -1;    //can't find 
}





function wtp_get_channel(&$WTP, $idx)
{
	if (isset($WTP) == false || isset($WTP[$idx]) == false) {   //not exists
		error_log("wtp_get_channel: Error parameters: $idx.");
		return;
	}
	$neighbor_idx = 0;
	$neighbor_channel = 0;
	$channel_list = array();
	$channel_check = array();

	$unused_channel_cnt = 0;
	$used_channel_cnt = 0;
	

	if($WTP[$idx]["Flags"] == 1)
		return $WTP[$idx]["Channel"];
	if (!isset($WTP[$idx]["Neighbors"])) {
		error_log("wtp_get_channel: has no neighbors!");
		return;
	}
	$cnt = count($WTP[$idx]["Neighbors"]);
	for($i = 0; $i < $cnt; $i++) {
		if (isset($WTP[$idx]["Neighbors"][$i]["NeighborFlag"]) 
			&& isset($WTP[$idx]["Neighbors"][$i]["NeighborRadioChannel"]) ) {
			$neighbor_idx = $WTP[$idx]["Neighbors"][$i]["NeighborFlag"];
			$NeighborChannel  = $WTP[$idx]["Neighbors"][$i]["NeighborRadioChannel"];
		}
		else {
			error_log("wtp_get_channel: Can't find the index: \"NeighborFlag\"");
			continue;
		}

		//$neighbor_idx = get_neighbor_index($WTP, $NeighborRadioMac);    
		if (-2 == $neighbor_idx) {    //error
			error_log("wtp_get_channel: $neighbor_idx error!");
			return -1;
		}
		if ($neighbor_idx < 0 && $NeighborChannel != 0) {    //not controled by local AC
			for($j = 0; isset($WTP[$idx]["H_channel_list"][$j]); $j++) {
				if ($WTP[$idx]["H_channel_list"][$j] == $NeighborChannel) {
					$WTP[$idx]["H_channel_list"][$j] = 0;
					$channel_check[$used_channel_cnt] = $NeighborChannel;
					$used_channel_cnt++;
					break;
				}
			}
		}else if(1 == $WTP[$neighbor_idx]["Flags"]){    //controled by local AC
			for($j = 0; isset($WTP[$idx]["H_channel_list"][$j]); $j++){
				if ($WTP[$idx]["H_channel_list"][$j] == $WTP[$neighbor_idx]["Channel"]) {
					$WTP[$idx]["H_channel_list"][$j] = 0;
					$channel_check[$used_channel_cnt] = $WTP[$neighbor_idx]["Channel"];
					$used_channel_cnt++;
					break;
				}
			}
		}
	}
	for($i = 0; $i < count($WTP[$idx]["H_channel_list"]); $i++){
		$flags = 0;
		if ($WTP[$idx]["H_channel_list"][$i] != 0){
			for($j = 0; $j < count($WTP[$idx]["N_channel_list"]); $j++){    //find the channels which are not used
				if ($WTP[$idx]["H_channel_list"][$i] == $WTP[$idx]["N_channel_list"][$j]) {
					$flags = 1;
					break;
				}
			}
			if($flags == 0){
				if (isset($WTP[$idx]["Channel"]) && $WTP[$idx]["Channel"] != 0) {
					$channel_list[$unused_channel_cnt] = $WTP[$idx]["H_channel_list"][$i];
					$unused_channel_cnt++;
				}else {
					return $WTP[$idx]["H_channel_list"][$i];
				}
			}
		}
	}
	if($unused_channel_cnt != 0){    //has valid channels
		for($k = 0; $k < $unused_channel_cnt; $k++){
			if(($WTP[$idx]["Channel"] != 0)&&($WTP[$idx]["Channel"] == $channel_list[$k]))
				return $WTP[$idx]["Channel"];    //use past channel
		}
		return $channel_list[0];    //return first valid Channel value
	}


	$N_channel_max_cnt = count($WTP[$idx]["N_channel_list"]);    //Get max channel list

	for($i = $N_channel_max_cnt-1; $i >= 0; $i--) {    //Channel values ares used up
		if($WTP[$idx]["N_channel_list"][$i] != 0){				
			for($j = 0; isset($WTP[$idx]["H_channel_list"][$j]) ; $j++){
				if ($WTP[$idx]["H_channel_list"][$j] == $WTP[$idx]["N_channel_list"][$i]){
					//if($i == 0)
						//echo "WTP[$idx]->channel WTP[idx]['H_channel_list'][$j] may interfere other WTP";
					return $WTP[$idx]["H_channel_list"][$j];    //Choose to use a weaker channel value
				}
			}
			return $WTP[$idx]["N_channel_list"][$i];    //Choose to use a weaker channel value
		}
		if (isset($channel_check[$i]) && $channel_check[$i] != 0)
			return $channel_check[$i];
	}
	for($i = $N_channel_max_cnt-1; $i >= 0; $i--){
		if($channel_check[$i] != 0){
			return $channel_check[$i];
		}
	}	
	return 0;
}

function get_valid_channels($CountryCode)
{
	$ccodes = '[
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140 , 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11] , "code" : "840" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "US" , "name" : "United States"} ,
	{ "channels_na" : [ 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 149 , 153 , 157 , 161] , "channels_na_80" : [ 149 , 153 , 157 , 161] , "channels_na_dfs" : [ ] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "156" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "CN" , "name" : "China"} ,
	{ "channels_na" : [ 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140 , 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 60 , 64 , 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_80" : [ 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11] , "code" : "158" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "TW" , "name" : "Taiwan"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64] , "channels_na_40" : [ ] , "channels_na_80" : [ ] , "channels_na_dfs" : [ 52 , 56 , 60 , 64] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14] , "channels_ng_40" : [ ] , "code" : "392" , "has_na" : true , "has_ng" : true , "ht40_na" : false , "ht40_ng" : false , "key" : "JP" , "name" : "Japan"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140 , 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "702" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "SG" , "name" : "Singapore"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140 , 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "36" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "AU" , 
		"name" : "Australia"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "250" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "FR" , "name" : "France"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64] , "channels_na_dfs" : [ 52 , 56 , 60 , 64] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "376" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "IL" , "name" : "Israel"} ,
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "724" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "ES" , "name" : "Spain"} , 
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140 , 149 , 153 , 157 , 161 , 165] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "76" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "BR" , "name" : "Brazil"} , 
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136] , "channels_na_80" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "276" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "DE" , "name" : "Germany"} , 
	{ "channels_na" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140 , 149 , 153 , 157 , 161] , "channels_na_40" : [ 36 , 40 , 44 , 48 , 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 149 , 153 , 157 , 161] , "channels_na_dfs" : [ 52 , 56 , 60 , 64 , 100 , 104 , 108 , 112 , 116 , 120 , 124 , 128 , 132 , 136 , 140] , "channels_ng" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "channels_ng_40" : [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13] , "code" : "410" , "has_na" : true , "has_ng" : true , "ht40_na" : true , "ht40_ng" : true , "key" : "KR" , "name" : "Korea Republic"}
	]';
	$json = json_decode($ccodes);

	switch (json_last_error())
	{
		case JSON_ERROR_NONE:
			error_log("JSON_DECODE: - No Error");
		break;
		case JSON_ERROR_DEPTH:
			error_log("JSON_DECODE:-Maximum stack depth exceeded");
		break;
		case JSON_ERROR_STATE_MISMATCH:
			error_log("JSON_DECODE:- Underflow or the modes mismatch");
		break;
		case JSON_ERROR_CTRL_CHAR:
			error_log("JSON_DECODE:- Unexpected control character found");
		break;
		case JSON_ERROR_SYNTAX:
			error_log("JSON_DECODE:- Syntax error, malformed JSON");
		break;
		case JSON_ERROR_UTF8:
			error_log("JSON_DECODE:- Malformed UTF-8 characters, possibly incorrectly encoded");
		break;
		default:
			error_log("JSON_DECODE:- Unknown error");
		break;
	}
	$channel_list = array ( "ng"=>array(), "na"=>array());
	for ($k = 0; isset($json[$k]); $k++) {
		if (isset($json[$k]->{'code'}) && is_string($json[$k]->{'code'})){
			if ($json[$k]->{'code'} == $CountryCode) 
				break;
		}
	}
	if ($json[$k]->{'has_ng'}) {
		for ($i = 0, $j = 0; $i < count($json[$k]->{'channels_ng'}); $i = $i + 5) {
			$channel_list["ng"][$j++] = $json[$k]->{'channels_ng'}[$i];
		}
	}
	if ($json[$k]->{'has_na'}) {
		for ($i = 0; $i < count($json[$k]->{'channels_na'}); $i++) {
			$channel_list["na"][$i] = $json[$k]->{'channels_na'}[$i];
		}
	}
	
	return $channel_list;
}

function sort_neighbor_list_as_rssi(&$WTP)
{
	if (isset($WTP) == false) {
		error_log("sort_neighbor_list_as_rssi: Error parameter!");
		return;    //error
	}
	$ap_nums = count($WTP);
	$seq = array();
	
	for ($k = 0; $k < $ap_nums; $k++) {
		$seq[$k]["len"] = count($WTP[$k]["Neighbors"]);
		$seq[$k]["idx"] = $k;
	}
	foreach ($seq as $key => $value) {
		$lens[$key] = $value["len"];
		$idx[$key] = $value["idx"];
	}
	array_multisort($lens, SORT_NUMERIC, SORT_ASC, $idx, SORT_NUMERIC, $seq);
	
	
	for ($i = 0; $i < $ap_nums; $i++) {
		$index = $seq[$i]["idx"];
		foreach($WTP[$index]["Neighbors"] as $key => $value) {
			$Rssi[$key] = $value["NeighborRadioRssi"];
			$mac[$key] = $value["NeighborRadioMac"];
		}
		array_multisort($Rssi, SORT_NUMERIC, SORT_DESC, $mac, SORT_STRING, $WTP[$index]["Neighbors"]);
	}

	
	/*
	for ($i = 0; $i < $ap_nums; $i++) {    //check the sort's result
		print_r($WTP[$i]["Neighbors"]);
		echo "<br>";
	}
	*/
}

function print_neighbor_list($WTP)
{
	if (isset($WTP) == false) {
		error_log("PRINT_NEIGHBOR_LIST: Error parameter!");
		return -2;    //error
	}
	for ($i = 0; $i < count($WTP); $i++) {
		echo "--->NO.".$i."  ";
		echo "  MAC: ";
		echo $WTP[$i]["RadioMac"];
		echo "  ";
		echo "  RadioType: ";
		if ($WTP[$i]["RadioType"] == 0)
			echo "  2.4G";
		else if (1 == $WTP[$i]["RadioType"])
			echo "  5.8G";
		echo "    Channel: ";
		echo $WTP[$i]["Channel"];
		echo "  ";
		
		echo "<br> Neighbors lists---> <br>";
		for ($j = 0; isset($WTP[$i]["Neighbors"]) && $j < count($WTP[$i]["Neighbors"]); $j++) {	 
			echo "        ";
			echo $WTP[$i]["Neighbors"][$j]["NeighborRadioMac"];
			echo "  ";
			if ($WTP[$i]["Neighbors"][$j]["NeighborFlag"] < 0)
				echo "  Not local AP";
			else
				echo "!!!   Local AP";
			echo "   RSSI: ";
			echo $WTP[$i]["Neighbors"][$j]["NeighborRadioRssi"];
			echo "  Channel: ";
			echo $WTP[$i]["Neighbors"][$j]["NeighborRadioChannel"];
			echo "<br>";
		}
		echo "<br>";
	}
}


function init_wtp_info(&$WTP)
{
	if (isset($WTP) == false) {
		error_log("init_wtp_info: Error parameter!");
		return -2;    //error
	}
	//Get country code
	$country_code = get_country_code();
	$channel_list = get_valid_channels($country_code);
		
	
	$ret = get_aps_info();
	for ($i = 0; isset($ret) && $i < count($ret); $i++) {
		if (isset($ret[$i]["RadioMac"]))
			$WTP[$i]["RadioMac"] = $ret[$i]["RadioMac"];
		else
			error_log("Not find RadioMac");
		if (isset($ret[$i]["Channel"]))
			$WTP[$i]["Channel"] = $ret[$i]["Channel"];
		else
			error_log("Not find Channel");
		$WTP[$i]["Flags"] = 0;
		if (isset($ret[$i]["RadioType"])) { 
			$WTP[$i]["RadioType"] = $ret[$i]["RadioType"];
			if(0 == $ret[$i]["RadioType"]) {
				for ($j = 0; $j < count($channel_list['ng']); $j++) {
					$WTP[$i]["H_channel_list"][$j] = $channel_list['ng'][$j];
					$WTP[$i]["N_channel_list"][$j] = 0;
				}
			} else if (1 == $ret[$i]["RadioType"]) {
				for ($j = 0; $j < count($channel_list['na']); $j++) {
					$WTP[$i]["H_channel_list"][$j] = $channel_list['na'][$j];
					$WTP[$i]["N_channel_list"][$j] = 0;
				}
			}
		}
		else
			error_log("Not find RadioType");
		for ($j = 0; isset($ret[$i]["Neighbors"]) 
			&& $j < count($ret[$i]["Neighbors"]); $j++) {    //neighbor list
			//error_log("Neighbors lists............".$ret[$i]["Neighbors"][$j]["NeighborRadioChannel"]);
			if (isset($ret[$i]["Neighbors"][$j]["NeighborRadioMac"])) {
				$WTP[$i]["Neighbors"][$j]["NeighborRadioMac"] = $ret[$i]["Neighbors"][$j]["NeighborRadioMac"];
			} else {
				error_log("Not find NeighborRadioMac");
			}
			if (isset($ret[$i]["Neighbors"][$j]["NeighborRadioRssi"])) {
				$WTP[$i]["Neighbors"][$j]["NeighborRadioRssi"] = $ret[$i]["Neighbors"][$j]["NeighborRadioRssi"];
			} else {
				error_log("Not find NeighborRadioMac");
			}
			if (isset($ret[$i]["Neighbors"][$j]["NeighborFlag"])) {
				$neighbor_flag = $ret[$i]["Neighbors"][$j]["NeighborFlag"];
				$WTP[$i]["Neighbors"][$j]["NeighborFlag"] = $neighbor_flag;
			}else {
				echo "hhhahahhahahahahhaha<br>";
				error_log("Not find NeighborFlag");
			}
			if ($neighbor_flag < 0 && isset($ret[$i]["Neighbors"][$j]["NeighborRadioChannel"])) {    //not local AC
				$WTP[$i]["Neighbors"][$j]["NeighborRadioChannel"] = 
					Ap_Channel_Check($ret[$i]["Neighbors"][$j]["NeighborRadioChannel"], $ret[$i]["RadioType"]);
			}
			else if ($neighbor_flag >= 0){
				$WTP[$i]["Neighbors"][$j]["NeighborRadioChannel"] = 0;
			}
		}	
	}
	print_neighbor_list($WTP);
	//sort_neighbor_list_as_rssi($WTP);
}


function check_WTP_and_Neighbors_channel(&$WTP, $idx)
{
	if (isset($WTP) == false || isset($WTP[$idx]) == false) {   //not exists
		error_log("check_WTP_and_Neighbors_channel: Error parameters!");
		return;
	}
	$channel_list_N = array();
	$n_channel_idx = 0;
	$channel = wtp_get_channel($WTP, $idx);
	if($channel)
		$WTP[$idx]["Channel"] = $channel;
	$WTP[$idx]["Flags"] = 1;
	$channel_list_N[$n_channel_idx++] = $channel;
	

	for($i = 0; isset($WTP[$idx]["Neighbors"][$i]); $i++){
		if (isset($WTP[$idx]["Neighbors"][$i]["NeighborFlag"])) {
			$neighbor_idx = $WTP[$idx]["Neighbors"][$i]["NeighborFlag"];
		}
		else
			continue;
		//$neighbor_idx = get_neighbor_index($WTP, $neighbor_radiomac);
		if (-2 == $neighbor_idx) {
			error_log("check_WTP_and_Neighbors_channel: neighbor_idx-->". $neighbor_idx." error!\n");
			return;
		}
		if ($neighbor_idx < 0)
			continue;
		//echo "idx: $idx, neighbor_idx = $neighbor_idx <br>";
		if(0 == $WTP[$neighbor_idx]["Flags"]) {
			for ($j = 0; isset($channel_list_N[$j]); $j++)
				$WTP[$neighbor_idx]["N_channel_list"][$j] = $channel_list_N[$j];
			$channel = wtp_get_channel($WTP, $neighbor_idx);
			if($channel){
				$WTP[$neighbor_idx]["Channel"] = $channel;				
			}			
			$WTP[$neighbor_idx]["Flags"] = 1;
			for ($j = 0; isset($channel_list_N[$j]); $j++) {
				if ($channel_list_N[$j] == $channel) {
					break;
				}
			}
			if ($j >= count($channel_list_N))
				$channel_list_N[$n_channel_idx++] = $channel;
		}
	}
	return;
}


function init_wtp_channel(&$WTP)
{
	if (isset($WTP) == false) {   //not exists
		error_log("Init_wtp_channel: Error parameters.");
		return;
	}
	//global $channel_list0;
	//global $channel_list1;
	$country_code = get_country_code();
	$channel_list = get_valid_channels($country_code);
	
	$index0 = 0;
	$index1 = 0;
	$channel_2_max = count($channel_list['ng']);
	$channel_5_max = count($channel_list['na']);

	for ($idx = 0; isset($WTP[$idx]); $idx++) {
		$index0 = $index0 % $channel_2_max;
		$index1 = $index1 % $channel_5_max;
		if ($WTP[$idx]["RadioType"] == 0 
			&& $WTP[$idx]["Channel"] == 0) {
			$WTP[$idx]["Channel"] = $channel_list['ng'][$index0++];
			//echo "2.4G <br>";
		}
		else if ($WTP[$idx]["RadioType"] == 1 
			&& $WTP[$idx]["Channel"] == 0) {
			//echo "5G <br>";
			$WTP[$idx]["Channel"] = $channel_list['na'][$index1++];
		}
	}
				
}

function check_neighbors_list($WTP)
{
	if (isset($WTP) == false) {   //not exists
		error_log("Check_neighbor_list: Error parameters.");
		return -2;
	}
	$cnt = count($WTP);
	for ($idx = 0; isset($WTP) && $idx < $cnt; $idx++) {
		for ($i = 0; isset($WTP[$idx]["Neighbors"]) && $i < count($WTP[$idx]["Neighbors"]); $i++)
			if (isset($WTP[$idx]["Neighbors"][$i]["NeighborRadioChannel"])) {
				if ($WTP[$idx]["Neighbors"][$i]["NeighborRadioChannel"] == 0)
					continue;
				else
					return 0;
			} else
				continue;
	}
	return 1;
}



function dynamtic_channel_selection(&$output_param)
{
	$WTP_INFO = array();
	
	init_wtp_info($WTP_INFO);
	$result = check_neighbors_list($WTP_INFO);
	if ($result) {
		echo "Init Channel list: <br>";
		init_wtp_channel($WTP_INFO);
	}
	else {
		for ($idx = 0; isset($WTP_INFO[$idx]); $idx++) {
			check_WTP_and_Neighbors_channel($WTP_INFO, $idx);
		}
	}

	echo "<table width=800 height=70 border=1 cellspacing=1> "; 
	echo "<th width=150 scope=col>MACµØÖ·</th> ";
	echo "<th width=100 scope=col>2G</th> ";
 	echo "<th width=100 scope=col>5G</th>";
  	echo "</tr>";
	$n = count($WTP_INFO);

	for ($i = 0; $i < $n; $i++) {
		if ($WTP_INFO[$i]["RadioType"] == 0){
			echo "<tr><td>".$WTP_INFO[$i]["RadioMac"]."</td>";
			echo "<td>".$WTP_INFO[$i]["Channel"]."</td>";
			for ($k = 0; $k < $n; $k++) {
				if ($WTP_INFO[$k]["RadioType"] == 1 
					&& 0 == strncmp($WTP_INFO[$k]["RadioMac"], $WTP_INFO[$i]["RadioMac"], 16)) {
					echo "<td>".$WTP_INFO[$k]["Channel"]."</td>";
					break;
				}
			}
		}
		
	}
	echo "</table>";

	for ($i = 0; isset($WTP_INFO[$i]); $i++) {
		echo $WTP_INFO[$i]["RadioMac"]."--->";
		echo $WTP_INFO[$i]["Channel"]."<br>";
	}
	
	for ($i = 0; isset($WTP_INFO[$i]); $i++) {
		$output_param[$i]["RadioMac"]  = $WTP_INFO[$i]["RadioMac"];
		$output_param[$i]["Channel"]   = $WTP_INFO[$i]["Channel"];
	}
}
?>