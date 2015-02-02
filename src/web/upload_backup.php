<?php
error_log($_FILES["restoreFile"]["tmp_name"]);
error_log(system("cat ".$_FILES["restoreFile"]["tmp_name"]));
$json_str = '{"data": [], "meta": {"filename":"'.$_FILES["restoreFile"]["name"].'","size":"1","filesize":"129136","rc":"ok","timestamp":"'.(string)(time()*1E3).'","version":"2.4.6"}"}';

echo $json_str;
?>
