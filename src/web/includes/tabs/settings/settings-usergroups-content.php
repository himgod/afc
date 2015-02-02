<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="Settings"><?php echo search($lpublic, "settings");?></a></li>
	<li class="list"><a href="javascript:void(0);" title="User Groups"><?php echo search($lpublic, "user_groups");?></a></li>
	<li class="form" style="display:none"><a href="javascript:void(0);"><?php echo search($lpublic, "user_group");?></a></li>
</ul>

<form id="ie6_form3" class="settings-form settings-list">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "user_group_settings");?></legend>
		<div id="UserGroupsTableContainer-none">
			<p><?php echo search($lpublic, "no_user_group_settings");?></p>
		</div>
		<div id="UserGroupsTableContainer">
		</div>
	</fieldset>
	<input type="button" name="create" value="<?php echo search($lpublic, "create");?>" class="button" />
</form>

<form id="UserGroupsForm" class="settings-form" style="display:none">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "user_group_settings");?></legend>

		<label><?php echo search($lpublic, "name");?></label>
		<p>
			<input name="name" type="text" class="text-input" />
		</p>

		<label><?php echo search($lpublic, "bandwidth_limit");?>(<?php echo search($lpublic, "download");?>)</label>
		<p>
			<input name="downRateEnabled" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "limited_to");?></span>
			<input name="downRate" type="text" class="text-input" size="5"/> Kbps
		</p>

		<label><?php echo search($lpublic, "bandwidth_limit");?>(<?php echo search($lpublic, "upload");?>)</label>
		<p>
			<input name="upRateEnabled" type="checkbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "limited_to");?></span>
			<input name="upRate" type="text" class="text-input" size="5"/> Kbps
		</p>

	</fieldset>

	<input name="apply" class="button" type="button" value="<?php echo search($lpublic, "create");?>" />

	<input name="cancel" class="button" type="button" value="<?php echo search($lpublic, "cancel");?>" />
</form>


