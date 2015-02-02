<div class="system-title" id="en-system-title">
    <label><?php echo search($lpublic, "get_help_info");?></label>
    <span class="service_code"><?php echo search($lpublic, "service_code");?> : <span id="my_service_code"></span></span>
</div>
<div>
    <div class="ordinarySupport">
        <div id="accessBBS" class="support-element">
            <div class="support-icon "></div>
            <div class="supportPrompt">
                <p class="proText" id="en-proText"><a href="http://www.open-wlan.org">http://www.open-wlan.org</a> </p>
                <?php echo search($lpublic, "openwlanBBS");?>
                
           	</div>
        </div>
        <div id="QrCode" class="support-element">
            <div class="support-icon icon-weixin"></div>
            <div class="supportPrompt">
                <p class="proText" id="en-proText"> <a class="custom-width"  href="javascript:void(0)" rel="/pic/autelan-qrcode.png"> <?php echo search($lpublic, "autelan");?> </a>	</p>
                <?php echo search($lpublic, "focus_QRcode");?>
           </div>
        </div>
        <div id="phone" class="support-element">
            <div class="support-icon icon-phone"></div>
            <div class="supportPrompt">
                <p class="proText" id="en-proText">400-611-1829  </p>
            	<?php echo search($lpublic, "service_phone_call");?>
            </div>
        </div>
        <div id="qq" class="support-element">
            <div class="support-icon icon-qq"></div>
            <div class="supportPrompt">
                <p class="proText" id="en-proText"><?php echo search($lpublic, "qq_group");?>:319577163  </p>
                <?php echo search($lpublic, "online_chat");?>
                </div>
        </div>
        <div id="other" class="support-element">
            <div class="support-icon icon-other"></div>
            <div class="supportPrompt">
                <p class="proText" id="en-proText"><a id="downbutton" href="#qq" class="support-down"> <?php echo search($lpublic, "other_method2");?></a> </p>                
            	 <?php echo search($lpublic, "other_method");?>     
           	</div>
        </div>
    </div>
</div>
<div id="othersupport" class="otherSupport" style="display:none" >
<div class="supportFeedback">
    <p class="prompSupport" id="en-prompSupport"><?php echo search($lpublic, "feedback_info");?></p>
    <p class="proText" id="en-proText"><?php echo search($lpublic, "problem_protmp");?></p>
    <form action="" href="javascript:void(0)" >
        <div class="problem-select">
            <input type="checkbox" name="cannotConnect"  id="cannotConnect"/>
            <label for="cannotConnect"><?php echo search($lpublic, "internet_problem");?></label>
            <input type="checkbox" name="wirelessSignalweak" id="wirelessSignalweak" />
            <label for="wirelessSignalweak"><?php echo search($lpublic, "wireless_signal_problem");?></label>
            <input type="checkbox" name="SpeedSlow"  id="SpeedSlow"/>
            <label for="SpeedSlow"><?php echo search($lpublic, "speed_problem");?></label>
            <input type="checkbox" name="WirelessOutage" id="WirelessOutage" />
            <label for="WirelessOutage"><?php echo search($lpublic, "wireless_problem");?></label>
            <input type="checkbox" name="difficultToOperate" id="difficultToOperate" />
            <label for="difficultToOperate"><?php echo search($lpublic, "software_operation_problem");?></label>
            <input type="checkbox" name="Uglyinterface" id="Uglyinterface" />
            <label for="Uglyinterface"><?php echo search($lpublic, "interface_problem");?></label>
            <input type="checkbox" name="badPlugin" id="badPlugin" />
            <label for="badPlugin"><?php echo search($lpublic, "plugin_problem");?></label>
            <input type="checkbox" name="softwareCollapse" id="softwareCollapse" />
            <label for="softwareCollapse"><?php echo search($lpublic, "software-crash_problem");?></label>
            <input type="checkbox" name="other" id="other" />
            <label for="other"><?php echo search($lpublic, "other_problem");?></label>
            <br />
            <textarea rows="6" cols="114" wrap="hard"></textarea>
            <br/>
            <p>
                <input type="checkbox" name="Allow" id="feedback-allow" />
                <label for="feedback-allow"><?php echo search($lpublic, "feedback_allow");?></label>
            </p>
        </div>
        <div class="contact-info">
            <p class="proText" id="en-proText"><?php echo search($lpublic, "contact_information");?></p>
            <span><?php echo search($lpublic, "email");?>:</span>
            <input type="text" name="email" />
            <span><?php echo search($lpublic, "cellphoneNum");?>:</span>
            <input type="text" name="phoneNum" />
            <div class="select-style contact-select" id="en-contact-select">
                <select>
                    <option value="qq">QQ</option>
                    <option value="weixin"><?php echo search($lpublic, "weixin");?></option>
                </select>
            </div>
            <input type="text" class="contact-select-text"/>
        </div>
        <div class="feedbackButton">
            <button class="feedback-clear"><?php echo search($lpublic, "clear");?></button>
            <button class="feedback-submit"><?php echo search($lpublic, "submit_feedback");?></button>
        </div>
    </form>
</div>
</div>