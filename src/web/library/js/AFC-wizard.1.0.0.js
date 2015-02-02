var debug = false, step = 0, sections = ["welcome", "secure", "discover", "wireless", "submit","finish"], wizform;
var text_store1;

$(function () {
	$(".navigation-active").addClass("active"+(step+1));
    wizform = $("#WizardForm");
    if (debug) {
        wizform.find("input[name=corp-ssid]").val("UserSsid");
        wizform.find("input[name=do-guest]").attr("checked", "checked");
        wizform.find("input[name=guest-ssid]").val("Guest");
        wizform.find("input[name=admin-name]").val("admin");
        wizform.find("input[name=admin-pass1]").val("admin");
        wizform.find("input[name=admin-pass2]").val("admin")
    }
    showSection("welcome");
    wizform.find("input[name=nextButton]").click(function () {
        next();
        return false
    });
    wizform.find("input[name=backButton]").click(function () {
        back();
    });
	
    wizform.find("select[name=lang]").focus().val(currentLocale).change(function () {
        location.href = "/wizard.php?lang=" + wizform.find("select[name=lang]").val()
    }).keypress(function (b) {
        b.which == 13 && next()
    });
    wizform.find(".restore-button").click(function () {
        //$("#BackupFile, #BackupFile-confirm").setvisible(true);
        $("#FilePathText, #BackupFileButton, #BackupFile-confirm").setvisible(true);
        return false
    });
    $("#BackupFile").jupload({path:$("#FilePathText"), btn:$("#BackupFileButton")}).bind("jupload_uploaded", function (b, c) {
        $("#BackupFile-confirm .confirm-text").text(V.localizeMsg(Msg.I_ConfirmRestore, c))
    });
    wizform.find('input[name="confirm"]').click(function () {
        //location.href =
        //    "/op/restore"
        location.href = "/waiting.php?waiting_cmd=restore";
    });
    wizform.find('input[name="cancel"]').click(function () {
        $("#BackupFile").jupload("cancel");
    });
    wizform.find("#refresh-but").click(function () {
        $("#discovered").jtable("refresh");
    });
	//openWlan Agreement
	var agrment=$(".openWlan-agreement");	
	$("#openwlan").change(function(){
		  if($("#openwlan").is(":checked"))
		  {	agrment.show().delay(3000);
		  	wizform.find("input[name=nextButton]").attr("disabled", "disabled");
			wizform.find("input[name=backButton]").attr("disabled", "disabled");
		   // agrment.draggable();
		  	var agrment_but = agrment.find(".openWlan-button");
			$("#finishRead").change(function(){
		  	if($(this).is(":checked")) 
				{
				  agrment_but.children().removeClass("disabled");
				  agrment_but.children().removeAttr("disabled");
				  agrment_but.children().click(function(){agrment.hide(); wizform.find("input[name=nextButton]").removeAttr("disabled");wizform.find("input[name=backButton]").removeAttr("disabled");});
				 // agrment_but.children().last().click(function(){agrment.hide();wizform.find("input[name=nextButton]").enable();wizform.find("input[name=backButton]").enable();});
				}
			else  {agrment_but.children().addClass("disabled");
				  agrment_but.children().prop("disabled","disabled");}
		  });
		  }
		  else agrment.hide();
		});
	
	
	//启用访客网络名
	var text_store = $(".enable-guest-access").html();
	text_store1=text_store;
    wizform.find("input[name=do-guest]").change(function () {
        $(".enable-guest-access").toggle(wizform.find("input[name=do-guest]").is(":checked"));
    });
	$(function(){
	$("#NetworkName").bind('keyup focus', function () {
		var text=$("#NetworkName").val();
		if(text != ""){
		$(".enable-guest-access").html(text_store);
		var text1=$(".enable-guest-access").html();	
		var text2 = text1 + "<span>"+text + "-Guest"+"</span>";
		$(".enable-guest-access").html(text2);}
		else $(".enable-guest-access").html(text_store);
		});
	});
	wizform.find("input[name=submitButton]").click(commit);
    wizform.find("input[name=finishButton]").click(show);
	
    var a=wizform.find("select[name=countryCode]");
		$.jpost("/stat_sysinfo.php",{},function(b){
				$.each(b.data[0].ccodes, function(c,d){
						var showname;
						if(currentLocale == "en_US")
						{
							showname = d.name;
						}
						else
						{
							showname = country[d.name];
						}
						$("<option></option>").val(d.code).text(showname).appendTo(a);
					}
				);
				a.val("156")
			}
		);
    $("#discovered").jtable(
				{
					url: "/stat_device.php", 
					params: {state: 1, "default": true}, 
					selectable: "true", 
					objname: "device", 
					pagesize: 5,
					titleshown:false
				})
				.jtable("addcol", "", "locate_state",true,Renderer.status)
				.jtable("addcol",Msg.Version, "name")
				.jtable("addcol", Msg.MACAddress, "mac")
				.jtable("addaction", Msg.Locate, "locate") 
				//function (e) {
				//	return /*e.state == 2 && e["default"] || */Renderer.apStatusType(e.state) == "ok"}, 
				//function (e) {
					//		return(e.locating ? "locating " : "") + "locate-button"})
		//.jtable("addcol", Msg.Uptime, "uptime", true, Renderer.uptime)
		//.bind("jtable_locate", function (b, c) { c.push({"switch_value":e.locating ? "off":"on"});$.jcmd.locate_ap(c) })
		.jtable("addcol", Msg.MACAddress, "state",true,Renderer.state)
		.bind("jtable_refresh-done", function (b, c) {c.selectall()})
		.jtable("refresh")
});
function next() {
	var res = verifySection(sections[step]);
    if (res) {
		if(step > 0)
		{
			$(".navigation-active").removeClass("active"+(step+1));
		}
        step++;
		$(".navigation-active").addClass("active"+(step+1));
        showSection(sections[step])
    }
}
function back() {
	if(step < sections.length)
	{
		$(".navigation-active").removeClass("active"+(step+1));
	}
    step--;
	$(".navigation-active").addClass("active"+(step+1));
    showSection(sections[step])
}
function verifySection(a) {
    if (a == "wireless") {
        if (!V.checkEmpty(wizform.find("input[name=corp-ssid]"), Msg.SecureSSID) || !V.checkSsid(wizform.find("input[name=corp-ssid]"), Msg.SecureSSID))return false;
        if (!V.checkEmpty(wizform.find("input[name=corp-passphrase]"), Msg.SecurityKey) || !V.checkPassphrase(wizform.find("input[name=corp-passphrase]"), Msg.SecurityKey))return false;
		if($('#WirelessPwd').children().last().prop("className")!='pass-rule-position') return false;
		//if($('#WirelessPwd').children().last().className!='pass-rule-position') return false;
        /*if (wizform.find("input[name=do-guest]").is(":checked"))
		{
			if (!V.checkEmpty(wizform.find(".enable-guest-access span"), Msg.Guest+" SSID") 
			|| !V.checkSsid(wizform.find(".enable-guest-access span"), Msg.Guest+" SSID"))
			{
				return false;
			}
			alert("return false");
		}*/
    } else if (a == "secure") {
		if (!V.checkEmpty(wizform.find("input[name=admin-name]"), Msg.AdminAccount) )return false;
       // if (!V.checkEmpty(wizform.find("input[name=admin-name]"), Msg.AdminAccount) || !V.checkAdminName(wizform.find("input[name=admin-name]"), Msg.AdminAccount))return false;
        if (!V.checkAdminPasswords(wizform.find("input[name=admin-pass1]"), wizform.find("input[name=admin-pass2]"), Msg.AdminPassword, Msg.ComfirmPassword))return false
    }
    return true;
}

