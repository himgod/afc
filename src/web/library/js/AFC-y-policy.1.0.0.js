$(function () {
	var posX = 0;
	var move_width = 39;
    $("div.y-panel").hide();
    $("#y map area").each(function () {
        $(this).click(function () {
            var e = $(this).attr("href");
            $(e).show();
            $("#y").hide();
            return false
        })
    });	
    $("ul.bread-crumbs li:first-child").each(function () {
        $(this).click(function () {
            $("#y").show();
            $("div.y-panel").hide();
            return false
        })
    });
    $("#PolicyCenterTab").click(function () {
        $("#y").show();
        $("div.y-panel").hide();
        return false
    });
	$("#span3").attr("class", "span3");
	$("#btn span").each(function(){
		$(this).click(function(){
			$("#btn span").attr("class", "");
			$(this).attr("class", $(this).attr("name"));
		});
	});
	var switches = $(".switch");
	switches.each(function(){
		var one = $(this);
		one.posX = 0;
		one.move_width = 39;
		one.dmove = false;
		one.moved = false;
		one.set_switch = function(obj, on)
		{			
			if(on == true || on == "on")
			{
				obj.attr("class", "switch on");
				obj.attr("checked", true);
			}
			else if(on == false || on == "off")
			{
				obj.attr("class", "switch off");
				obj.attr("checked", false);
			}
			//else
			{
				//do nothing
			}
		};
		one.switch_click = function(ev)
		{
			if(one.moved)
			{
				return ;
			}
			one.moved = false;
			if($(this).hasClass("off"))
			{
				one.set_switch($(this), "on");
			}
			else //if($(this).hasClass("on"))
			{
				one.set_switch($(this), "off");
			}
			one.dmove = false;
			$(this).css("backgroundPosition", "0px 0");
		  	//$(document).unbind("mousemove", one.switch_mousemove);
		  	//$(document).unbind("mouseup", one.switch_mouseup);
		};
		one.switch_mousedown = function(ev)
		{
			if(!ev)
			{
				ev = window.event;
			}
			
			if($(this).hasClass("off"))
			{
				one.posX = one.move_width+ev.clientX;
			}
			else if($(this).hasClass("on"))
			{
				one.posX = ev.clientX;
			}
			else
			{
				one.posX = ev.clientX-parseInt($(this).css("backgroundPositionX"));
			}
			//$(document).mousemove(one.switch_mousemove);
			//$(document).mouseup(one.switch_mouseup);
			one.dmove = true;
			one.moved = false;
		};
		one.switch_mouseup = function(ev)
		{
			if(ev==null) ev = window.event;
			if(!one.dmove)
			{
				return ;
			}
			var x = ((ev.clientX - one.posX) < -one.move_width) ? -one.move_width : ((ev.clientX - one.posX) > 0) ? 0 : (ev.clientX - one.posX);

			if(x >= -(Math.floor(one.move_width/2)+1))
			{
				one.set_switch($(this), "on");
			}
			else 
			{
				one.set_switch($(this), "off");
			}
			one.dmove = false;
			//one.moved = false;
		  	//$(document).unbind("mousemove", one.switch_mousemove);
		  	//$(document).unbind("mouseup", one.switch_mouseup);
			$(this).css("backgroundPosition", "0px 0");
		};
		one.switch_mousemove = function (ev)
		{
			if(ev==null) ev = window.event;
			if(!one.dmove)
			{
				return ;
			}
			clearSelection();
			one.moved = true;
			$(this).attr("class", "switch moving");
			var x = ((ev.clientX - one.posX) < -one.move_width) ? -one.move_width : ((ev.clientX - one.posX) > 0) ? 0 : (ev.clientX - one.posX);
			$(this).css("backgroundPosition", x+"px 0");
		};
		//one.mousedown(one.switch_mousedown);
		//one.mousemove(one.switch_mousemove);
		//one.mouseup(one.switch_mouseup);
		//one.mouseout(one.switch_mouseup);
		one.click(one.switch_click);
		if($(this).hasClass("on"))
		{
			$(this).attr("checked", true);
		}
		else
		{
			$(this).attr("checked", false);
		}
	});
	
	var e = $("#Y-Wifi-Policy");
	$("#wifi-add").click(function(){$("#wifi-set").show();e.find("li.form").show();$("#wifi-list").hide();});
	$(".wifi-info").click(function(){$("#wifi-set").show();e.find("li.form").show();$("#wifi-list").hide();});
	e.find("li.list a").click(function(){$("#wifi-set").hide();e.find("li.form").hide();$("#wifi-list").show();});
	var e2 = $("#Y-User-Policy");
	$("#user-blist-add").click(function(){$("#user-list-for-block").show();e2.find("li.form").show();$("#user-blist").hide();});
	e2.find("li.list a").click(function(){$("#user-list-for-block").hide();e2.find("li.form").hide();$("#user-blist").show();});

	var e3 = $("#Y-AFi-Policy");
	$("#afi-blist-add").click(function(){$("#afi-for-blist-add").show();e3.find("li.form").show();$("#afi-blist").hide();});
	e3.find("li.list a").click(function(){$("#afi-for-blist-add").hide();e3.find("li.form").hide();$("#afi-blist").show();});
	function setlabelcontent(text)
	{
		if(text["content"])
		{
			return "<span "+(text["color"] ? ("style='color:white;background-color:"+text["color"]+"'"):"")+">"+text["content"]+"</span>";
		}
		else
		{
			return "<span></span>";
		}
	}
	function clearSelection()
	{
		window.getSelection ? window.getSelection().removeAllRanges() : document.selection.empty();
	}
	
	var se_drag = $("#security-drag");
	se_drag.data("textcontent", [{content:Msg.WeakStr, color:"#EE3439"}, 
		{content:Msg.ZHONGStr, color:"#FF8E42"}, 
		{content:Msg.StrengthStr, color:"green"}/*, 
		{content:"WPA2-E+AES", color:"green"}, 
		{content:"WPA2-E+TKIP", color:"green"}*/]);
	se_drag.css("width", 175);
	se_drag.data("securitys", [{security:"open", enc_type:"none"}, 
		{security:"wpapsk", enc_type:"ccmp"},
		{security:"wpapsk", enc_type:"tkip"}/*,
		{security:"wpaeap", enc_type:"ccmp"},
		{security:"wpaeap", enc_type:"tkip"}*/]);
	se_drag.bind("set_notch", 
		function(ev, i){
			se_drag.data("security", se_drag.data("securitys")[i]);
			switch(i)
			{
				case 1:
					$("#wpapsk_pass").show();
					$("#wpaeap_pass").hide();
					$("#wpaeap_pass2").hide();
					$("#password-strength").show();
					$("#create-wlan-validity-period").show();
					break;
				case 2:
					$("#wpapsk_pass").hide();
					$("#wpaeap_pass").show();
					$("#wpaeap_pass2").show();
					$("#password-strength").show();
					$("#create-wlan-validity-period").show();
					break;
				// case 3:
				// case 4:
				// 	$("#wpapsk_pass").hide();
				// 	$("#wpaeap_pass").show();
				// 	$("#wpaeap_pass2").show();
				// 	$("#password-strength").show();
				// 	$("#create-wlan-validity-period").show();
				// 	break;
				case 0:
				default:
					$("#wpapsk_pass").hide();
					$("#wpaeap_pass").hide();
					$("#wpaeap_pass2").hide();
					$("#password-strength").hide();
					$("#create-wlan-validity-period").hide();
					break;
			}
		}
	);
	//se_drag.data("notch", 5);
	var op_drag = $("#optimize-drag");
	op_drag.data("textcontent", [{content:"时间间隔(Per 5 mins)"}, 
		{content:"时间间隔(Per 20 mins)"}, 
		{content:"时间间隔(Per 1 hours)"}, 
		{content:"时间间隔(Per 2 hours)"}, 
		{content:"Just By Manual"}]);
	op_drag.css("width", 309);
	//op_drag.data("notch", 5);
	
	var lb_drag = $("#loadbalance-drag");
	lb_drag.data("textcontent", [{content:"用户最多"},
		{content:"范围最广"},
		{content:"带宽最大"},
		{content:"自动优化"}]);
	lb_drag.css("width", 309);
	//lb_drag.data("notch", 5);
	lb_drag.data("adsorb", true);
	var drag = $(".dragging-sliders");
	drag.each(function(){
		var se = $(this);
		se.textContent = se.data("textcontent");
		se.width_all = (parseInt(se.css("width"))-7)||309;
		se.notch = se.data("notch") || se.textContent.length || 3;//5;
		se.ballX = 0;
		se.dmove = false;
		se.notch_len = Math.round(se.width_all/(se.notch-1));
		se.adsorb = se.data("adsorb") || false;
		se.find(".dragging-gra").css("width", se.css("width"));
		//se.find(".dragging-gra").css("backgroundPosition", (parseInt(se.css("width"))-500)+"px 0");
		se.find(".dragging-comment").css("left", (se.width_all+70)+"px");
		se.find(".dragging-comment-se").css("left", (se.width_all+20)+"px");
		se.find(".dragging-comment").html(setlabelcontent(se.textContent[0]));
		se.find(".dragging-comment-se").html(setlabelcontent(se.textContent[0]));
		var objEvt = se.data("events");
		//if(objEvt && objEvt["set_notch"])
		{
			se.trigger("set_notch", 0);
		}
		
		se.find(".dragging-ball").mousedown(function(ev)
		{
			if(!ev) ev = window.event;
			se.ballX = ev.clientX - parseInt($(this).position().left);
			se.dmove = true; 
			$(document).mousemove(se.mousemove);
			$(document).mouseup( se.mouseup);
		});
		
		se.find(".dragging-comment").mousemove(clearSelection);
		se.find(".dragging-comment-se").mousemove(clearSelection);
		se.mousedown = function(ev)
		{
			if(ev==null) ev = window.event;
			
			se.dmove = true;
			$(document).mousemove(se.mousemove);
			$(document).mouseup( se.mouseup);
			var ballLeft = ev.clientX - ev.offsetX;
			se.ballX = ballLeft+10;
			var x = ((ev.clientX - se.ballX) < 0) ? 0 : ((ev.clientX - se.ballX) > se.width_all) ? se.width_all : (ev.clientX - se.ballX);
			var i = Math.round(x/se.notch_len);
			if(se.adsorb)
			{
				x = i*se.notch_len;
			}
			se.find(".dragging-ball").css("left", x+"px");
			se.find(".dragging-comment").html(setlabelcontent(se.textContent[i]));
			se.find(".dragging-comment-se").html(setlabelcontent(se.textContent[i]));
			var objEvt = se.data("events");
			//if(objEvt && objEvt["set_notch"])
			{
				se.trigger("set_notch", i);
			}
			se.find(".dragging-blu").css("width", se.find(".dragging-ball").position().left);
		}
		se.mouseup = function(ev)
		{
			if(ev==null) ev = window.event;
			
			var x = ((ev.clientX - se.ballX) < 0) ? 0 : ((ev.clientX - se.ballX) > se.width_all) ? se.width_all : (ev.clientX - se.ballX);
			var i = Math.round(x/se.notch_len);
			if(se.dmove)
			{
				se.dmove = false;
				$(document).unbind("mousemove", se.mousemove);
				$(document).unbind("mouseup", se.mouseup);
				if(se.adsorb)
				{
					x = i*se.notch_len;
				}
		  		se.find(".dragging-comment").html(setlabelcontent(se.textContent[i]));
		  		se.find(".dragging-comment-se").html(setlabelcontent(se.textContent[i]));
				var objEvt = se.data("events");
				//if(objEvt && objEvt["set_notch"])
				{
					se.trigger("set_notch", i);
				}
				se.find(".dragging-blu").css("width", se.find(".dragging-ball").position().left);
			}
		}
		se.mousemove = function (ev)
		{
		  if(ev==null) ev = window.event;
		  
			var x = ((ev.clientX - se.ballX) < 0) ? 0 : ((ev.clientX - se.ballX) > se.width_all) ? se.width_all : (ev.clientX - se.ballX);
			var i = Math.round(x/se.notch_len);
			if(se.dmove)
			{
				clearSelection(); 
				if(se.adsorb)
				{
					x = i*se.notch_len;
				}
				se.find(".dragging-ball").css("left", x+"px");
				//$("#comment").css("left", (x-80)+"px");
				se.find(".dragging-comment").html(setlabelcontent(se.textContent[i]));
				se.find(".dragging-comment-se").html(setlabelcontent(se.textContent[i]));
				var objEvt = se.data("events");
				//if(objEvt && objEvt["set_notch"])
				{
					se.trigger("set_notch", i);
				}
				se.find(".dragging-blu").css("width", se.find(".dragging-ball").position().left);
			}
		}
		se.find(".dragging-blu").mouseup( se.mouseup);
		se.find(".dragging-gra").mouseup( se.mouseup);
		se.find(".dragging-comment").mouseup( se.mouseup);
		se.find(".dragging-comment-se").mouseup( se.mouseup);
		se.find(".dragging-outer").mouseup( se.mouseup);
		se.find(".dragging-blu").mousedown(se.mousedown);
		se.find(".dragging-gra").mousedown(se.mousedown);
	});	
	var b = "`~!@#$%^&*()[]\\';/.,<>?:\"{}|-=_+ ";
    var charMode = function(f) {
        var e = f.charCodeAt(0);
        if (e >= 48 && e <= 57){
            return 1
        } else {
            if (e >= 65 && e <= 90) {
                return 2
            } else {
                if (e >= 97 && e <= 122) {
                    return 4
                } else {
                    if ( - 1 < b.indexOf(f)){
                        return 8
                    }
                }
            }
        }
        return 0
    };

	var checkPasswordStrength = function(e) {
        var d = 0, f = 0, c = 0;
        for (i = 0; i < e.length; i++) {
            f = charMode(e.charAt(i));
            if (0 == f) {
                return - 1
            }
            if (0 == (d & f)) {
                d |= f; ++c
            }
        }
        return c
    };
	function B(W, minlen, maxlen, obj, tips) {
		var len = W.length;
		if(len > maxlen)
		{
			return false;
		}
        var V = checkPasswordStrength(W);
        if ( - 1 == V)
		{
            return false;
        }
		if(len < minlen)
		{
			obj.hide();
			tips.show();
		}
		else
		{
			obj.show();
			tips.hide();
	        switch (V) {
	        case 2:
	            obj.attr("class", "pswState pswState-normal");
	            break;
	        case 3:
	        case 4:
	            obj.attr("class", "pswState pswState-strong");
	            break;
	        default:
	            obj.attr("class", "pswState pswState-poor");
	            break
	        }
		}
    }
	var objs = $(".security_container");
	objs.data("minlen", 8);
	objs.data("maxlen", 64);
	objs.each(function(){
		var obj = $(this);
		var pass = obj.find("input[type=password]");
		var txt = obj.find("input[type=text]");
		var eyes = obj.find("div.eyes");
		pass.keyup(function()
		{
			return B(pass.val(), obj.data("minlen"), obj.data("maxlen"),
				obj.find(".pswState"), obj.find(".txt-tips"));
		});
		var mouseup = function()
		{
			pass.show();
			txt.hide();
		};
		eyes.mousedown(
			function(){
				pass.hide();
				txt.val(pass.val());
				txt.show();
			}
		);
		eyes.mouseup(mouseup);
		eyes.mouseout(mouseup);
	});
});


