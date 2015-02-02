<h2><?php echo search($lpublic, "statistics_information");?>
	<ul class="navigation-tabs">
	</ul>
</h2>
<table class="ChartsContainer">
	<tbody>
		<tr><td class="col1">
			<div class="bl chart-panel-wrapper">
			<div class="tl chart-panel">
				<h3><?php echo search($lpublic, "clients");?></h3>
				<div class="chart" id="wlan-chart"></div>
			</div>
			</div>
		</td><td class="remaining">
			<div class="bl chart-panel-wrapper">
			<div class="tr chart-panel usage-distribution">
				<h3><?php echo search($lpublic, "current_usage");?> - <?php echo search($lpublic, "top_access_points");?></h3>
				<div class="chart" id="usage-chart"></div>
			</div>
			</div>
		</td></tr>
	</tbody>
</table>
<table class="ChartsContainer">
	<tbody>
		<tr><td class="col1">
			<div class="bl chart-panel-wrapper">
			<div class="bl chart-panel">
				<h3><?php echo search($lpublic, "quick_look");?></h3>
				<div class="chart quick-look">
					<dl>
						<dt><?php echo search($lpublic, "most_active_ap");?></dt>
						<dd class="most-active-ap">
							<p><span class="name"></span></p>
							<p><?php echo search($lpublic, "download");?>: <span class="sent"></span></p>
							<p><?php echo search($lpublic, "upload");?>: <span class="received"></span></p>
						</dd>
					</dl>
					<dl>
						<dt><?php echo search($lpublic, "most_active_client");?></dt>
						<dd class="most-active-sta">
							<p><span class="name"></span></p>
							<p><?php echo search($lpublic, "download");?>: <span class="sent"></span></p>
							<p><?php echo search($lpublic, "upload");?>: <span class="received"></span></p>
						</dd>
					</dl>
					<dl>
						<dt><?php echo search($lpublic, "all_time");?><?php echo search($lpublic, "top_client");?></dt>
						<dd class="top-user">
							<p><span class="name"></span></p>
							<p><?php echo search($lpublic, "connected");?>: <span class="duration"></span></p>
							<p><?php echo search($lpublic, "download");?>: <span class="sent"></span></p>
							<p><?php echo search($lpublic, "upload");?>: <span class="received"></span></p>
						</dd>
					</dl>
				</div>
			</div>
			</div>
		</td><td class="remaining">
			<div class="bl chart-panel-wrapper chart-activity">
			<div class="br chart-panel">
				<ul class="choose-period buttonbar thin" style="float:right">
					<li class="ui-tabs-selected"><a href="#hourly">24<?php echo search($lpublic, "hours");?></a></li>
					<li><a href="#daily">30<?php echo search($lpublic, "days");?></a></li>
				</ul>
				<h3><?php echo search($lpublic, "recent_activities");?></h3>
				<div class="chart" id="time-chart"></div>
			</div>
			</div>
		</td></tr>
	</tbody>
</table>


