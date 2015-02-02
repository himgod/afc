<?php
$json=json_decode($_POST['json']);

$json_str = '{"data":[],"meta":{"rc":"ok"}}';
switch($json->cmd)
{
	case "block-sta":
	case "unblock-sta":
	case "kick-sta":
	case "unauthorize-guest":
	case "authorize-guest":
		break;
	default:
		break;
}
echo $json_str;
?>
