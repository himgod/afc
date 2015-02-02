<div class="wireless-network-setting wirlessConfi-div">
    <div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "wireless_network_setting");?></label>
    </div>
    <div >
        <div class="settings-list" id="SettingsWirelessNetworksControl">
            <div id="wirelessNetworkSettingTable" class="wirelessConfTable1">
                <div id="WlanConfTableContainer-none">
                    <table class="data-table">
                        <thead>
     					<tr>
                        <th></th>
                            <th><?php echo search($lpublic, "wireless_networks");?></th>
                            <th><?php echo search($lpublic, "signal_status");?></th>
                            <th><?php echo search($lpublic, "detailed_settings");?></th>
     					</tr>
                            </thead>
                    </table>
                    <div class="no-data-div">
                    	<img src="/pic/cry.png"  />
                        <span class="no-data"><?php echo search($lpublic, "no_data_info");?></span>
                    </div>
                </div>
                <div class="data-table-container" id="WlanConfTableContainer"></div>
            </div>
            <button id="wireless-add1" class="system-add" href="#" name="create">
            <img src="pic/add.png">
            <label><?php echo search($lpublic, "add");?></label>
            </button>
        </div>
        <div class="wirlessConf-content ">
            <div class="countryCode wirlessConf-content-element">
                <label><?php echo search($lpublic, "country_code");?></label>
                <div class="select-style country-select">
                    <select id="CountryCode" name="countryCode">
                    </select>
                </div>
            </div>
            <div class="optimize wirlessConf-content-element">
                <label><?php echo search($lpublic, "automatic_optimization_strategy");?></label>
                <div class="optimize-control">
                    <div class="optimize-preimg optimize-font">0</div>
                    <div style="position:absolute;left:22px;">
                        <div  class="dragging-sliders" id="optimize-drag">
                            <div class="dragging-gra"> </div>
                            <div style="width: 66px;" class="dragging-blu"> </div>
                            <div style="left: 66px;" class="dragging-ball"> </div>
                            <div style="left: 229px;" class="dragging-comment"><span>时间间隔(Per 20 min)</span></div>
                        </div>
                    </div>
                    <div class="optimize-sufimg optimize-font">100</div>
                </div>
            </div>
            <span class="prot"><?php echo search($lpublic, "optimization_note");?></span> </div>
    </div>
</div>
<div class="AFi-strategy wirlessConfi-div">
    <div class="system-title" id="en-system-title">
        <label>AFi<?php echo search($lpublic, "policy");?></label>
    </div>
    <div class="wirlessConf-content">
        <div class="version-update wirlessConf-content-element">
            <label><?php echo search($lpublic, "version_update");?></label>
            <div>
                <ul class="header-nav-ul update-nav-ul" id="en-update-nav-ul">
                    <li id="choose-button"><a><?php echo search($lpublic, "not_download");?></a></li>
                    <li><a><?php echo search($lpublic, "download_and_not_update");?></a></li>
                    <li><a><?php echo search($lpublic, "download_and_update");?></a></li>
                </ul>
            </div>
        </div>
        <div class="network-adaption wirlessConf-content-element">
            <label><?php echo search($lpublic, "network_adaptive");?></label>
            <div id="f1" class="switch off" checked="false"></div>
        </div>
        <div class="access-control wirlessConf-content-element">
            <label><?php echo search($lpublic, "access_control");?></label>
            <div id="f2" class="switch off" checked="false"></div>
        </div>
        <div class="blacklist wirlessConf-content-element">
            <label><?php echo search($lpublic, "blacklist");?></label>
            <div id="f3" class="switch off" checked="false"></div>
        </div>
    </div>
    <div id="AFIStrategyTable" class="wirelessConfTable2">
        <div id="BlackListTableContainer1-none"></div>
        <div class="data-table-container" id="BlackListTableContainer1"></div>
    </div>
    <button id="wireless-add2" class="system-add" href="#">
    <img src="pic/add.png">
    <label><?php echo search($lpublic, "add");?></label>
    </button>
