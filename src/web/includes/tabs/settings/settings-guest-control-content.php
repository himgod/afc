<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "settings");?>"><?php echo search($lpublic, "settings");?></a></li>
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, "guest_control");?>"><?php echo search($lpublic, "guest_control");?></a></li>
</ul>

<form id="GuestControlForm" class="settings-form">
	<fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "guest_policies");?></legend>
	
		<label><?php echo search($lpublic, "guest_portal");?></label>
		<p>
      <input name="portalEnabled" value="enabled" type="checkbox" class="check-box-input" />
      <span class="label"><?php echo search($lpublic, "enable_guest_portal");?></span>
		</p>
		<label class="auth-type"><?php echo search($lpublic, "authentication");?></label>
		<p class="auth-type">
      <input name="guestAuthentication" value="none" type="radio" class="radio-input" />
      <span class="label"><?php echo search($lpublic, "no_authentication");?></span>
      <input name="guestAuthentication" value="password" type="radio" class="radio-input" />
      <span class="label"><?php echo search($lpublic, "simple_password");?></span>
<?php
/*
			<input name="guestAuthentication" value="hotspot" type="radio" class="radio-input" />
			<span class="label"><?php echo search($lpublic, "hotspot");?></span>
*/
?>
			<input name="guestAuthentication" value="custom" type="radio" class="radio-input" />
			<span class="label"><?php echo search($lpublic, "external_portal_server");?></span>
    </p>
	
		<label class="auth-password"><?php echo search($lpublic, "guest_password");?></label>
		<p class="auth-password">
			<input name="guestPassword" value="" type="text" class="text-input" /><br />
		</p>
		
		<label class="expire"><?php echo search($lpublic, "expiration");?></label>
		<p class="expire">
			<select name="expire">
				<option value="480">8 <?php echo search($lpublic, "hours");?></option>
				<option value="1440">24 <?php echo search($lpublic, "hours");?></option>
				<option value="2880">2 <?php echo search($lpublic, "days");?></option>
				<option value="4320">3 <?php echo search($lpublic, "days");?></option>
				<option value="5760">4 <?php echo search($lpublic, "days");?></option>
				<option value="10080">7 <?php echo search($lpublic, "days");?></option>
				<option value="custom"><?php echo search($lpublic, "user_defined");?></option>
			</select>
			<span class="customExpire">
				<input name="expireNumber" value="" type="text" class="text-input" size="6" />
				<select name="expireUnit">
					<option value="1"><?php echo search($lpublic, "minute");?></option>
					<option value="60"><?php echo search($lpublic, "hour");?></option>
					<option value="1440"><?php echo search($lpublic, "day");?></option>
				</select>
			</span>
		</p>
		
		<label class="redirect"><?php echo search($lpublic, "landing_page");?></label>
		<p class="redirect">
			<input type="radio" name="doRedirect" class="radio-input" value="false" />
			<span class="label"><?php echo search($lpublic, "redirect_to_original");?></span>
		</p>
		<label class="redirect"></label>
		<p class="redirect">
			<input type="radio" name="doRedirect" class="radio-input" value="true" />
			<span class="label"><?php echo search($lpublic, "promotional");?>URL</span>
			<input type="text" name="redirectUrl" size="40" class="text-input" />
		</p>
		
		<label class="custom-ip" style="display:none"><?php echo search($lpublic, "custom_portal");?></label>
		<p class="custom-ip" style="display:none">
			<?php echo search($lpublic, "ip_address");?><input name="customIp" type="text" class="text-input" size="16" />
		</p>
		
		<label class="custom-portal"><?php echo search($lpublic, "potal_customization");?></label>
		<p class="custom-portal">
			<input type="checkbox" name="doCustomPortal" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_portal_customization");?></span>
		</p>
		
		<label class="portal-hostname"><?php echo search($lpublic, "portal_url_hostname");?></label>
		<p class="portal-hostname">
			<input type="checkbox" name="portalUseHostname" class="check-box-input" value="true" />
			<span class="label"><?php echo search($lpublic, "redirect_using_hostname");?></span>
			<input type="text" name="portalHostname" size="25" class="text-input" />
		</p>

	</fieldset>
	
	<fieldset class="settings-fieldset hotspot">
		<legend><?php echo search($lpublic, "hotspot");?></legend>
		<label class="voucher"><?php echo search($lpublic, "voucher");?></label>
		<p class="voucher">
			<input type="checkbox" name="doVoucher" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_voucher_based_authorization");?></span>
		</p>
		<label class="payment"><?php echo search($lpublic, "payment");?></label>
		<p class="payment">
			<input type="checkbox" name="doPayment" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "enable_pament_based_info");?></span>
		</p>
		<label class="paypal"><?php echo search($lpublic, "paypal");?></label>
		<p class="paypal">
			<?php echo search($lpublic, "username");?>: <input type="text" name="paypalUsername" class="paypal" size="48" />
		</p>
		<label class="paypal"></label>
		<p class="paypal">
			<?php echo search($lpublic, "password");?>: <input type="text" name="paypalPassword" class="paypal" size="48" />
		</p>
		<label class="paypal"></label>
		<p class="paypal">
			<?php echo search($lpublic, "signature");?>: <input type="text" name="paypalSignature" class="paypal" size="48" />
		</p>
		<label class="paypal"></label>
		<p class="paypal">
			<input type="checkbox" name="doPaypalUseSandbox" class="check-box-input" />
			<span class="label"><?php echo search($lpublic, "use_paypal_sandbox");?></span> (<a href="http://developer.paypal.com/" target="_blank"><?php echo search($lpublic, "apply_sandbox_account");?></a>)
		</p>
		<label class="admin"><?php echo search($lpublic, "hotspot_operator");?></label>
		<p class="admin">
			<a href="/hotspot" target="_blank"><?php echo search($lpublic, "go_to_hotspot_manager");?></a>
		</p>
		
	</fieldset>
	
  <fieldset class="settings-fieldset">
		<legend><?php echo search($lpublic, "access_control");?></legend>
		<table class="layout">
			<tr><td style="vertical-align:top">
				<label><?php echo search($lpublic, "restricted_subnets");?></label>
				<p class="restricted-subnets dual-col">
				</p>
			</td><td style="vertical-align:top">
				<label><?php echo search($lpublic, "allowed_subnets");?></label>
				<p class="allowed-subnets dual-col">
				</p>
			</td></tr>
		</table>
	</fieldset>

  <input name="apply" type="button" value="<?php echo search($lpublic, "apply");?>" class="button" />
</form>


