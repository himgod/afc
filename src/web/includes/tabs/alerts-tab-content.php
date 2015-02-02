<div class="controlBar ui-corner-all">
  <form>
    <fieldset>
			<ol>
				<li class="control">
					<label><?php echo search($lpublic, "search");?></label>
					<input id="AlertsDataTableSearch" class="text search" type="text" />
				</li>
				<li class="control">
					<ul class="choose-shown buttonbar thin">
						<li class="ui-tabs-selected"><a href="#unarchived"><?php echo search($lpublic, "unarchived");?></a></li>
						<li><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
				</li>
				<li class="control">
					<input type="button" class="archive-all button" name="archive-all" value="<?php echo search($lpublic, "archive_all");?>" />
				</li>
				<li class="control" style="display:none">
					<label><?php echo search($lpublic, "filter_by_ap");?></label>
					<select class="map-filter">
					</select>
				</li>
			</ol>
    </fieldset>
  </form>
</div>
<div id="AlertsDataTableContainer-none" class="empty-message-container content ui-corner-top">
	<p><?php echo search($lpublic, "no_alerts_info");?></p>
</div>
<div id="AlertsDataTableContainer" style="display: none;">
</div>

