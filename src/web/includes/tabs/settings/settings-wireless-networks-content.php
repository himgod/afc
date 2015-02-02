<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "settings");?>"><?php echo search($lpublic, "settings");?></a></li>
	<li class="list"><a href="#" title="<?php echo search($lpublic, "wireless_networks");?>"><?php echo search($lpublic, "wireless_networks");?></a></li>
	<li class="form" style="display:none"><a href="javascript:void(0);">WLAN</a></li>
</ul>

<form id="ie6_form" class="settings-form settings-list">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "wireless_configurations");?></legend>
		<div id="WlanConfTableContainer-none">
			<p><?php echo search($lpublic, "no_wireless_configurations");?></p>
		</div>
		<div id="WlanConfTableContainer">
		</div>
	</fieldset>
	<input type="button" name="create" value='<?php echo search($lpublic, "create");?>' class="button" />
</form>

<form id="WirelessNetworksForm" class="settings-form" style="display:none">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "wireless_configurations");?></legend>

		<label><?php echo search($lpublic, "name");?> / SSID</label>
		<p>
			<input name="name" type="text" class="text-input" />
			<input name="enabled" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enabled");?></span>
		</p>

		<label><?php echo search($lpublic, "security");?></label>
		<p>
			<input name="securityType" value="open" type="radio" class="radio-input" />
			<span class="label"><?php echo search($lpublic, "open_for_security");?></span>
			<input name="securityType" value="wep" type="radio" class="radio-input" />
			<span class="label">WEP</span>
			<input name="securityType" value="wpapsk" type="radio" class="radio-input" />
			<span class="label">WPA-P</span>
			<input name="securityType" value="wpaeap" type="radio" class="radio-input" id="wpaeap" />
			<span class="label">WPA-E</span>
		</p>

		<label class="security wpapsk"><?php echo search($lpublic, "security_key");?></label>
		<p class="security wpapsk">
			<input name="passphrase" type="text" class="text-input" />
		</p>

		<label class="security wep">WEP<?php echo search($lpublic, "_key");?></label>
		<p class="security wep">
			<input name="wepKey" type="text" class="text-input" />
			<?php echo search($lpublic, "key_index");?>
			<select name="wepIdx">
				<option value="1">1</option>
				<option value="2">2</option>
				<option value="3">3</option>
				<option value="4">4</option>
			</select>
		</p>
		
		<label class="security wpaeap">RADIUS <?php echo search($lpublic, "server");?></label>
		<p class="security wpaeap">
			<?php echo search($lpublic, "ip_address");?>: 
			<input name="radiusIp1" type="text" class="text-input" />
			<?php echo search($lpublic, "port");?>:
			<input name="radiusPort1" type="text" class="text-input" value="1812" size="6" />
		</p>
		<label class="security wpaeap"></label>
		<p class="security wpaeap">
			<?php echo search($lpublic, "password");?>: 
			<input name="radiusSecret1" type="text" class="text-input" />
		</p>

		<label class="isGuest"><?php echo search($lpublic, "guest_policy");?></label>
		<p class="isGuest">
			<input type="checkbox" name="is_guest" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "apply_guest_policies_info");?></span>
		</p>
	</fieldset>

	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "advanced");?></legend>

		<label>VLAN</label>
		<p>
			<input name="vlanEnabled" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "use_vlan_id");?></span>
			<input name="vlan" type="text" class="text-input" size="5" /> (2-4095)
		</p>

		<label><?php echo search($lpublic, "hide2");?> SSID</label>
		<p>
			<input name="hideSSID" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "hide2");?> SSID</span>
		</p>
		
		<label class="WPA">WPA</label>
		<ol class="WPA">
			<li>
				WPA <?php echo search($lpublic, "mode");?>
				<select name="wpaMode">
					<option value="auto"><?php echo search($lpublic, "both");?></option>
					<option value="wpa1"><?php echo search($lpublic, "only1");?>WPA1 <?php echo search($lpublic, "only2");?></option>
					<option value="wpa2"><?php echo search($lpublic, "only1");?>WPA2 <?php echo search($lpublic, "only2");?></option>
				</select>
			</li>
			<li>
				<?php echo search($lpublic, "encryption");?>
				<select name="wpaEnc">
					<option value="auto"><?php echo search($lpublic, "auto");?></option>
					<option value="tkip"><?php echo search($lpublic, "only1");?>TKIP <?php echo search($lpublic, "only2");?></option>
					<option value="ccmp"><?php echo search($lpublic, "only1");?>AES/CCMP <?php echo search($lpublic, "only2");?></option>
				</select>
			</li>
		</ol>

		<label><?php echo search($lpublic, "user_group");?></label>
		<p>
			<select class="usergroups" id="usergroups" name="userGroup">
			</select>
		</p>
	</fieldset>

	<input name="id" type="hidden" />

	<input name="apply" class="button" type="button" value='<?php echo search($lpublic, "create");?>' />

	<input name="cancel" class="button" type="button" value='<?php echo search($lpublic, "cancel");?>' />
</form>


