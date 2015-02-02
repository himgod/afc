<?php
//include("expired_check.php");
//include("api/encoding_json.php");

$ret = ext_sys_manage("load_wireless_config");

$data = array();
$meta = "";
$meta["rc"] = "ok";

$result = array("data"=>$data, "meta"=>$meta);

//echo json_encode($result);

?>

