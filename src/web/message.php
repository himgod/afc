<?php
function typetopath($ftype)
{
 $rootpath = $_SERVER['DOCUMENT_ROOT'];
 $fileprefix = trim($rootpath)."/text/php_";
 $filepostfix = ".txt";
 return $fileprefix.$ftype.$filepostfix;
}

//function languageRefresh($lpublic)
{
	if(languageIsChinese())
	{		
		$lpublic = ext_funcpublic("get_chain_head","zh", typetopath("public"));
		$lcountry = ext_funcpublic("get_chain_head","zh", typetopath("country"));
	}
	else
	{
		$lpublic = ext_funcpublic("get_chain_head","en",typetopath("public"));
		$lcountry = ext_funcpublic("get_chain_head","en", typetopath("country"));
	}
}
function search($list,$message)
{
	for( $i = 0; $i < $list->length[0]; $i++ ) {
		if(0 == strcmp($list->value[$i][0], $message))
		{
			//echo " i ".$i." message ".$list->value[$i][0]. " value ". $list->value[$i][1]."\n";
			return $list->value[$i][1];
		}
	}
	return "";
}
?>
