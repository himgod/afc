<div class="dialog-header add-page-title">
  <a href="javascript:void(0);" class="dialog-hide-button"><span class="close-icon"></span></a>
  
  <h2 class="ap-name"></h2>
</div>
<div class="ap-status ui-corner-all add-page-state">(<?php echo search($lpublic, "disconnected");?>)</div>
<div class="dialog-tabs-div">
<ul class="dialog-tabs header-nav-ul">
  <li class="details-tab"><a href="#details-tab-content"><?php echo search($lpublic, "details");?></a></li>
  <li class="users-tab station-details"><a href="#users-tab-content"><?php echo search($lpublic, "users");?></a></li>
  <li class="guests-tab station-details"><a href="#guests-tab-content"><?php echo search($lpublic, "guests");?></a></li>
  <li class="config-tab ap-configs"><a href="#config-tab-content"><?php echo search($lpublic, "config");?></a></li>
</ul>
</div>
<div class="details-tab-content tab-content ui-corner-bottom ">
	<h3 class="add-page-table-title"><?php echo search($lpublic, "overview1");?></h3>
  <div class="ap-overview-content ap-details-content">
    <dl id="traffic-left">
      <dt class="mac-address"><?php echo search($lpublic, "mac_address");?></dt>
      <dd class="mac-address"></dd>
      <dt class="model"><?php echo search($lpublic, "model");?></dt>
      <dd class="model"></dd>
      <dt class="version"><?php echo search($lpublic, "version");?></dt>
      <dd class="version"></dd>
		</dl>
    <dl id="traffic-left">
      <dt class="ip-address"><?php echo search($lpublic, "ip_address");?></dt>
      <dd class="ip-address"></dd>
      <dt class="uptime"><?php echo search($lpublic, "uptime");?></dt>
      <dd class="uptime"></dd>
    <?php /*?>  <dt class="last-seen"><?php echo search($lpublic, "last_seen");?></dt>
      <dd class="last-seen"></dd><?php */?>
		</dl>
    <dl class="station-details" id="traffic-left">
      <dt class="num-users"><?php echo search($lpublic, "num_users");?></dt>
      <dd class="num-users"></dd>
      <dt class="num-guests"><?php echo search($lpublic, "num_guests");?></dt>
      <dd class="num-guests"></dd>
    </dl>
  </div>
  <h3 class="ap-uplink-details ap-uplink-header add-page-table-title"><?php echo search($lpublic, "traffic");?> </h3>
  <div class="ap-uplink-details ap-uplink-content ap-details-content">
