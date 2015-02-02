<script src="lib/highcharts/js/highcharts.js" type="text/javascript"></script>
<script src="lib/highcharts/js/highcharts-3d.js" type="text/javascript"></script>
<script src="lib/highcharts/js/modules/exporting.js" type="text/javascript"></script>
<script type="text/javascript" src="lib/highcharts/js/highcharts-more.js"></script>
<script type="text/javascript" src="lib/highcharts/js/modules/data.js"></script>  
<script src="library/js/pie.js" type="text/javascript"></script>
<script src="library/js/selfdefine.chart.js" type="text/javascript"></script>
<script src="library/js/overview.js" type="text/javascript"></script>

<div id="overview" >
    <div id="overview1">
        <div id="Title">
            <div class="title-symbol"></div>
            <span class="title-font" id="en-title-font1"><?php echo search($lpublic, "system_health_score");?></span> 
        </div>
        <div class="scorePicture" id = "scoreOverwiewContainer"></div>
        <div id="view"></div>
        <!-- <button id="test111">test</button> -->
        <a id="system-health-viewHistory" class="view-history" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a>
         <div class="overview-div-big">
        	<div class="overview-div">
            	<div class="overview-div-title">
            		<label>当前活跃用户TOP3：</label>
                </div>
                <div class="overview-user-rank">
                	<a>1、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>112MB</span><br />
                    	<label>发送 :</label><span>56MB</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>2、11:22:33:44:33:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>102MB</span><br />
                    	<label>发送 :</label><span>40MB</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>3、11:22:33:44:66:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>93MB</span><br />
                    	<label>发送 :</label><span>17MB</span>
                	</div>
                </div>                 
            </div>
            <div class="overview-div">
            	<div class="overview-div-title">
            		<label>当前不稳定用户TOP3：</label>
                </div>
                <div class="overview-user-rank">
                	<a>1、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>5分0秒</span><br />
                    	<label>最短在线 :</label><span>48秒</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>2、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>8分10秒</span><br />
                    	<label>最短在线 :</label><span>1分16秒</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>3、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>11分42秒</span><br />
                    	<label>最短在线 :</label><span>2分48秒</span>
                	</div>
                </div>                 
            </div>
        	<div class="overview-div">
            	<div class="overview-div-title">
	            	<label>当前活跃用户TOP3：</label>
    			</div>	
                <div class="overview-user-rank">
                	<a>1、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>112MB</span><br />
                    	<label>发送 :</label><span>56MB</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>2、11:22:33:44:33:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>102MB</span><br />
                    	<label>发送 :</label><span>40MB</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>3、11:22:33:44:66:22</a> 
                	<div class="overview-user-rank-data">
                		<label>接收 :</label><span>93MB</span><br />
                    	<label>发送 :</label><span>17MB</span>
                	</div>
                </div>                 
            </div>
            <div class="overview-div">
            	<div class="overview-div-title">
	            	<label>当前不稳定用户TOP3：</label>
    			</div>
                <div class="overview-user-rank">
                	<a>1、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>5分0秒</span><br />
                    	<label>最短在线 :</label><span>48秒</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>2、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>8分10秒</span><br />
                    	<label>最短在线 :</label><span>1分16秒</span>
                	</div>
                </div>
                 <div class="overview-user-rank">
                	<a>3、11:22:33:44:11:22</a> 
                	<div class="overview-user-rank-data">
                		<label>最长在线 :</label><span>11分42秒</span><br />
                    	<label>最短在线 :</label><span>2分48秒</span>
                	</div>
                </div>                 
            </div>
        </div>
    </div>
    <div id="overlays" href="javascript:void(0)" class="history-page" >
        <div class="history-title"  >
            <label class="history-font"><?php echo search($lpublic, "view_history");?></label>
            <a>
            <span class="history-close close-icon"></span>
            </a> </div>
        <ul class="header-nav-ul time-button-position">
            <li id="choose-button"> <a   href="javascript:void(0)">24<?php echo search($lpublic, "hours");?></a> </li>
            <li > <a  href="javascript:void(0)">30<?php echo search($lpublic, "days");?></a> </li>
        </ul>
        <div id="chart1" style="margin:0 auto"> </div>
    </div>
    <div id="overview2">
        <!-- Start: /overview2Tab -->
			<?php include ("includes/tabs/overview2-tab-content.php");?>
		<!-- End:/overview2 Tab -->
    </div>
    <div id="overview4"></div>
    <div class="overview3" >
        <div id="Title">
            <div class="title-symbol"></div>
            <span class="title-font" id="en-title-font"><?php echo search($lpublic, "wireless_business_overview");?></span> </div>
        <ul class="header-nav-ul business-button-position" id="en-business-nav-ul">
            <li id="choose-button"> <a class="navi" href="#overview4"><?php echo search($lpublic, "according_to_the_access_point");?></a> </li>
            <li > <a  class="navi" href="#overview4"><?php echo search($lpublic, "according_to_the_network_signal");?></a> </li>
        </ul>
        <div id="AccessPoint">
        <!-- Start: /overview-acessPoint Tab -->
			<?php include ("includes/tabs/overview-accessPoint-tab-content.php");?>
		<!-- End:/overview-acessPoint Tab -->
        </div>
        <div id="NetworkSignal">
        <!-- Start: /overview-acessPoint Tab -->
			<?php include ("includes/tabs/overview-networkSignal-tab-content.php");?>
		<!-- End:/overview-acessPoint Tab -->
        
   		 </div>
    
 
</div>

<div class="overview-cover"></div>
</div>
