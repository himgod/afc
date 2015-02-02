<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, 'policy_center');?>"><?php echo search($lpublic, "policy_center");?></a></li>
	<li class="list"><a href="javascript:void(0);" title="AFi">AFi Policy</a></li>
	<li class="form" style="display:none"><a href="javascript:void(0);">AFi List</a></li>
</ul>
<form id="afi-blist" >
<table align="center">
  <tr>
    <td class="td1">已安装插件：</td>
    <td>
        <div id="btn" >
          <span id="span1" name="span1" > 不下载</span>
          <span id="span2" name="span2" >下载不更新</span>
          <span id="span3" name="span3" >下载并更新</span>
        </div>
    </td>
  </tr>
  <tr>
    <td class="td1">网络自适应：</td>
    <td style="position:relative;">
        <div id="xy1" style="width:59px;height:23px;position: absolute;top:0px;left:0px;">
			<div id="f1" class="switch off"></div>
			<div id="debug1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
        <span class="no_b" style="margin-left:65px;">启用</span>
    </td>
  </tr>
  <tr>
    <td class="td1">接入控制：</td>
    <td style="position:relative;">
        <div id="xy2" style="width:59px;height:23px;position: absolute;top:0px;left:0px;">
			<div id="f2" class="switch off"></div>
			<div id="debug2" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
		</div>
		<span class="no_b" style="margin-left:65px;">手动准入</span>
	</td>

  </tr>
  <tr>
	<td class="td1">黑名单：</td>
	<td style="position:relative;">
		<div id="xy3" style="width:59px;height:23px;position: absolute;top:0px;left:0px;">
			<div id="f3" class="switch off"></div>
			<div id="debug3" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
    </td>

  </tr>
</table>

<table id="user_blist" align="center">
  <tr>
    <td class="wuser"><span> autelan </span></td>
    <td><a class="blist_del" href="#"><img src="../../../media/global/del.jpg"></a></td>
  </tr>
  <tr>
    <td class="wuser"><span> xxxxxx dfgdx gxcfh</span></td>
    <td><a class="blist_del" href="#"><img src="../../../media/global/del.jpg"></a></td>
  </tr>
  <tr>
    <td colspan="2">
        <span class="no_b">
	        <a id="afi-blist-add" href="#" style="position:relative;">
		        <img src="../../../media/global/add.jpg" style="position:absolute;top:0px;left:0px;">
		        <span style="padding-left:20px;font-size:12px;font-family: Helvetica, Arial, sans-serif;">添加</span>
	        </a>
	        …
        </span>
    </td>
  </tr>
</table>
</form>
<form id="afi-for-blist-add" style="display:none;">

<table align="center" style="margin-top:0px;width:500px;text-align:center; border-collapse:collapse;">
  <tr>
    <td colspan="2" style="text-align:left;"><h3 style="padding:0px;margin:0px;font-size:14px;">您可能想阻止这些AFi：</h3></td>
  </tr>
  <tr class="th"><td style="border-right:1px solid #ccc;">名称/MAC地址</td><td>是否阻止</td></tr>
  <tr class="t1">
    <td style="position:relative;"><img src="../../../media/global/ap.png" style="position:absolute;left:70px;top:5px;"><span>autelan/00:1f:64:ea:02:c5</span></td>
    <td style="position:relative;">
        <div id="xya1" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
			<div id="fa1" class="switch off"></div>
			<div id="debuga1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
    </td>
  </tr>
    <tr class="t2">
    <td style="position:relative;"><img src="../../../media/global/ap.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
        <div id="xya2" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
			<div id="fa2" class="switch off"></div>
			<div id="debuga2" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
    </td>
  </tr>
    <tr class="t1">
    <td style="position:relative;"><img src="../../../media/global/ap.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
        <div id="xya3" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
			<div id="fa3" class="switch off"></div>
			<div id="debuga3" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
    </td>
  </tr>
    <tr class="t2">
    <td style="position:relative;"><img src="../../../media/global/ap.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
        <div id="xya3" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
			<div id="fa3" class="switch off"></div>
			<div id="debug1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
			</div>
        </div>
	</td>
  </tr>
  <tr height="40px;"><td colspan="2" align="center"><input type="button" style="border:none;cursor:pointer;width:60px;height:21px;background:url(img/a_btn.jpg) no-repeat;"></td></tr>
</table>   

</form>