</div>
<div class="user-strategy wirlessConfi-div">
    <div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "user_policy");?></label>
    </div>
    <div class="wirlessConf-content">
        <div class="optimize wirlessConf-content-element">
            <label><?php echo search($lpublic, "automatic_optimization_strategy");?></label>
            <div class="optimize-control">
                <div class="optimize-preimg " id="optimize-position"><?php echo search($lpublic, "more_user");?></div>
                <div class="dragging" id="en-dragging">
                    <div class="dragging-sliders" id="loadbalance-drag" >
                        <div class="dragging-gra" > </div>
                        <div class="dragging-blu" > </div>
                        <div class="dragging-ball" > </div>
                    </div>
                </div>
                <div class="optimize-sufimg" id="optimize-position" style="left: 400px;">宽带更大</div>
            </div>
        </div>
    </div>
    <div id="limitedSpeedTable" class="wirelessConfTable3">
        <div id="UserGroupsTableContainer-none">
            <table class="data-table">
                <thead>
     			<tr>
                <th></th>
                    <th><?php echo search($lpublic, "speed_limit_policy_group");?></th>
                    <th><?php echo search($lpublic, "broadband_limit");?></th>
                    <th><?php echo search($lpublic, "detailed_settings");?></th>
     			</tr>
                        </thead>
                <tbody>
                    <tr class="odd">
                        <td></td>
                        <td>Default</td>
                        <td><?php echo search($lpublic, "download");?> : <?php echo search($lpublic, "no_limit");?> <?php echo search($lpublic, "upload");?>：<?php echo search($lpublic, "no_limit");?></td>
                        <td><button><?php echo search($lpublic, "block");?></button>
                            <button><?php echo search($lpublic, "reconnection");?></button></td>
                    </tr>
                </tbody>
            </table>
            <div class="no-data-div">
                    	<img src="/pic/cry.png"  />
                        <span class="no-data"><?php echo search($lpublic, "no_data_info");?></span>
                    </div>
        </div>
        <div id="UserGroupsTableContainer" class="data-table-container"></div>
    </div>
    <button id="wireless-add3" class="system-add" href="#" name="create">
    <img src="pic/add.png">
    <label><?php echo search($lpublic, "add");?></label>
    </button>
    <div class="wirlessConf-content">
        <div class="blacklist wirlessConf-content-element">
            <label><?php echo search($lpublic, "blacklist");?></label>
            <div id="f4" class="switch off" checked="false"></div>
        </div>
    </div>
    <div id="blacklistTable" class="wirelessConfTable4">
        <div id="BlackListTableContainer2-none"></div>
        <div class="data-table-container" id="BlackListTableContainer2"></div>
    </div>
    <button id="wireless-add4" class="system-add" href="#">
    <img src="pic/add.png">
    <label><?php echo search($lpublic, "add");?></label>
    </button>
