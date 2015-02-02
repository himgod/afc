<?php
if(isset($_REQUEST))
{
	$request = $_REQUEST;

	$model = "";
	$mac = "";
	$sn = "";
	$date = "";

	if(isset($request["m"]))
	{
		$model = $request["m"];
	}
	if($request["a"])
	{
		$mac = $request["a"];
		$mac = trim($mac, '"');
		//$mac = preg_replace('/"([0-9a-fA-F]+)"/', '$1', $mac); same function with the prev line 
	}
	if(isset($request["s"]))
	{
		$sn = $request["s"];
	}
	if(isset($request["d"]))
	{
		$date = $request["d"];
	}
	
	echo "
<html>
	<head>
		<title>
			AFi-info
		</title>
	</head>
	<body>
		<table>
			<tr><td></td><td>model</td><td>: $model</td></tr>
			<tr><td></td><td>mac  </td><td>: \"001f64$mac\"</td></tr>
			<tr><td></td><td>sn   </td><td>: $sn</td></tr>
			<tr><td></td><td>date </td><td>: $date</td></tr>
		</table>
		";
		//system("/home/autelan/testupgrade_afi.sh");
		echo "
	</body>
<html>";
}
else
{
	echo "
<html>
	<head>
		<title>
			AFi-info
		</title>
	</head>
	<body>
		Error:<br/>
			get null request args!
	</body>
<html>";
}
?>
