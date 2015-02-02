<?php

function retrieve($url)
{ /* get the filename of the url */
        preg_match('/\/([^\/]+\.[a-zA-Z]+)[^\/]*$/',$url,$match);
        return isset($match[1]) ? $match[1] : "";
}
function getsubfix($filename)
{ /* get the filename of the url */
        preg_match('/.*\.([a-zA-Z\-]+)\s*$/',$filename,$match);
        return isset($match[1]) ? $match[1] : "";
}
function get_ap_version_info()
{
	$filepath = "/opt/img/upgrade.info";
	$myfile = fopen($filepath, "r") or die("Unable to open file!");
	$version = "";
	$imgFile = "";
	$miniImgFile = "";
	
	if($myfile && !feof($myfile)) {
		$version = fgets($myfile);
		$version = trim($version);
	}
	if($myfile && !feof($myfile)) {
		$imgFile = fgets($myfile);
		$imgFile = trim($imgFile);
	}
	if($myfile && !feof($myfile)) {
		$miniImgFile = fgets($myfile);
		$miniImgFile = trim($miniImgFile);
	}
	if($myfile)
	{
		fclose($myfile);
	}
	return array("version"=>$version, "imgFile"=>$imgFile, "miniImgFile"=>$miniImgFile);
}

function get_ac_version_info()
{
	$filepath = "/etc/version/";
	$versionfile = $filepath."version";
	$buildnofile = $filepath."buildno";
	$myfile = fopen($versionfile, "r");// or die("Unable to open file!");
	$version = "";
	
	if($myfile && !feof($myfile)) {
		$version = fgets($myfile);
		$version = trim($version);
	}
	if($myfile)
	{
		fclose($myfile);
	}
	
	$myfile = fopen($buildnofile, "r");// or die("Unable to open file!");
	$buildno = "";
	if($myfile && !feof($myfile)) {
		$buildno = fgets($myfile);
		$buildno = trim($buildno);
	}
	if($myfile)
	{
		fclose($myfile);
	}

	return array("version"=>$version, "buildno"=>$buildno);
}

function get_ac_new_version_info($infofile)
{
	$myfile = fopen($infofile, "r");// or return array("version"=>"", "url"=>"");//die("Unable to open file!");
	$version = "";
	$url = "";
	
	if($myfile && !feof($myfile)) {
		$version = fgets($myfile);
		$version = trim($version);
	}
	
	if($myfile && !feof($myfile)) {
		$url = fgets($myfile);
		$url = trim($url);
	}
	if($myfile)
	{
		fclose($myfile);
	}

	return array("version"=>$version, "url"=>$url);
}

function get_activities_by_time($attrname, $starttime, $endtime)
{
	if($attrname == "bytes")
	{
		return 0;//rand(0, 1000000000);
	}
	else if($attrname == "num_sta")
	{
		return 0;//rand(0, 50);
	}
	else if($attrname == "time")
	{
		return $endtime;
	}
	else
	{
		return "";
	}
}
function version_is_newer($new_version, $old_version)
{
	$new_ver_arr = split("\.", $new_version);
	$old_ver_arr = split("\.", $old_version);
	$i = 0;
	$new_len = count($new_ver_arr);
	$old_len = count($old_ver_arr);
	$len = min($new_len, $old_len);
	for($i = 0; $i < $len; $i++)
	{
		if($new_ver_arr[$i] > $old_ver_arr[$i])
		{
			return true;
		}
		else if($new_ver_arr[$i] < $old_ver_arr[$i])
		{
			return false;
		}
	}
	if($new_len > $old_len)
	{
		return true;
	}
	return false;
}
function get_afc_version_server_addr()
{
	return "192.168.1.242";
}
function get_ac_upgrade_over()
{
	$filepath = "/opt/run/upgraded";
	$myfile = fopen($filepath, "r");// or return false;//die("Unable to open file!");	
	$over = 0;
	if($myfile && !feof($myfile)) {
		$over = fgets($myfile);
		$over = trim($over);
	}
	if($myfile)
	{
		fclose($myfile);
	}

	return ($over == "1" || $over == 1);
}
?>