</div>
<div id="GuestControlForm" class="guest-strategy wirlessConfi-div">
    <div class="system-title" id="en-system-title">
        <label><?php echo search($lpublic, "guest_policy");?></label>
    </div>
    <div class="wirlessConf-content">
        <div class="guest-portal wirlessConf-content-element">
            <label><?php echo search($lpublic, "guest_portal");?></label>
            <div id="f4-2" class="switch off" checked="false"></div>
        </div>
        <div id="guest-portal">
            <div class="authentication-method wirlessConf-content-element">
                <label><?php echo search($lpublic, "authentication_method");?></label>
                <div class="radio-style">
                    <input type="radio" name="guestAuthentication" value="no" id="no" checked="checked" />
                    <label class="radio-select" for="no"><?php echo search($lpublic, "no_authentication");?></label>
                    <input type="radio" name="guestAuthentication" value="PORTAL" id="PORTAL" />
                    <label class="radio-unselect" for="PORTAL">PORTAL<?php echo search($lpublic, "account");?>/<?php echo search($lpublic, "password");?></label>
                </div>
            </div>
            <div class="authentication">
                <p>
                    <label><?php echo search($lpublic, "account");?></label>
                    <input class="account-name system-confi-input" id= "auth-user" name="admin-name" type="text" onkeyup="this.value=this.value.replace(/^ +| +$/g,'')" autocomplete="off" value="<?php echo $username;?>" />
                </p>
                <p  >
                    <label style="float:left;margin-top: 5px;"><?php echo search($lpublic, "password");?></label>
                <div class="Password">
                    <input id="Password" name="admin-pass" type="password" autocomplete="off" value="admin" />
                    <div class="eye"   onclick="changepasswd(this);"></div>
                </div>
                <div class="pass_stren"> <span><?php echo search($lpublic, "weak");?></span> <span><?php echo search($lpublic, "medium");?></span> <span><?php echo search($lpublic, "stronger");?></span> <span><?php echo search($lpublic, "strongest");?></span> </div>
                </p>
                <p class="account-button"> <a id="auth-list"><?php echo search($lpublic, "query");?></a> <a class="a-line" id="auth-add"><?php echo search($lpublic, "add");?></a> <a class="a-line" id="auth-delete"><?php echo search($lpublic, "delete");?></a> </p>
                <hr color="#eee"/>
                <div id="AuthUserDataTable" class="data-table-container" style="display: none;">
                    <table class="data-table stat">
                        <thead>
                            <tr class="first-row">
                            	<th class="sortable"><span></span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "account");?></span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "mac_address");?></span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "ip_address");?></span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "upload_flow");?>(KB)</span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "download_flow");?>(KB)</span></th>
                                <th class="sortable"><span><?php echo search($lpublic, "create_time");?></span></th>
                            </tr>
                        </thead>
                        <tbody>
                        </tbody>
                    </table>
                    <div class="data-table-status">
                        <div class="data-table-paging">
                            <span class="data-table-paging-details">0 - 0 / 0</span>
                            <a href="javascript:void(0);" class="back-button-disabled"></a>
                            <span class="ui-corner-all" style="display:none">1</span>
                            <a href="javascript:void(0);" class="next-button-disabled"></a>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="validity wirlessConf-content-element ">
            <label><?php echo search($lpublic, "validity_of_the_certification");?></label>
            <div class="select-style validity-select">
                <select id = "auth-time">
                    <option value="8">8<?php echo search($lpublic, "hours");?></option>
                    <option value="24">24<?php echo search($lpublic, "hours");?></option>
                    <option value="72">3<?php echo search($lpublic, "days");?></option>
                    <option value="168">7<?php echo search($lpublic, "days");?></option>
                    <option value=""><?php echo search($lpublic, "custom");?></option>
                </select>
            </div>
        </div>
        <div class="jump-after wirlessConf-content-element">
            <label><?php echo search($lpublic, "jump_after_the_certification");?></label>
            <div class="radio-style">
                <input type="radio" name="jump" value="original" id="original" checked="checked"/>
                <label class="radio-select" for="original"><?php echo search($lpublic, "original_access_page");?></label>
                <br />
                <input type="radio" name="jump" value="custom"  id="custom"/>
                <label class="radio-unselect" for="custom"><?php echo search($lpublic, "custom_portal_page");?></label>
                <input type="text" name="protal" class="system-confi-input" disabled="disabled" id="custom_portal" value="http://"/>
            </div>
        </div>
        <div id="subnet" >
            <div> <span><?php echo search($lpublic, "restricted_subnets");?></span>
                <p class="restricted-subnets" >
                    <input name="subnet" text="text" class="restricted-subnets text-input"/>
                </p>
            </div>
            <div> <span><?php echo search($lpublic, "allowed_subnets");?></span>
                <p class="allowed-subnets">
                    <input type="text"  name="subnet"  class="allowed-subnets text-input"/>
                </p>
                <br />
            </div>
        </div>
    </div>