function showSection(a) {
    $("div.section-content").hide();
    $("ul.wizard-navigation li").removeClass("active");
    $("#WizardForm").data("section", a);
    wizform.find("input[name=backButton]").setvisible(a != "welcome");
    wizform.find("input[name=nextButton]").setvisible(a != "finish" && a != "submit");
    wizform.find("input[name=refreshButton]").setvisible(a == "discover");
	wizform.find("input[name=submitButton]").setvisible(a == "submit");
    wizform.find("input[name=finishButton]").setvisible(a == "finish");
    switch (a) {
        case "welcome":
            $("#Indicator").animate({marginLeft: "0px"}, 250);
            $("#WelcomeTab").addClass("active");
            $("#WelcomeContent").show();
			wizform.find("input[name=nextButton]").select().focus();
            break;
        case "discover":
            $("#Indicator").animate({marginLeft: "200px"}, 250);
            $("#DiscoverTab").addClass("active");
            $("#DiscoverContent").show();
            break;
        case "wireless":
            $("#Indicator").animate({marginLeft: "300px"}, 250);
            $("#WirelessTab").addClass("active");
            $("#WirelessContent").show();
           // wizform.find("input[name=corp-ssid]").select().focus();
		   
            break;
        case "secure":
            $("#Indicator").animate({marginLeft: "100px"}, 250);
            $("#AdministrationTab").addClass("active");
            $("#AdministrationContent").show();
            //wizform.find("input[name=nextButton]").select().focus();
            break;
		case "submit":
            $("#WizardSummary").empty();
			$("#Qr-code").css("display","none");
            $("#WizardSummary").append($("<dt></dt>").text($('.input-table .form-element').first().find('label').text()));
            $("#WizardSummary").append($("<dd></dd>").text(wizform.find("input[name=corp-ssid]").val()));
            if (wizform.find("input[name=do-guest]").is(":checked")) {
                $("#WizardSummary").append($("<dt></dt>").text(text_store1));
                $("#WizardSummary").append($("<dd></dd>").text(wizform.find(".enable-guest-access span").html()))
            }
            $("#WizardSummary").append($("<dt></dt>").text($('.admini-table .form-element').first().find('label').text()));
            $("#WizardSummary").append($("<dd></dd>").text(wizform.find("input[name=admin-name]").val()));
            $("#Indicator").animate({marginLeft: "400px"}, 250);
            $("#FinishTab").addClass("active");
            $("#FinishContent").show();
			wizform.find("input[name=submitButton]").select().focus();
            break;
        case "finish": 
			$("#WizardSummary").find("dd").first().after($("<dd></dd>").attr("class",res2)); 
			$("#WizardSummary").find("dd").first().next().next().after($("<dd></dd>").attr("class",res1));
			if($("#WizardSummary").find("dd").length==5)
			$("#WizardSummary").find("dd").last().after($("<dd></dd>").attr("class",res3));
			$("#Qr-code").css("display","block");
			$(".guest-Qrcode-title").css("display","none");
			$(".wireless-Qrcode").empty();
			$(".guest-Qrcode").empty();
			$(".wireless-Qrcode").qrcode({width:74, height:74, text: "WIFI:S:"+ wizform.find("input[name=corp-ssid]").val()+"T:WPA;P:"+wizform.find("input[name=corp-passphrase]").val()});
			if (wizform.find("input[name=do-guest]").is(":checked")) 
			 {
			 $(".guest-Qrcode-title").css("display","block");
			 $(".guest-Qrcode").qrcode({width: 74,height:74, text: "WIFI:S:"+wizform.find(".enable-guest-access span").html()+"T:nopass;"}) ;
			 $(".guest-Qrcode").css("display","block");
			 }
			 //else { $(".guest-Qrcode").css("display","none");}
            $("#FinishContent").show();
			wizform.find("input[name=finishButton]").select().focus();
            break
    }
}
function commit() {
    //$("div.modal").show();
   commit_admin();
   
}
function commit_admin() {

	
    $.jpost("/add_admin.php", {name: wizform.find("input[name=admin-name]").val(), lang: wizform.find("select[name=lang]").val(), x_password: hex_md5(wizform.find("input[name=admin-pass1]").val())}, commit_corp()/*commit_last()*/)
}
function commit_corp() {
    $.jpost("/add_wlanconf.php", {enabled: true, name: wizform.find("input[name=corp-ssid]").val(), security: "wpapsk", x_passphrase: wizform.find("input[name=corp-passphrase]").val()}, commit_guest())
}
function commit_guest() {
	
    wizform.find("input[name=do-guest]").is(":checked") ? $.jpost("/add_wlanconf.php", {name: wizform.find("input[name=guest-ssid]").val(), enabled: true, is_guest: true, security: "open"}, commit_adopted()) : commit_adopted()

	}
