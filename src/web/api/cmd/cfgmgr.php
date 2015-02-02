<?php
include("../../publichead.php");
$json_str = '{"data": [], "meta": {"rc": "ok"}}';
$json_for_select_map = $json_str;
$json_for_delete_map = $json_str;
$json_for_test_mail = $json_str;
$json = json_decode($_POST['json']);
if($json->cmd == "select_map")
{
	echo $json_for_select_map;
}
else if($json->cmd == "delete_map")
{
	echo $json_for_delete_map;
}
else if($json->cmd == "test_mail")
{
	$PHPMailerPath = "/usr/share/php/libphp-phpmailer";
	$PHPMailerLocation = $PHPMailerPath."/class.phpmailer.php";
	require_once($PHPMailerLocation);
	$mail = new PHPMailer();
	$address = $json->email;
	$mail->IsSMTP();
	if($json->use_ssl == true)
	{
		$mail->Host = "ssl://".$json->host;
	}
	else
	{
		$mail->Host = $json->host;
	}
	$mail->Port = $json->port;
	$mail->SMTPAuth = $json->use_auth;
	$mail->Username = $json->username;
	$mail->Password = $json->x_password;
	if(true != $json->use_sender || null == $json->sender || "" == $json->sender)
	{
	        $json->sender = $json->email;
	}
	
	$mail->SetFrom($json->sender, getSoftwareName());
	$mail->AddAddress($address, $address);
	$mail->Subject = "Test Mail from ".getSoftwareName();
	$mail->Body = "This is a test email generated from ".getSoftwareName()."<br>\n--<br>\n".getSoftwareName()."<br>\n";
	$mail->AltBody = $mail->Body;
	$mail->Send();
	
	echo $json_for_test_mail;
}
else if($json->cmd == "save_gsmap")
{
	echo $json_str;
}
?>
