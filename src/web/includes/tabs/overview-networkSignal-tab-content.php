    <div class="selAFi2" id="en-sel-afi">
                <span><?php echo search($lpublic, "select_network_signal");?></span>
                <div class="select-AFi  select-style" id="en-select-AFI">
                    <select>
                    </select>
                </div>
            </div>
            <div class="businessData">
                 <div class="total-indicator-pic"> </div>
                <div class="data data3"> 
                	<span class="data-title"><?php echo search($lpublic, "data_screening");?></span>
                    <p>
                    	<span><?php echo search($lpublic, "num_users");?>:</span>
                        <a class="moreinfo" title="" id="wireless_nw1-info"><span id="wireless_nw1">0</span></a>
                        <span id="wireless_nw1-1"></span>
                     </p>
                    <p>
                    	<span><?php echo search($lpublic, "uplink_total_flow");?>(<?php echo search($lpublic, "byte");?>):</span>
                        <a class="moreinfo" title="" id="wireless_nw2-info"><span id="wireless_nw2">0</span></a><span id="wireless_nw2_unit">Bytes</span>
                        <span id="wireless_nw2-1"></span>
                    <br />
                    	<span><?php echo search($lpublic, "uplink_total_flow");?>(<?php echo search($lpublic, "packet");?>):</span>
                        <a class="moreinfo" title="" id="wireless_nw3-info"><span id="wireless_nw3">0</span></a>
                        <span id="wireless_nw3-1"></span>
                    </p>
                    <p>
                    	<span><?php echo search($lpublic, "downlink_total_flow");?>(<?php echo search($lpublic, "byte");?>):</span>
                        <a class="moreinfo" title="" id="wireless_nw4-info"><span id="wireless_nw4">0</span></a><span id="wireless_nw4_unit">Bytes</span>
                        <span id="wireless_nw4-1"></span>
                    <br />    
                    	<span><?php echo search($lpublic, "downlink_total_flow");?>(<?php echo search($lpublic, "packet");?>):</span>
                        <a class="moreinfo" title="" id="wireless_nw5-info"><span id="wireless_nw5">0</span></a>
                        <span id="wireless_nw5-1"></span>
                    </p>
                </div>
                <a id="viewHistory3" class="view-history view-history3" href="javascript:void(0)"><?php echo search($lpublic, "view_history");?></a> </div>
            <div class="singal-data">
            </div>