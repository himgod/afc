<?php
include("expired_check.php");
include("encoding_json.php");
//$json_str = '{ "data" : [ { "_id" : "5388665748f029f89fb8d52f" , "file_id" : "5388665748f029f89db8d52f" , "name" : "Sample2" , "order" : 0 , "selected" : false , "unit" : "m" , "upp" : 0.014906338472313001 , "url" : "/map/example2.jpg"}] , "meta" : { "rc" : "ok"}}';
$data = array();
$ret = ext_map("map_list");
if(is_object($ret) && isset($ret->maps) && ($ret->maps["map_num"] > 0))
{
	$data = $ret->value;
}
else
{
	//$meta["rc"] = "error";
	//$meta["msg"] = "api.err.NoAnyMapsFoundOnServer";
}
//echo $json_str;
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);
//error_log(json_encode($result));
?>
