<div class="header-nav">
    <ul class="header-nav-ul support-nav-ul" id="en-nav-ul">
        <li id="choose-button" ><a class="navi" href="#problemFeedback"><?php echo search($lpublic, "problem_feedback");?></a></li>
        <li><a class="navi" href="#utility"><?php echo search($lpublic, "utility");?></a></li>
    </ul>
	
	<div id="problemFeedback" class="system-content-div">
		<!-- Start: problemFeedback Tab -->
			<?php include ("includes/tabs/problemFeedback-tab-content.php");?>
		<!-- End: problemFeedback Tab -->
	</div>

	<div id="utility" class="system-content-div">
		<!-- Start: utility Tab -->
			<?php include ("includes/tabs/utility-tab-content.php");?>
		<!-- End: utility Tab -->
	</div>
	
</div>