<?php /*?>    <dl class="wireless">
      <dt class="uplink-ap"><?php echo search($lpublic, "uplink_ap");?></dt>
      <dd class="uplink-ap"></dd>
      <dt class="uplink-rssi"><?php echo search($lpublic, "signal");?></dt>
      <dd class="uplink-rssi"></dd>
      <dt class="uplink-tx-rate"><?php echo search($lpublic, "tx_rate");?></dt>
      <dd class="uplink-tx-rate"></dd>
      <dt class="uplink-rx-rate"><?php echo search($lpublic, "rx_rate");?></dt>
      <dd class="uplink-rx-rate"></dd>
		</dl>
		<dl class="wire">
			<dt class="eth-speed"><?php echo search($lpublic, "speed");?></dt>
			<dd class="eth-speed"></dd>
			<dt class="eth-duplex"><?php echo search($lpublic, "duplex");?></dt>
			<dd class="eth-duplex"></dd>
		</dl><?php */?>
		<dl >
      <dt class="uplink-received"><?php echo search($lpublic, "down_pkts_bytes");?></dt>
      <dd class="uplink-received"></dd>
      <dt class="uplink-sent"><?php echo search($lpublic, "up_pkts_bytes");?></dt>
      <dd class="uplink-sent"></dd>
     <?php /*?> <dt class="uplink-bytes-rate"><?php echo search($lpublic, "activity");?></dt>
      <dd class="uplink-bytes-rate"></dd><?php */?>
    </dl>
	</div>
	<?php /*?><h3 class="ap-adv-adopt add-page-table-title"><?php echo search($lpublic, "advanced_adoption");?></h3>
	<div class="content ap-adv-adopt">
		<form action="/" class="ap-adv-adopt" name="ap-adv-adopt">
			<fieldset class="ui-corner-all">
				<label>IP</label>
				<p><input type="text" value="" name="ip" size="12" />:<input type="text" value="22" name="port" size="4" /></p>
				<label><?php echo search($lpublic, "username");?></label>
				<p><input type="text" value="" name="username" /></p>
				<label><?php echo search($lpublic, "password");?></label>
				<p><input type="password" value="" name="password" /></p>
				<label><?php echo search($lpublic, "inform");?> URL</label>
				<p><input type="text" value="" name="url" size="20" /></p>
			</fieldset>
			<input type="submit" value="<?php echo search($lpublic, "adopt");?>" class="button" name="apply" />
		</form>
	</div>
  <h3 class="ap-downlink-details ap-downlink-header add-page-table-title"><?php echo search($lpublic, "downlinks");?></h3>
  <div class="ap-downlink-details ap-downlink-content content">
    <div class="downlink-data-table">
    </div>
  </div><?php */?>
  <h3 class="ap-ng-details ap-ng-header add-page-table-title"><?php echo search($lpublic, "radio");?> (11n/b/g)</h3>
  <div class="ap-ng-details ap-ng-content ap-details-content">
    <dl >
      <dt class="ng-channel"><?php echo search($lpublic, "channel");?></dt>
      <dd class="ng-channel"></dd>
      <dt class="ng-tx-power"><?php echo search($lpublic, "transmit_power");?></dt>
      <dd class="ng-tx-power"></dd>
      <dt class="ng-tx"><?php echo search($lpublic, "tx_pkts_bytes");?></dt>
      <dd class="ng-tx"></dd>
      <dt class="ng-rx"><?php echo search($lpublic, "rx_pkts_bytes");?></dt>
      <dd class="ng-rx"></dd>
      <dt class="ng-tx-error"><?php echo search($lpublic, "tx_retry_dropped");?></dt>
      <dd class="ng-tx-error"></dd>
      <dt class="ng-rx-error"><?php echo search($lpublic, "rx_error_dropped");?></dt>
      <dd class="ng-rx-error"></dd>
    </dl>
    <dl >
      <dt class="ng-num-users"><?php echo search($lpublic, "num_users");?></dt>
      <dd class="ng-num-users"></dd>
      <dt class="ng-num-guests"><?php echo search($lpublic, "num_guests");?></dt>
      <dd class="ng-num-guests"></dd>
    </dl>
  </div>
  <h3 class="ap-na-details ap-na-header add-page-table-title"><?php echo search($lpublic, "radio");?> (11n/a)</h3>
  <div class="ap-na-details ap-na-content ap-details-content">
    <dl>
      <dt class="na-channel"><?php echo search($lpublic, "channel");?></dt>
      <dd class="na-channel"></dd>
      <dt class="na-tx-power"><?php echo search($lpublic, "transmit_power");?></dt>
      <dd class="na-tx-power"></dd>
      <dt class="na-tx"><?php echo search($lpublic, "tx_pkts_bytes");?></dt>
      <dd class="na-tx"></dd>
      <dt class="na-rx"><?php echo search($lpublic, "rx_pkts_bytes");?></dt>
      <dd class="na-rx"></dd>
      <dt class="na-tx-error"><?php echo search($lpublic, "tx_retry_dropped");?></dt>
      <dd class="na-tx-error"></dd>
      <dt class="na-rx-error"><?php echo search($lpublic, "rx_error_dropped");?></dt>
      <dd class="na-rx-error"></dd>
    </dl>
    <dl>
      <dt class="na-num-users"><?php echo search($lpublic, "num_users");?></dt>
      <dd class="na-num-users"></dd>
      <dt class="na-num-guests"><?php echo search($lpublic, "num_guests");?></dt>
      <dd class="na-num-guests"></dd>
    </dl>
  </div>
</div>

<div class="ui-corner-bottom users-tab-content tab-content">
	<div class="users-data-table">
	</div>
</div>

<div class="ui-corner-bottom guests-tab-content tab-content">
	<div class="guests-data-table">
	</div>
</div>

