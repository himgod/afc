<div id="afc-map" class="map">
	<div class="map-toolbar2">
		<div class="map-tool-show" >
			<div class="map-tool-show-buttons">
				<input type="checkbox" class="map-tool-checkbox checkbox label" /><label class="checkbox-label" ><?php echo search($lpublic, "map_label")?></label>						
				<input type="checkbox" class="map-tool-checkbox checkbox detail" /><label class="checkbox-detail" ><?php echo search($lpublic, "map_detail")?></label>
				<input type="checkbox" class="map-tool-checkbox checkbox coverage" /><label class="checkbox-coverage" ><?php echo search($lpublic, "map_coverage")?></label>
				<input type="checkbox" class="map-tool-checkbox checkbox usershow" /><label class="checkbox-usershow" ><?php echo search($lpublic, "map_user")?></label>
				<input type="checkbox" class="map-tool-checkbox checkbox topology" /><label class="checkbox-topology" ><?php echo search($lpublic, "map_topology")?></label>
			</div>
		</div>
	</div>
    <div class="map-toolbar">
			<div class="map-tool-map">
				<label class="map-tool-map"><?php echo search($lpublic, "map_map")?></label>
                <div class="select-style map-select">
				<select></select>
                </div>
				<input type="button" class="button" value='<?php echo search($lpublic, "map_config")?>' />
			</div>
		</div>
	<div class="map-mainpanel">
		<div class="map-unplacedlist">
			<div class="map-unplacedhead"><?php echo search($lpublic, "map_unplaceddevices")?></div>
			<div class="map-unplaceddevices"></div>
		</div>
		<div class="map-unplacedhide">
			<label class="map-unplacedhide">&gt;</label>
		</div>
		
		<div class="map-zoombar">
			<div class="map-zoomset">
				<label class="map-zoomchange plus" ></label>
				<div class="map-zoom"></div>
				<label class="map-zoomchange minus" ></label>
				<label class="map-zoompercent"></label>
			</div>
			<div class="map-setscale">
			</div>
		</div>
		<div class="map-cover cover-for-scaletool">
		</div>
		<div class="canvas scale-canvas">
			
		</div>		
		<div class="map-scaletool">
			<div class="map-scalesettipstitle">
				<label class="map-scalesettipstitle">
					<?php echo search($lpublic, "map_setmapscale")?>
				</label>
			</div>
			<div class="map-scalesettipscontent first-step">
				<label class="map-scalesettipscontent first-step"> 
				1. <?php echo search($lpublic, "map_setscalestep1"); ?>
				</label>
			</div>
			<div class="map-scalesetbutton first-step">
				<input type="button" class="button next" value='<?php echo search($lpublic, "next"); ?>'/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button cancel" value='<?php echo search($lpublic, "cancel"); ?>' />
			</div>
			<div class="map-scalesettipscontent second-step">
				<label class="map-scalesettipscontent">
				2. <?php echo search($lpublic, "map_setscalestep2"); ?>:<input type="text" class="map-distanceinput"/>
				</label>
				<label class="map-scalesettipscontent unit">m</label>
			</div>
			<div class="map-scalesetbutton second-step">
				<input type="button" class="button confirm" value='<?php echo search($lpublic, "ok"); ?>'/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button back" value='<?php echo search($lpublic, "back"); ?>' />
			</div>
		</div>
		<!--
			<div class="map-scaleadjust">
			</div>
			<div class="map-ruler">
				<div class="map-rulermarkcontainer">
					<img src="mark5em.bmp" class="map-rulermark" />
				</div>
				<label class="map-rulerscale">1:100</label>
			</div>
		-->
			
		<div class="map-content">
			<div class="map-images">
			</div>
		</div>
		<div class="map-scale">
			<label class="map-scaletextsuit">
				<span class='map-scaletext'>1</span>
				<span class=" map-scaleunit">m</span>
			</label>
		</div>
		<div class="map-win map-config-win map-draggable-win" title='<?php echo search($lpublic, "map_configmap"); ?>'>
			<div class="map-win-content">
				<div class="map-newmapadd-panel">
					<div class="map-newmapadd-button"></div>
					<label class="label map-mapadd-text"><?php echo search($lpublic, "add"); ?></label>
				</div>
				<div class="map-mapname-list">
					<div class="map-mapname-item">
						<label class="map-mapnamelabel">example</label>
						<div class="edit-icon"></div>
						<div class="delete-icon"></div>
					</div>
				</div>
			</div>			
		</div>
		<div class="map-win map-add-win map-draggable-win" title='<?php echo search($lpublic, "map_addmap"); ?>'>
			<div class="map-win-content">
					<iframe src="/map_upload_iframe.php" class="map-upload-iframe"></iframe>
			</div>		
		</div>
		<div class="map-win map-edit-win map-draggable-win" title='<?php echo search($lpublic, "map_editmap"); ?>'>
			<div class="map-win-content">
				<p>
					<label><?php echo search($lpublic, "map_inputnewname");?>:</label>
					<input type="text" class="text map-editmapnametext" />
				</p>
			</div>			
		</div>
		<div class="map-win map-delete-confirm-win map-draggable-win" title='<?php echo search($lpublic, "map_deletemap"); ?>'>
			<div class="map-win-content">
				<p>
					<label><?php echo search($lpublic, "map_deleteconfirm"); ?>?</label>
				</p>
			</div>
		</div>
		<div class="map-win map-common-alert-win map-draggable-win" title='<?php echo search($lpublic, "map_alertmessage"); ?>'>
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-ok-button" value='<?php echo search($lpublic, "ok"); ?>' />
			</div>
			<div class="map-win-content">
				<p>
					<label><?php echo search($lpublic, "map_alertmsgexample");?></label>
				</p>
			</div>
		</div>
		<div class="map-win map-draggable-win map-model-win" title='<?php echo search($lpublic, "map_windowtemplet");?>'>
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-confirm-button" value='<?php echo search($lpublic, "ok"); ?>' />
				<input type="button" class="button map-win-cancel-button" value='<?php echo search($lpublic, "cancel"); ?>' />
			</div>
		</div>
		<div class="map-win map-draggable-win map-common-confirm-win" title='<?php echo search($lpublic, "map_confirmwindow"); ?>'>
			<div class="map-win-content">
				<p>
					<label><?php echo search($lpublic, "map_confirmmsgexample");?></label>
				</p>
			</div>
		</div>		
	</div>
       
</div>
	 <div id="overview2" style="display:none;">
        <!-- Start: /overview2Tab -->
			<?php include ("includes/tabs/overview2-tab-content.php");?>
		<!-- End:/overview2 Tab -->
    </div>
