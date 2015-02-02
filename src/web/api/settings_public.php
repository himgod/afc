<?php
function php_set_to_xml($filename, $p_node,$c_node,$value, $setall)
{	
	$xmlDoc = new DOMDocument();
	$xmlDoc->load($filename);
	$xmlNod=$xmlDoc->getElementsByTagName($p_node);
	$i = 0;
	while($xmlNod->item($i) != null)
//	for($i=0;$i < $itemsnum;$i++)
	{
		$aa=$xmlNod->item($i)->getElementsByTagName($c_node);
		foreach($aa as $params)
		{
			$params->nodeValue=$value;
			if(false == $setall)
			{
				$xmlDoc->save($filename);
				return "setsingle";
				//break;
			}
		}
		$i++;
	}
	$xmlDoc->save($filename);
	return "setall";
}

function php_del_all_nodes_by_name($nodename)
{
// delete child node of root by node name 
        $p_node=$nodename;
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
        $filename=$rootpath."/xml_php.xml";
        $xmlDoc = new DOMDocument();
        $xmlDoc->load($filename);
        $rootNod=$xmlDoc->getElementsByTagName("root")->item(0);
        $xmlNod=$xmlDoc->getElementsByTagName($p_node);
        $i = 0;
        while(null != $xmlNod->item($i))
        {
        	$rootNod->removeChild($xmlNod->item($i));
        }
	$xmlDoc->save($filename);
        return "success";
}

function php_del_all_users()
{
	return php_del_all_nodes_by_name("usr_inf");
}

function php_del_one_user($username)
{
        $p_node="usr_inf";
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
	$filename=$rootpath."/xml_php.xml";
        $xmlDoc = new DOMDocument();
        $xmlDoc->load($filename);
	$rootNod=$xmlDoc->getElementsByTagName("root")->item(0);
        $xmlNod=$xmlDoc->getElementsByTagName($p_node);
        $i = 0;
        while(null != $xmlNod->item($i))
        {
                $usernode=$xmlNod->item($i)->getElementsByTagName("usr_name");
                $k = 0;
                while($usernode->item($k) != null)
                {
                        $tmpuser=$usernode->item($k)->nodeValue;
                        if($username == $tmpuser)
                        {
				$rootNod->removeChild($xmlNod->item($i));
				$xmlDoc->save($filename);
				return "success";
                        }
                        $k++;
                }
                $i++;
        }
	return "nouser";
}

function php_modify_user_info($username, $infoname, $value)
{
        $p_node="usr_inf";
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
        $filename=$rootpath."/xml_php.xml";
	$xmlDoc = new DOMDocument();
        $xmlDoc->load($filename);
        $xmlNod=$xmlDoc->getElementsByTagName($p_node);
        $i = 0;
        if($infoname == "usr_name")
        {
        	$oldusers=php_get_user_name_list();
		if($oldusers != null)
		{
			foreach($oldusers as $username)
			{
				if($username == $value)
				{
					return "nameconflict";
				}
			}
		}
        }
        while(null != $xmlNod->item($i))
        {
                $usernode=$xmlNod->item($i)->getElementsByTagName("usr_name");
                $cnode   =$xmlNod->item($i)->getElementsByTagName($infoname);
                $k = 0;
                while($usernode->item($k) != null)
                {
                        $tmpuser=$usernode->item($k)->nodeValue;
                        if($username == $tmpuser)
                        {
                        	$cnode->item($k)->nodeValue = $value;
				$xmlDoc->save($filename);
				return "success";
                        }
                        $k++;
                }
                $i++;
        }
	return "nouser";
}

function php_add_new_user($userinfo)
{
	$p_node="usr_inf";
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
	$filename=$rootpath."/xml_php.xml";
	$xmlDoc = new DOMDocument();
	$xmlDoc->load($filename);
	$xmlNod=$xmlDoc->getElementsByTagName("root")->item(0);
	$oldusers=php_get_user_name_list();
	if($oldusers != null)
	{
		foreach($oldusers as $username)
		{
			if($username == $userinfo['name'])
			{
				return "userexists";
			}
		}
	}
	$usr_inf_node=$xmlDoc->createElement($p_node);
	
	foreach($userinfo as $info=>$value)
	{
		if($value)
		{
			$infoNode=$xmlDoc->createElement($info);
			$infoText=$xmlDoc->createTextNode($value);
			$infoNode->appendChild($infoText);
			$usr_inf_node->appendChild($infoNode);
		}
	}
	$xmlNod->appendChild($usr_inf_node);
	$xmlDoc->save($filename);
	return "success";
}

