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
			<label class="map-tool-show">��ͼ:</label >
			<div class="map-tool-show-buttons">
				<input type="checkbox" class="map-tool-checkbox checkbox label" ><label class="checkbox-label" >��ǩ</label>						
				<input type="checkbox" class="map-tool-checkbox checkbox detail" ><label class="checkbox-detail" >����</label>
				<input type="checkbox" class="map-tool-checkbox checkbox coverage" ><label class="checkbox-coverage" >����</label>
				<input type="checkbox" class="map-tool-checkbox checkbox usershow" ><label class="checkbox-usershow" >�û�</label>
				<input type="checkbox" class="map-tool-checkbox checkbox topology" ><label class="checkbox-topology" >����</label>
			</div>
		</div>
	</div>
	<div class="map-mainpanel">
		<div class="map-unplacedlist">
			<div class="map-unplacedhead">�������豸</div>
			<div class="map-unplaceddevices"></div>
		</div>
		<div class="map-unplacedhide">
			<label class="map-unplacedhide">&gt;&gt;</label>
		</div>
		<div class="map-toolbar">
			<div class="map-tool-map">
				<label class="map-tool-map">��ͼ: </label>
				<select></select>
				<input type="button" class="button" value="���õ�ͼ" />
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
					���õ�ͼ������
				</label>
			</div>
			<div class="map-scalesettipscontent first-step">
				<label class="map-scalesettipscontent first-step"> 
				1. �������������϶�����ڵ�ͼ�ϻ���һ���߶�<br/><br/>
				   ���"��һ��"�����߶�����ʾ�ĵ�ͼ�еľ���
				</label>
			</div>
			<div class="map-scalesetbutton first-step">
				<input type="button" class="button next" value="��һ��"/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button cancel" value="ȡ��" />
			</div>
			<div class="map-scalesettipscontent second-step">
				<label class="map-scalesettipscontent">
				2. �����߶�����ʾ�ĵ�ͼ�еľ��� <br/><br/>
				   ���"ȷ��"�����ñ�����<br/><br/>
				   ���������:<input type="text" class="map-distanceinput"/>
				</label>
				<label class="map-scalesettipscontent unit">m</label>
			</div>
			<div class="map-scalesetbutton second-step">
				<input type="button" class="button confirm" value="ȷ��"/>&nbsp;&nbsp;&nbsp;
				<input type="button" class="button back" value="����" />
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
		<div class="map-win map-config-win map-draggable-win" title="���õ�ͼ">
			<div class="map-win-content">
				<div class="map-newmapadd-panel">
					<div class="map-newmapadd-button"></div>
					<label class="label map-mapadd-text">��ӵ�ͼ</label>
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
		<div class="map-win map-add-win map-draggable-win" title="��ӵ�ͼ">
			<div class="map-win-content">
					<iframe src="/map_upload_iframe.php" class="map-upload-iframe"></iframe>
			</div>		
		</div>
		<div class="map-win map-edit-win map-draggable-win" title="�༭��ͼ">
			<div class="map-win-content">
				<p>
					<label>������������:</label>
					<input type="text" class="text map-editmapnametext" />
				</p>
			</div>			
		</div>
		<div class="map-win map-delete-confirm-win map-draggable-win" title="ɾ����ͼ">
			<div class="map-win-content">
				<p>
					<label>ȷ��Ҫɾ���˵�ͼ��?</label>
				</p>
			</div>
		</div>
		<div class="map-win map-common-alert-win map-draggable-win" title="��ʾ��Ϣ">
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-ok-button" value="ȷ��" />
			</div>
			<div class="map-win-content">
				<p>
					<label>ϵͳ��ʾ��Ϣʾ��</label>
				</p>
			</div>
		</div>
		<div class="map-win map-draggable-win map-model-win" title="����ģ��">
			<div class="map-win-title">
				<label class="label map-win-title-label">
					
				</label>
				<div class="close-icon">
				</div>
			</div>	
			<div class="map-win-bottom">
				<input type="button" class="button map-win-confirm-button" value="ȷ��" />
				<input type="button" class="button map-win-cancel-button" value="ȡ��" />
			</div>
		</div>
		<div class="map-win map-draggable-win map-common-confirm-win" title="ȷ�ϴ���">
			<div class="map-win-content">
				<p>
					<label>�˴���������ʾȷ����Ϣ</label>
				</p>
			</div>
		</div>		
	</div>
</div>
</body>
</html>
