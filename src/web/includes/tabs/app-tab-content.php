<div class="header-nav">
    <ul class="header-nav-ul app-nal-ul" id="en-nav-ul">
        <li id="choose-button"><a class="navi" href="#installed"><?php echo search($lpublic, "installed");?></a></li>
        <li><a class="navi" href="#toBeDownload"><?php echo search($lpublic, "to_be_download");?></a></li>
    </ul>


<div id="installed" class="system-content-div">
	<!-- Start: installed Tab -->
		<?php include ("includes/tabs/installed-tab-content.php");?>
	<!-- End: installed Tab -->
</div>

<div id="toBeDownload" class="system-content-div">
	<!-- Start: toBeDownload Tab -->
		<?php include ("includes/tabs/toBeDownload-tab-content.php");?>
	<!-- End: toBeDownload Tab -->
</div>
</div>