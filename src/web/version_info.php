<?php

$cmd = isset($_REQUEST["cmd"]) ? $_REQUEST["cmd"]:"";
$modify = isset($_REQUEST["modify"]) ? $_REQUEST["modify"]:0;

$host = "localhost";
$user = "root";
$pass = "autelan";
$database = "afc_release_notes_DB";
$table = "version_info";

$con = mysql_connect($host, $user, $pass);
if (!$con)
{
  die('Could not connect: ' . mysql_error());
}

mysql_select_db($database, $con);
if($cmd == "add")
{
	$version = isset($_REQUEST["version"]) ? $_REQUEST["version"]:"";
	$type = isset($_REQUEST["type"]) ? $_REQUEST["type"]:"";
	if($type == "AFi")
	{
		$buildno = '-1';
	}
	else
	{
		$buildno = isset($_REQUEST["buildno"]) ? $_REQUEST["buildno"]:"";
	}
	$release_time = (isset($_REQUEST["release_time"]) && "" != $_REQUEST["release_time"])? 
					"'".$_REQUEST["release_time"]."'":"NULL";
	$comment = isset($_REQUEST["comment"]) ? $_REQUEST["comment"]:"";
	$comment_cn = isset($_REQUEST["comment_cn"]) ? $_REQUEST["comment_cn"]:"";
	//$comment = urlencode(iconv('gb2312', 'utf-8', $comment));
	//$comment_cn = urlencode(iconv('gb2312', 'utf-8', $comment_cn));
	$comment = str_replace("\\", "\\\\", $comment);
	$comment = str_replace("'", "\'", $comment);
	$comment_cn = str_replace("\\", "\\\\", $comment_cn);
	$comment_cn = str_replace("'", "\'", $comment_cn);

	if($version != "")
	{
		mysql_query("insert into ".$table." values(NULL, '".$version."', '".$buildno."', ".$release_time.", '".$comment."', '".$comment_cn."')");
	}	
}
else if($cmd == "delete")
{
	$id = isset($_REQUEST["id"]) ? $_REQUEST["id"]:0;
	if($id > 0)
	{
		mysql_query("delete from ".$table." where id = ".$id);
	}
}

?>
<html>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<head>
<title>
AFC-AFi Release Notes
</title>
<script type="text/javascript">
function add_new_version(type)
{
	if(type == "afi")
	{
		document.getElementById("afi_version_add").style.display = "block";
		if(document.getElementById("afi_r_now").checked)
		{
			document.getElementById("afirelease_time").disabled = true;
			document.getElementById("afirelease_time").value = get_current_time();
		}
	}
	else
	{
		document.getElementById("afc_version_add").style.display = "block";
		if(document.getElementById("afc_r_now").checked)
		{
			document.getElementById("afcrelease_time").disabled = true;
			document.getElementById("afcrelease_time").value = get_current_time();
		}
	}	
}
function add_version_cancel(type)
{
	if(type == "afi")
	{
		document.getElementById("afi_version_add").style.display = "none";
	}
	else
	{
		document.getElementById("afc_version_add").style.display = "none";
	}	
}

function get_current_time()
{
	var now = new Date();
	var timestr = ""+now.getFullYear()+"-"+(1+now.getMonth())+"-"+now.getDate()
					+" "+now.getHours()+":"+now.getMinutes()+":"+now.getSeconds();
	return timestr;
}

function add_form_reset(type)
{
	if(type == "afi")
	{
		document.getElementById("afiversion").value = "";	
		document.getElementById("afi_r_now").checked = true;
		document.getElementById("afirelease_time").disabled = true;
		document.getElementById("afirelease_time").value = get_current_time();
		document.getElementById("aficomment").value = "";
		document.getElementById("aficomment_cn").value = "";	
	}
	else
	{
		document.getElementById("afcversion").value = "";
		document.getElementById("afcbuildno").value = "";
		document.getElementById("afc_r_now").checked = true;
		document.getElementById("afcrelease_time").disabled = true;
		document.getElementById("afcrelease_time").value = get_current_time();
		document.getElementById("afccomment").value = "";
		document.getElementById("afccomment_cn").value = "";	
	}	
}

function add_form_submit(type)
{
	if(type == "afi")
	{
		document.getElementById("version").value = document.getElementById("afiversion").value;
		document.getElementById("buildno").value = "-1";
		//if(document.getElementById("afi_r_input").checked)
		{
			document.getElementById("release_time").value = document.getElementById("afirelease_time").value;
		}
		//else
		{
			//document.getElementById("release_time").value = get_current_time();
		}
		document.getElementById("comment").value = document.getElementById("aficomment").value;
		document.getElementById("comment_cn").value = document.getElementById("aficomment_cn").value;
		document.getElementById("type").value = "AFi";
		document.getElementById("cmd").value = "add";
		document.getElementById("form1").submit();
	}
	else
	{
		document.getElementById("version").value = document.getElementById("afcversion").value;
		document.getElementById("buildno").value = document.getElementById("afcbuildno").value;
		//if(document.getElementById("afc_r_input").checked)
		{
			document.getElementById("release_time").value = document.getElementById("afcrelease_time").value;
		}
		//else
		{
			//document.getElementById("release_time").value = get_current_time();
		}
		document.getElementById("comment").value = document.getElementById("afccomment").value;
		document.getElementById("comment_cn").value = document.getElementById("afccomment_cn").value;
		document.getElementById("type").value = "AFC";
		document.getElementById("cmd").value = "add";
		document.getElementById("form1").submit();
	}	
}

