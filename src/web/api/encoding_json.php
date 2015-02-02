<?php
function encoding_chinese($value)
{
	return urlencode(iconv('gb2312', 'utf-8', $value));
}

function json_encode_c($value)
{
	return json_encode(encoding_chinese($value));
}

?>
