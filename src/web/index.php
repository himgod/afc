<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php 
	if(!isset($_SESSION)){
		include("session_start.php");
	}
	include("init_lang.php");
	include("publichead.php");
	include("message.php"); 
	//$_SESSION["check_ac_version"] = "";/* for new version info update after index refresh */
	if(ToWizardIfAdminNotExists())
	{
		return;
	}
	CheckLegality();
	if(isset($_SESSION['username']) 
		&& "" != $_SESSION['username'] 
		&& $_COOKIE["username"] != $_SESSION['username'])
	{
		setcookie('username', $_SESSION['username'],time()+10*24*3600);
	}
	if(isset($_SESSION['language_select']) 
		&& "" != $_SESSION['language_select'] 
		&& $_COOKIE["language"] != $_SESSION['language_select'])
	{
		setcookie('language',$_SESSION['language_select'],time()+10*24*3600);
	}
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' dir="ltr">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<meta name="viewport" content="width=device-width, initial-scale=1" />

<link href="/library/css/index.css" rel="stylesheet" type="text/css" />
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>


<?php 
	$_SESSION['maplet_http_port'] = "8080";
	echo "<title>".getSoftwareName()."</title>";
	
	importJsFor("manage");
	$username = (isset($_SESSION) && isset($_SESSION["username"]))?$_SESSION['username']:"";
	$lasttime = (isset($_SESSION) && isset($_SESSION["last_login_time"]))?$_SESSION['last_login_time']:"unknown time";
	$lastip = (isset($_SESSION) && isset($_SESSION["last_login_ip"]))?$_SESSION['last_login_ip']:"unknown address";
	if("" == $lasttime && "" == $lastip)
	{
		$lastinfo = search($lpublic, "firsttimelogin");
	}
	else
	{
		$lastinfo = $lasttime.search($lpublic,"loginfrom").$lastip;
	}
?>
<!--<link type="text/css" rel="stylesheet" media="all" href="/library/css/his.css" />
<link type="text/css" rel="stylesheet" media="all" href="/library/css/index2.css" />-->

<script type="text/javascript" src="/library/js/AFC-y-policy.1.0.0.js"></script>
    <?php include($_SERVER['DOCUMENT_ROOT']."/includes/styles/ie6_css.php");?>
    <!--[if IE 6]>
    <script type="text/javascript">
     alert("由于您使用的浏览器是IE6,由此出现的页面问题我们无法负责,建议您升级或采用高版本浏览器,如Chrome、Firefox等.");
  </script>
  <style type="text/css">
   #MainPanel div.tab-wrapper-fullscreen{width:96%;height:600px;}
   #MainPanel div.tab-wrapper-scrollable{width:96%;}
  </style>
  <![endif]-->




<!--[if gt IE 5]> 
<style type="text/css">
.time-select select,.select-style select,.select-AFi select{
	padding-bottom:5px;
}

.jiao{
	border-style:dashed solid dashed dashed;
    top:0px;
}
</style>
<![endif]-->

<!--[if IE 9]>
<style type="text/css">
.select-style select {
    height: 39px;
	}
</style>
<![endif]-->


<!--[if lt IE 9]>

<style type="text/css">
body{
	width:1400px;
}
.indicator-pic{
	width:134px;
}
#MainPanel DIV.naviation-tabs-background{
	width:180px;
}
</style>
<![endif]-->

<!--[if IE 7]>
<style type="text/css">
.select-style select {
	margin-top:-2px;
    margin-left:-2px;
    }
.contact-select{
	margin:0px;
	}
.system-add{
	border:none;
	}
.system-add img{
    margin-left: -25px;
    margin-top: 5px;
	}
.system-add label{
    top: -24px;
    position: relative;
    left: -7px;
	}
.radio-style INPUT[type='radio']{
	margin:3px 3px 0px 2px;
}
input#custom{
	margin:9px 2px 0px 2px;
}
.service_code{
	position:relative;
    top:-40px;
}
</style>
<![endif]-->
    
</head>
<body>
<div id="model-aps" style="display:none"></div>
	<div id="model-stas" style="display:none"></div>
	<div id="model-events" style="display:none"></div>
    <div id="model-uevents" style="display:none"></div>
	<div id="model-maps" style="display:none"></div>
	<div id="model-apfilters" style="display:none"></div>
	<div id="model-usergroups" style="display:none"></div>
	<div id="model-wlanconfs" style="display:none"></div>
	<div id="model-maintabpagesizer" style="display:none"></div>
	<input type="text" id="ui-locate-blinker" style="display:none" />

	<?php include ("includes/dialogs/system-dialog.php");?>
	<?php include ("includes/dialogs/gmap-dialog.php");?>
	
