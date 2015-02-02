<?php
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");

//$trbackgroud = " onmouseover='this.style.background=\"#E0ECFF\";' onmouseout='this.style.background=\"#FFFFFF\";'";
function getServerName()
{
		return $_SERVER['SERVER_NAME'];
}
function getSoftwareName()
{
	  return "AFi-Controller";
}
function getSoftwareShortName()
{
	  return "AFC";
}
function getSoftwareVersion()
{
	$version = get_ac_version_info();
	$full_version = $version["version"].".".$version["buildno"];
	return $full_version;
}
function getJsVersion()
{
	return "1.0.0";
}
function getCssVersion()
{
	return "1.0.0";
}
function PhpShowAlert($message)
{
	echo "<script type='text/javascript'>alert('{$message}');</script>";
}
function ForwardToPage($page)
{
	echo "<script type='text/javascript'>top.location.href='".$page."';</script>";
}
function ReturnToLoginPage()
{
	echo "<script type='text/javascript'>top.location.href='login.php';</script>";
}

function ToWizardIfAdminNotExists()
{
	if(!isset($_SESSION["admin_count"]) || "" == $_SESSION["admin_count"])
	{
		$ret = ext_admin("get_admin_userinfo_count");	
		$_SESSION["admin_count"] = $ret;
	}
	else
	{
		$ret = $_SESSION["admin_count"];
	}
	if(!isset($ret->value["result"]) || ($ret->value["result"] <= 0))
	{
	//PhpShowAlert($ret->value["result"]."to wizard");
		ForwardToPage("wizard.php");
		return true;
	}
	return false;
}

function ToIndexIfAdminExists()
{
	$ret = ext_admin("get_admin_userinfo_count");
	if(isset($ret->value["result"]) && ($ret->value["result"] > 0))
	{
	//PhpShowAlert($ret->value["result"]."to index");
		ForwardToPage("index.php");
		return true;
	}
	return false;
}
//---------------------------------------------------------
// following is related to SESSION 
include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
$sessionPath = session_save_path();//"/var/lib/php5/";
//include("init_lang.php");
function languageIsChinese()
{
	return ((isset($_SESSION['language_select']) && $_SESSION['language_select']=='zh_CN')||
			(((!isset($_SESSION['language_select'])) || ($_SESSION['language_select']=='auto'))
			&& isset($_SERVER["HTTP_ACCEPT_LANGUAGE"]) && (0 == strncmp($_SERVER['HTTP_ACCEPT_LANGUAGE'],"zh",2))));
}

//include("message.php"); 

function importJsFor($page)
{
	$version = getJsVersion();//get version heres
	$cssversion = getCssVersion();
	$soft = getSoftwareShortName();
	$language = (isset($_SESSION) && isset($_SESSION['language_select']))?$_SESSION['language_select']:"auto";
	
 // echo '<link type="text/css" rel="stylesheet" media="all" href="/library/css/'.$soft.'.'.$cssversion.'.css" />';
//echo '<link rel="stylesheet" href="/lib/css/map_style.css">';
echo "<script type='text/javascript'>
var currentLocale = '".$language."';
</script>
";
echo '<script type="text/javascript" src="/library/js/md5.js"></script>
';
//	echo '<script type="text/javascript" src="/library/js/'.$soft.'-lib.'.$version.'.js"></script>
//';
	echo '<script type="text/javascript" src="/lib/js/jquery-1.11.1.min.js"></script>
';
	echo '<script type="text/javascript" src="/lib/js/jquery-ui.min.js"></script>
';
	echo '<script type="text/javascript" src="/library/js/countries.js"></script>
';
echo '<script type="text/javascript" src="/library/js/jquery.cluetip.js"></script>
';

echo '<script type="text/javascript" src="/library/js/jquery.qrcode.min.js"></script>';

	if(languageIsChinese())
	{
		echo '<script type="text/javascript" src="/library/js/'.$soft.'-msg_zh_CN.'.$version.'.js"></script>
';
	}
	else
	{
		echo '<script type="text/javascript" src="/library/js/'.$soft.'-msg_en_US.'.$version.'.js"></script>
';
echo '<link type="text/css" href="/library/css/en-index.css"  rel="stylesheet"></link>
';

	}
	echo '<script type="text/javascript" src="/library/js/'.$soft.'.'.$version.'.js"></script>
';
echo '<script type="text/javascript" src="/library/js/map_action.js"></script>';
	echo '<script type="text/javascript" src="/library/js/'.$soft.'-'.$page.'.'.$version.'.js"></script>
';
echo '<script type="text/javascript" src="/library/js/authpuppyProxy.js"></script>';
}
function CheckWhetherUserLIlegal()
{
	if(!isset($_SESSION['username']))
	{
	/*
		if(languageIsChinese())
		{
			PhpShowAlert("非法用户，请重新登录！");
		}
		else
		{
			PhpShowAlert("Illegal users, please login again!");
		}
	*/
		ReturnToLoginPage();
	}
}

function CheckWhetherTheLoginTimeout()
{
	$idle_timeout = ext_sys_manage("get_idle_timeout_threshold"/*, $_SERVER["DOCUMENT_ROOT"]*/);
	global $sessionPath;
	if(0 > $idle_timeout)
	{
		$idle_timeout = 1236;
	}
	//PhpShowAlert($idle_timeout);	
	$TimeNow = time();
	$LastTime = isset($_SESSION['lasttime'])?$_SESSION['lasttime']:0;
	if($TimeNow - $LastTime >= $idle_timeout)
	{
		
		if(languageIsChinese())
		{
			PhpShowAlert("登录超时，请重新登录！");
		}
		else
		{
			PhpShowAlert("login timeout, please login again!");
		}
		ext_funcpublic("delete_garbage_session_file", session_id(), $sessionPath);
		session_destroy();
		$_SESSION = array();
		ReturnToLoginPage();		
	}
	else
	{
		$_SESSION['lasttime'] = $TimeNow;
	}
}

function CheckLegality()
{
	CheckWhetherUserLIlegal();
	CheckWhetherTheLoginTimeout();
}

?>

