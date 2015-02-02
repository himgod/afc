<?php
include("expired_check.php");

//include("settings_public.php");
$data=array();
$ret = -1;
if(isset($_SESSION["userid"]) && $_SESSION["userid"] != "")
{
	if(!isset($_SESSION["get_userinfo_ret"]) 
		|| !isset($_SESSION["get_userinfo_time"]) 
		|| ((time()- $_SESSION["get_userinfo_time"]) > 10*60))
	{
		$ret = ext_admin("get_userinfo_by_id", $_SESSION["userid"]);
		$_SESSION["get_userinfo_ret"] = $ret;
		$_SESSION["get_userinfo_time"] = time();
	}
	else
	{
		$ret = $_SESSION["get_userinfo_ret"];
	}
}
if(is_object($ret) 
	&& isset($ret->result)
	&& isset($ret->result["ret"]) 
	&& (0 == $ret->result["ret"]))
{
	$user = $ret->value;

	$user["x_password"] = md5($user["name"]);/* hidden thd password */
	$data=array($user);
}
else
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.LoginRequired";//session timeout or can't find any userinfo by current userid 
	//$meta["msg"] = "api.err.ListAdminFailed";/*unexpected error , no error msg for it*/
	//$meta["ret"] = isset($ret->result["ret"]) ? ("ret->result['ret'] ".$ret->result["ret"]):("ret ".$ret);
}
//$meta["msg"] = json_encode($user);
$result=array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>
