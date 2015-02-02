
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
	include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
	include("init_lang.php");
	include("publichead.php");
	include("message.php"); 
	$ret = ToIndexIfAdminExists();
	//if(true == $ret)
	//{
		//return;
	//}
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' lang='<?php echo languageIsChinese()?"zh_CN":"en"; ?>' dir="ltr">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<link href="/library/css/wizard.css" rel="stylesheet" type="text/css" />
<head>	
<script type="text/javascript" src="/library/js/md5.js"></script>
<?php
	if(true != $ret)
	{
		ext_admin("init_softac_config");
	}
	importJsFor("wizard");
	//$waiting_cmd = $_REQUEST['waiting_cmd'];
?>
<script type="text/javascript" src="/library/js/autoComplete.js"></script>


  <title><?php echo search($lpublic, "wizard")." - ".getSoftwareName();?></title>


<meta http-equiv = "X-UA-Compatible" content = "IE=edge" />
 

<!--[if lte IE 7]>
<style type="text/css">
.wizard-page{
	border:2px #eee solid;
}
.wizard-navigation-background{
	margin-top:20px;
}
.wizard-navigation-background ul{
	margin:0px;
}
.wizard-navigation-background ul li{
	width:130px;
}

.next-previous-controls{
	bottom:20px;
}
#button{
	 border:0px;
}
</style>
<![endif]-->
  
<!--[if lte IE 8]>
<style type="text/css">
.next-button{
	background:url('/pic/button-blue2.png') no-repeat;
}
.back-button{
	background:url('/pic/button-white.png') no-repeat;
    border:0px;
}
#refresh-but{
	background:url('/pic/button-white2.png') no-repeat;
    border:0px;
}
.finish-button{
	background:url('/pic/button-orange.png') no-repeat;
    border:0px;
}
#en-form-element input[type="text"],#en-form-element input[type="password"]{
	background:url('/pic/input-border2.png') no-repeat;   
    border:0px;
    line-height:28px;
    width:232px;
}
.eye{
	margin-left:-10px;
}
.Password{
    background:url('/pic/input-border2.png') no-repeat;   
    border:0px;
}
.time-select select,.welcome-select select{
	padding-bottom:8px;
}

</style>
<![endif]-->
  