</div>
<div class="wireless-network-add">
    <p class="add-page-title"><?php echo search($lpublic, "create_wireless_network");?><span class="close-icon" id="close1"></span></p>
    <div class="add-page-state"><?php echo search($lpublic, "in_using");?></div>
    <div class="add-page-table">
        <div class="add-page-table-title"><?php echo search($lpublic, "basic_info");?></div>
        <form id="WirelessNetworksForm">
            <div class="Create-Wlan-form">
                <div  class="Create-Wlan-form-element" id="create-auto">
                    <label><?php echo search($lpublic, "create_auto");?></label>
                    <div class=" Create-Wlan-form-div">
                	<div id="w1" class="switch off" checked="false" ></div>
                	</div>
                </div>
                <div  class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "hide_signal");?></label>
                    <div  class=" Create-Wlan-form-div">
                	<div id="w2" class="switch off" checked="false"></div>
                	</div>
                </div>
                <div  class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "signal_name");?></label>
                    <div class="Create-Wlan-form-div">
                   		<input type="text" value="ylink-L5m2n8" class="Create-Wlan-form-input1" name="name" />
                	</div>
                </div >
                <div  class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "security");?></label>       
                    	<div class="Create-Wlan-form-div create-wlan-control">        		 
                            <div class="dragging">
                                <div class="dragging-sliders" id="security-drag" >
                                     <div class="dragging-gra" > </div>
                                     <div class="dragging-blu" > </div>
                                     <div class="dragging-ball" > </div>
                                     <div style="left: 180px;" class="dragging-comment-se"><span></span></div>
                                </div>
                            </div>
						</div>
                </div>
                <div   id="wpapsk_pass" class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "connect_password");?></label>
                
                	<div class="Create-Wlan-form-div">
                    	<input type="text" name="passphrase" class="Create-Wlan-form-input1" />
                	</div>
                </div>
                <div   id="wpaeap_pass" class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "ip_address");?></label>
                
                	<div class="Create-Wlan-form-div">
                    	<input type="text" value="" name="radiusIp1" class="Create-Wlan-form-input2"/>
                        <span class="ip-address"><?php echo search($lpublic, "port");?></span>
                        <input type="text" value="1812" name="radiusPort1" class="ip-address-input"/>
                	</div>
                </div>
                <div id="wpaeap_pass2" class="Create-Wlan-form-element">
                	 <label><?php echo search($lpublic, "connect_password");?></label>
                     <div class="Create-Wlan-form-div">
                		<input name="radiusSecret1" class="passwd-input" maxlength="64" type="text">
                    </div>
                </div>
                <div  class="Create-Wlan-form-element"> 
                    <label><?php echo search($lpublic, "speed_limit_policy");?></label>
                
                <div class="select-style validity-select1 Create-Wlan-form-div">
                    <select id="Create-Wlan-speed-limit">
                        
                    </select>
                </div>
                </div>
                <div class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "guest_policy");?></label>
                    <div class=" Create-Wlan-form-div">
                	<div id="w3" class="switch off" checked="false"></div>
                	</div>
                </div>
                <div  class="Create-Wlan-form-element">
                    <label><?php echo search($lpublic, "create_qrcode");?></label>
               		<div class="Create-Wlan-form-div">
                    	<img src="/pic/make-qrcode.png" style="position: relative;top: 5px;margin-left: 2px;"/>
                    	<input type="button"  value='<?php echo search($lpublic, "create_card");?>' class="create-qrcode-button" />
                	</div>
                </div>
            </div>
            <div class="wlan-detail">
                <div   style="border-bottom: 1px dashed #EEE;border-top: 1px dashed #EEE;">
                    <p class="Create-Wlan-form-element">
                        <label><?php echo search($lpublic, "num_users");?></label>
                        <span class="user-num">phpdata</span>
                    </p>
                    <p class="Create-Wlan-form-element">
                        <label><?php echo search($lpublic, "num_guests");?></label>
                        <span class="guest-num">phpdata</span>
                    </p>
                </div>
				 
                    <p class="Create-Wlan-form-element">
                        <label><?php echo search($lpublic, "up_pkts_bytes");?></label>
                        <span class="up-link">phpdata</span>
                    </p>
                    <p class="Create-Wlan-form-element">
                        <label><?php echo search($lpublic, "down_pkts_bytes");?></label>
                        <span class="down-link">phpdata</span>
                    </p>
                
            
            </div>
            <p style="text-align:center">
                <input type="button" value='<?php echo search($lpublic, "apply");?>' id="add-network-commit-button"  class="add-page-button" name="apply"/>
            </p>
        </form>
    </div>
</div>

