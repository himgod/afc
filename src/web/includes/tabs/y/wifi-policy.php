<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, 'policy_center');?>"><?php echo search($lpublic, "policy_center");?></a></li>
	<li class="list"><a href="javascript:void(0);" title="��������">��������</a></li>
	<li class="form" style="display:none"><a href="javascript:void(0);">WLAN</a></li>
</ul>
<form id="wifi-list">

<table style="margin-left:30%;margin-top:30px;">
  <tr>
    <td class="wuser"><span>autelan </span></td>
    <td><span style="position:relative;">
    	<img src="../../../media/global/lock.jpg" class="info_img" style="top:12px;left:2px;">
    	<a class="wifi-info">
    		<img src="../../../media/global/info.jpg" class="info_img">
    	</a>
    </span></td>
  </tr>
  <tr>
    <td class="wuser"><span>xxxxxxxxxxxxx</span></td>
    <td><span style="position:relative;">
    	<a class="wifi-info">
    		<img src="../../../media/global/info.jpg" class="info_img">
    	</a>
    </span></td>
  </tr>
  <tr>
    <td colspan="2">
        <span class="no_b">
        	<a id="wifi-add" href="#" style="position:relative;">
        		<img src="../../../media/global/add.jpg" style="position:absolute;top:0px;left:0px;">
        		<span style="padding-left:20px;font-size:12px;font-family: Helvetica, Arial, sans-serif;">���</span>
        	</a>
        		��
        	</span>
    </td>
  </tr>
</table>

<table style="margin-left:30%;margin-top:30px;">
  <tr>
    <td class="td1" style="text-align:left;">�Զ��Ż����ԣ�</td>
  </tr>
  <tr>
    <td style="position:relative;left:0px;">
    	<div class="optimize-preimg"></div>
    	<div style="position:absolute;left:13px;">
			<div class="dragging-sliders" id="optimize-drag">
			  <div class="dragging-gra">
			  </div>
			  <div class="dragging-blu">
			  </div>
			  <div class="dragging-ball">
			  </div>
			  <div class="dragging-comment">
			  	<label></label>
			  </div>
			</div>
		</div>
    	<div class="optimize-sufimg"></div>
    </td>
  </tr>
  <tr>
    <td>
        <span class="no_b" style="font-size:12px;font-family: Helvetica, Arial, sans-serif;">��ʾ��xxxxxxxxxxx</span>
    </td>
  </tr>
</table>

</form>
<form id="wifi-set" style="display:none">
<table style="margin-left:30%;margin-top:30px;">
	<tr>
	    <td class="td1">�Զ�����</td>
	    <td style="position:relative;left:10px;" colspan="2">
	        <div id="xy" style="width:59px;height:23px;position: absolute;top:0px;left:0px;">
	        <div id="f" class="switch off"></div>
	        <div id="debug1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
	        </div>
	        </div>
	    </td>

	</tr>
  	<tr>
	    <td class="td1">����/SSID</td>
	    <td style="padding-left:10px;"><input type="text" value="name"></td>
	    <td style="position:relative;">
	        <span class="no_b" style="padding-left:10px;">����</span>
	        <div id="xy13" style="width:59px;height:23px;position: absolute;top:0px;left:40px;">
	        	<div id="f13" class="switch off"></div>
	        <div id="debug13" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
	        </div>
	        </div>
	    </td>
	</tr>
	<tr>
	    <td class="td1">��ȫ��&nbsp;</td>
	    <td colspan="2"  style="left:10px;top:-5px;position:relative;">
	    	<div class="security-preimg"></div>
	    	<div style="position:absolute;left:18px;">
	              <div class="dragging-sliders" id="security-drag">
		              <div class="dragging-gra">
		              </div>
		              <div class="dragging-blu">
		              </div>
			          <div class="dragging-ball">
		              </div>
		              <div class="dragging-comment">
		              	<label></label>
		              </div>
	              </div>
			</div>
	    	<div class="security-sufimg"></div>
	    </td>
	</tr>
	<tr id="wpapsk_pass" style="display:none">
	    <td class="td1">
			<label class="security wpapsk"><?php echo search($lpublic, "security_key");?></label>
		</td>
		<td colspan=2 class="security_container" style="padding-left:10px;position: relative;">
			<div class="security wpapsk">
				<div class="security-combo" style="position:absolute;left:10px;top:0px;">
					<input name="passphrase" type="password" class="passwd-input" maxlength=64 />
					<input name="passphrase" type="text" class="passwd-input"  maxlength=64 style="display:none;"/>
					<div class="eyes"></div>
					<div id="wpapskPwdTips" class="tips">
						<div id="wpapskPwdStatus" class="pswState pswState-poor">
							<span class="s1">��</span>
							<span class="s2">�е�</span>
							<span class="s3">ǿ</span>
						</div>
						<span id="wpapskTips" class="txt-tips">8~64λ��ASCII���ַ�</span>
					</div>	
				</div>
			</div>    	
		</td>
	</tr>
	<tr id="wpaeap_pass" style="display:none">
		<td class="td1">
			<label class="security wpaeap">RADIUS <?php echo search($lpublic, "server");?></label>
		</td>
		<td style="padding-left:10px;width:200px;">
			<p class="security wpaeap">
				<?php echo search($lpublic, "ip_address");?>: 
				<input name="radiusIp1" type="text" class="text-input" />
			</p>
		</td>
		<td style="padding-left:10px;">
			<p class="security wpaeap">
				<?php echo search($lpublic, "port");?>:
				<input name="radiusPort1" type="text" class="text-input" value="1812" size="6" />
			</p>
		</td>
	</tr>
	<tr  id="wpaeap_pass2" style="display:none">
	   	<td class="td1">
			<label class="security wpaeap"></label>
		</td>
		<td  colspan=2 class="security_container" style="padding-left:10px;position: relative;">
			<div class="security wpaeap">
				&nbsp;&nbsp;&nbsp;<?php echo search($lpublic, "password");?>: 
				<div class="security-combo" style="position:absolute;left:56px;top:0px;">
					<input name="radiusSecret1" type="password" class="passwd-input"  maxlength=64 />
					<input name="radiusSecret1" type="text" class="passwd-input"  maxlength=64 style="display:none"/>
					<div class="eyes"></div>
			
					<div id="wpaeapPwdTips" class="tips">
						<div id="wpaeapPwdStatus" class="pswState pswState-poor">
							<span class="s1">��</span>
							<span class="s2">�е�</span>
							<span class="s3">ǿ</span>
						</div>
						<span class="txt-tips">8~64λ��ASCII���ַ�</span>
					</div>
				</div>
			</div>
		</td>
	</tr>
	<tr>
	    <td class="td1">��������</td>
	    <td style="position:relative;" colspan="2">
	       <span class="no_b" style="padding-left:10px;">����</span>
	        <div id="xy" style="width:59px;height:23px;position: absolute;top:0px;left:40px;">
	        <div id="f" class="switch off"></div>
	        <div id="debug1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
	        </div>
	        </div>
	    </td>
	</tr>
	<tr>
		<td style="position: relative;">
			<input type="button" class="blank-button" value="Ӧ��"/>
		</td>
		<td>
		</td>
		<td style="position: relative;">
			<input type="button" class="blank-button" value="ȡ��"/>
		</td>
	</tr>
</table>
</form>
