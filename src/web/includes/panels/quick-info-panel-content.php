<dl class="quick-info-details ui-corner-bottom">
  <dt class="access-points-details"><?php echo search($lpublic, "access_points");?>:</dt>

  <dd>
    <dl class="access-points-information">

      <dt><?php echo search($lpublic, "connected");?>:</dt>
      <dd id="ConnectedAP" class="connection-details first">
	<p class="ie8_len"><span id="ConnectedAPCount" class="count">0</span><a class='quick-info-link ok' href="#"><?php echo search($lpublic, "_connected");?></a></p>
	<div id="ConnectedAPDetails" class="quick-ap-list ui-corner-bottom">
		<div>
	  <h3><?php echo search($lpublic, "connected_access_points");?></h3>
	  <ul id="ConnectedAPList">
	  </ul>
	  </div>
	</div>
      </dd>

      <dt><?php echo search($lpublic, "disconnected");?>:</dt>
      <dd id="DisconnectedAP" class="connection-details">
	<p id="ie6_a_bg" class="ie8_len"><span id="DisconnectedAPCount" class="count">0</span><a class='quick-info-link error' href="#"><?php echo search($lpublic, "_disconnected");?></a></p>
	<div id="DisconnectedAPDetails" class="quick-ap-list ui-corner-bottom">
	  <h3><?php echo search($lpublic, "disconnected_access_points");?></h3>
	  <ul id="DisconnectedAPList">
	  </ul>
	</div>
      </dd>

      <dt><?php echo search($lpublic, "pending");?>:</dt>
      <dd id="PendingAP" class="connection-details">
	<p class="ie8_len"><span id="PendingAPCount" class="count">0</span><a class='quick-info-link pending' href="#"><?php echo search($lpublic, "_pending");?></a></p>
	<div id="PendingAPDetails" class="quick-ap-list ui-corner-bottom">
	  <h3><?php echo search($lpublic, "pending_access_points");?></h3>
	  <ul id="PendingAPList">
	  </ul>
	</div>
      </dd>

    </dl>
  </dd>

  <dt class="station-details"><?php echo search($lpublic, "stations");?>:</dt>
  <dd>
	<dl class='clients-information'>
      <dt class="users-term"><?php echo search($lpublic, "users");?>:</dt>
      <dd class="connection-details first">
	<p class="ie8_len"><span id="UserCount" class="count">0</span><a class='quick-info-link' href="#"><?php echo search($lpublic, "_users");?></a></p>
      </dd>
      <dt class="guests-term"><?php echo search($lpublic, "guests");?>:</dt>
      <dd class="connection-details">
	<p class="ie8_len"><span id="GuestCount" class="count">0</span><a class='quick-info-link' href="#"><?php echo search($lpublic, "_guests");?></a></p>
      </dd>
		</dl>
	</dd>
</dl>

