<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "settings");?>"><?php echo search($lpublic, "settings");?></a></li>
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "admin");?>"><?php echo search($lpublic, "admin_settings");?></a></li>
</ul>

<form id="AdminForm" class="settings-form">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "admin_preferences");?></legend>
		<label><?php echo search($lpublic, "admin_name");?></label>
		<p>
			<input name="_id" type="hidden" autocomplete="false"  />
			<input name="name" type="text" autocomplete="false"  />
			<input name="oldname" type="hidden" autocomplete="false"  />
		</p>
		<label><?php echo search($lpublic, "oldpassword");?></label>
		<p>
			<input name="oldpassword" type="password" autocomplete="false" />
		</p>
		<label><?php echo search($lpublic, "password");?></label>
		<p>
			<input name="password1" type="password" autocomplete="false" />
		</p>
		<label><?php echo search($lpublic, "confirm");?></label>
		<p>
			<input name="password2" type="password" autocomplete="false" />
		</p>
		<label><?php echo search($lpublic, "language");?></label>
		<p>
			<select id="Lang" name="lang">
				<option value="auto"><?php echo search($lpublic, "auto_for_language");?></option>
				<option value="en_US">English</option>
				<option value="zh_CN"><?php echo search($lpublic, "chinese");?></option>
			</select>
		</p>
		<label><?php echo search($lpublic, "email_alert");?></label>		
		<p>
			<input name="alertEnabled" type="checkbox" class="check-box-input" />
			<span><?php echo search($lpublic, "send_alert_to_email");?></span>
			<input name="email" type="text" class="text-input"/>
		</p>
	</fieldset>

  <input name="apply" type="button" value="<?php echo search($lpublic, "apply");?>" class="button" />
</form>

