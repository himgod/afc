<h2><?php echo search($lpublic, "active_wireless_users");?></h2>
<div class="controlBar ui-corner-all">
	<form>
    <fieldset>
      <ol>
        <li class="control">
          <label><?php echo search($lpublic, "search");?></label>
          <input id="UsersDataTableSearch" class="text search" type="text" />
        </li>
        <li class="control">
					<ul class="choose-na-ng buttonbar thin" id="ie6_tab">
						<li><a href="#ng">2G</a></li>
						<li><a href="#na">5G</a></li>
						<li class="ui-tabs-selected"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
					</ul>
        </li>
        <li class="control">
					<label><?php echo search($lpublic, "filter_by_ap");?></label>
					<select id="UsersDataTableFilterByAP" class="ap-filter">
					</select>
        </li>
				<li class="pagesize control right">
					<label><?php echo search($lpublic, "page_size");?></label>
					<select class="table-pagesizer"></select>
				</li>
      </ol>
    </fieldset>
	</form>
</div>
<div id="UsersDataTableContainer-none" class="empty-message-container content ui-corner-all">
	<p><?php echo search($lpublic, "no_active_wireless_users");?></p>
</div>
<div id="UsersDataTableContainer">
</div>