function commit_adopted() {
    var a = [];
	var m = [];
    $("#discovered").jtable("getselected").each(function () {
        a.push($(this).attr("mac"));
        m.push($(this).attr("model"));
    });
    $.jcmd("devmgr", "adopt", {macs: a, models: m}, commit_system);
    //$.jcmd("devmgr", "adopt", {macs: a}, commit_last)
}
function commit_system() {
    $.jpost("/set_setting_country.php", {code: wizform.find('select[name="countryCode"]').val()}, commit_last)
}
function commit_last() {
    $.jcmd("system", "set-installed", {}, function () {
	showSection("finish");
    })
};

function show() {
     location.href = "/login.php";
};

$(document).ready(function () {
	var netText=$("#NetworkName").val();
	$(".enable-guest-access").append($('<span></span>').text(netText+"-Guest"));
	
		$('#AdminName').bind('focus keyup',function(){
		$('#AdministrationContent .name-support').css("display","block");
		});
		$('#AdminName').blur(function(){
		$('#AdministrationContent .name-support').css("display","none");
		});
		$('#NetworkName').bind('focus keyup',function(){
			$('#WirelessContent .name-support').css("display","block");
		});
		$('#NetworkName').blur(function(){
		$('#WirelessContent .name-support').css("display","none");
		});
	
        //文本框绑定事件
		password1_bind();
		password2_bind();
   	    $("#AdminName").mailAutoComplete({
			autoClass: false,
			boxClass: "out_box", 
			listClass: "list_box", 
			focusClass: "focus_box", 
			markCalss: "mark_box"
												  
		});
});

