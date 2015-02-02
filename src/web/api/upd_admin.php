<?php
include("expired_check.php");

$json = $_POST["json"];
//$meta["msg"]=$json;
$json=json_decode($json);
$userid = $json->_id;
$oldname=$json->oldname;
$oldpasswd = $json->oldpasswd;
$name = $json->name;
$x_password = $json->x_password;
$email_alert_enabled=isset($json->email_alert_enabled)?$json->email_alert_enabled:false;
$email = "";
$administrator = $_SESSION['userprivilege'];/*not change */
$email = isset($json->email) ? $json->email:"";
$lang = isset($json->lang) ? $json->lang : "";
if($userid == "" && isset($_SESSION["userid"]))
{
	$userid = $_SESSION["userid"];
}

//$session_timeout = $json->session_timeout;
$data=array();
$ret = ext_admin("update_userinfo_by_id_or_passwd", $userid, $oldname, $name, 
	$oldpasswd, $x_password, $administrator, $email_alert_enabled,$email,$lang);
if(!isset($ret->value) || !isset($ret->value["result"]))
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.OperationFailed";
	$meta["ret"] = "ret ".json_encode($ret);
}
else if($ret->value["result"] == -2)
{
	$meta["rc"] = "ok";
	$meta["msg"] = "api.err.OldPasswordError";
}
else if($ret->value["result"] == 2)
{/*modify other same name user*/
	$meta["rc"] = "ok";
}
else if($ret->value["result"] == 3)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.PermissionDenied";
}
else if($ret->value["result"] != 0)
{
	$meta["rc"] = "error";
	$meta["msg"] = "api.err.OperationFailed";
	$meta["ret"] = "ret->value['result']".$ret->value["result"];
}
else
{
	$meta["rc"] = "ok";
	//$meta["msg"] = "api.err.OperationSuccess";
	if($oldname != $name)
	{
		$_SESSION["username"] = $name;
		//if(isset($_SESSION['logintime']) && $_SESSION['logintime'] != "")
		//{
			//setcookie('username', $name,$_SESSION["logintime"]+10*24*3600);
		//}
	}
	if(!isset($_SESSION['language_select']) || $lang != $_SESSION['language_select'])
	{
		$_SESSION['language_select'] = $lang;
		//setcookie('language',$_SESSION['language_select'],$_SESSION["logintime"]+10*24*3600);
	}
	if(isset($_SESSION["get_userinfo_ret"]))
	{
		$ret = $_SESSION["get_userinfo_ret"];
		if(is_object($ret) 
			&& isset($ret->result)
			&& isset($ret->result["ret"]) 
			&& (0 == $ret->result["ret"]))
		{
			$ret->value["name"] = $name;
			$ret->value["lang"] = $lang;
			$ret->value["email_alert_enabled"] = $email_alert_enabled;
			$ret->value["email"] = $email;
		}		
	}
}

$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);

?>
