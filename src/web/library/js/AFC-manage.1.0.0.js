$(
    function () {
        $("#model-aps").data("ap_names", {});
        $("#model-aps").jtablemodel(
            {
                url: "/stat_device.php",
                params: {_depth: 2, test: null}
            }
        );
        $("#model-stas").data("sta_names", {});
        $("#model-stas").jtablemodel({url: "/stat_sta.php"});
        $("#model-events").jtablemodel({url: "/stat_event.php"});
		$("#model-uevents").jtablemodel({url: "/stat_event.php"});
        $("#model-maps").jlistmodel(
            {
                url: "/list_map.php",
                params: {_sort: "order"},
                objname: "map"
            }
        );
        $("#model-usergroups").jlistmodel({url: "/list_usergroup.php"});
        $("#model-wlanconfs").jtablemodel({url: "/list_wlanconf.php"});
        $("#model-aps").bind("jtablemodel_refresh-done",
            function (q, j) {
                var o = $("#model-aps").data("ap_names");
                $.each(j._rows,
                    function () {
                        var l = $(this);
                        o[l.attr("mac")] = l.attr("name") || l.attr("mac")
                    }
                )
            }
        );
        $("#model-stas").bind("jtablemodel_refresh-done",
            function (q, j) {
                var o = $("#model-stas").data("sta_names");
                $.each(j._rows,
                    function () {
                        o[this.mac] = this.name || this.hostname || this.mac
                    }
                )
            }
        );
        $("#model-apfilters").jlistmodel(
            {
                tableModel: $("#model-aps"),
                tableModelRowConverter: function (q) {
                    if (!q.adopted)
                        return null;
                    return q
                }
            }
        );
        refreshSysinfo();
        refreshNewVerinfo($("#version_server").val());
        $("#MainPanel").tabs().bind("tabsshow",
            function (q, j) {
            	if(!(j.panel.id)){j.panel.id = j.panel.attr("id");}
                var o = $("#" + j.panel.id).data("fn_shown");
                o && o();
            }
        ).tabs("select", 0);
		var maintabs = ["Overview", "Map", "System", "App", "Support"];
		var tabselected = false;
		for(ii = 0; ii < maintabs.length; ii++)
		{
		  if(top.location.href.match("#"+maintabs[ii]+"$")){
			$("#MainPanel").tabs("select", ii);
			$("#navtab"+(ii+1)).addClass('index-navi-active');
			//alert($("#"+ii).length)
			tabselected = true;
		  }
		}
		if(!tabselected)
		{
			$("#navtab"+1).addClass('index-navi-active');
		}
		
		$("#accessPointTab").click(
			function(){
				$("#AccessPointDataTable")
					.jtable("delfilter", "*")
					.jtable("filter");
			}
		);
        $("#BottomPanelTabs").tabs().bind("tabsshow",
            function (q, j) {
                $(j.panel).height() == 0 && $("#BottomPanelTabContent").animate({height: "show"})&& $("#BottomPanelHideButton").removeClass("hided");
                	//$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_expand.gif) no-repeat");
                j.panel.id == "Settings" && $.jpost("/list_setting.php", {},
                    function (l) {
                        var p = {};
                        $.each(l.data,
                            function (u, k) {
                                p[k.key] = k
                            }
                        );
                        $("#SystemForm").data("fn_fillform")(p);
                        $("#GuestControlForm").data("fn_fillform")(p)
                    }
                );
				var o = $("#" + j.panel.id).data("fn_shown");
				o && o()
		}).tabs("select", 1);
        $("#BottomPanel").animate({bottom: 0}, "slow");
        $("#BottomPanelNavigation a").click(
            function () {
                $("#BottomPanelTabContent").animate({height: "show"});$("#BottomPanelHideButton").removeClass("hided");
                //$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_expand.gif) no-repeat");
            }
        );
        $("#AdminControls a.settings-button").click(
            function () {
                $("#BottomPanelTabs").tabs("select", 2);
                $("#BottomPanelTabContent").animate({height: "show"});$("#BottomPanelHideButton").removeClass("hided");
                //$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_expand.gif) no-repeat");
            }
        );
		var tmpdata = new Array();
		var index = 0;
		$("#wai-info").jtimer({interval: 5}).bind("jtimer_due", function(){
			//alert($(this).val());
			//alert(tmpdata.length);
			if(index < tmpdata.length && index < 10)
			{
				$("#wai-info p.settings-response2").html((index+1)+"/"+tmpdata.length+" "+tmpdata[index]);
				$("#wai-info p.settings-response2").fadeIn(500).delay(($(this).val()-1)*1000).fadeOut(500);
			}
			else
			{
				$("#wai-info p.settings-response2").html(Msg.I_WaiSlogon);
				$("#wai-info p.settings-response2").fadeIn(500);
			}
			index++;
			}).jtimer("start");
        $("#RefreshSelect").jtimer({interval: $.cookie("refreshtime")||30}).bind("jtimer_due",
            function () {
                $("#model-aps").jtablemodel("refresh");
                $("#model-stas").jtablemodel("refresh");
				$.jpost("/wai_info.php", {}, function (e) {
					index = 0;
					tmpdata = e.data;
				});
				//refreshNewVerinfo("update");
            }
        )
		.bind("jtimer_cache-select", function(){
			var sec = Math.floor($(this).val()/tmpdata.length);
			$.cookie("refreshtime", $(this).val(), {expires: 30});
			$("#wai-info").val((sec < 2 ? 2: (sec > 5)?5 : sec));
			$("#wai-info").trigger();
		})
		.jtimer("start");
        $("#RefreshControl a, .system-dialog a").click(
            function () {
                $("#RefreshSelect").jtimer("trigger");
                return false
            }
        );
         $("#model-maintabpagesizer").jlistmodel(
            {
                rows: [
                    {_id: 5},
                    {_id: 10},
                    {_id: 20},
                    {_id: 30},
                    {_id: 50},
                    {_id: 100}
                ],
                selected: $.cookie("maintab.pagesize") || 5
            }
        ).bind("jlistmodel_select-changed",
            function () {
                $.cookie("maintab.pagesize", $(this).jlistmodel("selected"), {expires: 30})
            }
        );
        $("#MainPanel select.table-pagesizer").jselect({model: $("#model-maintabpagesizer"), renderer: function (q, j) {
            return"" + j._id
        }, selectionSynchronized: true});
        $("#model-maintabpagesizer").jlistmodel("refresh");
        //$("a.ap-link").live("click",
         $(document).on("click","a.ap-link",
            function () {
                var q = $(this).attr("title");
				q = q.replace(/ID .* MAC /g, "");
				q = q.toLowerCase();
                $(document.body).dialogManager("addItem", q, "ap");
            });
        //$("a.sta-link").live("click", 
        $(document).on("click", "a.sta-link",
			function () {
            $(document.body).dialogManager("addItem", $(this).attr("title"), "sta")
        });
        //$("span.label").live("click", 
        $(document).on("click","span.label" ,
        	function () {
            var q = $(this).prev();
            if (q.is(".radio-input") || q.is(".check-box-input"))q.click().change()
        });
        //$("span.localizable").live("dblclick", function (q) {
        $(document).on("dblclick","span.localizable" ,function (q) {
            if (q.altKey) {
                q = $('<input class="localizing" type="text" name="text" value="' + $(this).html() + '" key="' + $(this).attr("key") +
                    '" />"');
                $(this).replaceWith(q);
                q.focus().select()
            }
        });
        //$("input.localizing").live("keydown", function (q) {
        $(document).on("keydown","input.localizing", function (q) {
            if (q.which == 13 && $(this).val()) {
                q = $('<span class="localizable" key="' + $(this).attr("key") + '">' + $(this).val() + "</span>");
                $(this).replaceWith(q)
            }
        });
        $("#ui-locate-blinker").jtimer({interval: 0.5}).bind("jtimer_due", function () {
            $(".locate-button.locating").toggleClass("on")
        });
        $("#model-aps").bind("jtablemodel_refresh-done", function (q, j) {
            $("#ui-locate-blinker").jtimer("stop");
            $.each(j._rows, function (o, l) {
                if (l.locating) {
                    $("#ui-locate-blinker").jtimer("start");
                    return false
                }
            })
        });
        $("#model-maps").jlistmodel("refresh");
        $("#model-usergroups").jlistmodel("refresh");
        $("#model-wlanconfs").jtablemodel("refresh");
        $("#BottomPanelHideButton").click(
            function () {
                $("#BottomPanelTabContent").animate({height: "toggle"})
                if($("#BottomPanelTabContent").height() > 1)
                {
                	//$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_collapse.gif) no-repeat");
                	$("#BottomPanelHideButton").addClass("hided")
                }
                else
                {              	
                	//$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_expand.gif) no-repeat");
                	$("#BottomPanelHideButton").removeClass("hided")
                }
            }
        );
        closeBottomPanel();
        $(window).keydown(
            function (q) {
                q.which == 27 && closeBottomPanel()
            }
        );
        $(document.body).dialogManager(
            {
                apTemplate: "div.ap-dialog",
                staTemplate: "div.sta-dialog"
            }
        );
        var e = $(".system-dialog");
        e.dialog
        (
            {
                autoOpen: false,
                modal: true,
                draggable: false,
                resizable: false,
                closeOnEscape: false,
                width: 400,
                height: 250,
                closeText: "",
                dialogClass: "system-warning-dialog"
            }
        );
        var c = false;
        $.jpost.fnReflectConnectionStatus = function (q) {
	        //if (!$.jpost.session_expired)
	        //{
			    	//if (!q && !c) {
		                //c = true;
		                //e.dialog("open")
		        //}
		        //else if (c && q) {
		                //c = false;
		                //e.dialog("close")
		        //}
	        //}
	     }	     
    });

function closeBottomPanel() {
    $("#BottomPanelTabContent").animate({height: "hide"})
    //$("#BottomPanelHideButton").css("background-position", "0px 0px");
    //$("#BottomPanelHideButton").css("background", "url(../../media/global/panel_tool_collapse.gif) no-repeat");
    $("#BottomPanelHideButton").addClass("hided");
}
function refreshSysinfo() {

    $.jpost("/stat_sysinfo.php", {}, function (e) {
        $(document.body).data("sysinfo", e.data[0]);
        var c = e.data[0].cur_ccode, q = $('div.ap-dialog .ap-ng-configs select[name="channel"]');
        e = $('<option value="0">'+Msg.AutoStr+'</option>');
        q.empty().append(e);
        $.each(c.channels_ng, function (o, l) {
            $('<option value="' + l + '">' + l + "</option>").appendTo(q)
        });
        var j = $('div.ap-dialog .ap-na-configs select[name="channel"]');
        $.each(c.channels_na, function (o, l) {
            var p = l < 52 ? 1 : l < 149 ? 2 : 3, u = $.inArray(l, c.channels_na_dfs) >= 0;
            $('<option value="' +
                l + '" band="' + p + '">' + l + (u ? " (dfs)" : "") + "</option>").appendTo(j)
        });
		
        var q = $(document.body).data("sysinfo");
        q && $("#ServerVersion").html(q.version);
    })
}