function password1_bind(){
	var pwTips=$('#AdministrationContent').find('.pass-support');
    var oTips = $('#AdministrationContent').find(".pass_stren");	
    var aSpan=oTips.children();
	pwTips.css("display","none");
 	$('#Password').bind('keyup focus', function () {
		if(this.value.length< 1){ pwTips.css("display","none");}
		else{
			pwTips.css("display","block");
			var index = checkStrong($('#Password').val());
			var rank = "s" + index;		
			for (var i = aSpan.length-1; i >=0; i--) {
				//先清空在赋值
				//aSpan[i].className = "";
				//赋值
				if(i >= index)
				{
					aSpan[i].className = "s_clock";
				}		
				else{
					aSpan[i].className = rank + "_active";
					if(i != index - 1){
						aSpan[i].className += " " +rank + "_hidden";
					}		
				}
	
			}//end for
			if(this.value.length<4 || this.value.length>20){
				$('.pass-support-positon .pass-rule').first().children().first().addClass('pass-rule-err');
			}
			else $('.pass-support-positon .pass-rule').first().children().first().removeClass('pass-rule-err');
		}
		
	});
	  $('#Password').blur( function () {
		  pwTips.css("display","none");
		  if( $('.pass-support-positon .pass-rule').first().children().first().prop("className")=='pass-rule-err' || $('.pass-support-positon .pass-rule').first().children().last().prop("className")=='pass-rule-position pass-rule-err' ) 
		  $('.pass-not').css("display","block");  
		  else  $('.pass-not').css("display","none");  
		  });
}

