<?php
$json=json_decode($_POST['json']);

$json_str = '{"data": [], "meta": {"rc": "ok"}}';
$json_apply='{"data":[{"_id":"53b21e2bf5f929f8e1f424fd","first_seen":1404182057,"hostname":"MI2S-xiaomishouji","is_guest":true,"last_seen":1404985275,"mac":"f8:a4:5f:85:98:af","name":"11111","note":"w","noted":true,"oui":"","usergroup_id":"538d4aba508629f8b5b6ebf9"}],"meta":{"rc":"ok"}}';
$json_fixed_ap = '{"data":[{"_id":"53b21e2bf5f929f8e1f424fd","first_seen":1404182057,"fixed_ap":"24:a4:3c:8c:07:c0","hostname":"MI2S-xiaomishouji","is_guest":true,"last_seen":1404985419,"mac":"f8:a4:5f:85:98:af","name":"11111","note":"w","noted":true,"oui":"","usergroup_id":"538d4aba508629f8b5b6ebf9"}],"meta":{"rc":"ok"}}';
if($json->name)
{
	echo $json_apply;
}
else if($json->fixed_ap)
{
	echo $json_fixed_ap;
}
?>
