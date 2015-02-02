<?php
include("expired_check.php");
$meta=array("rc"=>"ok");
$json = $_POST["json"];
$json=json_decode($json);

$name = $json->name;
$x_password = $json->x_password;
//$email_alert_enabled=$json->email_alert_enabled;
$email_alert_enabled = false;
$email = "";
$administrator = true;

$lang = $json->lang;
//$session_timeout = $json->session_timeout;
$data=array();
$ret = ext_admin("get_userinfo_by_name", $name);
if($ret->users["count"] > 0)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.UserNameExists";	
}
else
{
	$ret = ext_admin("add_new_user_to_admin_user", $name, $x_password, $administrator, $email_alert_enabled, $email, $lang);
	
	if(isset($ret->value) && isset($ret->value["result"]) && ($ret->value["result"] == 0) && ($ret->value["userid"] > 0))
	{
		$meta["rc"] = "ok";
		$_SESSION["username"] = $name;
		$_SESSION["loginip"] = $_SERVER["REMOTE_ADDR"];
	}
	else
	{
		$meta["rc"] = "error";
		$meta["msg"] = "api.err.OperationFailed";
		$meta["ret"] = (isset($ret->value)?$ret->value["result"]:$ret);
	}
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>
