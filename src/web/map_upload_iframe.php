<?php
	include("init_lang.php");
	include("publichead.php");
	include("message.php"); 
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<style>
.map-win-content-block
{
	position:absolute;
	left:10px;
	top:5px;
}
input.map-newmapnameinput
{	
	position:absolute;
	width:135px;
	height: 25px;
	left: 86px;
	border: 1px solid #EBEBEB;
	font-family: "微软雅黑";
	border-radius: 5px;
	margin-left: 5px;
	padding-left: 5px;
	top: 45px;
}
.map-upload-container
{
	position:absolute;
	width:210px;
	height:25px;
	top:80px;
	left:86px;
}
.map-upload
{
	position:absolute;
	left:0px;
	top:0px;
	height:100%;
	width:100%;
}
.map-upload .map-upload-text
{
	position:absolute;
	left:0px;
	width:135px;
	top:0px;
	height: 25px;
	border: 1px solid #EBEBEB;
	font-family: "微软雅黑";
	border-radius: 5px;
	margin-left: 5px;
	padding-left: 5px;
}
.map-upload .map-upload-button
{
	position:absolute;
	left:150px;
	width:75px;
	height:25px;
	top:0px;
	border-radius: 5px;
	text-align: center;
	color: #666;
	border: 1px solid #D0D0D0;
	background-color: #FFF;
}
.file
{	
  	filter:alpha(opacity=0);
  	-moz-opacity:0; 
  	opacity:0;
  	z-index:7;
}
label
{
	font-size: 12px;
}
.main-content
{
	position:relative;
	top:10px;
	left:0px;
	width:100%;
	height:auto;
}
.error-info
{
	position:relative;
	top:0px;
	left:0px;
	width:100%;
	height:20px;
	display:none;
	color:red;
}
body
{
	margin-top:0px;
	margin-bottom:0px;		
	border:none;
	font-family: "微软雅黑";
	font-size: 12px;
}
</style>
<script  type="text/javascript" src="/lib/js/jquery-1.11.1.min.js"></script>
<script  type="text/javascript" src="/lib/js/jquery-ui.min.js"></script>
<script type="text/javascript">
$(
	function(){
		var mappath = $("input.map-upload");
		mappath.change(function(){$("input.map-upload-text").val(mappath.val());});
		var test = $("iframe", parent.document);
<?php
	if(isset($_REQUEST["reload"])){
?>
		
		var add_win = $(".map-add-win", parent.document);
		var config_win = $(".map-config-win", parent.document);
		var map_image = $(".map .map-images", parent.document);
		map_image.attr("reloaded", true);
		config_win.show();
		add_win.hide();
		
<?php
	}
	else if(isset($_REQUEST["err_msg"]))
	{
?>
		$(".error-info").html('<?php echo $_REQUEST["err_msg"]; ?>');
		$(".error-info").show();
<?php
	}
?>
		$("div.map-upload, .map-upload-text, .map-upload-button").focus(function(){
			$("input.map-upload").focus();
			$("input.map-upload").click();
		});
	}
);
</script>
</head>
<body>
<div class="map-win-content-block">
<div class="error-info" style=""></div>
<div class="main-content">
	<form id="map-upload-form" action="api/map_upload.php"  method="post"
enctype="multipart/form-data">
		<label><?php echo search($lpublic, "map_mapuploadtips");?>(*.jpg,*.png,*.gif,*.bmp)</label><br/><br/>
		<label><?php echo search($lpublic, "map_inputmapname");?>:</label>
		<input type="text" name="map-name" id="map-name" class="text map-newmapnameinput" /><br/><br/>
		<label><?php echo search($lpublic, "map_selectmapfile");?>:</label>
		<div class="map-upload-container">
			<div class="map-upload">
				<input type="text" disabled="disabled" class="text map-upload-text" />
				<input type="button" class="button map-upload-button" value='<?php echo search($lpublic, "browse");?>...'/>
			</div>
			<input type="file" name="file" id="file" class="file map-upload" />
			<input type="hidden" name="initscaleunit" id="initscaleunit" value="m" />
			<input type="hidden" name="initscalemeter" id="initscalemeter" value="1" />
			<input type="hidden" name="initscalewidth" id="initscalewidth" value="38" />
		</div>
	</form>
	</div>
</div>
</body>
</html>