<div id="PageContent">
<!-- Start: Header -->
<div id="PageHeader">
	<h1 id="Logo"><?php echo getSoftwareName();?></h1>
	<div id="AdminControls">
        <ul>
            <li class="system-name"><?php echo search($lpublic, "system_name");?>:<a href="#">AFC-279DD0</a></li>
            <li class=""><?php echo search($lpublic, "welcome_").$username;?></li>
            <li><a title='<?php echo $lastinfo?>' href="#"><?php echo search($lpublic, "lastlogin");?></a></li>
            <li><a href="/logout.php"><?php echo search($lpublic, "logout");?></a></li>
        </ul>
    </div>
	<div id="RefreshControl">
		<form>
		<fieldset>
			<label ><a href="javascript:void(0);" id="RefreshLink"><?php echo search($lpublic, "refresh");?></a></label>
			<div class="refreshTime-select select-style">
                <select id="RefreshSelect">
    <?php
                    $options = array(0, 10, 30, 60, 300, 600);
                    $default = $options[2];
                    if(isset($_SESSION["default_refresh_time"]))
                    {
                        $default = $_SESSION["default_refresh_time"];
                    }
                    $i = 0;
                    for($i = 0;$i < count($options); $i++)
                    {
                        $optionvalue = "";
                        $select = "";
                        if($options[$i] == 0)
                        {
                            $optionvalue = search($lpublic, "manually");
                        }
                        else if($options[$i] == 1)
                        {
                            $optionvalue = search($lpublic, "every").$options[$i].search($lpublic, "second");
                        }
                        else if($options[$i] > 1 && $options[$i] < 60)
                        {
                            $optionvalue = search($lpublic, "every").$options[$i].search($lpublic, "seconds");
                        }
                        else if($options[$i] == 60)
                        {
                            $optionvalue = search($lpublic, "every").round($options[$i]/60).search($lpublic, "minute");
                        }
                        else if($options[$i] > 60)
                        {
                            $optionvalue = search($lpublic, "every").round($options[$i]/60).search($lpublic, "minutes");
                        }
                        if($default == $options[$i])
                        {
                            $select = " selected='selected' ";
                        }
                        echo "<option value='".$options[$i]."' ".$select.">".$optionvalue."</option>";
                    }				
    ?>
                </select>
            </div>
			</fieldset>
		</form>
	</div>
	
</div>

<!-- End: Header -->

<!-- Start: Body -->
<div id="MainPanel">
	<div class="naviation-tabs-background">
	<ul class="navigation-tabs">
		<li id="navtab1" class="index-navigation-tab1"><a  href="#Overview"><?php echo search($lpublic, "overview");?></a></li>
		<li id="navtab2" class="index-navigation-tab2"><a href="#Map"><?php echo search($lpublic, "map_for_tab");?></a></li>
		<li id="navtab3" class="index-navigation-tab3"><a href="#System"><?php echo search($lpublic, "system");?></a></li>
		<li id="navtab4" class="index-navigation-tab4"><a href="#App">A&nbsp;P&nbsp;P</a></li>
		<li id="navtab5" class="index-navigation-tab5"><a href="#Support"><?php echo search($lpublic, "support");?></a></li>
	</ul>
	</div>
	<div id="Overview" class="tab-wrapper-scrollable">
		<div class="tab-content">
		<!-- Start: Overview Tab -->
		<?php include ("includes/tabs/overview-tab-content.php");?>
		<!-- End: Overview Tab -->
		</div>
	</div>
	
	<div id="Map" class="tab-wrapper-fullscreen">
		<!-- Start: Map Tab -->
        <div class="tab-content">
		<?php include ("includes/tabs/map-tab-content.php");?>
		</div>
        <!-- End: Map Tab -->
	</div>
	
	<div id="System" class="tab-wrapper-scrollable">
		<div class="tab-content">
		<!-- Start: System Tab -->
		<?php include ("includes/tabs/system-tab-content.php");?>
		<!-- End: System Tab -->
		</div>
	</div>
    <div id="App" class="tab-wrapper-scrollable">
		<div class="tab-content">
		<!-- Start: App Tab -->
		<?php include ("includes/tabs/app-tab-content.php");?>
		<!-- End: App Tab -->
		</div>
	</div>
	<div id="Support" class="tab-wrapper-scrollable">
		<div class="tab-content">
		<!-- Start: Support Tab -->
		<?php include ("includes/tabs/support-tab-content.php");?>
		<!-- End: Support Tab -->
		</div>
	</div>

<!-- End: Body -->
</div>

<!-- Start: Access Point Dialog -->
<div class="ap-dialog floating-dialog ui-corner-all" style="display:none;">
	<div class="floating-dialog-content ui-corner-all">
	<?php include ("includes/dialogs/access-point-dialog-content.php");?>
	</div>
</div>

<div class="sta-dialog floating-dialog ui-corner-all" style="display:none;">
	<div class="floating-dialog-content ui-corner-all">
	<?php include ("includes/dialogs/sta-dialog-content.php");?>
	</div>
</div>

<!-- End: Access Point Dialog -->
	</body>
</html>