<!--[if lte IE 9]>
<style type="text/css">
#EnableGuestAccess{
	margin-top:5px;
}
</style>
<![endif]-->
    

  
</head>
<body class="wizard-page" id="en-wizard-page">

    <div class="modal">
        <div class="loading-indicator ui-corner-all">
            <img class="loading-image" src="/media/global/loader-black.gif" alt="Loading" />
            <span class="loading-message"><?php echo search($lpublic, "saving_settings");?></span>
        </div>
    </div>
    
    <div class="page-content">
        <form id="WizardForm" action="/">
            <div class="head">
                <div id="Logo" ></div>        
            </div>
            <div class="wizard-content content-box">
                <div class="wizard-navigation-background">
                    <ul class="wizard-navigation" id="en-wizard-navigation">
                        <li id="WelcomeTab"><?php echo search($lpublic, "welcome");?></li>
                        <li id="AdministrationTab"><?php echo search($lpublic, "administration");?></li>
                        <li id="DiscoverTab"><?php echo search($lpublic, "discover");?></li>
                        <li id="WirelessTab"><?php echo search($lpublic, "config");?></li>
                        <li id="FinishTab"><?php echo search($lpublic, "finish");?></li>
                    </ul>
                </div>
                <div class="navigation-active">
                    
                </div>
    
                <div id="WelcomeContent" class="section-content">
                    <p class="wizard-title"><?php echo search($lpublic, "welcome_info");?></p>
                    <br />
                    <p>
                        <label class="welcome-lable" for="Lang"><?php echo search($lpublic, "select_language_info");?></label>
                        <div class="welcome-select">
                            <select id="Lang" name="lang">
                                <option value="auto"><?php echo search($lpublic, "auto_for_language");?></option>
                                <option value="en_US">English</option>
                                <option value="zh_CN"><?php echo search($lpublic, "chinese");?></option>
                            </select>
                        </div>
                    </p>
                    <p>
                        <label class="welcome-lable" for="CountryCode"><?php echo search($lpublic, "select_contry");?></label>
                        <div class="welcome-select">
                            <select id="CountryCode" name="countryCode">
                                
                            </select>
                        </div>
                    </p>
                </div>
    
                <div id="AdministrationContent" class="section-content">
                    <p  class="wizard-title"><?php echo search($lpublic, "enter_name_and_password");?></p>
                    <fieldset class="admini-table">
                        <p class="form-element" id="en-form-element">
                            <label for="AdminName"><?php echo search($lpublic, "administrator_name");?></label>
                            
                            <input id="AdminName"  name="admin-name" type="text" onkeyup="this.value=this.value.replace(/^ +| +$/g,'')" autocomplete="off" value="admin" maxlength="32" />
                         	<div class="name-support-position name-support">
								<span class="bot"></span>
                                <span class="top"></span>
                                <div class="pass-rule">
                                	<div><span><?php echo search($lpublic, "support_char1");?></span></div>
                                    <div><span><?php echo search($lpublic, "support_char2");?></span></div>
                                </div>
								
                             </div>   
                        </p>
                        
                        <p class="form-element" id="en-form-element">
                            <label><?php echo search($lpublic, "administrator_password");?></label>
                            <div class="Password">
                            	<input id="Password" name="admin-pass1" type="password" autocomplete="off" value="admin" />
                                <div class="eye"   onclick="changepasswd(this);"></div>
                            </div>
                 			<div class="pass-not"><span><?php echo search($lpublic, "pass_not");?></span></div>
                            <div class="pass-support-positon pass-support">
                            	<span class="bot"></span>
                                <span class="top"></span>
                                <div class="pass_stren">
                                    <span><?php echo search($lpublic, "weak");?></span>
                                    <span><?php echo search($lpublic, "medium");?></span>
                                    <span><?php echo search($lpublic, "stronger");?></span>
                                    <span><?php echo search($lpublic, "strongest");?></span>
                                </div>
                                <div class="pass-rule">
                                    <div>
                                        <span><?php echo search($lpublic, "support_pass1");?></span>
                                    </div>
                                    <div >
                                        <span><?php echo search($lpublic, "support_pass2");?></span>
                                    </div>
                                    <div class="pass-rule-position">
                                        <span><?php echo search($lpublic, "support_pass3");?></span>
                                    </div>
                                </div>
                       		</div>
                        </p>
                    
                        <p class="form-element" id="en-form-element">
                            <label for="PasswordConfirm"><?php echo search($lpublic, "password_conf");?></label>
                            <input id="PasswordConfirm" name="admin-pass2" type="password" autocomplete="off" value="admin"/>
                        </p>
                        <p class="openWlancheck" id="en-form-element">
                            <input id="openwlan" type="checkbox" value="checkbox"/>
                            <label id="openWlan-agreement" for="openwlan"><?php echo search($lpublic, "open_wlan_info");?></label>
                            <span class="openWlan-title">(<a class="openWlanHref" href="http://www.open-wlan.org">http://www.open-wlan.org</a>)</span>
                        </p>
                    </fieldset>
                    <p class="wizard-title join-openWlan"><?php echo search($lpublic, "join_open_wlan");?></p>
                </div>
    
                <div id="DiscoverContent" class="section-content">
                    <br />
                    <div class="ap-data-table">
                        <div id="discovered">
                            <p class="wizard-title"><?php echo search($lpublic, "select_device_info"); ?>(<?php echo search($lpublic, "locate_info");?>)</p>
                        </div>
                        <div id="discovered-none">
                        	<p><?php echo search($lpublic, "select_device_info"); ?></p>
                            <p>
                                <span><?php echo search($lpublic, "no_devices_discovered");?></span>
                                <input type="button" value='<?php echo search($lpublic, "refresh");?>' class="refresh-button" id="refresh-but"/>
                            </p>
    
                            <p class="nextDirect">(<?php echo search($lpublic, "next_direct");?>)</p>
                        
                        </div>
                    </div>
                </div>
    
                <div id="WirelessContent" class="section-content">
                    <p class="wizard-title"><?php echo search($lpublic, "create_wireless_info");?></p>
                    <fieldset class="input-table">
                        <p class="form-element" id="en-form-element">
                            <label for="NetworkName"><?php echo search($lpublic, "wireless_networks");?></label>
                            <input id="NetworkName" name="corp-ssid" type="text"  maxlength="32" value="test" onkeyup="this.value=this.value.replace(/^ +| +$/g,'')" />
                           	<div class="name-support">
								<span class="bot"></span>
                                <span class="top"></span>
                                <div class="pass-rule">
                                	<div><span><?php echo search($lpublic, "support_char1");?></span></div>
                                    <div><span><?php echo search($lpublic, "support_char2");?></span></div>
                              	</div>
								
                             </div>   
                        </p>
                        <p class="form-element" id="en-form-element">
                            <label for="Passphrase"><?php echo search($lpublic, "connect_password");?></label>
                             <div class="Password">
                            <input id="Passphrase" name="corp-passphrase" type="password" maxlength="20" autocomplete="off" value="testtest"/>
                            <div class="eye"  onclick="changepasswd1(this);"></div>
                            </div>
                            <div class="pass-not-positon pass-not"><span><?php echo search($lpublic, "pass_not");?></span></div>
                             <div class="pass-support">
                            	<span class="bot"></span>
                                <span class="top"></span>
                                <div class="pass_stren">
                                    <span><?php echo search($lpublic, "weak");?></span>
                                    <span><?php echo search($lpublic, "medium");?></span>
                                    <span><?php echo search($lpublic, "stronger");?></span>
                                    <span><?php echo search($lpublic, "strongest");?></span>
                                </div>
                                <div id="WirelessPwd"  class="pass-rule">
                                    <div>
                                        <span><?php echo search($lpublic, "support_pass1");?></span>
                                    </div>
                                    <div >
                                        <span><?php echo search($lpublic, "support_pass2");?></span>
                                    </div>
                                    <div class="pass-rule-position">
                                        <span><?php echo search($lpublic, "support_pass3");?></span>
                                    </div>
                                </div>
                       		</div>
                        </p>
                        
                        <p class="form-element" id="en-form-element">
                            <label><?php echo search($lpublic, "pass_valid");?></label>
                            <div class="time-select">
                                <select id="Time" name="time">
                                    <option value="day"><?php echo search($lpublic, "one_day");?></option>
                                    <option value="week"><?php echo search($lpublic, "one_week");?></option>
                                    <option selected value="month"><?php echo search($lpublic, "one_month");?></option>
                                    <option value="year"><?php echo search($lpublic, "one_year");?></option>
                                </select>
                            </div>
                        </p>
                        
                    </fieldset>
                    <hr class="line" />
                    <fieldset >
                        <p class="wizard-title"><?php echo search($lpublic, "create_open_wireless");?></p>
                        <p class="form-element check-guest" id="en-form-element">
                            <label for="EnableGuestAccess"><?php echo search($lpublic, "enable_guest_access");?></label>
                            <input id="EnableGuestAccess" name="do-guest" type="checkbox" />
                        </p>
                        <p class="enable-guest-access" style="display:none" id="en-enable-guest-access">
                            <?php echo search($lpublic, "guest_wireless_networks");?>
                        </p>
                    </fieldset>
                </div>
    
                <div id="FinishContent" class="section-content">
                    <p class="wizard-title"><?php echo search($lpublic, "review_settings_info");?></p>
                    <dl id="WizardSummary">
                     
                    </dl>
                    <div id="Qr-code" >
                    	<div class="wireless-Qrcode-title"><div class="Qr-symbol"></div><span class="en-wireless-net"><?php echo search($lpublic, "wireless_net");?></span></div>
                    	<div class="wireless-Qrcode"></div>                        
                        <div class="guest-Qrcode"></div>
                        <div class="guest-Qrcode-title"><div class="Qr-symbol2"></div><span class="en-guest-wireless"><?php echo search($lpublic, "guest_wireless");?></span></div>
                    </div>
                </div>
                <p class="next-previous-controls">
                    
                    <input name="backButton" value='<?php echo search($lpublic, "back");?>' type="button" class="back-button" id='button' />
                    <input name="nextButton" value='<?php echo search($lpublic, "next");?>' type="submit" class="next-button" id='button' />
                    <input name="finishButton" value='<?php echo search($lpublic, "finish");?>' type="button" class="finish-button" id='button' />
                    <input name="submitButton" value='<?php echo search($lpublic, "submit");?>' type="button" class="next-button" id='button' />
                    
                </p>
            </div>
        </form>
    </div>
    <div id="openWlan-page">
	 <!-- Start: openWlan Tab -->
           <?php include ("includes/tabs/openWlan.php");?>
      <!-- End: openWlan Tab -->
</div>
</body>
</html>
