<?php
$json = "";
$filepath = "/opt/run/wcpss/y-afi.log";
if(isset($_REQUEST["json"]))
{
	$json = $_REQUEST["json"];
}
$data = array();
$myfile = fopen($filepath, "r") or die("Unable to open file!");
$i = 0;
while(!feof($myfile)) {
	$content = fgets($myfile);
	if($content)
	{
		$data[$i++] =  $content;
	}
}
fclose($myfile);
//$data = array("test wai info 1", "tet wai info 2");
$meta["rc"] = "ok";

$result = array("data"=>$data, "meta"=>$meta);

echo json_encode($result);

?>
