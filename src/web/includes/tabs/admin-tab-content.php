<div class="content ui-corner-top">
  <fieldset class="settings-fieldset">
    <legend><?php echo search($lpublic, "server_information");?></legend>
	<table>
	<tr>
	<td>
	<label><?php echo search($lpublic, "version_server");?></label>
	</td>
	<td>
	http://<input id="version_server" type="text" name="version_server" value="<?php echo isset($_SESSION["version_server"])? $_SESSION["version_server"] : get_afc_version_server_addr();?>" ><br/>
	</td>
	</tr>
	<tr>
	<td>
	<label><?php echo search($lpublic, "version");?></label>
	</td>
	<td>
	<p><span id="ServerVersion"></span>
			<span id="UpgradeVersion" style="display:none"></span>
			<input id="UpgradeButton" type="button" name="upgrade" value="<?php echo search($lpublic, "upgrade");?>" class="button" style="display:none"/>
    </p>
    </td>
    </tr>
    </table>
  </fieldset>

  <fieldset class="settings-fieldset">
    <legend><?php echo search($lpublic, "backup");?></legend>

    <label><?php echo search($lpublic, "download");?></label>
    <p>
      <a href="javascript:void(0);" id="DownloadBackupLink"><?php echo search($lpublic, "download_backup_settings");?></a>
    </p>
  </fieldset>

  <fieldset class="settings-fieldset">
    <legend><?php echo search($lpublic, "restore");?></legend>

    <label><?php echo search($lpublic, "browse");?></label>
    <div>
    <div style="position: relative;" class="text-input">
    <input id="FilePathText" type='text' style='width:140px' value='' disabled='disabled' class='text-input'/>
    <input type='button' id='BackupFileButton' class='button' value='<?php echo search($lpublic, "browse");?>' />
    <input id="BackupFile" name="restoreFile" type="file" class="text-input file"/>
    </div>
	<div id="BackupFile-confirm">
		<div class="confirm-text"> </div>
		<input type="button" name="confirm" value="<?php echo search($lpublic, "confirm");?>" class="button" />
		<input type="button" name="cancel" value="<?php echo search($lpublic, "cancel");?>" class="button" />
    </div>
    </div>
  </fieldset>

  <fieldset class="settings-fieldset">
    <legend><?php echo search($lpublic, "support_info");?></legend>

    <label><?php echo search($lpublic, "download");?></label>
    <p>
      <a href="javascript:void(0);" id="DownloadSupportLink"><?php echo search($lpublic, "download_support_info");?></a>
    </p>
  </fieldset>
</div>


