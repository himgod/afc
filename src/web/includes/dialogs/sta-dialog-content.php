<div class="dialog-header add-page-title">
  <a href="javascript:void(0);" class="dialog-hide-button"><span class="close-icon"></span></a>
  
  <h2 class="sta-name"></h2>
</div>

<ul class="dialog-tabs header-nav-ul">
  <li class="details-tab"><a href="#details-tab-content"><?php echo search($lpublic, "details");?></a></li>
  <li class="statistics-tab"><a href="#statistics-tab-content"><?php echo search($lpublic, "statistics");?></a></li>
  <li class="history-tab"><a href="#history-tab-content"><?php echo search($lpublic, "history");?></a></li>
  <li class="config-tab"><a href="#config-tab-content"><?php echo search($lpublic, "config");?></a></li>
</ul>

<div class="details-tab-content tab-content ui-corner-bottom">
  <h3 class="add-page-table-title"><?php echo search($lpublic, "overview");?></h3>
  <div class="ap-overview-content content sta-detail-content">
    <dl>
      <dt class="mac-address"><?php echo search($lpublic, "mac_address");?></dt>
      <dd class="mac-address"></dd>
      <dt class="hostname"><?php echo search($lpublic, "hostname");?></dt>
      <dd class="hostname"></dd>
		</dl>
    <dl>
      <dt class="ip-address"><?php echo search($lpublic, "ip_address");?></dt>
      <dd class="ip-address"></dd>
      <dt class="uptime"><?php echo search($lpublic, "uptime");?></dt>
      <dd class="uptime"></dd>
      <dt class="connected-ap"><?php echo search($lpublic, "connected_ap");?></dt>
      <dd class="connected-ap"></dd>
      <dt class="last-seen"><?php echo search($lpublic, "last_seen");?></dt>
      <dd class="last-seen"></dd>
		</dl>
  </div>
</div>

<div class="statistics-tab-content tab-content ui-corner-bottom">
  <h3 class="add-page-table-title"><?php echo search($lpublic, "statistics");?></h3>
  <div class="content sta-detail-content">
    <dl>
      <dt class="essid">ESSID</dt>
      <dd class="essid"></dd>
      <dt class="connected-ap"><?php echo search($lpublic, "connected_ap");?></dt>
      <dd class="connected-ap"></dd>
    </dl>
    <dl>
      <dt class="channel"><?php echo search($lpublic, "channel");?></dt>
      <dd class="channel"></dd>
      <dt class="signal"><?php echo search($lpublic, "signal");?></dt>
      <dd class="signal"></dd>
      <dt class="tx-rate"><?php echo search($lpublic, "tx_rate");?></dt>
      <dd class="tx-rate"></dd>
      <dt class="rx-rate"><?php echo search($lpublic, "rx_rate");?></dt>
      <dd class="rx-rate"></dd>
      <dt class="bytes-rate"><?php echo search($lpublic, "activity");?></dt>
      <dd class="bytes-rate"></dd>
			<dt class="powersave"><?php echo search($lpublic, "power_save");?></dt>
			<dd class="powersave"></dd>
		</dl>
		<dl>
      <dt class="received"><?php echo search($lpublic, "received_pkts_bytes");?></dt>
      <dd class="received"></dd>
      <dt class="sent"><?php echo search($lpublic, "sent_pkts_bytes");?></dt>
      <dd class="sent"></dd>
    </dl>
  </div>
</div>

<div class="history-tab-content tab-content ui-corner-bottom">
	<h3 class="add-page-table-title"><?php echo search($lpublic, "recent_connections");?></h3>
	<div class="history-content content sta-detail-content">
		<div class="history-data-table-empty empty-message-container content ui-corner-all">
			<p><?php echo search($lpublic, "no_prior_connections");?></p>
		</div>
		<div class="history-data-table">
		</div>
	</div>
</div>

<div class="config-tab-content tab-content ui-corner-bottom">
	<h3 class="add-page-table-title"><?php echo search($lpublic, "config");?></h3>
  <div class="content sta-detail-content">
	<?php /*?><p class="ui-corner-all settings-response"><?php echo search($lpublic, "settings_saved");?></p><?php */?>
    <form action="/">
      <fieldset class="ui-corner-all">
     <?php /*?>   <legend><?php echo search($lpublic, "general_settings");?></legend><?php */?>
     	 <div class="ap-confi-element" style="height:auto">
            <label><?php echo search($lpublic, "alias");?></label>
            <input type="text" name="alias" class="ap-configs-text" />
       	 </div>
          <div class="ap-confi-element" style="height:auto">
	        <label><?php echo search($lpublic, "note");?></label>        
          	<textarea name="note" class="sta-configs-textarea"></textarea>
         </div>
          <div class="ap-confi-element">
        	<label><?php echo search($lpublic, "usergroups");?></label>
        	<div class="select-style ap-radio-select3">
              <select class="usergroups" id="usergroups" name="sta-usergroups">
              </select>
          	</div>
        </p>
      </fieldset>
      <p style="text-align:center">
	      <input name="apply" class="button APP-button ui-corner-all" type="submit" value="<?php echo search($lpublic, "apply");?>" />
      </p>
    </form>
  </div>
  <h3 class="add-page-table-title"><?php echo search($lpublic, "debug");?></h3>
  <div class="content sta-detail-content">
    <form action="/">
      <p class="forget-this-ap-text"><?php echo search($lpublic, "sta_dialog_debug_info");?></p>
      <fieldset class="ui-corner-all">
        <legend><?php echo search($lpublic, "fixed_connection");?></legend>
         <div class="ap-confi-element">
            <label><?php echo search($lpublic, "access_point");?></label>
            <div class="select-style ap-radio-select2">
                <select class="fixedAp">                  
                </select>
            </div>
        </div>
      </fieldset>
     <?php /*?> <dl>
        <dt class="oui"><?php echo search($lpublic, "device_type");?></dt>
        <dd class="oui"></dd>
      </dl><?php */?>
    </form>
  </div>
</div>

<div class="dialog-controls">
  <a href="javascript:void(0);" class="button ui-corner-all block-button wifi-qrcode-button wifi-qrcode-button-big"><?php echo search($lpublic, "block");?></a>
  <a href="javascript:void(0);" class="button ui-corner-all reconnect-button wifi-qrcode-button wifi-qrcode-button-big wifi-qrcode-button-blue"><?php echo search($lpublic, "reconnection");?></a>
<?php /*?>  <a href="javascript:void(0);" class="button ui-corner-all unauthorize-button"><?php echo search($lpublic, "unauthorize");?></a><?php */?>
</div>