<div class="Qrcode-card">
    <div class="Qrcode-card-title">
        <span class="Qrcode-card-title-image">
            <?php echo search($lpublic, "qrcode_card_generator");?>
        </span>
        <span class="close-icon" id="close2"></span>
    </div>
   <div class="qrcode-scroll">
    <div class="Qrcode-card-div">
    	<div class="Qrcode-card-title2">WIFI <?php echo search($lpublic, "qrcode");?></div>
        <div class="wifi-qrcode-content">
        	<p class="wifi-qrcode-prompt"><?php echo search($lpublic, "create_qrcode_info");?></p>
        	<div class="wifi-qrcode-prompt2">
            	<?php echo search($lpublic, "create_qrcode_info2");?>
            </div>
            <div class="wifi-qrcode-data">
                <p>
                    <label><?php echo search($lpublic, "wireless_networks");?>/SSID</label>
                    <span>phpdata</span>
                </p>
                <p>
                    <label><?php echo search($lpublic, "connect_password");?></label>
                    <span>phpdata</span>
                </p>
                <p>
                    <label><?php echo search($lpublic, "security_settings");?></label>
                    <span>phpdata</span>
                </p>
                <p>
                    <label><?php echo search($lpublic, "pass_valid");?></label>
                    <span>phpdata</span>
                </p>
            </div>
            <div class="wifi-qrcode-err">
            	<div class="wifi-qrcode-prompt wifi-qrcode-error">
                	<?php echo search($lpublic, "scan_qrcode_info");?><br />
					<?php echo search($lpublic, "scan_qrcode_info2");?>

				</div>    
                <input value='<?php echo search($lpublic, "return");?>' type="button"  class="wifi-qrcode-button wifi-qrcode-button-back"/>             
            </div>
            <div class="wifi-qrcode-design">
            	<p class="addlogo">
                	<input type="checkbox" name="logo" id="add-logo"/><label for="add-logo"><?php echo search($lpublic, "add2");?>LOGO</label>
                    <input value='<?php echo search($lpublic, "browse");?>' type="button"  class="wifi-qrcode-button  wifi-qrcode-button-liulan"/> 
                </p>
                <p>
                	<span>LOGO<?php echo search($lpublic, "effect");?></span>
                    <input type="radio" name="logo-effect" value="original-pic" id="original-pic" checked="checked"/>
                	<label class="radio-select wifi-qrcode-radio" for="original-pic"><?php echo search($lpublic, "original_drawing");?></label>
                    <input type="radio" name="logo-effect" value="fillet"  id="fillet"/>
                    <label class="radio-unselect wifi-qrcode-radio" for="fillet"><?php echo search($lpublic, "fillet");?></label>
                </p>
                <p>
                	<input type="button" value='<?php echo search($lpublic, "create_qrcode");?>' class="wifi-qrcode-button wifi-qrcode-button-big " />
                    <input type="button" value='<?php echo search($lpublic, "download_pic");?>' class="wifi-qrcode-button wifi-qrcode-button-big wifi-qrcode-button-blue"/>
                </p>
                <div class="down-wifi-card-div"> 
                	<?php echo search($lpublic, "create_wifi_card");?>
                </div>
            </div>
        </div>
        <div class="wifi-qrcode-result wifi-qrcode-result1"><img src="/pic/wifi-qrcode.png" /></div>
    </div>
    <div class="Qrcode-card-div Qrcode-card-div2">
    	<div class="Qrcode-card-title2"><?php echo search($lpublic, "create_wifi_card");?></div>
        <div class="wifi-qrcode-content wifi-qrcode-color">
        	<p class="wifi-qrcode-prompt"><?php echo search($lpublic, "printed_card_info");?></p>
            <input type="text" placeholder='<?php echo search($lpublic, "printed_card_title");?>'  class="title-input"/>
            <textarea placeholder='<?php echo search($lpublic, "printed_card_content");?>'  rows="20" cols="20" class="content-input"></textarea>
            <p style="margin-top: 35px;">
                	<input type="button" value='<?php echo search($lpublic, "generation_card");?>' class="wifi-qrcode-button wifi-qrcode-button-big " />
                    <input type="button" value='<?php echo search($lpublic, "Direct_printing");?>' class="wifi-qrcode-button wifi-qrcode-button-big wifi-qrcode-button-blue"/>
            </p>
        </div>
		<div class="wifi-qrcode-result wifi-qrcode-result2 wifi-qrcode-color">
    		<img src="/pic/wifi-qrcode.png" />
    		<span>Powered by Open-Wlan<?php echo search($lpublic, "league");?></span>
        	<span>http://www.open-wlan.org</span>
            <span class="font-green">密码有效期：2014年11月18日至2014年12月17日</span>
            <span class="wifi-title-input">title</span>
            <span class="wifi-content-input">content</span>
            <div class="jiao"> 
            	    <div class="qrcode-autelan"></div>
            		<span class="qrcode-autelan-font"><?php echo search($lpublic, "User_service");?></span>
            </div> 
        
    	</div>
    </div>
    </div>

</div>

