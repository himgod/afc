<div class="content" id="SettingsNavigationDiv">
	<ul id="SettingsNavigation">
		<li class="system-button settings-button">
			<dl>
				<dt><a href="#SettingsSystem"><?php echo search($lpublic, "system");?></a></dt>
				<dd><?php echo search($lpublic, "system_related_settings");?></dd>
			</dl>
		</li>
		<li class="wireless-networks-button settings-button">
			<dl>
				<dt><a href="#SettingsWirelessNetworksControl"><?php echo search($lpublic, "wireless_networks");?></a></dt>
				<dd><?php echo search($lpublic, "wireless_networks");?></dd>
			</dl>
		</li>
		
		<li class="guest-control-button settings-button">
			<dl>
				<dt><a href="#SettingsGuestControl"><?php echo search($lpublic, "guest_control");?></a></dt>
				<dd><?php echo search($lpublic, "guest_portal_and_policies");?></dd>
			</dl>
		</li>
		
		<li class="blocked-devices-button settings-button">
			<dl>
				<dt><a href="#SettingsBlockedDevices"><?php echo search($lpublic, "blocked_devices");?></a></dt>
				<dd><?php echo search($lpublic, "list_of_blocked");?></dd>
			</dl>
		</li>
		<li class="admins-button settings-button">
			<dl>
				<dt><a href="#SettingsAdmins"><?php echo search($lpublic, "admin_settings");?></a></dt>
				<dd><?php echo search($lpublic, "admin_settings_info");?></dd>
			</dl>
		</li>
		<li class="usergroups-button settings-button">
			<dl>
				<dt><a href="#SettingsUserGroups"><?php echo search($lpublic, "user_groups");?></a></dt>
				<dd><?php echo search($lpublic, "user_group_settings");?></dd>
			</dl>
		</li>
	</ul>
	<div id="SettingsSystem" class="settings-panel">
		<?php include ("settings/settings-system-content.php");?>
	</div>
	<div id="SettingsGuestControl" class="settings-panel">
		<?php include ("settings/settings-guest-control-content.php");?>
	</div>
	<div id="SettingsWirelessNetworksControl" class="settings-panel">
		<?php include ("settings/settings-wireless-networks-content.php");?>
	</div>
	<div id="SettingsBlockedDevices" class="settings-panel">
		<?php include ("settings/settings-blocked-devices-content.php");?>
	</div>
	<div id="SettingsAdmins" class="settings-panel">
		<?php include ("settings/settings-admins-content.php");?>
	</div>
	<div id="SettingsUserGroups" class="settings-panel">
		<?php include ("settings/settings-usergroups-content.php");?>
	</div>
</div>

