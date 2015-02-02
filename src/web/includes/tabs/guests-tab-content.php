<h2><?php echo search($lpublic, "active_wireless_guests");?></h2>
<div class="controlBar ui-corner-all">
	<form>
    <fieldset>
      <ol>
        <li class="control">
					<label><?php echo search($lpublic, "search");?></label>
					<input id="GuestsDataTableSearch" class="text search" type="text" />
        </li>
        <li class="control">
					<ul class="choose-na-ng buttonbar thin">
						<li><a href="#ng">2G</a></li>
						<li><a href="#na">5G</a></li>
						<li class="ui-tabs-selected"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
        </li>
        <li class="control">
					<label><?php echo search($lpublic, "filter_by_ap");?></label>
					<select id="GuestsDataTableFilterByAP" class="ap-filter">
					</select>
        </li>
				<li class="control right">
					<label><?php echo search($lpublic, "page_size");?></label>
					<select class="table-pagesizer"></select>
				</li>
      </ol>
		</fieldset>
	</form>
</div>
<div id="GuestsDataTableContainer-none" class="empty-message-container content ui-corner-all">
	<p><?php echo search($lpublic, "no_active_wireless_guests");?></p>
</div>
<div id="GuestsDataTableContainer">
</div>