function php_get_items_from_xml($filename, $p_node,$c_node, $getall)
{
	$xmlDoc = new DOMDocument();
	$xmlDoc->load($filename);
	$xmlNod=$xmlDoc->getElementsByTagName($p_node);
	$k=0;
	$values=array();
	$i = 0;
	while(null != $xmlNod->item($i))
	{
		$cnode=$xmlNod->item($i)->getElementsByTagName($c_node);
		foreach($cnode as $onenode)
		{
			if(false == $getall)
			{
				return $onenode->nodeValue;
			}
			$values[$k]=$onenode->nodeValue;
			$k++;
		}
		$i++;	
	}
	
	if($k==0)
	{
		return null;
	}
	else
	{
		return $values;
	}
}

function php_get_user_name_list()
{
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
        $filename = $rootpath."/xml_php.xml";
        $p_node = "usr_inf";
        
        return php_get_items_from_xml($filename, $p_node, "usr_name", true);
}

function php_get_user_info($username, $c_node)
{
	$p_node="usr_inf";
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
	$filename=$rootpath."/xml_php.xml";
	$xmlDoc = new DOMDocument();
	$xmlDoc->load($filename);
	$xmlNod=$xmlDoc->getElementsByTagName($p_node);
	$k=0;
	$cnodeValue="";
	$i = 0;
	while(null != $xmlNod->item($i))
	{
		$usernode=$xmlNod->item($i)->getElementsByTagName("usr_name");
		$cnode   =$xmlNod->item($i)->getElementsByTagName($c_node);
		$k = 0;
		while($usernode->item($k) != null)
		{
			$tmpuser=$usernode->item($k)->nodeValue;
			$cnodeValue = $cnode->item($k)->nodeValue;
			if($username == $tmpuser)
			{
				
				//echo " get ".$c_node." info for user ".$username.": ".$cnodeValue."\n";
				return $cnodeValue;
			}
			$k++;
		}
		$i++;
	}       
	//echo " get ".$c_node." info for user ".$username.": null \n";
  return null;
}

function php_get_one_admin_user_name()
{
        $p_node="usr_inf";
	$rootpath=$_SERVER['DOCUMENT_ROOT'];
        $filename=$rootpath."/xml_php.xml";
        $xmlDoc = new DOMDocument();
        $xmlDoc->load($filename);
        $xmlNod=$xmlDoc->getElementsByTagName($p_node);
        $k=0;
        $cnodeValue="";
        $i = 0;
        while(null != $xmlNod->item($i))
        {
                $usernode=$xmlNod->item($i)->getElementsByTagName("usr_name");
                $cnode   =$xmlNod->item($i)->getElementsByTagName("administrator");
                $k = 0;
                while($usernode->item($k) != null)
                {
                        $tmpuser=$usernode->item($k)->nodeValue;
                        $cnodeValue = $cnode->item($k)->nodeValue;
                        if($cnodeValue == "true")
                        {
                                return $tmpuser;
                        }
                        $k++;
                }
                $i++;
        }
  	return null;
}

function php_check_user_password($username, $passwd)
{
	if(!$username || !$passwd || $username == "" || $passwd == ""){return false;}
	return ($passwd == php_get_user_info($username, "usr_passwd"))?true:false;
}
//$userinfo["usr_name"]="test";
//$userinfo["usr_passwd"]="newpasswd";
//$rootpath=$_SERVER['DOCUMENT_ROOT'];
//$users=php_get_items_from_xml($rootpath."/xml_php.xml", "usr_inf", "usr_name", true);
//print_r($users);
//php_get_user_info_from_xml($rootpath."/xml_php.xml", "admin", "for_hotspot");
//php_del_all_users();
?>
