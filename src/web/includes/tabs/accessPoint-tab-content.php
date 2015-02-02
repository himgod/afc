<div class="manageAcessPoint system-div">
	<div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "managed_ap");?></label>
    </div>
    <div class="menuBar" >
         <div class="search"> 
            <input  id="AccessPointDataTableSearch" type="text" placeholder='<?php echo search($lpublic, "search_content");?>'>     
        </div>
        <div class="page" id="en-page">
        	<span><?php echo search($lpublic, "display_each_page");?></span>
             <div class="select-style page-select"> 
           <select class="table-pagesizer"></select>
            </div>
        </div>
    </div>
    <div id="AccessPointDataTable-none" class="empty-message-container content ui-corner-all table-container">
 	 <table class="data-table">
     	<thead>
     			<tr>
            	<th></th>
                <th ><?php echo search($lpublic, "name");?>/<?php echo search($lpublic, "mac_address");?></th>
                <th ><?php echo search($lpublic, "ip_address");?></th>
                <th ><?php echo search($lpublic, "status");?></th>
                <th ><?php echo search($lpublic, "client_num");?></th>
                <th ><?php echo search($lpublic, "download");?></th>
                <th ><?php echo search($lpublic, "upload");?></th>
                <th ><?php echo search($lpublic, "channel");?></th>
                <th><?php echo search($lpublic, "operating");?></th>
                </tr>
     	</thead>
     </table>
     <div class="no-data-div">
        <img src="/pic/cry.png"  />
        <span class="no-data"><?php echo search($lpublic, "no_data_info");?></span>
     </div>
	</div>
	<div id="AccessPointDataTable" class="table-container"></div>
</div>

<div id="AccessPointEvent" class="system-div">
	<div class="system-title" id="en-system-title">
        <label class="title"><?php echo search($lpublic, "event_notification_and_tips");?></label>
    </div>
    <div class="menuBar">
       <div class="search"> 
            <input id="AccessPointEventDataTableSearch" type="text" placeholder='<?php echo search($lpublic, "search_content");?>'>     
        </div>
    	 <div class="classify">
            <ul class="choose-shown buttonbar thin">
            		<li id="classify-active"><a href="#all"><?php echo search($lpublic, "all");?></a></li>
                    
                    <li><a href="#admin"><?php echo search($lpublic, "admin");?></a></li>
                    <li><a href="#ap"><?php echo search($lpublic, "ap");?></a></li>
                    
            </ul>
        </div>
       <div  class="time-filter"> 
        	<span><?php echo search($lpublic, "within");?></span>
            <div class="filter-select2 select-style filter-select">
            <select >
                <option value="1"  selected="selected">1<?php echo search($lpublic, "hour");?></option>
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
    <div id="AccessPointEventDataTable-none" class="empty-message-container content ui-corner-top table-container" >
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
<div id="AccessPointEventDataTable" class="table-container" >
</div>
</div>