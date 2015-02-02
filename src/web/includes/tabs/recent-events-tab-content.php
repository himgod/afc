<div class="controlBar ui-corner-all">
  <form>
    <fieldset>
			<ol>
				<li class="control">
					<label><?php echo search($lpublic, "search");?></label>
					<input id="EventsDataTableSearch" class="text search" type="text" />
				</li>
				<li class="control">
					<ul class="choose-shown buttonbar thin">
						<li><a href="#admin"><?php echo search($lpublic, "admin");?></a></li>
						<li><a href="#ap"><?php echo search($lpublic, "ap");?></a></li>
						<li class="ui-tabs-selected"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
				</li>
				<li class="control">
					<label><?php echo search($lpublic, "within");?></label>
					<select class="time-filter">
						<option value="1" selected="selected">1 <?php echo search($lpublic, "hour");?></option>
						<option value="8">8 <?php echo search($lpublic, "hours");?></option>
						<option value="24">24 <?php echo search($lpublic, "hours");?></option>
						<option value="48">2 <?php echo search($lpublic, "days");?></option>
						<option value="144">7 <?php echo search($lpublic, "days");?></option>
						<option value="288">2 <?php echo search($lpublic, "weeks");?></option>
						<option value="720">1 <?php echo search($lpublic, "month");?></option>
					</select>
					<label><?php echo search($lpublic, "within1");?></label>
				</li>
			</ol>
    </fieldset>
  </form>
</div>
<div id="EventsDataTableContainer-none" class="empty-message-container content ui-corner-top">
	<p><?php echo search($lpublic, "no_events_to_show");?></p>
</div>
<div id="EventsDataTableContainer">
</div>

