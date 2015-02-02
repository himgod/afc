<?php 
	if(!isset($meta)) {
		$meta["rc"] = "ok"; 
	}
	$service_code = "";
	$ret = ext_operate("get_service_code");
	if (is_object($ret) && isset($ret->value["service_code"])) {
		$service_code = $ret->value["service_code"];
	}
	else {
		$service_code = "Sorry, not get the service code!";
	}
	$result = array("service_code"=>$service_code, "meta"=>$meta);
	
	echo json_encode($result);
?>