function delete_version(id)
{
	if(!confirm("Are you sure to delete this item?"))
	{
		return false;
	}
	document.getElementById("id").value = id;
	document.getElementById("cmd").value = "delete";
	document.getElementById("form1").submit();
}

function release_radio_check(type)
{
	if(type == "afi")
	{
		if(document.getElementById("afi_r_now").checked)
		{
			document.getElementById("afirelease_time").disabled = true;
			document.getElementById("afirelease_time").value = get_current_time();
		}
		else if(document.getElementById("afi_r_input").checked)
		{
			document.getElementById("afirelease_time").disabled = false;
			document.getElementById("afirelease_time").select();
		}
	}
	else
	{
		if(document.getElementById("afc_r_now").checked)
		{
			document.getElementById("afcrelease_time").disabled = true;
			document.getElementById("afcrelease_time").value = get_current_time();
		}
		else if(document.getElementById("afc_r_input").checked)
		{
			document.getElementById("afcrelease_time").disabled = false;
			document.getElementById("afcrelease_time").select();
		}
	}
}
</script>
<style type="text/css"> 
table .datatable{ border-collapse:collapse;font-size:12px;font-family:Helvetica, Arial, sans-serif;}
table .datatable td{border:1px solid #3F3F3F;}
table .datatable th{background:#999;height:60px;border:1px solid #3F3F3F;}
.thh{padding:30px 0px 0px 20px;border:none;/*background:#CCDBEC;*/font-size:15px;font-weight:bold;}
</style>
</head>
<body style="background:#CCB9DC;">
<?php
if($modify)
{
   echo "<div width='100%' style='text-align:right;padding-right:20px;'><a href='".$_SERVER["SCRIPT_NAME"]."'>Finish</a></div>";
}
else
{
   echo "<div width='100%' style='text-align:right;padding-right:20px'><a href='".$_SERVER["SCRIPT_NAME"]."?modify=1'>Modify</a></div>";
}
if($cmd != "")
{
?>
<script type="text/javascript">
	top.location.href = top.location.href;
</script>
<?php
}
?>
<table width='1200'>
<tr valign='top'>
<td class='thh'>
<div align="center" style="background:#CCDBEC;border:1px solid #3F3F3F;">
AFC release notes
<table width='600' class="datatable" style='margin:20px 20px 0px 10px;padding-left:10px;border:1px solid #3F3F3F;'>
<tr>
<th>Num</th>
<th>Version</th>
<th>Buildno</th>
<th>Release Time</th>
<th>Comment(En)</th>
<th>Comment(Ch)</th>
<?php
if($modify)
{
?>
  <th>Operator</th>
<?php
}
?>
</tr>
<?php

$result = mysql_query("SELECT * FROM ".$table." where buildno != '-1'");

$i = 1;
while($row = mysql_fetch_array($result))
{
  echo "<tr>";
  echo "<td>" . $i++ . "</td>";
  echo "<td>" . $row['version'] . "</td>";
  echo "<td>" . (($row['buildno'] == '' || $row['buildno'] == '0') ? "-":$row['buildno']) . "</td>";
  echo "<td>" . $row['releasetime'] . "</td>";
  echo "<td>" . str_replace("\n", "<br/>", $row['comment']) . "</td>";
  echo "<td>" . str_replace("\n", "<br/>", $row['comment_cn']) . "</td>";
  //echo "<td>" . iconv('utf-8', 'gb2312', urldecode($row['comment'])) . "</td>";
  //echo "<td>" . iconv('utf-8', 'gb2312', urldecode($row['comment_cn'])) . "</td>";
  if($modify)
  {
  	echo "<td><input type='button' onclick='delete_version(\"" . $row['id'] . "\")' value='Delete' /></td>";
  }
  echo "</tr>
  ";
}
?>
</table><br/>
<div style="text-align:left;padding-left:10px;">
<?php
if($modify)
{
?>
<input type='button' onclick='add_new_version("afc");' value='Add...' /><br/><br/>
<?php
}
?>
<div id="afc_version_add" style="display:none">
<table width="600" style="background:#CCDBEC;">
<tr>
<td>
Version: </td><td colspan=2><input type="text" id="afcversion" />
</td>
</tr>
<tr>
<td>
Buildno: </td><td colspan=2><input type="text" id="afcbuildno" />
</td>
</tr>
<tr><td>
Release Time: </td><td colspan=2>
<input type="radio" id="afc_r_now" name="afc_r_time" value="now" checked=true onclick="release_radio_check('afc');"/>Current Time
<input type="radio" id="afc_r_input" name="afc_r_time" value="input"  onclick="release_radio_check('afc');"/>Input Time: <input type="text" id="afcrelease_time" />
</td>

</tr>
<tr>
<td>
Comment(En): </td><td colspan=2><textarea cols="40" rows="5" id="afccomment" ></textarea>
</td>
</tr>
<tr>
<td>
Comment(Ch): </td><td colspan=2><textarea cols="40" rows="5" id="afccomment_cn" ></textarea>
</td>
</tr>
</table>
<table width="600" align="center" style="border:none;">
<tr>
<td><input type="button" id="afcsubmit" value="Submit" onclick="add_form_submit('afc');" /></td>
<td><input type="reset" id="afcreset" value="Reset" onclick="add_form_reset('afc');" /></td>
<td><input type="button" id="afccancel" value="Cancel" onclick="add_version_cancel('afc');"/></td>
</tr>
</table>
<br/>
</div>
</div>
</div>
</td>
<td colspan='10'>
</td>
<td class='thh'>
<div align="center" style="background:#CCDBEC;border:1px solid #3F3F3F;">
AFi release notes
<table width='600' class="datatable" style='margin:20px 20px 0px 10px;border:1px solid #3F3F3F;'>
<tr>
<th>Num</th>
<th>Version</th>
<th>Release Time</th>
<th>Comment(En)</th>
<th>Comment(Ch)</th>
<?php
if($modify)
{
?>
<th>Operator</th>
<?php
}
?>
</tr>
<?php

$result = mysql_query("SELECT * FROM ".$table." where buildno = '-1'");

$i = 1;
while($row = mysql_fetch_array($result))
{
  echo "<tr>";
  echo "<td>" . $i++ . "</td>";
  echo "<td>" . $row['version'] . "</td>";
  echo "<td>" . $row['releasetime'] . "</td>";
  echo "<td>" . str_replace("\n", "<br/>", $row['comment']) . "</td>";
  echo "<td>" . str_replace("\n", "<br/>", $row['comment_cn']) . "</td>";
  //echo "<td>" . iconv('utf-8', 'gb2312', urldecode($row['comment'])) . "</td>";
  //echo "<td>" . iconv('utf-8', 'gb2312', urldecode($row['comment_cn'])) . "</td>";
  if($modify)
  {
  	echo "<td><input type='button' onclick='delete_version(\"" . $row['id'] . "\")' value='Delete' /></td>";
  }
  echo "</tr>
  ";
}
mysql_close($con);
?>
</table><br/>
<div style="text-align:left;padding-left:10px;">
<?php
if($modify)
{
?>
<input type='button' onclick='add_new_version("afi");' value='Add...' /><br/><br/>
<?php
}
?>
<div id="afi_version_add" style="display:none">
<table width="600" style="background:#CCDBEC;">
<tr>
<td>
Version: </td><td><input type="text" id="afiversion" />
</td>
</tr>
<tr>
<td>
Release Time: </td><td colspan=2>
<input type="radio" id="afi_r_now" name="afi_r_time" value="now" checked=true onclick="release_radio_check('afi');"/>Current Time
<input type="radio" id="afi_r_input" name="afi_r_time" value="input" onclick="release_radio_check('afi');"/>Input Time: <input type="text" id="afirelease_time" />
</td>
</tr>
<tr>
<td>
Comment(En): </td><td><textarea cols="40" rows="5" id="aficomment" ></textarea>
</td>
</tr>
<tr>
<td>
Comment(Ch): </td><td><textarea cols="40" rows="5" id="aficomment_cn" ></textarea>
</td>
</tr>
</table>
<table width="600" align="center" style="border:none;">
<tr>
<td><input type="button" id="afisubmit" value="Submit" onclick="add_form_submit('afi');" /></td>
<td><input type="reset" id="afireset" value="Reset" onclick="add_form_reset('afi');" /></td>
<td><input type="button" id="aficancel" value="Cancel" onclick="add_version_cancel('afi');"/></td>
</tr>
</table>
<br/>
</div>
</div>
</div>
</td>
</tr>
</table>
<form id="form1" method="post" action="version_info.php?modify=1">
<input type="hidden" id="id" name="id" />
<input type="hidden" id="version" name="version" />
<input type="hidden" id="buildno" name="buildno" />
<input type="hidden" id="release_time" name="release_time" />
<input type="hidden" id="comment" name="comment" />
<input type="hidden" id="comment_cn" name="comment_cn" />
<input type="hidden" id="type" name="type" value="" />
<input type="hidden" id="cmd" name="cmd" value="" />
</form>
</body>
</html>
