<style>
img{border:none;}
#y{background:#f2f2f2;align:center;width:850px;margin:0 auto;}
</style>
<div class="content" id="YPolicyDiv">
	<div id="y">
	<img src="../../media/global/y.jpg" usemap="#Map">
	<map name="Map">
	  <area shape="rect" coords="405,25,620,87" href="#Y-AFi-Policy">
	  <area shape="rect" coords="201,141,413,200" href="#Y-Wifi-Policy">
	  <area shape="rect" coords="406,245,620,306" href="#Y-User-Policy">
	  <area shape="rect" coords="612,141,824,202" href="#Y-Maintain-Policy">
	  <area shape="circle" coords="514,167,72" href="#Y-Plugin-Policy">
	</map>
	</div>
	<div id="Y-AFi-Policy" class="y-panel">
	  <?php include("y/afi-policy.php");?>
	</div>
	<div id="Y-Wifi-Policy" class="y-panel">
	  <?php include("y/wifi-policy.php");?>
	</div>
	<div id="Y-User-Policy" class="y-panel">
	  <?php include("y/user-policy.php");?>
	</div>
	<div id="Y-Maintain-Policy" class="y-panel">
	  <?php include("y/maintain-policy.php");?>
	</div>
	<div id="Y-Plugin-Policy" class="y-panel">
	  <?php include("y/plugin-policy.php");?>
	</div>
</div>

