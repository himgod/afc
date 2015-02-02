<?php
//include("api/settings_public.php");
$lang="";
if(!empty($_POST['Lang']))
{
	$lang=$_POST['Lang'];
}
else if(!empty($_REQUEST['lang']))
{
	$lang=$_REQUEST['lang'];
}
if($lang != "")
{
        if(!isset($_SESSION['language_select']) || $_SESSION['language_select'] != $lang)
        {
                if(isset($_SESSION['language_select']))
                {/* maybe not the first time access and language changed */
                	echo "<script type='text/javascript'>top.location.href=top.location.href;</script>";
                }
                else
                {/*here maybe mean session is invalidate */
                }
                $_SESSION['language_select'] = $lang;
        }
}
else if(!isset($_SESSION['language_select']) || $_SESSION['language_select'] == "")
{
	if(isset($_COOKIE["language"]))
	{
		$lang = $_COOKIE["language"];
	}

	if($lang != null && $lang != "")
	{
  			$_SESSION['language_select'] = $lang;
	}
}
if(!isset($_SESSION['language_select']))
{
        $_SESSION['language_select'] = 'auto';
}
?>