function password2_bind(){
	var pwTips=$('#WirelessContent').find('.pass-support');
    var oTips = $('#WirelessContent').find(".pass_stren");	
    var aSpan=oTips.children();
	pwTips.css("display","none");
    //文本框绑定事件
    $('#Passphrase').bind('keyup focus', function () {
		
		if(this.value.length< 1){ pwTips.css("display","none"); }
		else{
			pwTips.css("display","block");
        var index = checkStrong(this.value);
        var rank = "s" + index;		
        for (var i = aSpan.length-1; i >=0; i--) {
            //先清空在赋值
            //aSpan[i].className = "";
            //赋值
				if(i >= index)
				{
					aSpan[i].className = "s_clock";
				}		
				else{
					aSpan[i].className = rank + "_active";
				if(i != index - 1){
						aSpan[i].className += " " +rank + "_hidden";
					}		
				}}
		if(this.value.length<8 || this.value.length>20){
				$('#WirelessPwd').children().first().addClass('pass-rule-err');
			}
			else $('#WirelessPwd').children().first().removeClass('pass-rule-err');
		}	
    });

	 $('#Passphrase').blur( function () {
		pwTips.css("display","none");
		  if( $('#WirelessPwd').children().first().prop("className")=='pass-rule-err' || $('#WirelessPwd').children().last().prop("className")=='pass-rule-position pass-rule-err' ) 
		 $('.input-table .pass-not-positon').css("display","block");  
		  else  $('.input-table .pass-not-positon').css("display","none");  
		  });
	}

function checkStrong(sValue) {
    var modes = 0;
    //正则表达式验证符合要求的
    if (sValue.length < 1) return modes;
    if (/\d/.test(sValue)) modes++; //数字
    if (/[a-z]/.test(sValue)) modes++; //小写
    if (/[A-Z]/.test(sValue)) modes++; //大写  
    if (/\W/.test(sValue) && !(/\s/.test(sValue)) ) modes++; //特殊字符
    if(/\s/.test(sValue))
	 { $('.pass-support-positon .pass-rule').first().children().last().addClass('pass-rule-err');
	$('#WirelessPwd').children().last().addClass('pass-rule-err');
		//&&!(/\s/.test(sValue))
		 }   else 
		 {$('.pass-support-positon .pass-rule').first().children().last().removeClass('pass-rule-err'); 
		 $('#WirelessPwd').children().last().removeClass('pass-rule-err');}
		
   //逻辑处理
    switch (modes) {
        case 1:
            return 1;
            break;
        case 2:
            return 2; break;
        case 3:
		   return  3;
            break;
        case 4:
            return  4;
            break;
    }
}

	function changepasswd(node){
		var pNode=document.getElementById('Password');
		var temp=pNode.value;
    if(pNode.type=="password"){
        var inputps = document.createElement('input');
        inputps.setAttribute("name","admin-pass1");
        inputps.setAttribute("type","text");
		inputps.setAttribute("id","Password");
		inputps.setAttribute("value",temp);
        node.parentNode.replaceChild(inputps,pNode);
		password1_bind();
    }
	   else{
        var inputps = document.createElement('input');
        inputps.setAttribute("name","admin-pass1");
        inputps.setAttribute("type","password");
		inputps.setAttribute("id","Password");
		inputps.setAttribute("value",temp);
        node.parentNode.replaceChild(inputps,pNode);
		password1_bind();
    }
}
	function changepasswd1(node){
		var pNode=document.getElementById('Passphrase');
		var temp=pNode.value;
    if(pNode.type=="password"){
        var inputps = document.createElement('input');
        inputps.setAttribute("name","corp-passphrase");
        inputps.setAttribute("type","text");
		inputps.setAttribute("id","Passphrase");
		inputps.setAttribute("value",temp);
        node.parentNode.replaceChild(inputps,pNode);
		password2_bind();
    }
	   else{
        var inputps = document.createElement('input');
        inputps.setAttribute("name","corp-passphrase");
        inputps.setAttribute("type","password");
		inputps.setAttribute("id","Passphrase");
		inputps.setAttribute("value",temp);
        node.parentNode.replaceChild(inputps,pNode);
		password2_bind();
    }
}
