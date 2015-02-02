<?php
	include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
	require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
		
	$meta=array("rc"=>"ok");
	/*here load all system config */
	$idle_timeout = ext_sys_manage("get_idle_timeout_threshold"/*, $_SERVER["DOCUMENT_ROOT"]*/);
	if(0 > $idle_timeout)
	{
		$idle_timeout = 1235;
	}
	
	if(isset($_SERVER['HTTP_REFERER']) && ('wizard.php' == retrieve($_SERVER['HTTP_REFERER'])))
	{
		return;
	}
	
	$TimeNow = time();
	$LastTime = 0;
	if(isset($_SESSION['lasttime']))
	{
		$LastTime = $_SESSION['lasttime'];
	}
	//$meta["msg"] = $idle_timeout;
	if($TimeNow - $LastTime >= $idle_timeout)
	{
		$meta=array("rc"=>"error", "msg"=>"api.err.LoginRequired");
        	$result=array("data"=>array(), "meta"=>$meta);
        	echo json_encode($result);
        	exit;
	}
	else
	{
		$_SESSION['lasttime'] = time();
	}

?>
