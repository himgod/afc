<?php
include("expired_check.php");
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");

$json = isset($_REQUEST['json']) ? json_decode($_REQUEST['json']) : "";
$attrs = isset($json->attrs) ? $json->attrs : "";
$start = isset($json->start) ? $json->start : "";
$end = isset($json->end) ? $json->end : "";
$att_count = count($attrs);
$data=array();
$meta["rc"] = "ok";

for($i = 0, $time=$end; ($i <= 24 && $time >= $start); $i++, $time-=3600000)
{
	for($j = 0; $j < $att_count; $j++)
	{
		$data[$i][$attrs[$j]] = get_activities_by_time($attrs[$j], 
			(($time-3600000) > 0)?($time-3600000):0, $time);
	}
}

$result = array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>
