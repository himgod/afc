<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office">
<head>  
<style>
	v\:*{behavior:url(#default#VML);}
	o\:*{behavior:url(#default#VML);}
</style>

<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Map</title>
  <!--<link rel="stylesheet" href="/lib/css/jquery-ui.css">-->
  <link rel="stylesheet" href="/lib/css/map_style.css">
  <script  type="text/javascript" src="/lib/js/jquery-1.11.1.min.js"></script>
  <script  type="text/javascript" src="/lib/js/jquery-ui.min.js"></script>

  <script  type="text/javascript" src="/lib/js/raphael-min.js"></script>
  <script  type="text/javascript" src="/lib/js/map_action.js"></script>

</head>
<body>
<div id="afc-map" class="map">
	<div class="map-toolbar2">
		<div class="map-tool-show" >
			<label class="map-tool-show">视图:</label >
			<div class="map-tool-show-buttons">
				<input type="checkbox" class="map-tool-checkbox checkbox label" ><label class="checkbox-label" >标签</label>						
				<input type="checkbox" class="map-tool-checkbox checkbox detail" ><label class="checkbox-detail" >详情</label>
				<input type="checkbox" class="map-tool-checkbox checkbox coverage" ><label class="checkbox-coverage" >覆盖</label>
				<input type="checkbox" class="map-tool-checkbox checkbox usershow" ><label class="checkbox-usershow" >用户</label>
				<input type="checkbox" class="map-tool-checkbox checkbox topology" ><label class="checkbox-topology" >拓扑</label>
			</div>
		</div>
	</div>
	<div class="map-mainpanel">
		<div class="map-unplacedlist">
			<div class="map-unplacedhead">待布置设备</div>
			<div class="map-unplaceddevices"></div>
		</div>
		<div class="map-unplacedhide">
			<label class="map-unplacedhide">&gt;&gt;</label>
		</div>
		<div class="map-toolbar">
			<div class="map-tool-map">
				<label class="map-tool-map">地图: </label>
				<select></select>
				<input type="button" class="button" value="配置地图" />
			</div>
		</div>
		<div class="map-zoombar">
			<div class="map-zoomset">
				<label class="map-zoomchange plus" >+</label>
				<div class="map-zoom"></div>
				<label class="map-zoomchange minus" >-</label>
				<label class="map-zoompercent"></label>
			</div>
			<div class="map-setscale">
			</div>
		</div>
		<div class="map-cover cover-for-scaletool">
		</div>
		<div class="canvas scale-canvas">
			
		</div>		
		<div class="map-scaletool">
			<div class="map-scalesettipstitle">
				<label class="map-scalesettipstitle">
					设置地图比例尺
				</label>
			</div>
			<div class="map-scalesettipscontent first-step">
				<label class="map-scalesettipscontent first-step"> 
				1. 按下鼠标左键并拖动鼠标在地图上画出一条线段<br/><br/>
				   点击"下一步"设置线段所表示的地图中的距离
				</label>
			</div>
			<div class="map-scalesetbutton first-step">
				<input type="button" class="button next" value="下一步"/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button cancel" value="取消" />
			</div>
			<div class="map-scalesettipscontent second-step">
				<label class="map-scalesettipscontent">
				2. 输入线段所表示的地图中的距离 <br/><br/>
				   点击"确定"以设置比例尺<br/><br/>
				   请输入距离:<input type="text" class="map-distanceinput"/>
				</label>
				<label class="map-scalesettipscontent unit">m</label>
			</div>
			<div class="map-scalesetbutton second-step">
				<input type="button" class="button confirm" value="确定"/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button back" value="返回" />
			</div>
		</div>
		<!--
			<div class="map-scaleadjust">
			</div>
			<div class="map-ruler">
				<div class="map-rulermarkcontainer">
					<img src="mark5em.bmp" class="map-rulermark" />
				</div>
				<label class="map-rulerscale">1:100</label>
			</div>
		-->
			
		<div class="map-content">
			<div class="map-images">
			</div>
		</div>
		<div class="map-scale">
			<label class="map-scaletextsuit">
				<span class='map-scaletext'>1</span>
				<span class=" map-scaleunit">m</span>
			</label>
		</div>
		<div class="map-win map-config-win map-draggable-win" title="配置地图">
			<div class="map-win-content">
				<div class="map-newmapadd-panel">
					<div class="map-newmapadd-button"></div>
					<label class="label map-mapadd-text">添加地图</label>
				</div>
				<div class="map-mapname-list">
					<div class="map-mapname-item">
						<label class="map-mapnamelabel">example</label>
						<div class="edit-icon"></div>
						<div class="delete-icon"></div>
					</div>
				</div>
			</div>			
		</div>
		<div class="map-win map-add-win map-draggable-win" title="添加地图">
			<div class="map-win-content">
					<iframe src="/map_upload_iframe.php" class="map-upload-iframe"></iframe>
			</div>		
		</div>
		<div class="map-win map-edit-win map-draggable-win" title="编辑地图">
			<div class="map-win-content">
				<p>
					<label>请输入新名称:</label>
					<input type="text" class="text map-editmapnametext" />
				</p>
			</div>			
		</div>
		<div class="map-win map-delete-confirm-win map-draggable-win" title="删除地图">
			<div class="map-win-content">
				<p>
					<label>确认要删除此地图吗?</label>
				</p>
			</div>
		</div>
		<div class="map-win map-common-alert-win map-draggable-win" title="提示信息">
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-ok-button" value="确认" />
			</div>
			<div class="map-win-content">
				<p>
					<label>系统提示信息示例</label>
				</p>
			</div>
		</div>
		<div class="map-win map-draggable-win map-model-win" title="窗口模板">
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-confirm-button" value="确认" />
				<input type="button" class="button map-win-cancel-button" value="取消" />
			</div>
		</div>
		<div class="map-win map-draggable-win map-common-confirm-win" title="确认窗口">
			<div class="map-win-content">
				<p>
					<label>此窗口用来显示确认信息</label>
				</p>
			</div>
		</div>		
	</div>
</div>
</body>
</html>
