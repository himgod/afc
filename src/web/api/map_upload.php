<?php
require_once($_SERVER["DOCUMENT_ROOT"]."/common.php");
$json = isset($_POST["json"]) ? json_decode($_POST["json"]) : "";
$meta = array();
$meta["rc"] = "ok";
$data = array();
$mappath = "/map/";
$mapname = (isset($json) && isset($json->name)) ? $json->name : "";
$postcmd = (isset($json) && isset($json->cmd)) ? $json->cmd:"add";

$admin="";
if(isset($_SESSION["username"]))
{
	$loginip = isset($_SESSION["loginip"]) ? $_SESSION["loginip"] : "";
	$admin = $_SESSION["username"].":".$loginip;
}
//error_log($postcmd." ".$mapname);
if($postcmd == "add")
{
	if ((($_FILES["file"]["type"] == "image/gif")
		|| ($_FILES["file"]["type"] == "image/jpeg")
		|| ($_FILES["file"]["type"] == "image/pjpeg")
		|| ($_FILES["file"]["type"] == "image/png")
		|| ($_FILES["file"]["type"] == "image/x-png")))
	{
		if($_FILES["file"]["error"] > 0)
		{
			$meta["rc"] = "error";
			$meta["msg"] = "api.err.MapFileUploadFailed";
		}
		else if($_FILES["file"]["size"] > 1024*1024)
		{
			$meta["rc"] = "error";
			$meta["msg"] = "api.err.MapFileTooLarge";
		}
		else
		{
			$mapname = (isset($_POST["map-name"])) ? $_POST["map-name"] : "";
			$mapname = iconv("gb2312", "utf-8", $mapname);/*from web page gb2312 to xml file utf-8 */
		  	$subfix = getsubfix($_FILES["file"]["name"]);
		  	$newfile = $mappath.$mapname;//.($subfix == "" ? "":".".$subfix);
		  	$nameadd = "";
		  	while(file_exists($newfile.$nameadd))
		  	{
				//$meta["rc"] = "error";
				//$meta["msg"] = "api.err.MapFileExists";
				$nameadd = ($nameadd||0)+1;
		  	}
		  	//else
		  	{
		  		$url = $newfile.$nameadd.($subfix == "" ? "":".".$subfix);
		  		$newfilename = $_SERVER["DOCUMENT_ROOT"].$url;
			    $cmd = "sudo cp ".$_FILES["file"]["tmp_name"]." ".$newfilename."\n";
			    $cmd .= "sudo chown www-data:www-data ".$newfilename."\n";
			    $ret = system($cmd);
			    $initscaleunit = (isset($_POST["initscaleunit"])) ? $_POST["initscaleunit"] : "m";
			    $initscalemeter = (isset($_POST["initscalemeter"])) ? $_POST["initscalemeter"] : "1";
			    $initscalewidth = (isset($_POST["initscalewidth"])) ? $_POST["initscalewidth"] : "38";/* one centimeter is 38 px */
			    if("" != $ret && 0 != $ret)
			    {	    	
					$meta["rc"] = "error";
					$meta["msg"] = "api.err.MapFileRestoreFailed";
			  
			    }
			    else
			    {
					$ret = ext_map("add_modify_map", "", $mapname, $url, $initscaleunit,
						$initscalemeter, $initscalewidth, 0/* false means selected when add */, $admin);
					if(is_object($ret) && isset($ret->maps) && ($ret->maps["map_num"] > 0))
					{
						$data = $ret->value;
					}
					else if(-2 == $ret)	
					{						
						$meta["rc"] = "error";
						$meta["msg"] = "api.err.TooManyMapsUploaded";
						header("location:/map_upload_iframe.php?err_msg=".$meta["msg"]);
						return;
					}
					header("location:/map_upload_iframe.php?reload=true");
					return;
			    }
			}
		}
	}
	else
	{
		$meta["rc"] = "error";
		$meta["msg"] = "api.err.InvalidMapFileUpload";
	}
	header("location:/map_upload_iframe.php?err_msg=".$meta["msg"]);
	return;
}
else if($postcmd == "rename")
{
  	//$subfix = (isset($json) && isset($json->subfix)) ? $json->subfix:"";
	$oldname = (isset($json) && isset($json->oldname)) ? $json->oldname:"";
	
	if($mapname != "" && $oldname != "")
	{
		//$oldfile = $mappath.$oldname.($subfix == "" ? "":".".$subfix);
	  	//$newfile = $mappath.$mapname.($subfix == "" ? "":".".$subfix);
	  	//if(file_exists($newfile))
	  	//{
		//	$meta["rc"] = "error";
		//	$meta["msg"] = "api.err.MapFileExists";
	  	//}
	  	//else
	  	{
		    //if("" != $ret && 0 != $ret)
		    //{	    	
			//  $meta["rc"] = "error";
			//  $meta["msg"] = "api.err.MapFileRenameFailed";
		    //}
		    //else
		    {
				$ret = ext_map("rename_map", $oldname, $mapname, $admin);
				if(-2 == $ret)
				{
					$meta["rc"] = "error";
			  		$meta["msg"] = "api.err.MapNewnameExists";
				}
				else if(-1 == $ret)
				{
					$meta["rc"] = "error";
			  		$meta["msg"] = "api.err.MapFileRenameFailed";
				}
				$ret = ext_map("map_list");
				if(is_object($ret) && isset($ret->maps) && ($ret->maps["map_num"] > 0))
				{
					$data = $ret->value;
				}
		    }
		}
	}
}
else if($postcmd == "delete")
{
	if($mapname != "")
	{
		$ret = ext_map("remove_map", $mapname, $admin);
		$ret = ext_map("map_list");
		if(is_object($ret) && isset($ret->maps) && ($ret->maps["map_num"] > 0))
		{
			$data = $ret->value;
		}
	}
}
else if($postcmd == "set_selected")
{
	if($mapname != "")
	{
		$ret = ext_map("set_selected_map", $mapname, $admin);
	}
}
else if($postcmd == "set_scale")
{
	if($mapname != "")
	{
		$initscalemeter = isset($json->initscalemeter) ? $json->initscalemeter:"";
		$initscalewidth = isset($json->initscalewidth) ? $json->initscalewidth:"";
		$initscaleunit = isset($json->initscaleunit) ? $json->initscaleunit:"";
		$ret = ext_map("set_map_scale", $mapname, $initscaleunit, $initscalemeter, $initscalewidth, $admin);
	}
}
$result = array("data"=>$data, "meta"=>$meta);
echo json_encode($result);
?>
