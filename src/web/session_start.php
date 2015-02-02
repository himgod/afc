<?php
if(!isset($_SESSION))
{
	$sessPath = $_SERVER['DOCUMENT_ROOT']."/session";
	//if(!file_exists($sessPath))
	//{
	//	system("sudo mkdir -p ".$sessPath);
	//}
	//system(" sudo chown www-data:www-data ".$sessPath." 2>/dev/null; sudo chmod 775 ".$sessPath." 2>/dev/null");
	
	if(file_exists($sessPath) && is_dir($sessPath) && is_writable($sessPath))
	{
		session_save_path($sessPath);
	}
	session_start();
}
?>
