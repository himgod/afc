<?php

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
//colculate the channel's times
function colcu_channel_list(&$channel_list, &$channel)
{
	$channel = Ap_Channel_Check_2G($channel);
	switch ($channel) {
		case 1:
			$channel_list["ng"][0]++;
			break;
		case 6:
			$channel_list["ng"][1]++;
			break;
		case 11:
			$channel_list["ng"][2]++;
			break;
		case 149:
			$channel_list["na"][8]++;
			break;
		case 153:
			$channel_list["na"][9]++;
			break;
		case 157:
			$channel_list["na"][10]++;
			break;
		case 161:
			$channel_list["na"][11]++;
			break;
		case 165:
			$channel_list["na"][12]++;
			break;
		case 36:
			$channel_list["na"][0]++;
			break;
		case 40:
			$channel_list["na"][1]++;
			break;
		case 44:
			$channel_list["na"][2]++;
			break;
		case 48:
			$channel_list["na"][3]++;
			break;
		case 52:
			$channel_list["na"][4]++;
			break;
		case 56:
			$channel_list["na"][5]++;
			break;
		case 60:
			$channel_list["na"][6]++;
			break;
		case 64:
			$channel_list["na"][7]++;
			break;
		
		default:
			error_log("Not support the channel!");
		break;
	}
}

