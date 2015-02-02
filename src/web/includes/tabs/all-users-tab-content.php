<h2><?php echo search($lpublic, "all_wireless_clients");?></h2>
<div class="controlBar ui-corner-all">
	<form>
    <fieldset>
      <ol>
        <li class="control">
          <label><?php echo search($lpublic, "search");?></label>
          <input id="AllUsersDataTableSearch" class="text search" type="text" />
        </li>
        <li class="control">
					<ul class="choose-guest-user buttonbar thin">
						<li><a href="#blocked"><?php echo search($lpublic, "blocked");?></a></li>
						<li><a href="#noted"><?php echo search($lpublic, "noted");?></a></li>
						<li><a href="#user"><?php echo search($lpublic, "user");?></a></li>
						<li><a href="#guest"><?php echo search($lpublic, "guest");?></a></li>
						<li class="ui-tabs-selected"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
        </li>
        <li class="control offline-filter">
					<ul class="choose-offline buttonbar thin">
						<li><a href="#offline"><?php echo search($lpublic, "offline_only");?></a></li>
						<li class="ui-tabs-selected"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
        </li>
				<li class="control time-filter">
					<label><?php echo search($lpublic, "last_seen");?></label>
					<select class="time-filter">
						<option value="24">1 <?php echo search($lpublic, "day");?></option>
						<option value="72">3 <?php echo search($lpublic, "days");?></option>
						<option value="144" selected="selected">7 <?php echo search($lpublic, "days");?></option>
						<option value="288">2 <?php echo search($lpublic, "weeks");?></option>
						<option value="720">1 <?php echo search($lpublic, "month");?></option>
						<option value="1400">2 <?php echo search($lpublic, "months");?></option>
						<option value="8760">1 <?php echo search($lpublic, "year");?></option>
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
<div id="AllUsersDataTableContainer-none" class="empty-message-container content ui-corner-all">
	<p><?php echo search($lpublic, "no_client_found");?></p>
</div>
<div id="AllUsersDataTableContainer">
</div>


