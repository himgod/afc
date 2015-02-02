<?php
$json_for_order0='{"data":[{"_id":"5388665748f029f89fb8d52f","file_id":"5388665748f029f89db8d52f","name":"Sample","order":0,"selected":true,"unit":"m","upp":0.014906338472313001,"url":"/file/map/5388665748f029f89db8d52f"}],"meta":{"rc":"ok"}}';
$json_for_order1='{"data":[{"_id":"53bcaef0422729f8edea045a","file_id":"53bcaef0422729f8ebea045a","name":"Sample222","order":1,"selected":false,"url":"/file/map/53bcaef0422729f8ebea045a"}],"meta":{"rc":"ok"}}';
$json_for_modify='{"data":[{"_id":"5388665748f029f89fb8d52f","file_id":"5388665748f029f89db8d52f","name":"Sample1","order":0,"selected":true,"unit":"m","upp":0.014906338472313001,"url":"/file/map/5388665748f029f89db8d52f"}],"meta":{"rc":"ok"}}';
$json=json_decode($_POST['json']);
if($json->order == 0)
{
	echo $json_for_order0;
}
else if(1 == $json->order)
{
	echo $json_for_order1;
}
else if($json->name)
{
	echo $json_for_modify;
}
else
{
	$meta["rc"] = "ok";
	$meta["msg"] = json_encode($json);
	$json_obj = array("data"=>array(), "meta"=>$meta);
	echo json_encode($json_obj);
}

?>