<div class="ui-corner-bottom config-tab-content tab-content ap-configs">
	<h3 class="ap-config add-page-table-title"><?php echo search($lpublic, "general_settings");?></h3>
	<div class="ap-config content ap-config-content">
	<?php /*?>	<p class="ui-corner-all settings-response"><?php echo search($lpublic, "settings_saved");?></p><?php */?>
		<form action="/" id="tform">
			<fieldset class="ui-corner-all ap-basic-configs">
				<?php /*?><legend><?php echo search($lpublic, "general_settings");?></legend><?php */?>
				<label><?php echo search($lpublic, "alias");?></label>
				
					<input type="text" name="alias" class="ap-configs-text"  />
				
			</fieldset>
			
			<?php /*?><fieldset class="ap-poe-configs">
				<legend><?php echo search($lpublic, "power_over_ethernet");?></legend>
				<label><?php echo search($lpublic, "passthrough");?></label>
				<p>
					<select name="poe_passthrough">
						<option value="false"><?php echo search($lpublic, "off");?></option>
						<option value="true"><?php echo search($lpublic, "on");?></option>
					</select>
				</p>
			</fieldset><?php */?>
            <P style="text-align:center">
			<input name="apply" class="button APP-button" type="submit" value="<?php echo search($lpublic, "apply");?>"  />
			</P>
        </form>
	</div>
	
  <h3 class="ap-radio add-page-table-title"><?php echo search($lpublic, "radios");?></h3>
  <div class="ap-radio ap-content ap-config-content">
		<?php /*?><p class="ui-corner-all settings-response"><?php echo search($lpublic, "settings_saved");?></p><?php */?>
		<form action="/">
        	<ul class="header-nav-ul radio-nav-ul1">
            	<li id="choose-button"><a>2.4G</a></li>
                <li><a>5.8G</a></li>
            </ul>
			<fieldset class="ap-ng-configs">		
				<div class="ap-confi-element">
                	<label><?php echo search($lpublic, "channel");?></label>
                	<div class="select-style ap-radio-select">
                        <select name="channel">
                            <option value="0"><?php echo search($lpublic, "auto");?></option>
                        </select>
                    </div>
                    <div class="select-style ap-radio-select">
                        <select name="ht">
                            <option value="20">HT20</option>
                            <option value="40">HT40</option>
                        </select>
                    </div>
				</div>

				
				<div class="tx-power-settings ap-confi-element">
                	<label class="tx-power-settings"><?php echo search($lpublic, "tx_power");?></label>
                	<div class="select-style ap-radio-select">
                        <select name="tx-power-mode">
                            <option value="auto"><?php echo search($lpublic, "auto");?></option>
                            <option value="high"><?php echo search($lpublic, "high");?></option>
                            <option value="medium"><?php echo search($lpublic, "medium");?></option>
                            <option value="low"><?php echo search($lpublic, "low");?></option>
                            <option value="custom"><?php echo search($lpublic, "custom");?></option>
                        </select>
                    </div>
					<span class="tx-power-custom">
					<input class="tx-power-custom" name="tx-power" type="text" size="2" /> dBm
					</span>
				<?php /*?>	<div class="antenna-gain">
						<label class="antenna-gain" id="antenna-gain-label"><?php echo search($lpublic, "antenna_gain");?></label>
						<input class="antenna-gain" name="antenna-gain" type="text" size="2" /> dBi
					</div><?php */?>
                </div>
				<!--<div class="tx-power-custom"><label></label>
					<p class="max-txpower"></p>
				</div>-->
				
				<div class="mode-settings ap-confi-element">
                	<label class="mode-settings"><?php echo search($lpublic, "compatibility");?></label>
                	<div class="select-style ap-radio-select">
                        <select name="radio-mode">
                            <option value="all"><?php echo search($lpublic, "auto");?></option>
                            <option value="b_disabled"><?php echo search($lpublic, "only1");?>11n/g <?php echo search($lpublic, "only2");?></option>
                            <option value="n_only"><?php echo search($lpublic, "only1");?>11n <?php echo search($lpublic, "only2");?></option>
                            <option value="n_disabled"><?php echo search($lpublic, "only1");?>11g <?php echo search($lpublic, "only2");?></option>
                        </select>
                    </div>
				</div>
			</fieldset>
			<fieldset class="ap-na-configs" style="display:none">
				<div class="ap-confi-element">
					<label><?php echo search($lpublic, "channel");?></label>
                    <div class="select-style ap-radio-select">
                	<select name="channel">
						<option value="auto"><?php echo search($lpublic, "auto");?></option>
					</select>
                    </div>
                    <div class="select-style ap-radio-select">
					<select name="ht">
						<option value="20">HT20</option>
						<option value="40">HT40</option>
						<option value="80">HT80</option>
					</select>
                    </div>
				</div>

				
				<div class="tx-power-settings ap-confi-element">
					<label class="tx-power-settings"><?php echo search($lpublic, "tx_power");?></label>
                    <div class="select-style ap-radio-select">
                	<select name="tx-power-mode">
						<option value="auto"><?php echo search($lpublic, "auto");?></option>
						<option value="high"><?php echo search($lpublic, "high");?></option>
						<option value="medium"><?php echo search($lpublic, "medium");?></option>
						<option value="low"><?php echo search($lpublic, "low");?></option>
						<option value="custom"><?php echo search($lpublic, "custom");?></option>
					</select>
                    </div>
					<span class="tx-power-custom">
					<input class="tx-power-custom" name="tx-power" type="text" size="2" /> dBm
					</span>
                    <?php /*?><div class="antenna-gain">
                        <label class="antenna-gain" id="antenna-gain-label"><?php echo search($lpublic, "antenna_gain");?></label>
                        <input class="antenna-gain" name="antenna-gain" type="text" size="2" /> dBi
                    </div><?php */?>
				</div>
				<!--<div class="tx-power-custom"><label></label>
					<p class="max-txpower"></p>
				</div>-->
			
				
				
				<div class="mode-settings ap-confi-element" style="display:none">
					<label class="mode-settings" style="display:none"><?php echo search($lpublic, "compatibility");?></label>
                    <div class="select-style ap-radio-select">
                	<select name="radio-mode">
						<option value="all"><?php echo search($lpublic, "auto");?></option>
						<option value="n_only"><?php echo search($lpublic, "only1");?>11n <?php echo search($lpublic, "only2");?></option>
						<option value="n_disabled"><?php echo search($lpublic, "only1");?>11a <?php echo search($lpublic, "only2");?></option>
					</select>
                   </div>
				</div>
			</fieldset>
            <p style="text-align:center">
			<input name="apply" class="APP-button" type="submit" value="<?php echo search($lpublic, "apply");?>" />
            </p>
		</form>
	</div>
			
	<h3 class="ap-wlan add-page-table-title"><?php echo search($lpublic, "uwlans");?></h3>
	<div class="ap-wlan ap-config-content">
	<?php /*?>	<p class="ui-corner-all settings-response"><?php echo search($lpublic, "settings_saved");?></p><?php */?>
		<form class="settings-list">
		<fieldset class="wlan-data-table wlan-data-table1">
				<div class="wlan-data-table-ng">
				</div>
			</fieldset>
			<fieldset class="wlan-data-table wlan-data-table2" style="display:none">
				<div class="wlan-data-table-na">
				</div>
			</fieldset>
		</form>
		<form class="settings-form" style="display:none">
			<fieldset class="override-detail ui-corner-all">
				<legend><?php echo search($lpublic, "override");?></legend>

				<label><?php echo search($lpublic, "enabled");?></label>
				<p>
					<input name="wlanEnabled" type="checkbox" class="check-box-input" />
					<span class="label"><?php echo search($lpublic, "enabled_on_this_ap");?></span>
				</p>
				
				<label>VLAN</label>
				<p>
					<input name="vlanEnabled" type="checkbox" class="check-box-input" />
					<span class="label"><?php echo search($lpublic, "use_vlan_id");?></span>
					<input name="vlan" type="text" class="text-input" size="5" />
				</p>

				<label>SSID</label>
				<p>
					<input name="name" type="text" class="text-input" />
				</p>
				
				<label class="wpapsk">PSK</label>
				<p class="wpapsk">
					<input name="passphrase" type="text" class="text-input" />
				</p>
				
			</fieldset>
			<input name="apply" class="button" type="button" value="<?php echo search($lpublic, "apply");?>" />
			<input name="restore" class="button" type="button" value="<?php echo search($lpublic, "restore");?>" />
			<input name="cancel" class="button" type="button" value="<?php echo search($lpublic, "cancel");?>" />
		</form>
	</div>
	<h3 class="ap-network add-page-table-title"><?php echo search($lpublic, "network");?></h3>
	<div class="ap-network ap-config-content">
		<?php /*?><p class="ui-corner-all settings-response"><?php echo search($lpublic, "settings_saved");?></p><?php */?>
		<form action="" class="ap-network" name="ap-network">
			<fieldset class="ui-corner-all">								
                <div class="ap-confi-element">
                	<label><?php echo search($lpublic, "configure_ip");?></label>
                    <div class="select-style ap-radio-select">
					<select name="network_config">
						<option value="dhcp"><?php echo search($lpublic, "using_dhcp");?></option>
						<option value="static"><?php echo search($lpublic, "static_ip");?></option>
					</select>
                    </div>
				</div>
				<div class="ip-settings">
                    <div class="ap-confi-element">
                        <label><?php echo search($lpublic, "ip_address");?></label>
                        <input name="address" class="ip-setting" type="text" size="17"/>
                    </div>
                    <div class="ap-confi-element">
                        <label><?php echo search($lpublic, "subnet_mask");?></label>
                        <input name="netmask" class="ip-setting" type="text" size="17"/>
                    </div>
                    <div class="ap-confi-element">
                        <label><?php echo search($lpublic, "gateway");?></label>
                        <input name="gateway" class="ip-setting" type="text" size="17"/>
                    </div>
                    <div class="ap-confi-element">
                        <label><?php echo search($lpublic, "preferred_dns");?></label>
                        <input name="dns1" class="ip-setting" type="text" size="17"/>
                    </div>
                    <div class="ap-confi-element">
                    <label><?php echo search($lpublic, "alternate_dns");?></label>
                    <input name="dns2" class="ip-setting" type="text" size="17"/>
                    </div>
				</div>
			</fieldset>
            <p style="text-align:center">
			<input name="apply" class="APP-button ui-corner-all" type="submit" value="<?php echo search($lpublic, "apply");?>" />
			</p>
        </form>
	</div>
	<?php /*?><h3 class="ap-uplinks-select add-page-table-title"><?php echo search($lpublic, "wireless_uplinks");?></h3>
	<div class="ap-uplinks-select content">
		<fieldset class="ui-corner-all">
			<legend><?php echo search($lpublic, "potential_uplinks");?></legend>
			<div class="uplinks-data-table-none">
				<p><?php echo search($lpublic, "no_wireless_uplinks_available");?></p>
			</div>
			<div class="uplinks-data-table"></div>
		</fieldset>
		<input name="uplink-scan" class="button ui-corner-all" type="button" value="<?php echo search($lpublic, "find_more");?>" /><?php echo search($lpublic, "at");?> 
		<select class="by-map"></select>
	</div><?php */?>
	<h3 class="add-page-table-title"><?php echo search($lpublic, "remove");?></h3>
	<div class="content ap-config-content">
		<p class="forget-this-ap-text">
			<?php echo search($lpublic, "forget_this_ap_info");?><br />
            <?php echo search($lpublic, "forget_this_ap_info1");?>
		</p>
        <p style="text-align:center">
		<input name="remove" class="button APP-button ui-corner-all" type="button" value="<?php echo search($lpublic, "remove");?>" />
		</p>
    </div>
	
</div>

<div class="dialog-controls">
  <button href="javascript:void(0);" class="locate-button button ui-corner-all wifi-qrcode-button wifi-qrcode-button-big "><?php echo search($lpublic, "locate");?></button>
  <button href="javascript:void(0);" class="restart-button button ui-corner-all wifi-qrcode-button wifi-qrcode-button-big wifi-qrcode-button-blue"><?php echo search($lpublic, "restart");?></button>
  <a href="javascript:void(0);" class="adopt-button button ui-corner-all" style="display:none"><?php echo search($lpublic, "adopt");?></a>
  <a href="javascript:void(0);" class="scan-button button ui-corner-all" style="display:none"><?php echo search($lpublic, "scan");?></a>
</div>