function refreshNewVerinfo(version_server) {
	var param = {};
	if(version_server == "update")
	{
		param = {update: true, server: $("#version_server").val()};
	}
	else if(version_server)
	{
		param = {server: version_server};
	}
    $.jpost("/stat_newverinfo.php", param, function (e) {
        $(document.body).data("newverinfo", e.data);
		
        var q = e.data;
        //q && $("#ServerVersion").html(q.version);
        if (q && q.upgrade_status && q.version != q.update_version/* && q.update_downloaded*/) {
            $("#UpgradeVersion").html(V.localize(Msg.UpgradingToNewVersion, 
				q.update_version, $("#version_server").val()));
            $("#UpgradeVersion").css("display", "inline");
            $("#UpgradeButton").css("display", "none");
			$("#version_server").disable();
        }
		else if (q && q.update_available/* && q.update_downloaded*/) {
            $("#UpgradeVersion").html(V.localize(Msg.UpdateAvaliable, q.update_version));
            $("#UpgradeVersion, #UpgradeButton").css("display", "inline");
			$("#version_server").enable();
        }
		else
		{
			$("#UpgradeVersion").html("");
			$("#UpgradeVersion, #UpgradeButton").css("display", "none");
			$("#version_server").enable();
		}
    })
}
var swfobject = function () {
    function e() {
        if (!za) {
            try {
                var v = N.getElementsByTagName("body")[0].appendChild(N.createElement("span"));
                v.parentNode.removeChild(v)
            } catch (A) {
                return
            }
            za = true;
            v = ua.length;
            for (var E = 0; E < v; E++)ua[E]()
        }
    }

    function c(v) {
        if (za)v(); else ua[ua.length] = v
    }

    function q(v) {
        if (typeof aa.addEventListener != ea)aa.addEventListener("load", v, false); else if (typeof N.addEventListener != ea)N.addEventListener("load", v, false); else if (typeof aa.attachEvent != ea)K(aa, "onload", v); else if (typeof aa.onload == "function") {
            var A =
                aa.onload;
            aa.onload = function () {
                A();
                v()
            }
        } else aa.onload = v
    }

    function j() {
        var v = N.getElementsByTagName("body")[0], A = N.createElement(Q);
        A.setAttribute("type", ca);
        var E = v.appendChild(A);
        if (E) {
            var R = 0;
            (function () {
                if (typeof E.GetVariable != ea) {
                    var U = E.GetVariable("$version");
                    if (U) {
                        U = U.split(" ")[1].split(",");
                        W.pv = [parseInt(U[0], 10), parseInt(U[1], 10), parseInt(U[2], 10)]
                    }
                } else if (R < 10) {
                    R++;
                    setTimeout(arguments.callee, 10);
                    return
                }
                v.removeChild(A);
                E = null;
                o()
            })()
        } else o()
    }

    function o() {
        var v = va.length;
        if (v > 0)for (var A =
            0; A < v; A++) {
            var E = va[A].id, R = va[A].callbackFn, U = {success: false, id: E};
            if (W.pv[0] > 0) {
                var Z = D(E);
                if (Z)if (C(va[A].swfVersion) && !(W.wk && W.wk < 312)) {
                    O(E, true);
                    if (R) {
                        U.success = true;
                        U.ref = l(E);
                        R(U)
                    }
                } else if (va[A].expressInstall && p()) {
                    U = {};
                    U.data = va[A].expressInstall;
                    U.width = Z.getAttribute("width") || "0";
                    U.height = Z.getAttribute("height") || "0";
                    if (Z.getAttribute("class"))U.styleclass = Z.getAttribute("class");
                    if (Z.getAttribute("align"))U.align = Z.getAttribute("align");
                    var B = {};
                    Z = Z.getElementsByTagName("param");
                    for (var ka =
                        Z.length, M = 0; M < ka; M++)if (Z[M].getAttribute("name").toLowerCase() != "movie")B[Z[M].getAttribute("name")] = Z[M].getAttribute("value");
                    u(U, B, E, R)
                } else {
                    k(Z);
                    R && R(U)
                }
            } else {
                O(E, true);
                if (R) {
                    if ((E = l(E)) && typeof E.SetVariable != ea) {
                        U.success = true;
                        U.ref = E
                    }
                    R(U)
                }
            }
        }
    }

    function l(v) {
        var A = null;
        if ((v = D(v)) && v.nodeName == "OBJECT")if (typeof v.SetVariable != ea)A = v; else if (v = v.getElementsByTagName(Q)[0])A = v;
        return A
    }

    function p() {
        return!Ka && C("6.0.65") && (W.win || W.mac) && !(W.wk && W.wk < 312)
    }

    function u(v, A, E, R) {
        Ka = true;
        da = R || null;
        ia = {success: false, id: E};
        var U = D(E);
        if (U) {
            if (U.nodeName == "OBJECT") {
                La = z(U);
                I = null
            } else {
                La = U;
                I = E
            }
            v.id = fa;
            if (typeof v.width == ea || !/%$/.test(v.width) && parseInt(v.width, 10) < 310)v.width = "310";
            if (typeof v.height == ea || !/%$/.test(v.height) && parseInt(v.height, 10) < 137)v.height = "137";
            N.title = N.title.slice(0, 47) + " - Flash Player Installation";
            R = W.ie && W.win ? "ActiveX" : "PlugIn";
            R = "MMredirectURL=" + aa.location.toString().replace(/&/g, "%26") + "&MMplayerType=" + R + "&MMdoctitle=" + N.title;
            if (typeof A.flashvars != ea)A.flashvars +=
                "&" + R; else A.flashvars = R;
            if (W.ie && W.win && U.readyState != 4) {
                R = N.createElement("div");
                E += "SWFObjectNew";
                R.setAttribute("id", E);
                U.parentNode.insertBefore(R, U);
                U.style.display = "none";
                (function () {
                    U.readyState == 4 ? U.parentNode.removeChild(U) : setTimeout(arguments.callee, 10)
                })()
            }
            G(v, A, E)
        }
    }

    function k(v) {
        if (W.ie && W.win && v.readyState != 4) {
            var A = N.createElement("div");
            v.parentNode.insertBefore(A, v);
            A.parentNode.replaceChild(z(v), A);
            v.style.display = "none";
            (function () {
                v.readyState == 4 ? v.parentNode.removeChild(v) : setTimeout(arguments.callee,
                    10)
            })()
        } else v.parentNode.replaceChild(z(v), v)
    }

    function z(v) {
        var A = N.createElement("div");
        if (W.win && W.ie)A.innerHTML = v.innerHTML; else if (v = v.getElementsByTagName(Q)[0])if (v = v.childNodes)for (var E = v.length, R = 0; R < E; R++)!(v[R].nodeType == 1 && v[R].nodeName == "PARAM") && v[R].nodeType != 8 && A.appendChild(v[R].cloneNode(true));
        return A
    }

    function G(v, A, E) {
        var R, U = D(E);
        if (W.wk && W.wk < 312)return R;
        if (U) {
            if (typeof v.id == ea)v.id = E;
            if (W.ie && W.win) {
                var Z = "";
                for (var B in v)if (v[B] != Object.prototype[B])if (B.toLowerCase() ==
                    "data")A.movie = v[B]; else if (B.toLowerCase() == "styleclass")Z += ' class="' + v[B] + '"'; else if (B.toLowerCase() != "classid")Z += " " + B + '="' + v[B] + '"';
                B = "";
                for (var ka in A)if (A[ka] != Object.prototype[ka])B += '<param name="' + ka + '" value="' + A[ka] + '" />';
                U.outerHTML = '<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"' + Z + ">" + B + "</object>";
                wa[wa.length] = v.id;
                R = D(v.id)
            } else {
                ka = N.createElement(Q);
                ka.setAttribute("type", ca);
                for (var M in v)if (v[M] != Object.prototype[M])if (M.toLowerCase() == "styleclass")ka.setAttribute("class",
                    v[M]); else M.toLowerCase() != "classid" && ka.setAttribute(M, v[M]);
                for (Z in A)if (A[Z] != Object.prototype[Z] && Z.toLowerCase() != "movie") {
                    v = ka;
                    B = Z;
                    M = A[Z];
                    E = N.createElement("param");
                    E.setAttribute("name", B);
                    E.setAttribute("value", M);
                    v.appendChild(E)
                }
                U.parentNode.replaceChild(ka, U);
                R = ka
            }
        }
        return R
    }

    function w(v) {
        var A = D(v);
        if (A && A.nodeName == "OBJECT")if (W.ie && W.win) {
            A.style.display = "none";
            (function () {
                if (A.readyState == 4) {
                    var E = D(v);
                    if (E) {
                        for (var R in E)if (typeof E[R] == "function")E[R] = null;
                        E.parentNode.removeChild(E)
                    }
                } else setTimeout(arguments.callee,
                    10)
            })()
        } else A.parentNode.removeChild(A)
    }

    function D(v) {
        var A = null;
        try {
            A = N.getElementById(v)
        } catch (E) {
        }
        return A
    }

    function K(v, A, E) {
        v.attachEvent(A, E);
        la[la.length] = [v, A, E]
    }

    function C(v) {
        var A = W.pv;
        v = v.split(".");
        v[0] = parseInt(v[0], 10);
        v[1] = parseInt(v[1], 10) || 0;
        v[2] = parseInt(v[2], 10) || 0;
        return A[0] > v[0] || A[0] == v[0] && A[1] > v[1] || A[0] == v[0] && A[1] == v[1] && A[2] >= v[2] ? true : false
    }

    function H(v, A, E, R) {
        if (!(W.ie && W.mac)) {
            var U = N.getElementsByTagName("head")[0];
            if (U) {
                E = E && typeof E == "string" ? E : "screen";
                if (R)qa = X =
                    null;
                if (!X || qa != E) {
                    R = N.createElement("style");
                    R.setAttribute("type", "text/css");
                    R.setAttribute("media", E);
                    X = U.appendChild(R);
                    if (W.ie && W.win && typeof N.styleSheets != ea && N.styleSheets.length > 0)X = N.styleSheets[N.styleSheets.length - 1];
                    qa = E
                }
                if (W.ie && W.win)X && typeof X.addRule == Q && X.addRule(v, A); else X && typeof N.createTextNode != ea && X.appendChild(N.createTextNode(v + " {" + A + "}"))
            }
        }
    }

    function O(v, A) {
        if (xa) {
            var E = A ? "visible" : "hidden";
            if (za && D(v))D(v).style.visibility = E; else H("#" + v, "visibility:" + E)
        }
    }

    function P(v) {
        return/[\\\"<>\.;]/.exec(v) !=
            null && typeof encodeURIComponent != ea ? encodeURIComponent(v) : v;//"
    }

    var ea = "undefined", Q = "object", ca = "application/x-shockwave-flash", fa = "SWFObjectExprInst", aa = window, N = document, ha = navigator, oa = false, ua = [function () {
        oa ? j() : o()
    }], va = [], wa = [], la = [], La, I, da, ia, za = false, Ka = false, X, qa, xa = true, W = function () {
        var v = typeof N.getElementById != ea && typeof N.getElementsByTagName != ea && typeof N.createElement != ea, A = ha.userAgent.toLowerCase(), E = ha.platform.toLowerCase(), R = E ? /win/.test(E) : /win/.test(A);
        E = E ? /mac/.test(E) : /mac/.test(A);
        A = /webkit/.test(A) ? parseFloat(A.replace(/^.*webkit\/(\d+(\.\d+)?).*$/, "$1")) : false;
        var U = !+"\u000b1", Z = [0, 0, 0], B = null;
        if (typeof ha.plugins != ea && typeof ha.plugins["Shockwave Flash"] == Q) {
            if ((B = ha.plugins["Shockwave Flash"].description) && !(typeof ha.mimeTypes != ea && ha.mimeTypes[ca] && !ha.mimeTypes[ca].enabledPlugin)) {
                oa = true;
                U = false;
                B = B.replace(/^.*\s+(\S+\s+\S+$)/, "$1");
                Z[0] = parseInt(B.replace(/^(.*)\..*$/, "$1"), 10);
                Z[1] = parseInt(B.replace(/^.*\.(.*)\s.*$/, "$1"), 10);
                Z[2] = /[a-zA-Z]/.test(B) ? parseInt(B.replace(/^.*[a-zA-Z]+(.*)$/,
                    "$1"), 10) : 0
            }
        } else if (typeof aa.ActiveXObject != ea)try {
            var ka = new ActiveXObject("ShockwaveFlash.ShockwaveFlash");
            if (ka)if (B = ka.GetVariable("$version")) {
                U = true;
                B = B.split(" ")[1].split(",");
                Z = [parseInt(B[0], 10), parseInt(B[1], 10), parseInt(B[2], 10)]
            }
        } catch (M) {
        }
        return{w3: v, pv: Z, wk: A, ie: U, win: R, mac: E}
    }();
    (function () {
        if (W.w3) {
            if (typeof N.readyState != ea && N.readyState == "complete" || typeof N.readyState == ea && (N.getElementsByTagName("body")[0] || N.body)){e();}
            if (!za) {
                typeof N.addEventListener != ea && N.addEventListener("DOMContentLoaded",
                    e, false);
                if (W.ie && W.win) {
                    N.attachEvent("onreadystatechange", function () {
                        if (N.readyState == "complete") {
                            N.detachEvent("onreadystatechange", arguments.callee);
                            {e()}
                        }
                    });
                    aa == top && function () {
                        if (!za) {
                            try {
                                N.documentElement.doScroll("left")
                            } catch (v) {
                                setTimeout(arguments.callee, 0);
                                return
                            }
                            {e()}
                        }
                    }()
                }
                W.wk && function () {
                    za || (/loaded|complete/.test(N.readyState) ? e() : setTimeout(arguments.callee, 0))
                }();
                q(e)
            }
        }
    })();
    (function () {
        W.ie && W.win && window.attachEvent("onunload", function () {
            for (var v = la.length, A = 0; A < v; A++)la[A][0].detachEvent(la[A][1],
                la[A][2]);
            v = wa.length;
            for (A = 0; A < v; A++)w(wa[A]);
            for (var E in W)W[E] = null;
            W = null;
            for (var R in swfobject)swfobject[R] = null;
            swfobject = null
        })
    })();
    return{registerObject: function (v, A, E, R) {
        if (W.w3 && v && A) {
            var U = {};
            U.id = v;
            U.swfVersion = A;
            U.expressInstall = E;
            U.callbackFn = R;
            va[va.length] = U;
            O(v, false)
        } else R && R({success: false, id: v})
    }, getObjectById: function (v) {
        if (W.w3)return l(v)
    }, embedSWF: function (v, A, E, R, U, Z, B, ka, M, Ca) {
        var Ga = {success: false, id: A};
        if (W.w3 && !(W.wk && W.wk < 312) && v && A && E && R && U) {
            O(A, false);
            c(function () {
                E +=
                    "";
                R += "";
                var pa = {};
                if (M && typeof M === Q)for (var Da in M)pa[Da] = M[Da];
                pa.data = v;
                pa.width = E;
                pa.height = R;
                Da = {};
                if (ka && typeof ka === Q)for (var Ba in ka)Da[Ba] = ka[Ba];
                if (B && typeof B === Q)for (var sa in B)if (typeof Da.flashvars != ea)Da.flashvars += "&" + sa + "=" + B[sa]; else Da.flashvars = sa + "=" + B[sa];
                if (C(U)) {
                    Ba = G(pa, Da, A);
                    pa.id == A && O(A, true);
                    Ga.success = true;
                    Ga.ref = Ba
                } else if (Z && p()) {
                    pa.data = Z;
                    u(pa, Da, A, Ca);
                    return
                } else O(A, true);
                Ca && Ca(Ga)
            })
        } else Ca && Ca(Ga)
    }, switchOffAutoHideShow: function () {
        xa = false
    }, ua: W, getFlashPlayerVersion: function () {
        return{major: W.pv[0],
            minor: W.pv[1], release: W.pv[2]}
    }, hasFlashPlayerVersion: C, createSWF: function (v, A, E) {
        if (W.w3)return G(v, A, E)
    }, showExpressInstall: function (v, A, E, R) {
        W.w3 && p() && u(v, A, E, R)
    }, removeSWF: function (v) {
        W.w3 && w(v)
    }, createCSS: function (v, A, E, R) {
        W.w3 && H(v, A, E, R)
    }, addDomLoadEvent: c, addLoadEvent: q, getQueryParamValue: function (v) {
        var A = N.location.search || N.location.hash;
        if (A) {
            if (/\?/.test(A))A = A.split("?")[1];
            if (v == null)return P(A);
            A = A.split("&");
            for (var E = 0; E < A.length; E++)if (A[E].substring(0, A[E].indexOf("=")) == v)return P(A[E].substring(A[E].indexOf("=") +
                1))
        }
        return""
    }, expressInstallCallback: function () {
        if (Ka) {
            var v = D(fa);
            if (v && La) {
                v.parentNode.replaceChild(La, v);
                if (I) {
                    O(I, true);
                    if (W.ie && W.win)La.style.display = "block"
                }
                da && da(ia)
            }
            Ka = false
        }
    }}
}();
Raphael = function () {
    function e() {
        if (e.is(arguments[0], Ba)) {
            for (var a = arguments[0], b = Za[qa](e, a.splice(0, 3 + e.is(a[0], pa))), f = b.set(), h = 0, i = a[B]; h < i; h++) {
                var g = a[h] || {};
                La.test(g.type) && f[na](b[g.type]().attr(g))
            }
            return f
        }
        return Za[qa](e, arguments)
    }

    function c() {
    }

    function q(a) {
        if (e.vml) {
            var b = /^\s+|\s+$/g;
            q = l(function (h) {
                var i;
                h = (h + v)[ta](b, v);
                try {
                    var g = new za.ActiveXObject("htmlfile");
                    g.write("<body>");
                    g.close();
                    i = g.body
                } catch (m) {
                    i = za.createPopup().document.body
                }
                g = i.createTextRange();
                try {
                    i.style.color =
                        h;
                    var n = g.queryCommandValue("ForeColor");
                    n = (n & 255) << 16 | n & 65280 | (n & 16711680) >>> 16;
                    return"#" + ("000000" + n[sa](16)).slice(-6)
                } catch (s) {
                    return"none"
                }
            })
        } else {
            var f = ia.createElement("i");
            f.title = "Rapha\u00ebl Colour Picker";
            f.style.display = "none";
            ia.body[X](f);
            q = l(function (h) {
                f.style.color = h;
                return ia.defaultView.getComputedStyle(f, v).getPropertyValue("color")
            })
        }
        return q(a)
    }

    function j() {
        return"hsb(" + [this.h, this.s, this.b] + ")"
    }

    function o() {
        return this.hex
    }

    function l(a, b, f) {
        function h() {
            var i = Array[I].slice.call(arguments,
                0), g = i[Z]("\u25ba"), m = h.cache = h.cache || {}, n = h.count = h.count || [];
            if (m[da](g))return f ? f(m[g]) : m[g];
            n[B] >= 1E3 && delete m[n.shift()];
            n[na](g);
            m[g] = a[qa](b, i);
            return f ? f(m[g]) : m[g]
        }

        return h
    }

    function p(a) {
        var b = [];
        if (!e.is(a, Ba) || !e.is(a && a[0], Ba))a = e.parsePathString(a);
        for (var f = 0, h = a[B]; f < h; f++) {
            b[f] = [];
            for (var i = 0, g = a[f][B]; i < g; i++)b[f][i] = a[f][i]
        }
        b[sa] = e._path2string;
        return b
    }

    function u(a, b, f, h, i, g) {
        var m = 1 / 3, n = 2 / 3;
        return[m * a + n * f, m * b + n * h, m * i + n * f, m * g + n * h, i, g]
    }

    function k(a, b, f, h, i, g, m, n, s, t) {
        var y = M.PI,
            F = y * 120 / 180, L = y / 180 * (+i || 0), J = [], x, S = l(function (ya, Ja, Sa) {
                var sb = ya * M.cos(Sa) - Ja * M.sin(Sa);
                ya = ya * M.sin(Sa) + Ja * M.cos(Sa);
                return{x: sb, y: ya}
            });
        if (t) {
            ba = t[0];
            x = t[1];
            g = t[2];
            ja = t[3]
        } else {
            x = S(a, b, -L);
            a = x.x;
            b = x.y;
            x = S(n, s, -L);
            n = x.x;
            s = x.y;
            M.cos(y / 180 * i);
            M.sin(y / 180 * i);
            x = (a - n) / 2;
            ba = (b - s) / 2;
            ja = x * x / (f * f) + ba * ba / (h * h);
            if (ja > 1) {
                ja = M.sqrt(ja);
                f = ja * f;
                h = ja * h
            }
            ja = f * f;
            var Aa = h * h;
            ja = (g == m ? -1 : 1) * M.sqrt(M.abs((ja * Aa - ja * ba * ba - Aa * x * x) / (ja * ba * ba + Aa * x * x)));
            g = ja * f * ba / h + (a + n) / 2;
            var ja = ja * -h * x / f + (b + s) / 2, ba = M.asin(((b - ja) / h).toFixed(7));
            x = M.asin(((s - ja) / h).toFixed(7));
            ba = a < g ? y - ba : ba;
            x = n < g ? y - x : x;
            ba < 0 && (ba = y * 2 + ba);
            x < 0 && (x = y * 2 + x);
            if (m && ba > x)ba -= y * 2;
            if (!m && x > ba)x -= y * 2
        }
        y = x - ba;
        if (M.abs(y) > F) {
            J = x;
            y = n;
            Aa = s;
            x = ba + F * (m && x > ba ? 1 : -1);
            n = g + f * M.cos(x);
            s = ja + h * M.sin(x);
            J = k(n, s, f, h, i, 0, m, y, Aa, [x, J, g, ja])
        }
        y = x - ba;
        i = M.cos(ba);
        g = M.sin(ba);
        m = M.cos(x);
        x = M.sin(x);
        y = M.tan(y / 4);
        f = 4 / 3 * f * y;
        y = 4 / 3 * h * y;
        h = [a, b];
        a = [a + f * g, b - y * i];
        b = [n + f * x, s - y * m];
        n = [n, s];
        a[0] = 2 * h[0] - a[0];
        a[1] = 2 * h[1] - a[1];
        if (t)return[a, b, n][xa](J); else {
            J = [a, b, n][xa](J)[Z]()[E](",");
            t = [];
            n = 0;
            for (s = J[B]; n <
                s; n++)t[n] = n % 2 ? S(J[n - 1], J[n], L).y : S(J[n], J[n + 1], L).x;
            return t
        }
    }

    function z(a, b, f, h, i, g, m, n, s) {
        var t = 1 - s;
        return{x: ma(t, 3) * a + ma(t, 2) * 3 * s * f + t * 3 * s * s * i + ma(s, 3) * m, y: ma(t, 3) * b + ma(t, 2) * 3 * s * h + t * 3 * s * s * g + ma(s, 3) * n}
    }

    function G(a, b, f, h) {
        if (e.is(a, Da) || e.is(a, "object")) {
            a = e.is(a, Da) ? ia.getElementById(a) : a;
            if (a.tagName)return b == null ? {container: a, width: a.style.pixelWidth || a.offsetWidth, height: a.style.pixelHeight || a.offsetHeight} : {container: a, width: b, height: f}
        } else return{container: 1, x: a, y: b, width: f, height: h}
    }

    function w(a, b) {
        var f = this;
        for (var h in b)if (b[da](h) && !(h in a))switch (typeof b[h]) {
            case "function":
                (function (i) {
                    a[h] = a === f ? i : function () {
                        return i[qa](f, arguments)
                    }
                })(b[h]);
                break;
            case "object":
                a[h] = a[h] || {};
                w.call(this, a[h], b[h]);
                break;
            default:
                a[h] = b[h];
                break
        }
    }

    function D(a, b) {
        a == b.top && (b.top = a.prev);
        a == b.bottom && (b.bottom = a.next);
        a.next && (a.next.prev = a.prev);
        a.prev && (a.prev.next = a.next)
    }

    function K(a, b) {
        if (b.top !== a) {
            D(a, b);
            a.next = null;
            a.prev = b.top;
            b.top.next = a;
            b.top = a
        }
    }

    function C(a, b) {
        if (b.bottom !== a) {
            D(a, b);
            a.next = b.bottom;
            a.prev = null;
            b.bottom.prev = a;
            b.bottom = a
        }
    }

    function H(a, b, f) {
        D(a, f);
        b == f.top && (f.top = a);
        b.next && (b.next.prev = a);
        a.next = b.next;
        a.prev = b;
        b.next = a
    }

    function O(a, b, f) {
        D(a, f);
        b == f.bottom && (f.bottom = a);
        b.prev && (b.prev.next = a);
        a.prev = b.prev;
        b.prev = a;
        a.next = b
    }

    function P(a) {
        return function () {
            throw Error("Rapha\u00ebl: you are calling to method \u201c" + a + "\u201d of removed object");
        }
    }

    function ea() {
        this.returnValue = false
    }

    function Q() {
        return this.originalEvent.preventDefault()
    }

    function ca() {
        this.cancelBubble =
            true
    }

    function fa() {
        return this.originalEvent.stopPropagation()
    }

    function aa() {
        return this.x + A + this.y
    }

    function N(a, b) {
        return function (f, h, i) {
            f = Ta(f);
            for (var g, m, n, s, t = "", y = {}, F = 0, L = 0, J = f.length; L < J; L++) {
                n = f[L];
                if (n[0] == "M") {
                    g = +n[1];
                    m = +n[2]
                } else {
                    s = tb(g, m, n[1], n[2], n[3], n[4], n[5], n[6]);
                    if (F + s > h) {
                        if (b && !y.start) {
                            g = db(g, m, n[1], n[2], n[3], n[4], n[5], n[6], h - F);
                            t += ["C", g.start.x, g.start.y, g.m.x, g.m.y, g.x, g.y];
                            if (i)return t;
                            y.start = t;
                            t = ["M", g.x, g.y + "C", g.n.x, g.n.y, g.end.x, g.end.y, n[5], n[6]][Z]();
                            F += s;
                            g = +n[5];
                            m = +n[6];
                            continue
                        }
                        if (!a && !b) {
                            g = db(g, m, n[1], n[2], n[3], n[4], n[5], n[6], h - F);
                            return{x: g.x, y: g.y, alpha: g.alpha}
                        }
                    }
                    F += s;
                    g = +n[5];
                    m = +n[6]
                }
                t += n
            }
            y.end = t;
            g = a ? F : b ? y : e.findDotsAtSegment(g, m, n[1], n[2], n[3], n[4], n[5], n[6], 1);
            g.alpha && (g = {x: g.x, y: g.y, alpha: g.alpha});
            return g
        }
    }

    function ha() {
        var a = +new Date;
        for (var b in Fa)if (b != "length" && Fa[da](b)) {
            var f = Fa[b];
            if (f.stop || f.el.removed) {
                delete Fa[b];
                Fa[B]--
            } else {
                var h = a - f.start, i = f.ms, g = f.easing, m = f.from, n = f.diff, s = f.to, t = f.t, y = f.prev || 0, F = f.el, L = f.callback, J = {}, x;
                if (h <
                    i) {
                    L = e.easing_formulas[g] ? e.easing_formulas[g](h / i) : h / i;
                    for (var S in m)if (m[da](S)) {
                        switch ($a[S]) {
                            case "along":
                                x = L * i * n[S];
                                s.back && (x = s.len - x);
                                g = Ua(s[S], x);
                                F.translate(n.sx - n.x || 0, n.sy - n.y || 0);
                                n.x = g.x;
                                n.y = g.y;
                                F.translate(g.x - n.sx, g.y - n.sy);
                                s.rot && F.rotate(n.r + g.alpha, g.x, g.y);
                                break;
                            case pa:
                                x = +m[S] + L * i * n[S];
                                break;
                            case "colour":
                                x = "rgb(" + [oa(ra(m[S].r + L * i * n[S].r)), oa(ra(m[S].g + L * i * n[S].g)), oa(ra(m[S].b + L * i * n[S].b))][Z](",") + ")";
                                break;
                            case "path":
                                x = [];
                                g = 0;
                                for (var Aa = m[S][B]; g < Aa; g++) {
                                    x[g] = [m[S][g][0]];
                                    for (var ja = 1, ba = m[S][g][B]; ja < ba; ja++)x[g][ja] = +m[S][g][ja] + L * i * n[S][g][ja];
                                    x[g] = x[g][Z](A)
                                }
                                x = x[Z](A);
                                break;
                            case "csv":
                                switch (S) {
                                    case "translation":
                                        x = n[S][0] * (h - y);
                                        g = n[S][1] * (h - y);
                                        t.x += x;
                                        t.y += g;
                                        x = x + A + g;
                                        break;
                                    case "rotation":
                                        x = +m[S][0] + L * i * n[S][0];
                                        m[S][1] && (x += "," + m[S][1] + "," + m[S][2]);
                                        break;
                                    case "scale":
                                        x = [+m[S][0] + L * i * n[S][0], +m[S][1] + L * i * n[S][1], 2 in s[S] ? s[S][2] : v, 3 in s[S] ? s[S][3] : v][Z](A);
                                        break;
                                    case "clip-rect":
                                        x = [];
                                        for (g = 4; g--;)x[g] = +m[S][g] + L * i * n[S][g];
                                        break
                                }
                                break
                        }
                        J[S] = x
                    }
                    F.attr(J);
                    F._run && F._run.call(F)
                } else {
                    if (s.along) {
                        g =
                            Ua(s.along, s.len * !s.back);
                        F.translate(n.sx - (n.x || 0) + g.x - n.sx, n.sy - (n.y || 0) + g.y - n.sy);
                        s.rot && F.rotate(n.r + g.alpha, g.x, g.y)
                    }
                    (t.x || t.y) && F.translate(-t.x, -t.y);
                    s.scale && (s.scale += v);
                    F.attr(s);
                    delete Fa[b];
                    Fa[B]--;
                    F.in_animation = null;
                    e.is(L, "function") && L.call(F)
                }
                f.prev = h
            }
        }
        e.svg && F && F.paper && F.paper.safari();
        Fa[B] && za.setTimeout(ha)
    }

    function oa(a) {
        return Ca(Ga(a, 255), 0)
    }

    function ua(a, b) {
        if (a == null)return{x: this._.tx, y: this._.ty, toString: aa};
        this._.tx += +a;
        this._.ty += +b;
        switch (this.type) {
            case "circle":
            case "ellipse":
                this.attr({cx: +a +
                    this.attrs.cx, cy: +b + this.attrs.cy});
                break;
            case "rect":
            case "image":
            case "text":
                this.attr({x: +a + this.attrs.x, y: +b + this.attrs.y});
                break;
            case "path":
                var f = eb(this.attrs.path);
                f[0][1] += +a;
                f[0][2] += +b;
                this.attr({path: f});
                break
        }
        return this
    }

    function va(a) {
        return function (b, f, h, i) {
            var g = {back: a};
            e.is(h, "function") ? i = h : g.rot = h;
            b && b.constructor == T && (b = b.attrs.path);
            b && (g.along = b);
            return this.animate(g, f, i)
        }
    }

    function wa(a) {
        this.items = [];
        this[B] = 0;
        this.type = "set";
        if (a)for (var b = 0, f = a[B]; b < f; b++)if (a[b] && (a[b].constructor ==
            T || a[b].constructor == wa)) {
            this[this.items[B]] = this.items[this.items[B]] = a[b];
            this[B]++
        }
    }

    e.version = "1.4.3";
    var la = /[, ]+/, La = /^(circle|rect|path|ellipse|text|image)$/, I = "prototype", da = "hasOwnProperty", ia = document, za = window, Ka = {was: Object[I][da].call(za, "Raphael"), is: za.Raphael}, X = "appendChild", qa = "apply", xa = "concat", W = "createTouch"in ia, v = "", A = " ", E = "split", R = "click dblclick mousedown mousemove mouseout mouseover mouseup touchstart touchmove touchend orientationchange touchcancel gesturestart gesturechange gestureend"[E](A),
        U = {mousedown: "touchstart", mousemove: "touchmove", mouseup: "touchend"}, Z = "join", B = "length", ka = String[I].toLowerCase, M = Math, Ca = M.max, Ga = M.min, pa = "number", Da = "string", Ba = "array", sa = "toString", ub = Object[I][sa], ma = M.pow, na = "push", Ma = /^(?=[\da-f]$)/, fb = /^url\(['"]?([^\)]+?)['"]?\)$/i, vb = /^\s*((#[a-f\d]{6})|(#[a-f\d]{3})|rgba?\(\s*([\d\.]+\s*,\s*[\d\.]+\s*,\s*[\d\.]+(?:\s*,\s*[\d\.]+)?)\s*\)|rgba?\(\s*([\d\.]+%\s*,\s*[\d\.]+%\s*,\s*[\d\.]+%(?:\s*,\s*[\d\.]+%))\s*\)|hs[bl]\(\s*([\d\.]+\s*,\s*[\d\.]+\s*,\s*[\d\.]+)\s*\)|hs[bl]\(\s*([\d\.]+%\s*,\s*[\d\.]+%\s*,\s*[\d\.]+%)\s*\))\s*$/i,
        ra = M.round, ga = parseFloat, Ia = parseInt, Va = String[I].toUpperCase, Wa = {blur: 0, "clip-rect": "0 0 1e9 1e9", cursor: "default", cx: 0, cy: 0, fill: "#fff", "fill-opacity": 1, font: '10px "Arial"', "font-family": '"Arial"', "font-size": "10", "font-style": "normal", "font-weight": 400, gradient: 0, height: 0, href: "http://raphaeljs.com/", opacity: 1, path: "M0,0", r: 0, rotation: 0, rx: 0, ry: 0, scale: "1 1", src: "", stroke: "#000", "stroke-dasharray": "", "stroke-linecap": "butt", "stroke-linejoin": "butt", "stroke-miterlimit": 0, "stroke-opacity": 1, "stroke-width": 1,
            target: "_blank", "text-anchor": "middle", title: "Raphael", translation: "0 0", width: 0, x: 0, y: 0}, $a = {along: "along", blur: pa, "clip-rect": "csv", cx: pa, cy: pa, fill: "colour", "fill-opacity": pa, "font-size": pa, height: pa, opacity: pa, path: "path", r: pa, rotation: "csv", rx: pa, ry: pa, scale: "csv", stroke: "colour", "stroke-opacity": pa, "stroke-width": pa, translation: "csv", width: pa, x: pa, y: pa}, ta = "replace";
    e.type = za.SVGAngle || ia.implementation.hasFeature("http://www.w3.org/TR/SVG11/feature#BasicStructure", "1.1") ? "SVG" : "VML";
    if (e.type ==
        "VML") {
        var Na = ia.createElement("div");
        Na.innerHTML = "<!--[if vml]><br><br><![endif]--\>";
        if (Na.childNodes[B] != 2)return e.type = null;
        Na = null
    }
    e.svg = !(e.vml = e.type == "VML");
    c[I] = e[I];
    e._id = 0;
    e._oid = 0;
    e.fn = {};
    e.is = function (a, b) {
        b = ka.call(b);
        return b == "object" && a === Object(a) || b == "undefined" && typeof a == b || b == "null" && a == null || ka.call(ub.call(a).slice(8, -1)) == b
    };
    e.setWindow = function (a) {
        za = a;
        ia = za.document
    };
    e.hsb2rgb = l(function (a, b, f) {
        if (e.is(a, "object") && "h"in a && "s"in a && "b"in a) {
            f = a.b;
            b = a.s;
            a = a.h
        }
        var h;
        if (f ==
            0)return{r: 0, g: 0, b: 0, hex: "#000"};
        if (a > 1 || b > 1 || f > 1) {
            a /= 255;
            b /= 255;
            f /= 255
        }
        h = ~~(a * 6);
        a = a * 6 - h;
        var i = f * (1 - b), g = f * (1 - b * a), m = f * (1 - b * (1 - a));
        a = [f, g, i, i, m, f, f][h];
        b = [m, f, f, g, i, i, m][h];
        h = [i, i, m, f, f, g, i][h];
        a *= 255;
        b *= 255;
        h *= 255;
        f = {r: a, g: b, b: h, toString: o};
        a = (~~a)[sa](16);
        b = (~~b)[sa](16);
        h = (~~h)[sa](16);
        a = a[ta](Ma, "0");
        b = b[ta](Ma, "0");
        h = h[ta](Ma, "0");
        f.hex = "#" + a + b + h;
        return f
    }, e);
    e.rgb2hsb = l(function (a, b, f) {
        if (e.is(a, "object") && "r"in a && "g"in a && "b"in a) {
            f = a.b;
            b = a.g;
            a = a.r
        }
        if (e.is(a, Da)) {
            var h = e.getRGB(a);
            a = h.r;
            b = h.g;
            f = h.b
        }
        if (a > 1 || b > 1 || f > 1) {
            a /= 255;
            b /= 255;
            f /= 255
        }
        var i = Ca(a, b, f), g = Ga(a, b, f);
        h = i;
        if (g == i)return{h: 0, s: 0, b: i}; else {
            var m = i - g;
            g = m / i;
            a = a == i ? (b - f) / m : b == i ? 2 + (f - a) / m : 4 + (a - b) / m;
            a /= 6;
            a < 0 && a++;
            a > 1 && a--
        }
        return{h: a, s: g, b: h, toString: j}
    }, e);
    var wb = /,?([achlmqrstvxz]),?/gi, Xa = /\s*,\s*/, xb = {hs: 1, rg: 1};
    e._path2string = function () {
        return this.join(",")[ta](wb, "$1")
    };
    e.getRGB = l(function (a) {
        if (!a || (a += v).indexOf("-") + 1)return{r: -1, g: -1, b: -1, hex: "none", error: 1};
        if (a == "none")return{r: -1, g: -1, b: -1, hex: "none"};
        !(xb[da](a.substring(0,
            2)) || a.charAt() == "#") && (a = q(a));
        var b, f, h, i, g;
        if (a = a.match(vb)) {
            if (a[2]) {
                h = Ia(a[2].substring(5), 16);
                f = Ia(a[2].substring(3, 5), 16);
                b = Ia(a[2].substring(1, 3), 16)
            }
            if (a[3]) {
                h = Ia((g = a[3].charAt(3)) + g, 16);
                f = Ia((g = a[3].charAt(2)) + g, 16);
                b = Ia((g = a[3].charAt(1)) + g, 16)
            }
            if (a[4]) {
                a = a[4][E](Xa);
                b = ga(a[0]);
                f = ga(a[1]);
                h = ga(a[2]);
                i = ga(a[3])
            }
            if (a[5]) {
                a = a[5][E](Xa);
                b = ga(a[0]) * 2.55;
                f = ga(a[1]) * 2.55;
                h = ga(a[2]) * 2.55;
                i = ga(a[3])
            }
            if (a[6]) {
                a = a[6][E](Xa);
                b = ga(a[0]);
                f = ga(a[1]);
                h = ga(a[2]);
                return e.hsb2rgb(b, f, h)
            }
            if (a[7]) {
                a = a[7][E](Xa);
                b = ga(a[0]) * 2.55;
                f = ga(a[1]) * 2.55;
                h = ga(a[2]) * 2.55;
                return e.hsb2rgb(b, f, h)
            }
            a = {r: b, g: f, b: h};
            b = (~~b)[sa](16);
            f = (~~f)[sa](16);
            h = (~~h)[sa](16);
            b = b[ta](Ma, "0");
            f = f[ta](Ma, "0");
            h = h[ta](Ma, "0");
            a.hex = "#" + b + f + h;
            isFinite(ga(i)) && (a.o = i);
            return a
        }
        return{r: -1, g: -1, b: -1, hex: "none", error: 1}
    }, e);
    e.getColor = function (a) {
        a = this.getColor.start = this.getColor.start || {h: 0, s: 1, b: a || 0.75};
        var b = this.hsb2rgb(a.h, a.s, a.b);
        a.h += 0.075;
        if (a.h > 1) {
            a.h = 0;
            a.s -= 0.2;
            a.s <= 0 && (this.getColor.start = {h: 0, s: 1, b: a.b})
        }
        return b.hex
    };
    e.getColor.reset =
        function () {
            delete this.start
        };
    var yb = /([achlmqstvz])[\s,]*((-?\d*\.?\d*(?:e[-+]?\d+)?\s*,?\s*)+)/ig, zb = /(-?\d*\.?\d*(?:e[-+]?\d+)?)\s*,?\s*/ig;
    e.parsePathString = l(function (a) {
        if (!a)return null;
        var b = {a: 7, c: 6, h: 1, l: 2, m: 2, q: 4, s: 4, t: 2, v: 1, z: 0}, f = [];
        if (e.is(a, Ba) && e.is(a[0], Ba))f = p(a);
        f[B] || (a + v)[ta](yb, function (h, i, g) {
            var m = [];
            h = ka.call(i);
            g[ta](zb, function (n, s) {
                s && m[na](+s)
            });
            if (h == "m" && m[B] > 2) {
                f[na]([i][xa](m.splice(0, 2)));
                h = "l";
                i = i == "m" ? "l" : "L"
            }
            for (; m[B] >= b[h];) {
                f[na]([i][xa](m.splice(0, b[h])));
                if (!b[h])break
            }
        });
        f[sa] = e._path2string;
        return f
    });
    e.findDotsAtSegment = function (a, b, f, h, i, g, m, n, s) {
        var t = 1 - s, y = ma(t, 3) * a + ma(t, 2) * 3 * s * f + t * 3 * s * s * i + ma(s, 3) * m;
        t = ma(t, 3) * b + ma(t, 2) * 3 * s * h + t * 3 * s * s * g + ma(s, 3) * n;
        var F = a + 2 * s * (f - a) + s * s * (i - 2 * f + a), L = b + 2 * s * (h - b) + s * s * (g - 2 * h + b), J = f + 2 * s * (i - f) + s * s * (m - 2 * i + f), x = h + 2 * s * (g - h) + s * s * (n - 2 * g + h);
        a = (1 - s) * a + s * f;
        b = (1 - s) * b + s * h;
        i = (1 - s) * i + s * m;
        g = (1 - s) * g + s * n;
        n = 90 - M.atan((F - J) / (L - x)) * 180 / M.PI;
        (F > J || L < x) && (n += 180);
        return{x: y, y: t, m: {x: F, y: L}, n: {x: J, y: x}, start: {x: a, y: b}, end: {x: i, y: g}, alpha: n}
    };
    var Ya = l(function (a) {
        if (!a)return{x: 0,
            y: 0, width: 0, height: 0};
        a = Ta(a);
        for (var b = 0, f = 0, h = [], i = [], g, m = 0, n = a[B]; m < n; m++) {
            g = a[m];
            if (g[0] == "M") {
                b = g[1];
                f = g[2];
                h[na](b);
                i[na](f)
            } else {
                b = Ab(b, f, g[1], g[2], g[3], g[4], g[5], g[6]);
                h = h[xa](b.min.x, b.max.x);
                i = i[xa](b.min.y, b.max.y);
                b = g[5];
                f = g[6]
            }
        }
        a = Ga[qa](0, h);
        g = Ga[qa](0, i);
        return{x: a, y: g, width: Ca[qa](0, h) - a, height: Ca[qa](0, i) - g}
    }), eb = l(function (a) {
        if (!e.is(a, Ba) || !e.is(a && a[0], Ba))a = e.parsePathString(a);
        var b = [], f = 0, h = 0, i = 0, g = 0, m = 0;
        if (a[0][0] == "M") {
            f = a[0][1];
            h = a[0][2];
            i = f;
            g = h;
            m++;
            b[na](["M", f, h])
        }
        for (var n =
            a[B]; m < n; m++) {
            var s = b[m] = [], t = a[m];
            if (t[0] != ka.call(t[0])) {
                s[0] = ka.call(t[0]);
                switch (s[0]) {
                    case "a":
                        s[1] = t[1];
                        s[2] = t[2];
                        s[3] = t[3];
                        s[4] = t[4];
                        s[5] = t[5];
                        s[6] = +(t[6] - f).toFixed(3);
                        s[7] = +(t[7] - h).toFixed(3);
                        break;
                    case "v":
                        s[1] = +(t[1] - h).toFixed(3);
                        break;
                    case "m":
                        i = t[1];
                        g = t[2];
                    default:
                        for (var y = 1, F = t[B]; y < F; y++)s[y] = +(t[y] - (y % 2 ? f : h)).toFixed(3)
                }
            } else {
                b[m] = [];
                if (t[0] == "m") {
                    i = t[1] + f;
                    g = t[2] + h
                }
                s = 0;
                for (y = t[B]; s < y; s++)b[m][s] = t[s]
            }
            t = b[m][B];
            switch (b[m][0]) {
                case "z":
                    f = i;
                    h = g;
                    break;
                case "h":
                    f += +b[m][t - 1];
                    break;
                case "v":
                    h += +b[m][t - 1];
                    break;
                default:
                    f += +b[m][t - 2];
                    h += +b[m][t - 1]
            }
        }
        b[sa] = e._path2string;
        return b
    }, 0, p), Oa = l(function (a) {
        if (!e.is(a, Ba) || !e.is(a && a[0], Ba))a = e.parsePathString(a);
        var b = [], f = 0, h = 0, i = 0, g = 0, m = 0;
        if (a[0][0] == "M") {
            f = +a[0][1];
            h = +a[0][2];
            i = f;
            g = h;
            m++;
            b[0] = ["M", f, h]
        }
        for (var n = a[B]; m < n; m++) {
            var s = b[m] = [], t = a[m];
            if (t[0] != Va.call(t[0])) {
                s[0] = Va.call(t[0]);
                switch (s[0]) {
                    case "A":
                        s[1] = t[1];
                        s[2] = t[2];
                        s[3] = t[3];
                        s[4] = t[4];
                        s[5] = t[5];
                        s[6] = +(t[6] + f);
                        s[7] = +(t[7] + h);
                        break;
                    case "V":
                        s[1] = +t[1] + h;
                        break;
                    case "H":
                        s[1] =
                            +t[1] + f;
                        break;
                    case "M":
                        i = +t[1] + f;
                        g = +t[2] + h;
                    default:
                        for (var y = 1, F = t[B]; y < F; y++)s[y] = +t[y] + (y % 2 ? f : h)
                }
            } else {
                y = 0;
                for (F = t[B]; y < F; y++)b[m][y] = t[y]
            }
            switch (s[0]) {
                case "Z":
                    f = i;
                    h = g;
                    break;
                case "H":
                    f = s[1];
                    break;
                case "V":
                    h = s[1];
                    break;
                default:
                    f = b[m][b[m][B] - 2];
                    h = b[m][b[m][B] - 1]
            }
        }
        b[sa] = e._path2string;
        return b
    }, null, p), Ab = l(function (a, b, f, h, i, g, m, n) {
        var s = i - 2 * f + a - (m - 2 * i + f), t = 2 * (f - a) - 2 * (i - f), y = a - f, F = (-t + M.sqrt(t * t - 4 * s * y)) / 2 / s;
        s = (-t - M.sqrt(t * t - 4 * s * y)) / 2 / s;
        var L = [b, n], J = [a, m];
        M.abs(F) > 1E12 && (F = 0.5);
        M.abs(s) > 1E12 && (s =
            0.5);
        if (F > 0 && F < 1) {
            F = z(a, b, f, h, i, g, m, n, F);
            J[na](F.x);
            L[na](F.y)
        }
        if (s > 0 && s < 1) {
            F = z(a, b, f, h, i, g, m, n, s);
            J[na](F.x);
            L[na](F.y)
        }
        s = g - 2 * h + b - (n - 2 * g + h);
        t = 2 * (h - b) - 2 * (g - h);
        y = b - h;
        F = (-t + M.sqrt(t * t - 4 * s * y)) / 2 / s;
        s = (-t - M.sqrt(t * t - 4 * s * y)) / 2 / s;
        M.abs(F) > 1E12 && (F = 0.5);
        M.abs(s) > 1E12 && (s = 0.5);
        if (F > 0 && F < 1) {
            F = z(a, b, f, h, i, g, m, n, F);
            J[na](F.x);
            L[na](F.y)
        }
        if (s > 0 && s < 1) {
            F = z(a, b, f, h, i, g, m, n, s);
            J[na](F.x);
            L[na](F.y)
        }
        return{min: {x: Ga[qa](0, J), y: Ga[qa](0, L)}, max: {x: Ca[qa](0, J), y: Ca[qa](0, L)}}
    }), Ta = l(function (a, b) {
        function f(J, x) {
            var S;
            if (!J)return["C", x.x, x.y, x.x, x.y, x.x, x.y];
            !(J[0]in{T: 1, Q: 1}) && (x.qx = x.qy = null);
            switch (J[0]) {
                case "M":
                    x.X = J[1];
                    x.Y = J[2];
                    break;
                case "A":
                    J = ["C"][xa](k[qa](0, [x.x, x.y][xa](J.slice(1))));
                    break;
                case "S":
                    S = x.x + (x.x - (x.bx || x.x));
                    x = x.y + (x.y - (x.by || x.y));
                    J = ["C", S, x][xa](J.slice(1));
                    break;
                case "T":
                    x.qx = x.x + (x.x - (x.qx || x.x));
                    x.qy = x.y + (x.y - (x.qy || x.y));
                    J = ["C"][xa](u(x.x, x.y, x.qx, x.qy, J[1], J[2]));
                    break;
                case "Q":
                    x.qx = J[1];
                    x.qy = J[2];
                    J = ["C"][xa](u(x.x, x.y, J[1], J[2], J[3], J[4]));
                    break;
                case "L":
                    J = ["C"][xa]([x.x, x.y,
                        J[1], J[2], J[1], J[2]]);
                    break;
                case "H":
                    J = ["C"][xa]([x.x, x.y, J[1], x.y, J[1], x.y]);
                    break;
                case "V":
                    J = ["C"][xa]([x.x, x.y, x.x, J[1], x.x, J[1]]);
                    break;
                case "Z":
                    J = ["C"][xa]([x.x, x.y, x.X, x.Y, x.X, x.Y]);
                    break
            }
            return J
        }

        function h(J, x) {
            if (J[x][B] > 7) {
                J[x].shift();
                for (var S = J[x]; S[B];)J.splice(x++, 0, ["C"][xa](S.splice(0, 6)));
                J.splice(x, 1);
                s = Ca(g[B], m && m[B] || 0)
            }
        }

        function i(J, x, S, Aa, ja) {
            if (J && x && J[ja][0] == "M" && x[ja][0] != "M") {
                x.splice(ja, 0, ["M", Aa.x, Aa.y]);
                S.bx = 0;
                S.by = 0;
                S.x = J[ja][1];
                S.y = J[ja][2];
                s = Ca(g[B], m && m[B] || 0)
            }
        }

        var g = Oa(a), m = b && Oa(b);
        a = {x: 0, y: 0, bx: 0, by: 0, X: 0, Y: 0, qx: null, qy: null};
        b = {x: 0, y: 0, bx: 0, by: 0, X: 0, Y: 0, qx: null, qy: null};
        for (var n = 0, s = Ca(g[B], m && m[B] || 0); n < s; n++) {
            g[n] = f(g[n], a);
            h(g, n);
            m && (m[n] = f(m[n], b));
            m && h(m, n);
            i(g, m, a, b, n);
            i(m, g, b, a, n);
            var t = g[n], y = m && m[n], F = t[B], L = m && y[B];
            a.x = t[F - 2];
            a.y = t[F - 1];
            a.bx = ga(t[F - 4]) || a.x;
            a.by = ga(t[F - 3]) || a.y;
            b.bx = m && (ga(y[L - 4]) || b.x);
            b.by = m && (ga(y[L - 3]) || b.y);
            b.x = m && y[L - 2];
            b.y = m && y[L - 1]
        }
        return m ? [g, m] : g
    }, null, p), gb = l(function (a) {
        for (var b = [], f = 0, h = a[B]; f < h; f++) {
            var i = {},
                g = a[f].match(/^([^:]*):?([\d\.]*)/);
            i.color = e.getRGB(g[1]);
            if (i.color.error)return null;
            i.color = i.color.hex;
            g[2] && (i.offset = g[2] + "%");
            b[na](i)
        }
        f = 1;
        for (h = b[B] - 1; f < h; f++)if (!b[f].offset) {
            a = ga(b[f - 1].offset || 0);
            g = 0;
            for (i = f + 1; i < h; i++)if (b[i].offset) {
                g = b[i].offset;
                break
            }
            if (!g) {
                g = 100;
                i = h
            }
            g = ga(g);
            for (g = (g - a) / (i - f + 1); f < i; f++) {
                a += g;
                b[f].offset = a + "%"
            }
        }
        return b
    }), hb = /^r(?:\(([^,]+?)\s*,\s*([^\)]+?)\))?/;
    if (e.svg) {
        c[I].svgns = "http://www.w3.org/2000/svg";
        c[I].xlink = "http://www.w3.org/1999/xlink";
        ra = function (a) {
            return+a +
                (~~a === a) * 0.5
        };
        var Y = function (a, b) {
            if (b)for (var f in b)b[da](f) && a.setAttribute(f, b[f] + v); else {
                a = ia.createElementNS(c[I].svgns, a);
                a.style.webkitTapHighlightColor = "rgba(0,0,0,0)";
                return a
            }
        };
        e[sa] = function () {
            return"Your browser supports SVG.\nYou are running Rapha\u00ebl " + this.version
        };
        var ib = function (a, b) {
            var f = Y("path");
            b.canvas && b.canvas[X](f);
            b = new T(f, b);
            b.type = "path";
            Ha(b, {fill: "none", stroke: "#000", path: a});
            return b
        }, Pa = function (a, b, f) {
            var h = "linear", i = 0.5, g = 0.5, m = a.style;
            b = (b + v)[ta](hb, function (y, F, L) {
                h = "radial";
                if (F && L) {
                    i = ga(F);
                    g = ga(L);
                    y = (g > 0.5) * 2 - 1;
                    ma(i - 0.5, 2) + ma(g - 0.5, 2) > 0.25 && (g = M.sqrt(0.25 - ma(i - 0.5, 2)) * y + 0.5) && g != 0.5 && (g = g.toFixed(5) - 1.0E-5 * y)
                }
                return v
            });
            b = b[E](/\s*\-\s*/);
            if (h == "linear") {
                var n = b.shift();
                n = -ga(n);
                if (isNaN(n))return null;
                n = [0, 0, M.cos(n * M.PI / 180), M.sin(n * M.PI / 180)];
                var s = 1 / (Ca(M.abs(n[2]), M.abs(n[3])) || 1);
                n[2] *= s;
                n[3] *= s;
                if (n[2] < 0) {
                    n[0] = -n[2];
                    n[2] = 0
                }
                if (n[3] < 0) {
                    n[1] = -n[3];
                    n[3] = 0
                }
            }
            b = gb(b);
            if (!b)return null;
            s = a.getAttribute("fill");
            (s = s.match(/^url\(#(.*)\)$/)) && f.defs.removeChild(ia.getElementById(s[1]));
            s = Y(h + "Gradient");
            s.id = "r" + (e._id++)[sa](36);
            Y(s, h == "radial" ? {fx: i, fy: g} : {x1: n[0], y1: n[1], x2: n[2], y2: n[3]});
            f.defs[X](s);
            f = 0;
            for (n = b[B]; f < n; f++) {
                var t = Y("stop");
                Y(t, {offset: b[f].offset ? b[f].offset : !f ? "0%" : "100%", "stop-color": b[f].color || "#fff"});
                s[X](t)
            }
            Y(a, {fill: "url(#" + s.id + ")", opacity: 1, "fill-opacity": 1});
            m.fill = v;
            m.opacity = 1;
            return m.fillOpacity = 1
        }, ab = function (a) {
            var b = a.getBBox();
            Y(a.pattern, {patternTransform: e.format("translate({0},{1})", b.x, b.y)})
        }, Ha = function (a, b) {
            function f(x, S) {
                if (S = h[ka.call(S)]) {
                    var Aa =
                        x.attrs["stroke-width"] || "1";
                    x = {round: Aa, square: Aa, butt: 0}[x.attrs["stroke-linecap"] || b["stroke-linecap"]] || 0;
                    for (var ja = [], ba = S[B]; ba--;)ja[ba] = S[ba] * Aa + (ba % 2 ? 1 : -1) * x;
                    Y(i, {"stroke-dasharray": ja[Z](",")})
                }
            }

            var h = {"": [0], none: [0], "-": [3, 1], ".": [1, 1], "-.": [3, 1, 1, 1], "-..": [3, 1, 1, 1, 1, 1], ". ": [1, 3], "- ": [4, 3], "--": [8, 3], "- .": [4, 3, 1, 3], "--.": [8, 3, 1, 3], "--..": [8, 3, 1, 3, 1, 3]}, i = a.node, g = a.attrs, m = a.rotate();
            b[da]("rotation") && (m = b.rotation);
            var n = (m + v)[E](la);
            if (n.length - 1) {
                n[1] = +n[1];
                n[2] = +n[2]
            } else n = null;
            ga(m) && a.rotate(0, true);
            for (var s in b)if (b[da](s))if (Wa[da](s)) {
                var t = b[s];
                g[s] = t;
                switch (s) {
                    case "blur":
                        a.blur(t);
                        break;
                    case "rotation":
                        a.rotate(t, true);
                        break;
                    case "href":
                    case "title":
                    case "target":
                        var y = i.parentNode;
                        if (ka.call(y.tagName) != "a") {
                            var F = Y("a");
                            y.insertBefore(F, i);
                            F[X](i);
                            y = F
                        }
                        y.setAttributeNS(a.paper.xlink, s, t);
                        break;
                    case "cursor":
                        i.style.cursor = t;
                        break;
                    case "clip-rect":
                        y = (t + v)[E](la);
                        if (y[B] == 4) {
                            a.clip && a.clip.parentNode.parentNode.removeChild(a.clip.parentNode);
                            var L = Y("clipPath");
                            F = Y("rect");
                            L.id = "r" + (e._id++)[sa](36);
                            Y(F, {x: y[0], y: y[1], width: y[2], height: y[3]});
                            L[X](F);
                            a.paper.defs[X](L);
                            Y(i, {"clip-path": "url(#" + L.id + ")"});
                            a.clip = F
                        }
                        if (!t) {
                            (t = ia.getElementById(i.getAttribute("clip-path")[ta](/(^url\(#|\)$)/g, v))) && t.parentNode.removeChild(t);
                            Y(i, {"clip-path": v});
                            delete a.clip
                        }
                        break;
                    case "path":
                        if (a.type == "path")Y(i, {d: t ? g.path = Oa(t) : "M0,0"});
                        break;
                    case "width":
                        i.setAttribute(s, t);
                        if (g.fx) {
                            s = "x";
                            t = g.x
                        } else break;
                    case "x":
                        if (g.fx)t = -g.x - (g.width || 0);
                    case "rx":
                        if (s == "rx" && a.type ==
                            "rect")break;
                    case "cx":
                        n && (s == "x" || s == "cx") && (n[1] += t - g[s]);
                        i.setAttribute(s, ra(t));
                        a.pattern && ab(a);
                        break;
                    case "height":
                        i.setAttribute(s, t);
                        if (g.fy) {
                            s = "y";
                            t = g.y
                        } else break;
                    case "y":
                        if (g.fy)t = -g.y - (g.height || 0);
                    case "ry":
                        if (s == "ry" && a.type == "rect")break;
                    case "cy":
                        n && (s == "y" || s == "cy") && (n[2] += t - g[s]);
                        i.setAttribute(s, ra(t));
                        a.pattern && ab(a);
                        break;
                    case "r":
                        a.type == "rect" ? Y(i, {rx: t, ry: t}) : i.setAttribute(s, t);
                        break;
                    case "src":
                        a.type == "image" && i.setAttributeNS(a.paper.xlink, "href", t);
                        break;
                    case "stroke-width":
                        i.style.strokeWidth =
                            t;
                        i.setAttribute(s, t);
                        g["stroke-dasharray"] && f(a, g["stroke-dasharray"]);
                        break;
                    case "stroke-dasharray":
                        f(a, t);
                        break;
                    case "translation":
                        t = (t + v)[E](la);
                        t[0] = +t[0] || 0;
                        t[1] = +t[1] || 0;
                        if (n) {
                            n[1] += t[0];
                            n[2] += t[1]
                        }
                        ua.call(a, t[0], t[1]);
                        break;
                    case "scale":
                        t = (t + v)[E](la);
                        a.scale(+t[0] || 1, +t[1] || +t[0] || 1, isNaN(ga(t[2])) ? null : +t[2], isNaN(ga(t[3])) ? null : +t[3]);
                        break;
                    case "fill":
                        if (y = (t + v).match(fb)) {
                            L = Y("pattern");
                            var J = Y("image");
                            L.id = "r" + (e._id++)[sa](36);
                            Y(L, {x: 0, y: 0, patternUnits: "userSpaceOnUse", height: 1, width: 1});
                            Y(J, {x: 0, y: 0});
                            J.setAttributeNS(a.paper.xlink, "href", y[1]);
                            L[X](J);
                            t = ia.createElement("img");
                            t.style.cssText = "position:absolute;left:-9999em;top-9999em";
                            t.onload = function () {
                                Y(L, {width: this.offsetWidth, height: this.offsetHeight});
                                Y(J, {width: this.offsetWidth, height: this.offsetHeight});
                                ia.body.removeChild(this);
                                a.paper.safari()
                            };
                            ia.body[X](t);
                            t.src = y[1];
                            a.paper.defs[X](L);
                            i.style.fill = "url(#" + L.id + ")";
                            Y(i, {fill: "url(#" + L.id + ")"});
                            a.pattern = L;
                            a.pattern && ab(a);
                            break
                        }
                        y = e.getRGB(t);
                        if (y.error) {
                            if (({circle: 1,
                                ellipse: 1}[da](a.type) || (t + v).charAt() != "r") && Pa(i, t, a.paper)) {
                                g.gradient = t;
                                g.fill = "none";
                                break
                            }
                        } else {
                            delete b.gradient;
                            delete g.gradient;
                            !e.is(g.opacity, "undefined") && e.is(b.opacity, "undefined") && Y(i, {opacity: g.opacity});
                            !e.is(g["fill-opacity"], "undefined") && e.is(b["fill-opacity"], "undefined") && Y(i, {"fill-opacity": g["fill-opacity"]})
                        }
                        y[da]("o") && Y(i, {"fill-opacity": y.o / 100});
                    case "stroke":
                        y = e.getRGB(t);
                        i.setAttribute(s, y.hex);
                        s == "stroke" && y[da]("o") && Y(i, {"stroke-opacity": y.o / 100});
                        break;
                    case "gradient":
                        (({circle: 1,
                            ellipse: 1})[da](a.type) || (t + v).charAt() != "r") && Pa(i, t, a.paper);
                        break;
                    case "opacity":
                    case "fill-opacity":
                        if (g.gradient) {
                            if (y = ia.getElementById(i.getAttribute("fill")[ta](/^url\(#|\)$/g, v))) {
                                y = y.getElementsByTagName("stop");
                                y[y[B] - 1].setAttribute("stop-opacity", t)
                            }
                            break
                        }
                    default:
                        s == "font-size" && (t = Ia(t, 10) + "px");
                        y = s[ta](/(\-.)/g, function (x) {
                            return Va.call(x.substring(1))
                        });
                        i.style[y] = t;
                        i.setAttribute(s, t);
                        break
                }
            }
            Bb(a, b);
            if (n)a.rotate(n.join(A)); else ga(m) && a.rotate(m, true)
        }, Bb = function (a, b) {
            if (!(a.type !=
                "text" || !(b[da]("text") || b[da]("font") || b[da]("font-size") || b[da]("x") || b[da]("y")))) {
                var f = a.attrs, h = a.node, i = h.firstChild ? Ia(ia.defaultView.getComputedStyle(h.firstChild, v).getPropertyValue("font-size"), 10) : 10;
                if (b[da]("text")) {
                    for (f.text = b.text; h.firstChild;)h.removeChild(h.firstChild);
                    b = (b.text + v)[E]("\n");
                    for (var g = 0, m = b[B]; g < m; g++)if (b[g]) {
                        var n = Y("tspan");
                        g && Y(n, {dy: i * 1.2, x: f.x});
                        n[X](ia.createTextNode(b[g]));
                        h[X](n)
                    }
                } else {
                    b = h.getElementsByTagName("tspan");
                    g = 0;
                    for (m = b[B]; g < m; g++)g && Y(b[g],
                        {dy: i * 1.2, x: f.x})
                }
                Y(h, {y: f.y});
                a = a.getBBox();
                (a = f.y - (a.y + a.height / 2)) && isFinite(a) && Y(h, {y: f.y + a})
            }
        }, T = function (a, b) {
            this[0] = a;
            this.id = e._oid++;
            this.node = a;
            a.raphael = this;
            this.paper = b;
            this.attrs = this.attrs || {};
            this.transformations = [];
            this._ = {tx: 0, ty: 0, rt: {deg: 0, cx: 0, cy: 0}, sx: 1, sy: 1};
            !b.bottom && (b.bottom = this);
            (this.prev = b.top) && (b.top.next = this);
            b.top = this;
            this.next = null
        };
        T[I].rotate = function (a, b, f) {
            if (this.removed)return this;
            if (a == null) {
                if (this._.rt.cx)return[this._.rt.deg, this._.rt.cx, this._.rt.cy][Z](A);
                return this._.rt.deg
            }
            var h = this.getBBox();
            a = (a + v)[E](la);
            if (a[B] - 1) {
                b = ga(a[1]);
                f = ga(a[2])
            }
            a = ga(a[0]);
            if (b != null)this._.rt.deg = a; else this._.rt.deg += a;
            f == null && (b = null);
            this._.rt.cx = b;
            this._.rt.cy = f;
            b = b == null ? h.x + h.width / 2 : b;
            f = f == null ? h.y + h.height / 2 : f;
            if (this._.rt.deg) {
                this.transformations[0] = e.format("rotate({0} {1} {2})", this._.rt.deg, b, f);
                this.clip && Y(this.clip, {transform: e.format("rotate({0} {1} {2})", -this._.rt.deg, b, f)})
            } else {
                this.transformations[0] = v;
                this.clip && Y(this.clip, {transform: v})
            }
            Y(this.node,
                {transform: this.transformations[Z](A)});
            return this
        };
        T[I].hide = function () {
            !this.removed && (this.node.style.display = "none");
            return this
        };
        T[I].show = function () {
            !this.removed && (this.node.style.display = "");
            return this
        };
        T[I].remove = function () {
            if (!this.removed) {
                D(this, this.paper);
                this.node.parentNode.removeChild(this.node);
                for (var a in this)delete this[a];
                this.removed = true
            }
        };
        T[I].getBBox = function () {
            if (this.removed)return this;
            if (this.type == "path")return Ya(this.attrs.path);
            if (this.node.style.display == "none") {
                this.show();
                var a = true
            }
            var b = {};
            try {
                b = this.node.getBBox()
            } catch (f) {
            } finally {
                b = b || {}
            }
            if (this.type == "text") {
                b = {x: b.x, y: Infinity, width: 0, height: 0};
                for (var h = 0, i = this.node.getNumberOfChars(); h < i; h++) {
                    var g = this.node.getExtentOfChar(h);
                    g.y < b.y && (b.y = g.y);
                    g.y + g.height - b.y > b.height && (b.height = g.y + g.height - b.y);
                    g.x + g.width - b.x > b.width && (b.width = g.x + g.width - b.x)
                }
            }
            a && this.hide();
            return b
        };
        T[I].attr = function (a, b) {
            if (this.removed)return this;
            if (a == null) {
                a = {};
                for (var f in this.attrs)if (this.attrs[da](f))a[f] = this.attrs[f];
                this._.rt.deg && (a.rotation = this.rotate());
                (this._.sx != 1 || this._.sy != 1) && (a.scale = this.scale());
                a.gradient && a.fill == "none" && (a.fill = a.gradient) && delete a.gradient;
                return a
            }
            if (b == null && e.is(a, Da)) {
                if (a == "translation")return ua.call(this);
                if (a == "rotation")return this.rotate();
                if (a == "scale")return this.scale();
                if (a == "fill" && this.attrs.fill == "none" && this.attrs.gradient)return this.attrs.gradient;
                return this.attrs[a]
            }
            if (b == null && e.is(a, Ba)) {
                b = {};
                f = 0;
                for (var h = a.length; f < h; f++)b[a[f]] = this.attr(a[f]);
                return b
            }
            if (b !=
                null) {
                f = {};
                f[a] = b;
                Ha(this, f)
            } else a != null && e.is(a, "object") && Ha(this, a);
            return this
        };
        T[I].toFront = function () {
            if (this.removed)return this;
            this.node.parentNode[X](this.node);
            var a = this.paper;
            a.top != this && K(this, a);
            return this
        };
        T[I].toBack = function () {
            if (this.removed)return this;
            if (this.node.parentNode.firstChild != this.node) {
                this.node.parentNode.insertBefore(this.node, this.node.parentNode.firstChild);
                C(this, this.paper)
            }
            return this
        };
        T[I].insertAfter = function (a) {
            if (this.removed)return this;
            var b = a.node;
            b.nextSibling ? b.parentNode.insertBefore(this.node, b.nextSibling) : b.parentNode[X](this.node);
            H(this, a, this.paper);
            return this
        };
        T[I].insertBefore = function (a) {
            if (this.removed)return this;
            var b = a.node;
            b.parentNode.insertBefore(this.node, b);
            O(this, a, this.paper);
            return this
        };
        T[I].blur = function (a) {
            if (+a !== 0) {
                var b = Y("filter"), f = Y("feGaussianBlur");
                this.attrs.blur = a;
                b.id = "r" + (e._id++)[sa](36);
                Y(f, {stdDeviation: +a || 1.5});
                b.appendChild(f);
                this.paper.defs.appendChild(b);
                this._blur = b;
                Y(this.node, {filter: "url(#" +
                    b.id + ")"})
            } else {
                if (this._blur) {
                    this._blur.parentNode.removeChild(this._blur);
                    delete this._blur;
                    delete this.attrs.blur
                }
                this.node.removeAttribute("filter")
            }
        };
        var jb = function (a, b, f, h) {
            b = ra(b);
            f = ra(f);
            var i = Y("circle");
            a.canvas && a.canvas[X](i);
            a = new T(i, a);
            a.attrs = {cx: b, cy: f, r: h, fill: "none", stroke: "#000"};
            a.type = "circle";
            Y(i, a.attrs);
            return a
        }, kb = function (a, b, f, h, i, g) {
            b = ra(b);
            f = ra(f);
            var m = Y("rect");
            a.canvas && a.canvas[X](m);
            a = new T(m, a);
            a.attrs = {x: b, y: f, width: h, height: i, r: g || 0, rx: g || 0, ry: g || 0, fill: "none",
                stroke: "#000"};
            a.type = "rect";
            Y(m, a.attrs);
            return a
        }, lb = function (a, b, f, h, i) {
            b = ra(b);
            f = ra(f);
            var g = Y("ellipse");
            a.canvas && a.canvas[X](g);
            a = new T(g, a);
            a.attrs = {cx: b, cy: f, rx: h, ry: i, fill: "none", stroke: "#000"};
            a.type = "ellipse";
            Y(g, a.attrs);
            return a
        }, mb = function (a, b, f, h, i, g) {
            var m = Y("image");
            Y(m, {x: f, y: h, width: i, height: g, preserveAspectRatio: "none"});
            m.setAttributeNS(a.xlink, "href", b);
            a.canvas && a.canvas[X](m);
            a = new T(m, a);
            a.attrs = {x: f, y: h, width: i, height: g, src: b};
            a.type = "image";
            return a
        }, nb = function (a, b, f, h) {
            var i = Y("text");
            Y(i, {x: b, y: f, "text-anchor": "middle"});
            a.canvas && a.canvas[X](i);
            a = new T(i, a);
            a.attrs = {x: b, y: f, "text-anchor": "middle", text: h, font: Wa.font, stroke: "none", fill: "#000"};
            a.type = "text";
            Ha(a, a.attrs);
            return a
        }, ob = function (a, b) {
            this.width = a || this.width;
            this.height = b || this.height;
            this.canvas.setAttribute("width", this.width);
            this.canvas.setAttribute("height", this.height);
            return this
        }, Za = function () {
            var a = G[qa](0, arguments), b = a && a.container, f = a.x, h = a.y, i = a.width;
            a = a.height;
            if (!b)throw Error("SVG container not found.");
            var g = Y("svg");
            f = f || 0;
            h = h || 0;
            i = i || 512;
            a = a || 342;
            Y(g, {xmlns: "http://www.w3.org/2000/svg", version: 1.1, width: i, height: a});
            if (b == 1) {
                g.style.cssText = "position:absolute;left:" + f + "px;top:" + h + "px";
                ia.body[X](g)
            } else b.firstChild ? b.insertBefore(g, b.firstChild) : b[X](g);
            b = new c;
            b.twidth = i/2;
            b.theight = a/2;
            b.width = i;
            b.height = a;
            b.canvas = g;
            w.call(b, b, e.fn);
            b.clear();
            return b
        };
        c[I].clear = function () {
            for (var a = this.canvas; a.firstChild;)a.removeChild(a.firstChild);
            this.bottom = this.top = null;
            (this.desc = Y("desc"))[X](ia.createTextNode("Created with Rapha\u00ebl"));
            a[X](this.desc);
            a[X](this.defs = Y("defs"))
        };
        c[I].remove = function () {
            this.canvas.parentNode && this.canvas.parentNode.removeChild(this.canvas);
            for (var a in this)this[a] = P(a)
        }
    }
    if (e.vml) {
        var pb = {M: "m", L: "l", C: "c", Z: "x", m: "t", l: "r", c: "v", z: "x"}, Cb = /([clmz]),?([^clmz]*)/gi, Db = /-?[^,\s-]+/g, Qa = 1E3 + A + 1E3, Ra = {path: 1, rect: 1}, Eb = function (a) {
            var b = /[ahqstv]/ig, f = Oa;
            (a + v).match(b) && (f = Ta);
            b = /[clmz]/g;
            if (f == Oa && !(a + v).match(b))return a = (a + v)[ta](Cb, function (s, t, y) {
                var F = [], L = ka.call(t) == "m", J = pb[t];
                y[ta](Db, function (x) {
                    if (L &&
                        F[B] == 2) {
                        J += F + pb[t == "m" ? "l" : "L"];
                        F = []
                    }
                    F[na](ra(x * 10))
                });
                return J + F
            });
            b = f(a);
            var h;
            a = [];
            for (var i = 0, g = b[B]; i < g; i++) {
                f = b[i];
                h = ka.call(b[i][0]);
                h == "z" && (h = "x");
                for (var m = 1, n = f[B]; m < n; m++)h += ra(f[m] * 10) + (m != n - 1 ? "," : v);
                a[na](h)
            }
            return a[Z](A)
        };
        e[sa] = function () {
            return"Your browser doesn\u2019t support SVG. Falling down to VML.\nYou are running Rapha\u00ebl " + this.version
        };
        ib = function (a, b) {
            var f = Ea("group");
            f.style.cssText = "position:absolute;left:0;top:0;width:" + b.width + "px;height:" + b.height + "px";
            f.coordsize =
                b.coordsize;
            f.coordorigin = b.coordorigin;
            var h = Ea("shape"), i = h.style;
            i.width = b.width + "px";
            i.height = b.height + "px";
            h.coordsize = Qa;
            h.coordorigin = b.coordorigin;
            f[X](h);
            h = new T(h, f, b);
            i = {fill: "none", stroke: "#000"};
            a && (i.path = a);
            h.isAbsolute = true;
            h.type = "path";
            h.path = [];
            h.Path = v;
            Ha(h, i);
            b.canvas[X](f);
            return h
        };
        Ha = function (a, b) {
            a.attrs = a.attrs || {};
            var f = a.node, h = a.attrs, i = f.style, g;
            g = (b.x != h.x || b.y != h.y || b.width != h.width || b.height != h.height || b.r != h.r) && a.type == "rect";
            var m = a;
            for (var n in b)if (b[da](n))h[n] =
                b[n];
            if (g) {
                h.path = qb(h.x, h.y, h.width, h.height, h.r);
                a.X = h.x;
                a.Y = h.y;
                a.W = h.width;
                a.H = h.height
            }
            b.href && (f.href = b.href);
            b.title && (f.title = b.title);
            b.target && (f.target = b.target);
            b.cursor && (i.cursor = b.cursor);
            "blur"in b && a.blur(b.blur);
            if (b.path && a.type == "path" || g)f.path = Eb(h.path);
            b.rotation != null && a.rotate(b.rotation, true);
            if (b.translation) {
                g = (b.translation + v)[E](la);
                ua.call(a, g[0], g[1]);
                if (a._.rt.cx != null) {
                    a._.rt.cx += +g[0];
                    a._.rt.cy += +g[1];
                    a.setBox(a.attrs, g[0], g[1])
                }
            }
            if (b.scale) {
                g = (b.scale + v)[E](la);
                a.scale(+g[0] || 1, +g[1] || +g[0] || 1, +g[2] || null, +g[3] || null)
            }
            if ("clip-rect"in b) {
                g = (b["clip-rect"] + v)[E](la);
                if (g[B] == 4) {
                    g[2] = +g[2] + +g[0];
                    g[3] = +g[3] + +g[1];
                    n = f.clipRect || ia.createElement("div");
                    var s = n.style, t = f.parentNode;
                    s.clip = e.format("rect({1}px {2}px {3}px {0}px)", g);
                    if (!f.clipRect) {
                        s.position = "absolute";
                        s.top = 0;
                        s.left = 0;
                        s.width = a.paper.width + "px";
                        s.height = a.paper.height + "px";
                        t.parentNode.insertBefore(n, t);
                        n[X](t);
                        f.clipRect = n
                    }
                }
                if (!b["clip-rect"])f.clipRect && (f.clipRect.style.clip = v)
            }
            if (a.type ==
                "image" && b.src)f.src = b.src;
            if (a.type == "image" && b.opacity) {
                f.filterOpacity = " progid:DXImageTransform.Microsoft.Alpha(opacity=" + b.opacity * 100 + ")";
                i.filter = (f.filterMatrix || v) + (f.filterOpacity || v)
            }
            b.font && (i.font = b.font);
            b["font-family"] && (i.fontFamily = '"' + b["font-family"][E](",")[0][ta](/^['"]+|['"]+$/g, v) + '"');//'
            b["font-size"] && (i.fontSize = b["font-size"]);
            b["font-weight"] && (i.fontWeight = b["font-weight"]);
            b["font-style"] && (i.fontStyle = b["font-style"]);
            if (b.opacity != null || b["stroke-width"] != null || b.fill !=
                null || b.stroke != null || b["stroke-width"] != null || b["stroke-opacity"] != null || b["fill-opacity"] != null || b["stroke-dasharray"] != null || b["stroke-miterlimit"] != null || b["stroke-linejoin"] != null || b["stroke-linecap"] != null) {
                f = a.shape || f;
                i = f.getElementsByTagName("fill") && f.getElementsByTagName("fill")[0];
                g = false;
                !i && (g = i = Ea("fill"));
                if ("fill-opacity"in b || "opacity"in b) {
                    a = ((+h["fill-opacity"] + 1 || 2) - 1) * ((+h.opacity + 1 || 2) - 1) * ((+e.getRGB(b.fill).o + 1 || 2) - 1);
                    a < 0 && (a = 0);
                    a > 1 && (a = 1);
                    i.opacity = a
                }
                b.fill && (i.on = true);
                if (i.on ==
                    null || b.fill == "none")i.on = false;
                if (i.on && b.fill)if (a = b.fill.match(fb)) {
                    i.src = a[1];
                    i.type = "tile"
                } else {
                    i.color = e.getRGB(b.fill).hex;
                    i.src = v;
                    i.type = "solid";
                    if (e.getRGB(b.fill).error && (m.type in{circle: 1, ellipse: 1} || (b.fill + v).charAt() != "r") && Pa(m, b.fill)) {
                        h.fill = "none";
                        h.gradient = b.fill
                    }
                }
                g && f[X](i);
                i = f.getElementsByTagName("stroke") && f.getElementsByTagName("stroke")[0];
                g = false;
                !i && (g = i = Ea("stroke"));
                if (b.stroke && b.stroke != "none" || b["stroke-width"] || b["stroke-opacity"] != null || b["stroke-dasharray"] || b["stroke-miterlimit"] ||
                    b["stroke-linejoin"] || b["stroke-linecap"])i.on = true;
                (b.stroke == "none" || i.on == null || b.stroke == 0 || b["stroke-width"] == 0) && (i.on = false);
                a = e.getRGB(b.stroke);
                i.on && b.stroke && (i.color = a.hex);
                a = ((+h["stroke-opacity"] + 1 || 2) - 1) * ((+h.opacity + 1 || 2) - 1) * ((+a.o + 1 || 2) - 1);
                n = (ga(b["stroke-width"]) || 1) * 0.75;
                a < 0 && (a = 0);
                a > 1 && (a = 1);
                b["stroke-width"] == null && (n = h["stroke-width"]);
                b["stroke-width"] && (i.weight = n);
                n && n < 1 && (a *= n) && (i.weight = 1);
                i.opacity = a;
                b["stroke-linejoin"] && (i.joinstyle = b["stroke-linejoin"] || "miter");
                i.miterlimit =
                    b["stroke-miterlimit"] || 8;
                b["stroke-linecap"] && (i.endcap = b["stroke-linecap"] == "butt" ? "flat" : b["stroke-linecap"] == "square" ? "square" : "round");
                if (b["stroke-dasharray"]) {
                    a = {"-": "shortdash", ".": "shortdot", "-.": "shortdashdot", "-..": "shortdashdotdot", ". ": "dot", "- ": "dash", "--": "longdash", "- .": "dashdot", "--.": "longdashdot", "--..": "longdashdotdot"};
                    i.dashstyle = a[da](b["stroke-dasharray"]) ? a[b["stroke-dasharray"]] : v
                }
                g && f[X](i)
            }
            if (m.type == "text") {
                i = m.paper.span.style;
                h.font && (i.font = h.font);
                h["font-family"] &&
                (i.fontFamily = h["font-family"]);
                h["font-size"] && (i.fontSize = h["font-size"]);
                h["font-weight"] && (i.fontWeight = h["font-weight"]);
                h["font-style"] && (i.fontStyle = h["font-style"]);
                m.node.string && (m.paper.span.innerHTML = (m.node.string + v)[ta](/</g, "&#60;")[ta](/&/g, "&#38;")[ta](/\n/g, "<br>"));
                m.W = h.w = m.paper.span.offsetWidth;
                m.H = h.h = m.paper.span.offsetHeight;
                m.X = h.x;
                m.Y = h.y + ra(m.H / 2);
                switch (h["text-anchor"]) {
                    case "start":
                        m.node.style["v-text-align"] = "left";
                        m.bbx = ra(m.W / 2);
                        break;
                    case "end":
                        m.node.style["v-text-align"] =
                            "right";
                        m.bbx = -ra(m.W / 2);
                        break;
                    default:
                        m.node.style["v-text-align"] = "center";
                        break
                }
            }
        };
        Pa = function (a, b) {
            a.attrs = a.attrs || {};
            var f = "linear", h = ".5 .5";
            a.attrs.gradient = b;
            b = (b + v)[ta](hb, function (s, t, y) {
                f = "radial";
                if (t && y) {
                    t = ga(t);
                    y = ga(y);
                    ma(t - 0.5, 2) + ma(y - 0.5, 2) > 0.25 && (y = M.sqrt(0.25 - ma(t - 0.5, 2)) * ((y > 0.5) * 2 - 1) + 0.5);
                    h = t + A + y
                }
                return v
            });
            b = b[E](/\s*\-\s*/);
            if (f == "linear") {
                var i = b.shift();
                i = -ga(i);
                if (isNaN(i))return null
            }
            var g = gb(b);
            if (!g)return null;
            a = a.shape || a.node;
            b = a.getElementsByTagName("fill")[0] || Ea("fill");
            !b.parentNode && a.appendChild(b);
            if (g[B]) {
                b.on = true;
                b.method = "none";
                b.color = g[0].color;
                b.color2 = g[g[B] - 1].color;
                a = [];
                for (var m = 0, n = g[B]; m < n; m++)g[m].offset && a[na](g[m].offset + A + g[m].color);
                b.colors && (b.colors.value = a[B] ? a[Z]() : "0% " + b.color);
                if (f == "radial") {
                    b.type = "gradientradial";
                    b.focus = "100%";
                    b.focussize = h;
                    b.focusposition = h
                } else {
                    b.type = "gradient";
                    b.angle = (270 - i) % 360
                }
            }
            return 1
        };
        T = function (a, b, f) {
            this[0] = a;
            this.id = e._oid++;
            this.node = a;
            a.raphael = this;
            this.Y = this.X = 0;
            this.attrs = {};
            this.Group = b;
            this.paper =
                f;
            this._ = {tx: 0, ty: 0, rt: {deg: 0}, sx: 1, sy: 1};
            !f.bottom && (f.bottom = this);
            (this.prev = f.top) && (f.top.next = this);
            f.top = this;
            this.next = null
        };
        T[I].rotate = function (a, b, f) {
            if (this.removed)return this;
            if (a == null) {
                if (this._.rt.cx)return[this._.rt.deg, this._.rt.cx, this._.rt.cy][Z](A);
                return this._.rt.deg
            }
            a = (a + v)[E](la);
            if (a[B] - 1) {
                b = ga(a[1]);
                f = ga(a[2])
            }
            a = ga(a[0]);
            if (b != null)this._.rt.deg = a; else this._.rt.deg += a;
            f == null && (b = null);
            this._.rt.cx = b;
            this._.rt.cy = f;
            this.setBox(this.attrs, b, f);
            this.Group.style.rotation =
                this._.rt.deg;
            return this
        };
        T[I].setBox = function (a, b, f) {
            if (this.removed)return this;
            var h = this.Group.style, i = this.shape && this.shape.style || this.node.style;
            a = a || {};
            for (var g in a)if (a[da](g))this.attrs[g] = a[g];
            b = b || this._.rt.cx;
            f = f || this._.rt.cy;
            var m = this.attrs, n;
            switch (this.type) {
                case "circle":
                    a = m.cx - m.r;
                    g = m.cy - m.r;
                    n = m = m.r * 2;
                    break;
                case "ellipse":
                    a = m.cx - m.rx;
                    g = m.cy - m.ry;
                    n = m.rx * 2;
                    m = m.ry * 2;
                    break;
                case "image":
                    a = +m.x;
                    g = +m.y;
                    n = m.width || 0;
                    m = m.height || 0;
                    break;
                case "text":
                    this.textpath.v = ["m", ra(m.x), ", ",
                        ra(m.y - 2), "l", ra(m.x) + 1, ", ", ra(m.y - 2)][Z](v);
                    a = m.x - ra(this.W / 2);
                    g = m.y - this.H / 2;
                    n = this.W;
                    m = this.H;
                    break;
                case "rect":
                case "path":
                    if (this.attrs.path) {
                        m = Ya(this.attrs.path);
                        a = m.x;
                        g = m.y;
                        n = m.width;
                        m = m.height
                    } else {
                        g = a = 0;
                        n = this.paper.width;
                        m = this.paper.height
                    }
                    break;
                default:
                    g = a = 0;
                    n = this.paper.width;
                    m = this.paper.height;
                    break
            }
            b = b == null ? a + n / 2 : b;
            f = f == null ? g + m / 2 : f;
            b -= this.paper.width / 2;
            f -= this.paper.height / 2;
            var s;
            h.left != (s = b + "px") && (h.left = s);
            h.top != (s = f + "px") && (h.top = s);
            this.X = Ra[da](this.type) ? -b : a;
            this.Y =
                Ra[da](this.type) ? -f : g;
            this.W = n;
            this.H = m;
            if (Ra[da](this.type)) {
                i.left != (s = -b * 10 + "px") && (i.left = s);
                i.top != (s = -f * 10 + "px") && (i.top = s)
            } else if (this.type == "text") {
                i.left != (s = -b + "px") && (i.left = s);
                i.top != (s = -f + "px") && (i.top = s)
            } else {
                h.width != (s = this.paper.width + "px") && (h.width = s);
                h.height != (s = this.paper.height + "px") && (h.height = s);
                i.left != (s = a - b + "px") && (i.left = s);
                i.top != (s = g - f + "px") && (i.top = s);
                i.width != (s = n + "px") && (i.width = s);
                i.height != (s = m + "px") && (i.height = s)
            }
        };
        T[I].hide = function () {
            !this.removed && (this.Group.style.display =
                "none");
            return this
        };
        T[I].show = function () {
            !this.removed && (this.Group.style.display = "block");
            return this
        };
        T[I].getBBox = function () {
            if (this.removed)return this;
            if (Ra[da](this.type))return Ya(this.attrs.path);
            return{x: this.X + (this.bbx || 0), y: this.Y, width: this.W, height: this.H}
        };
        T[I].remove = function () {
            if (!this.removed) {
                D(this, this.paper);
                this.node.parentNode.removeChild(this.node);
                this.Group.parentNode.removeChild(this.Group);
                this.shape && this.shape.parentNode.removeChild(this.shape);
                for (var a in this)delete this[a];
                this.removed = true
            }
        };
        T[I].attr = function (a, b) {
            if (this.removed)return this;
            if (a == null) {
                a = {};
                for (var f in this.attrs)if (this.attrs[da](f))a[f] = this.attrs[f];
                this._.rt.deg && (a.rotation = this.rotate());
                (this._.sx != 1 || this._.sy != 1) && (a.scale = this.scale());
                a.gradient && a.fill == "none" && (a.fill = a.gradient) && delete a.gradient;
                return a
            }
            if (b == null && e.is(a, Da)) {
                if (a == "translation")return ua.call(this);
                if (a == "rotation")return this.rotate();
                if (a == "scale")return this.scale();
                if (a == "fill" && this.attrs.fill == "none" && this.attrs.gradient)return this.attrs.gradient;
                return this.attrs[a]
            }
            if (this.attrs && b == null && e.is(a, Ba)) {
                var h = {};
                f = 0;
                for (b = a[B]; f < b; f++)h[a[f]] = this.attr(a[f]);
                return h
            }
            if (b != null) {
                h = {};
                h[a] = b
            }
            b == null && e.is(a, "object") && (h = a);
            if (h) {
                if (h.text && this.type == "text")this.node.string = h.text;
                Ha(this, h);
                if (h.gradient && ({circle: 1, ellipse: 1}[da](this.type) || (h.gradient + v).charAt() != "r"))Pa(this, h.gradient);
                (!Ra[da](this.type) || this._.rt.deg) && this.setBox(this.attrs)
            }
            return this
        };
        T[I].toFront = function () {
            !this.removed && this.Group.parentNode[X](this.Group);
            this.paper.top != this && K(this, this.paper);
            return this
        };
        T[I].toBack = function () {
            if (this.removed)return this;
            if (this.Group.parentNode.firstChild != this.Group) {
                this.Group.parentNode.insertBefore(this.Group, this.Group.parentNode.firstChild);
                C(this, this.paper)
            }
            return this
        };
        T[I].insertAfter = function (a) {
            if (this.removed)return this;
            a.Group.nextSibling ? a.Group.parentNode.insertBefore(this.Group, a.Group.nextSibling) : a.Group.parentNode[X](this.Group);
            H(this, a, this.paper);
            return this
        };
        T[I].insertBefore = function (a) {
            if (this.removed)return this;
            a.Group.parentNode.insertBefore(this.Group, a.Group);
            O(this, a, this.paper);
            return this
        };
        var Fb = / progid:\S+Blur\([^\)]+\)/g;
        T[I].blur = function (a) {
            var b = this.node.style, f = b.filter;
            f = f.replace(Fb, "");
            if (+a !== 0) {
                this.attrs.blur = a;
                b.filter = f + " progid:DXImageTransform.Microsoft.Blur(pixelradius=" + (+a || 1.5) + ")";
                b.margin = Raphael.format("-{0}px 0 0 -{0}px", Math.round(+a || 1.5))
            } else {
                b.filter = f;
                b.margin = 0;
                delete this.attrs.blur
            }
        };
        jb = function (a, b, f, h) {
            var i = Ea("group"), g = Ea("oval");
            i.style.cssText = "position:absolute;left:0;top:0;width:" +
                a.width + "px;height:" + a.height + "px";
            i.coordsize = Qa;
            i.coordorigin = a.coordorigin;
            i[X](g);
            g = new T(g, i, a);
            g.type = "circle";
            Ha(g, {stroke: "#000", fill: "none"});
            g.attrs.cx = b;
            g.attrs.cy = f;
            g.attrs.r = h;
            g.setBox({x: b - h, y: f - h, width: h * 2, height: h * 2});
            a.canvas[X](i);
            return g
        };
        var qb = function (a, b, f, h, i) {
            return i ? e.format("M{0},{1}l{2},0a{3},{3},0,0,1,{3},{3}l0,{5}a{3},{3},0,0,1,{4},{3}l{6},0a{3},{3},0,0,1,{4},{4}l0,{7}a{3},{3},0,0,1,{3},{4}z", a + i, b, f - i * 2, i, -i, h - i * 2, i * 2 - f, i * 2 - h) : e.format("M{0},{1}l{2},0,0,{3},{4},0z",
                a, b, f, h, -f)
        };
        kb = function (a, b, f, h, i, g) {
            var m = qb(b, f, h, i, g);
            a = a.path(m);
            var n = a.attrs;
            a.X = n.x = b;
            a.Y = n.y = f;
            a.W = n.width = h;
            a.H = n.height = i;
            n.r = g;
            n.path = m;
            a.type = "rect";
            return a
        };
        lb = function (a, b, f, h, i) {
            var g = Ea("group"), m = Ea("oval");
            g.style.cssText = "position:absolute;left:0;top:0;width:" + a.width + "px;height:" + a.height + "px";
            g.coordsize = Qa;
            g.coordorigin = a.coordorigin;
            g[X](m);
            m = new T(m, g, a);
            m.type = "ellipse";
            Ha(m, {stroke: "#000"});
            m.attrs.cx = b;
            m.attrs.cy = f;
            m.attrs.rx = h;
            m.attrs.ry = i;
            m.setBox({x: b - h, y: f - i, width: h *
                2, height: i * 2});
            a.canvas[X](g);
            return m
        };
        mb = function (a, b, f, h, i, g) {
            var m = Ea("group"), n = Ea("image");
            m.style.cssText = "position:absolute;left:0;top:0;width:" + a.width + "px;height:" + a.height + "px";
            m.coordsize = Qa;
            m.coordorigin = a.coordorigin;
            n.src = b;
            m[X](n);
            n = new T(n, m, a);
            n.type = "image";
            n.attrs.src = b;
            n.attrs.x = f;
            n.attrs.y = h;
            n.attrs.w = i;
            n.attrs.h = g;
            n.setBox({x: f, y: h, width: i, height: g});
            a.canvas[X](m);
            return n
        };
        nb = function (a, b, f, h) {
            var i = Ea("group"), g = Ea("shape"), m = g.style, n = Ea("path"), s = Ea("textpath");
            i.style.cssText =
                "position:absolute;left:0;top:0;width:" + a.width + "px;height:" + a.height + "px";
            i.coordsize = Qa;
            i.coordorigin = a.coordorigin;
            n.v = e.format("m{0},{1}l{2},{1}", ra(b * 10), ra(f * 10), ra(b * 10) + 1);
            n.textpathok = true;
            m.width = a.width;
            m.height = a.height;
            s.string = h + v;
            s.on = true;
            g[X](s);
            g[X](n);
            i[X](g);
            m = new T(s, i, a);
            m.shape = g;
            m.textpath = n;
            m.type = "text";
            m.attrs.text = h;
            m.attrs.x = b;
            m.attrs.y = f;
            m.attrs.w = 1;
            m.attrs.h = 1;
            Ha(m, {font: Wa.font, stroke: "none", fill: "#000"});
            m.setBox();
            a.canvas[X](i);
            return m
        };
        ob = function (a, b) {
            var f = this.canvas.style;
            a == +a && (a += "px");
            b == +b && (b += "px");
            f.width = a;
            f.height = b;
            f.clip = "rect(0 " + a + " " + b + " 0)";
            return this
        };
        var Ea;
        ia.createStyleSheet().addRule(".rvml", "behavior:url(#default#VML)");
        try {
            !ia.namespaces.rvml && ia.namespaces.add("rvml", "urn:schemas-microsoft-com:vml");
            Ea = function (a) {
                return ia.createElement("<rvml:" + a + ' class="rvml">')
            }
        } catch (Ib) {
            Ea = function (a) {
                return ia.createElement("<" + a + ' xmlns="urn:schemas-microsoft.com:vml" class="rvml">')
            }
        }
        Za = function () {
            var a = G[qa](0, arguments), b = a.container, f = a.height,
                h = a.width, i = a.x;
            a = a.y;
            if (!b)throw Error("VML container not found.");
            var g = new c, m = g.canvas = ia.createElement("div"), n = m.style;
            i = i || 0;
            a = a || 0;
            h = h || 512;
            f = f || 342;
            g.twidth = h/2;
            g.theight = f/2;
            h == +h && (h += "px");
            f == +f && (f += "px");
            g.width = 1E3;
            g.height = 1E3;
            g.coordsize = 1E4 + A + 1E4;
            g.coordorigin = "0 0";
            g.span = ia.createElement("span");
            g.span.style.cssText = "position:absolute;left:-9999em;top:-9999em;padding:0;margin:0;line-height:1;display:inline;";
            m[X](g.span);
            n.cssText = e.format("width:{0};height:{1};display:inline-block;position:relative;clip:rect(0 {0} {1} 0);overflow:hidden",
                h, f);
            if (b == 1) {
                ia.body[X](m);
                n.left = i + "px";
                n.top = a + "px";
                n.position = "absolute"
            } else b.firstChild ? b.insertBefore(m, b.firstChild) : b[X](m);
            w.call(g, g, e.fn);
            return g
        };
        c[I].clear = function () {
            this.canvas.innerHTML = v;
            this.span = ia.createElement("span");
            this.span.style.cssText = "position:absolute;left:-9999em;top:-9999em;padding:0;margin:0;line-height:1;display:inline;";
            this.canvas[X](this.span);
            this.bottom = this.top = null
        };
        c[I].remove = function () {
            this.canvas.parentNode.removeChild(this.canvas);
            for (var a in this)this[a] =
                P(a);
            return true
        }
    }
    c[I].safari = /^Apple|^Google/.test(za.navigator.vendor) && (!(za.navigator.userAgent.indexOf("Version/4.0") + 1) || za.navigator.platform.slice(0, 2) == "iP") ? function () {
        var a = this.rect(-99, -99, this.width + 99, this.height + 99);
        za.setTimeout(function () {
            a.remove()
        })
    } : function () {
    };
    var Gb = function () {
        if (ia.addEventListener)return function (a, b, f, h) {
            function i(m) {
                if (W && U[da](b))for (var n = 0, s = m.targetTouches && m.targetTouches.length; n < s; n++)if (m.targetTouches[n].target == a) {
                    s = m;
                    m = m.targetTouches[n];
                    m.originalEvent =
                        s;
                    m.preventDefault = Q;
                    m.stopPropagation = fa;
                    break
                }
                return f.call(h, m)
            }

            var g = W && U[b] ? U[b] : b;
            a.addEventListener(g, i, false);
            return function () {
                a.removeEventListener(g, i, false);
                return true
            }
        }; else if (ia.attachEvent)return function (a, b, f, h) {
            function i(g) {
                g = g || za.event;
                g.preventDefault = g.preventDefault || ea;
                g.stopPropagation = g.stopPropagation || ca;
                return f.call(h, g)
            }

            a.attachEvent("on" + b, i);
            return function () {
                a.detachEvent("on" + b, i);
                return true
            }
        }
    }();
    for (Na = R[B]; Na--;)(function (a) {
        e[a] = T[I][a] = function (b) {
            if (e.is(b,
                "function")) {
                this.events = this.events || [];
                this.events.push({name: a, f: b, unbind: Gb(this.shape || this.node || ia, a, b, this)})
            }
            return this
        };
        e["un" + a] = T[I]["un" + a] = function (b) {
            for (var f = this.events, h = f[B]; h--;)if (f[h].name == a && f[h].f == b) {
                f[h].unbind();
                f.splice(h, 1);
                !f.length && delete this.events;
                return this
            }
            return this
        }
    })(R[Na]);
    T[I].hover = function (a, b) {
        return this.mouseover(a).mouseout(b)
    };
    T[I].unhover = function (a, b) {
        return this.unmouseover(a).unmouseout(b)
    };
    T[I].drag = function (a, b, f) {
        function h(m) {
            var n = m.clientX,
                s = m.clientY;
            if (W)for (var t = m.touches.length, y; t--;) {
                y = m.touches[t];
                if (y.identifier == g._drag.id) {
                    n = y.clientX;
                    s = y.clientY;
                    (m.originalEvent ? m.originalEvent : m).preventDefault();
                    break
                }
            } else m.preventDefault();
            a && a.call(g, n - g._drag.x, s - g._drag.y, n, s)
        }

        function i() {
            g._drag = {};
            Raphael.unmousemove(h).unmouseup(i);
            f && f.call(g)
        }

        this._drag = {};
        var g = this.mousedown(function (m) {
            (m.originalEvent ? m.originalEvent : m).preventDefault();
            this._drag.x = m.clientX;
            this._drag.y = m.clientY;
            this._drag.id = m.identifier;
            b && b.call(this,
                m.clientX, m.clientY);
            Raphael.mousemove(h).mouseup(i)
        });
        return this
    };
    c[I].circle = function (a, b, f) {
        return jb(this, a || 0, b || 0, f || 0)
    };
    c[I].rect = function (a, b, f, h, i) {
        return kb(this, a || 0, b || 0, f || 0, h || 0, i || 0)
    };
    c[I].ellipse = function (a, b, f, h) {
        return lb(this, a || 0, b || 0, f || 0, h || 0)
    };
    c[I].path = function (a) {
        a && !e.is(a, Da) && !e.is(a[0], Ba) && (a += v);
        return ib(e.format[qa](e, arguments), this)
    };
    c[I].image = function (a, b, f, h, i) {
        return mb(this, a || "about:blank", b || 0, f || 0, h || 0, i || 0)
    };
    c[I].text = function (a, b, f) {
        return nb(this, a ||
            0, b || 0, f || v)
    };
    c[I].set = function (a) {
        arguments[B] > 1 && (a = Array[I].splice.call(arguments, 0, arguments[B]));
        return new wa(a)
    };
    c[I].setSize = ob;
    c[I].top = c[I].bottom = null;
    c[I].raphael = e;
    T[I].resetScale = function () {
        if (this.removed)return this;
        this._.sx = 1;
        this._.sy = 1;
        this.attrs.scale = "1 1"
    };
    T[I].scale = function (a, b, f, h) {
        if (this.removed)return this;
        if (a == null && b == null)return{x: this._.sx, y: this._.sy, toString: aa};
        b = b || a;
        !+b && (b = a);
        var i, g, m = this.attrs;
        if (a != 0) {
            var n = this.getBBox(), s = n.x + n.width / 2, t = n.y + n.height /
                2;
            i = a / this._.sx;
            g = b / this._.sy;
            f = +f || f == 0 ? f : s;
            h = +h || h == 0 ? h : t;
            n = ~~(a / M.abs(a));
            var y = ~~(b / M.abs(b)), F = this.node.style, L = f + (s - f) * i;
            t = h + (t - h) * g;
            switch (this.type) {
                case "rect":
                case "image":
                    var J = m.width * n * i, x = m.height * y * g;
                    this.attr({height: x, r: m.r * Ga(n * i, y * g), width: J, x: L - J / 2, y: t - x / 2});
                    break;
                case "circle":
                case "ellipse":
                    this.attr({rx: m.rx * n * i, ry: m.ry * y * g, r: m.r * Ga(n * i, y * g), cx: L, cy: t});
                    break;
                case "text":
                    this.attr({x: L, y: t});
                    break;
                case "path":
                    s = eb(m.path);
                    for (var S = true, Aa = 0, ja = s[B]; Aa < ja; Aa++) {
                        var ba = s[Aa],
                            ya = Va.call(ba[0]);
                        if (!(ya == "M" && S)) {
                            S = false;
                            if (ya == "A") {
                                ba[s[Aa][B] - 2] *= i;
                                ba[s[Aa][B] - 1] *= g;
                                ba[1] *= n * i;
                                ba[2] *= y * g;
                                ba[5] = +!(n + y ? !+ba[5] : +ba[5])
                            } else if (ya == "H") {
                                ya = 1;
                                for (var Ja = ba[B]; ya < Ja; ya++)ba[ya] *= i
                            } else if (ya == "V") {
                                ya = 1;
                                for (Ja = ba[B]; ya < Ja; ya++)ba[ya] *= g
                            } else {
                                ya = 1;
                                for (Ja = ba[B]; ya < Ja; ya++)ba[ya] *= ya % 2 ? i : g
                            }
                        }
                    }
                    g = Ya(s);
                    i = L - g.x - g.width / 2;
                    g = t - g.y - g.height / 2;
                    s[0][1] += i;
                    s[0][2] += g;
                    this.attr({path: s});
                    break
            }
            if (this.type in{text: 1, image: 1} && (n != 1 || y != 1))if (this.transformations) {
                this.transformations[2] = "scale("[xa](n,
                    ",", y, ")");
                this.node.setAttribute("transform", this.transformations[Z](A));
                i = n == -1 ? -m.x - (J || 0) : m.x;
                g = y == -1 ? -m.y - (x || 0) : m.y;
                this.attr({x: i, y: g});
                m.fx = n - 1;
                m.fy = y - 1
            } else {
                this.node.filterMatrix = " progid:DXImageTransform.Microsoft" + ".Matrix(M11="[xa](n, ", M12=0, M21=0, M22=", y, ", Dx=0, Dy=0, sizingmethod='auto expand', filtertype='bilinear')");
                F.filter = (this.node.filterMatrix || v) + (this.node.filterOpacity || v)
            } else if (this.transformations) {
                this.transformations[2] = v;
                this.node.setAttribute("transform", this.transformations[Z](A));
                m.fx = 0;
                m.fy = 0
            } else {
                this.node.filterMatrix = v;
                F.filter = (this.node.filterMatrix || v) + (this.node.filterOpacity || v)
            }
            m.scale = [a, b, f, h][Z](A);
            this._.sx = a;
            this._.sy = b
        }
        return this
    };
    T[I].clone = function () {
        if (this.removed)return null;
        var a = this.attr();
        delete a.scale;
        delete a.translation;
        return this.paper[this.type]().attr(a)
    };
    var db = l(function (a, b, f, h, i, g, m, n, s) {
            for (var t = 0, y, F = 0; F < 1.001; F += 0.0010) {
                var L = e.findDotsAtSegment(a, b, f, h, i, g, m, n, F);
                F && (t += ma(ma(y.x - L.x, 2) + ma(y.y - L.y, 2), 0.5));
                if (t >= s)return L;
                y = L
            }
        }),
        tb = l(function (a, b, f, h, i, g, m, n) {
            for (var s = {x: 0, y: 0}, t = 0, y = 0; y < 1.01; y += 0.01) {
                var F = z(a, b, f, h, i, g, m, n, y);
                y && (t += ma(ma(s.x - F.x, 2) + ma(s.y - F.y, 2), 0.5));
                s = F
            }
            return t
        }), rb = N(1), Ua = N(), bb = N(0, 1);
    T[I].getTotalLength = function () {
        if (this.type == "path") {
            if (this.node.getTotalLength)return this.node.getTotalLength();
            return rb(this.attrs.path)
        }
    };
    T[I].getPointAtLength = function (a) {
        if (this.type == "path")return Ua(this.attrs.path, a)
    };
    T[I].getSubpath = function (a, b) {
        if (this.type == "path") {
            if (M.abs(this.getTotalLength() - b) <
                1.0E-6)return bb(this.attrs.path, a).end;
            b = bb(this.attrs.path, b, 1);
            return a ? bb(b, a).end : b
        }
    };
    e.easing_formulas = {linear: function (a) {
        return a
    }, "<": function (a) {
        return ma(a, 3)
    }, ">": function (a) {
        return ma(a - 1, 3) + 1
    }, "<>": function (a) {
        a *= 2;
        if (a < 1)return ma(a, 3) / 2;
        a -= 2;
        return(ma(a, 3) + 2) / 2
    }, backIn: function (a) {
        return a * a * (2.70158 * a - 1.70158)
    }, backOut: function (a) {
        a -= 1;
        return a * a * (2.70158 * a + 1.70158) + 1
    }, elastic: function (a) {
        if (a == 0 || a == 1)return a;
        return ma(2, -10 * a) * M.sin((a - 0.075) * 2 * M.PI / 0.3) + 1
    }, bounce: function (a) {
        if (a <
            1 / 2.75)a = 7.5625 * a * a; else if (a < 2 / 2.75) {
            a -= 1.5 / 2.75;
            a = 7.5625 * a * a + 0.75
        } else if (a < 2.5 / 2.75) {
            a -= 2.25 / 2.75;
            a = 7.5625 * a * a + 0.9375
        } else {
            a -= 2.625 / 2.75;
            a = 7.5625 * a * a + 0.984375
        }
        return a
    }};
    var Fa = {length: 0};
    T[I].animateWith = function (a, b, f, h, i) {
        Fa[a.id] && (b.start = Fa[a.id].start);
        return this.animate(b, f, h, i)
    };
    T[I].animateAlong = va();
    T[I].animateAlongBack = va(1);
    T[I].onAnimation = function (a) {
        this._run = a || 0;
        return this
    };
    T[I].animate = function (a, b, f, h) {
        if (e.is(f, "function") || !f)h = f || null;
        var i = {}, g = {}, m = {};
        for (var n in a)if (a[da](n))if ($a[da](n)) {
            i[n] =
                this.attr(n);
            i[n] == null && (i[n] = Wa[n]);
            g[n] = a[n];
            switch ($a[n]) {
                case "along":
                    var s = rb(a[n]), t = Ua(a[n], s * !!a.back), y = this.getBBox();
                    m[n] = s / b;
                    m.tx = y.x;
                    m.ty = y.y;
                    m.sx = t.x;
                    m.sy = t.y;
                    g.rot = a.rot;
                    g.back = a.back;
                    g.len = s;
                    a.rot && (m.r = ga(this.rotate()) || 0);
                    break;
                case pa:
                    m[n] = (g[n] - i[n]) / b;
                    break;
                case "colour":
                    i[n] = e.getRGB(i[n]);
                    s = e.getRGB(g[n]);
                    m[n] = {r: (s.r - i[n].r) / b, g: (s.g - i[n].g) / b, b: (s.b - i[n].b) / b};
                    break;
                case "path":
                    s = Ta(i[n], g[n]);
                    i[n] = s[0];
                    t = s[1];
                    m[n] = [];
                    s = 0;
                    for (y = i[n][B]; s < y; s++) {
                        m[n][s] = [0];
                        for (var F = 1, L =
                            i[n][s][B]; F < L; F++)m[n][s][F] = (t[s][F] - i[n][s][F]) / b
                    }
                    break;
                case "csv":
                    t = (a[n] + v)[E](la);
                    s = (i[n] + v)[E](la);
                    switch (n) {
                        case "translation":
                            i[n] = [0, 0];
                            m[n] = [t[0] / b, t[1] / b];
                            break;
                        case "rotation":
                            i[n] = s[1] == t[1] && s[2] == t[2] ? s : [0, t[1], t[2]];
                            m[n] = [(t[0] - i[n][0]) / b, 0, 0];
                            break;
                        case "scale":
                            a[n] = t;
                            i[n] = (i[n] + v)[E](la);
                            m[n] = [(t[0] - i[n][0]) / b, (t[1] - i[n][1]) / b, 0, 0];
                            break;
                        case "clip-rect":
                            i[n] = (i[n] + v)[E](la);
                            m[n] = [];
                            for (s = 4; s--;)m[n][s] = (t[s] - i[n][s]) / b;
                            break
                    }
                    g[n] = t
            }
        }
        this.stop();
        this.in_animation = 1;
        Fa[this.id] =
        {start: a.start || +new Date, ms: b, easing: f, from: i, diff: m, to: g, el: this, callback: h, t: {x: 0, y: 0}};
        ++Fa[B] == 1 && ha();
        return this
    };
    T[I].stop = function () {
        Fa[this.id] && Fa[B]--;
        delete Fa[this.id];
        return this
    };
    T[I].translate = function (a, b) {
        return this.attr({translation: a + " " + b})
    };
    T[I][sa] = function () {
        return"Rapha\u00ebl\u2019s object"
    };
    e.ae = Fa;
    wa[I][na] = function () {
        for (var a, b, f = 0, h = arguments[B]; f < h; f++)if ((a = arguments[f]) && (a.constructor == T || a.constructor == wa)) {
            b = this.items[B];
            this[b] = this.items[b] = a;
            this[B]++
        }
        return this
    };
    wa[I].pop = function () {
        delete this[this[B]--];
        return this.items.pop()
    };
    for (var cb in T[I])if (T[I][da](cb))wa[I][cb] = function (a) {
        return function () {
            for (var b = 0, f = this.items[B]; b < f; b++)this.items[b][a][qa](this.items[b], arguments);
            return this
        }
    }(cb);
    wa[I].attr = function (a, b) {
        if (a && e.is(a, Ba) && e.is(a[0], "object")) {
            b = 0;
            for (var f = a[B]; b < f; b++)this.items[b].attr(a[b])
        } else {
            f = 0;
            for (var h = this.items[B]; f < h; f++)this.items[f].attr(a, b)
        }
        return this
    };
    wa[I].animate = function (a, b, f, h) {
        (e.is(f, "function") || !f) && (h = f ||
            null);
        var i = this.items[B], g = i, m, n = this, s;
        h && (s = function () {
            !--i && h.call(n)
        });
        f = e.is(f, Da) ? f : s;
        for (m = this.items[--g].animate(a, b, f, s); g--;)this.items[g].animateWith(m, a, b, f, s);
        return this
    };
    wa[I].insertAfter = function (a) {
        for (var b = this.items[B]; b--;)this.items[b].insertAfter(a);
        return this
    };
    wa[I].getBBox = function () {
        for (var a = [], b = [], f = [], h = [], i = this.items[B]; i--;) {
            var g = this.items[i].getBBox();
            a[na](g.x);
            b[na](g.y);
            f[na](g.x + g.width);
            h[na](g.y + g.height)
        }
        a = Ga[qa](0, a);
        b = Ga[qa](0, b);
        return{x: a, y: b, width: Ca[qa](0,
            f) - a, height: Ca[qa](0, h) - b}
    };
    wa[I].clone = function (a) {
        a = new wa;
        for (var b = 0, f = this.items[B]; b < f; b++)a[na](this.items[b].clone());
        return a
    };
    e.registerFont = function (a) {
        if (!a.face)return a;
        this.fonts = this.fonts || {};
        var b = {w: a.w, face: {}, glyphs: {}}, f = a.face["font-family"];
        for (var h in a.face)if (a.face[da](h))b.face[h] = a.face[h];
        if (this.fonts[f])this.fonts[f][na](b); else this.fonts[f] = [b];
        if (!a.svg) {
            b.face["units-per-em"] = Ia(a.face["units-per-em"], 10);
            for (var i in a.glyphs)if (a.glyphs[da](i)) {
                f = a.glyphs[i];
                b.glyphs[i] = {w: f.w, k: {}, d: f.d && "M" + f.d[ta](/[mlcxtrv]/g, function (m) {
                    return{l: "L", c: "C", x: "z", t: "m", r: "l", v: "c"}[m] || "M"
                }) + "z"};
                if (f.k)for (var g in f.k)if (f[da](g))b.glyphs[i].k[g] = f.k[g]
            }
        }
        return a
    };
    c[I].getFont = function (a, b, f, h) {
        h = h || "normal";
        f = f || "normal";
        b = +b || {normal: 400, bold: 700, lighter: 300, bolder: 800}[b] || 400;
        if (e.fonts) {
            var i = e.fonts[a];
            if (!i) {
                a = RegExp("(^|\\s)" + a[ta](/[^\w\d\s+!~.:_-]/g, v) + "(\\s|$)", "i");
                for (var g in e.fonts)if (e.fonts[da](g))if (a.test(g)) {
                    i = e.fonts[g];
                    break
                }
            }
            var m;
            if (i) {
                g =
                    0;
                for (a = i[B]; g < a; g++) {
                    m = i[g];
                    if (m.face["font-weight"] == b && (m.face["font-style"] == f || !m.face["font-style"]) && m.face["font-stretch"] == h)break
                }
            }
            return m
        }
    };
    c[I].print = function (a, b, f, h, i, g) {
        g = g || "middle";
        var m = this.set(), n = (f + v)[E](v), s = 0;
        e.is(h, f) && (h = this.getFont(h));
        if (h) {
            f = (i || 16) / h.face["units-per-em"];
            var t = h.face.bbox.split(la);
            i = +t[0];
            g = +t[1] + (g == "baseline" ? t[3] - t[1] + +h.face.descent : (t[3] - t[1]) / 2);
            t = 0;
            for (var y = n[B]; t < y; t++) {
                var F = t && h.glyphs[n[t - 1]] || {}, L = h.glyphs[n[t]];
                s += t ? (F.w || h.w) + (F.k &&
                    F.k[n[t]] || 0) : 0;
                L && L.d && m[na](this.path(L.d).attr({fill: "#000", stroke: "none", translation: [s, 0]}))
            }
            m.scale(f, f, i, g).translate(a - i, b - g)
        }
        return m
    };
    var Hb = /\{(\d+)\}/g;
    e.format = function (a, b) {
        var f = e.is(b, Ba) ? [0][xa](b) : arguments;
        a && e.is(a, Da) && f[B] - 1 && (a = a[ta](Hb, function (h, i) {
            return f[++i] == null ? v : f[i]
        }));
        return a || v
    };
    e.ninja = function () {
        Ka.was ? Raphael = Ka.is : delete Raphael;
        return e
    };
    e.el = T[I];
    return e
}();
(function () {
    var e = Math.max, c = Math.min;
    Raphael.fn.g = Raphael.fn.g || {};
    Raphael.fn.g.markers = {disc: "disc", o: "disc", flower: "flower", f: "flower", diamond: "diamond", d: "diamond", square: "square", s: "square", triangle: "triangle", t: "triangle", star: "star", "*": "star", cross: "cross", x: "cross", plus: "plus", "+": "plus", arrow: "arrow", "->": "arrow"};
    Raphael.fn.g.shim = {stroke: "none", fill: "#000", "fill-opacity": 0};
    Raphael.fn.g.txtattr = {font: "12px Arial, sans-serif"};
    Raphael.fn.g.colors = [];
    for (var q = [0.6, 0.2, 0.05, 0.1333, 0.75,
        0], j = 0; j < 10; j++)j < q.length ? Raphael.fn.g.colors.push("hsb(" + q[j] + ", .75, .75)") : Raphael.fn.g.colors.push("hsb(" + q[j - q.length] + ", 1, .5)");
    Raphael.fn.g.text = function (o, l, p) {
        return this.text(o, l, p).attr(this.g.txtattr)
    };
    Raphael.fn.g.labelise = function (o, l, p) {
        return o ? (o + "").replace(/(##+(?:\.#+)?)|(%%+(?:\.%+)?)/g, function (u, k, z) {
            if (k)return(+l).toFixed(k.replace(/^#+\.?/g, "").length);
            if (z)return(l * 100 / p).toFixed(z.replace(/^%+\.?/g, "").length) + "%"
        }) : (+l).toFixed(0)
    };
    Raphael.fn.g.finger = function (o, l, p, u, k, z, G) {
        if (k && !u || !k && !p)return G ? "" : this.path();
        z = {square: "square", sharp: "sharp", soft: "soft"}[z] || "round";
        var w;
        u = Math.round(u);
        p = Math.round(p);
        o = Math.round(o);
        l = Math.round(l);
        switch (z) {
            case "round":
                if (k) {
                    k = ~~(p / 2);
                    if (u < k) {
                        k = u;
                        w = ["M", o - ~~(p / 2), l, "l", 0, 0, "a", ~~(p / 2), k, 0, 0, 1, p, 0, "l", 0, 0, "z"]
                    } else w = ["M", o - k, l, "l", 0, k - u, "a", k, k, 0, 1, 1, p, 0, "l", 0, u - k, "z"]
                } else {
                    k = ~~(u / 2);
                    if (p < k) {
                        k = p;
                        w = ["M", o + 0.5, l + 0.5 - ~~(u / 2), "l", 0, 0, "a", k, ~~(u / 2), 0, 0, 1, 0, u, "l", 0, 0, "z"]
                    } else w = ["M", o + 0.5, l + 0.5 - k, "l", p - k, 0, "a", k, k, 0,
                        1, 1, 0, u, "l", k - p, 0, "z"]
                }
                break;
            case "sharp":
                if (k) {
                    k = ~~(p / 2);
                    w = ["M", o + k, l, "l", -p, 0, 0, -e(u - k, 0), k, -c(k, u), k, c(k, u), k, "z"]
                } else {
                    k = ~~(u / 2);
                    w = ["M", o, l + k, "l", 0, -u, e(p - k, 0), 0, c(k, p), k, -c(k, p), k + (k * 2 < u), "z"]
                }
                break;
            case "square":
                w = k ? ["M", o + ~~(p / 2), l, "l", 1 - p, 0, 0, -u, p - 1, 0, "z"] : ["M", o, l + ~~(u / 2), "l", 0, -u, p, 0, 0, u, "z"];
                break;
            case "soft":
                if (k) {
                    k = c(Math.round(p / 5), u);
                    w = ["M", o - ~~(p / 2), l, "l", 0, k - u, "a", k, k, 0, 0, 1, k, -k, "l", p - 2 * k, 0, "a", k, k, 0, 0, 1, k, k, "l", 0, u - k, "z"]
                } else {
                    k = c(p, Math.round(u / 5));
                    w = ["M", o + 0.5, l + 0.5 - ~~(u / 2), "l",
                            p - k, 0, "a", k, k, 0, 0, 1, k, k, "l", 0, u - k * 2, "a", k, k, 0, 0, 1, -k, k, "l", k - p, 0, "z"]
                }
        }
        return G ? w.join(",") : this.path(w)
    };
    Raphael.fn.g.disc = function (o, l, p) {
        return this.circle(o, l, p)
    };
    Raphael.fn.g.line = function (o, l, p) {
        return this.rect(o - p, l - p / 5, 2 * p, 2 * p / 5)
    };
    Raphael.fn.g.square = function (o, l, p) {
        p *= 0.7;
        return this.rect(o - p, l - p, 2 * p, 2 * p)
    };
    Raphael.fn.g.triangle = function (o, l, p) {
        p *= 1.75;
        return this.path("M".concat(o, ",", l, "m0-", p * 0.58, "l", p * 0.5, ",", p * 0.87, "-", p, ",0z"))
    };
    Raphael.fn.g.diamond = function (o, l, p) {
        return this.path(["M",
            o, l - p, "l", p, p, -p, p, -p, -p, p, -p, "z"])
    };
    Raphael.fn.g.flower = function (o, l, p, u) {
        p *= 1.25;
        p = p;
        var k = p * 0.5;
        u = +u < 3 || !u ? 5 : u;
        for (var z = ["M", o, l + k, "Q"], G, w = 1; w < u * 2 + 1; w++) {
            G = w % 2 ? p : k;
            z = z.concat([+(o + G * Math.sin(w * Math.PI / u)).toFixed(3), +(l + G * Math.cos(w * Math.PI / u)).toFixed(3)])
        }
        z.push("z");
        return this.path(z.join(","))
    };
    Raphael.fn.g.star = function (o, l, p, u, k) {
        u = u || p * 0.382;
        k = k || 5;
        for (var z = ["M", o, l + u, "L"], G, w = 1; w < k * 2; w++) {
            G = w % 2 ? p : u;
            z = z.concat([o + G * Math.sin(w * Math.PI / k), l + G * Math.cos(w * Math.PI / k)])
        }
        z.push("z");
        return this.path(z.join(","))
    };
    Raphael.fn.g.cross = function (o, l, p) {
        p /= 2.5;
        return this.path("M".concat(o - p, ",", l, "l", [-p, -p, p, -p, p, p, p, -p, p, p, -p, p, p, p, -p, p, -p, -p, -p, p, -p, -p, "z"]))
    };
    Raphael.fn.g.plus = function (o, l, p) {
        p /= 2;
        return this.path("M".concat(o - p / 2, ",", l - p / 2, "l", [0, -p, p, 0, 0, p, p, 0, 0, p, -p, 0, 0, p, -p, 0, 0, -p, -p, 0, 0, -p, "z"]))
    };
    Raphael.fn.g.arrow = function (o, l, p) {
        return this.path("M".concat(o - p * 0.7, ",", l - p * 0.4, "l", [p * 0.6, 0, 0, -p * 0.4, p, p * 0.8, -p, p * 0.8, 0, -p * 0.4, -p * 0.6, 0], "z"))
    };
    Raphael.fn.g.tag = function (o, l, p, u, k) {
        u = u || 0;
        k = k == null ? 5 : k;
        p =
                p == null ? "$9.99" : p;
        var z = 0.5522 * k, G = this.set();
        G.push(this.path().attr({fill: "#000", stroke: "#000"}));
        G.push(this.text(o, l, p).attr(this.g.txtattr).attr({fill: "#fff", "font-family": "Helvetica, Arial"}));
        G.update = function () {
            this.rotate(0, o, l);
            var w = this[1].getBBox();
            if (w.height >= k * 2)this[0].attr({path: ["M", o, l + k, "a", k, k, 0, 1, 1, 0, -k * 2, k, k, 0, 1, 1, 0, k * 2, "m", 0, -k * 2 - 3, "a", k + 3, k + 3, 0, 1, 0, 0, (k + 3) * 2, "L", o + k + 3, l + w.height / 2 + 3, "l", w.width + 6, 0, 0, -w.height - 6, -w.width - 6, 0, "L", o, l - k - 3].join(",")}); else {
                var D = Math.sqrt(Math.pow(k +
                    3, 2) - Math.pow(w.height / 2 + 3, 2));
                this[0].attr({path: ["M", o, l + k, "c", -z, 0, -k, z - k, -k, -k, 0, -z, k - z, -k, k, -k, z, 0, k, k - z, k, k, 0, z, z - k, k, -k, k, "M", o + D, l - w.height / 2 - 3, "a", k + 3, k + 3, 0, 1, 0, 0, w.height + 6, "l", k + 3 - D + w.width + 6, 0, 0, -w.height - 6, "L", o + D, l - w.height / 2 - 3].join(",")})
            }
            this[1].attr({x: o + k + 3 + w.width / 2, y: l});
            u = (360 - u) % 360;
            this.rotate(u, o, l);
            u > 90 && u < 270 && this[1].attr({x: o - k - 3 - w.width / 2, y: l, rotation: [180 + u, o, l]});
            return this
        };
        G.update();
        return G
    };
    Raphael.fn.g.popupit = function (o, l, p, u, k) {
        u = u == null ? 2 : u;
        k = k || 5;
        o = Math.round(o);
        l = Math.round(l);
        var z = p.getBBox(), G = Math.round(z.width / 2), w = Math.round(z.height / 2), D = ["M", o - [0, G + k * 2, 0, -G - k * 2][u], l - [-w * 2 - k * 3, -w - k, 0, -w - k][u], "l", -k, (u == 2) * -k, -e(G - k, 0), 0, "a", k, k, 0, 0, 1, -k, -k, "l", 0, -e(w - k, 0), (u == 3) * -k, -k, (u == 3) * k, -k, 0, -e(w - k, 0), "a", k, k, 0, 0, 1, k, -k, "l", e(G - k, 0), 0, k, !u * -k, k, !u * k, e(G - k, 0), 0, "a", k, k, 0, 0, 1, k, k, "l", 0, e(w - k, 0), (u == 1) * k, k, (u == 1) * -k, k, 0, e(w - k, 0), "a", k, k, 0, 0, 1, -k, k, "l", -e(G - k, 0), 0, "z"].join(",");
        o = [
            {x: o, y: l + k * 2 + w},
            {x: o - k * 2 - G, y: l},
            {x: o, y: l - k * 2 - w},
            {x: o + k * 2 + G, y: l}
        ][u];
        p.translate(o.x -
            G - z.x, o.y - w - z.y);
        return this.path(D).attr({fill: "#000", stroke: "none"}).insertBefore(p.node ? p : p[0])
    };
    Raphael.fn.g.popup = function (o, l, p, u, k) {
        u = u == null ? 2 : u > 3 ? 3 : u;
        k = k || 5;
        p = p || "$9.99";
        var z = this.set();
        z.push(this.path().attr({fill: "#000", stroke: "#000"}));
        z.push(this.text(o, l, p).attr(this.g.txtattr).attr({fill: "#fff", "font-family": "Helvetica, Arial"}));
        z.update = function (G, w, D) {
            G = G || o;
            w = w || l;
            var K = this[1].getBBox(), C = K.width / 2, H = K.height / 2;
            K = ["M", G - [0, C + k * 2, 0, -C - k * 2][u], w - [-H * 2 - k * 3, -H - k, 0, -H - k][u], "l", -k,
                    (u == 2) * -k, -e(C - k, 0), 0, "a", k, k, 0, 0, 1, -k, -k, "l", 0, -e(H - k, 0), (u == 3) * -k, -k, (u == 3) * k, -k, 0, -e(H - k, 0), "a", k, k, 0, 0, 1, k, -k, "l", e(C - k, 0), 0, k, !u * -k, k, !u * k, e(C - k, 0), 0, "a", k, k, 0, 0, 1, k, k, "l", 0, e(H - k, 0), (u == 1) * k, k, (u == 1) * -k, k, 0, e(H - k, 0), "a", k, k, 0, 0, 1, -k, k, "l", -e(C - k, 0), 0, "z"].join(",");
            G = [
                {x: G, y: w + k * 2 + H},
                {x: G - k * 2 - C, y: w},
                {x: G, y: w - k * 2 - H},
                {x: G + k * 2 + C, y: w}
            ][u];
            G.path = K;
            D ? this.animate(G, 500, ">") : this.attr(G);
            return this
        };
        return z.update(o, l)
    };
    Raphael.fn.g.flag = function (o, l, p, u) {
        u = u || 0;
        p = p || "$9.99";
        var k = this.set();
        k.push(this.path().attr({fill: "#000",
            stroke: "#000"}));
        k.push(this.text(o, l, p).attr(this.g.txtattr).attr({fill: "#fff", "font-family": "Helvetica, Arial"}));
        k.update = function (z, G) {
            this.rotate(0, z, G);
            var w = this[1].getBBox(), D = w.height / 2;
            this[0].attr({path: ["M", z, G, "l", D + 3, -D - 3, w.width + 6, 0, 0, w.height + 6, -w.width - 6, 0, "z"].join(",")});
            this[1].attr({x: z + D + 3 + w.width / 2, y: G});
            u = 360 - u;
            this.rotate(u, z, G);
            u > 90 && u < 270 && this[1].attr({x: z - r - 3 - w.width / 2, y: G, rotation: [180 + u, z, G]});
            return this
        };
        return k.update(o, l)
    };
    Raphael.fn.g.label = function (o, l, p) {
        var u =
            this.set();
        u.push(this.rect(o, l, 10, 10).attr({stroke: "none", fill: "#000"}));
        u.push(this.text(o, l, p).attr(this.g.txtattr).attr({fill: "#fff"}));
        u.update = function () {
            var k = this[1].getBBox(), z = c(k.width + 10, k.height + 10) / 2;
            this[0].attr({x: k.x - z / 2, y: k.y - z / 2, width: k.width + z, height: k.height + z, r: z})
        };
        u.update();
        return u
    };
    Raphael.fn.g.labelit = function (o) {
        var l = o.getBBox(), p = c(20, l.width + 10, l.height + 10) / 2;
        return this.rect(l.x - p / 2, l.y - p / 2, l.width + p, l.height + p, p).attr({stroke: "none", fill: "#000"}).insertBefore(o.node ?
            o : o[0])
    };
    Raphael.fn.g.drop = function (o, l, p, u, k) {
        u = u || 30;
        k = k || 0;
        var z = this.set();
        z.push(this.path(["M", o, l, "l", u, 0, "A", u * 0.4, u * 0.4, 0, 1, 0, o + u * 0.7, l - u * 0.7, "z"]).attr({fill: "#000", stroke: "none", rotation: [22.5 - k, o, l]}));
        k = (k + 90) * Math.PI / 180;
        z.push(this.text(o + u * Math.sin(k), l + u * Math.cos(k), p).attr(this.g.txtattr).attr({"font-size": u * 12 / 30, fill: "#fff"}));
        z.drop = z[0];
        z.text = z[1];
        return z
    };
    Raphael.fn.g.blob = function (o, l, p, u, k) {
        u = (+u + 1 ? u : 45) + 90;
        k = k || 12;
        var z = Math.PI / 180, G = k * 12 / 12, w = this.set();
        w.push(this.path().attr({fill: "#000",
            stroke: "none"}));
        w.push(this.text(o + k * Math.sin(u * z), l + k * Math.cos(u * z) - G / 2, p).attr(this.g.txtattr).attr({"font-size": G, fill: "#fff"}));
        w.update = function (D, K, C) {
            D = D || o;
            K = K || l;
            var H = this[1].getBBox(), O = e(H.width + G, k * 25 / 12), P = e(H.height + G, k * 25 / 12);
            H = D + k * Math.sin((u - 22.5) * z);
            var ea = K + k * Math.cos((u - 22.5) * z);
            D = D + k * Math.sin((u + 22.5) * z);
            K = K + k * Math.cos((u + 22.5) * z);
            var Q = (D - H) / 2, ca = (K - ea) / 2;
            O = O / 2;
            P = P / 2;
            var fa = -Math.sqrt(Math.abs(O * O * P * P - O * O * ca * ca - P * P * Q * Q) / (O * O * ca * ca + P * P * Q * Q));
            ca = fa * O * ca / P + (D + H) / 2;
            Q = fa * -P * Q / O +
                (K + ea) / 2;
            C ? this.animate({x: ca, y: Q, path: ["M", o, l, "L", D, K, "A", O, P, 0, 1, 1, H, ea, "z"].join(",")}, 500, ">") : this.attr({x: ca, y: Q, path: ["M", o, l, "L", D, K, "A", O, P, 0, 1, 1, H, ea, "z"].join(",")});
            return this
        };
        w.update(o, l);
        return w
    };
    Raphael.fn.g.colorValue = function (o, l, p, u) {
        return"hsb(" + [c((1 - o / l) * 0.4, 1), p || 0.75, u || 0.75] + ")"
    };
    Raphael.fn.g.snapEnds = function (o, l, p) {
        function u(w) {
            return Math.abs(w - 0.5) < 0.25 ? ~~w + 0.5 : Math.round(w)
        }

        var k = o, z = l;
        if (k == z)return{from: k, to: z, power: 0};
        k = (z - k) / p;
        var G = z = ~~k;
        p = 0;
        if (z) {
            for (; G;) {
                p--;
                G = ~~(k * Math.pow(10, p)) / Math.pow(10, p)
            }
            p++
        } else {
            for (; !z;) {
                p = p || 1;
                z = ~~(k * Math.pow(10, p)) / Math.pow(10, p);
                p++
            }
            p && p--
        }
        z = u(l * Math.pow(10, p)) / Math.pow(10, p);
        if (z < l)z = u((l + 0.5) * Math.pow(10, p)) / Math.pow(10, p);
        k = u((o - (p > 0 ? 0 : 0.5)) * Math.pow(10, p)) / Math.pow(10, p);
        return{from: k, to: z, power: p}
    };
    Raphael.fn.g.axis = function (o, l, p, u, k, z, G, w, D, K) {
        K = K == null ? 2 : K;
        D = D || "t";
        z = z || 10;
        var C = D == "|" || D == " " ? ["M", o + 0.5, l, "l", 0, 0.0010] : G == 1 || G == 3 ? ["M", o + 0.5, l, "l", 0, -p] : ["M", o, l + 0.5, "l", p, 0];
        u = this.g.snapEnds(u, k, z);
        var H = u.from,
            O = u.to, P = u.power;
        k = 0;
        u = this.set();
        d = (O - H) / z;
        O = H;
        var ea = P > 0 ? P : 0;
        Q = p / z;
        if (+G == 1 || +G == 3) {
            z = l;
            for (H = (G - 1 ? 1 : -1) * (K + 3 + !!(G - 1)); z >= l - p;) {
                D != "-" && D != " " && (C = C.concat(["M", o - (D == "+" || D == "|" ? K : !(G - 1) * K * 2), z + 0.5, "l", K * 2 + 1, 0]));
                u.push(this.text(o + H, z, w && w[k++] || (Math.round(O) == O ? O : +O.toFixed(ea))).attr(this.g.txtattr).attr({"text-anchor": G - 1 ? "start" : "end"}));
                O += d;
                z -= Q
            }
            if (Math.round(z + Q - (l - p))) {
                D != "-" && D != " " && (C = C.concat(["M", o - (D == "+" || D == "|" ? K : !(G - 1) * K * 2), l - p + 0.5, "l", K * 2 + 1, 0]));
                u.push(this.text(o + H, l - p, w &&
                    w[k] || (Math.round(O) == O ? O : +O.toFixed(ea))).attr(this.g.txtattr).attr({"text-anchor": G - 1 ? "start" : "end"}))
            }
        } else {
            O = H;
            ea = (P > 0) * P;
            H = (G ? -1 : 1) * (K + 9 + !G);
            P = o;
            var Q = p / z, ca = 0;
            for (z = 0; P <= o + p;) {
                D != "-" && D != " " && (C = C.concat(["M", P + 0.5, l - (D == "+" ? K : !!G * K * 2), "l", 0, K * 2 + 1]));
                u.push(ca = this.text(P, l + H, w && w[k++] || (Math.round(O) == O ? O : +O.toFixed(ea))).attr(this.g.txtattr));
                ca = ca.getBBox();
                if (z >= ca.x - 5)u.pop(u.length - 1).remove(); else z = ca.x + ca.width;
                O += d;
                P += Q
            }
            if (Math.round(P - Q - o - p)) {
                D != "-" && D != " " && (C = C.concat(["M", o + p +
                    0.5, l - (D == "+" ? K : !!G * K * 2), "l", 0, K * 2 + 1]));
                u.push(this.text(o + p, l + H, w && w[k] || (Math.round(O) == O ? O : +O.toFixed(ea))).attr(this.g.txtattr))
            }
        }
        o = this.path(C);
        o.text = u;
        o.all = this.set([o, u]);
        o.remove = function () {
            this.text.remove();
            this.constructor.prototype.remove.call(this)
        };
        return o
    };
    Raphael.el.lighter = function (o) {
        o = o || 2;
        var l = [this.attrs.fill, this.attrs.stroke];
        this.fs = this.fs || [l[0], l[1]];
        l[0] = Raphael.rgb2hsb(Raphael.getRGB(l[0]).hex);
        l[1] = Raphael.rgb2hsb(Raphael.getRGB(l[1]).hex);
        l[0].b = c(l[0].b * o, 1);
        l[0].s /=
            o;
        l[1].b = c(l[1].b * o, 1);
        l[1].s /= o;
        this.attr({fill: "hsb(" + [l[0].h, l[0].s, l[0].b] + ")", stroke: "hsb(" + [l[1].h, l[1].s, l[1].b] + ")"})
    };
    Raphael.el.darker = function (o) {
        o = o || 2;
        var l = [this.attrs.fill, this.attrs.stroke];
        this.fs = this.fs || [l[0], l[1]];
        l[0] = Raphael.rgb2hsb(Raphael.getRGB(l[0]).hex);
        l[1] = Raphael.rgb2hsb(Raphael.getRGB(l[1]).hex);
        l[0].s = c(l[0].s * o, 1);
        l[0].b /= o;
        l[1].s = c(l[1].s * o, 1);
        l[1].b /= o;
        this.attr({fill: "hsb(" + [l[0].h, l[0].s, l[0].b] + ")", stroke: "hsb(" + [l[1].h, l[1].s, l[1].b] + ")"})
    };
    Raphael.el.original =
        function () {
            if (this.fs) {
                this.attr({fill: this.fs[0], stroke: this.fs[1]});
                delete this.fs
            }
        }
})();
Raphael.fn.g.piechart = function (e, c, q, j, o) {
    o = o || {};
    var l = this, p = [], u = this.set(), k = this.set(), z = this.set(), G = j.length, w = 0, D = 0, K = 0, C = 9, H = true;
    k.covers = u;
    if (G == 1) {
        z.push(this.circle(e, c, q).attr({fill: this.g.colors[0], stroke: o.stroke || "#fff", "stroke-width": o.strokewidth == null ? 1 : o.strokewidth}));
        u.push(this.circle(e, c, q).attr(this.g.shim));
        D = j[0];
        j[0] = {value: j[0], order: 0, valueOf: function () {
            return this.value
        }};
        z[0].middle = {x: e, y: c};
        z[0].mangle = 180
    } else {
        for (var O = function (Q, ca, fa, aa, N) {
            var ha = Math.PI / 180,
                oa = Q + fa * Math.cos(-aa * ha), ua = Q + fa * Math.cos(-N * ha), va = Q + fa / 2 * Math.cos(-(aa + (N - aa) / 2) * ha), wa = ca + fa * Math.sin(-aa * ha), la = ca + fa * Math.sin(-N * ha);
            ha = ca + fa / 2 * Math.sin(-(aa + (N - aa) / 2) * ha);
            Q = ["M", Q, ca, "L", oa, wa, "A", fa, fa, 0, +(Math.abs(N - aa) > 180), 1, ua, la, "z"];
            Q.middle = {x: va, y: ha};
            return Q
        }, P = 0; P < G; P++) {
            D += j[P];
            j[P] = {value: j[P], order: P, valueOf: function () {
                return this.value
            }}
        }
        o.sort && j.sort(function (Q, ca) {
            return ca.value - Q.value
        });
        for (P = 0; P < G; P++) {
            if (H && j[P] * 360 / D <= 1.5) {
                C = P;
                H = false
            }
            if (P > C) {
                H = false;
                j[C].value += j[P];
                j[C].others = true;
                K = j[C].value
            }
        }
        G = Math.min(C + 1, j.length);
        K && j.splice(G) && (j[C].others = true);
        for (P = 0; P < G; P++) {
            K = w - 360 * j[P] / D / 2;
            if (!P) {
                w = 90 - K;
                K = w - 360 * j[P] / D / 2
            }
            if (o.init)var ea = O(e, c, 1, w, w - 360 * j[P] / D).join(",");
            C = O(e, c, q, w, w -= 360 * j[P] / D);
            H = this.path(o.init ? ea : C).attr({fill: o.colors && o.colors[P] || this.g.colors[P] || "#666", stroke: o.stroke || "#fff", "stroke-width": o.strokewidth == null ? 1 : o.strokewidth, "stroke-linejoin": "round"});
            H.value = j[P];
            H.middle = C.middle;
            H.mangle = K;
            p.push(H);
            z.push(H);
            o.init && H.animate({path: C.join(",")},
                    +o.init - 1 || 1E3, ">")
        }
        for (P = 0; P < G; P++) {
            H = l.path(p[P].attr("path")).attr(this.g.shim);
            o.href && o.href[P] && H.attr({href: o.href[P]});
            H.attr = function () {
            };
            u.push(H);
            z.push(H)
        }
    }
    k.hover = function (Q, ca) {
        ca = ca || function () {
        };
        for (var fa = this, aa = 0; aa < G; aa++)(function (N, ha, oa) {
            var ua = {sector: N, cover: ha, cx: e, cy: c, mx: N.middle.x, my: N.middle.y, mangle: N.mangle, r: q, value: j[oa], total: D, label: fa.labels && fa.labels[oa]};
            ha.mouseover(function () {
                Q.call(ua)
            }).mouseout(function () {
                ca.call(ua)
            })
        })(z[aa], u[aa], aa);
        return this
    };
    k.each =
        function (Q) {
            for (var ca = this, fa = 0; fa < G; fa++)(function (aa, N, ha) {
                Q.call({sector: aa, cover: N, cx: e, cy: c, x: aa.middle.x, y: aa.middle.y, mangle: aa.mangle, r: q, value: j[ha], total: D, label: ca.labels && ca.labels[ha]})
            })(z[fa], u[fa], fa);
            return this
        };
    k.click = function (Q) {
        for (var ca = this, fa = 0; fa < G; fa++)(function (aa, N, ha) {
            var oa = {sector: aa, cover: N, cx: e, cy: c, mx: aa.middle.x, my: aa.middle.y, mangle: aa.mangle, r: q, value: j[ha], total: D, label: ca.labels && ca.labels[ha]};
            N.click(function () {
                Q.call(oa)
            })
        })(z[fa], u[fa], fa);
        return this
    };
    k.inject = function (Q) {
        Q.insertBefore(u[0])
    };
    p = function (Q, ca, fa, aa) {
        var N = e + q + q / 5, ha = c + 10;
        Q = Q || [];
        aa = aa && aa.toLowerCase && aa.toLowerCase() || "east";
        fa = l.g.markers[fa && fa.toLowerCase()] || "disc";
        k.labels = l.set();
        for (var oa = 0; oa < G; oa++) {
            var ua = z[oa].attr("fill"), va = j[oa].order;
            j[oa].others && (Q[va] = ca || "Others");
            Q[va] = l.g.labelise(Q[va], j[oa], D);
            k.labels.push(l.set());
            k.labels[oa].push(l.g[fa](N + 5, ha, 5).attr({fill: ua, stroke: "none"}));
            k.labels[oa].push(ua = l.text(N + 20, ha, Q[va] || j[va]).attr(l.g.txtattr).attr({fill: o.legendcolor ||
                "#000", "text-anchor": "start"}));
            u[oa].label = k.labels[oa];
            ha += ua.getBBox().height * 1.2
        }
        Q = k.labels.getBBox();
        k.labels.translate.apply(k.labels, {east: [0, -Q.height / 2], west: [-Q.width - 2 * q - 20, -Q.height / 2], north: [-q - Q.width / 2, -q - Q.height - 10], south: [-q - Q.width / 2, q + 10]}[aa]);
        k.push(k.labels)
    };
    o.legend && p(o.legend, o.legendothers, o.legendmark, o.legendpos);
    k.push(z, u);
    k.series = z;
    k.covers = u;
    return k
};
Raphael.el.isAbsolute = true;
Raphael.el.absolutely = function () {
    this.isAbsolute = 1;
    return this
};
Raphael.el.relatively = function () {
    this.isAbsolute = 0;
    return this
};
Raphael.el.moveTo = function (e, c) {
    this._last = {x: e, y: c};
    return this.attr({path: this.attrs.path + ["m", "M"][+this.isAbsolute] + parseFloat(e) + " " + parseFloat(c)})
};
Raphael.el.lineTo = function (e, c) {
    this._last = {x: e, y: c};
    return this.attr({path: this.attrs.path + ["l", "L"][+this.isAbsolute] + parseFloat(e) + " " + parseFloat(c)})
};
Raphael.el.arcTo = function (e, c, q, j, o, l, p) {
    this._last = {x: o, y: l};
    return this.attr({path: this.attrs.path + ["a", "A"][+this.isAbsolute] + [parseFloat(e), parseFloat(c), +p, q, j, parseFloat(o), parseFloat(l)].join(" ")})
};
Raphael.el.curveTo = function () {
    var e = Array.prototype.splice.call(arguments, 0, arguments.length), c = [0, 0, 0, 0, "s", 0, "c"][e.length] || "";
    this.isAbsolute && (c = c.toUpperCase());
    this._last = {x: e[e.length - 2], y: e[e.length - 1]};
    return this.attr({path: this.attrs.path + c + e})
};
Raphael.el.cplineTo = function (e, c, q) {
    this.attr({path: this.attrs.path + ["C", this._last.x + q, this._last.y, e - q, c, e, c]});
    this._last = {x: e, y: c};
    return this
};
Raphael.el.qcurveTo = function () {
    var e = [0, 1, "t", 3, "q"][arguments.length], c = Array.prototype.splice.call(arguments, 0, arguments.length);
    if (this.isAbsolute)e = e.toUpperCase();
    this._last = {x: c[c.length - 2], y: c[c.length - 1]};
    return this.attr({path: this.attrs.path + e + c})
};
Raphael.el.addRoundedCorner = function (e, c) {
    var q = this.isAbsolute;
    q && this.relatively();
    this._last = {x: e * (!!(c.indexOf("r") + 1) * 2 - 1), y: e * (!!(c.indexOf("d") + 1) * 2 - 1)};
    this.arcTo(e, e, 0, {lu: 1, rd: 1, ur: 1, dl: 1}[c] || 0, this._last.x, this._last.y);
    q && this.absolutely();
    return this
};
Raphael.el.andClose = function () {
    return this.attr({path: this.attrs.path + "z"})
};
(function (e) {
    e.widget("ui.dialogManager", {options: {apTemplate: null, staTemplate: null, startZ: 100}, _init: function () {
        this.dialogs = [];
        return this
    }, addItem: function (c, q) {
        var j = this.dialogs, o;
        for (var l in j)if (j[l].mac == c) {
            o = j[l].element;
            break
        }
        if (o == null) {//alert(c+" "+this.options.apTemplate);
            if (q == "ap" || q == "afi") {
                o = e(this.options.apTemplate + ":first").clone();
                e(o).apDialog({mac: c, model: e("#model-aps")})
            }
	    else if(q == "sta"){
                o = e(this.options.staTemplate + ":first").clone();
                e(o).staDialog({mac: c, model: e("#model-stas")})
            }
            e(o).attr("title", c);
            e(this.element).append(o);
            j.push({element: o,
                mac: c});
            this.centerItem(o)
        }
        this.bringToFront(o)
    }, removeItem: function (c) {
        for (var q = this.dialogs, j = 0, o = 0, l = 0; l < q.length; l++)if (q[l].mac == c.attr("title"))o = l; else {
            e(q[l].element).css("z-index", this.options.startZ + j);
            j++
        }
        q.splice(o, 1);
        e(c).remove()
    }, centerItem: function (c) {
        var q = this.dialogs;
        e(c).css("position", "absolute");
        e(c).css("top", (e(window).height() - 600) / 2 + e(window).scrollTop() + 15 * q.length + "px");
        e(c).css("left", (e(window).width() - 300) / 2 + e(window).scrollLeft() + 15 * q.length + "px")
    }, bringToFront: function (c) {
        for (var q =
            this.dialogs, j = 0, o = 0, l = 0; l < q.length; l++)if (q[l].mac == c.attr("title"))o = l; else {
            e(q[l].element).css("z-index", this.options.startZ + j);
            j++
        }
        q.splice(o, 1);
        q.push({element: c, mac: c.attr("title")});
        c.css("z-index", this.options.startZ + q.length - 1)
    }});
    e.widget("ui.dialogTabs", {option: {}, _init: function () {
        var c = this;
        c.element.find("div.tab-content").hide();
        c.element.find("div.tab-content:first").show();
        c.element.find("ul.dialog-tabs li").each(function () {
            var q = e(this).find("a").attr("href").substring(1);
            e(this).find("a").attr("href",
                "javascript:void(0);");
            e(this).click(function () {
                c.element.find("li").attr("id","");
                e(this).attr("id","choose-button");
                c.element.find("div.tab-content").hide();
                c.element.find("div." + q).show()
            })
        });
        c.element.find("ul.dialog-tabs li:first").attr("id","choose-button");
    }});
    e.widget("ui.apDialog", {options: {mac: null, model: null}, _init: function () {
        var c = this;
        c.element.dialogTabs();
        c.element.find("div.details-tab-content").accordion({heightStyle: true});
        c.element.find("div.config-tab-content").accordion({heightStyle: true});
		c.element.find("div.details-tab-content").css('height','auto');
        c.element.mousedown(function () {
            e(document.body).dialogManager("bringToFront", c.element)
        });
        c.element.draggable({ containment: "document"});//
		var j = c.element.find("div.ap-radio");
		j.find(".radio-nav-ul1 li").first().attr("id","choose-button");
        c.element.find("div.users-data-table").jtable({model: e("#model-stas"), className: "stat", pagesize: 5}).jtable("addcol", "", "model", true, Renderer.apIcon).jtable("addcol", Msg.NameStr, "mac", true, Renderer.staDetailLink).jtable("addcol", Msg.UWLAN, "essid", true).jtable("addaction", Msg.Block, "block", function () {
            return true
        }).jtable("addaction", Msg.Reconnect, "kick", function () {
            return true
        }).jtable("addfilter", "is_guest",
            false).jtable("addfilter", "ap_mac", this.options.mac).bind("jtable_block", function (w, D) {
                e.jcmd.block_sta(D)
            }).bind("jtable_kick", function (w, D) {
                e.jcmd.kick_sta(D)
            });
        c.element.find("div.guests-data-table").jtable({model: e("#model-stas"), className: "stat", pagesize: 5}).jtable("addcol", Msg.NameStr, "mac", true, Renderer.staDetailLink).jtable("addcol", Msg.UWLAN, "essid", true).jtable("addaction", Msg.Block, "block", function () {
            return true
        }).jtable("addaction", Msg.Reconnect, "kick", function () {
            return true
        }).jtable("addfilter",
            "is_guest", true).jtable("addfilter", "ap_mac", this.options.mac).bind("jtable_block", function (w, D) {
                e.jcmd.block_sta(D)
            }).bind("jtable_kick", function (w, D) {
                e.jcmd.kick_sta(D)
            });
        var q = c.element.find("div.ap-config");
        q.find('input[name="apply"]').japply({get_posts: function () {
            c.ap.name = c.element.find('input[name="alias"]').val();
            if (c.ap.has_poe_passthrough)c.ap.poe_passthrough = c.element.find('select[name="poe_passthrough"]').val();
            e("#model-aps").data("ap_names")[c.ap.mac] = c.ap.name;
            return[
                {url: "/upd_device.php", payload: {_id: c.ap._id, mac: c.ap.mac, name: c.ap.name, poe_passthrough: c.ap.poe_passthrough}, changed: function () {
                    return true
                }}
            ]
        }, validate: function () {
            q.find('input[name="alias"]').val() == "" && q.find('input[name="alias"]').val(c.ap.mac);
            return true
        }, done: function () {
            q.find("p.settings-response").addClass("success").show().delay(1E3).fadeOut(500);
            e("#model-aps").jtablemodel("firechanged")
        }});
        
		j.find(".radio-nav-ul1 li").first().click(function(){
		j.find(".radio-nav-ul1 li").last().attr('id',"");
        $(this).attr('id',"choose-button");
		$(".ap-na-configs").show();
		$(".ap-ng-configs").hide();
		}); 
		j.find(".radio-nav-ul1 li").last().click(function(){
		j.find(".radio-nav-ul1 li").first().attr('id',"");
        $(this).attr('id',"choose-button");
		$(".ap-ng-configs").show();
		$(".ap-na-configs").hide();
		}); 
        j.find('.ap-ng-configs select[name="tx-power-mode"]').change(function () {
            j.find(".ap-ng-configs .tx-power-custom").setvisible(e(this).val() ==
                "custom");
            j.find(".ap-ng-configs .antenna-gain").attr("disabled") != "disabled" && j.find(".ap-ng-configs .antenna-gain").setvisible(e(this).val() == "custom");
            j.find('.ap-ng-configs input[name="antenna-gain"]').val(c.ap["ng-antenna_gain"]);
            if (e(this).val() != "custom")j.find('.ap-ng-configs input[name="tx-power"]').val("auto"); else c.ap["ng-desired_txpower"] == "auto" ? j.find('.ap-ng-configs input[name="tx-power"]').val(c.ap["ng-tx_power"] || "0") : j.find('.ap-ng-configs input[name="tx-power"]').val(c.ap["ng-desired_txpower"] ||
                "0")
        });
        j.find('.ap-na-configs select[name="tx-power-mode"]').change(function () {
            j.find(".ap-na-configs .tx-power-custom").setvisible(e(this).val() == "custom");
            j.find(".ap-na-configs .antenna-gain").attr("disabled") != "disabled" && j.find(".ap-na-configs .antenna-gain").setvisible(e(this).val() == "custom");
            j.find('.ap-na-configs input[name="antenna-gain"]').val(c.ap["na-antenna_gain"]);
            if (e(this).val() != "custom")j.find('.ap-na-configs input[name="tx-power"]').val("auto"); else c.ap["na-desired_txpower"] ==
            "auto" ? j.find('.ap-na-configs input[name="tx-power"]').val(c.ap["na-tx_power"] || "0") : j.find('.ap-na-configs input[name="tx-power"]').val(c.ap["na-desired_txpower"] || "0")
        });
        j.find('.ap-na-configs select[name="channel"]').change(function () {
            var w = parseInt(e(this).val()), D = e(document.body).data("sysinfo").cur_ccode, K = isNaN(w) || w == 0, C = e.inArray(w, D.channels_na_40) >= 0 || K;
            w = e.inArray(w, D.channels_na_80) >= 0 || K;
            j.find('.ap-na-configs select[name="ht"]').find("option[value=40]").setvisible(C);
            j.find('.ap-na-configs select[name="ht"]').find("option[value=80]").setvisible(w);
            C || j.find('.ap-na-configs select[name="ht"]').val("20")
        });
        j.find('input[name="apply"]').japply({get_posts: function () {
            var w = [];
            c.element.hasClass("ap-ng-radio") && w.push({radio: "ng", builtin_antenna: c.ap["ng-builtin_antenna"],
			builtin_ant_gain: c.ap["ng-builtin_ant_gain"], max_txpower: c.ap["ng-max_txpower"],
			channel: c.element.find('.ap-ng-configs select[name="channel"]').val(),
			ht: c.element.find('.ap-ng-configs select[name="ht"]').val(),
			tx_power_mode: c.element.find('.ap-ng-configs select[name="tx-power-mode"]').val(),
			tx_power: c.element.find('.ap-ng-configs input[name="tx-power"]').val(),
			antenna_gain: c.element.find('.ap-ng-configs input[name="antenna-gain"]').val(),
			mode: c.element.find('.ap-ng-configs select[name="radio-mode"]').val()});
            c.element.hasClass("ap-na-radio") && w.push({radio: "na", builtin_antenna: c.ap["na-builtin_antenna"],
			builtin_ant_gain: c.ap["na-builtin_ant_gain"], max_txpower: c.ap["na-max_txpower"],
			channel: c.element.find('.ap-na-configs select[name="channel"]').val(),
			ht: c.element.find('.ap-na-configs select[name="ht"]').val(),
			tx_power_mode: c.element.find('.ap-na-configs select[name="tx-power-mode"]').val(),
			tx_power: c.element.find('.ap-na-configs input[name="tx-power"]').val(),
			antenna_gain: c.element.find('.ap-na-configs input[name="antenna-gain"]').val(),
			mode: c.element.find('.ap-na-configs select[name="radio-mode"]').val()});
            c.ap.radio_table = w;
            c.ap["ng-tx_power"] = c.element.find('.ap-ng-configs input[name="tx-power"]').val();
            c.ap["na-tx_power"] = c.element.find('.ap-na-configs input[name="tx-power"]').val();
            return[
		{url: "/upd_device.php", payload: {_id: c.ap._id, mac: c.ap.mac, radio_table: w}, changed: function () {
                    return true
                }}
            ]
        }, validate: function () {
            q.find('input[name="alias"]').val() == "" && q.find('input[name="alias"]').val(c.ap.mac);
            if (c.element.hasClass("ap-ng-radio")) {
                if (j.find('.ap-ng-configs select[name="tx-power-mode"]').val() == "custom")if (!V.checkInteger(j.find('.ap-ng-configs input[name="tx-power"]'), Msg.TransmitPowerng))return false;
                if (!V.checkInteger(j.find('.ap-ng-configs input[name="antenna-gain"]'), Msg.AntennaGainng))return false
            }
            if (c.element.hasClass("ap-na-radio")) {
                if (j.find('.ap-na-configs select[name="tx-power-mode"]').val() ==
                    "custom")if (!V.checkInteger(j.find('.ap-na-configs input[name="tx-power"]'), Msg.TransmitPowerna))return false;
                if (!V.checkInteger(j.find('.ap-na-configs input[name="antenna-gain"]'), Msg.AntennaGainna))return false
            }
            return true
        }, done: function () {
            j.find("p.settings-response").addClass("success").show().delay(1E3).fadeOut(500);
            e("#model-aps").jtablemodel("firechanged")
        }});
        var o = c.element.find("div.ap-network");
        o.find('select[name="network_config"]').change(function () {
            o.find("div.ip-settings").setvisible(e(this).val() ==
                "static")
        });
        o.find('input[name="apply"]').japply({get_posts: function () {
            var w = null;
            w = c.element.find('select[name="network_config"]').val() == "static" ? {type: c.element.find('select[name="network_config"]').val(), ip: c.element.find('input[name="address"]').val(), gateway: c.element.find('input[name="gateway"]').val(), netmask: c.element.find('input[name="netmask"]').val(), dns1: c.element.find('input[name="dns1"]').val(), dns2: c.element.find('input[name="dns2"]').val()} : {type: c.element.find('select[name="network_config"]').val()};
            c.ap.config_network = w;
            return[
		{url: "/upd_device.php", payload: {_id: c.ap._id, mac: c.ap.mac, config_network: w}}
            ]
        }, validate: function () {
            var w = true, D = c.element.find('input[name="address"]').val();
            c.element.find('select[name="network_config"]').val() == "static" && c.element.find("input.ip-setting").each(function (K, C) {
            	  var msg = {"address": Msg.IPAddress, "netmask": Msg.SubnetMask, "gateway": Msg.Gateway, "dns1": Msg.PreferredDNS, "dns2": Msg.AlternateDNS };
                return w = C.name == "address" ? !V.checkEmpty(e(C), msg[C.name]) || !V.checkIP(e(C), e(C).val()) ? false : true : C.name == "netmask" ? !V.checkEmpty(e(C), msg[C.name]) || !V.checkNetmask(e(C), e(C).val(), D) ? false : true : e(C).val() && !V.checkIP(e(C), e(C).val()) ?
                    false : true
            });
            return w
        }, done: function () {
            e("#model-aps").jtablemodel("firechanged");
            c._paint()
        }});
        var l = c.element.find("form.ap-adv-adopt");
        l.find(".dialog-hide-button").click(function () {
            l.hide()
        });
        l.find('input[name="apply"]').japply({get_posts: function () {
            return{url: "/cmd/devmgr.php", payload: {cmd: "adv-adopt", mac: c.options.mac, ip: l.find('input[name="ip"]').val(), port: l.find('input[name="port"]').val(), username: l.find('input[name="username"]').val(), password: l.find('input[name="password"]').val(), url: l.find('input[name="url"]').val()}}
        },
            validate: function () {
                var w = l.find('input[name="username"]'), D = l.find('input[name="password"]');
                if (!V.checkEmpty(w, Msg.Username))return false;
                if (!V.checkEmpty(D, Msg.PasswordStr))return false;
                return true
            }, done: function () {
                c.element.find("div.details-tab-content").accordion("activate", 0);
                e("#model-aps").jtablemodel("refresh")
            }});
        var p = c.element.find("div.ap-wlan"), u = p.find("form.settings-list"), k = p.find("form.settings-form"), z = function (w, D) {
            var K = null;
            e.each(c.ap.wlan_overrides || [], function (C, H) {
		if (H.wlan_id == w && H.radio == D)
		{
                    K = H;
                    return false
                }
            });
            return K
        }, G = function (w) {
            if (isNaN(w.vlan))w.vlan = 0;
            k.find('input[name="name"]').val(w.name);
            k.find('input[name="wlanEnabled"]').setchecked(w.enabled);
            k.find('input[name="vlanEnabled"]').setchecked(w.vlan_enabled);
            k.find('input[name="vlan"]').val(w.vlan > 1 ? w.vlan : "");
            k.find(".wpapsk").setvisible(w.security == "wpapsk");
            k.find('input[name="passphrase"]').val(w.x_passphrase)
        };
		p.find(".radio-nav-ul2 li").first().attr('id',"choose-button");
		p.find(".radio-nav-ul2 li").click(function(){
		p.find(".radio-nav-ul2 li").first().attr('id',"");
		p.find(".radio-nav-ul2 li").last().attr('id',"");
        $(this).attr('id',"choose-button");
		});  
        e.each(["ng", "na"], function (w, D) {
            c.element.find("div.wlan-data-table-" + D).jtable({rows: [], hidetoolbar: true,
                pagesize: 0}).jtable("addcol", "", "model", true, Renderer.apIcon).jtable("addcol", Msg.NameStr, "name", false).jtable("addcol", Msg.Overrides, "_id", false, function (K, C) {
                var H = [], O = z(K, D);
		if (!O)	return "";
		if (O.enabled == false)	return "Disabled";
                O.name != C.name && H.push("SSID:" + O.name);
                if (C.vlan_enabled || O.vlan_enabled)
			if (C.vlan_enabled && !O.vlan_enabled)
				H.push("VLAN:disabled");
			else if (!C.vlan_enabled && O.vlan_enabled)
				H.push("VLAN:" + O.vlan);
			else
				C.vlan != O.vlan && H.push("VLAN:" + O.vlan);
                	C.security == "wpapsk" && O.x_passphrase != C.x_passphrase && H.push("PSK");
                return H.join(", ")
            }).jtable("addaction",
                Msg.Override, "override").bind("jtable_override", function (K, C) {
                    var H = z(C._id, D) || {wlan_id: C._id, radio: D, enabled: C.enabled, name: C.name, vlan_enabled: C.vlan_enabled, vlan: C.vlan || 0, security: C.security, x_passphrase: C.x_passphrase};
                    u.hide();
                    G(H);
                    k.data("o", H).data("wlan", C).show()
                });
            k.find("input[name=cancel]").click(function () {
                k.hide();
                u.show()
            });
            k.find("input[name=restore]").click(function () {
                G(k.data("wlan"));
                k.find('input[name="apply"]').click()
            })
        });
        k.find('input[name="apply"]').japply({validate: function () {
            if (!V.checkEmpty(k.find('input[name="name"]'),
                "SSID"))return false;
            if (k.find('input[name="vlanEnabled"]').checked())if (!V.checkMinMax(k.find('input[name="vlan"]'), 2, 4095, "VLAN ID"))return false;
            if (k.data("wlan").security == "wpapsk")if (!V.checkPassphrase(k.find('input[name="passphrase"]'), "PSK"))return false;
            return true
        }, get_posts: function () {
            c.ap.wlan_overrides = c.ap.wlan_overrides || [];
            var w = k.data("o").wlan_id, D = k.data("o").radio, K = z(w, D);
            if (K == null) {
                K = k.data("o");
                c.ap.wlan_overrides.push(K)
            }
            K.enabled = k.find('input[name="wlanEnabled"]').checked();
            K.vlan_enabled = k.find('input[name="vlanEnabled"]').checked();
            K.vlan = k.find('input[name="vlan"]').val();
            K.name = k.find('input[name="name"]').val();
            K.x_passphrase = k.find('input[name="passphrase"]').val();
            w = k.data("wlan");
            D = true;
            if (K.name != w.name)D = false;
            if (!K.enabled != !w.enabled)D = false;
            if (!K.vlan_enabled == !w.vlan_enabled) {
                if (K.vlan_enabled && K.vlan != w.vlan)D = false
            } else D = false;
            if (K.x_passphrase != w.x_passphrase)D = false;
            if (D)c.ap.wlan_overrides = e.grep(c.ap.wlan_overrides, function (C) {
                return C != K
            });
	    return{url: "/upd_device.php", payload: {_id: c.ap._id, mac: c.ap.mac, wlan_overrides: c.ap.wlan_overrides}}
        }, done: function () {
            c._paint();
            p.find("p.settings-response").addClass("success").show().delay(1E3).fadeOut(500);
            k.hide();
            u.show()
        }});
        c.element.find('div.ap-configs input[name="remove"]').click(function () {
            confirm(Msg.C_ForgetAP) && e.jcmd("devmgr", "delete-ap", {_id: c.ap._id}, function () {
                e("#model-aps").jtablemodel("refresh");
                c.dispose()
            })
        });
        c.element.find(".locate-button").click(function () {
            c.element.find(".locate-button").attr("disabled") != "disabled" && e.jcmd.locate_ap(c.ap)
        });
        c.element.find(".restart-button").click(function () {
            e.jcmd.restart_ap(c.ap)
        });
        c.element.find(".adopt-button").click(function () {
            c.element.find(".adopt-button").attr("disabled") != "disabled" && e.jcmd.adopt_ap(c.ap)
        });
        c.element.find(".scan-button").click(function () {
            if (!c.ap.scanning) {
                c.ap.scanning = true;
                e.jcmd.scan_ap(c.ap)
            }
        });
        c.element.find(".downlink-data-table").jtable({rows: [],
            pagesize: 0}).jtable("addcol", Msg.APStr, "ap_mac", false, Renderer.apDetailLink).jtable("addcol", Msg.Signal, "rssi", false, Renderer.rssi).jtable("addaction", Msg.Remove, "unset_downlink", function () {
            return true
        }).bind("jtable_unset_downlink", function (w, D) {
            e(w.originalTarget).disable();
            e.jcmd("devmgr", "unset-vwire-uplink", {mac: D.ap_mac, uplink: c._mac}, function () {
                e("#model-aps").jtablemodel("refresh");
                alert(Msg.I_TopologyChanged);
            })
        });
        c.element.find(".uplinks-data-table").jtable({rows: [],
            emptydiv: c.element.find(".uplinks-data-table-none"), pagesize: 0, debug: true}).jtable("addcol", Msg.APStr, "mac", false, Renderer.apDetailLink).jtable("addcol", Msg.Channel, "channel", false).jtable("addcol", Msg.Signal, "rssi", false, Renderer.rssi).jtable("addaction", Msg.SelectStr, "set_uplink", function (w) {
            return!w.configured && w.available
        }).jtable("addaction", Msg.Remove, "unset_uplink", function (w) {
            return w.configured
        }).bind("jtable_set_uplink", function (w, D) {
            e(w.originalTarget).disable();
            e.jcmd("devmgr", "set-vwire-uplink", {mac: c._mac,
                uplink: D.mac}, function () {
                alert(Msg.I_TopologyChanged);
                e("#model-aps").jtablemodel("refresh")
            })
        }).bind("jtable_unset_uplink", function (w, D) {
            e(w.originalTarget).disable();
            e.jcmd("devmgr", "unset-vwire-uplink", {mac: c._mac, uplink: D.mac}, function () {
                e("#model-aps").jtablemodel("refresh");
                alert(Msg.I_TopologyChanged)
            })
        });
        c.element.find("select.by-map").jselect({model: e("#model-maps"), defaultItem: Msg.AllLocations}).jselect("repaint");
        c.element.find("input[name=uplink-scan]").click(function () {
            var w = e(this);
            w.disable();
            e.jcmd("devmgr", "scan-uplink", {map_id: c.element.find("select.by-map").val()}, function () {
                setTimeout(function () {
                    w.enable();
                    e("#model-aps").jtablemodel("refresh")
                }, 5E3)
            })
        });
        e("#model-aps").bind("jtablemodel_refresh-done", c.model_aps_listener = function () {
            c.refresh()
        });
        e("#model-stas").bind("jtablemodel_refresh-done", c.model_stas_listener = function () {
            c._paint()
        });
        c.element.find(".dialog-hide-button").click(function () {
            c.dispose()
        });
        c.refresh();
        return c
    }, dispose: function () {
        e("#model-aps").unbind("jtablemodel_refresh-done", this.model_aps_listener);
        e("#model-stas").unbind("jtablemodel_refresh-done", this.model_stas_listener);
        e(document.body).dialogManager("removeItem", this.element)
    }, refresh: function () {
        this._mac = this.options.mac;
        this._model = this.options.model;
        (this.ap = this._model.jtablemodel("findrow", "mac", this._mac)) || this.dispose();
        this._paint();
    }, _paint: function () {
        var c = this.ap;
		if(!c)
		{
			return ;
		}
		var q = Renderer.apStatusType(c.state) == "ok", j = this.element;
        j.find("h2.ap-name").text(c.name || c.mac);
        j.find('input[name="alias"]:nofocus').val(c.name);
        j.find("dd.mac-address").html(c.mac);
        j.find("dd.model").html(Renderer.apModel(c.model));
        j.find("dd.version").html(c.version);
        j.find("dd.ip-address").html(c.ip);
        j.addClass("ap-model-" + c.model);
        if (c.has_poe_passthrough) {
            j.addClass("ap-has-poe-passthrough");
            j.find('select[name="poe_passthrough"]').val(c.poe_passthrough)
        }
        j.removeClass("ap-unmanaged");
        j.removeClass("ap-adopted");
        j.removeClass("ap-adoptfailed");
        if (c.adopted) {
            j.addClass("ap-adopted");
            c.locating ? j.find(".locate-button").addClass("locating") : j.find(".locate-button").removeClass("locating");
            //c.state == 10 && j.addClass("ap-adoptfailed");
            j.find(".scan-button").setenabled(!c.scanning)
        } else c["default"] || j.addClass("ap-unmanaged");
        var o = false, l = false;
        if (c.radio_table) {
            e.each(c.radio_table, function (K, C) {
                if (C.radio == "ng")o = true; else if (C.radio == "na")l = true;
                c[C.radio + "-builtin_antenna"] = C.builtin_antenna;
                c[C.radio + "-max_txpower"] = C.max_txpower;
                c[C.radio + "-desired_txpower"] = V.isNumber(C.tx_power) ?
                    C.tx_power : c[C.radio + "-tx_power"];
                j.find(".ap-" + C.radio + '-configs select[name="channel"]').val(C.channel || 0);
                j.find(".ap-" + C.radio + '-configs select[name="ht"]').val(C.ht || 20);
                j.find(".ap-" + C.radio + '-configs input[name="tx-power"]').val(C.tx_power || "0");
                j.find(".ap-" + C.radio + '-configs input[name="antenna-gain"]').val(C.antenna_gain || "6");
                j.find(".ap-" + C.radio + "-configs .antenna-gain").setenabled(!C.builtin_antenna);
                j.find(".ap-" + C.radio + "-configs .antenna-gain").setvisible(!C.builtin_antenna);
                j.find(".ap-" +
                    C.radio + '-configs select[name="tx-power-mode"]').val(C.tx_power_mode || "auto").change();
                j.find(".ap-" + C.radio + "-configs p.max-txpower").html("(Max: " + C.max_txpower + " dBm)");
                j.find(".ap-" + C.radio + '-configs select[name="radio-mode"]').val(C.mode || "all").change()
            });
            o ? j.addClass("ap-ng-radio") : j.removeClass("ap-ng-radio");
            l ? j.addClass("ap-na-radio") : j.removeClass("ap-na-radio")
        }
        j.find("div.ap-status").html(Renderer.apStatus(c.state, c));
        j.removeClass("ap-connected");
        j.removeClass("ap-disconnected");
        j.removeClass("ap-pending");
        j.removeClass("ap-uplink-wire");
        j.removeClass("ap-uplink-wireless");
        j.removeClass("ap-has-downlinks");
        j.removeClass("ap-needs-uplinks");
        if (q) {
            j.addClass("ap-connected");
            j.find("dd.uptime").html(Renderer.uptime(c.uptime));
            j.find("dd.num-users").html(c["user-num_sta"] || 0);
            j.find("dd.num-guests").html(c["guest-num_sta"] || 0);
            j.find(".locate-button").setenabled(true);
            var p = c.uplink || {};
            if (p.name == "eth0" || p.name == "eth1") {
                j.addClass("ap-uplink-wire");
                j.find(".uplink-type").html("("+Msg.WireStr+")")
            } else if (p.name ==
                "ath0") {
                j.addClass("ap-uplink-wireless");
                j.find(".uplink-type").html("("+Msg.Wireless+")")
            }
            j.find("dd.eth-duplex").html(p.full_duplex ? Msg.FullDuplex : Msg.HalfDuplex);
            j.find("dd.eth-speed").html(p.speed || Msg.Unknown);
            j.find("dd.uplink-ap").html(Renderer.apDetailLink(p.ap_mac));
            j.find("dd.uplink-rssi").html(Renderer.rssi(p.rssi));
            j.find("dd.uplink-tx-rate").html(Renderer.apXRate(p.tx_rate));
            j.find("dd.uplink-rx-rate").html(Renderer.apXRate(p.rx_rate));
            var u = p.tx_packets || 0, k = p.tx_bytes || 0, z = p.rx_packets || 0, G = p.rx_bytes ||
                0;
            p = (p["tx_bytes.r"] || 0) + (p["rx_bytes.r"] || 0);
            j.find("dd.uplink-bytes-rate").html(Renderer.pktsBytes(p) + "B/sec");
            j.find("dd.uplink-received").html(Renderer.pktsBytes(z) + " / " + Renderer.pktsBytes(G));
            j.find("dd.uplink-sent").html(Renderer.pktsBytes(u) + " / " + Renderer.pktsBytes(k));
            u = c.downlink_table || [];
            j.find(".downlink-data-table").jtable("option", "rows", u).jtable("option", "pagesize", u.length).jtable("refresh_options");
            if (u.length > 0)j.addClass("ap-has-downlinks"); else j.find("div.ap-downlink-content").hasClass("ui-accordion-content-active") &&
            j.find("div.details-tab-content").accordion("activate", 0);
            var w = c.stat || {};
            c.radio_table = c.radio_table || [];
            e.each(c.radio_table, function (K, C) {
                var H = C.radio, O = w[H + "-tx_packets"] || 0, P = w[H + "-tx_bytes"] || 0, ea = w[H + "-tx_dropped"] || 0, Q = w[H + "-rx_packets"] || 0, ca = w[H + "-rx_bytes"] || 0, fa = w[H + "-rx_dropped"] || 0, aa = w[H + "-tx_retries"] || 0, N = w[H + "-rx_errors"] || 0, ha = O + ea, oa = Q + fa, ua = Math.min(c[H + "-tx_power"], 30), va = c[H + "-gain"] || 0, wa = c[H + "-eirp"] || parseInt(ua) + va, la = c[H + "-extchannel"] || 0;
                la = c[H + "-channel"] + (la > 0 ? ",+1" :
                        la < 0 ? ",-1" : "");
                if ("DFS_WAIT" == (c[H + "-state"] || "RUN"))la += " (DFS Waiting)";
                j.find("dd." + H + "-channel").html(la);
                wa = wa + " dBm (EIRP)";
                ua = ua + " dBm" + (va > 0 ? "+" + va + " dBi" : "");
                j.find("dd." + H + "-tx-power").html(wa);
                j.find("dd." + H + "-tx-power").attr("title", ua);
                j.find("dd." + H + "-tx").html(Renderer.pktsBytes(O) + " / " + Renderer.pktsBytes(P));
                j.find("dd." + H + "-rx").html(Renderer.pktsBytes(Q) + " / " + Renderer.pktsBytes(ca));
                j.find("dd." + H + "-tx-error").html(Renderer.percentage(aa, ha) + " / " + Renderer.percentage(ea, ha));
                j.find("dd." +
                    H + "-rx-error").html(Renderer.percentage(N, oa) + " / " + Renderer.percentage(fa, oa));
                j.find("dd." + H + "-num-users").html(w["user-" + H + "-num_sta"] || 0);
                j.find("dd." + H + "-num-guests").html(w["guest-" + H + "-num_sta"] || 0)
            })
        } else {
            j.addClass("ap-disconnected");
            j.addClass("ap-" + Renderer.apStatusType(c.state, c));
            j.find(".adopt-button").setenabled(c["default"]);
            j.find(".locate-button").setenabled(0 && c["default"] && c.state == 1);
            j.find("dd.last-seen").html(Renderer.lastseen(c.last_seen * 1E3))
        }
        u = j.find("div.ap-radio");
        k = u.find(".ap-na-configs select[name=channel]");
        z = e(document.body).data("sysinfo").cur_ccode;
        if (z.key == "US" || z.key == "CA") {
            this.ap.model == "U5O" && k.find("[band=1]").remove();
            this.ap.model == "U7P" && k.find("[band=2]").remove()
        }
        if (!z.channels_na_80 || z.channels_na_80.length == 0 || this.ap.model != "U7E" && this.ap.model != "U7O" && this.ap.model != "U7Ev2")u.find('.ap-na-configs select[name="ht"] option[value=80]').remove();
        if (this.ap.model == "U7E" || this.ap.model == "U7O" || this.ap.model == "U7Ev2")k.find("[band=2]").remove();
        j.find("div.users-data-table").jtable("repaint");
        j.find("div.guests-data-table").jtable("repaint");
        u = e("#model-wlanconfs").jtablemodel("rows");
        this.element.find("div.wlan-data-table-ng").jtable("option", "rows", u).jtable("option", "pagesize", u.length).jtable("repaint");
        this.element.find("div.wlan-data-table-na").jtable("option", "rows", u).jtable("option", "pagesize", u.length).jtable("repaint");
        u = j.find("form.ap-adv-adopt");
        u.find("input[name=ip]").val(c.adopt_ip || c.ip);
        u.find("input[name=url]").val(c.adopt_url || c.inform_url || "http://afi-controller:8080/inform");
        u = j.find("form.ap-network");
        k = c.config_network || {};
        u.find('select[name="network_config"]').val(k.type || "dhcp").change();
        u.find('input[name="address"]').val(k.ip);
        u.find('input[name="gateway"]').val(k.gateway);
        u.find('input[name="netmask"]').val(k.netmask);
        u.find('input[name="dns1"]').val(k.dns1);
        u.find('input[name="dns2"]').val(k.dns2);
        c.uplink = c.uplink || {};
        u = c.uplink.type == "wireless";
        k = c.uplink_table || {};
        var D = false;
        e.each(k, function (K, C) {
            if (C.configured)D = true
        });
        j.find(".uplinks-data-table").jtable("option",
            "rows", k).jtable("option", "pagesize", k.length).jtable("refresh_options");
        if (c.vwireEnabled && (u || D || !q))j.addClass("ap-needs-uplinks");
        j.css("display", "block");
    }, widgetEventPrefix: "apDialog."});
    e.widget("ui.staDialog", {options: {mac: null, model: null}, _init: function () {
        var c = this;
        this._mac = this.options.mac;
        this._model = this.options.model;
        c.element.dialogTabs();
        c.element.find("div.config-tab-content").accordion({heightStyle: true});
        c.element.mousedown(function () {
            e(document.body).dialogManager("bringToFront",
                c.element)
        });
        c.element.draggable({ containment: "document"});//
        e("#model-usergroups").jlistmodel("refresh");
        c.element.find('select[name="sta-usergroups"]').jselect({model: e("#model-usergroups"), defaultItem: Msg.Automatic});
        c.element.find("div.history-data-table").jtable({url: "/list_session.php", params: {_limit: 5, _sort: "-assoc_time", mac: c.options.mac}, pagesize: 5, emptydiv: c.element.find(".history-data-table-empty")}).jtable("addcol", "", "model", true, Renderer.apIcon).jtable("addcol", Msg.DateTime, "assoc_time", true, function (q) {
            return Renderer.timestamp(q *
                1E3)
        }).jtable("addcol", Msg.Duration, "duration", true, Renderer.uptime).jtable("addcol", Msg.Download, "tx_bytes", true, Renderer.pktsBytes).jtable("addcol", Msg.Upload, "rx_bytes", true, Renderer.pktsBytes);
        c.element.find('input[name="apply"]').japply({get_posts: function () {
            var q = c.element.find("select.fixedAp").val() != "auto" || c.element.find('textarea[name="note"]').val() != "";
            return{url: "/upd_user.php", payload: {mac: c._mac, name: c.element.find('input[name="alias"]').val() || c.sta.mac, note: c.element.find('textarea[name="note"]').val(),
                noted: q, usergroup_id: c.element.find('select[name="sta-usergroups"]').val() == "" ? null : c.element.find('select[name="sta-usergroups"]').val()}}
        }, validate: function () {
            return true
        }, done: function () {
            var q = c.element.find('input[name="alias"]').val() || c.sta.mac, j = c.element.find('textarea[name="note"]').val(), o = c.element.find('select[name="sta-usergroups"]').val();
            e("#model-stas").data("sta_names")[c.sta.mac] = q;
            var l = c._model.jtablemodel("findrow", "mac", c.sta.mac);
            if (l) {
                l.name = q;
                l.note = j;
                l.usergroup_id = o
            }
            c.element.find("h2.sta-name").text(q);
            c.element.find('input[name="alias"]').val(q);
            c.element.find('textarea[name="note"]').val(j);
            c.element.find('select[name="sta-usergroups"]').val(o);
            c.element.find("p.settings-response").addClass("success").show().delay(1E3).fadeOut(500);
            e("#model-stas").jtablemodel("firechanged")
        }});
        c.element.find("select.fixedAp").change(function () {
            var q = c.element.find("select.fixedAp").val() || "auto", j = q != "auto" || c.element.find('textarea[name="note"]').val() != "";
            e.jpost("/upd_user.php", {mac: c._mac, fixed_ap: q, noted: j},
                function () {
                    var o = c._model.jtablemodel("findrow", "mac", c.sta.mac);
                    if (o)o.fixed_ap = q
                })
        });
        e("#model-aps").bind("jtablemodel_refresh-done", c.model_aps_listener = function () {
            c.refresh()
        });
        e("#model-stas").bind("jtablemodel_refresh-done", c.model_stas_listener = function () {
            c.refresh()
        });
        e("#model-usergroups").bind("jlistmodel_refresh-done", c.model_usergroups_listener = function () {
            c.refresh()
        });
        c.element.find(".dialog-hide-button").click(function () {
            c.dispose()
        });
        c.element.find(".block-button").click(function () {
            e.jcmd.block_sta(c.sta)
        });
        c.element.find(".reconnect-button").click(function () {
            e.jcmd.kick_sta(c.sta)
        });
        c.element.find(".unauthorize-button").click(function () {
            e.jcmd.unauthorize_guest(c.sta)
        });
        c.refresh();
        return c
    }, dispose: function () {
        e("#model-aps").unbind("jtablemodel_refresh-done", this.model_aps_listener);
        e("#model-stas").unbind("jtablemodel_refresh-done", this.model_stas_listener);
        e("#model-usergroups").unbind("jlistmodel_refresh-done", this.model_usergroups_listener);
        e(document.body).dialogManager("removeItem", this.element)
    },
        refresh: function () {
            var c = this;
            c.sta = this._model.jtablemodel("findrow", "mac", this._mac);
            if (c.sta) {
                c.sta.connected = true;
                c.user_id = c.sta.user_id;
                c._paint()
            } else e.jpost("/stat_user.php", {mac: c._mac}, function (q) {
                c.sta = q.data[0];
                c.user_id = c.sta._id;
                c._paint()
            })
        }, _paint: function () {
            var c = this.sta, q = this.options.mac;
            q = c.name || c.hostname || q;
            this.element.find("h2.sta-name").text(q);
            this.element.find("dd.mac-address").html(c.mac);
            this.element.find("dd.hostname").text(c.hostname);
            this.element.find("dd.connected-ap").html(Renderer.apDetailLink(c.ap_mac));
            this.element.find("dd.ip-address").html(c.ip);
            this.element.find('input[name="alias"]:nofocus').val(q);
            this.element.find('textarea[name="note"]:nofocus').val(c.note);
            this.element.find('select[name="sta-usergroups"]:nofocus').val(c.usergroup_id || "");
            this.element.find("dd.oui").text(c.oui || "Unknown");
            var j = this.element.find("select.fixedAp");
            j.empty().append(e('<option value="auto">Any AP</option>'));
            e.each(e("#model-aps").jtablemodel("rows"), function (k, z) {
                if (!z.adopted)return true;
                var G = Renderer.apName(z.mac);
                G += z.mac == c.ap_mac ? " (*)" : z.state == 1 ? " (up)" : " (down)";
                e('<option value="' + z.mac + '">' + G + "</option>").appendTo(j)
            });
            j.val(this.sta.fixed_ap || "auto");
            this.element.find("div.history-data-table").jtable("refresh");
            this.element.removeClass("sta-connected");
            this.element.removeClass("sta-disconnected");
            this.element.removeClass("sta-isguest");
            this.element.removeClass("sta-authorized");
            if (c.connected) {
                this.element.addClass("sta-connected");
                c.is_guest && this.element.addClass("sta-isguest");
                c.authorized && this.element.addClass("sta-authorized");
                q = c.tx_packets || 0;
                var o = c.tx_bytes || 0, l = c.rx_packets || 0, p = c.rx_bytes || 0, u = (c["tx_bytes.r"] || 0) + (c["rx_bytes.r"] || 0);
                this.element.find("dd.essid").text(c.essid);
                this.element.find("dd.uptime").html(Renderer.uptime(c.uptime));
                this.element.find("dd.channel").html(Renderer.staChannel(c.channel, c));
                this.element.find("dd.signal").html(Renderer.rssi(c.rssi) + " (" + c.signal + " dBm)");
                this.element.find("dd.powersave").html(c.powersave_enabled ? Msg.Enabled : Msg.NotEnabled);
                this.element.find("dd.tx-rate").html(c.tx_rate /
                    1E3 + "M");
                this.element.find("dd.rx-rate").html(c.rx_rate / 1E3 + "M");
                this.element.find("dd.bytes-rate").html(Renderer.pktsBytes(u) + "B/sec");
                this.element.find("dd.received").html(Renderer.pktsBytes(q) + " / " + Renderer.pktsBytes(o));
                this.element.find("dd.sent").html(Renderer.pktsBytes(l) + " / " + Renderer.pktsBytes(p))
            } else {
                this.element.addClass("sta-disconnected");
                this.element.find("dd.last-seen").html(Renderer.lastseen(c.last_seen * 1E3))
            }
            this.element.css("display", "block")
        }, widgetEventPrefix: "staDialog."})
})(jQuery);
function gmap_inited() {
}
function gmap_failed() {
    $(".gmap-dialog").find("iframe").remove();
    $(".gmap-dialog").dialog("close");
    alert(Msg.E_NoInternetConnection);
}
$(function () {
    var e = $(".gmap-dialog");
    e.data("fn_open", function (c) {
        $(".gmap-dialog").data("name", c);
        $(".gmap-dialog").find("h2").html(Msg.MapStr +"[" + c + "]");
        e.find("iframe").length == 0 && e.find("div.gsmap").append('<iframe src="/bdmap_iframe.php" height="100%" width="100%" style="border:0"></iframe>');
        $(".gmap-dialog").dialog("open")
    });
    e.dialog({autoOpen: false, modal: true, draggable: false, resizable: false, closeOnEscape: false, width: 640, height: 680, closeText: "", dialogClass: "gmap-snapshot-dialog"});
    e.find("input.cancel").click(function () {
        e.dialog("close")
    });
    e.find("input.location").jsearch().bind("jsearch_search", function () {
        e.find("iframe").attr("contentWindow").set_location(e.find("input.location").val())
    });
    e.find("input.snapshot").click(function () {
        var c = e.find("iframe").attr("contentWindow").map;
        c && $.jcmd("cfgmgr", "save_gsmap", {name: e.data("name"), maptype: c.getMapTypeId(), zoom: c.getZoom(), lat: c.getCenter().lat(), lng: c.getCenter().lng()}, function (q) {
            q = q.data[0].map;
            q.selected = true;
            var j = $("#maplet").get(0);
            j && j.flexcall("addMaps", [q]);
            e.dialog("close")
        })
    })
});
$(function () {
    $("#model-aps").bind("jtablemodel_refresh-done", function (e, c) {
        var q = 0, j = 0, o = 0;
        $("#ConnectedAPList").empty();
        $("#DisconnectedAPList").empty();
        $("#PendingAPList").empty();
        //alert($("#ConnectedAP p a").attr("text"));        
        $(".access-points-information a.quick-info-link").click(
            function () {
	            //var stats = new Array(Msg.WTPStatSulking, Msg.WTPStatDiscovery, Msg.WTPStatJoin, 
					//Msg.WTPStatConfigure, Msg.WTPStatDataCheck, Msg.WTPStatRun, 
					//Msg.WTPStatReset, Msg.WTPStatQuit, Msg.WTPStatImageData, Msg.WTPStatBakRun, 
					//Msg.WTPStatOffline);
            	var wtpStats = 
				{
					"ok":"",//(stats[3]+"|"+stats[4]+"|"+stats[5]+"|"+stats[8]+"|"+stats[9]), 
					"error":"",//(stats[0]+"|"+stats[6]+"|"+stats[10]), 
					"pending":""//(stats[1+"|"+stats[2]]+"|"+stats[7])
				};
				var ii = 0;
				var len = stats.length;
				for(ii = 0; ii < len; ii++){
					if(wtpStats[Renderer.apStatusType(ii)])
					{
						wtpStats[Renderer.apStatusType(ii)] += "|";
					}
					wtpStats[Renderer.apStatusType(ii)] += stats[ii];
				}
                $("#MainPanel").tabs("select", 2);
				$("#AccessPointDataTableSearch").val();
                if($(this).hasClass("ok"))
                {
                	e = wtpStats["ok"];
                }
                if($(this).hasClass("error"))
                {
                	e = wtpStats["error"];
                }
                if($(this).hasClass("pending"))
                {
                	e = wtpStats["pending"];
                }
		        //var e = $("#AccessPointDataTableSearch").val();
		        $("#AccessPointDataTable").jtable("addfilter", "*", e == "" ? null : e).jtable("filter")
            }
        );       
        $(".clients-information a.quick-info-link").click(
            function () {
                if($("#UsersTab").text() == this.text)
                {
                	$("#UsersTab").click();
                }
                else if($("#GuestsTab").text() == this.text)
                {
                	$("#GuestsTab").click();
                }
            }
        );
        var l = $("#model-aps").data("ap_names");
        $.each(c._rows, function () {
            var p = $(this), u = p.attr("state");
            u = Renderer.apStatusType(u);
            var k = null;
            if (u == "ok") {
                k = $("#ConnectedAPList");
                o++
            } else if (u == "pending") {
                k = $("#PendingAPList");
                q++
            } else {
                k = $("#DisconnectedAPList");
                j++
            }
            if (k.find("li").length < 8)$('<li><a href="javascript:void(0);" class="ap-link" title="' +
                p.attr("mac") + '">' + l[p.attr("mac")] + "</a></li>").appendTo(k); else k.find("li").length == 8 && $("<li> ... </li>").appendTo(k)
        });
        $("#ConnectedAPList li").size() || $("#ConnectedAPList").append("<li>none</li>");
        $("#DisconnectedAPList li").size() || $("#DisconnectedAPList").append("<li>none</li>");
        $("#PendingAPList li").size() || $("#PendingAPList").append("<li>none</li>");
        $("#ConnectedAPCount").html(o);
		if(o > 0){$("#ConnectedAPCount").css("color", "lightgreen");}
		else{$("#ConnectedAPCount").css("color", "#E0E0E0");/*old color*/}
        $("#DisconnectedAPCount").html(j);
		if(j > 0){$("#DisconnectedAPCount").css("color", "#CC0000");}
		else{$("#DisconnectedAPCount").css("color", "#E0E0E0");}
        $("#PendingAPCount").html(q);
		if(q > 0){$("#PendingAPCount").css("color", "#F28500");}
		else{$("#PendingAPCount").css("color", "#E0E0E0");}
    });
    $("#model-stas").bind("jtablemodel_refresh-done",
        function (e, c) {
            var q = 0, j = 0;
            $.each(c._rows, function () {
                if (this.is_guest)j++; else q++
            });
            $("#UserCount").html(q);
			if(q > 0)$("#UserCount").css("color", "lightgreen");
			else{$("#UserCount").css("color", "#E0E0E0");}
            $("#GuestCount").html(j);
			if(j > 0)$("#GuestCount").css("color", "lightgreen");
			else{$("#GuestCount").css("color", "#E0E0E0");}
        });
    $("#model-aps").jtablemodel("refresh");
    $("#model-stas").jtablemodel("refresh");
    $("div.quick-ap-list").hide();
    $("dd.connection-details").hover(function () {
        $(this).find('div.quick-ap-list[showing!="true"]').attr("showing", "true").slideDown(250)
    }, function () {
        $(this).find("div.quick-ap-list").slideUp(250, function () {
            $(this).attr("showing", null)
        })
    })
});
$(function () {
	  var oldlang = "";
    var e = $("#basicConfForm");
	var agrment=$(".openWlan-agreement");
	  $("#open-wlan").change(function(){
		  if(e.find("input[type=checkbox]").is(":checked"))
		  {	agrment.show();
		    agrment.draggable({containment:"#sytem-page-id"});
		  	var agrment_but = agrment.find(".openWlan-button");
			$("#finishRead").change(function(){
		  	if($(this).is(":checked")) 
				{
				  agrment_but.children().removeClass("disabled");
				  agrment_but.children().removeAttr("disabled");
				  agrment_but.children().first().click(function(){agrment.hide();});
				}
			else  {agrment_but.children().addClass("disabled");
				  agrment_but.children().prop("disabled","disabled");}
		  });
		  }
		  else agrment.hide();
		});
    //e.find("select[name=lang]").change(function(){top.location.href = top.location.href;});
    $.jpost("/list_admin.php", {}, function (q) {
        var j = null;
        $.each(q.data, function (o, l) {
            if (l.for_hotspot)return true; 
            else if(l.currentlogin)
            {
                j = l;
                return false
            }
        });
        e.data("conf", j);
        e.find("input[name=_id]").val(j._id);
        e.find("input[name=name]").val(j.name);
        e.find("input[name=oldname]").val(j.name);
        e.find("input[type=password]").val($.jpass_hint);
        e.find("input[name=alertEnabled]").setchecked(j.email_alert_enabled);
        e.find("input[name=email]").val(e.find("input[name=alertEnabled]").checked() ? j.email : "")
        e.find("select[name=lang]").val(j.lang || "en_US");
        oldlang=e.find("select[name=lang]").val();
    });
    var c = function () {
        return e.find("input[name=newPass]").val() != $.jpass_hint || e.find("input[name=ConfirmPass]").val() != $.jpass_hint
    };
    e.find('input[name="apply"]').japply({get_posts: function () {
        var q = {sysName: e.find("input[name=SysName]").val(),
			_id: e.find("input[name=_id]").val(), 
			oldname: e.find("input[name=oldname]").val(), 
			oldpasswd: hex_md5(e.find("input[name=OriginalPass]").val()), 
			name: e.find('input[name="name"]').val()};
        c() && $.extend(q, {x_password: hex_md5(e.find("input[name=newPass]").val())});
        $.extend(q, {email_alert_enabled: e.find('input[name="alertEnabled"]').checked()});
        e.find('input[name="alertEnabled"]').checked() && $.extend(q, {email: e.find('input[name="email"]').val()});
        q.lang = e.find("select[name=lang]").val();
        
        return {url: "/upd_admin.php", payload: q};
    }, validate: function () {
        if (!V.checkEmpty(e.find("input[name=SysName]"),Msg.SystemName)||!V.checkEmpty(e.find("input[name=name]"), Msg.AdminAccount) || !V.checkAdminName(e.find("input[name=name]"), Msg.AdminAccount))return false;
        if (c())if (!V.checkAdminPasswords(e.find("input[name=newPass]"), e.find("input[name=ConfirmPass]"), Msg.AdminPassword, Msg.ConfirmPassword))return false;
        if(c() && !V.checkOriginalPasss(e.find("input[name=OriginalPass]"))) return false;
        if (e.find("input[name=alertEnabled]").checked()) {
            var q =
                e.find('input[name="email"]');
            if (!V.checkEmpty(q, Msg.EmailAddress))return false;
            if (!V.checkEmail(q, Msg.EmailAddress))return false
        }
        return true
    }, done: function(){if(oldlang != e.find("select[name=lang]").val()){top.location.href = (top.location.href).replace(/#.*/, "");}}})
});
//himgod update

function update_config(url_php,node,node_value)
{
    $.ajax({ 
            async: false, 
            type : "POST", 
            url : url_php, 
            data:{key_node:node,key_value:node_value},
            dataType : 'json', 
            success : function(data) { 
                
                //alert(tmpdata);
                } 
        });
}
$(document).ready(function(){

    $(".create-qrcode-button").click(function(){
        $(".Qrcode-card").css("display","block");
        $(".Qrcode-card").css("top","100px");
        $(".Qrcode-card").css("left","350px");
        $(".Qrcode-card").draggable({handle: "div.Qrcode-card-title",containment: "window"});

    });
    $("#original-pic").click(function(){
        $(".wifi-qrcode-design").find("label[for='original-pic']").addClass("radio-select").removeClass("radio-unselect");
        $(".wifi-qrcode-design").find("label[for='fillet']").addClass("radio-unselect").removeClass("radio-select");
    });
    $("#fillet").click(function(){
        $(".wifi-qrcode-design").find("label[for='original-pic']").addClass("radio-unselect").removeClass("radio-select");
        $(".wifi-qrcode-design").find("label[for='fillet']").addClass("radio-select").removeClass("radio-unselect");
    });
    
    $("#close2").click(function(){
        $(".Qrcode-card").css("display","none");
    });
    $(".down-wifi-card-div").click(function(){
        var other = $(".Qrcode-card-div2").css("display");
        if( other == "none")
        {
            $(".Qrcode-card-div2").css("display","block");
        }
        else
        {
            $(".Qrcode-card-div2").css("display","none");
        }
    });
    $("#f3").click(function(){
        if($("#f3").hasClass("on"))
        {
            $("#AFIStrategyTable").css("display","none");
            $("#wireless-add2").css("display","none");
        }
        else
        {
            $("#AFIStrategyTable").css("display","block");
            $("#wireless-add2").css("display","block");
        }
    });
    $("#f3").mouseup(function (){
      });
    $("#f3").mousedown(function (){
      });
    $("#f3").mouseout(function (){
      });
    $("#f3").mousemove(function (){
      }); 
    $("#f1").click(function(){
        if($("#f1").hasClass("on"))
        {
            //$("#f1").addClass("off").removeClass("on");
            update_config("../../api/update_config.php","network_adaption",0);
        }
        else
        {   //$("#f1").addClass("on").removeClass("off");
            update_config("../../api/update_config.php","network_adaption",1);
        }
    });
    $("#f2").click(function(){
        if($("#f2").hasClass("on"))
        {
            //$("#f1").addClass("off").removeClass("on");
            update_config("../../api/update_config.php","access_control",0);
        }
        else
        {   //$("#f1").addClass("on").removeClass("off");
            update_config("../../api/update_config.php","access_control",1);
        }
    });
    // $(".wifi-qrcode-button").click(function(){
    //     $(".user-group").css("display","none");
    // });
    $('#en-update-nav-ul li a').click(function(){
        $("#en-update-nav-ul li[id='choose-button']").removeAttr("id");
        $(this).parent().attr('id',"choose-button");
        update_config("../../api/update_config.php","version_update",$(this).html());
   })
    $("#CountryCode option").click(function(){
        var val = $(this).val();
        alert(val+"hello");
    });
    $("#CountryCode").change(function(){
        var val = $("#CountryCode").find("option:selected").val();
        update_config("../../api/update_config.php","country_code",val);
    });
    $("#optimize-drag").mouseup(function(){
        var optimize_val = $("#optimize-drag").find(".dragging-comment span").html();
        update_config("../../api/update_config.php","global_auto_optimize",optimize_val);
    });

});

$(function () {
    var q = $(".AFI-black-list-add");
    $("#wireless-add2").click(function () {
        j({name: "Name", security: "wpapsk", enabled: true})
    });
    $("#close4").click(function(){
        q.css("display","none");
    }); 
    var j = function (l) {
        var p = l._id == undefined ? true :false;
        if(!p)
        {
            $("#afi-blacklist-add-title").html(Msg.View);
            var tmp = $(".afi-black-list-table");
            tmp.html("<table><tr><td></td><td>" + Msg.NameOrMac + "</td><td>" + Msg.IsBlocked + "</td></tr><tr><td></td><td>" + l.name + "</td><td><div class='switch on'></div></td></tr></table>");
            var zEvent = tmp.find(".switch");
            zEvent.click(function(){
                if(zEvent.hasClass('on'))
                {
                    zEvent.addClass('off').removeClass('on');
                }
                else
                {
                    zEvent.addClass('on').removeClass('off');
                }
            });
        }
        else
        {
            $("#afi-blacklist-add-title").html(Msg.AddUser);
            var tmp = $(".afi-black-list-table");
            tmp.html("");
            $(".afi-black-list-table").jtable({url: "/list_user.php", params: {blocked: true}, objname: "user", className: "stat"}).jtable("addcol","","",true).jtable("addcol", Msg.NameOrMac, "mac", true, function (e, c) {
                return Renderer.staDetailLinkWithName(e, c.name || c.hostname || c.mac)
            }).jtable("addaction",Msg.View, "unblock").bind("jtable_unblock", function (e, c) {
                j(c)
            }).jtable("refresh");
        }
        q.css("display","block");
        q.css("margin-left","-230px");
        q.css("top","38%");
        q.css("left","50%");
        q.draggable({handle: "p.add-page-title",containment: "#system-page-id"});
        //p && q.find('input[name="name"]').select().focus()
    };

    $("#BlackListTableContainer1").jtable({url: "/list_user.php", params: {blocked: true}, objname: "user", className: "stat"}).jtable("addcol","","",true).jtable("addcol", "", "mac", true, function (e, c) {
        return Renderer.staDetailLinkWithName(e, c.name || c.hostname || c.mac)
    }).jtable("addaction",Msg.View, "unblock").bind("jtable_unblock", function (e, c) {
        j(c)
    }).jtable("addaction", Msg.DeleteStr, "unblock2").bind("jtable_unblock", function (e, c) {
       // $.jcmd.unblock_sta(c)
    }).jtable("refresh");
    $("BlackListTableContainer1").bind("jcmd_stablocked", function () {
        $("#BlackListTableContainer1").jtable("refresh")
    }).bind("jcmd_staunblocked",
        function () {
            $("#BlackListTableContainer1").jtable("refresh")
        })
});
//himgod
$(document).ready(function(){
         
    $("#f4").click(function(){
        if($("#f4").hasClass("on"))
        {
            $("#blacklistTable").css("display","none");
            $("#wireless-add4").css("display","none");

        }
        else
        {
            $("#blacklistTable").css("display","block");
            $("#wireless-add4").css("display","block");
        }
    });
    $("#f4").mouseup(function (){
      });
    $("#f4").mousedown(function (){
      });
    $("#f4").mouseout(function (){
      });
    $("#f4").mousemove(function (){
      });
}); 
$(function () {
    var q = $(".user-black-list-add");
    $("#wireless-add4").click(function () {
        j({name: "Name", security: "wpapsk", enabled: true})
    });
    $("#close3").click(function(){
        q.css("display","none");
    }); 
    var j = function (l) {
        var p = l._id == undefined ? true :false;
        if(!p)
        {
            $("#user-blacklist-add-title").html(Msg.View);
            var tmp = $(".user-black-list-table");
            tmp.html("<table><tr><td></td><td>" + Msg.NameOrMac + "</td><td>" + Msg.IsBlocked + "</td></tr><tr><td></td><td>" + l.name + "</td><td><div class='switch on' ></div></td></tr></table>");
            var zEvent = tmp.find(".switch");
            zEvent.click(function(){
                if(zEvent.hasClass('on'))
                {
                    zEvent.addClass('off').removeClass('on');
                }
                else
                {
                    zEvent.addClass('on').removeClass('off');
                }
            });
        }
        else
        {
            $("#user-blacklist-add-title").html(Msg.AddUser);
            var tmp = $(".user-black-list-table");
            tmp.html("");
            $(".user-black-list-table").jtable({url: "/list_user.php", params: {blocked: true}, objname: "user", className: "stat"}).jtable("addcol","","",true).jtable("addcol", Msg.NameOrMac, "mac", true, function (e, c) {
                return Renderer.staDetailLinkWithName(e, c.name || c.hostname || c.mac)
            }).jtable("addaction",Msg.View, "unblock").bind("jtable_unblock", function (e, c) {
                j(c)
            }).jtable("refresh");
        }
        q.css("display","block");
        q.css("margin-left","-230px");
        q.css("top","70%");
        q.css("left","50%");
        q.draggable({handle: "p.add-page-title",containment: "#system-page-id"});
        //p && q.find('input[name="name"]').select().focus()
    };

    $("#BlackListTableContainer2").jtable({url: "/list_user.php", params: {blocked: true}, objname: "user", className: "stat"}).jtable("addcol","","",true).jtable("addcol", "", "mac", true, function (e, c) {
        return Renderer.staDetailLinkWithName(e, c.name || c.hostname || c.mac)
    }).jtable("addaction", Msg.View, "unblock").bind("jtable_unblock", function (e, c) {
        j(c)
    }).jtable("addaction", Msg.DeleteStr, "unblock2").bind("jtable_unblock1", function (e, c) {
        //$.jcmd.unblock_sta(c)
    }).jtable("refresh");
    $("BlackListTableContainer2").bind("jcmd_stablocked", function () {
        $("#BlackListTableContainer2").jtable("refresh")
    }).bind("jcmd_staunblocked",

        function () {
            $("#BlackListTableContainer2").jtable("refresh")
        })
});


$(function () {
    $("#BlockedDevicesTableContainer").jtable({url: "/list_user.php", params: {blocked: true}, objname: "user", className: "stat"}).jtable("addcol", Msg.MACAddress, "mac", true, function (e, c) {
        return Renderer.staDetailLinkWithName(e, c.name || c.hostname || c.mac)
    }).jtable("addaction", Msg.Unblock, "unblock").bind("jtable_unblock", function (e, c) {
        $.jcmd.unblock_sta(c)
    }).jtable("refresh");
    $("#BlockedDevicesTableContainer").bind("jcmd_stablocked", function () {
        $("#BlockedDevicesTableContainer").jtable("refresh")
    }).bind("jcmd_staunblocked",
        function () {
            $("#BlockedDevicesTableContainer").jtable("refresh")
        })
});


$(function () {
    function e() {
        var j = c.find('input[name="portalEnabled"]').checked();
        c.find(".auth-type").setvisible(j);
        var o = c.val_radio("guestAuthentication");
        c.find(".auth-password").setvisible(j && o == "password");
        c.find(".term-of-use").setvisible(j && (o == "none" || o == "password"));
        c.find(".expire").setvisible(j && (o == "none" || o == "password"));
        c.find(".redirect").setvisible(j && (o == "none" || o == "password" || o == "hotspot"));
        c.find(".custom-ip").setvisible(j && o == "custom");
        c.find(".custom-portal").setvisible(j && o !=
            "custom");
        c.find("fieldset.hotspot").setvisible(j && o == "hotspot");
        c.find(".portal-hostname").setvisible(j)
    }
//himgod add
function displaySearchData(dataset, index){
    if(dataset == undefined || dataset == '')
        return false;
    var num = 0;
    index = index * 6 - 6;
    $("#AuthUserDataTable table tbody").empty();
    $.each(dataset, function(){
        var lineOddEven = '';
        if(num >= 6)
            return false;
        if(num%2 == 0)
            lineOddEven = "odd";
        else
            lineOddEven = "even";
        var appendTables = '';
        if(dataset[index]==undefined)
            return false;
        appendTables += '<tr class="'+lineOddEven+'">';
        appendTables += '<td></td>';
        appendTables += '<td>'+dataset[index].username+'</td>';
        appendTables += '<td>'+dataset[index].mac+'</td>';
        appendTables += '<td>'+dataset[index].ip+'</td>';
        appendTables += '<td>'+Math.round((dataset[index].outgoing/1024) * 100) / 100+'</td>';
        appendTables += '<td>'+Math.round((dataset[index].incoming/1024) * 100) / 100+'</td>';
        appendTables += '<td>'+dataset[index].created_at+'</td>';
        appendTables += '</tr>';
        $("#AuthUserDataTable table tbody").append(appendTables);
        /* $("#AuthUserDataTable table tbody").append('<tr class="'+lineOddEven+'"></tr>');
        $("#AuthUserDataTable table tbody").append('<td>'+dataset[index].username+'</td>');
        $("#AuthUserDataTable table tbody").append('<td>'+dataset[index].mac+'</td>');
        $("#AuthUserDataTable table tbody").append('<td>'+dataset[index].ip+'</td>');
        $("#AuthUserDataTable table tbody").append('<td>'+(dataset[index].outgoing)/1024+'</td>');
        $("#AuthUserDataTable table tbody").append('<td>'+(dataset[index].incoming)/1024+'</td>');
        $("#AuthUserDataTable table tbody").append('<td>'+dataset[index].created_at+'</td>');
        */
        num ++;
        index ++;
    });
}
    var auth_url = "../../api/auth_proxy.php";
    function authConfig(action, data){
        var dataString = "action=" + action;
        if(data == null || data == undefined || data == "")
            dataString += "&data";
        else 
            dataString += "&data=" + data;
        $.ajax({
            type: 'post',
            async: false,
            url: auth_url,
            data: dataString,
            success: function(){
                //return true;
            },
            error: function(){
                //return false;
            }
        });
    }
    function authAddUser(user, passwd){
        var dataString = "";
        dataString += "&username=" + user;
        dataString += "&password=" + passwd;
        authConfig('authAddUser', dataString);
    }

    /*function checkStrong(sValue) {
        var modes = 0;
        //正则表达式验证符合要求的
        if (sValue.length < 1) return modes;
        if (/\d/.test(sValue)) modes++; //数字
        if (/[a-z]/.test(sValue)) modes++; //小写
        if (/[A-Z]/.test(sValue)) modes++; //大写  
        if (/\W/.test(sValue)) modes++; //特殊字符
       
       //逻辑处理
        switch (modes) {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
            case 4:
                return sValue.length < 12 ? 3 : 4
                break;
        }
    }*/


    $(document).ready(function(){
        
        $('#guest-portal .eye').click(function() {
            /* Act on the event */
            var type = $('#Password').attr('type');
            if(type == 'password')
                $('#Password').attr('type', 'text');
            else
                $('#Password').attr('type', 'password');
        }); 

        $("#f4-2").click(function(){
            if($("#f4-2").hasClass("off"))
            {
                $("#guest-portal").css("display","none");
                $(".validity").css("display","none");
                $(".jump-after").css("display","none");
                $("#subnet").css("display","none");
                $("#f4-2").attr("checked","true");
            }
            else
            {
                $("#guest-portal").css("display","block");
                $(".validity").css("display","block");
                $(".jump-after").css("display","block");
                $("#subnet").css("display","block");
            }
        }); 

        $("#auth-time option").click(function(){
            var authTime = $(this).attr("value");
            //console.log(authTime);
            authConfig('authSetAuthEndTimeminute', authTime*60);
        });

        $("#original").click(function(){
           if($("#original").is(":checked")) 
           {
                //if($('input[name=jump][checked]').val();
                $(".radio-style label[for='custom']").addClass("radio-unselect").removeClass("radio-select");
                $(".radio-style label[for='original']").addClass("radio-select").removeClass("radio-unselect");
                $(".radio-style").find("input[name='protal']").attr("disabled",true);
                authConfig('authClosePortalPage', $("#custom_portal").attr("value"));
           } 
       });
        $("#custom").click(function(){
           if($("#custom").is(":checked")) 
           {
                $(".radio-style label[for='original']").addClass("radio-unselect").removeClass("radio-select");
                $(".radio-style label[for='custom']").addClass("radio-select").removeClass("radio-unselect");
                $(".radio-style").find("input[name='protal']").attr("disabled",false);
                authConfig('authSetPortalPage', $("#custom_portal").attr("value"));
           } 
       });

        $("#custom_portal").focusout(function(){
            var portal = $("#custom_portal").val();
            console.log(portal);
            var reg = new RegExp("(?:https|http)://");
            if(!reg.test(portal)){
                portal = "http://" + portal;
                //console.log(portal);
            }
            authConfig('authSetPortalPage', portal);
            $("#custom_portal").attr("value", portal);
            $("#custom_portal").val(portal);
        }); 

        $("#no").click(function(){
           if($("#no").is(":checked")) 
           {
                //if($('input[name=jump][checked]').val();
                $(".radio-style label[for='PORTAL']").addClass("radio-unselect").removeClass("radio-select");
                $(".radio-style label[for='no']").addClass("radio-select").removeClass("radio-unselect");
                $(".authentication").css("display","none");
           } 
           authConfig('authSwitchApAuthSplashOnlyPlugin', '');
       });
        /*$('#auth-user').focus(function() {
            $('#AdministrationContent .name-support').css("display", "block");
        });
        $('#auth-user').keyup(function() {
            this.value = this.value.replace(/^ +| +$/g, '');
        });
        $('#auth-user').blur(function() {
            $('.authentication .name-support').css("display", "none");
        });*/

        $("#auth-add").click(function(){
            var auth_user = $("#auth-user").val();
            var auth_pw = $("#Password").val();
            var res = authAddUser(auth_user, auth_pw);
            if(res == '1')
                alert(Msg.Auth_AddSuccess);
            else if(res == '0')
                alert(Msg.Auth_AddExistUser);
            else
                alert(Msg.Auth_AddNetError);
        });

        $("#auth-delete").click(function(){
            var auth_user = $("#auth-user").val();
            var res = authConfig('authDeleteUser', auth_user);
            if(res == '1')
                alert(Msg.Auth_DeleteSuccess);
            else if(res == '0')
                alert(Msg.Auth_DeleteUnexistUser);
            else
                alert(Msg.Auth_DeleteNetError);
        });
/*
        auth_search_user = [{"id":"3","username":"hello","mac":"c46ab7fb3838","ip":"192.168.46.47","incoming":"0.00","outgoing":"0.00","created_at":"2015-01-07 16:55:04","node_id":"1"},
        {"id":"2","username":"user","mac":"c46ab7fb3838","ip":"192.168.46.47","incoming":"49139920.00","outgoing":"2324031.00","created_at":"2015-01-05 15:23:12","node_id":"1"},
        {"id":"2","username":"user","mac":"6c71d96c4af3","ip":"192.168.46.30","incoming":"1477568.00","outgoing":"0.00","created_at":"2015-01-05 16:00:18","node_id":"1"},
        {"id":"2","username":"user","mac":"a84481cb3520","ip":"192.168.46.49","incoming":"23712574.00","outgoing":"1000073.00","created_at":"2015-01-06 10:10:13","node_id":"1"},
        {"id":"2","username":"user","mac":"6c71d96c4af3","ip":"192.168.46.30","incoming":"4170226.00","outgoing":"968668.00","created_at":"2015-01-07 16:31:26","node_id":"1"},
        {"id":"2","username":"user","mac":"6c71d96c4af3","ip":"192.168.46.30","incoming":"2612586.00","outgoing":"255248.00","created_at":"2015-01-07 16:41:19","node_id":"1"},
        {"id":"2","username":"user","mac":"6c71d96c4af3","ip":"192.168.46.30","incoming":"825620.00","outgoing":"183512.00","created_at":"2015-01-07 16:44:37","node_id":"1"},
        {"id":"2","username":"user","mac":"6c71d96c4af3","ip":"192.168.46.30","incoming":"8175667.00","outgoing":"1447573.00","created_at":"2015-01-07 16:46:42","node_id":"1"}];   
        */
        $("#auth-list").click(function(){
            var auth_user = $("#auth-user").val();
            var auth_search_user = authConfig('authSearchUser', auth_user);
            
            $("#AuthUserDataTable").css("display", "block");
            $("#AuthUserDataTable table tbody").html("");
            displaySearchData(auth_search_user, 1);
            var $data_table_paging_details = $("#AuthUserDataTable .data-table-paging-details");
            var userCount = auth_search_user.length;
            if(userCount <= 0){
                $data_table_paging_details.html('0 - 0 / 0');
            }
            else if(userCount <= 6){
                $data_table_paging_details.html('1 - ' + userCount + ' / '+userCount);
            }
            else{
                var last_page = Math.floor(userCount / 6)+1;
                $data_table_paging_details.html('1 - 6 / ' + userCount);
                $("#AuthUserDataTable .data-table-status a:last").removeClass('next-button-disabled');
                $("#AuthUserDataTable .data-table-status a:last").addClass('next-button');
                $("#AuthUserDataTable .data-table-status a:last").click(function(){
                    var $pageEle = $("#AuthUserDataTable .data-table-status .ui-corner-all");
                    var $nowPage = Number($pageEle.html())+1;
                    $pageEle.html($nowPage);
                    if($nowPage > 1){
                        $("#AuthUserDataTable .data-table-status a:first").removeClass('back-button-disabled');
                        $("#AuthUserDataTable .data-table-status a:first").addClass('back-button');
                    }
                    displaySearchData(auth_search_user, $nowPage);
                    if($nowPage == last_page){
                        $("#AuthUserDataTable .data-table-status a:last").removeClass('next-button');
                        $("#AuthUserDataTable .data-table-status a:last").addClass('next-button-disabled');
                        $data_table_paging_details.html($nowPage*6-5+' - ' + userCount + ' / '+userCount);
                    }
                    else{
                        $data_table_paging_details.html($nowPage*6-5+' - ' + $nowPage*6 + ' / '+userCount);
                    }
                });
                $("#AuthUserDataTable .data-table-status a:first").click(function(){
                    var $pageEle = $("#AuthUserDataTable .data-table-status .ui-corner-all");
                    var $nowPage = Number($pageEle.html())-1;
                    $pageEle.html($nowPage);
                    if($nowPage < last_page){
                        $("#AuthUserDataTable .data-table-status a:last").removeClass('next-button-disabled');
                        $("#AuthUserDataTable .data-table-status a:last").addClass('next-button');
                    }
                    if($nowPage == '1'){
                        $("#AuthUserDataTable .data-table-status a:first").removeClass('back-button');
                        $("#AuthUserDataTable .data-table-status a:first").addClass('back-button-disabled');
                    }
                    displaySearchData(auth_search_user, $nowPage);
                    $data_table_paging_details.html($nowPage*6-5+' - ' + $nowPage*6 + ' / '+userCount);
                });
            
            }
        });

        $("#PORTAL").click(function(){
           if($("#PORTAL").is(":checked")) 
           {
                 $(".radio-style label[for='no']").addClass("radio-unselect").removeClass("radio-select");
                 $(".radio-style label[for='PORTAL']").addClass("radio-select").removeClass("radio-unselect");
                 $(".authentication").css("display","block");
           } 
           authConfig('authSwitchApAuthLocalUserPlugin', '');
       });


    });

    var c = $("#GuestControlForm");
    c.find('input[name="portalEnabled"]').change(e);
    c.find('input[name="guestAuthentication"]').change(e);
    c.find('input[name="doRedirect"]').change(function () {
        c.find('input[name="redirectUrl"]').setenabled(c.val_radio("doRedirect") == "true")
    });
    c.find('select[name="expire"]').change(function () {
        c.find("span.customExpire").toggle(c.find('[name="expire"]').val() == "custom")
    });
    c.find("input[name=doPayment]").change(function () {
        c.find("input.paypal").setenabled(c.find("input[name=doPayment]").checked())
    });
    c.find("input[name=portalUseHostname]").change(function () {
        c.find("input[name=portalHostname]").setenabled(c.find("input[name=portalUseHostname]").checked())
    });
    $.each(["restricted", "allowed"], function (j, o) {
        //c.find("input." + o + "-subnets").live("keyup", function () {
        c.find("input." + o + "-subnets:last").on("keyup", function () {
            var l = true;
            c.find("input." + o + "-subnets").each(function (p, u) {
                if ($(u).val() == "")l = false;
            });
            l && q(o, "");
        });
    });
    var q = function (j, o) {
        var l = c.find("p." + j + "-subnets");
        var newCreate = $('<input text="text" class="' + j + '-subnets text-input" name="subnet"/>');
        newCreate.val(o).appendTo(l);
        $("<br />").appendTo(l);
        newCreate.on("keyup", function () {
            var l = true;
            c.find("input." + j + "-subnets").each(function (p, u) {
                if ($(u).val() == "")l = false;
            });
            l && q(j, "");
        });
    };
    c.data("fn_fillform", function (j) {
        var o = $(j.guest_access || {});
        c.find('input[name="portalEnabled"]').setchecked(o.attr_is("portal_enabled", false));
        c.check_radio("guestAuthentication", o.attr_get("auth", "none"));
        c.find('input[name="guestPassword"]').jpass({value: o.attr("x_password")});
        c.check_radio("doRedirect", o.attr_get("redirect_enabled", false));
        c.find('input[name="redirectUrl"]').val(o.attr("redirect_url"));
        c.find('input[name="customIp"]').val(o.attr("custom_ip"));
        c.find('select[name="expire"]').val(o.attr_get("expire", 480)).change();
        c.find('input[name="expireNumber"]').val(o.attr_get("expire_number", 8));
        c.find('select[name="expireUnit"]').val(o.attr_get("expire_unit", 60));
        c.find("input[name=doVoucher]").setchecked(o.attr_is("voucher_enabled", false));
        c.find("input[name=doPayment]").setchecked(o.attr_is("payment_enabled", false));
        c.find("input[name=paypalUsername]").val(o.attr("x_paypal_username"));
        c.find("input[name=paypalPassword]").val(o.attr("x_paypal_password"));
        c.find("input[name=paypalSignature]").val(o.attr("x_paypal_signature"));
        c.find("input[name=doPaypalUseSandbox]").setchecked(o.attr_is("paypal_use_sandbox", false));
        c.find("input[name=doCustomPortal]").setchecked(o.attr_is("portal_customized", false));
        c.find("input[name=portalUseHostname]").setchecked(o.attr_is("portal_use_hostname", false));
        c.find("input[name=portalHostname]").val(o.attr("portal_hostname"));
        $.each(["restricted", "allowed"], function (l, p) {
            c.find("p." + p + "-subnets").empty();
            for (var u = 1; u <
                32; u++) {
                var k = o.attr(p + "_subnet_" + u);
                if (!k)break;
                q(p, k)
            }
            q(p, "")
        })
    });
    c.find('input[name="apply"]').japply({get_posts: function () {
        var j = {};
        j.portal_enabled = c.find('input[name="portalEnabled"]').checked();
        j.auth = c.val_radio("guestAuthentication");
        j.x_password = c.find('input[name="guestPassword"]').jpass("val");
        j.redirect_enabled = c.val_radio("doRedirect") == "true";
        j.redirect_url = c.find('input[name="redirectUrl"]').val();
        j.custom_ip = c.find('input[name="customIp"]').val();
        j.expire = c.find('select[name="expire"]').val();
        if (j.expire == "custom") {
            j.expire_number = c.find('input[name="expireNumber"]').val();
            j.expire_unit = c.find('select[name="expireUnit"]').val()
        }
        j.voucher_enabled = c.find("input[name=doVoucher]").checked();
        j.payment_enabled = c.find("input[name=doPayment]").checked();
        j.x_paypal_username = c.find("input[name=paypalUsername]").val();
        j.x_paypal_password = c.find("input[name=paypalPassword]").val();
        j.x_paypal_signature = c.find("input[name=paypalSignature]").val();
        j.paypal_use_sandbox = c.find("input[name=doPaypalUseSandbox]").checked();
        j.portal_customized = c.find("input[name=doCustomPortal]").checked();
        j.portal_use_hostname = c.find("input[name=portalUseHostname]").checked();
        j.portal_hostname = c.find("input[name=portalHostname]").val();
        $.each(["restricted", "allowed"], function (o, l) {
            o = 1;
            c.find("input." + l + "-subnets").each(function () {
                var p = $.trim($(this).val());
                if (p)j[l + "_subnet_" + o++] = p; else j[l + "_subnet_" + o++] = null
            })
        });
        return{url: "/set_setting_guest_access.php", payload: j}
    }, validate: function () {
        if (c.find('input[name="portalEnabled"]').checked()) {
            var j =
                c.val_radio("guestAuthentication");
            if (c.find('[name="expire"]').val() == "custom")if (!V.checkInteger(c.find('input[name="expireNumber"]'), Msg.ExpireTime))return false;
            if (j == "password") {
                if (!V.checkEmpty(c.find('input[name="guestPassword"]'), Msg.GuestPassword))return false
            } else if (j == "custom")if (!V.checkEmpty(c.find('input[name="customIp"]'), Msg.IPAddress) || !V.checkIP(c.find('input[name="customIp"]'), Msg.CustomPortal))return false;
            if (c.val_radio("doRedirect") == "true")if (!V.checkEmpty(c.find('input[name="redirectUrl"]'),
                Msg.PromotionalUrl))return false;
            if (c.find("input[name=portalUseHostname]").checked())if (!V.checkEmpty(c.find("input[name=portalHostname]"), Msg.PortalHostname) || !V.checkRE(c.find("input[name=portalHostname]"), Msg.PortalHostname, V.reHostname, Msg.E_InvalidHostname))return false;
            if (c.find("input[name=doPayment]").checked()) {
                if (!V.checkEmpty(c.find("input[name=paypalUsername]"), Msg.PaypalUsername))return false;
                if (!V.checkEmpty(c.find("input[name=paypalPassword]"), Msg.PaypalPassword))return false;
                if (!V.checkEmpty(c.find("input[name=paypalSignature]"),
                    Msg.PayPalSignature))return false
            }
            if (j == "hotspot" && !c.find("input[name=doPayment]").checked() && !c.find("input[name=doVoucher]").checked()) {
                alert(V.localize(Msg.E_GuestNoHotspotPaymentSelected));
                return false
            }
        }
        var o = true;
        c.find("input.restricted-subnets, input.allowed-subnets").each(function () {
            var l = $.trim($(this).val());
            if (l)if (!V.checkSubnetSpec($(this), l))return o = false
        });
        if (!o)return false;
        return true
    }, done: function () {
        c.find('input[name="guestPassword"]').jpass("hide")
    }})
});
$(function () {
    var e = $("#SystemForm");
    e.find('input[name="useFixedURL"]').change(function () {
        e.find('input[name="fixedURLHost"], #SystemForm input[name="fixedURLPort"]').setenabled(e.find('input[name="useFixedURL"][value="true"]').checked())
    });
    e.find('input[name="enableLoadBalance"]').change(function () {
        e.find('input[name="maxSta"]').setenabled($(this).checked())
    });
    e.find('input[name="enableConnectivityMonitor"]').change(function () {
        e.find(".isolated-wlan-status").setvisible($(this).checked());
        e.find(".uplink-type").setvisible($(this).checked());
        $(this).checked() ? e.find('input[name="uplinkType"]').change() : e.find(".custom-uplink-host").setvisible(false)
    });
    e.find('input[name="uplinkType"]').change(function () {
        var j = e.val_radio("uplinkType");
        e.find(".custom-uplink-host").setvisible(j == "custom")
    });
    e.find('input[name="enableRemoteSyslog"]').change(function () {
        e.find('input[name="syslogHost"], input[name="syslogPort"]').setenabled($(this).checked())
    });
    e.find('input[name="enableSmtp"]').change(function () {
        var j = $(this).checked();
        e.find('input[name="smtpHost"], input[name="smtpPort"]').setenabled(j);
        e.find(".mail-server,.mail-sender,.test-mail-server").setvisible(j)
    });
    e.find('input[name="smtpSSL"]').change(function () {
        e.find('input[name="smtpPort"]').val($(this).checked() ? "465" : "25")
    });
    e.find('input[name="smtpAuthentication"]').change(function () {
        var j = $(this).checked();
        e.find('input[name="userName"], input[name="userPassword"]').setenabled(j)
    });
    e.find('input[name="smtpSender"]').change(function () {
        e.find('input[name="sender"]').setenabled($(this).checked())
    });
    e.find("input[name=scanNow]").click(function () {
        var j =
            $(this);
        j.disable();
        $.jcmd("devmgr", "scan-uplink", {map_id: ""}, function () {
            setTimeout(function () {
                j.enable();
                $("#model-aps").jtablemodel("refresh")
            }, 15E3)
        })
    });
    $.jpost("/stat_config.php", {}, function (j) {
        j = j.data[0];
        e.find("span.current-management-url").data("inform", j).html(j.url)
    });
    var c = function () {
        var j = e.find('input[name="smtpHost"]'), o = e.find('input[name="smtpPort"]'), l = e.find('input[name="sender"]');
        if (!V.checkEmpty(j, Msg.SMTPServerHost))return false;
        if (!V.checkEmpty(o, Msg.SMTPServerPort) || !V.checkMinMax(o,
            1, 65535, Msg.SMTPServerPort))return false;
        if (e.find('input[name="smtpSender"]').checked()) {
            if (!V.checkEmpty(l, Msg.SMTPSenderAddress))return false;
            if (!V.checkEmail(l, Msg.SMTPSenderAddress))return false
        }
        if (e.find('input[name="smtpAuthentication"]').checked()) {
            j = e.find('input[name="userName"]');
            o = e.find('input[name="userPassword"]');
            if (!V.checkEmpty(j, Msg.Username))return false;
            if (!V.checkEmpty(o, Msg.PasswordStr))return false
        }
        return true
    };
    e.find('input[name="testSmtp"]').click(function () {
        e.find('input[name="testSmtp"]');
        if (e.find('input[name="enableSmtp"]').checked()) {
            if (c()) {
                var j =
                    e.find('input[name="email"]');
                if (!V.checkEmpty(j, Msg.EmailAddress) || !V.checkEmail(j, Msg.EmailAddress))return false;
                $.jcmd("cfgmgr", "test_mail", {email: j.val(), host: e.find('input[name="smtpHost"]').val(), port: e.find('input[name="smtpPort"]').val(), use_sender: e.find('input[name="smtpSender"]').checked(), sender: e.find('input[name="sender"]').val(), use_ssl: e.find('input[name="smtpSSL"]').checked(), use_auth: e.find('input[name="smtpAuthentication"]').checked(), username: e.find('input[name="userName"]').val(),
                    x_password: e.find("input[name=userPassword]").jpass("val")}, function () {
                    alert(V.localize(Msg.I_SmtpEmailSent, j.val()))
                })
            }
        } else alert(V.localize(Msg.E_SmtpNotConfigured))
    });
    e.data("fn_fillform", function (j) {
        var o = $(j.identity || {}), l = $(j.inform || {}), p = $(j.rsyslogd || {}), u = $(j.connectivity || {}), k = $(j.mgmt || {}), z = $(j.bgscan || {}), G = $(j.load_balance || {}), w = $(j.country || {});
        j = $(j.smtp || {});
        $("#SystemForm").data("identity", o).data("mgmt", k).data("connectivity", u).data("inform", l).data("rsyslogd", p).data("bgscan",
            z).data("load_balance", G).data("country", w).data("smtp", j);
        e.find('input[name="systemName"]').val(o.attr("name"));
        $("#SystemForm").check_radio("useFixedURL", l.attr_is("customized", false));
        e.find('input[name="fixedURLHost"]').val(l.attr_get("host", ""));
        e.find('input[name="fixedURLPort"]').val(l.attr_get("port", ""));
        var D = e.find('select[name="countryCode"]');
        $.each($(document.body).data("sysinfo").ccodes, function (K, C) {
        		var showname;
						if(currentLocale == "en_US")
						{
							showname = C.name;
						}
						else
						{
							showname = country[C.name];
						}
            $("<option></option>").val(C.code).html(showname).appendTo(D)
        });
        D.val(w.attr_get("code",
            156));
        e.find('input[name="enableBackgroundScanning"]').setchecked(z.attr_is("enabled", true));
        e.find('input[name="enableLoadBalance"]').setchecked(G.attr_is("enabled", false));
        e.find('input[name="maxSta"]').val(G.attr_get("max_sta", 30));
        e.find('input[name="enableServerDiscoverable"]').setchecked(k.attr_is("discoverable", true));
        e.find('input[name="enableLED"]').setchecked(k.attr_is("led_enabled", true));
        e.find('input[name="enableAutoUpgrade"]').setchecked(k.attr_is("auto_upgrade", false));
        e.check_radio("uplinkType",
            u.attr_get("uplink_type", "gateway"));
        e.find('input[name="uplink_host"]').val(u.attr("uplink_host"));
        e.find('input[name="enableConnectivityMonitor"]').setchecked(u.attr_is("enabled", true));
        e.find('input[name="enableIsolatedWLAN"]').setchecked(u.attr_is("enable_isolated_wlan", false));
        e.find('input[name="enableRemoteSyslog"]').setchecked(p.attr_is("enabled", false));
        e.find('input[name="syslogHost"]').val(p.attr("ip"));
        e.find('input[name="syslogPort"]').val(p.attr_get("port", 514));
        e.find('input[name="enableSmtp"]').setchecked(j.attr_is("enabled",
            false));
        e.find('input[name="smtpSender"]').setchecked(j.attr_is("use_sender", false));
        e.find('input[name="sender"]').val(j.attr("sender"));
        e.find('input[name="smtpHost"]').val(j.attr("host"));
        e.find('input[name="smtpSSL"]').setchecked(j.attr_is("use_ssl", false));
        e.find('input[name="smtpPort"]').val(j.attr_get("port", 25));
        e.find('input[name="smtpAuthentication"]').setchecked(j.attr_is("use_auth", false));
        e.find('input[name="userName"]').val(j.attr("username"));
        e.find('input[name="userPassword"]').jpass({value: j.attr("x_password")})
    });
    var q = false;
    e.find('input[name="apply"]').japply({get_posts: function () {
        return[
            {url: "/set_setting_identity.php", payload: {name: e.find('input[name="systemName"]').val()}, changed: function () {
                return e.find('input[name="systemName"]').val() != $("#SystemForm").data("identity").attr("name")
            }},
            {url: "/set_setting_inform.php", payload: {customized: e.find('input[name="useFixedURL"][value="true"]').checked(), host: e.find('input[name="fixedURLHost"]').val(), port: e.find('input[name="fixedURLPort"]').val()}, changed: function () {
                var j =
                    $("#SystemForm").data("inform");
                if (e.find('input[name="useFixedURL"][value="true"]').checked() != !j.attr_is("customized", false))return true;
                if (e.find('input[name="useFixedURL"][value="true"]').checked())return e.find('input[name="fixedURLHost"]').val() != j.attr_get("host", "undefined") || e.find('input[name="fixedURLPort"]').val() != j.attr_get("port", "undefined");
                return false
            }},
            {url: "/set_setting_mgmt.php", payload: {discoverable: e.find('input[name="enableServerDiscoverable"]').checked(), auto_upgrade: e.find('input[name="enableAutoUpgrade"]').checked(),
                led_enabled: e.find('input[name="enableLED"]').checked()}, changed: function () {
                return true
            }},
            {url: "/set_setting_connectivity.php", payload: {enabled: e.find('input[name="enableConnectivityMonitor"]').checked(), uplink_type: e.val_radio("uplinkType"), uplink_host: e.find('input[name="uplink_host"]').val(), enable_isolated_wlan: e.find('input[name="enableIsolatedWLAN"]').checked()}, changed: function () {
                return true
            }},
            {url: "/set_setting_bgscan.php", payload: {enabled: e.find('input[name="enableBackgroundScanning"]').checked()},
                changed: function () {
                    return true
                }},
            {url: "/set_setting_load_balance.php", payload: {enabled: e.find('input[name="enableLoadBalance"]').checked(), max_sta: e.find('input[name="maxSta"]').val()}, changed: function () {
                return true
            }},
            {url: "/set_setting_rsyslogd.php", payload: {enabled: e.find('input[name="enableRemoteSyslog"]').checked(), ip: e.find('input[name="syslogHost"]').val(), port: e.find('input[name="syslogPort"]').val()}},
            {url: "/set_setting_country.php", payload: {code: e.find('select[name="countryCode"]').val()}, changed: function () {
                return q =
                    e.find('select[name="countryCode"]').val() != e.data("country").attr_get("code")
            }},
            {url: "/set_setting_smtp.php", payload: {enabled: e.find('input[name="enableSmtp"]').checked(), host: e.find('input[name="smtpHost"]').val(), port: e.find('input[name="smtpPort"]').val(), use_ssl: e.find('input[name="smtpSSL"]').checked(), use_auth: e.find('input[name="smtpAuthentication"]').checked(), use_sender: e.find('input[name="smtpSender"]').checked(), sender: e.find('input[name="sender"]').val(), username: e.find('input[name="userName"]').val(),
                x_password: e.find("input[name=userPassword]").jpass("val")}}
        ]
    }, validate: function () {
        if (!V.checkEmpty(e.find('input[name="systemName"]'), Msg.SystemName))return false;
        if (e.find('input[name="useFixedURL"][value="true"]').checked() && (!V.checkEmpty(e.find('input[name="fixedURLHost"]'), Msg.ManagementHost) || !V.checkEmpty(e.find('input[name="fixedURLPort"]'), Msg.ManagementPort)))return false;
        if (e.find('input[name="enableConnectivityMonitor"]').checked() && e.val_radio("uplinkType") == "custom" && !V.checkIP(e.find('input[name="uplink_host"]'),
            Msg.UplinkIpAdress))return false;
        if (e.find('input[name="enableRemoteSyslog"]').checked()) {
            var j = e.find('input[name="syslogHost"]'), o = e.find('input[name="syslogPort"]');
            if (!V.checkEmpty(j, Msg.RemoteSyslogServerHost))return false;
            if (!V.checkEmpty(o, Msg.RemoteSyslogServerPort) || !V.checkMinMax(o, 1, 65535, Msg.RemoteSyslogServerPort))return false
        }
        if (e.find('input[name="enableSmtp"]').checked() && !c())return false;
        if (e.find('input[name="enableLoadBalance"]').checked())if (!V.checkMinMax(e.find('input[name="maxSta"]'), 1, 128, Msg.NumberofStations))return false;
        return true
    }, done: function () {
        if (q)location.reload(); else e.find('input[name="smtpAuthentication"]').checked() && e.find('input[name="userPassword"]').jpass("hide")
    }})
});
$(function () {
    var e = $("#SettingsUserGroups"), c = e.find(".settings-list"), q = $(".user-group");
    $("#wireless-add3").click(function () {
        j({name: "UserGroup", qos_rate_max: "-1#-1"})
    });
    $("#close5").click(function(){
        $(".user-group").css("display","none");
    }); 
    e.find("li.list a").click(function () {
        o()
    });
    var j = function (l) {
        var p = l._id == undefined ? true : false;
        if(!p)
        {
            $("#user-group-add-title").html(Msg.View);
        }
        else
        {
            $("#user-group-add-title").html(Msg.AddUser);
        }
        //e.find("li.form a").html(p ? Msg.NewUserGroup : l.name);
        //e.find("li.form").show();
        q.find('input[name="apply"]').val(p ? Msg.Create : Msg.ApplyStr);
        q.data("conf", l);
        q.find('input[name="usergroupname"]').val(l.name);
        if(l.qos_rate_max.split("#")[0] == "-1")
        {
            q.find('input[name="download"]').prop("checked",false);//setchecked("checked");
            q.find("#bandwidth-limit-text1").val("Unlimited");
        }
        else
        {
             q.find('input[name="download"]').prop("checked",true);
             q.find("#bandwidth-limit-text1").val(l.qos_rate_max.split("#")[0]);
        }
        if(l.qos_rate_max.split("#")[1] == "-1")
        {
            q.find('input[name="upload"]').prop("checked",false);
            q.find("#bandwidth-limit-text2").val("Unlimited");
        }
        else
        {
            q.find('input[name="upload"]').prop("checked",true);
            q.find("#bandwidth-limit-text2").val(l.qos_rate_max.split("#")[1]);
        }
       //var downRate = l.qos_rate_max).split("#")[0];
    
        //q.find('input[name="download"]').val(parseInt(downRate > -1 ? l.qos_rate_max_down : "");
       //  q.find('input[name="upRate"]').val(l.qos_rate_max_up > -1 ? l.qos_rate_max_up : "");
       // c.hide();
        q.css("display","block"); 
        q.css("margin-left","-230px");     
        q.css("top","60%");
        q.css("left","50%");
        q.draggable({handle: "p.add-page-title",containment: "#system-page-id"});
        //q.show();
        p && q.find('input[name="usergroupname"]').select().focus()
    }, o = function () {
        //e.find("li.form").hide();
        q.hide();
        c.show()
    };
    //by himgod
    $("#UserGroupsTableContainer").jtable({url: "/list_usergroup.php", objname: "usergroup", className: "stat"}).jtable("addcol","","",true).jtable("addcol", Msg.UserLimitPolicyName, "name", true).jtable("addcol",
        Msg.BandwidthLimit, "qos_rate_max", true, Renderer.qosRate).jtable("addaction", Msg.View, "edit").jtable("addaction", Msg.DeleteStr, "delete", function (l) {
            return!l.attr_no_delete
        }).bind("jtable_edit", function (l, p) {
            j(p)
        }).bind("jtable_delete", function (l, p) {
            if (p.attr_no_delete)alert(V.localize(Msg.E_InvalidDelete, p.name)); else confirm(Msg.C_DeleteObject) && $.jpost("/del_usergroup.php", {_id: p._id}, function () {
                $("#UserGroupsTableContainer").jtable("option", "model").jtablemodel("deleterow",
                    "_id", p._id).jtablemodel("firechanged");
                $("#model-usergroups").jlistmodel("deleterow", "_id", p._id)
            })
        }).bind("jtable_refresh-done", function () {
            $("#model-usergroups").jlistmodel("refresh")
        }).jtable("refresh");
    q.find('input[name="apply"]').japply({validate: function () {
        if (!V.checkEmpty(q.find('input[name="usergroupname"]'), Msg.NameStr))return false;
        // if (q.find('input[name="download"]').checked())if (!V.checkMinMax(q.find('#bandwidth-limit-text1'), 2, 102400, Msg.DownloadBandLimit) || !V.checkEmpty(q.find('#bandwidth-limit-text1'),
        //     Msg.DownloadBandwidth))return false;
        // if (q.find('input[name="upload"]').checked())if (!V.checkMinMax(q.find('#bandwidth-limit-text2'), 2, 102400, Msg.UploadBandLimit) || !V.checkEmpty(q.find('#bandwidth-limit-text2'), Msg.UploadBandwidth))return false;
        return true
    }, get_posts: function () {
        var l = q.data("conf")._id, p = q.find('input[name="download"]').checked(), u = q.find('input[name="upload"]').checked();
        var down_rate = p ? q.find('#bandwidth-limit-text1').val() : -1;
        var up_rate = u ? q.find('#bandwidth-limit-text2').val() : -1;
        var up_down_max = down_rate + "#" + up_rate;
        return{url: l ? "/upd_usergroup.php" : "/add_usergroup.php", payload: {_id: q.data("conf")._id, name: q.find('input[name="usergroupname"]').val(),
            downRate_enabled: q.find('input[name="download"]').checked(), upRate_enabled: q.find('input[name="upload"]').checked(),qos_rate_max: up_down_max}}
    }, done: function () {
        $("#UserGroupsTableContainer").jtable("refresh");
        o()
    }});
    // q.find('input[name="download"]').change(function () {
    //     q.find('input[name="downRate"]').setenabled($(this).checked())
    // });
    // q.find('input[name="upload"]').change(function () {
    //     q.find('input[name="upRate"]').setenabled($(this).checked())
    // });
    q.find('input[name="cancel"]').click(function () {
        o()
    })

    $('#bandwidth-limit-text1').val($('#bandwidth-limit-select1').children('option:selected').val());
    $('#bandwidth-limit-select1').change(function(){
        var value = $(this).children('option:selected').val();
        $('#bandwidth-limit-text1').val(value);
        /*do sth to save value*/
    });
    $('#bandwidth-limit-text1').focusout(function() {
        /* Act on the event */
    });

    $('#bandwidth-limit-text2').val($('#bandwidth-limit-select2').children('option:selected').val());
    $('#bandwidth-limit-select2').change(function(){
        var value = $(this).children('option:selected').val();
        $('#bandwidth-limit-text2').val(value);
        /*do sth to save value*/
    });
    $('#bandwidth-limit-text2').focusout(function() {
        /* Act on the event */
    });

});

function setlabelcontent(text)
{
    if(text["content"])
    {
        return "<span "+(text.color ? ("style='color:white;background-color:"+text.color+"'"):"")+">"+text.content+"</span>";
    }
    else
    {
        return "<span></span>";
    }
}

function getCountryCode(country_url)
{
    hashtable = new HashTable();
    hashtable.add(156,Msg.China);
    hashtable.add(840,Msg.UnitedStates);
    hashtable.add(392,Msg.Japan);
    hashtable.add(250,Msg.France);
    hashtable.add(724,Msg.Spain);
    var wlan = $(".wireless-network-setting"), code = wlan.find('#CountryCode');
    tmpdata = "";
    $.ajax({ 
            async: false, 
            type : "POST", 
            url : country_url,
            dataType : 'json', 
            success : function(data) { 
            if(data.meta.rc == "ok")
            {
                tmpdata = data; 
            }
            else
            {
                tmpdata = "";
            }
        } 
    });

    if(tmpdata != "")
    {
        var cur_code = tmpdata.cur_country_code;
        var selected = "";
        for(var icode = 0; icode < tmpdata.country_list.length;icode++)
        {
            if( cur_code == tmpdata.country_list[icode].country_code )
            {
                selected = "selected";
            }
            else
            {
                selected = "";
            }
            var tmpOption = "<option value='" + tmpdata.country_list[icode].country_code +"' " + selected + ">" + hashtable.getValue(tmpdata.country_list[icode].country_code) + "</option>";
            code.append(tmpOption);
        }       
    }
}
$(function () {

    var e = $("#WirelessNetworksForm"), c = e.find(".settings-list"),q = $(".wireless-network-add"),t = $("#security-drag");
	//fill select of country code
    getCountryCode("../../api/country_list.php");

    $("#wireless-add1").click(function () {
        j({name: "Name", security: "open", wpa_enc:"none"});
        $("#add-network-commit-button").removeAttr("disabled"); 
    });
    $("#close1").click(function(){
        $(".wireless-network-add").css("display","none");
        //$("#add-network-commit-button").attr("disabled",true); 
    });
    // e.find("li.list a").click(function () {
    //     o()
    // });
    q.find('input[name="passphrase"]').jpass();
    // q.find('input[name="wepKey"]').jpass();
    //q.find('input[name="radiusSecret1"]').jpass();
    q.find('select[name="userGroup"]').jselect({model: $("#model-usergroups")});
    var j = function (l) {
        var p = l._id == undefined ? true :false;
        q.css("display","block");
        q.css("margin-left","-232px");
        q.css("top","0%");
        q.css("left","50%");
        q.draggable({handle: "p.add-page-title",containment: "#sytem-page-id"});
        $("#add-network-commit-button").removeAttr("disabled");
        if(!p)
        {
           $("#create-auto").hide();
           $(".wlan-detail").show();
        }
        else
        {
            $("#create-auto").show();
            $(".wlan-detail").hide();
        }
        q.find('input[name="name"]').val(decodeURI(l.name));
        if(l.hide_ssid == true)
        {
            e.find("#w2").removeClass("off").addClass("on");
        }
        else
        {
            e.find("#w2").removeClass("on").addClass("off");
        }
        if(l.is_guest == true)
        {
            e.find("#w3").removeClass("off").addClass("on");
        }
        else
        {
            e.find("#w3").removeClass("on").addClass("off");
        }
        var i = 0;
        if(l.security == "open" && l.wpa_enc == "none")
        {
            i = 0;
        }
        else if(l.security == "wpapsk" && l.wpa_enc == "ccmp")
        {
             i = 1;
        }
        else if(l.security == "wpapsk" && l.wpa_enc == "tkip")
        {
             i = 2;
        }/*
        else if(l.security == "wpaeap" && l.wpa_enc == "ccmp")
        {
             i = 3;
        }
        else if(l.security == "wpaeap" && l.wpa_enc == "tkip")
        {
            i = 4;
        }*/
        else
        {   
            i = 0;
        }
        t.trigger("set_notch", i);
        // var x = 60;//((ev.clientX - t.ballX) < 0) ? 0 : ((ev.clientX - t.ballX) > t.width_all) ? t.width_all : (ev.clientX - t.ballX);
        t.width_all = (parseInt(t.css("width"))-7)||309;

        t.find(".dragging-ball").css("left", i*Math.round(t.width_all/5)+"px");
        t.find(".dragging-comment-se").html(setlabelcontent(t.data('textcontent')[i]));
        t.find(".dragging-blu").css("width", t.find(".dragging-ball").position().left);
        q.find(".down-link").html(l.downlink_bandwidth);
        q.find(".up-link").html(l.uplink_bandwidth);
         //e.find("li.form a").html(p ? Msg.NewWirelessNetwork : decodeURI(l.name));
        // e.find("li.form").show();
        q.find('input[name="apply"]').val(p ? Msg.Create : Msg.ApplyStr);
        q.data("conf", l);
        // q.find('input[name="name"]').val(decodeURI(l.name));
        // q.find('input[name="enabled"]').setchecked(l.enabled);
        // q.check_radio("securityType", l.security);
        q.find('input[name="passphrase"]').jpass("val", l.x_passphrase || "").jpass(p ? "show" : "hide");
        // q.find('select[name="wepIdx"]').val(l.wep_idx || 1);
        // q.find('input[name="wepKey"]').jpass("val", l.x_wep || "").jpass(p ? "show" :
        //     "hide");
        // q.find('input[name="radiusIp1"]').val(l.radius_ip_1);
        // q.find('input[name="radiusPort1"]').val(l.radius_port_1 || 1812);
        // q.find('input[name="radiusSecret1"]').jpass("val", l.x_radius_secret_1 || "").jpass(p ? "show" : "hide");
        // q.find('input[name="is_guest"]').setchecked(l.is_guest);
        // var u = l.vlan || 0;
        // q.find('input[name="vlanEnabled"]').setchecked(l.vlan_enabled);
        // q.find('input[name="vlan"]').val(u > 1 ? u : "");
        // q.find('input[name="hideSSID"]').setchecked(l.hide_ssid || false);
        // q.find("select[name=wpaMode]").val(l.wpa_mode ||
        //     "auto");
        // q.find("select[name=wpaEnc]").val(l.wpa_enc || "auto");
        // u = l.security == "wpapsk" || l.security == "wpaeap" ? true : false;
        // q.find(".WPA").setvisible(u);
        // q.find('select[name="userGroup"]').val(l.usergroup_id);
        //c.hide();
        //q.show();
        
        p && q.find('input[name="name"]').select().focus()
    }, o = function () {
        e.find("li.form").hide();
        q.hide();
        c.show()
    };
    //himgod write 
    //himgod update
    $("#WlanConfTableContainer").jtable({model: $("#model-wlanconfs"), hidetoolbar: true, className: "stat", pagesize:16}).jtable("addcol","","",true).jtable("addcol", Msg.WlanListSetting, "name", true).jtable("addcol", Msg.SignalStat, "security",
        true).jtable("addaction", Msg.View, "edit").jtable("addaction", Msg.DeleteStr, "delete").bind("jtable_edit", function (l, p) {
            j(p)
        }).bind("jtable_delete", function (l, p) {
            confirm(Msg.C_DeleteObject) && $.jpost("/del_wlanconf.php", {_id: p._id}, function () {
                $("#WlanConfTableContainer").jtable("option", "model").jtablemodel("deleterow", "_id", p._id).jtablemodel("firechanged")
            })
        }).bind("jtable_refresh-done", function (l, p) {
            c.find('input[name="create"]').setenabled(p._model.jtablemodel("rows").length <
                16)
        });
    /*q.find('input[name="securityType"]').change(function () {
        var l = q.val_radio("securityType");
        q.find(".security").hide();
        q.find(".security." + l).show();
        l = l == "wpapsk" || l == "wpaeap" ? true : false;
        q.find(".WPA").setvisible(l)
    });*/
    /*q.find('input[name="vlanEnabled"]').change(function () {
        q.find('input[name="vlan"]').setenabled($(this).checked())
    });*/
    q.find('input[name="apply"]').japply({validate: function () {
        var l = q.find('input[name="name"]');
        if (!V.checkEmpty(l, Msg.NameStr) || !V.checkSsid(l, Msg.NameStr))return false;
        /*l = q.val_radio("securityType");
        if (l == "wep") {
            l = q.find('input[name="wepKey"]');
            if (!V.checkEmpty(l, Msg.WEPKey) || !V.checkRE(l, Msg.WEPKey, V.reWEP, Msg.E_InvalidWEP))return false
        } else if (l == "wpapsk") {
            l = q.find('input[name="passphrase"]');
            if (!V.checkPassphrase(l, Msg.Passphrase))return false
        } else if (l == "wpaeap") {
            l = q.find('input[name="radiusIp1"]');
            var p = q.find('input[name="radiusPort1"]'), u = q.find('input[name="radiusSecret1"]');
            if (!V.checkEmpty(l, Msg.IPAddress) || !V.checkIP(l, Msg.IPAddress))return false;
            if (!V.checkEmpty(p, Msg.PortStr) || !V.checkMinMax(p,
                1, 65535, Msg.PortStr))return false;
            if (!V.checkEmpty(u, Msg.PasswordStr))return false
        }*/
        //if (q.find('input[name="vlanEnabled"]').checked())if (!V.checkMinMax(q.find('input[name="vlan"]'), 2, 4095, "VLAN ID"))return false;
        return true;
    }, get_posts: function () {
        var secu_val = $("#security-drag").data('security');
        return{url: (q.data("conf") && q.data("conf")._id )  ? "/upd_wlanconf.php" : "/add_wlanconf.php", payload: {
        	_id: (q.data("conf") && q.data("conf")._id ) ? q.data("conf")._id:0, 
        	name: q.find('input[name="name"]').val(), 
        	security: secu_val ? secu_val.security : "", 
        	x_passphrase: q.find('input[name="passphrase"]').jpass("val"), 
        	//wep_idx: q.find('select[name="wepIdx"]').val(),          
         // x_wep: q.find('input[name="wepKey"]').jpass("val"), 
          enabled: true,//q.find('input[name="enabled"]').checked(), 
          // radius_ip_1: q.find('input[name="radiusIp1"]').val(), 
          // radius_port_1: q.find('input[name="radiusPort1"]').val(), 
          // x_radius_secret_1: q.find('input[name="radiusSecret1"]').jpass("val"), 
          is_guest: q.find("#w3").hasClass("on") ? true : false, 
          //vlan_enabled: q.find('input[name="vlanEnabled"]').checked(), 
          //vlan: q.find('input[name="vlan"]').val(), 
          hide_ssid: q.find("#w2").hasClass("on") ? true : false, 
          //wpa_mode: q.find('select[name="wpaMode"]').val(),          
          wpa_enc: secu_val ? secu_val.enc_type : "", 
          usergroup_id: 0}}//q.find('select[name="userGroup"]').val()}}
    }, done: function () {
        $("#model-wlanconfs").jtablemodel("refresh");
        o()
    }});
    q.find('input[name="cancel"]').click(function () {
        o()
    })
});
//AccessPoint table 
$(function () {
    $("#AccessPointDataTable").jtable({model: $("#model-aps"), className: "", pagesize: $("#model-maintabpagesizer").jlistmodel("selected"), sortby: "state", sortdir: "-"})
		.jtable("addcol", "", "model", true, Renderer.apIcon)
		.jtable("addcol", Msg.Name_MACAddress, "mac", true, Renderer.apDetailLink)
		.jtable("addcol", Msg.IPAddress, "ip", true)
		.jtable("addcol", Msg.StatusName, "state", true, Renderer.apStatus)
		.jtable("addcol", Msg.NumClients, "num_sta", true)
		.jtable("addcol", Msg.Download, "tx_bytes", true, Renderer.pktsBytes)
		.jtable("addcol",Msg.Upload, "rx_bytes", true, Renderer.pktsBytes)
		.jtable("addcol", Msg.Channel, "channel_ng", true, Renderer.apChannel)
		.jtable("addaction", Msg.Adopt, "adopt", function (e) {return(e.state == 1) && e["default"]})
		.jtable("addaction", Msg.Restart, "restart", 
			function (e) {
				return $(e).attr("state") == "5"})
		.jtable("addaction", Msg.Locate, "locate", 
			function (e) {
				return /*e.state == 2 && e["default"] || */Renderer.apStatusType(e.state) == "ok"}, 
			function (e) {
            			return(e.locating ? "locating " : "") + "locate-button"})
		.jtable("addaction", Msg.APUpgrade, "upgrade", 
			function (e) {
            			return e.state == 5 && e.upgradable;}, function(e){return "upgrabled"}, function(a){return a.version+" -> "+a.upgradable;})
		.jtable("addaction", Msg.APForget, "forget", function(e){return e.state != 1;/*visiable*/})
			
		.bind("jtable_adopt", 
			function (e, c) {
            			$(e.originalTarget).disable();
            			$.jcmd("devmgr", "adopt", {mac: $(c).attr("mac"),model: $(c).attr("model")},
					function (q) {
                				q.meta.rc == "error" && alert(V.localizeMsg(ApiMsg[q.meta.msg], q.meta));
                				$("#model-aps").jtablemodel("refresh");
                				$("#alarm-table").jtable("refresh")})
        		})
        	.bind("jtable_restart", function (e, c) {
            $(e.originalTarget).disable();
            $.jcmd.restart_ap(c)
        }).bind("jtable_locate", function (e, c) {
            $.jcmd.locate_ap(c)
        }).bind("jtable_upgrade", function (e, c) {
            $(e.originalTarget).disable();
            $.jcmd.upgrade_ap(c);
        }).bind("jtable_forget", function(e, c){
        	$.jcmd.delete_ap(c);
		});
    $("#AccessPointDataTableSearch").jsearch().bind("jsearch_search", function () {
        var e = $("#AccessPointDataTableSearch").val();
        $("#AccessPointDataTable").jtable("addfilter", "*", e == "" ? null : e).jtable("filter")
    });
    $("#model-maintabpagesizer").bind("jlistmodel_select-changed", function () {
        $("#AccessPointDataTableContainer").jtable("option", "pagesize", $(this).jlistmodel("selected")).jtable("repaint")
    });
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        $("#AccessPointDataTable").jtable("repaint")
    })
});
$(function () {
    $("#BottomPanelTabs").bind("tabsshow", function (e, c) {
        if (c.panel.id == "Admin") {
            var q = $(document.body).data("sysinfo");
            q && $("#ServerVersion").html(q.version);
        }
        return true
    });
	
	$("#version_server").change(function(){
		refreshNewVerinfo($(this).val());
	});
	
    $("#UpgradeButton").click(function () {
        if (confirm(Msg.C_AFCUpdate)) {
            $("#UpgradeButton").disable();
            location.href = "/waiting.php?waiting_cmd=upgrade";
        }
    });
    $("#BackUp").click(function () {
        $("#BackUp").disable();
        $.jcmd("system", "backup", {}, function (e) {
            location.replace(e.data[0].url);
            $("#BackUp").enable()
        })
    });
    
    $("#BackupFile").jupload({path:$("#FilePathText"), btn:$("#BackupFileButton")}).bind("jupload_uploaded", function (e, c) {
        $("#BackupFile-confirm .confirm-text").html(V.localizeMsg(Msg.I_ConfirmRestore, c))
    });
    
    $('#BackupFile-confirm input[name="confirm"]').click(function () {
        location.href = "/waiting.php?waiting_cmd=restore";
    });
    $('#BackupFile-confirm input[name="cancel"]').click(function () {
        $("#BackupFile").jupload("cancel")
    });
    $("#DownloadSupportLink").click(function () {
        $("#DownloadSupportLink").disable();
        $.jcmd("system", "support", {browser_info: $.extend({userAgent: navigator.userAgent, language: navigator.language}, swfobject.ua)}, function () {
            location.replace("/dl/support");
            $("#DownloadSupportLink").enable()
        })
    })
});
$(function () {
    var e = $("#Alerts").find("ul.choose-shown"), c = function () {
        var q = {};
        if (e.jchoose("selected") == "unarchived")q.archived = false;
        $("#AlertsDataTableContainer").jtable("refresh", q)
    };
    $("#AlertPopUp").hide();
    $("#AlertsDataTableContainer").jtable({url: "/list_alarm.php", params: {_sort: "-time"}, className: "stat"}).jtable("addcol", Msg.DateTime, "time", true, Renderer.timestamp).jtable("addcol", Msg.Message, "msg", true, Renderer.eventMessage).jtable("addaction", Msg.Adopt, "adopt", function (q) {
        return(/*q.key == "EVT_AP_DiscoveredPending" ||
            q.key == "EVT_AP_RediscoveredPending"*/q.key == "AP_WAS_DISCOVERY_PENDING") && !q.archived
    }).jtable("addaction", Msg.Archive, "archive", function (q) {
        return!q.archived
    }).bind("jtable_adopt", function (q, j) {
        $.jcmd("devmgr", "adopt", {mac: $(j).attr("ap")}, function () {
            $("#model-aps").jtablemodel("refresh");
            c()
        })
    }).bind("jtable_archive", function (q, j) {
		var msg = $(j).attr("msg");
    	var matstr = msg.match(/AP\[(([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2})\]/);
		var apmacs = "";
		if(matstr && matstr.length > 1)
		{
			apmacs = matstr[1];
		}
		
        $.jcmd("evtmgr", "archive-alarm", {_id: $(j).attr("_id"), apmac: apmacs}, function () {
            c()
        })
    }).bind("jtable_refresh-done", function () {
        var q = 0;
        $.each($("#AlertsDataTableContainer").jtablemodel("rows"), function () {
            this.archived ==
            false && q++
        });
		if (q > 0)
		{
			$("#AlertsTab span").addClass("alert");
			q == 1 ? $("#AlertPopUp span").html(q + Msg.ItemRequireAttention) : $("#AlertPopUp span").html(q + Msg.ItemsRequireAttention);
			$("#AlertPopUp").show()
			$("#AlertsTab span.count").css("display", "inline-block").html(q)
		}
		else
		{
			$("#AlertsTab span").removeClass("alert");
			$("#AlertsTab span.count").hide();
			$("#AlertPopUp").hide()
		}
    }).jtable("refresh");
    e.jchoose().bind("jchoose_selected", c);
    $("#AlertsDataTableSearch").jsearch().bind("jsearch_search", function () {
        var q = $("#AlertsDataTableSearch").val();
        $("#AlertsDataTableContainer").jtable("addfilter", "*", q == "" ? null : q).jtable("filter")
    });
    $("#RefreshSelect").bind("jtimer_due", c);
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        $("#AlertsDataTableContainer").jtable("repaint")
    });
    $("#model-stas").bind("jtablemodel_refresh-done", function () {
        $("#AlertsDataTableContainer").jtable("repaint")
    });
    $("#Alerts").find('input[name="archive-all"]').click(function () {
        var q = $(this);
        q.disable();
        $.jcmd("evtmgr", "archive-all-alarms", {}, function () {
            c();
            q.enable()
        })
    })
});
$(function () {
    var e = $("#AllUsers"), c = e.find("ul.choose-guest-user"), q = e.find("ul.choose-offline"), j = e.find("select.time-filter"), o = function () {
        var l = c.jchoose("selected"), p = q.jchoose("selected") == "offline";
        p = {type: l, is_offline: p};
        if (l != "blocked")p.within = j.val();
        $("#AllUsersDataTableContainer").jtable("refresh", p)
    };
    $("#AllUsersDataTableContainer").jtable({url: "/stat_alluser.php", className: "stat", pagesize: $("#model-maintabpagesizer").jlistmodel("selected")}).jtable("addcol", Msg.Name_MACAddress, "mac", true,
        function (l, p) {
			return Renderer.staDetailLinkWithName(l, p.name || p.hostname || p.mac)
		}).jtable("addcol", Msg.UserGuest, "is_guest", true, function (l) {
			return l ? Msg.Guest : Msg.User;
		}).jtable("addcol", Msg.Device, "device", true, function (l) {
			return !l.blocked
		}).jtable("addcol", Msg.Download, "tx_bytes", true, Renderer.pktsBytes).jtable("addcol", Msg.Upload, "rx_bytes", true, Renderer.pktsBytes).jtable("addcol", Msg.LastSeen, "last_seen", true, Renderer.timestamp_sec).jtable("addaction", Msg.Block, "block", function (l) {
			return !l.blocked
		}).jtable("addaction", Msg.Unblock, "unblock", function (l) {
			return l.blocked
		}).bind("jtable_block",
        function (l, p) {
            $.jcmd.block_sta(p)
        }).bind("jtable_unblock", function (l, p) {
            $.jcmd.unblock_sta(p)
        }).bind("jcmd_stablocked", o).bind("jcmd_staunblocked", o);
    c.jchoose({selected: "all"}).bind("jchoose_selected", function () {
        var l = c.jchoose("selected");
        e.find("li.time-filter, li.offline-filter").setvisible(l != "blocked" && l != "noted");
        o()
    });
    q.jchoose({selected: "all"}).bind("jchoose_selected", o);
    j.change(o);
    $("#AllUsersDataTableSearch").jsearch().bind("jsearch_search", function () {
        var l = $("#AllUsersDataTableSearch").val();
        $("#AllUsersDataTableContainer").jtable("addfilter", "*", l == "" ? null : l).jtable("filter")
    });
    $("#model-maintabpagesizer").bind("jlistmodel_select-changed", function () {
        $("#AllUsersDataTableContainer").jtable("option", "pagesize", $(this).jlistmodel("selected")).jtable("repaint")
    });
    e.data("fn_shown", o)
});
$(function () {
    var e = $("#Guests"), c = e.find("#GuestsDataTableContainer"), q = e.find("ul.choose-na-ng");
    c.jtable({model: $("#model-stas"), className: "stat", pagesize: $("#model-maintabpagesizer").jlistmodel("selected")}).jtable("addcol", Msg.Name_MACAddress, "mac", true, Renderer.staDetailLink).jtable("addcol", Msg.StatusName, "authorized", true, function (j) {
        return j ? "Authorized" : "Pending"
    }).jtable("addcol", Msg.IPAddress, "ip", true).jtable("addcol", Msg.AccessPoint, "ap_mac", true, Renderer.apDetailLink).jtable("addcol", Msg.Signal,
        "rssi", true, Renderer.rssiWithRadio).jtable("addcol", Msg.Download, "tx_bytes", true, Renderer.pktsBytes).jtable("addcol", Msg.Upload, "rx_bytes", true, Renderer.pktsBytes).jtable("addcol", Msg.Activity, "tx_bytes.r", true, function (j, o) {
            return Renderer.activityBar(o["tx_bytes.r"] + o["rx_bytes.r"])
        }).jtable("addcol", Msg.Uptime, "uptime", true, Renderer.uptime).jtable("addaction", Msg.Block, "block", function () {
            return true
        }).jtable("addaction", Msg.Authorize, "authorize", function (j) {
            return!j.authorized
        }).jtable("addaction", Msg.Unauthorize, "unauthorize",
        function (j) {
            return j.authorized
        }).bind("jtable_block", function (j, o) {
            $.jcmd.block_sta(o)
        }).bind("jtable_kick", function (j, o) {
            $.jcmd.kick_sta(o)
        }).bind("jtable_unauthorize", function (j, o) {
            $.jcmd.unauthorize_guest(o)
        }).bind("jtable_authorize", function (j, o) {
            $.jcmd.authorize_guest(o)
        });
    c.jtable("addfilter", "is_guest", true);
    c.jtable("filter");
    $("#GuestsDataTableFilterByAP").jselect({model: $("#model-apfilters"), defaultItem: Msg.AllStr, renderer: function (j, o) {
		var l = Renderer.apName(o.mac), p = o["guest-num_sta"];
		if (p > 0)
			l = l + "(" + p + ")";
		return l
    }});
    $("#GuestsDataTableFilterByAP").change(function () {
        var j = $("#GuestsDataTableFilterByAP").val();
        j = $("#model-apfilters").jlistmodel("findrow", "_id", j);
        c.jtable("addfilter", "ap_mac", j ? j.mac : null).jtable("filter")
    });
    q.jchoose().bind("jchoose_selected", function () {
        var j = q.jchoose("selected");
        j == "all" ? c.jtable("delfilter", "radio") : c.jtable("addfilter", "radio", j);
        c.jtable("filter")
    });
    $("#GuestsDataTableSearch").jsearch().bind("jsearch_search", function () {
        var j = $("#GuestsDataTableSearch").val();
        c.jtable("addfilter", "*", j == "" ? null : j).jtable("filter")
    });
    $("#model-maintabpagesizer").bind("jlistmodel_select-changed", function () {
        c.jtable("option", "pagesize", $(this).jlistmodel("selected")).jtable("repaint")
    });
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        c.jtable("repaint")
    })
});

$(function(){
	$(".adm-ctrl-name").hover(function(e){
		$(this).css("font-size","120%");
		$(this).css("color","#ffffff");
	},function(){
		$(this).css("font-size","100%");
		$(this).css("color","#e0e0e0");
	});
}); 

function maplet_jscall(e, c) {
    $.log("jscall: " + e + "(", c, ")");
    var q = {};
    switch (e) {
        case "maplet_loaded":
            maplet_loaded();
            break;
        case "show_ap_detail":
            $(document.body).dialogManager("addItem", c.mac, "ap");
            break;
        case "ap_modified":
            var j = $("#model-aps").jtablemodel("findrow", "_id", c._id);
            if (!j)break;
            $.each(["mac", "map_id", "x", "y", "locked"], function () {
                q[this] = c[this];
                j[this] = c[this]
            });
            $.jpost("/upd_device.php", q, function () {
                $("#model-maps").jlistmodel("refresh")
            });
            break;
        case "ap_set_uplink":
            $.jcmd("devmgr", "set-vwire-uplink",
                {mac: c.downlinkMac, uplink: c.uplinkMac}, function () {
                });
            break;
        case "ap_unset_uplink":
            $.jcmd("devmgr", "unset-vwire-uplink", {mac: c.downlinkMac}, function () {
            });
            break;
        case "map_ordered":
            $(c).each(function (o, l) {
                $.jpost("/upd_map.php", {_id: l, order: o})
            });
            break;
        case "map_removed":
            $.jcmd("cfgmgr", "delete_map", {_id: c._id}, function () {
                $("#model-maps").jlistmodel("deleterow", "_id", c._id)
            });
            break;
        case "map_selected":
            $.jcmd("cfgmgr", "select_map", {_id: c._id}, function () {
            });
            break;
        case "map_modified":
            $.each(["description", "upp",
                "unit", "scale", "theta", "name", "_id"], function () {
                q[this] = c[this]
            });
            $.jpost("/upd_map.php", q);
            break;
        case "esc_pressed":
            closeBottomPanel();
            break;
        case "prompt_gmap":
            $(".gmap-dialog").data("fn_open")(c.name || "unknown");
            break;
        case "log":
            $.log(c);
            break;
        default:
            $.log("jscall " + e + "() not handled")
    }
}
var maplet;
function maplet_loaded() {
    maplet = $("#maplet").get(0);
    var e = [];
    $("#Map").data("ap_ids", e);
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        if ($("#Map").data("inited")) {
            var c = $.extend([], e);
            e.splice(0, e.length);
            $.each($("#model-aps").jtablemodel("rows"), function (q, j) {
                if (!j.adopted/* || j.state == 8*/)return true;
                e.push(j._id);
                var o = $.inArray(j._id, c);
                if (o >= 0) {
                    maplet.flexcall("updateAp", j);
                    c.splice(o, 1)
                } else {
                    maplet.flexcall("addAp", j);
                    e.push(j._id)
                }
            });
            $.each(c, function (q, j) {
                maplet.flexcall("deleteAp",
                    {_id: j})
            })
        }
    });
    $.jpost("/list_map.php", {_sort: "order"}, function (c) {
        maplet.flexcall("setUploadInfo", {host: location.hostname, port: parseInt($("#maplet-http-port").text()), key: "foryoureyesonly"});
        $.each(c.data, function (q, j) {//j.url="/temp/example.jpg";
            j.url ? maplet.flexcall("addMaps", [j]) : $.jcmd("cfgmgr", "save_gsmap", j, function (o) {
                j.url || maplet.flexcall("addMaps", [o.data[0].map])
            })
        });
        maplet.flexcall("deleteAllAps");
        $.each($("#model-aps").jtablemodel("rows"), function (q, j) {
            if (!j.adopted/* || j.state == 8*/)return true;
            maplet.flexcall("addAp", j);
            e.push(j._id)
        });
        $("#Map").data("inited", true);
        $.log("maplet_inited")
    })
}
/*
$(function () {
    swfobject.embedSWF("/library/swf/MapApplet.swf", "Maplet", "100%", "100%", "10.0.0", "library/js/flex/expressInstall.swf", {}, {wmode: "transparent", allowScriptAccess: "always"}, {id: "maplet"}, function (e) {
        e.success || $("#Maplet div.install-flash a").show()
    })
});*/

//User event table 
$(function () {
	var e = $("#UserEvent"), c = e.find("ul.choose-shown"), q = e.find("select"),
        j = function () {
            var o = c.jchoose("selected"), l = {};
            if (o == "all") l.user="all";
			if(o=="user") l.user="all";
			if(o=="admin") l.admin="all";
            l.within = q.val();
            $("#UserEventDataTable").jtable("refresh", l)
        };
    $("#UserEventDataTable").jtable({model: $("#model-uevents"), className: "stat", sortby: "time", sortdir: "-"}).jtable("addcol", "", "model", true, Renderer.apIcon).jtable("addcol", Msg.DateTime, "time", true, Renderer.timestamp).jtable("addcol", Msg.Message, "msg", true, Renderer.eventMessage).jtable("addaction", Msg.Adopt, "adopt", function (q) {
        return(/*q.key == "EVT_AP_DiscoveredPending" ||
            q.key == "EVT_AP_RediscoveredPending"*/q.key == "AP_WAS_DISCOVERY_PENDING") && q.alert
    }).jtable("addaction", Msg.Archive, "archive", function (q) {
        return q.alert&&!q.archived
    }).bind("jtable_adopt", function (q, j) {
        $.jcmd("devmgr", "adopt", {mac: $(j).attr("ap")}, function () {
            $("#model-aps").jtablemodel("refresh");
            j()
        })
    }).bind("jtable_archive", function (q, j) {
		var msg = $(j).attr("msg");
    	var matstr = msg.match(/AP\[(([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2})\]/);
		var apmacs = "";
		if(matstr && matstr.length > 1)
		{
			apmacs = matstr[1];
		}
		
        $.jcmd("evtmgr", "archive-alarm", {_id: $(j).attr("_id"), apmac: apmacs}, function () {
            j()
        })
    });
	 e.data("fn_shown", j);
    $("#UserEventDataTableSearch").jsearch().bind("jsearch_search", function () {
        var o = $("#UserEventDataTableSearch").val();
        $("#UserEventDataTable").jtable("addfilter", "*", o == "" ? null : o).jtable("filter")
    });
    
    $("#RefreshSelect").bind("jtimer_due", function () {
        if ($("#model-events").data("lastId")) {
            var o = c.jchoose("selected");
            $.jpost("/stat_event.php", {lastId: $("#model-uevents").data("lastId")}, function (l) {
                l.data.length > 0 && $("#model-events").jtablemodel("appendrows", l.data)
				&& $("#model-events").data("lastId", l.meta.lastId);
            })
        } else j()
    });
    c.jchoose({selected: "all"}).bind("jchoose_selected",
        j);
    q.change(j);
    $("#model-uevents").bind("jtablemodel_refresh-done", function () {
        $("#UserEventDataTable").jtable("repaint");
        //$("#model-events").jtablemodel("rows").length > 0 && $("#model-events").data("lastId", $("#model-events").jtablemodel("rows")[$("#model-events").jtablemodel("rows").length-1]._id);
		//alert($("#model-events").jtablemodel("rows")[0]._id);
    });
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        $("#UserEventDataTable").jtable("repaint")
    });
    $("#model-stas").bind("jtablemodel_refresh-done", function () {
        $("#UserEventDataTable").jtable("repaint")
    })
});

//AccessPoint event table 
$(function () {
	var e = $("#AccessPointEvent"), c = e.find("ul.choose-shown"), q = e.find("select"),
        j = function () {
            var o = c.jchoose("selected"), l = {};
            if (o != "all")l.type = o;
			l.within = q.val();
            $("#AccessPointEventDataTable").jtable("refresh", l);
			
        };
    $("#AccessPointEventDataTable").jtable({model: $("#model-events"), className: "stat", sortby: "time", sortdir: "-"}).jtable("addcol", "", "model", true, Renderer.apIcon).jtable("addcol", Msg.DateTime, "time", true, Renderer.timestamp).jtable("addcol", Msg.Message, "msg", true, Renderer.eventMessage).jtable("addaction", Msg.Adopt, "adopt", function (q) {
		
        return(/*q.key == "EVT_AP_DiscoveredPending" ||
            q.key == "EVT_AP_RediscoveredPending"*/q.key == "AP_WAS_DISCOVERY_PENDING") && q.alert
    }).jtable("addaction", Msg.Archive, "archive", function (q) {
        return q.alert&&!q.archived
    }).bind("jtable_adopt", function (q, j) {
        $.jcmd("devmgr", "adopt", {mac: $(j).attr("ap")}, function () {
            $("#model-aps").jtablemodel("refresh");
            j()
        })
    }).bind("jtable_archive", function (q, j) {
		var msg = $(j).attr("msg");
    	var matstr = msg.match(/AP\[(([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2})\]/);
		var apmacs = "";
		if(matstr && matstr.length > 1)
		{
			apmacs = matstr[1];
		}
		
        $.jcmd("evtmgr", "archive-alarm", {_id: $(j).attr("_id"), apmac: apmacs}, function () {
            j()
        })
    });
	 e.data("fn_shown", j);
    $("#AccessPointEventDataTableSearch").jsearch().bind("jsearch_search", function () {
        var o = $("#AccessPointEventDataTableSearch").val();
        $("#AccessPointEventDataTable").jtable("addfilter", "*", o == "" ? null : o).jtable("filter")
    });
    
    $("#RefreshSelect").bind("jtimer_due", function () {
        if ($("#model-events").data("lastId")) {
            var o = c.jchoose("selected");
            $.jpost("/stat_event.php", {lastId: $("#model-events").data("lastId"), type: o == "all" ? null : o}, function (l) {
                l.data.length > 0 && $("#model-events").jtablemodel("appendrows", l.data)
				&& $("#model-events").data("lastId", l.meta.lastId);
            })
        } else j()
    });
    c.jchoose({selected: "all"}).bind("jchoose_selected",
        j);
    q.change(j);
    $("#model-events").bind("jtablemodel_refresh-done", function () {
        $("#AccessPointEventDataTable").jtable("repaint");
        //$("#model-events").jtablemodel("rows").length > 0 && $("#model-events").data("lastId", $("#model-events").jtablemodel("rows")[$("#model-events").jtablemodel("rows").length-1]._id);
		//alert($("#model-events").jtablemodel("rows")[0]._id);
    });
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        $("#AccessPointEventDataTable").jtable("repaint")
    });
    $("#model-stas").bind("jtablemodel_refresh-done", function () {
        $("#AccessPointEventDataTable").jtable("repaint")
    })
});
$(function () {
    $("div.settings-panel").hide();
    $("#SettingsNavigation li").each(function () {
        $(this).click(function () {
            var e = $(this).find("a").attr("href");
            $(e).show();
            $("#SettingsNavigation").hide();
            return false
        })
    });
    $("#SettingsNavigation li").each(function () {
        $(this).click(function () {
            var e = $(this).find("a").attr("href");
            $(e).show();
            $("#SettingsNavigation").hide();
            return false
        })
    });
    $("ul.bread-crumbs li:first-child").each(function () {
        $(this).click(function () {
            $("#SettingsNavigation").show();
            $("div.settings-panel").hide();
            return false
        })
    });
    $("#SettingsTab").click(function () {
        $("#SettingsNavigation").show();
        $("div.settings-panel").hide();
        return false
    })
});
$(function () {
    var e = $("#Statistics"), c = function () {
        $.jpost("/stat_quicklook.php", {}, function (p) {
            var u = p.data[0].aps_current?p.data[0].aps_current[0]:null, k = $("div.quick-look dd.most-active-ap");
            if (u) {
                k.find(".name").html(Renderer.apDetailLink(u.ap));
                k.find(".sent").html(Renderer.pktsBytes(u.tx_bytes));
                k.find(".received").html(Renderer.pktsBytes(u.rx_bytes))
            } else k.find(".name").html("N/A");
            k.find("p:nth-child(1)").siblings().setvisible(u);
            u = p.data[0].stas_current ? p.data[0].stas_current[0]:null;
            k = $("div.quick-look dd.most-active-sta");
            if (u) {
                k.find(".name").html(Renderer.staDetailLink(u.mac));
                k.find(".sent").html(Renderer.pktsBytes(u.tx_bytes));
                k.find(".received").html(Renderer.pktsBytes(u.rx_bytes))
            } else k.find(".name").html("N/A");
            k.find("p:nth-child(1)").siblings().setvisible(u);
            u = p.data[0].users_life[0];
            k = $("div.quick-look dd.top-user");
            if (u) {
                k.find(".name").html(Renderer.staDetailLink(u.user));
                k.find(".sent").html(Renderer.pktsBytes(u.tx_bytes));
                k.find(".received").html(Renderer.pktsBytes(u.rx_bytes));
                k.find(".duration").html(Renderer.uptime(u.duration))
            } else k.find(".name").html("N/A");
            k.find("p:nth-child(1)").siblings().setvisible(u)
        })
    }, q = $("#usage-chart").jusagechart({url: "/stat_current.php", attrs: [
        {name: "bytes", label: Msg.Traffic},
        {name: "num_sta", label: Msg.NumofClients}
    ], by_attr: "ap", no_data_msg: Msg.WaitingforMoreData, legendRenderer: Renderer.apName, limit: 10, autoResize: true}),
    j = $("#wlan-chart").jusagechart({url: "/stat_current.php", attrs: [
        {name: "num_sta", label: Msg.NumofClients}
    ], legend_x: 220, by_attr: "wlan", no_data_msg: Msg.WaitingforData, limit: 10}), 
    o = $("#time-chart").jtimechart({autoResize: true});
    e.find("ul.choose-period").jchoose().bind("jchoose_selected", function (p, u) {
        var k = u == "daily";
        o.jtimechartmodel("option", "scale", k ? "daily" : "hourly");
        o.jtimechart("option", "timeRenderer", k ? Renderer.timeMonthDay : Renderer.timeHour24);
        o.jtimechart("refresh")
    });
    var l = $("#Statistics");
    l.data("fn_shown", function () {
        q.jusagechart("refresh");
        j.jusagechart("refresh");
        o.jtimechart("refresh");
        c()
    });
    var cc = l.data("fn_shown");
    cc&&cc();//alert("fn"); load the statistics page 
    $("#RefreshSelect").bind("jtimer_due", function () {
        if (!l.hasClass("ui-tabs-hide")) {
            q.jusagechart("refresh");
            j.jusagechart("refresh");
            o.jtimechart("refresh");
            c()
        }
    })
});


//System usertable
$(function () {
	var e = $("#System"), c = e.find("#UserDataTable"), q = e.find("ul.choose-na-ng");
	c.jtable({model: $("#model-stas"), className: "stat", pagesize: $("#model-maintabpagesizer").jlistmodel("selected"), sortby: "tx_bytes", sortdir: "-"})
	.jtable("addcol", "", "deviceType", true, Renderer.DeviceIcon)
	.jtable("addcol", Msg.Name_MACAddress, "mac", true, Renderer.staDetailLink)
	.jtable("addcol", Msg.IPAddress, "ip", true)
	.jtable("addcol", Msg.UWLAN, "essid", true)
	.jtable("addcol", Msg.AccessPoint, "ap_mac", true, Renderer.apDetailLink)
	.jtable("addcol", Msg.Signal, "rssi", true, Renderer.rssiWithRadio)
	.jtable("addcol", Msg.Download, "tx_bytes", true, Renderer.pktsBytes)
	.jtable("addcol", Msg.Upload, "rx_bytes", true, Renderer.pktsBytes)
	.jtable("addcol", Msg.Uptime, "uptime", true, Renderer.uptime)
	.jtable("addaction", Msg.Block, "block", function () {
			return true
		})
	.jtable("addaction", Msg.Reconnect, "kick", function () {
            return true
		})
	.bind("jtable_block", function (j, o) {
            $.jcmd.block_sta(o)
		})
	.bind("jtable_kick", function (j, o) {
            $.jcmd.kick_sta(o)
		});
	c.jtable("addfilter", "is_guest", false);
	c.jtable("filter");
    $("#UserDataTableFilterByAP").jselect({model: $("#model-apfilters"), defaultItem: Msg.AllStr, renderer: function (j, o) {
        var l = Renderer.apName(o.mac), p = o["user-num_sta"];
        if (p > 0)l = l + "(" + p + ")";
        return l
    }});
    $("#UserDataTableFilterByAP").change(function () {
        var j = $("#UserDataTableFilterByAP").val();
        j = $("#model-apfilters").jlistmodel("findrow", "_id", j);
        c.jtable("addfilter", "ap_mac", j ? j.mac : null).jtable("filter")
    });
    q.jchoose().bind("jchoose_selected", function () {
        var j = q.jchoose("selected");
        j == "all" ? c.jtable("delfilter",
            "radio") : c.jtable("addfilter", "radio", j);
        c.jtable("filter")
    });
    $("#UserDataTableSearch").jsearch().bind("jsearch_search", function () {
        var j = $("#UserDataTableSearch").val();
        c.jtable("addfilter", "*", j == "" ? null : j).jtable("filter")
    });
    $("#model-maintabpagesizer").bind("jlistmodel_select-changed", function () {
        c.jtable("option", "pagesize", $(this).jlistmodel("selected")).jtable("repaint")
    });
    $("#model-aps").bind("jtablemodel_refresh-done", function () {
        c.jtable("repaint")
    })
});
   
