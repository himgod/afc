
<div id="Title">
    <div class="title-symbol"></div>
    <span class="title-font" id="en-title-font"><?php echo search($lpublic, "system_important_indicators");?></span> </div>
<div class="indicators">
    <div class="indicator indicator-line indicator-line2">
        <div class="indicator-pic pic1" id="en-indicator-pic1"> </div>
        <div class="data data1">
            <p>
            	<span><?php echo search($lpublic, "snr");?>:</span>
                <a class="moreinfo" title="" id="signal1-info"><span id="signal1">0</span></a>dBm
                <span id="signal1-1"></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "channel_utilization");?>(2G):</span>
                <a class="moreinfo" title="" id="signal2-info"><span id="signal2">0</span></a>%
                <span id="signal2-1"></span><br /> 
            	<span><?php echo search($lpublic, "channel_utilization");?>(5G):</span>
                <a class="moreinfo" title="" id="signal3-info"><span id="signal3">0</span></a>%
                <span id="signal3-1"></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "the_interference_signal_intensity");?>:</span>
                <a class="moreinfo" title="" id="signal4-info"><span id="signal4">0</span></a>dBm
                <span id="signal4-1"></span>
            </p>
        </div>
        <a id="viewHistory2-1" class="view-history2" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a> </div>
    <div class="indicator indicator-line2">
        <div class="indicator-pic pic2" id="en-indicator-pic2">  </div>
        <div class="data data2">
            <p>
           		<span><?php echo search($lpublic, "signal_strength");?>:</span >
                <a class="moreinfo" title="" id="terminal1-info"><span id="terminal1">0</span></a>db
                <span id="terminal1-1"></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "access_rate");?>(<?php echo search($lpublic, "receive");?>):</span>
                <a class="moreinfo" title="" id="terminal2-info"><span  id="terminal2">0</span></a>Mbps
                <span id="terminal2-1"></span>
            	 <br />
             	<span><?php echo search($lpublic, "access_rate");?>(<?php echo search($lpublic, "sent");?>):</span>
             	<a class="moreinfo" title="" id="terminal3-info"><span id="terminal3">0</span></a>Mbps
                <span id="terminal3-1"></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "wireless_retransmission_rate");?>(<?php echo search($lpublic, "receive");?>):</span>
                <a class="moreinfo" title="" id="terminal4-info"><span id="terminal4"></span></a>%
                <span id="terminal4-1"></span>
           		<br />
            	<span><?php echo search($lpublic, "wireless_retransmission_rate");?>(<?php echo search($lpublic, "sent");?>):</span>
            	<a class="moreinfo" title="" id="terminal5-info"><span id="terminal5">0</span></a>%
                <span id="terminal5-1"></span>
                
            </p>
        </div>
        <a id="viewHistory2-2" class="view-history2" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a> </div>
    <div class="indicator indicator-line indicator3">
        <div class="indicator-pic pic3" id="en-indicator-pic3"> </div>
        <div class="data data5">
            <p> 
            	<span><?php echo search($lpublic, "downlink_bandwidth");?>:</span>
                <a class="moreinfo" title="" id="ap1-info"><span id="ap1">0</span></a>Mbps
                <span id="ap1-1"></span>
			</p>
            <p>
            	<span><?php echo search($lpublic, "uplink_bandwidth");?>:</span>
            	<a class="moreinfo" title="" id="ap2-info"><span id="ap2">0</span></a>Mbps
                <span id="ap2-1"></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "average_delay");?>:</span>
                <a class="moreinfo" title="" id="ap3-info"><span id="ap3">0</span></a>ms
                <span id="ap3-1"></span>
            </p>
        </div>
        <a id="viewHistory2-3"  class="view-history2" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a> </div>
    <div class="indicator">
        <div class="indicator-pic pic4" id="en-indicator-pic4" >  </div>
        <div class="data data6">
            <p>
            	<span><?php echo search($lpublic, "upload_flow");?>(<?php echo search($lpublic, "byte");?>):</span>
                <a class="moreinfo" title="" id="flow1-info"><span id="flow1" >0</span></a><span id="unitflow_1">Bytes</span>
                <span id="flow1-1" ></span>
            	<br /> 
            	<span><?php echo search($lpublic, "upload_flow");?>(<?php echo search($lpublic, "packet");?>):</span>   
                <a class="moreinfo" title="" id="flow2-info"><span id="flow2" >0</span></a>
                <span id="flow2-1" ></span>
            </p>
            <p>
            	<span><?php echo search($lpublic, "download_flow");?>(<?php echo search($lpublic, "byte");?>):</span>
                <a class="moreinfo" title="" id="flow3-info"><span id="flow3">0</span></a><span id="unitflow_2">Bytes</span>
                <span id="flow3-1" ></span>
				<br />
            	<span><?php echo search($lpublic, "download_flow");?>(<?php echo search($lpublic, "packet");?>):</span> 
                <a class="moreinfo" title="" id="flow4-info"><span id="flow4">0</span></a>
                <span id="flow4-1" ></span> 
                 
           </p>
        </div>
        <a id="viewHistory2-4"  class="view-history2" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a> </div>
</div>
