<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
	include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
	require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
	include("init_lang.php");
	include("publichead.php");
	include("message.php"); 
	if((isset($_SERVER['HTTP_REFERER']) && 'index.php' != retrieve($_SERVER['HTTP_REFERER'])) && 
		!isset($_SESSION["upgrade_status"]))
	{
		ForwardToPage("index.php");
	}
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' dir="ltr">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<head>
<?php
importJsFor("waiting");
$waiting_cmd = $_REQUEST['waiting_cmd'];
$progress = 0;
$percent = "";
if($waiting_cmd == "upgrade" && isset($_SESSION["upgrade_status"]) && "" != $_SESSION["upgrade_status"])
{
	if(isset($_SESSION["totalsecond"]) && isset($_SESSION["starttime"]) &&
		0 != $_SESSION["totalsecond"] && 0 != $_SESSION["starttime"])
	{
		if(time()-$_SESSION["starttime"] > $_SESSION["totalsecond"]+3)
		{
			$_SESSION["upgrade_status"] = "";
			$_SESSION["starttime"] = 0;
			$_SESSION["totalsecond"] = 0;
		}
		else
		{
			$progress = round(((time()-$_SESSION["starttime"])/$_SESSION["totalsecond"])*300);
			$percent = (((time()-$_SESSION["starttime"])/$_SESSION["totalsecond"])*100);
		}
	}
}
?>
	<title><?php echo search($lpublic, "server_busy")." - ".getSoftwareName();?></title>
</head>
<body class="login-page waiting-page">

<div id="cfinstall"></div>

<div class="page-content">
	<form id="LoginForm" class="configuration-form" method="post" action="/login">
		<h1 class="autefi-logo"><?php echo getSoftwareName();?></h1>
		<div class="login-content content-box">
			<h2 class="header"></h2>
			<div class="progress"></div>
			<div style="padding-left: 30px;">
		    <div style="padding: 0; background-color: grey; border: 1px solid navy; width: 300px"> 
		    <div id="progress" style="padding: 0; background-color: #009900; border: 0; width: <?php echo $progress;?>px; text-align: center;   height: 16px"></div>             
		    </div> 
		    </div>
		    <div id="status">&nbsp;</div>
		</div>
	</form>
</div>
<input type='hidden' name='waiting_cmd' id='waiting_cmd' value='<?php echo $waiting_cmd;?>'>
<input type='hidden' name='percent' id='percent' value='<?php echo $percent;?>' >

</body>
</html>