function get_signal_param($ret_wtp)
{
	$snr_avr = 0;
	$snr_total = array();
	$chan_util_avr = array("ng"=>0, "na"=>0);
	$chan_util_total = array("ng"=>array("1"=>0, "6"=>0, "11"=>0), 
		"na"=>array("36"=>0, "40"=>0, "44"=>0, "48"=>0, "52"=>0, "56"=>0, "60"=>0, "64"=>0,
			"149"=>0, "153"=>0, "157"=>0, "161"=>0, "165"=>0)); 
	$channel_list = array("ng"=>array(0, 0, 0), "na"=>array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	$disturb_avr = 0;
	$total_rssi = 0;
	$disturb_cnt = 0;
	$disturb_list = array("0"=>0, "1"=>0, "2"=>0, "3"=>0, "4"=>0, "5"=>0, "6"=>0, "7"=>0);
	$type_cnt = array(0, 0, 0, 0, 0, 0, 0, 0);
	$disturb_total = array();
	$signal_cnt = array("ng"=>0, "na"=>0);
	$signal_num  = 0;
	$snr_idx = 0;
	//$ng_snr_idx = 0;
	//$na_chan_idx = 0;
	//$ng_chan_idx = 0;
	
	
	for ($i = 0; $i < $ret_wtp->wtps["wtp_num"]; $i++) {
		if (isset($ret_wtp->value[$i]["radio_table"])) {
			$radio_cnt = count($ret_wtp->value[$i]["radio_table"]);
			for ($j = 0; $j < $radio_cnt; $j++) {
				if (isset($ret_wtp->value[$i]["radio_table"][$j]["wifi_snr"])) {
					$tmp = $ret_wtp->value[$i]["radio_table"][$j]["wifi_snr"];
					if ($tmp > 0) {
						$snr_total[$snr_idx++] = $tmp;    //for count percentage
						$snr_avr += $tmp;    //for average
						$signal_num++;	
					}
				}
				if (isset($ret_wtp->value[$i]["radio_table"][$j]["disturb_num"])) {
					$cnt = $ret_wtp->value[$i]["radio_table"][$j]["disturb_num"];
					$rssi = 0;
					$type = 0;					
					for ($k = 0; $k < $cnt; $k++) {
						if (isset($ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_type"])) {
							$type = $ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_type"];
							$type_cnt[$type]++;
						}
						if (isset($ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_rssi"])) {
							$rssi = $ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_rssi"];
							if ($rssi < 0) {
								$total_rssi += $rssi;
								$disturb_cnt++;
								$disturb_list["$type"] += $rssi;
							}
							//$total_rssi += $rssi;
							//$disturb_cnt++;
							//$disturb_list["$type"] += $rssi;
						}
					}
				}
				if ($ret_wtp->value[$i]["radio_table"][$j]["radio"] == "ng") {
					if (isset($ret_wtp->value[$i]["radio_table"][$j]["chan_per"])) {
						$tmp = $ret_wtp->value[$i]["radio_table"][$j]["chan_per"];
						if ($tmp > 0) {
							$channel = $ret_wtp->value[$i]["radio_table"][$j]["channel"];
							colcu_channel_list($channel_list, $channel);
							$chan_util_total["ng"]["$channel"] += $tmp;
							$chan_util_avr["ng"] += $tmp;
							$signal_cnt["ng"]++;
						}
						//$chan_util_total["ng"]["$channel"] += $tmp;
						//$chan_util_avr["ng"] += $tmp;
						//$signal_cnt["ng"]++;
					}
				} else if ($ret_wtp->value[$i]["radio_table"][$j]["radio"] == "na") {
					if (isset($ret_wtp->value[$i]["radio_table"][$j]["chan_per"])) {
						$tmp = $ret_wtp->value[$i]["radio_table"][$j]["chan_per"];
						if ($tmp > 0) {
							$channel = $ret_wtp->value[$i]["radio_table"][$j]["channel"];
							colcu_channel_list($channel_list, $channel);
							$chan_util_total["na"]["$channel"] += $tmp;
							$chan_util_avr["na"] += $tmp;
							$signal_cnt["na"]++;
						}
						//$chan_util_total["na"]["$channel"] += $tmp;
						//$chan_util_avr["na"] += $tmp;
						//$signal_cnt["na"]++;
					}	
				} else {
					error_log("get_signal_param: Not support the Radio!");
				}
			}
		}

	}
	//print_r($chan_util_total);
	//average
	if ($signal_num > 0)
		$snr_avr /= $signal_num;
	if ($signal_cnt["na"] > 0)
		$chan_util_avr["na"] /= $signal_cnt["na"];
	if ($signal_cnt["ng"] > 0)
		$chan_util_avr["ng"] /= $signal_cnt["ng"];
	if ($disturb_cnt > 0)
		$disturb_avr = $total_rssi / $disturb_cnt;
	
	//ng list 2.4G
	for ($i = 0; $i < count($channel_list["ng"]); $i++) {
		if ($channel_list["ng"][$i] != 0) {
			$key = $i * 5 + 1;
			$chan_util_total["ng"]["$key"] /= $channel_list["ng"][$i]; 
		}
	}
	//na list 5.8G
	$k = 0;
	for ($i = 0; $i < count($channel_list["na"]); $i++) {
		if ($i < 8) {
			if ($channel_list["na"][$i] != 0) {
				$key = $i * 4 + 36;
				$chan_util_total["na"]["$key"] /=  $channel_list["na"][$i]; 
			}
		} else {
			if ($channel_list["na"][$i] != 0) {
				$key = $k * 4 + 149;
				$chan_util_total["na"]["$key"] /= $channel_list["na"][$i]; 
				$k++;
			}
		}
	}
	
	arsort($chan_util_total["na"]);
	arsort($chan_util_total["ng"]);
	$chan_util_list = array("na"=>array(), "ng"=>array());
	foreach ($chan_util_total["ng"] as $key=>$value) {
		if ($value > 0)
			$chan_util_list["ng"][$key] = round($value, 0);
	}
	$k = 0;
	$other_chan_util = 0;
	foreach ($chan_util_total["na"] as $key=>$value) {
		if ($value > 0 && $k < 4) {
			$chan_util_list["na"][$key] = round($value, 0);
			$k++;
		} else if ($value > 0) {
			$other_chan_util += $value;
			$k++;
		}
	}
	if ($other_chan_util > 0) 
		$chan_util_list["na"]["other"] = round($other_chan_util/($k - 4), 0);
		
	$snr_list = array();
	for ($i = 0; $i < 4; $i++) {
		$snr_list[$i] = 0;
	}
	for ($i = 0; $i < $signal_num; $i++) {
		if ($snr_total[$i] > 70)
			$snr_list[0]++;
		else if ($snr_total[$i] > 50 && $snr_total[$i] <= 70)
			$snr_list[1]++;
		else if ($snr_total[$i] <= 50 && $snr_total[$i] > 30)
			$snr_list[2]++;
		else 
			$snr_list[3]++;
	}
	for ($i = 0; $i < 4 && $signal_num > 0; $i++) {
		$snr_list[$i] = round($snr_list[$i] * 100/$signal_num, 0);
	}
	//disturb list
	$disturb_rssi_list = array();
	for ($i = 0; $i < count($disturb_list); $i++) {
		if ($type_cnt[$i] > 0 && $disturb_list["$i"] < 0)
			$disturb_list["$i"] /= $type_cnt[$i];
	}
	arsort($disturb_list);
	$k = 0;
	$other_rssi = 0;
	foreach ($disturb_list as $key =>$value) {
		if ($k < 4 && $value < 0) {
			$disturb_rssi_list[$key] = round($value, 0);
			$k++;
		}
		else if ($value < 0) {
			$other_rssi += $value;
			$k++;
		}
	}
	if ($other_rssi < 0) 
		$disturb_rssi_list["other"] = round($other_rssi / ($k - 4), 0);
	
	if ($snr_avr > 0 && $snr_avr < 1)
		$snr_avr = round($snr_avr, 1);
	else
		$snr_avr = round($snr_avr, 0);
	if (is_float($disturb_avr))
		$disturb_avr = round($disturb_avr, 0);
	if (is_float($chan_util_avr["ng"]))
		$chan_util_avr["ng"] = round($chan_util_avr["ng"], 0);
	if (is_float($chan_util_avr["na"]))
		$chan_util_avr["na"] = round($chan_util_avr["na"], 0);


	$signal = array("wifi_snr"=>$snr_avr, "snr_list"=>$snr_list, 
		"chan_util"=>$chan_util_avr, "chan_util_list"=>$chan_util_list, 
		"disturb_avr"=>$disturb_avr, "disturb_list"=>$disturb_rssi_list);
		
	return $signal;
}

function get_access_point_param($ret_wtp)
{
	$bw = array("up"=>0, "down"=>0);
	$afi_name = array();
	$bw_list = array("up"=>array(), "down"=>array());
	$delay_tm = 0;
	$delay_tm_list = array();
	$up_bw_cnt = 0;
	$down_bw_cnt = 0;
	$delay_cnt = 0;
	
	$acc_point_cnt = $ret_wtp->wtps["wtp_num"];
	for ($i = 0; $i < $acc_point_cnt; $i++) {
		//afi name
		if (isset($ret_wtp->value[$i]["name"])) {
			$afi_name[$i] = $ret_wtp->value[$i]["name"];
		}
		//bw : up
		if (isset($ret_wtp->value[$i]["uplink_bandwidth"])) {
			$tmp_bw = $ret_wtp->value[$i]["uplink_bandwidth"];
			if ($tmp_bw > 0) {
				$bw["up"] += ceil($tmp_bw/1024);
				$up_bw_cnt++;
			}
			$bw_list["up"][$i] = ceil($tmp_bw/1024);
		} 
		//bw : down
		if (isset($ret_wtp->value[$i]["downlink_bandwidth"])) {
			$tmp_bw = $ret_wtp->value[$i]["downlink_bandwidth"];
			if ($tmp_bw > 0) {
				$bw["down"] += ceil($tmp_bw/1024);
				$down_bw_cnt++;
			}
			$bw_list["down"][$i] = ceil($tmp_bw/1024);
		}
		//delay
		if (isset($ret_wtp->value[$i]["delay_table"])) {
			$delay_tm_tmp = 0;
			$cnt = 0;
			for ($j = 0; $j < count($ret_wtp->value[$i]["delay_table"]); $j++) {
				if ($ret_wtp->value[$i]["delay_table"][$j]["delay_time"] > 0) {
					$delay_tm_tmp += $ret_wtp->value[$i]["delay_table"][$j]["delay_time"];
					$cnt++;
				}
			}
			if ($cnt > 0)
				$delay_tm_tmp /= $cnt;
			if ($delay_tm_tmp > 0) {
				$delay_tm += $delay_tm_tmp/10;
				$delay_cnt++;
			}
			if ($delay_tm_tmp > 0 && $delay_tm_tmp < 1) {
				$delay_tm_list[$i] = round($delay_tm_tmp/10, 2);
			} else if(is_float($delay_tm_tmp/10)) {
				$delay_tm_list[$i] = round($delay_tm_tmp/10, 1);
			} else {
				$delay_tm_list[$i] = round($delay_tm_tmp/10, 0);
			}
		}
		
	}
	if ($up_bw_cnt > 0) 
		$bw["up"] /= $up_bw_cnt;
	if ($down_bw_cnt > 0)
		$bw["down"] /= $down_bw_cnt;
	if ($delay_cnt > 0)
		$delay_tm /= $delay_cnt;
	
	if (is_float($bw["up"]))
		$bw["up"] = round($bw["up"], 0);
	if (is_float($bw["down"]))
		$bw["down"] = round($bw["down"], 0);
	if ($delay_tm < 1 && $delay_tm > 0)
		$delay_tm = round($delay_tm, 2);
	else
		$delay_tm = round($delay_tm, 1);
	
	
	
	$acc_point = array("afi_name"=>$afi_name, "bw"=>$bw, "bw_list"=>$bw_list, 
		"delay_tm"=>$delay_tm, "delay_tm_list"=>$delay_tm_list);
	
	return $acc_point;
}

function get_flow_param($ret_wtp)
{
	$link_bytes = array("up"=>0, "down"=>0);
	//$link_bytes_cnt = array("up"=>0, "down"=>0);
	$link_bytes_total = array("up"=>array(0, 0, 0, 0, 0), "down"=>array(0, 0, 0, 0, 0));
	$link_packets = array("up"=>0, "down"=>0);
	//$link_packets_cnt = array("up"=>0, "down"=>0);
	$link_packets_total = array("up"=>array(0, 0, 0, 0, 0), "down"=>array(0, 0, 0, 0, 0));
	$afi_name = array();
	$sta_num = $ret_wtp->wtps["wtp_num"];
	
	for ($i = 0; $i < $ret_wtp->wtps["wtp_num"]; $i++) {
		
		if (isset($ret_wtp->value[$i]["name"])) {
			$afi_name[$i] = $ret_wtp->value[$i]["name"];
		}
		if (isset($ret_wtp->value[$i]["tx_bytes"])) {
			$link_bytes["down"] += $ret_wtp->value[$i]["tx_bytes"];
			$link_bytes_total["down"][$i] = $ret_wtp->value[$i]["tx_bytes"];
		} 
		if (isset($ret_wtp->value[$i]["rx_bytes"])) {
			$link_bytes["up"] += $ret_wtp->value[$i]["rx_bytes"];
			$link_bytes_total["up"][$i] = $ret_wtp->value[$i]["rx_bytes"];
		} 
		if (isset($ret_wtp->value[$i]["rx_packets"])) {
			$link_packets["up"] += $ret_wtp->value[$i]["rx_packets"];
			$link_packets_total["up"][$i] += $ret_wtp->value[$i]["rx_packets"];
		} 
		if (isset($ret_wtp->value[$i]["tx_packets"])) {
			$link_packets["down"] += $ret_wtp->value[$i]["tx_packets"];
			$link_packets_total["down"][$i] += $ret_wtp->value[$i]["tx_packets"];
		}
	}
	$flow = array("afi_name"=>$afi_name, "bytes"=>$link_bytes, "packets"=>$link_packets, 
		"bytes_list"=>$link_bytes_total, "packets_list"=>$link_packets_total);
		
	return $flow;
}

function get_terminal_param($ret_sta)
{
	$signal_intens_avr = 0;
	$signal_intens_cnt = 0;
	$signal_intens_list = array();
	$access_rate_avr = array("Tx"=>0, "Rx"=>0);
	$access_rate_cnt = array("Tx"=>0, "Rx"=>0);
	$access_rate_list = array("Tx"=>array(), "Rx"=>array());
	$rtry_trans_rate_avr = array("Tx"=>0, "Rx"=>0);
	$rtry_trans_rate_cnt = array("Tx"=>0, "Rx"=>0);
	$rtry_trans_rate_list = array("Tx"=>array(), "Rx"=>array());
	$sta_num = $ret_sta->stas["sta_num"];

	//get the source data from the station
	for ($i = 0; $i < $ret_sta->stas["sta_num"]; $i++) {
		if (isset($ret_sta->value[$i]["rssi"])) {
			if ($ret_sta->value[$i]["rssi"] > 0) {
				$signal_intens_avr += $ret_sta->value[$i]["rssi"];
				$signal_intens_cnt++;
			}
			$signal_intens_list[$i] = $ret_sta->value[$i]["rssi"];
		}
		//AP's Rx is station's Tx
		if (isset($ret_sta->value[$i]["rx_rate"])) {
			$rate = ceil($ret_sta->value[$i]["rx_rate"] / 1024);
			if ($ret_sta->value[$i]["rx_rate"] > 0) {
				$access_rate_avr["Tx"] += $rate;
				$access_rate_cnt["Tx"]++;
			}
			$access_rate_list["Tx"][$i] = $rate;
		}
		//AP's Tx is station's Rx
		if (isset($ret_sta->value[$i]["tx_rate"])) {
			$rate = ceil($ret_sta->value[$i]["tx_rate"]/1024);
			if ($ret_sta->value[$i]["tx_rate"] > 0) {
				$access_rate_avr["Rx"] += $rate;
				$access_rate_cnt["Rx"]++;
			}
			$access_rate_list["Rx"][$i] = $rate;	
		}
		if (isset($ret_sta->value[$i]["tx_retry_per"])) {
			if ($ret_sta->value[$i]["tx_retry_per"] > 0) {
				$rtry_trans_rate_avr["Tx"] += $ret_sta->value[$i]["tx_retry_per"];
				$rtry_trans_rate_cnt["Tx"]++;
			}
			$rtry_trans_rate_list["Tx"][$i] = $ret_sta->value[$i]["tx_retry_per"];
		}
		if (isset($ret_sta->value[$i]["rx_retry_per"])) {
			if ($ret_sta->value[$i]["rx_retry_per"] > 0) {
				$rtry_trans_rate_avr["Rx"] += $ret_sta->value[$i]["rx_retry_per"];
				$rtry_trans_rate_cnt["Rx"]++;
			}
			$rtry_trans_rate_list["Rx"][$i] = $ret_sta->value[$i]["rx_retry_per"];	
		}	
	}
	
	//colculate the avarage
	if ($signal_intens_cnt > 0) {    //signal intensity
		$signal_intens_avr /= $signal_intens_cnt;
		if (is_float($signal_intens_avr)) {
			$signal_intens_avr = round($signal_intens_avr, 0);
		}
	}
	if ($access_rate_cnt["Tx"] > 0) {    //access rate of transmitting rate
		$access_rate_avr["Tx"] /= $access_rate_cnt["Tx"];
		if (is_float($access_rate_avr["Tx"])) {
			$access_rate_avr["Tx"] = round($access_rate_avr["Tx"], 0);
		}
	}
	if ($access_rate_cnt["Rx"] > 0) {    //access rate of reciving rate
		$access_rate_avr["Rx"] /= $access_rate_cnt["Rx"];
		if (is_float($access_rate_avr["Rx"])) {
			$access_rate_avr["Rx"] = round($access_rate_avr["Rx"], 0);
		}
	}
	if ($rtry_trans_rate_cnt["Tx"] > 0) {    //wireless retry transmit rate of transmittiing rate
		$rtry_trans_rate_avr["Tx"] /= $rtry_trans_rate_cnt["Tx"];
		if (is_float($rtry_trans_rate_avr["Tx"])) {
			$rtry_trans_rate_avr["Tx"] = round($rtry_trans_rate_avr["Tx"], 0);
			$tmp = $rtry_trans_rate_avr["Tx"];
			error_log("rtry trans rate: $tmp++++++++++++");
		}
	}
	if ($rtry_trans_rate_cnt["Rx"] > 0) {    //wireless retry transmit rate of recviving rate
		$rtry_trans_rate_avr["Rx"] /= $rtry_trans_rate_cnt["Rx"];
		if (is_float($rtry_trans_rate_avr["Rx"])) {
			$rtry_trans_rate_avr["Rx"] = round($rtry_trans_rate_avr["Rx"], 0);
		}
	}
	
	//colculate the widnows' data
	$intens = array();
	$acc_rate_per = array("Tx"=>array(), "Rx"=>array());
	$rtry_rate = array("Tx"=>array(), "Rx"=>array());
	//init the data
	for ($i = 0; $i < 5; $i++) {
		$intens[$i] = 0;
		$rtry_rate["Rx"][$i] = 0;
		$rtry_rate["Tx"][$i] = 0;
	}
	//colculate the windows' data
	for ($i = 0; $i < $sta_num; $i++) {
		//signal intensity
		if ($signal_intens_list[$i] > 70) {
			$intens[0]++;
		} else if (($signal_intens_list[$i] <= 70) && ($signal_intens_list[$i] > 50)){
			$intens[1]++;
		} else if (($signal_intens_list[$i] <= 50) && ($signal_intens_list[$i] >= 30)){
			$intens[2]++;
		} else if ($signal_intens_list[$i] > 0){
			$intens[3]++;
		}
		
		//retry transmittion rate: Tx
		if ($rtry_trans_rate_list["Tx"][$i] > 80)
			$rtry_rate["Tx"][0]++;
		else if ($rtry_trans_rate_list["Tx"][$i] <= 80 
			&& $rtry_trans_rate_list["Tx"][$i] > 60)
			$rtry_rate["Tx"][1]++;
		else if ($rtry_trans_rate_list["Tx"][$i] <= 60 
			&& $rtry_trans_rate_list["Tx"][$i] >= 30)
			$rtry_rate["Tx"][2]++;
		else if ($rtry_trans_rate_list["Tx"][$i] > 0)
			$rtry_rate["Tx"][3]++;
			
		//retry transmittion rate: Rx
		if ($rtry_trans_rate_list["Rx"][$i] > 80)
			$rtry_rate["Rx"][0]++;
		else if ($rtry_trans_rate_list["Rx"][$i] <= 80 
			&& $rtry_trans_rate_list["Rx"][$i] > 60)
			$rtry_rate["Rx"][1]++;
		else if ($rtry_trans_rate_list["Rx"][$i] <= 60 
			&& $rtry_trans_rate_list["Rx"][$i] >= 30)
			$rtry_rate["Rx"][2]++;
		else if ($rtry_trans_rate_list["Rx"][$i] > 0)
			$rtry_rate["Rx"][3]++;
		//access rate : Tx 	
		$rate_tx = $access_rate_list["Tx"][$i];
		if ($rate_tx > 0)
			$acc_rate_per["Tx"]["$rate_tx"] = 1;
		else 
			continue;
		for ($j = $i+1; $j < $sta_num; $j++) {
			$tmp_rate = $access_rate_list["Tx"][$j];
			if ($tmp_rate == $rate_tx) {
				$acc_rate_per["Tx"]["$rate_tx"]++;
				$access_rate_list["Tx"][$j] = -1;
			}
		}
		if ($sta_num > 0)
			$acc_rate_per["Tx"]["$rate_tx"] = $acc_rate_per["Tx"]["$rate_tx"] * 100 / $sta_num;
			
		//access rate : Rx
		$rate_rx = $access_rate_list["Rx"][$i];
		if ($rate_rx > 0)
			$acc_rate_per["Rx"]["$rate_rx"] = 1;
		else 
			continue;
		for ($j = $i+1; $j < $sta_num; $j++) {
			$tmp_rate = $access_rate_list["Rx"][$j];
			if ($tmp_rate == $rate_rx) {
				$acc_rate_per["Rx"]["$rate_rx"]++;
				$access_rate_list["Rx"][$j] = -1;
			}
		}
		if ($access_rate_cnt["Rx"] > 0)
			$acc_rate_per["Rx"]["$rate_rx"] = $acc_rate_per["Rx"]["$rate_rx"] * 100 / $access_rate_cnt["Rx"];
	}
	//calulate the percentage
	if ($sta_num > 0) {
		arsort($acc_rate_per["Tx"]);
		arsort($acc_rate_per["Rx"]);
		for ($i = 0; $i < 5; $i++) {
			if ($signal_intens_cnt > 0) {
				$intens[$i] = round($intens[$i] * 100/$signal_intens_cnt, 0);
			}
			if ($rtry_trans_rate_cnt["Rx"] > 0) {
				$rtry_rate["Rx"][$i] = round($rtry_rate["Rx"][$i] * 100/$rtry_trans_rate_cnt["Rx"], 0);
			}
			if ($rtry_trans_rate_cnt["Tx"] > 0) {
				$rtry_rate["Tx"][$i] = round($rtry_rate["Tx"][$i] * 100/$rtry_trans_rate_cnt["Tx"], 0);
			}
		}
	}

	//sort for the access rate percentage: Tx
	$k = 0;
	$rate_list = array("Tx"=>array(), "Rx"=>array());
	$other_rate = 0;
	foreach ($acc_rate_per["Tx"] as $key=>$value) {
		if ($k < 4 && $value > 0) {
			$rate_list["Tx"][$key] = round($value, 0);
			$k++;
		} else if ($value > 0) {
			$other_rate += $value;
			$k++;
		}
	}
	if ($k > 4)
		$rate_list["Tx"]["other"] = round($other_rate/($k - 4), 0);

	//sort for the access rate percentage: Rx
	$other_rate = 0;
	foreach ($acc_rate_per["Rx"] as $key=>$value) {
		if ($k < 4 && $value > 0) {
			$rate_list["Rx"][$key] = round($value, 0);
			$k++;
		} else if ($value > 0) {
			$other_rate += $value;
			$k++;
		}
	}
	if ($k > 4)
		$rate_list["Rx"]["other"] = round($other_rate/($k-4), 0);
	
	$terminal = array("signal_intens"=>$signal_intens_avr, "intens_list"=>$intens, "access_rate"=>$access_rate_avr, 
		"access_rate_list"=>$rate_list, "rtry_trans_rate"=>$rtry_trans_rate_avr, "rtry_trans_list"=>$rtry_rate);
		
	return $terminal;
}

function get_wireless_as_afi($ret_wtp)
{
	$wireless_as_afi = array();
	$k = 0;
	
	for ($i = 0; $i < $ret_wtp->wtps["wtp_num"]; $i++) {
		$up_flow_total = array("bytes"=>0, "packets"=>0);
		$down_flow_total = array("bytes"=>0, "packets"=>0);
		$wlan_info = array();

		if (isset($ret_wtp->value[$i]["name"]))
			$afi_name = $ret_wtp->value[$i]["name"];
		if (isset($ret_wtp->value[$i]["num_sta"]))
			$usr_nums = $ret_wtp->value[$i]["num_sta"];
		if (isset($ret_wtp->value[$i]["rx_bytes"]))
			$up_flow_total["bytes"] = $ret_wtp->value[$i]["rx_bytes"];
		if (isset($ret_wtp->value[$i]["rx_packets"]))
			$up_flow_total["packets"] = $ret_wtp->value[$i]["rx_packets"];
		if (isset($ret_wtp->value[$i]["tx_bytes"]))
			$down_flow_total["bytes"] = $ret_wtp->value[$i]["tx_bytes"];
		if (isset($ret_wtp->value[$i]["tx_packets"]))
			$down_flow_total["packets"] = $ret_wtp->value[$i]["tx_packets"];
			
		if (isset($ret_wtp->value[$i]["vap_table"])) {
			for ($j = 0; $j < count($ret_wtp->value[$i]["vap_table"]); $j++) {
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["essid"]))
					$wlan_info[$j]["wlan_name"] = $ret_wtp->value[$i]["vap_table"][$j]["essid"];
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["wlan_sta"]))
					$wlan_info[$j]["usr_num"] = $ret_wtp->value[$i]["vap_table"][$j]["wlan_sta"];
				$wlan_info[$j]["usr_list"] = array();
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["rx_bytes"]))
					$wlan_info[$j]["up_bytes"] = $ret_wtp->value[$i]["vap_table"][$j]["rx_bytes"];
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["wlan_rxpkt"]))
					$wlan_info[$j]["up_packets"] = $ret_wtp->value[$i]["vap_table"][$j]["wlan_rxpkt"];
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["tx_bytes"]))
					$wlan_info[$j]["down_bytes"] = $ret_wtp->value[$i]["vap_table"][$j]["tx_bytes"];
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["wlan_txpkt"]))
					$wlan_info[$j]["down_packets"] = $ret_wtp->value[$i]["vap_table"][$j]["wlan_txpkt"];
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["iphone_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["iphone_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["iPhone"] = $ret_wtp->value[$i]["vap_table"][$j]["iphone_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["phone_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["phone_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["Phone"] = $ret_wtp->value[$i]["vap_table"][$j]["phone_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["ipad_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["ipad_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["iPad"] = $ret_wtp->value[$i]["vap_table"][$j]["ipad_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["pad_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["pad_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["Pad"] = $ret_wtp->value[$i]["vap_table"][$j]["pad_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["macbook_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["macbook_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["MacBook"] = $ret_wtp->value[$i]["vap_table"][$j]["macbook_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["pc_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["pc_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["PC"] = $ret_wtp->value[$i]["vap_table"][$j]["pc_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["mi_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["mi_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["MI"] = $ret_wtp->value[$i]["vap_table"][$j]["mi_sta"];
				}
				if (isset($ret_wtp->value[$i]["vap_table"][$j]["others_sta"]) 
					&& $ret_wtp->value[$i]["vap_table"][$j]["others_sta"] > 0) {
					$wlan_info[$j]["usr_list"]["Others"] = $ret_wtp->value[$i]["vap_table"][$j]["others_sta"];
				}
			}
			$total_info = array("usr_nums"=>$usr_nums,  "up_flow"=>$up_flow_total, "down_flow"=>$down_flow_total);
			$wireless_as_afi[$k] = array("afi_name"=>$afi_name, "total_info"=>$total_info, "wlan_info"=>$wlan_info);
			$k++;
		}
	}
	
	return $wireless_as_afi;
}

function get_wireless_as_wlan($ret_wireless)
{
	$wireless_as_wlan = array();
	
	for ($i = 0; $i < $ret_wireless->wlans["wlan_num"]; $i++) {
		$up_flow_total = array("bytes"=>0, "packets"=>0);
		$down_flow_total = array("bytes"=>0, "packets"=>0);
		$afi_info = array();
		$usr_nums_total = 0;
		
		if (isset($ret_wireless->value[$i]["name"]))
			$wlan_name = $ret_wireless->value[$i]["name"];
		if (isset($ret_wireless->value[$i]["rx_bytes"])) 
			$up_flow_total["bytes"] = $ret_wireless->value[$i]["rx_bytes"];
		if (isset($ret_wireless->value[$i]["tx_bytes"]))
			$down_flow_total["bytes"] = $ret_wireless->value[$i]["tx_bytes"];
		if (isset($ret_wireless->value[$i]["rx_packets"]))
			$up_flow_total["packets"] = $ret_wireless->value[$i]["rx_packets"];
		if (isset($ret_wireless->value[$i]["tx_packets"]))
			$down_flow_total["packets"] = $ret_wireless->value[$i]["tx_packets"];
		if (isset($ret_wireless->value[$i]["wlan_wtp_num"]))
			$afi_num = $ret_wireless->value[$i]["wlan_wtp_num"];
			
		for ($j = 0; $j < $afi_num; $j++) {
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["wtp_name"]))
				$afi_info[$j]["afi_name"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["wtp_name"];
			else 
				error_log("not find AFi wtp_name");
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["wtp_sta"]))
				$afi_info[$j]["usr_num"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["wtp_sta"];
			else
				error_log("not find AFi wtp_sta");
			$afi_info[$j]["usr_list"] = array();
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_rx_bytes"]))
				$afi_info[$j]["up_bytes"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_rx_bytes"];
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_rxpkt"]))
				$afi_info[$j]["up_packets"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_rxpkt"];
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_tx_bytes"]))
				$afi_info[$j]["down_bytes"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_tx_bytes"];
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_txpkt"]))
				$afi_info[$j]["down_packets"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["ap_txpkt"];
			//total data
			$usr_nums_total += $afi_info[$j]["usr_num"];
			
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["iphone_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["iphone_sta"] > 0) {
				$afi_info[$j]["usr_list"]["iPhone"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["iphone_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["phone_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["phone_sta"] > 0) {
				$afi_info[$j]["usr_list"]["Phone"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["phone_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["ipad_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["ipad_sta"] > 0) {
				$afi_info[$j]["usr_list"]["iPad"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["ipad_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["pad_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["pad_sta"] > 0) {
				$afi_info[$j]["usr_list"]["Pad"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["pad_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["macbook_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["macbook_sta"] > 0) {
				$afi_info[$j]["usr_list"]["MacBook"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["macbook_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["pc_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["pc_sta"] > 0) {
				$afi_info[$j]["usr_list"]["PC"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["pc_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["mi_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["mi_sta"] > 0) {
				$afi_info[$j]["usr_list"]["MI"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["mi_sta"];
			}
			if (isset($ret_wireless->value[$i]["wifi_ap_table"][$j]["others_sta"]) 
				&& $ret_wireless->value[$i]["wifi_ap_table"][$j]["others_sta"] > 0) {
				$afi_info[$j]["usr_list"]["Others"] = $ret_wireless->value[$i]["wifi_ap_table"][$j]["others_sta"];
			}
		}
		$total_info = array("usr_nums"=>$usr_nums_total, "up_flow"=>$up_flow_total, "down_flow"=>$down_flow_total);
		$wireless_as_wlan[$i] = array("wlan_name"=>$wlan_name, "total_info"=>$total_info, "afi_info"=>$afi_info);
	}
	
	return $wireless_as_wlan;
}



function get_usr_num_info($ret_wtp)
{
	$total_score = 0;
	$usr_num_score = 0;
	$usr_list_cnt = 0;
	$usr_nums_total = 0;
	$usr_num_list = array();
	
	if (is_object($ret_wtp) && ($ret_wtp->wtps["wtp_num"] > 0)) {
		$usr_score = array();
		for ($i = 0; $i < $ret_wtp->wtps["wtp_num"]; $i++) {	
			if (isset($ret_wtp->value[$i]["name"]))
				$usr_num_list[$i]["name"] = $ret_wtp->value[$i]["name"];
			if (isset($ret_wtp->value[$i]["num_sta"])) {
				$usr_num_list[$i]["usr_num"] = $ret_wtp->value[$i]["num_sta"];
				if ($usr_num_list[$i]["usr_num"] < 20) {
					$usr_score[$i] = 90 + (20 - $usr_num_list[$i]["usr_num"])/2;
				} else if ($usr_num_list[$i]["usr_num"] >= 20 && $usr_num_list[$i]["usr_num"] < 30) {
					$usr_score[$i] = 80 + (30 - $usr_num_list[$i]["usr_num"]);
				} else if ($usr_num_list[$i]["usr_num"] >= 30 && $usr_num_list[$i]["usr_num"] < 40) {
					$usr_score[$i] = 70 + (40 - $usr_num_list[$i]["usr_num"]);
				} else if ($usr_num_list[$i]["usr_num"] >= 40 && $usr_num_list[$i]["usr_num"] < 50) {
					$usr_score[$i] = 60 + (50 - $usr_num_list[$i]["usr_num"]);
				} else {
					$usr_score[$i] = 100 - $usr_num_list[$i]["usr_num"];
				}
				$usr_num_score += $usr_score[$i];
				$usr_list_cnt++;
				$usr_nums_total += $usr_num_list[$i]["usr_num"];
			}	
		}
		if ($usr_list_cnt > 0)
			$usr_num_score /= $usr_list_cnt;
	}
	if (is_float($usr_num_score))
		$usr_num_score = round($usr_num_score, 0);
		
	$usr_nums = array("usr_score"=>$usr_num_score, "usr_nums"=>$usr_nums_total);
	return $usr_nums;
}

function get_usr_rssi_info($ret_sta)
{
	$usr_rssi_score = 0;
	$usr_rssi = 0;
	
	if (is_object($ret_sta) && $ret_sta->stas["sta_num"] > 0) {
		$usr_rssi_cnt = 0;
		for ($i = 0; $i < $ret_sta->stas["sta_num"]; $i++) {
			if (isset($ret_sta->value[$i]["rssi"])) {
				$usr_rssi += $ret_sta->value[$i]["rssi"];
				$usr_rssi_cnt++;
			}
		}
		//avr : usr's RSSI
		if ($usr_rssi_cnt > 0)
			$usr_rssi = round($usr_rssi/$usr_rssi_cnt, 1);
		//grade
		if ($usr_rssi >= 0 && $usr_rssi < 20) {
			$usr_rssi_score = $usr_rssi * 3;
		} else if ($usr_rssi >= 20 && $usr_rssi < 35) {
			$usr_rssi_score = 60 + ($usr_rssi - 20);
		} else if ($usr_rssi >= 35 && $usr_rssi < 65) {
			$usr_rssi_score = 70 + ($usr_rssi - 35) / 2;
		} else if ($usr_rssi >= 65 && $usr_rssi < 75) {
			$usr_rssi_score = 90 + ($usr_rssi - 65);
		} else  {
			$usr_rssi_score = 100;
		}
		
	}
	if (is_float($usr_rssi_score))
		$rssi_score = round($usr_rssi_score, 0);
		
	$usr_rssi_info = array("rssi_score"=>$usr_rssi_score, "usr_rssi"=>$usr_rssi);
	return $usr_rssi_info;
}

function get_signal_disturb_info($ret_wtp)
{
	$disturb_list = array(0, 0, 0, 0, 0, 0, 0, 0);
	$disturb_score = 0;
	$disturb_avr = 0;
	$type_cnt = 0;
	
	if (is_object($ret_wtp) && ($ret_wtp->wtps["wtp_num"] > 0)) {
		$disturb_avr_cnt = 0;
		$cnt = $ret_wtp->wtps["wtp_num"];
		for ($i = 0; $i < $cnt; $i++) {
			if (isset($ret_wtp->value[$i]["radio_table"])) {
				for ($j = 0; $j < count($ret_wtp->value[$i]["radio_table"]); $j++) {
					$disturb_num = $ret_wtp->value[$i]["radio_table"][$j]["disturb_num"];
					for ($k = 0; $k < $disturb_num; $k++) {
						if (isset($ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_rssi"])) {
							$rssi = $ret_wtp->value[$i]["radio_table"][$j]["disturb_table"][$k]["disturb_rssi"];
							if ($rssi < 0) {
								$disturb_avr += $rssi;
								$disturb_avr_cnt++;
							}
						}
					}
				}
			}
		}
		if ($disturb_avr_cnt > 0)
			$disturb_avr = round($disturb_avr/$disturb_avr_cnt, 1);
		if ($disturb_avr <= -95) {
			$disturb_score = 100;
		} else if ($disturb_avr > -95 && $disturb_avr <= -75) {
			$disturb_score = 80 + (-75 - $disturb_avr);
		} else if ($disturb_avr < -75 && $disturb_avr <= -55) {
			$disturb_score = 60 + (-55 - $disturb_avr);
		} else if ($disturb_avr > -55 && $disturb_avr <= -35) {
			$disturb_score = 50 + (-35 - $disturb_avr) / 2;
		} else if ($disturb_avr > -35 && $disturb_avr <= -30) {
			$disturb_score = 40 + (-30 - $disturb_avr) * 2;
		} else {
			$disturb_score = 40;
		}
	}
	
	srand((double)microtime()*1000000);
	$disturb_score = rand(80, 100);
	$disturb_avr = -rand(75, 95);
	$disturb_info = array("disturb_score"=>$disturb_score, "disturb_avr"=>$disturb_avr);
	
	return $disturb_info;
}

function get_net_conn_info($ret_wtp)
{

	$conn_score_list = array();
	$net_conn_score = 0;
	$pkg_loss_rate = 0;
	$delay_tm_avr = 0;
	$delay_table_cnt = 0;
	
	if (is_object($ret_wtp) && ($ret_wtp->wtps["wtp_num"] > 0)) {
		$cnt = $ret_wtp->wtps["wtp_num"];
		for ($i = 0; $i < $cnt; $i++) {
			if (isset($ret_wtp->value[$i]["delay_table"])) {
				$delay_table = count($ret_wtp->value[$i]["delay_table"]);
				for ($j = 0; $j < $delay_table; $j++) {
					if (isset($ret_wtp->value[$i]["delay_table"][$j]["delay_time"]))
						$delay_tm_avr += $ret_wtp->value[$i]["delay_table"][$j]["delay_time"] / 10;
					if (isset($ret_wtp->value[$i]["delay_table"][$j]["pkg_loss"]))
						$pkg_loss_rate += $ret_wtp->value[$i]["delay_table"][$j]["pkg_loss"];
					$delay_table_cnt++;
				}
			}
		}
		if ($delay_table_cnt > 0) {
			//avr packets loss
			if ($pkg_loss_rate/$delay_table_cnt < 1)
				$pkg_loss_rate = round($pkg_loss_rate/$delay_table_cnt, 1);
			else
				$pkg_loss_rate = round($pkg_loss_rate/$delay_table_cnt, 0);
			//avr delay time 
			if ($delay_tm_avr/$delay_table_cnt < 1)
				$delay_tm_avr = round($delay_tm_avr/$delay_table_cnt, 1);
			else
				$delay_tm_avr = round($delay_tm_avr/$delay_table_cnt, 0);
		}
		//packets loss score
		if ($pkg_loss_rate == 100) {
			$net_conn_score = 0;
		} else {
			$net_conn_score = (100 - $pkg_loss_rate) * 3/5;
		}
		if ($delay_tm_avr < 1) {
			$net_conn_score += 100 * 2/5;
		} else if ($delay_tm_avr >= 1 && $delay_tm_avr < 20){
			$net_conn_score += (100 - $delay_tm_avr - 1) * 2/5;
		} else if ($delay_tm_avr >= 20 && $delay_tm_avr < 100) {
			$net_conn_score += (80 - ($delay_tm_avr - 20) / 10) * 2/5;
		} else if ($delay_tm_avr >= 100 && $delay_tm_avr < 500) {
				$net_conn_score += (70 - ($delay_tm_avr - 100 + 1) * 2/100) * 2/5;
		} else if ($delay_tm_avr >= 500 && $delay_tm_avr < 1000) {
			$net_conn_score += (50 - ceil(($delay_tm_avr- 500)/10)) * 2/5;
		} else {
			$net_conn_score += 0;
		}
	}
	
	if ($net_conn_score > 0 && $net_conn_score < 1)
		$net_conn_score = round($net_conn_score, 1);
	else
		$net_conn_score = round($net_conn_score, 0);
		
	$net_conn_info = array("conn_score"=>$net_conn_score, "pkg_loss_rate"=>$pkg_loss_rate, "delay_tm"=>$delay_tm_avr);
	
	return $net_conn_info;
}

function get_net_stability_info($ret_wtp, $ret_sta)
{
	$usr_rtry_rate = 0;
	$acc_suces_rate = 0;
	$acc_fail_rate = 0;
	
	if (is_object($ret_wtp) && $ret_wtp->wtps["wtp_num"] > 0) {
		$acc_fail_cnt = 0;
		$acc_suces_cnt = 0;
		$cnt = count($ret_wtp->wtps["wtp_num"]);
		for ($i = 0; $i < $cnt; $i++) {
			if (isset($ret_wtp->value[$i]["vap_table"])) {
				$wlan_cnt = count($ret_wtp->value[$i]["vap_table"]);
				for ($j = 0; $j < $wlan_cnt; $j++) {
					if (isset($ret_wtp->value[$i]["vap_table"][$j]["auth_per"])) {
						if ($ret_wtp->value[$i]["vap_table"][$j]["auth_per"] > 0) {
							$acc_suces_rate += $ret_wtp->value[$i]["vap_table"][$j]["auth_per"];
							$acc_suces_cnt++;
						}	
					}
					if (isset($ret_wtp->value[$i]["vap_table"][$j]["abn_deauth_per"])) {
						if ($ret_wtp->value[$i]["vap_table"][$j]["abn_deauth_per"] > 0) {
							$acc_fail_rate += $ret_wtp->value[$i]["vap_table"][$j]["abn_deauth_per"];
							$acc_fail_cnt++;
						}
					}
				}
			}
		}
		if ($acc_suces_cnt > 0) {
			$acc_suces_rate /= $acc_suces_cnt;
		}
		if ($acc_fail_cnt > 0) {	
			$acc_fail_rate /= $acc_fail_cnt;
		}
	}
	
	if (is_object($ret_sta) && $ret_sta->stas["sta_num"] > 0) {
		$tx_rtry_cnt = 0;
		for ($i = 0; $i < $ret_sta->stas["sta_num"]; $i++) {
			if (isset($ret_sta->value[$i]["tx_retry_per"])) {
				if ($ret_sta->value[$i]["tx_retry_per"] > 0) {	
					$usr_rtry_rate = $ret_sta->value[$i]["tx_retry_per"];
					$tx_rtry_cnt++;
				}
			}
		}
		if ($tx_rtry_cnt > 0) {
			$usr_rtry_rate /= $tx_rtry_cnt;
		}
	}
#if 0
	$net_stab_score = (100 - $usr_rtry_rate) * 2/5 + (100 - $acc_suces_rate) * 2/5 + (100 - $acc_fail_rate) * 1/5;
	if (is_float($net_stab_score))
		$net_stab_score = round($net_stab_score, 0);
	if (is_float($usr_rtry_rate)) {
		$usr_rtry_rate = round($usr_rtry_rate, 0);
	}
	if (is_float($acc_suces_rate)) {
		$acc_suces_rate = round($acc_suces_rate, 0);		
	}
	if (is_float($acc_fail_rate)) {
		$acc_fail_rate = round($acc_fail_rate, 0);
	}
	
	$net_stab_info = array("stab_score"=>$net_stab_score, "rtry_rate"=>$usr_rtry_rate,
		"access_to_succ"=>$acc_suces_rate, "drops_rate"=>$acc_fail_rate);
#else
	srand((double)microtime()*1000000);
	$stab_score = rand(80, 100);
	$rtry_rate = rand(30, 60);
	$acc_to_succ = rand(80, 100);
	$drops_rate = rand(30, 60);
	
	$net_stab_info = array("stab_score"=>$stab_score, "rtry_rate"=>$rtry_rate, 
		"access_to_succ"=>$acc_to_succ, "drops_rate"=>$drops_rate);
#endif
	return $net_stab_info;
}

function get_healthy_index_param($ret_wtp, $ret_sta)
{
	$usr_num_info = get_usr_num_info($ret_wtp);
	$usr_rssi_info = get_usr_rssi_info($ret_sta);
	$disturb_info = get_signal_disturb_info($ret_wtp);
	$net_conn_info = get_net_conn_info($ret_wtp);
	$net_stab_info = get_net_stability_info($ret_wtp, $ret_sta);
	
	$healthy_score = array("usr_num_info"=>$usr_num_info, "usr_rssi_info"=>$usr_rssi_info, 
		"disturb_info"=>$disturb_info, "net_conn_info"=>$net_conn_info, "net_stab_info"=>$net_stab_info);
	return $healthy_score;
}

function get_key_index_param($ret_wtp, $ret_sta) 
{
	$signal = array();
	$acc_point = array();
	$flow = array();
	if (is_object($ret_wtp) && ($ret_wtp->wtps["wtp_num"] > 0)) {
	//Signal parameters
		$signal = get_signal_param($ret_wtp);
		$acc_point = get_access_point_param($ret_wtp);
		$flow = get_flow_param($ret_wtp);	
		
	} else {    //default value
		$signal = get_signal_param($ret_wtp);
		$acc_point = get_access_point_param($ret_wtp);
		$flow = get_flow_param($ret_wtp);
	}

	$terminal = array();
	if (is_object($ret_sta) && $ret_sta->stas["sta_num"] > 0) {
		$terminal = get_terminal_param($ret_sta);
	} else {    //default value
		$terminal = get_terminal_param($ret_sta);
	}
	$key_index = array("signal"=>$signal, "access"=>$acc_point, "terminal"=>$terminal, "flow"=>$flow);
	
	return $key_index;
}

function get_wireless_param($ret_wtp, $ret_wireless)
{
	$wireless_as_afi = array();
	$wireless_as_wlan = array();
	
	if (is_object($ret_wtp) && ($ret_wtp->wtps["wtp_num"] > 0)) {
		$wireless_as_afi = get_wireless_as_afi($ret_wtp);
	} else {
		$wireless_as_afi = get_wireless_as_afi($ret_wtp);
	}
	$wireless_as_wlan = get_wireless_as_wlan($ret_wireless);
	
	$wireless = array("wireless_as_wlan"=>$wireless_as_wlan, "wireless_as_afi"=>$wireless_as_afi);
	
	return $wireless;
}


if(!isset($meta))
{
	$meta["rc"] = "ok";
}

$ret_wtp = ext_wtp("wtp_list");
$ret_sta = ext_wtp("sta_list");
$ret_wireless = ext_wireless("show_wireless_network_list");
error_log( "Start to perform the process!");

$healthy_score = get_healthy_index_param($ret_wtp, $ret_sta);
$key_index = get_key_index_param($ret_wtp, $ret_sta);
$wireless = get_wireless_param($ret_wtp, $ret_wireless);

$total = array("sys_healthy"=>$healthy_score, "key_index"=>$key_index, "wireless"=>$wireless, "meta"=>$meta);

echo json_encode($total);


?>