<div class="user-black-list-add">
    <p class="add-page-title"><?php echo search($lpublic, "user_blacklist");?><span class="close-icon" id="close3"></span></p>
    <div class="add-page-state"><?php echo search($lpublic, "in_using");?></div>
    <div class="add-page-table">
        <div class="add-page-table-title" id="user-blacklist-add-title"></div>
        <p class="add-page-table-prompt"><?php echo search($lpublic, "blacklist_info");?><?php echo search($lpublic, "user");?>?</p>
        <form id="UsersBlackList">
            <div class="user-black-list-table">userblacklist table</div>
            <p style="text-align:center">
                <input type="button" value='<?php echo search($lpublic, "apply");?>' name="apply"  class="add-page-button"/>
            </p>
        </form>
    </div>
</div>


<div class="AFI-black-list-add">
    <p class="add-page-title">AFi <?php echo search($lpublic, "blacklist");?><span class="close-icon" id="close4"></span></p>
    <div class="add-page-state"><?php echo search($lpublic, "in_using");?></div>
    <div class="add-page-table">
        <div class="add-page-table-title" id="afi-blacklist-add-title">?í??</div>
        <p class="add-page-table-prompt"><?php echo search($lpublic, "blacklist_info");?>AFI?</p>
        <form id="AFIBlackList">
            <div class="afi-black-list-table">AFi blacklist table</div>
            <p style="text-align:center">
                <input type="button" value='<?php echo search($lpublic, "apply");?>' name="apply" class="add-page-button"/>
            </p>
        </form>
    </div>
</div>
<div class="user-group">
    <p class="add-page-title"><?php echo search($lpublic, "user");?><?php echo search($lpublic, "speed_limit_policy");?><span class="close-icon" id="close5"></span></p>
    <div class="add-page-table">
        <div class="add-page-table-title" id="user-group-add-title"></div>
        <form id="UserGroup">
            <p>
            <label><?php echo search($lpublic, "name");?></label>
            <input type="text" value="UserGrp-1130" name="usergroupname" class="user-group-input"/>
            </p>
            <p>
            <label><?php echo search($lpublic, "bandwidth_limit");?>(<?php echo search($lpublic, "download");?>)</label>
            <input type="checkbox" name="download" />
            <label class="user-group-limit-label"><?php echo search($lpublic, "limited_to");?></label>
              <div class="select-style validity-select1">
                <select id="bandwidth-limit-select1" class="easyui-combobox">
                    <option value="50K">50K</option>
                    <option value="60K">60K</option>
                    <option value="512K" selected="selected">512K</option>
                    <option value="1M">1M</option>
                    <option value="2M">2M</option>
                    <option value="4M">4M</option>
                    <option value="8M">8M</option>
                </select>
                <input id="bandwidth-limit-text1" value="" style="position: absolute; width: 80px; left: 0px; z-index: 99; border: 1px solid #fff; height: 35px; top: -5px;" type="text">
            </div>
            <span>bps</span>
            </p>
            <p>
            <label><?php echo search($lpublic, "bandwidth_limit");?>(<?php echo search($lpublic, "upload");?>)</label>
            <input type="checkbox" name="upload" />
            <label class="user-group-limit-label"><?php echo search($lpublic, "limited_to");?></label>
              <div class="select-style validity-select1">
                <select id="bandwidth-limit-select2">
                    <option value="50K">50K</option>
                    <option value="60K" selected="selected">60K</option>
                    <option value="512K">512K</option>
                    <option value="1M">1M</option>
                    <option value="2M">2M</option>
                    <option value="4M">4M</option>
                    <option value="8M">8M</option>
                </select>
                <input id="bandwidth-limit-text2" value="" style="position: absolute; width: 80px; left: 0px; z-index: 99; border: 1px solid #fff; height: 35px; top: -5px;" type="text">
            </div>
             <span>bps</span>
            </p>
            
            <p style="text-align:center">
                <input type="button" value='<?php echo search($lpublic, "apply");?>' name="apply"  class="wifi-qrcode-button wifi-qrcode-button-big"/>
                <input type="button" value='<?php echo search($lpublic, "cancel");?>' class="wifi-qrcode-button wifi-qrcode-button-big wifi-qrcode-button-blue" name="cancel"/>
            </p>
    		</form>
    </div>
</div>
