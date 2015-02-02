<?php 
	if(!isset($meta))
	{
		$meta["rc"] = "ok";
	}
	
	$country_list = array( 
		array("country_name"=>"China", "country_code"=>156), 
		array("country_name"=>"United States", "country_code"=>840),
		array("country_name"=>"Japan", "country_code"=>392),
		array("country_name"=>"France", "country_code"=>250),
		array("country_name"=>"Spain", "country_code"=>724),
	);
	
	$cur_country_code = 156;
	$ret = ext_wtp("get_country_code");
	if (isset($ret->value["CountryCode"])) {
		$cur_country_code = $ret->value["CountryCode"];
	}
	else {
		error_log("function: get_country_code: get country code error!");
	} 
	$result = array("cur_country_code"=>$cur_country_code, "country_list"=>$country_list, "meta"=>$meta);
	echo json_encode($result);
?>