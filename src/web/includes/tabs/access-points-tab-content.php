<h2><?php echo search($lpublic, "managed");?><?php echo search($lpublic, "access_points");?></h2>
<div class="controlBar ui-corner-all">
  <form>
    <fieldset>
      <ol>
        <li class="control">
          <label><?php echo search($lpublic, "search");?></label>
          <input id="AccessPointDataTableSearch" class="text search" type="text" />
        </li>
				<li class="control right">
					<label><?php echo search($lpublic, "page_size");?></label>
					<select class="table-pagesizer"></select>
				</li>
      </ol>
    </fieldset>
  </form>
</div>
<div id="AccessPointDataTableContainer-none" class="empty-message-container content ui-corner-all">
  <p><?php echo search($lpublic, "no_access_point");?></p>
</div>
<div id="AccessPointDataTableContainer"></div>




