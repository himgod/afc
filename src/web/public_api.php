<?php

$cmd = isset($_REQUEST["cmd"]) ? $_REQUEST["cmd"]:"";
$modify = isset($_REQUEST["modify"]) ? $_REQUEST["modify"]:0;

$host = "localhost";
$user = "root";
$pass = "autelan";
$database = "afc_release_notes_DB";
$table = "public_api";

$con = mysql_connect($host, $user, $pass);
if (!$con)
{
  die('Could not connect: ' . mysql_error());
}

mysql_select_db($database, $con);
if($cmd == "add")
{
	$name = isset($_REQUEST["name"]) ? $_REQUEST["name"]:"";
	$language = isset($_REQUEST["language"]) ? $_REQUEST["language"]:"";
	$definition = isset($_REQUEST["definition"]) ? $_REQUEST["definition"]:"";
	$describe = isset($_REQUEST["describe"]) ? $_REQUEST["describe"]:"";
	
	$definition = str_replace("\\", "\\\\", $definition);
	$definition = str_replace("'", "\'", $definition);
	$describe = str_replace("\\", "\\\\", $describe);
	$describe = str_replace("'", "\'", $describe);
	
	$startver = isset($_REQUEST["startver"]) ? $_REQUEST["startver"]:"";
	$endver = isset($_REQUEST["endver"]) ? $_REQUEST["endver"]:"";
	$pub_time = (isset($_REQUEST["pub_time"]) && "" != $_REQUEST["pub_time"])? 
					"'".$_REQUEST["pub_time"]."'":"NULL";
	

	if($name != "" && $language != "" && $definition != "")
	{
		$insert_str = "insert into ".$table." values(NULL, '".$name."', '".$language."', '"
			.$definition."', '".$describe."', '".$startver."', '".$endver."', ".$pub_time.")";
			error_log($insert_str);
		mysql_query($insert_str);
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
Plugin API Release Notes
</title>
<script type="text/javascript">
function add_new_item()
{	
	document.getElementById("api_add").style.display = "block";
	if(document.getElementById("api_r_now").checked)
	{
		document.getElementById("apipub_time").disabled = true;
		document.getElementById("apipub_time").value = get_current_time();
	}
}
function add_form_cancel()
{
	document.getElementById("api_add").style.display = "none";
}

function get_current_time()
{
	var now = new Date();
	var timestr = ""+now.getFullYear()+"-"+(1+now.getMonth())+"-"+now.getDate()
					+" "+now.getHours()+":"+now.getMinutes()+":"+now.getSeconds();
	return timestr;
}

function add_form_reset()
{
	document.getElementById("apiname").value = "";	
	document.getElementById("apilanguage").value = "";
	document.getElementById("apidefinition").value = "";	
	document.getElementById("apidescribe").value = "";	
	document.getElementById("apistartver").value = "";	
	document.getElementById("apiendver").value = "";	
	document.getElementById("api_r_now").checked = true;
	document.getElementById("apipub_time").disabled = true;
	document.getElementById("apipub_time").value = get_current_time();
}

function add_form_submit()
{	
	document.getElementById("name").value = document.getElementById("apiname").value;
	document.getElementById("language").value = document.getElementById("apilanguage").value;
	document.getElementById("definition").value = document.getElementById("apidefinition").value;
	document.getElementById("describe").value = document.getElementById("apidescribe").value;
	document.getElementById("startver").value = document.getElementById("apistartver").value;
	document.getElementById("endver").value = document.getElementById("apiendver").value;
	document.getElementById("pub_time").value = document.getElementById("apipub_time").value;
	document.getElementById("cmd").value = "add";
	document.getElementById("form1").submit();
}

function delete_item(id)
{
	if(!confirm("Are you sure to delete this item?"))
	{
		return false;
	}
	document.getElementById("id").value = id;
	document.getElementById("cmd").value = "delete";
	document.getElementById("form1").submit();
}

function release_radio_check()
{
	if(document.getElementById("api_r_now").checked)
	{
		document.getElementById("apipub_time").disabled = true;
		document.getElementById("apipub_time").value = get_current_time();
	}
	else if(document.getElementById("api_r_input").checked)
	{
		document.getElementById("apipub_time").disabled = false;
		document.getElementById("apipub_time").select();
	}
}
</script>
<style type="text/css"> 
table .datatable{ border-collapse:collapse;font-size:12px;font-family:Helvetica, Arial, sans-serif;}
table .datatable td{border:1px solid #3F3F3F;}
table .datatable th{background:#999;height:60px;border:1px solid #3F3F3F;}
.thh{padding:30px 0px 0px 20px;font-size:15px;font-weight:bold;}
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
<table width="1200px">
<tr>
<td>
<div align="center" style="background:#CCDBEC;border:1px solid #3F3F3F;font-size:15px;font-weight:bold;">
Plugin API Release Notes
<table width="1200px" class="datatable" style='margin:20px 20px 0px 10px;padding-left:10px;border:1px solid #3F3F3F;'>
<tr>
<th>Num</th>
<th>Name</th>
<th>Language</th>
<th>Definition Time</th>
<th>Describe</th>
<th>Start Version</th>
<th>End Version</th>
<th>Release Time</th>
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

$result = mysql_query("SELECT * FROM ".$table);

$i = 1;
while($row = mysql_fetch_array($result))
{
  echo "<tr>";
  echo "<td>" . $i++ . "</td>";
  echo "<td>" . $row['name'] . "</td>";
  echo "<td>" . $row['language'] . "</td>";
  echo "<td>" . $row['definition'] . "</td>";
  echo "<td>" . str_replace("\n", "<br/>", $row['describe']) . "</td>";
  echo "<td>" . $row['startver'] . "</td>";
  echo "<td>" . $row['endver'] . "</td>";
  echo "<td>" . $row['pub_time'] . "</td>";
  if($modify)
  {
  	echo "<td><input type='button' onclick='delete_item(\"" . $row['id'] . "\")' value='Delete' /></td>";
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
<input type='button' onclick='add_new_item();' value='Add...' /><br/><br/>
<?php
}
?>
<div id="api_add" style="display:none">
<table width="600px" style="background:#CCDBEC;">
<tr>
<td>
Name: </td><td colspan=2><input type="text" id="apiname" />
</td>
</tr>
<tr>
<td>
Language: </td><td colspan=2><input type="text" id="apilanguage" /> (C,PHP,...)
</td>
</tr>
<tr>
<td>
Definition: </td><td colspan=2><input type="text" id="apidefinition" />
</td>
</tr>
<tr>
<td>
Describe: </td><td colspan=2><textarea cols="40" rows="5" id="apidescribe" ></textarea>
</td>
</tr>
<tr>
<td>
Start Version: </td><td colspan=2><input type="text" id="apistartver" />
</td>
</tr>
<tr>
<td>
End Version: </td><td colspan=2><input type="text" id="apiendver" />
</td>
</tr>
<tr>
<td>
Release Time: </td><td colspan=2>
<input type="radio" id="api_r_now" name="api_r_time" value="now" checked=true onclick="release_radio_check();"/>Current Time
<input type="radio" id="api_r_input" name="api_r_time" value="input"  onclick="release_radio_check();"/>Input Time: <input type="text" id="apipub_time" />
</td>
</tr>
</table>
<table width="600px" style="border:none;">
<tr>
<td align="center" ><input type="button" id="afcsubmit" value="Submit" onclick="add_form_submit();" /></td>
<td align="center" ><input type="reset" id="afcreset" value="Reset" onclick="add_form_reset();" /></td>
<td align="center" ><input type="button" id="afccancel" value="Cancel" onclick="add_form_cancel();"/></td>
</tr>
</table>
<br/>
</div>
</div>
</div>
</td>
</tr>
</table>
<form id="form1" method="post" action="<?php echo $_SERVER['SCRIPT_NAME'];?>?modify=1">
<input type="hidden" id="id" name="id" />
<input type="hidden" id="name" name="name" />
<input type="hidden" id="language" name="language" />
<input type="hidden" id="definition" name="definition" />
<input type="hidden" id="describe" name="describe" />
<input type="hidden" id="startver" name="startver" />
<input type="hidden" id="endver" name="endver" />
<input type="hidden" id="pub_time" name="pub_time" value="" />
<input type="hidden" id="cmd" name="cmd" value="" />
</form>
</body>
</html>

