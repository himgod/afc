<ul class="bread-crumbs">
	<li><a href="javascript:void(0);" title="<?php echo search($lpublic, 'policy_center');?>"><?php echo search($lpublic, "policy_center");?></a></li>
	<li class="list"><a href="javascript:void(0);" title="用户策略">用户策略</a></li>
	<li class="form" style="display:none"><a href="javascript:void(0);">User</a></li>
</ul>

<form id="user-blist">

<table style="margin-left:30%;margin-top:30px;">

  <tr>
    <td class="td1" style="text-align:left;">自动优化策略：</td>
  </tr>
  
  <tr>
    <td style="position:relative;">    	
    	<div style="position:relative;">
		<div class="dragging-sliders" id="loadbalance-drag" style="width:221px;height:8px;position: absolute;top:0px;left:0px;" >
		  <div class="dragging-gra" style="width:221px;height:8px;position: absolute;top:0px;left:0px; background:url(../../../media/global/gray.png) no-repeat -279px 0;">
		  </div>
		  <div class="dragging-blu" style="width:0px;height:8px;position: absolute;top:0px;left:0px; background:url(../../../media/global/blue.png) no-repeat 0 0;">
		  </div>
		  <div class="dragging-ball" style="position: absolute; width: 21px; height: 22px; cursor:pointer; background:url(../../../media/global/ball.png) no-repeat 0 0; top: -6px; left: 0px; z-index: 101; ">
		  </div>
		  <div class="dragging-comment" style="position: absolute; width:180px; height: 22px; text-align: left; 
		  			top:0px;left:250px;font-size: 15px;">
		  	<label></label>
		  </div>
		</div>
		</div>
    </td>
  </tr>
  
  <tr>
    <td >黑名单：</td>
    <td style="position:relative;">
        <div id="xy4" style="width:59px;height:23px;position: absolute;top:0px;left:0px;">
	        <div id="f4" class="switch off"></div>
	        <div id="debug4" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
	        </div>
        </div>
    </td>
  </tr>
  
</table>   

<table style="margin-left:30%;margin-top:30px;">

  <tr>
    <td class="wuser"><span> autelan </span></td>
    <td><a href="#"><img src="../../../media/global/del.jpg"></a></td>
  </tr>
  
  <tr>
    <td class="wuser"><span> xxxxxx dfgdx gxcfh</span></td>
    <td><a href="#"><img src="../../../media/global/del.jpg"></a></td>
  </tr>
  
  <tr>
    <td colspan="2">
        <span class="no_b">
        	<a id="user-blist-add" href="#" style="position:relative;">
        		<img src="../../../media/global/add.jpg" style="position:absolute;top:0px;left:0px;">
        			<span style="padding-left:20px;font-size:12px;font-family: Helvetica, Arial, sans-serif;">添加</span>
        	</a>
        	…
        </span>
    </td>
  </tr>
  
</table>

</form>

<form id="user-list-for-block" style="display:none">

<table align="center" style="margin-top:0px;width:500px;text-align:center; border-collapse:collapse;">
  <tr>
    <td colspan="2" style="text-align:left;"><h3 style="padding:0px;margin:0px;font-size:14px;">您可能想阻止这些用户：</h3></td>
  </tr>
  <tr class="th"><td style="border-right:1px solid #ccc;">名称/MAC地址</td><td>是否阻止</td></tr>
  <tr class="t1">
    <td style="position:relative;"><img src="../../../media/global/auser.png" style="position:absolute;left:70px;top:5px;"><span>autelan/00:1f:64:ea:02:c5</span></td>
    <td style="position:relative;">
        <div id="xyu1" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
        <div id="fu1" class="switch off"></div>
        <div id="debugu1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
        </div>
        </div>
    </td>
  </tr>
    <tr class="t2">
    <td style="position:relative;"><img src="../../../media/global/auser.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
        <div id="xyu2" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
        <div id="fu1" class="switch off"></div>
        <div id="debugu1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
        </div>
        </div>
    </td>
  </tr>
    <tr class="t1">
    <td style="position:relative;"><img src="../../../media/global/auser.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
        <div id="xyu3" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
        <div id="fu3" class="switch off"></div>
        <div id="debugu3" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
        </div>
        </div>
    </td>
  </tr>
    <tr class="t2">
    <td style="position:relative;"><img src="../../../media/global/auser.png" style="position:absolute;left:70px;top:5px;"><span>xxxxxxxxxxx</span></td>
    <td style="position:relative;">
		<div id="xyu3" style="width:59px;height:23px;position: absolute;top:0px;left:20px;">
		<div id="fu3" class="switch off"></div>
		<div id="debug1" style="width:59px;height:23px;position: absolute;top:20px;left:0px;">
		</div>
		</div>
    </td>
  </tr>
  <tr height="40px;"><td colspan="2" align="center"><input type="button" style="border:none;cursor:pointer;width:60px;height:21px;background:url(../../../media/global/a_btn.jpg) no-repeat;"></td></tr>
</table>   

</form>
