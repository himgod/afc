	<div style="display:none" class="system-dialog">
		<div class="modal-header">
			<h1 class="product-logo"><?php echo getSoftwareName();?></h1>
		</div>
		<div class="connection-lost system-dialog-content">
			<h1 class="section-title"><?php echo search($lpublic, "connection_lost");?></h1>
			<p><?php echo search($lpublic, "connection_lost_info");?></p>
			<a href="javascript:void(0);" class="button"><?php echo search($lpublic, "try_again");?></a>
		</div>
		
	</div>
