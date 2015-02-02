<!doctype html>
<html>
<head>
<meta charset="gb2312">
<title>autelan</title>
<link rel="stylesheet" type="text/css" href="library/css/welcome.css">
<script type="text/javascript" src="/library/js/AFC-lib.1.0.0.js"></script>
</head>

<body>
<div id="container">
  <div id="head">
    <img src="media/global/logo.png">
    <div class="fresh">
    <a href="#">刷新</a>
    <select>
      <option>手动</option>
      <option>每5秒</option>
      <option>每15秒</option>
      <option>每30秒</option>
      <option>每1分</option>
      <option selected="selected">每2分</option>
      <option>每5分</option>
    </select>
    </div>
    <div class="clear"></div>
    <div id="drag">
    <span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>
    </div>
  </div>

  <div id="content">
    <div id="ybg">
      <h1>98</h1>
      <input type="button" class="btn1">
      <a onclick="top.location.href='index.php';"><input type="button" class="btn2"></a>
      <p>
      <ul class="star" style="position:relative;" >
        <li style="position:relative;" >信号强度：
        	<div style="position:absolute; top:0px;left:80px;width:401px;height:72px;">
	        	<div id="signal_score1" class="start_score">
	        		<div class="start_mousemove"></div>
	        		<div class="start_score_set"></div>
	        	</div>
        	</div>
        </li>
        <li style="position:relative;" >设备状况：
        	<div style="position:absolute; top:0px;left:80px;width:401px;height:72px;">
	        	<div id="signal_score2" class="start_score">
	        		<div class="start_mousemove"></div>
	        		<div class="start_score_set"></div>
	        	</div>
        	</div></li>
        <li style="position:relative;" >网络速度：
        	<div style="position:absolute; top:0px;left:80px;width:401px;height:72px;">
	        	<div id="signal_score3" class="start_score">
	        		<div class="start_mousemove"></div>
	        		<div class="start_score_set"></div>
	        	</div>
        	</div></li>
        <li><a href="#">更多指标</a>&gt;&gt;<span style="color:#fe971f;">再和我多聊X天就有新指标了！</span></li>
      </ul>
      </p>
    </div>
  </div>
  
  <div id="footer">
    <p class="hdsum"><span><input type="button" id="to_his" style="margin-right:380px;" class="butom-iput" value="历史评分"></span><span><input type="button" id="to_spe" value="有话要说" class="butom-iput"></span></p>
    
    <div class="box-mask"></div>
    <div class="box">
    
       <p class="zfbotm">
          <a onclick="top.location.href='index.php';"><input type="button" value="进入主页" class="btnSure btnpub"/></a>
          <input type="button" value="关闭"class="btnCancel btnpub"/>
       </p>
       <iframe id="spe_frame" name="link" src="spe.php" frameborder="0" width="852px" height="550px">
       </iframe>
       <iframe id="his_frame" name="link" src="his.php" frameborder="0" width="852px" height="577px">
       </iframe>
    </div>
  </div>
</div>
<script type="text/javascript">
	 $(document).ready(function() {
    var isOpen = 0;
    $("#to_his").click(function() {
        //$(".box-mask").css({"display":"block"});
        $("#his_frame").show();
        $("#spe_frame").hide();
        $(".box-mask").fadeIn(500);
        center($(".box"));
        checkEvent($(this).parent(), $(".btnSure"), $(".btnCancel"));
    });
    $("#to_spe").click(function() {
        //$(".box-mask").css({"display":"block"});
        $("#spe_frame").show();
        $("#his_frame").hide();
        $(".box-mask").fadeIn(500);
        center($(".box"));
        checkEvent($(this).parent(), $(".btnSure"), $(".btnCancel"));
    });
    /*$(".butom-iput").click(function() {
        //$(".box-mask").css({"display":"block"});
        $(".box-mask").fadeIn(500);
        center($(".box"));
        checkEvent($(this).parent(), $(".btnSure"), $(".btnCancel"));
    });*/
    var ss_s = $(".start_score");
    ss_s.each(function(){
    	var ss = $(this);
	    ss.move = false;
	    ss.mousemove(
	    	function(ev)
	    	{
	    		var w = ss.width();
	    		var x = ev.offsetX;
	    		if(x > w){return false;}
	    		var i = (Math.floor((5*x)/w) + 1);
	    		var move = ss.find(".start_mousemove");
	    		var set = ss.find(".start_score_set");
	    		set.hide();
	    		
	    		move.width(Math.round(i*0.2*w));
	    		if(!ss.move)
	    		{
	    			move.show();
	    			ss.move = true;
	    		}
	    	}
	    );
	    ss.mouseleave(
	    	function()
	    	{
	    		var move = ss.find(".start_mousemove");
	    		var set = ss.find(".start_score_set");
	    		set.show();
	    		move.hide();
	    		ss.move = false;
	    	}
	    );
	    ss.click(
	    	function(ev)
	    	{
	    		var w = ss.width();
	    		var x = ev.offsetX;
	    		if(x > w){return false;}
	    		var i = (Math.floor((5*x)/w) + 1);
	    		ss.set_score(i);
	    	}
	    );
	    ss.set_score = 
	    	function(score)
	    	{
	    		var w = ss.width();
	    		var set = ss.find(".start_score_set");
	    		set.css("width", Math.round(score*0.2*w));
	    		//set.show();
	    		
	    	};
    });
    function center(obj) {
        var screenWidth = $(window).width(), screenHeigth = $(window).height();
        var scollTop = $(document).scrollTop();
        var objLeft = (screenWidth - obj.width()) / 2;
        var objTop = (screenHeigth - obj.height()) / 2 + scollTop;
        obj.css({
            left:objLeft + "px",
            top:objTop + "px"
        });
        obj.fadeIn(500);
        isOpen = 1;
        $(window).resize(function() {
            if (isOpen == 1) {
                screenWidth = $(window).width();
                screenHeigth = $(window).height();
                var scollTop = $(document).scrollTop();
                objLeft = (screenWidth - obj.width()) / 2;
                var objTop = (screenHeigth - obj.height()) / 2 + scollTop;
                obj.css({
                    left:objLeft + "px",
                    top:objTop + "px"
                });
                obj.fadeIn(500);
            }
        });
        $(window).scroll(function() {
            if (isOpen == 1) {
                screenWidth = $(window).width();
                screenHeigth = $(window).height();
                var scollTop = $(document).scrollTop();
                objLeft = (screenWidth - obj.width()) / 2;
                var objTop = (screenHeigth - obj.height()) / 2 + scollTop;
                obj.css({
                    left:objLeft + "px",
                    top:objTop + "px"
                });
                obj.fadeIn(500);
            }
        });
    }
    function checkEvent(obj, obj1, obj2,obj3) {
        obj2.click(function() {
            closed($(".box-mask"), $(".box"));
        });
		
	$(".tck").click(function() {
       closed($(".box-mask"), $(".box"));
    });
    }
    function closed(obj1, obj2) {
        obj1.fadeOut(500);
        obj2.fadeOut(500);
        isOpen = 0;
    }
});
</script>
</body>
</html>

