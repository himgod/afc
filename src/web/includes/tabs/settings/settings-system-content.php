<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "settings");?>"><?php echo search($lpublic, "settings");?></a></li>
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "system");?>"><?php echo search($lpublic, "system");?></a></li>
</ul>

<form id="SystemForm" class="settings-form">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "system_configuration");?></legend>
	
		<label><?php echo search($lpublic, "system_name");?></label>
		<p>
			<input name="systemName" type="text" class="text-input" size="25" />
		</p>

    <label style="display:none"><?php echo search($lpublic, "management_url");?></label>
		<p style="display:none">
      <input name="useFixedURL" value="false" type="radio" class="radio-input" />
      <span class="label"><?php echo search($lpublic, "use_default_management_url");?> (<?php echo search($lpublic, "current");?>: <span class="current-management-url"></span>) </span><br />
      <input name="useFixedURL" value="true" type="radio" class="radio-input" />
      <span class="label"><?php echo search($lpublic, "use_fixed_management_url");?> (http://<input name="fixedURLHost" type="text" />:<input name="fixedURLPort" type="text" size="4"/> /inform)</span>
    </p>

		<label><?php echo search($lpublic, "country");?></label>
		<p>
			<select name="countryCode">
			</select>
		</p>
	</fieldset>

  <fieldset class="settings-fieldset">
	<legend><?php echo search($lpublic, "services");?></legend>
	
    <label><?php echo search($lpublic, "automatic_upgrade");?></label>
    <p>
      <input name="enableAutoUpgrade" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "automatically_upgrade_firmware");?></span>
    </p>

    <label>LED</label>
    <p>
      <input name="enableLED" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_status_led");?></span>
    </p>

    <label><?php echo search($lpublic, "backgroud_scanning");?></label>
    <p>
      <input name="enableBackgroundScanning" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_background_scanning");?></span>
			<input type="button" name="scanNow" value="<?php echo search($lpublic, "scan_now");?>" class="button" />
    </p>
		
    <label><?php echo search($lpublic, "load_balancing");?></label>
    <p>
      <input name="enableLoadBalance" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "balance_number_per_radio");?>:</span>
			<input name="maxSta" type="text" size="4" />
    </p>

    <label><?php echo search($lpublic, "network_discovery");?></label>
    <p>
      <input name="enableServerDiscoverable" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "make").getSoftwareName().search($lpublic, "discoverable_via_upnp");?></span>
    </p>

    <label><?php echo search($lpublic, "uplink_connectivity_monitor");?></label>
    <p>
      <input name="enableConnectivityMonitor" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_connectivity_monitor");?></span>
    </p>
    <label class="uplink-type"></label>
    <p class="uplink-type">
	  <input name="uplinkType" value="gateway" type="radio" class="radio-input" />
	  <span class="label"><?php echo search($lpublic, "use_default_gateway");?></span>
	  <input name="uplinkType" value="custom" type="radio" class="radio-input" />
	  <span class="label"><?php echo search($lpublic, "use_custom_ip");?></span>
    </p>

    <label class="custom-uplink-host"></label>
    <p class="custom-uplink-host"><?php echo search($lpublic, "uplink").search($lpublic, "ip_address");?>: 
	  <input name="uplink_host" type="text" />
    </p>

	

    <label><?php echo search($lpublic, "remote_logging");?></label>
    <p>
      <input name="enableRemoteSyslog" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_remote_syslog_server");?></span>
			<input name="syslogHost" type="text" /> <?php echo search($lpublic, "port");?> <input name="syslogPort" type="text" size="4" />
    </p>
	</fieldset>
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "mail_server");?></legend>
		<label><?php echo search($lpublic, "smtp_server");?></label>
		<p>
			<input name="enableSmtp" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_mail_server");?></span>
			<input name="smtpHost" type="text"/> <?php echo search($lpublic, "port");?> <input name="smtpPort" type="text" size="4" />
			<input name="smtpSSL" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_ssl");?></span>
		</p>
		<label class="mail-server"></label>
		<p class="mail-server">
			<input name="smtpAuthentication" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_authentication");?>:</span> <?php echo search($lpublic, "username");?> <input name="userName" type="text" size="16" /> <?php echo search($lpublic, "password");?> <input name="userPassword" type="text" size="16"/>
		</p>
		<label class="mail-sender"></label>
		<p class="mail-sender">
			<input name="smtpSender" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "specify_sender_address");?>:</span>
			<input name="sender" type="text" size="40" />
		</p>
		<label class="test-mail-server"><?php echo search($lpublic, "test_smtp_server");?></label>
		<p class="test-mail-server">
			<?php echo search($lpublic, "send_test_email_to");?> 
			<input name="email" type="text" size="40" />
			<input name="testSmtp" type="button" value="<?php echo search($lpublic, "send");?>" class="button" />
		</p>
	</fieldset>
	<fieldset class="settings-fieldset" style="display:none">
		<legend><?php echo search($lpublic, "remote").search($lpublic, "adoption");?> (L3 <?php echo search($lpublic, "adoption");?>)</legend>
		<label><?php echo search($lpublic, "remote").search($lpublic, "adoption");?></label>
		<p>
			<input name="enableRemoteAdoption" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "make_ap_to_remote").getSoftwareName();?></span>
		</p>
		<label></label>
		<p><?php echo search($lpublic, "remote").getSoftwareName();?> URL: <input type="text" size="50" value="http://<?php echo getSoftwareShortName();?>:8080/inform" /></p>
	</fieldset>

	<input name="apply" type="button" value="<?php echo search($lpublic, "apply");?>" class="button" />
</form>

