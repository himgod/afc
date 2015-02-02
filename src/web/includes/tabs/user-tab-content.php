 <div class="wirelessUser system-div">
    <div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "active_wireless_users");?></label>
    </div>
    <div class="menuBar">
        <div class="search"> 
            <input id="UserDataTableSearch" type="text" placeholder='<?php echo search($lpublic, "search_content");?>'>     
        </div>
        <div class="classify">
            <ul class="choose-na-ng" id="ie6_tab">
            		 <li id="classify-active"><a href="#allUser"><?php echo search($lpublic, "all");?></a></li>
                    <li><a href="#2G">2G</a></li>
                    <li><a href="#5G">5G</a></li>
                   
            </ul>
        </div>
        <div class="menubar-float">
            <div class="page" id="en-page">
                <span><?php echo search($lpublic, "display_each_page");?></span>
                 <div class="select-style page-select"> 
                    <select class="table-pagesizer"></select>
                   <!-- <select>
                        <option value="5" >5</option>
                        <option value="10">10</option>
                        <option value="20">20</option>
                        <option value="30">30</option>
                        <option value="50">50</option>
                        <option value="100">100</option>
                    </select>-->
                </div>
            </div>
            <div class="filter"> <span><?php echo search($lpublic, "filter_by_ap");?></span>
                <div class="select-style filter-select">
                    <select id="UserDataTableFilterByAP" class="ap-filter">
                        
                    </select>
                </div>
            </div>
       </div>
       
    </div>
    
    <div id="UserDataTable-none" class="empty-message-container content ui-corner-all table-container" style="margin-left:15px">
		 <table class="data-table">
     		<thead>
     			<tr>
            	<th></th>
                <th ><?php echo search($lpublic, "name");?>/<?php echo search($lpublic, "mac_address");?></th>
                <th ><?php echo search($lpublic, "ip_address");?></th>
                <th >WLAN</th>
                <th ><?php echo search($lpublic, "client_num");?></th>
                <th ><?php echo search($lpublic, "signal");?></th>
                <th ><?php echo search($lpublic, "download");?></th>
                <th ><?php echo search($lpublic, "upload");?></th>
                <th><?php echo search($lpublic, "uptime");?></th>
                <th><?php echo search($lpublic, "operating");?></th>
     			</tr>
     		</thead>
     	</table>
        <div class="no-data-div">
            <img src="/pic/cry.png"  />
            <span class="no-data"><?php echo search($lpublic, "no_data_info");?></span>
   		</div>
	</div>
    <div id="UserDataTable" class="table-container">
    </div>
    
 
        
</div>
 <div id="UserEvent" class="system-div">
 	<div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "event_notification_and_tips");?></label>
    </div>
    <div class="menuBar">
        <div class="search"> 
            <input id="UserEventDataTableSearch" type="text" placeholder='<?php echo search($lpublic, "search_content");?>'>
        </div>
        <div class="classify">
            <ul class="choose-shown ">
            		<li id="classify-active"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
                    <li><a href="#user"><?php echo search($lpublic, "user");?></a></li>
                    <li><a href="#admin"><?php echo search($lpublic, "admin");?></a></li>
                    
                    
            </ul>
        </div>
        <div class="time-filter"> 
        	<span><?php echo search($lpublic, "within");?></span>
            <div class="filter-select2 select-style filter-select">
            <select>
                <option value="1" >1<?php echo search($lpublic, "hour");?></option>
                <option value="8">8<?php echo search($lpublic, "hours");?></option>
                <option value="24">1<?php echo search($lpublic, "day");?></option>
                <option value="48">2<?php echo search($lpublic, "days");?></option>
                <option value="168">7<?php echo search($lpublic, "days");?></option>
                <option value="336">2<?php echo search($lpublic, "week");?></option>
                <option value="720">1<?php echo search($lpublic, "month");?></option>
            </select>
            </div>
        </div>
    </div>
   <div id="UserEventDataTable-none" class="empty-message-container content ui-corner-all table-container" >
   	<table class="data-table">
     		<thead>
     			<tr>
            	<th></th>
                <th ><?php echo search($lpublic, "date");?>/<?php echo search($lpublic, "time");?></th>
                <th ><?php echo search($lpublic, "information");?></th>
     			</tr>
     		</thead>
     	</table>
	<div class="no-data-div">
        <img src="/pic/cry.png"  />
        <span class="no-data"><?php echo search($lpublic, "no_data_info");?></span>
     </div>
	</div>
    <div id="UserEventDataTable" class="table-container">
    </div>
</div>