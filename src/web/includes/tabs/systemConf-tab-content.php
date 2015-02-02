<div class="system-div">
	<div class="system-title" id="en-system-title">
    	<label><?php echo search($lpublic, "basic_configuration");?></label>
   	</div>
    <div class="wirlessConf-content basic-configuration" id="en-basic-configuration">
  	 <form id="basicConfForm" action="">
    <fieldset>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "system_name");?></label>            
            <input type="text" name="SysName" value="AFC-279DD0" />           
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "country");?></label>
           	 <div class="select-style country-select">
                    <select id="CountryCode" name="countryCode">
                    </select>
                </div>
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "language2");?></label>
            <div class="select-style country-select">
                <select id="Lang" name="lang">
                    <option value="auto"><?php echo search($lpublic, "auto_for_language");?></option>
                    <option value="en_US">English</option>
                    <option value="zh_CN"><?php echo search($lpublic, "chinese");?></option>
                </select>
            </div>
        </p>
        <p class="admin-name wirlessConf-content-element">
        	<label><?php echo search($lpublic, "admin_name");?></label>
           
			<input name="_id" type="hidden" autocomplete="false"  />
			<input name="name" type="text" autocomplete="false" value="zhangsanfeng@autelan.com" />
			<input name="oldname" type="hidden" autocomplete="false"  />
			
           
                <input type="checkbox" id="open-wlan" />
				<label id="open-wlan-info" class="open-wlan-info" for="open-wlan"><?php echo search($lpublic, "open_wlan_info");?></label>
           
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "oldpassword");?></label>
          
                <input type="text" name="OriginalPass" value="******" />
           
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "newpassword");?></label>
           
                <input type="text" name="newPass" value="******" />
        
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "password_conf");?></label>
            
                <input type="text" name="ConfirmPass" value="******" />
            
        </p>
        </fieldset>
        <p>
        	<input name="apply" type="button" class="add-page-button sys-conf-button" value='<?php echo search($lpublic, "submit");?>' />
        </p>
    </form>  
    </div>
	
</div>

<div class="system-div">
	<div class="system-title" id="en-system-title">
    	<label><?php echo search($lpublic, "service_configuration");?></label>
   </div>
   <form id="systemConfForm">
    <div class="wirlessConf-content service-configuration" id="en-service-configuration">
        <p class="wirlessConf-content-element">
        	<label>AFC<?php echo search($lpublic, "firmware_update");?></label>
            <div id="f5" class="switch off" checked="false">
                
            </div>
        </p>
        <p class="wirlessConf-content-element">
        	<label><?php echo search($lpublic, "version_server");?></label>
            
            <input id='version_server' type="text" name="version" value="http://192.168.1.241" />
            
            
        </p>
        <p class="wirlessConf-content-element">
			<label><?php echo search($lpublic, "latest_version");?></label>
            <span id="ServerVersion"></span>
			<span id="UpgradeVersion" style="display:none"></span>
			<input id="UpgradeButton" type="button" name="upgrade" value="<?php echo search($lpublic, "upgrade");?>" class="button" style="display:none"/>
    
        </p>
        <p class="wirlessConf-content-element">
        	<label>AFC<?php echo search($lpublic, "config");?></label>
            <input id='BackUp' type="button" value='<?php echo search($lpublic, "backup");?>' />
            <input id='Restore' type="button" value='<?php echo search($lpublic, "restore");?>' />
        </p>
        <p class="wirlessConf-content-element">
            <label><?php echo search($lpublic, "event_notification");?></label>
            <div id="f6" class="switch off" checked="false"></div>
         </p>
            <p class="wirlessConf-content-element">
            	<label><?php echo search($lpublic, "mail");?></label>
                <div id="f7" class="switch off" checked="false"> </div>
            </p>
            <p class="wirlessConf-content-element">
            	<label><?php echo search($lpublic, "mail_server");?></label>
                
                 <input type="text" name="mailServer" value="https://mail.autelan.com" />
               
            </p>
            <p class="wirlessConf-content-element">
            	<label><?php echo search($lpublic, "enable_ssl");?></label>
                <div id="f8" class="switch off" checked="false"> </div>
            </p>
            <p class="wirlessConf-content-element">
            	<label><?php echo search($lpublic, "specify_sender_address");?></label>
                
                <input type="text" name="mailReceive" value="netspt@xiaoy.com" />
                
            </p>
            <p class="wirlessConf-content-element">
            	<label><?php echo search($lpublic, "weixin");?></label>
                <div id="f9" class="switch off" checked="false"> </div>
            </p>
        
    </div>
    </form>
</div>

<div id="openWlan-page">
	 <!-- Start: openWlan Tab -->
           <?php include ("includes/tabs/openWlan.php");?>
      <!-- End: openWlan Tab -->
</div>