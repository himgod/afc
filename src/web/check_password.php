<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<head>
<?php
include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
include("init_lang.php");
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
require_once("publichead.php");
include("message.php"); 
$check = "";
if(isset($_SERVER['HTTP_REFERER']) && ('login.php' == retrieve($_SERVER['HTTP_REFERER'])))									/*previous link is login.php*/
{
	$user_name=$_REQUEST["user_name"];
	$user_pass=$_REQUEST["user_pass"];
	if(0 == strcmp($user_name,""))
	{	
		PhpShowAlert(search($lpublic, "user_name_not_empty"));
		ReturnToLoginPage();
	}
	elseif(0 == strcmp($user_pass,""))
	{
		PhpShowAlert(search($lpublic, "password_not_empty"));
		ReturnToLoginPage();
	}
	else
	{	
		$userip = $_SERVER['REMOTE_ADDR'];
		//$check = php_check_user_password($user_name, $user_pass);
		$check = false;
		$ret = ext_admin("check_user_passwd", $user_name, $user_pass, $userip);/*here check passwd and update login time and ip*/
		if(isset($ret->value["result"]))
		{
			$check = $ret->value["result"];
		}
		if(true == $check)
		{
			$_SESSION['username'] = $user_name;
			$_SESSION['language_select'] = $ret->value["language"];
			//setcookie('username', $user_name,time()+10*24*3600);  /* move to index.php page */
			//setcookie('language',$_SESSION['language_select'],time()+10*24*3600);
			
			$userid = $ret->value["_id"];
				
			//	$ret = ext_user("get_user_privilege", $user_name);
			
			//$ret = 0;
			//if("true" == php_get_user_info($user_name, "administrator"))
			//{
				//$ret = 1;
			//}
			$_SESSION['userprivilege'] = ($ret->value["administrator"] == 1)?1:0;
			$_SESSION['logintime'] = time();
			$_SESSION['lasttime'] = $_SESSION['logintime'];
			$_SESSION['userid'] = $userid;
			$_SESSION['loginip'] = $userip;
			$_SESSION['last_login_time'] = $ret->value['last_login_time'];
			$_SESSION['last_login_ip'] = $ret->value['last_login_ip'];
			/*to resolve the problem if refresh index.php after just login in */
			ForwardToPage("index.php");
		}
		else
		{
			PhpShowAlert(search($lpublic, "user_name_or_passwd_error"));
			ReturnToLoginPage();
		}
	}
}
else
{
	ReturnToLoginPage();
}
?>	
</head>
<body>
</body>
</html>
