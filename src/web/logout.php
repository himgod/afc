<?php 
	include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
	include("publichead.php");
//	global $sessionPath;
	ext_funcpublic("delete_garbage_session_file", session_id(), $sessionPath);
	session_destroy();
	$_SESSION=array();
	setcookie('username','',time()-1);
	//setcookie('language','',time()-1);
	//setcookie('maintab.pagesize','',time()-1);
	//setcookie('refreshtime','',time()-1);
	ReturnToLoginPage();
?>
