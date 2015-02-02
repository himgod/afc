var LOGGING_ENABLED = true, TIMER_DEBUG = false;
var DEBUG_ENABLED = true;
var ALLOW_SUCCESS_MSG = true;
jQuery.support.xhrUpload = window.XMLHttpRequest && ((new window.XMLHttpRequest).upload ? true : false);
jQuery.extend(jQuery.expr[":"], {focus: function (b) {
    return b == document.activeElement
}, nofocus: function (b) {
    return b != document.activeElement
}, shown: function (b) {
    return!(jQuery(b).is(":hidden") || jQuery(b).parents(":hidden").length)
}});
(function (b) {
    b.ajaxSetup({contentType: "application/x-www-form-urlencoded; charset=UTF-8"});
    b.jpass_hint = "********";
    b.jpost = function (a, c, d) {
        b.post("/api" + a, {json: b.toJSON(c)}, function (f, g, h) {
			//if(a.match(/stat_sysinfo/)){alert(a+ " "+ b.toJSON(c));}//if(a.match(/add_wlan/))alert("a "+a+" " +b.toJSON(f));//alert(a + " c " + c+" f " + f);
			//f = eval('('+decodeURI(f)+')');
            b.jpost.fnReflectConnectionStatus && b.jpost.fnReflectConnectionStatus(h.status == 200);
            if (h.status != 200)return false;
            g = f.meta;
			if(g.dataDecode)
			{// decode chinese characters if need 
				f.data = eval('('+decodeURI(b.toJSON(f.data))+')');
			}
			if(g.metaDecode)
			{
				g = eval('('+decodeURI(b.toJSON(g))+')');
			}
            if (g.rc == "error") {
                if (g.msg == "api.err.LoginRequired") {
                    if (!b.jpost.session_expired) {
                        b.jpost.session_expired = true;
                        alert(V.localize(Msg.E_SessionExpired));
                        location.href = "/logout.php"
                    }
                } else if (g.msg !=
                    "api.err.ServerBusy")(f = ApiMsg[g.msg]) ? alert(V.localizeMsg(f, g)) : alert("~" + g.msg + "~");
		if(DEBUG_ENABLED && g.ret)
		{
			alert("DEBUG: return value: " + g.ret);
		}
                
                return false
            }
            else if(ALLOW_SUCCESS_MSG && g.msg)
            {
            	(msg = ApiMsg[g.msg]) ? alert(V.localizeMsg(msg, g)) : alert(g.msg);
            }
			if(DEBUG_ENABLED && g.ret)
			{
				alert("DEBUG: return value: " + g.ret);
			}
            d && d(f);
            return true
        }
        , 'json'
        );
    };
    b(document).ajaxError(function () {
        b.jpost.fnReflectConnectionStatus && b.jpost.fnReflectConnectionStatus(false)
    });
    b.jcmd = function (a, c, d, f) {
        d = d || {};
        d.cmd = c;
        b.jpost("/cmd/" + a +".php", d, f)
    };
    b.jcmd.restart_ap = function (a) {
        confirm(Msg.C_APReboot) && b.jcmd("devmgr", "restart", {_id: b(a).attr("_id"), mac: b(a).attr("mac")}, function () {
            b("#model-aps").jtablemodel("refresh");
            b("#model-stas").jtablemodel("refresh")
        })
    };
    b.jcmd.adopt_ap = function (a) {
        b.jcmd("devmgr", "adopt", {mac: b(a).attr("mac"), model: b(a).attr("model")}, function (c) {
            c.meta.rc == "error" && alert(V.localizeMsg(ApiMsg[c.meta.msg], c.meta));
            b("#model-aps").jtablemodel("refresh")
        })
    };
    b.jcmd.upgrade_ap = function (a) {
        b.jcmd("devmgr", "upgrade", {_id: b(a).attr("_id"), mac: b(a).attr("mac"), ip: b(a).attr("ip")}, function () {
            b("#model-aps").jtablemodel("refresh")
        })
    };
    b.jcmd.scan_ap = function (a) {
        b.jcmd("devmgr", "scan", {mac: b(a).attr("mac")}, function () {
            b("#model-aps").jtablemodel("refresh")
        })
    };
    b.jhighlight_ap = function (a) {
        b("#MainPanel").tabs("select",
            0);
        var c = b("#maplet").get(0);
        c && c.flexcall("highlightAp", {_id: b(a).attr("_id")})
    };
    b.jcmd.locate_ap = function (a) {
        if (a.adopted) {
            a.locating = !a.locating;
            b("#model-aps").jtablemodel("firechanged")
        } else a.locating = false;
        b.jcmd("devmgr", a.adopted ? a.locating ? "set-locate" : "unset-locate" : "locate", {mac: b(a).attr("mac"), switch_value:a.switch_value}, function () {
            a.state == 1 && a.locating && b.jhighlight_ap(a)
        })
    };
	b.jcmd.delete_ap = function (a) {
		confirm(Msg.C_ForgetAP) && b.jcmd("devmgr", "delete-ap", {_id: a._id, mac: a.mac}, function () {
                b("#model-aps").jtablemodel("refresh");
                //c.dispose()
            })
	};
    b.jcmd.block_sta = function (a) {
        var c = b(a).attr("mac");
        b.jcmd("stamgr", "block-sta", {mac: c}, function () {
            b("#model-stas").jtablemodel("deleterow",
                "mac", c);
            b.event.trigger("jcmd_stablocked")
        })
    };
    b.jcmd.unblock_sta = function (a) {
        a = b(a).attr("mac");
        b.jcmd("stamgr", "unblock-sta", {mac: a}, function () {
            b.event.trigger("jcmd_staunblocked")
        })
    };
    b.jcmd.kick_sta = function (a) {
        var c = b(a).attr("mac");
        b.jcmd("stamgr", "kick-sta", {mac: c}, function () {
            b("#model-stas").jtablemodel("deleterow", "mac", c)
        })
    };
    b.jcmd.unauthorize_guest = function (a) {
        var c = b(a).attr("mac");
        b.jcmd("stamgr", "unauthorize-guest", {mac: c}, function () {
            b("#model-stas").jtablemodel("deleterow", "mac", c)
        })
    };
    b.jcmd.authorize_guest = function (a) {
        var c = b(a).attr("mac");
        b.jcmd("stamgr", "authorize-guest", {mac: c}, function () {
            b("#model-stas").jtablemodel("findrow", "mac", c).authorized = true;
            b("#model-stas").jtablemodel("firechanged")
        })
    };
    b.log = function () {
        if (LOGGING_ENABLED)if (window.console != undefined)if (console.log.apply)console.log.apply(console, arguments); else {
            for (var a = "", c = 0; c < arguments.length; c++)a += " " + arguments[c];
            console.log(a)
        }
    };
    b.trace = function () {
        LOGGING_ENABLED && window.console && window.console.trace &&
        console.trace()
    };
    var e = {};
    b.jtstart = function (a) {
        if (TIMER_DEBUG) {
            var c = (new Date).getTime();
            a = "[" + a + "]";
            e[a] = c;
            b.log(a + " start");
            return a
        }
    };
    b.jtend = function (a) {
        if (TIMER_DEBUG) {
            var c = e[a], d = (new Date).getTime();
            if (c) {
                e[c] = null;
                b.log(a + " end (" + (d - c) + "ms)")
            }
        }
    };
    b.fn.disable = function () {
        return this.each(function () {
            b(this).attr("disabled", "disabled").addClass("disabled")
        })
    };
    b.fn.enable = function () {
        return this.each(function () {
            b(this).attr("disabled", "").removeClass("disabled")
        })
    };
    b.fn.setenabled = function (a) {
        return a ?
            b(this).enable() : b(this).disable()
    };
    b.fn.setvisible = function (a) {
        return a ? b(this).show() : b(this).hide()
    };
    b.fn.checked = function () {
        return b(this).is(":checked")
    };
    b.fn.check_radio = function (a, c) {
        return this.find('input[name="' + a + '"][value="' + c + '"]').attr("checked", "checked").change()
    };
    b.fn.val_radio = function (a) {
        return this.find('input[name="' + a + '"]:checked').val()
    };
    b.fn.setchecked = function (a) {
        return this.each(function () {
            b(this).attr("checked", a ? "checked" : "").change()
        })
    };
    b.fn.attr_get = function (a, c) {
        var d =
            b(this);
        if (d.length < 1)return c;
        d = d.attr(a);
        if (typeof d == "undefined")return c;
        return d
    };
    b.fn.attr_is = function (a, c) {
        var d = b(this).attr(a);
        if (typeof d == "boolean")return d;
        if (typeof d == "undefined")return c;
        return d == "true"
    };
    b.widget("ui.jtable", {options: {titleshown:true,pagesize: 8, sortby: "", sortdir: "+", sortbyserver: false, viewsize: 1500, viewstart: 0, selectable: false, model: null, emptydiv: null, debug: false, hidetoolbar: false, rows: null}, _setOption: function (a, c) {
        this.options[a] = c;
        a === "rows" && this._model_refreshed();
        return this
    },
        _init: function () {
            var a = this;
            if (this._inited) {
                b.log("ERROR! already a " + this.widgetName, a.element);
                b.trace()
            } else this._inited = true;
            if (this.options.rows)this.options.model = {bind: function () {
            }, jtablemodel: function (c) {
                if (c == "rows")return a.options.rows; else b.log("jtable.jtablemodel(" + c + ") not implemented")
            }};
            if (!this.options.model)this.options.model = b(this.element).jtablemodel(this.options);
            this._model = this.options.model;
            this._model.bind("jtablemodel_refresh-done", function () {
                a._model_refreshed()
            });
            this.table =
                b("<table></table>").addClass("data-table").appendTo(this.element);
            this.options.className && this.table.addClass(this.options.className);
            this.emptydiv = this.options.emptydiv || b("#" + this.element.attr("id") + "-none");
            this.emptydiv.hide();
            this.element.addClass("data-table-container");
            this._coldefs = [];
            this._actions = [];
            this._filters = {};
            this.page = 0;
            return a
        }, destroy: function () {
            if (this.element.data("jtable")) {
                this.element.removeClass("data-table-container").empty().unbind("jtable").removeData("jtable");
                return this
            }
        },
        addcol: function (a, c, d, f, g, h) {
            if (g == null)g = true;
            this._coldefs.push({display: a, name: c, sortable: d, renderer: f, shown: g, comparator: h})
        }, addaction: function (a, c, d, f, tt) {
            this._actions.push({display: a, key: c, shown: d, fnclasses: f, fntitle: tt})
        }, addfilter: function (a, c) {
            this._filters[a] = c
        }, delfilter: function (a) {
            delete this._filters[a]
        }, setsort: function (a, c) {
            this.options.sortby = a;
            this.options.sortdir = c ? "+" : "-"
        }, hidecol: function (a) {
            this._getcol(a).shown = false
        }, showcol: function (a) {
            this._getcol(a).shown = true
        }, selectall: function () {
            b(this.table).find(".sel,.allsel").attr("checked",
                "checked")
        }, selectnone: function () {
            b(this.table).find(".sel,.allsel").attr("checked", "")
        }, getselected: function () {
            return b(this.table).find(".sel:checked").map(function () {
                return b.data(this, "o")
            })
        }, setparams: function (a) {
            this.options.params = a
        }, _getcol: function (a) {
            for (var c = 0; c < this._coldefs.length; c++)if (this._coldefs[c].name == a)return this._coldefs[c];
            return null
        }, repaint: function () {
            this._paint()
        }, _paint: function () {
            var a = this;
            if (!this._shown) {
                this._filter();
                this._sort()
            }
            a.options.pagesize = parseInt(this.options.pagesize) ||
                10;
            this.numpages = Math.floor((this._shown.length + this.options.pagesize - 1) / this.options.pagesize);
            if (this._shown.length == 0) {
                this.element.hide();
                this.emptydiv.show()
            } else {
                this.emptydiv.hide();
                this.element.show();
                this.table.empty();
                var c = b("<thead></thead>"), d = b("<tr></tr>");
                this.page == 0 && d.addClass("first-row");
                if (this.options.selectable) {
                    var f = b('<input type="checkbox" class="allsel"/>');
                    f.click(function () {
                        a.table.find("input.allsel").attr("checked") ? b(a.table).find("input.sel").attr("checked", "checked") :
                            b(a.table).find("input.sel").attr("checked", "")
                    });
                    d.append(b("<th></th>").append(f))
                }
                b.each(a._coldefs, function () {
                    var o = this;
                    if (!o.shown)return true;
                    var j = b("<th></th>"), l = b("<span>" + o.display + "</span>");
                    j.append(l);
                    var p = a.options.sortby, r = a.options.sortdir == "+";
                    if (p && o.name == p)l.addClass(r ? "ascending" : "descending");
                    if (this.sortable) {
                        j.addClass("sortable");
                        j.click(function () {
                            if (o.name == p)r = !r; else {
                                p = o.name;
                                r = true
                            }
                            a.setsort(p, r);
                            a.sort()
                        })
                    }
                    d.append(j)
                });
                d.find("th:first-child").addClass("first-col");
                //himgod update
                if(a.element[0].id == "BlackListTableContainer1" || a.element[0].id == "BlackListTableContainer2")
                {
                    a._actions.length > 0 && d.append(b("<td></td>"));
                }
                else if(a.element[0].id == "UserGroupsTableContainer" || a.element[0].id == "WlanConfTableContainer")
                {
                     a._actions.length > 0 && d.append(b("<td>" + Msg.DetailSetting + "</td>"));
                }
                else
                {
                a._actions.length > 0 && d.append(b("<th>" + Msg.Actions + "</th>"));
                }
                   
                c.append(d);
               if(this.options.titleshown)  a.table.append(c); 
                var g = b("<tbody></tbody>"), h = true;
                if (a._shown.length < a.page * a.options.pagesize)a.page = 0;
                b.each(a._shown, function (o, j) {
                    if (o < a.page * a.options.pagesize)return true;
                    if (o >= (a.page + 1) * a.options.pagesize)return false;
                    var l = b('<tr class="' + (h ? "odd" : "even") + '"></tr>');
                    h = !h;
                    if (a.options.selectable) {
                        var p = b('<input type="checkbox" class="sel" />');
                        b.data(p.get(0), "o", j);
                        l.append(b("<td></td>").append(p))
                    }
                    b.each(a._coldefs,
                        function () {
                            if (!this.shown)return true;
                            var q = b(j).attr(this.name), s = b("<td></td>");
                            if(a.element[0].id == "WlanConfTableContainer" && this.name == "name")
                            {
                                this.renderer && this.renderer.className ? s.addClass(this.renderer.className) : s.addClass(this.name);
                                s.addClass("table-wireless-icon");
                            }
                            else
                            {
                                this.renderer && this.renderer.className ? s.addClass(this.renderer.className) : s.addClass(this.name);
                            }
                            q = this.renderer ? this.renderer(q, j) : a._defaultRenderer(q);
                            typeof q == "string" ? s.text(q/*decodeURI(q)*/) : s.append(q);// decodeURI for ÖÐÎÄSSID 
                            l.append(s)
                        });
                    if (a._actions.length > 0) {
                        var r = b('<td class="actions"></td>');
                        b.each(a._actions, function () {
                            var q = this;
                            if (q.shown ? q.shown(j) : true) {
                                if(a.element[0] != "")
                                {
                                    if(a.element[0].className.split(' ')[0] == "afi-black-list-table" || a.element[0].className.split(' ')[0] == "user-black-list-table")
                                    {
                                        var s = b("<div class='switch off'></div>").click(function(t){
                                            if(s.hasClass('on'))
                                            {
                                                s.addClass("off").removeClass("on");
                                            }
                                            else
                                            {
                                                s.addClass("on").removeClass("off");
                                            }
                                        });
                                    
                                    // q.fnclasses && s.addClass(q.fnclasses(j));
                                    // q.fntitle && s.attr("title", q.fntitle(j));
                                        s.appendTo(r)
                                    }
                                    else
                                    {
                                        var s = b('<button href="javascript:void(0);" class="button">' + q.display + "</button>").data("o",
                                            j).click(function (t) {
                                                a._trigger(q.key, t, j);
                                                return false
                                            });
                                        q.fnclasses && s.addClass(q.fnclasses(j));
        								q.fntitle && s.attr("title", q.fntitle(j));
                                        s.appendTo(r)
                                    }
                                }
                            }               
                        });
                        l.append(r)
                    }
                    g.append(l)
                });
                a.table.append(g);
                f = g.find("tr:last");
                c = f.find("td").size();
                if (this.page + 1 == this.numpages) {
                    f.addClass("last-row");
                    if (this.numpages > 1) {
                        f = this.options.pagesize - g.find("tr").size();
                        for (var k = 0; k < f; k++)g.append(b('<tr class="empty-row"><td colspan="' + c + '">&nbsp;</td></tr>'))
                    }
                }
                if (a.numpages > 0 && !a.options.hidetoolbar) {
                    if (!a.toolbar) {
                        a.toolbar = b('<div class="data-table-status"></div>').appendTo(a.element);
                        a.toolbar.append(b('<span class="data-table-paging-details"></span>'));
                        c = b('<div class="data-table-paging"></div>');
                        f = b('<a href="javascript:void(0);" class="back-button-disabled"></a>'); 
                        var m = b('<div class="handle-page-number"><span class="ui-corner-all " style="display:none">1</span></div>'), n = b('<div class="data-table-slider"></div>');
                        k = b('<a href="javascript:void(0);" class="next-button"></a>');
                        k.addClass(a.page == a.numpages - 1 ? "next-button-disabled" : "next-button");
                        k.click(function () {
                            a.page = Math.min(a.numpages - 1, a.page + 1);
							if(a.page>0)  {f.removeClass("back-button-disabled");f.addClass("back-button");}
							if(a.page== a.numpages - 1)  {k.removeClass("next-button");k.addClass("next-button-disabled");}
                            a._paint();
                            n.slider("value", a.page);
                            m.find("span").html(Math.round(a.page) + 1);
                            m.show().delay(1E3).fadeOut("slow")
                        });
						
                        //f.addClass(a.page > 0 ? "back-button-disabled" : "back-button");
                        f.click(function () {
                            a.page = Math.max(0, a.page - 1);
							if(a.page==0) {f.removeClass("back-button");f.addClass("back-button-disabled");}
							if(a.page== a.numpages - 1)  {k.removeClass("next-button-disabled");k.addClass("next-button");}
                            a._paint();
                            n.slider("value", a.page);
                            m.find("span").html(Math.round(a.page) + 1);
                            m.show().delay(1E3).fadeOut("slow")
                        });
                        n.slider({min: 0, max: a.numpages - 1, value: a.page, start: function (o, j) {
                            a.page = Math.round(j.value);
                            m.find("span").html(Math.round(a.page) + 1)
                        }, slide: function (o, j) {
                            n.find("div.handle-page-number span").html(Math.round(j.value) +
                                1)
                        }, change: function (o, j) {
                            a.page = Math.round(j.value);
                            a._paint()
                        }});
                        m.find("span").html(Math.round(a.page) + 1);
                        m.hide();
                        n.find(".ui-slider-handle").append(m).mousedown(function () {
                            m.show()
                        }).mouseup(function () {
                            m.hide()
                        });
                        c.append(f).append(n).append(k);
                        a.toolbar.append(c)
                    }
                    a.toolbar.find("span.data-table-paging-details").html(this.page * this.options.pagesize + 1 + " - " + Math.min((this.page + 1) * this.options.pagesize, this._shown.length) + " / " + this._shown.length);
                    a.toolbar.find("div.data-table-slider").slider("option",
                        "max", a.numpages - 1)
                }
                a.element.find(".data-table-paging").setvisible(a.numpages > 1)
            }
        }, sort: function () {
            if (this.options.sortby) {
                this.page = 0;
                if (this.options.sortbyserver)this.refresh(); else {
                    this._sort();
                    this._paint()
                }
            }
        }, _sort: function () {
            if (this.options.sortby) {
                var a = this.options.sortby, c = this.options.sortdir != "-", d = this._getcol(a).comparator || this._defaultComparator;
                if (a.match("channel")) {
                    var f = a.split("_");
                    a = f[1] + "-" + f[0]
                }
                this._shown = b(this._shown).sort(function (g, h) {
                    var k = g[a], m = h[a];
                    if (a == "mac") {
                        k =
                            (b(g).attr("name") || b(g).attr("hostname") || b(g).attr(a)).toLowerCase();
                        m = (b(h).attr("name") || b(h).attr("hostname") || b(h).attr(a)).toLowerCase()
                    } else if (a == "ap_mac") {
                        k = Renderer.apName(k || "");
                        m = Renderer.apName(m || "")
                    } else if (a == "ip") {
                        var n = function (j, l, p) {
                            p = p - j.length;
                            for (var r = 0; r < p; r++)j = l + j;
                            return j
                        }, o = function (j) {
                            try {
                                var l = j.match(/(\d+).(\d+).(\d+).(\d+)/);
                                return[n(l[1], "0", 3), n(l[2], "0", 3), n(l[3], "0", 3), n(l[4], "0", 3)].join(".")
                            } catch (p) {
                                return""
                            }
                        };
                        k = o(k);
                        m = o(m)
                    }
                    k = k == null ? 0 : k;
                    m = m == null ? 0 : m;
                    k = d(k,
                        m);
                    return(c ? 1 : -1) * k
                })
            }
        }, filter: function () {
            this.page = 0;
            if (this.options.sortbyserver)this.refresh(); else {
                if (this.toolbar) {
                    this.toolbar.empty().remove();
                    this.toolbar = null
                }
                this._filter();
                this._sort();
                this._paint()
            }
        }, _filter: function () {
            var a = this._model.jtablemodel("rows"), c = b.jtstart("filtering " + this.element.attr("id") + "(" + a.length + ")"), d = this;
            this._shown = [];
            this._hidden = [];
            b.each(a, function () {
                var f = this, g = true;
                for (var h in d._filters) {
                    var k = d._filters[h];
                    if (k != null)if (h == "*") {
                        var m = RegExp(k, "i"), n =
                            false;
                        b.each(d._coldefs, function () {
                            if (!this.shown)return true;
                            var j = b(f).attr(this.name);
                            j = this.renderer ? this.renderer(j, f) : d._defaultRenderer(j);
                            if (j == undefined)return true;
                            if (j instanceof jQuery) {
                                var l = [j.text()], p = j.attr("title");
                                p && l.push(p);
                                b.each(j.find('[title^=""]'), function (r, q) {
                                    l.push(q.title)
                                });
                                j = l.join(" ")
                            }
                            if (typeof j != "string")j = j.toString();
                            if (j.search(m) >= 0) {
                                n = true;
                                return false
                            }
                        });
                        if (!n) {
                            g = false;
                            break
                        }
                    } else if (b(f).attr(h) == null) {
                        g = false;
                        break
                    } else {
                        var o = false;
                        b.each(k instanceof Array ?
                            k : [k], function (j, l) {
                            var p = b(f).attr(h);
                            if (p == l || p.toString().search(RegExp(l, "i")) >= 0) {
                                o = true;
                                return false
                            }
                        });
                        if (!o) {
                            g = false;
                            break
                        }
                    }
                }
                g && d._shown.push(f)
            });
            b.jtend(c)
        }, refresh: function (a) {
            this._model.jtablemodel("refresh", a)
        }, _apply_options: function () {
            var a = this;
            if (this.options.rows)this.options.model = {bind: function () {
            }, jtablemodel: function (c) {
                if (c == "rows")return a.options.rows; else b.log("jtable.jtablemodel(" + c + ") not implemented")
            }};
            if (!this.options.model)this.options.model = b(this.element).jtablemodel(this.options);
            this._model = this.options.model;
            this._model.bind("jtablemodel_refresh-done", function () {
                a._model_refreshed()
            });
            this.options.className && this.table.addClass(this.options.className);
            this.emptydiv = this.options.emptydiv || b("#" + this.element.attr("id") + "-none");
            this.emptydiv.hide()
        }, refresh_options: function () {
            this._apply_options();
            this._model_refreshed()
        }, model: function () {
            return this._model
        }, _model_refreshed: function () {
            this._filter();
            this._sort();
            if (this.toolbar) {
                this.toolbar.empty().remove();
                this.toolbar =
                    null
            }
            this._paint();
            this._trigger("refresh-done", 0, this)
        }, _defaultRenderer: function (a) {
            return a
        }, _defaultComparator: function (a, c) {
            if (V.isNumber(a) && V.isNumber(c)) {
                a = parseFloat(a);
                c = parseFloat(c)
            }
            return a > c ? 1 : a < c ? -1 : 0
        }, widgetEventPrefix: "jtable_"});
    b.widget("ui.jtablemodel", {options: {url: null, objname: null, params: {}, autoRefresh: false}, _init: function () {
        this._rows = []
    }, rows: function () {
        return this._rows
    }, findrow: function (a, c) {
        var d = null;
        b.each(this._rows, function () {
            if (b(this).attr(a) == c) {
                d = this;
                return false
            }
        });
        return d
    }, deleterow: function (a, c) {
        var d = this;
        b.each(this._rows, function (f, g) {
            if (b(g).attr(a) == c) {
                d._rows.splice(f, 1);
                d._trigger("refresh-done", 0, d);
                return false
            }
        })
    }, appendrows: function (a) {
        this._rows.concat(a);
        this._trigger("refresh-done", 0, self)
    }, prependrows: function (a) {
        this._rows = a.concat(this._rows);
        this._trigger("refresh-done", 0, self)
    }, model: function () {
        return this
    }, firechanged: function () {
        this._trigger("refresh-done", 0, this)
    }, refresh: function (a) {
        var c = this;
        a = b.extend(b.extend({}, this.options.params),
            a);
        b.jpost(this.options.url, a, function (d) {
            c._rows = d.data;
            c._trigger("refresh-done", 0, c)
        })
    }, widgetEventPrefix: "jtablemodel_"});
    b.widget("ui.jusagechart", {options: {url: "/stat_current.php", attrs: [], legend_x: 0, by_attr: "ap", no_data_msg: Msg.WaitingforMoreData, legendRenderer: null, limit: 10, autoResize: false}, _init: function () {
        var a = this;
        a.options.autoResize && b(window).resize(function () {
            if (a._timer) {
                clearTimeout(a._timer);
                a._timer = null
            }
            a._timer = setTimeout(function () {
                if (a._paper) {
                    a._paper.clear();
                    a._paper.remove();
                    a._paper = null;
                    a.element.data("paper", null)
                }
                a.element.is(":shown") && a._paint()
            }, 200)
        })
    }, setAttrs: function (a, c) {
        this.options.attrs[c].name = a
    }, refresh: function () {
        var a = this;
        b.jpost(this.options.url, {o: this.options.by_attr, _sort: "-" + this.options.attrs[0].name, _limit: this.options.limit}, function (c) {
            a.series = [];
            a.labels = [];
            b.each(a.options.attrs, function (d, f) {
                var g = [];
				if(c.data)
				{
	                b.each(c.data, function (h, k) {
	                    if (d == 0)a.labels.push({name: (a.options.legendRenderer || Renderer.original)(k[a.options.by_attr])});
	                    k[f.name] &&
	                    g.push(k[f.name])
	                });
                	a.series.push(g)
				}
            });
            a._paint()
        })
    }, _hoverin: function () {
        var a = this.sector, c = a.paper.chart;
        if (!a.tag) {/*(this.value.value * 100 / this.total).toFixed(2) + "%\n"+*/
            a.tag = a.paper.g.label(this.mx, this.my, this.value.value+"/"+this.total).attr({opacity: 0});
            a.tag.insertBefore(this.cover)
        }
        a.tag.animate({opacity: 1}, 100);
        c.labels[this.value.order].icon.animate({scale: 1.5}, 100);
        c.labels[this.value.order].txt.attr({"font-weight": 800})
    }, _hoverout: function () {
        var a = this.sector.paper.chart;
        this.sector.tag.animate({opacity: 0}, 100);
        a.labels[this.value.order].icon.animate({scale: 1},
            100);
        a.labels[this.value.order].txt.attr({"font-weight": 400})
    }, _msgbox: function (a) {
        a = this._paper.text(this._paper.twidth, this._paper.theight, a).attr({font: "16px Arial, sans-serif"});
        var c = a.getBBox();
        this._paper.rect(c.x - 10, c.y - 10, c.width + 20, c.height + 20, 5).attr({"stroke-width": 5, stroke: "#555", fill: "#eee"});
        a.toFront();
    }, _paint: function () {
        if (!this._paper) {
            this._paper = Raphael(this.element.get(0), this.element.width(), this.element.height());
            this._paper.chart = this
        }
        this._paper.clear();
        if (this.labels.length ==
            0){this._msgbox(this.options.no_data_msg);}else {
            var a = 0, c = true;
            b.each(this.series, function (j, l) {
                a += l.length;
                if (l.length == 0)c = false
            });
            if (a == 0 || !c)this._msgbox(this.options.no_data_msg); else {
                var d = this.element.height(), f = this.options.legend_x || this.element.width() - 150, g = this.options.attrs.length;
                this.pies = [];
                for (var h = 0; h < g; h++) {
                    var k = f - f / g * (h + 1) + 10, m = k + 50 + d / 2 - 10, n = d / 2, o = d / 2 * 0.8;
                    this._paper.text(k, 10, this.options.attrs[h].label).attr({font: "16px Arial, sans-serif", "text-anchor": "start"});
                    this.pies[h] =
                        this._paper.g.piechart(m, n, o, this.series[h].slice(0));
                    this.pies[h].hover(this._hoverin, this._hoverout)
                }
                d = this._paper.set();
                for (h = 0; h < this.labels.length; h++)if (this.pies[0].series[h]) {
                    g = this.labels[h].color = this.pies[0].series[h].attr("fill") || "#ccc";
                    g = this.labels[h].icon = this._paper.g.square(f + 5, 10 + h * 20, 5).attr({fill: g, stroke: "none"});
                    k = this.labels[h].txt = this._paper.g.text(f + 20, 10 + h * 20, this.labels[h].name).attr({"text-anchor": "start"});
                    d.push(g);
                    d.push(k)
                }
            }
        }
    }});
    b.widget("ui.jtimechart", {options: {model: null,
        timeRenderer: null, tooltip_width: 60, autoResize: false}, _init: function () {
        var a = this;
        if (!this.options.model)this.options.model = this.element.jtimechartmodel(this.options);
        this._model = this.options.model;
        this._model.bind("jtimechartmodel_refresh-done", function () {
            a._model_refreshed()
        });
        a = this;
        a.options.autoResize && b(window).resize(function () {
            if (a._timer) {
                clearTimeout(a._timer);
                a._timer = null
            }
            a._timer = setTimeout(function () {
                if (a._paper) {
                    a._paper.clear();
                    a._paper.remove();
                    a._paper = null;
                    a.element.data("paper",
                        null)
                }
                a.element.is(":shown") && a._paint()
            }, 200)
        })
    }, setScope: function (a) {
        this._model.jtimechartmodel("setScope", a)
    }, refresh: function () {
        this._model.jtimechartmodel("refresh")
    }, _model_refreshed: function () {
        this._paint()
    }, _paintGrid: function (a, c, d, f, g, h, k, m) {
        g = g || "#000";
        var n = [], o = f / 3, j = h.length - 1, l = d / j;
        n = n.concat(["M", a, c + o, "L", a + d, c + o]);
        for (d = 1; d < j; d++)if (h[d])n = n.concat(["M", a + d * l, c, "L", a + d * l, c + f]);
        this._paper.path(n.join(",")).attr({stroke: g});
        this._paper.text(a + 2, c + o - 5, m).attr({"text-anchor": "start",
            fill: g});
        for (d = 0; d < j; d++)h[d] && this._paper.text(a + d * l + 2, c + f - 7, h[d]).attr({"text-anchor": "start", fill: k})
    }, _paintSeries: function (a, c, d, f, g, h, k, m) {
        var n = this, o = this._paper.path().attr({stroke: g, "stroke-width": 4, "stroke-linejoin": "round"}), j = this._paper.path().attr({stroke: "none", opacity: 0.3, fill: g}).moveTo(a, c + f), l = d / (h.length - 1), p = k == 0 ? f / 10 : f / 1.5 / k / 1;
        b.each(h, function (r, q) {
            var s = a + l * r, t = c + f - p * q;
            j[r == 0 ? "lineTo" : "cplineTo"](s, t, 10);
            o[r == 0 ? "moveTo" : "cplineTo"](s, t, 10);
            if (q) {
                var u = n._paper.circle(s, t,
                    5).attr({fill: g, stroke: "#fff"}), v = n._paper.rect(s - l / 2, c, s + l / 2, c + f).attr({stroke: "none", fill: "#fff", opacity: 0});
                b(v.node).hover(function () {
                    var w = m ? m(q) : q;
                    n._showTooltip(s + 8, t - 33, w, u)
                }, function () {
                    n._hideTooltip(u)
                })
            }
        });
        j.lineTo(a + d, c + f).andClose()
    }, _paint: function () {
        var a = this;
        if (!a._paper) {
            a._paper = Raphael(a.element.get(0), a.element.width(), a.element.height());
            a.element.data("paper", a._paper)
        }
        a._paper.clear();
        var c = [
            [],
            []
        ], d = [0, 0], f = [], g = this._model.jtimechartmodel("data");
        b.each(g, function (n, o) {
            var j =
                o[0], l = o[1], p = l.num_sta || 0;
            c[0].push(p);
            d[0] = Math.max(d[0], p);
            l = l.bytes || 0;
            c[1].push(l);
            d[1] = Math.max(d[1], l);
            f.push(n % 2 == 1 ? (a.options.timeRenderer || Renderer.timeHour24)(j) : null)
        });
        g = 3;
        var h = a.element.width() - 6, k = a.element.height() / 2 - 6, m = "hsb(" + [0.6, 1, 0.75] + ")";
        a._paintGrid(3, g, h, k, "#eee", f, m, d[0]);
        a._paintSeries(3, g, h, k, m, c[0], d[0]);
        a._paper.text(5, g + 8, Msg.NumofClients).attr({font: "16px Arial, sans-serif", "text-anchor": "start"});
        g += a.element.height() / 2;
        a._paintGrid(3, g, h, k, "#eee", f, m, Renderer.pktsBytes(d[1]));
        a._paintSeries(3, g, h, k, m, c[1], d[1], Renderer.pktsBytes);
        a._paper.text(5, g + 5, Msg.Traffic).attr({font: "16px Arial, sans-serif", "text-anchor": "start"});
        a._tooltip_box = a._paper.rect(10, 10, a.options.tooltip_width, 25, 5).attr({fill: "#000", stroke: "#474747", "stroke-width": 2}).hide();
        a._tooltip_label = a._paper.text(a.options.tooltip_width / 2, 10, "15").attr({font: "12px Fontin-Sans, Arial", fill: "#fff"}).hide()
    }, _showTooltip: function (a, c, d, f) {
        if (a + this.options.tooltip_width > this.element.width())a = a - this.options.tooltip_width -
            10;
        clearTimeout(this._hidetooltip_timer);
        this._tooltip_box.show().animate({x: a, y: c}, 200 * self._tooltip_visible);
        this._tooltip_label.attr({text: d}).show().animateWith(this._tooltip_box, {x: a + this.options.tooltip_width / 2, y: c + 12}, 200 * self._tooltip_visible);
        f.attr("r", 7);
        self._tooltip_visible = true
    }, _hideTooltip: function (a) {
        var c = this;
        a.attr("r", 5);
        this._hidetooltip_timer = setTimeout(function () {
            c._tooltip_box.hide();
            c._tooltip_label.hide();
            c._tooltip_visible = false
        }, 1)
    }});
    b.widget("ui.jtimechartmodel", {tzoffsetMilli: (new Date).getTimezoneOffset() *
        60 * 1E3, hourMilli: 36E5, dayMilli: 864E5, options: {scope: "system", scale: "hourly", end_utc: null, attrs: ["bytes", "num_sta", "time"]}, _init: function () {
    }, setend: function (a) {
        this.options.scale == "daily" && a.setUTCHours(0);
        a.setUTCMinutes(0);
        a.setUTCSeconds(0);
        a.setUTCMilliseconds(0);
        this.end_utc = a.getTime();
        this.dt = this.options.scale == "hourly" ? this.hourMilli : this.dayMilli;
        this.n = this.options.scale == "hourly" ? 24 : 30;
        this.start_utc = this.end_utc - this.n * this.dt
    }, setScope: function (a) {
        this.options.scope = a == null ? "system" :
            "map." + a
    }, refresh: function () {
        var a = this;
        this.setend(this.options.end_utc ? new Date(this.options.end_utc) : new Date);
        b.jpost("/stat_report_"+this.options.scale+".php", {attrs: a.options.attrs, start: a.start_utc, end: a.end_utc}, function (c) {
        	//url: /stat/report/  + this.options.scale + "." + this.options.scope
            cdebug = c;
            a.response = c;
            a._data = [];
            var d = a.end_utc;
            for (b.each(c.data, function (f, g) {
                for (; g.time < d;) {
                    a._data.unshift([d, {}]);
                    d -= a.dt
                }
                g.time != d && b.log("error", d, g.time);
                a._data.unshift([d, g]);
                d -= a.dt
            }); a.start_utc <= d;) {
                a._data.unshift([d, {}]);
                d -= a.dt
            }
            a._trigger("refresh-done",
                0, a)
        })
    }, data: function () {
        return this._data
    }, widgetEventPrefix: "jtimechartmodel_"});
    b.widget("ui.jtimer", {options: {interval: 120}, _init: function () {
        var a = this;
        this.element.val(this.options.interval);
        this.element.change(function () {
		a.start();
		a.trigger();
		a._trigger("cache-select", 0, a);
        })
    }, start: function () {
        var a = this, c = a.element.val();
        a.stop();
        if (c != 0)a.timer = setInterval(function () {
            a.trigger()
        }, c * 1E3)
    }, stop: function () {
        this.timer && clearInterval(this.timer);
        this.timer = 0
    }, trigger: function () {
        this._trigger("due", 0, this)
    }, val: function (a) {
        if (a == undefined)return this.element.val();
        this.element.val(a);
        return a
    }, widgetEventPrefix: "jtimer_"});
    b.widget("ui.jpass", {options: {hint: b.jpass_hint, shown: false}, _init: function () {
        var a = this;
        this.value = this.options.value || "";
        this.shown = this.options.shown;
        this.element.focus(function () {
            a.element.val(a.value);
            a.shown = true
        });
        this.element.blur(function () {
            a.save();
            a.shown = false;
            a.element.val(a.options.hint)
        });
        this.element.mousedown(function () {
            a.element.val(a.value);
            a.shown = true
        });
        this.element.val(this.shown ? this.value : this.options.hint)
    }, hide: function () {
        this.shown =
            false;
        this.element.val(this.options.hint)
    }, show: function () {
        this.show = true;
        this.element.val(this.value)
    }, save: function () {
        var a = this.element.val();
        if (a != this.options.hint && a != this.value)this.value = a
    }, val: function (a) {
        if (a == undefined)return this.shown ? this.element.val() : this.value; else {
            this.value = a;
            this.shown && this.element.val(this.value)
        }
    }});
    b.widget("ui.jlistmodel", {options: {url: null, params: {}, tableModel: null, tableModelRowConverter: null, rows: null, selected: "*", autoRefresh: false}, _init: function () {
        var a =
            this;
        this._rows = [];
        if (this.options.tableModel)this.options.tableModel.bind("jtablemodel_refresh-done", function () {
            a._rows = [];
            b.each(a.options.tableModel.jtablemodel("rows"), function (c, d) {
                var f = a.options.tableModelRowConverter ? a.options.tableModelRowConverter(d) : d;
                f && a._rows.push(f)
            });
            a._trigger("refresh-done", 0, a)
        }); else if (this.options.rows)a._rows = this.options.rows
    }, rows: function () {
        return this._rows
    }, selected: function () {
        return this.options.selected;
    }, setselected: function (a) {
        this.options.selected = a;
        this._trigger("select-changed", 0, this);
    }, findrow: function (a, c) {
        var d = null;
        b.each(this._rows, function () {
            if (b(this).attr(a) == c) {
                d = this;
                return false
            }
        });
        return d
    }, deleterow: function (a, c) {
        var d = this;
        b.each(this._rows, function (f, g) {
            if (b(g).attr(a) == c) {
                d._rows.splice(f, 1);
                d._trigger("refresh-done", 0, d);
                return false
            }
        })
    }, addrow: function (a) {
        if (this.options.tableModel)alert("jlistmodel.addrow for tableModel not implemented"); else {
            this._rows.push(a);
            this._trigger("refresh-done", 0, this)
        }
    }, refresh: function (a) {
        var c = this;
	if (this.options.tableModel)
	{
		this.options.tableModel.jtablemodel("refresh");
	}
	else if (c.options.rows)
	{
		c._trigger("refresh-done", 0, c); 
	}
	else {
            a = b.extend(this.options.params, a);
            b.jpost(this.options.url, a, function (d) {
                c._rows = d.data; 
                c._trigger("refresh-done", 0, c)
            })
        }
    }, widgetEventPrefix: "jlistmodel_"});
    b.widget("ui.jselect", {options: {model: null, renderer: null, defaultItem: null, selectionSynchronized: false}, _init: function () {
        var a = this;
        this._model = this.options.model;
        this._model.bind("jlistmodel_refresh-done", function () {
            a._paint();
        });
        if (a.options.selectionSynchronized) {
            this._model.bind("jlistmodel_select-changed", function () {
                a.element.val() != a._model.jlistmodel("option", "selected") && a.element.val(a._model.jlistmodel("option", "selected"))
            });
            this.element.change(function () {
                a._model.jlistmodel("setselected", a.element.val())
            })
        }
    }, _paint: function () {
        var a = this, c = a.element.val();
        this.element.empty();
        a.options.defaultItem && b('<option value="">' + a.options.defaultItem + "</option>").appendTo(a.element);
        b.each(a._model.jlistmodel("rows"), function (d, f) {
            if (f.attr_hidden)return true;
            var g = a.options.renderer ? a.options.renderer(f.name, f) : f.name;
            if (g == null)return true;
            b('<option value="' + f._id + '">' + g + "</option>").appendTo(a.element)
        });
        if (a.options.selectionSynchronized)
	{
		this.element.val(a._model.jlistmodel("option", "selected")); 
	}
	else {
            if (c != null && a.element.find("option[value='" + c + "']").length == 0)c = null;
            if (c == null && a.element.find("option").length > 0)a.options.defaultItem != null ? a.element.val("") : a.element.val(a.element.find("option:first").val());
            else a.element.val(c);
			
        }
    }, repaint: function () {
        this._paint()
    }});
    b.widget("ui.jsearch", {options: {delay: 300, autocomplete: false}, _init: function () {
        var a = this;
        this.element[0].form && b(this.element[0].form).submit(function () {
            return false
        });
        this.element.click(function () {
            a.element.select()
        });
        this.element.keydown(function (c) {
            if (c.ctrlKey || c.metaKey || c.altKey)return true;
            if (a._timer) {
                clearTimeout(a._timer);
                a._timer = null
            }
            a._timer = setTimeout(function () {
                a._trigger("search", 0, a)
            }, a.options.delay)
        })
    }, widgetEventPrefix: "jsearch_"});
    b.widget("ui.japply", {options: {submitbyenter: true}, _init: function () {
        var a = this;
        this.subdone = false;
        this.element.click(function () {
            a.submit();
            return false
        });
        b(this.element.attr("form")).submit(function () {
            a.options.submitbyenter && a.submit();
            return false
        })
    }, submit: function () {
        this.element.disable();
        this.subdone = false;
        if (this.options.validate && !this.options.validate())this.element.enable(); else {
            if (this.options.get_posts) {
                this.posts = this.options.get_posts();
                if (!b.isArray(this.posts))this.posts = [this.posts];
            } else this.posts = [];
            this.nextapply()
        }
        return false
    },
        nextapply: function (a) {
            for (var c = this; this.posts.length > 0;) {
                var d = this.posts.shift();
                if (!(d.changed && !d.changed())) {
                    b.jpost(d.url, d.payload, function (f) {
                        c.nextapply(f)
                    });
                    return true;
                }
            }
            c.done(a)
            return false;
        }, done: function (a) {
            var c = this;
            setTimeout(function () {
                c.element.enable()
            }, 500);
            typeof c.options.done == "function" && c.options.done(a)
        }});
    b.widget("ui.jchoose", {option: {}, _create: function () {
        var a = this;
        if (!this.options.selected)this.options.selected = (a.element.find("a").attr("href") || "#").substring(1);
        a.element.find("li").click(function (c) {
            c =
                b(c.currentTarget);
            if (!c.hasClass("ui-tabs-selected")) {
                a.element.find("li.ui-tabs-selected").removeClass("ui-tabs-selected");
                c.addClass("ui-tabs-selected");
                c = c.find("a").attr("href") || "#";
                a.options.selected = c.substring(1);
                a._trigger("selected", 0, a.options.selected);
                return false
            }
        })
    }, selected: function () {
        return this.options.selected
    }, widgetEventPrefix: "jchoose_"});
    b.widget("ui.jupload", {options: {autostart: true, url: "/upload_backup.php"}, _create: function () {
        var a = this;
        a._id = this.element.attr("id");
        //a.element.hide();
        this.element.val("");
        this._progress = b('<div class="jupload-progress ui-corner-all"></div>').hide().insertAfter(this.element);
        this._filename = b('<span class="filename"></span>').appendTo(this._progress);
        this._bar = b('<div class="bar"></div>').progressbar().appendTo(this._progress);
        this._cancel = b('<a class="cancel-button button"></a>').html("Cancel").appendTo(this._progress).click(function () {
            a.cancel()
        });
        this._confirm = b("#" + a._id + "-confirm");
        this._confirm.appendTo(a._progress);
        this._tryagain = b('<div class="error-restore button"></div>').hide().appendTo(this._progress);
        this._error = b('<div class="error-text button"></div>').appendTo(this._tryagain);
        b('<a class="cancel-button button"></a>').html("Try Again").appendTo(this._tryagain).click(function () {
            a.cancel()
        });
        if (!jQuery.support.xhrUpload) {
            a._form = b('<form method="post" enctype="multipart/form-data"></form>').attr("id", a._id + "form").attr("target", a._id + "IF").attr("action", a.options.url).insertBefore(this.element);
            a.element.appendTo(this._form);
            a._iframe = b('<iframe src="/iframe.html" class="jupload-IF"></iframe>').attr("id",
                    a._id + "IF").attr("name", a._id + "IF").appendTo(this._form);
            b('<input type="hidden" name="id" value="' + a._id + '"/>').appendTo(this._form);
            a._form.height(1)
        }
        this.element.change(function () {
        		a.options.path.val(a.element.val());
            a.options.autostart && a.element.val() != "" && a.upload()
        });
        var obtn = this.options.btn;
        obtn.click(function () {
			//a.element.click();a.element.change();
		});
        b(".cancel button", this._progress).html("Cancel")
    }, destroy: function () {
    }, upload: function () {
        var a = this;
        a.element.hide();
        a.options.path.hide();
        a.options.btn.hide();
        a._progress.show();
        a._tryagain.hide();
        a._confirm.hide();
        a._filename.addClass("busy");
        a._bar.show().progressbar("value", 0);
        a._cancel.show();
        if (jQuery.support.xhrUpload) {
            var c =
                a.element.attr("files")[0], d = c.name || c.fileName, f = c.size || c.fileSize;
            a._filename.html(d + " (" + Renderer.pktsBytes(f) + " bytes)");
            var g = new XMLHttpRequest;
            g.upload.onprogress = function (h) {
                a._bar.progressbar("value", h.loaded / h.total * 100)
            };
            g.onload = function () {
                a._bar.progressbar("value", 100);//alert(g.responseText);
                a._uploaded(b.parseJSON(g.responseText).meta)
            };
            g.onabort = g.onerror = function () {
                a.cancel()
            };
            g.open("POST", a.options.url, true);
            g.setRequestHeader("Cache-Control", "no-cache");
            g.setRequestHeader("X-Requested-With", "XMLHttpRequest");
            g.setRequestHeader("X-File-Name", d);
            g.setRequestHeader("X-File-Size", f);
            g.setRequestHeader("Content-Type", "multipart/form-data;boundary="+"---------------------------714a6d158c9");
            g.send(c)
        } else {
            a._iframe.contents().find("div#" + a._id).remove();
            a._filename.html("Uploading ...");
            this._form.get(0).submit();
            this._monitor(this)
        }
    }, _uploaded: function (a) {
        this._cancel.hide();//alert("test "+a.rc);
        this._filename.removeClass("busy");
        if (a.rc == "error") {
            this._error.html(V.localizeMsg(ApiMsg[a.msg], a));
            this._tryagain.show()
        } else {
            this._confirm.show();
            this._trigger("uploaded", 0, a)
        }
    }, _monitor: function (a) {
        var c =
            a._iframe.contents().find("div#" + a._id);
        if (c.size() > 0) {
            c = b.parseJSON(c.attr("response")).meta;
            a._filename.text(c.filename + " (" + Renderer.pktsBytes(c.filesize) + " bytes)");
            a._bar.progressbar("value", 100);
            a._uploaded(c)
        } else setTimeout(function () {
            a._monitor(a)
        }, 1E3)
    }, cancel: function () {
        this._confirm.hide();
        this._progress.hide();
        this._tryagain.hide();
        this.element.show();
        this.element.val("");
        this.options.path.val("");
        this.options.path.show();
        this.options.btn.show();
    }, widgetEventPrefix: "jupload_"})
})(jQuery);
window.Renderer = function () {
};
Renderer.original = function (b) {
    return b
};
Renderer.qosRate = function (b) {
	if(!b){return "";}
    return"Download : " + (b.split("#")[0] == "-1" ? "Unlimited":b.split("#")[0] +" Kbps") +"   "+"Upload : " + (b.split("#")[1] == "-1" ? "Unlimited":b.split("#")[1] + " Kbps");//+ (b[0].qos_rate_max_down == -1 ? "Unlimited" : b[0].qos_rate_max_down + " Kbps") + ", Upload: " + (b[0].qos_rate_max_up == -1 ? "Unlimited" : b[0].qos_rate_max_up + " Kbps")
};
Renderer.pktsBytes = function (b) {
    var e = ["", "K", "M", "G", "T"];
    b = parseInt(b) || 0;
    for (var a = 0; a < e.length; a++)if (b < 999.5)return b.toPrecision(3) + e[a]; else if (b < 1024)return b.toFixed(0) + e[a]; else b /= 1024;
    return b.toFixed(0) + e[e.length - 1]
};
Renderer.pktsBytes.className = "number";
Renderer.activityBar = function (b) {
    var e = $('<ul class="activity-bar"></ul>');
    $.each([2048E3, 512E3, 64E3, 8E3, 500], function (a, c) {
        b > c && $("<li>&nbsp;</li>").appendTo(e)
    });
    return e
};
Renderer.boolValue = function (b) {
    return b ? "true" : "false"
};
Renderer.activityNumber = function (b) {
    return Renderer.pktsBytes(b) + "B/s"
};
Renderer.percentage = function (b, e) {
    return typeof e == "undefined" ? parseFloat(b).toFixed(1) + "%" : e == 0 ? "0%" : (parseInt(b) / e).toFixed(1) + "%"
};
Renderer.staRadio = function (b, e) {
    if (e.is_11ac)return"ac";
    return e.is_11n ? e.is_11a ? "na" : "ng" : e.radio == "na" ? "a" : e.is_11b ? "b" : "g"
};
Renderer.staChannel = function (b, e) {
    return b + " (11" + Renderer.staRadio(b, e) + ")"
};
Renderer.rssiWithRadio = function (b, e) {
    return $("<div>" + Renderer.rssi(b) + "</div>").addClass(Renderer.staRadio(b, e) + "-" + (e.powersave_enabled ? "pwrsave" : "pwr"))
};
Renderer.rssi = function (b) {
    b = parseFloat(b);
    if (!b)return"N/A";
    if (b > 45)b = 45;
    if (b < 5)b = 5;
    return((b - 5) / 40 * 99).toPrecision(2) + "%"
};
Renderer.rssi.className = "number";
Renderer.timestamp = function (b) {
    function e(c) {
        return c < 10 ? "0" + c : c
    }

    if (b == null || isNaN(b))return"";
    var a = new Date;
    a.setTime(parseInt(b));
    b = a.getFullYear() + "/" + e(a.getMonth() + 1) + "/" + e(a.getDate());
    b += "  ";
    b += e(a.getHours()) + ":" + e(a.getMinutes()) + ":" + e(a.getSeconds());
    return b
};
Renderer.timestamp_sec = function (b) {
    if (b == null || isNaN(b))return"";
    try {
        return Renderer.timestamp(parseInt(b) * 1E3)
    } catch (e) {
        return""
    }
};
Renderer.lastseen = function (b) {
    var e = (new Date).getTime() - parseInt(b);
    if (isNaN(e))return"N/A";
	//86400 is one day
    return e < 864E5 ? Renderer.uptime(e / 1E3) + " ago" : Renderer.timestamp(b)
};
Renderer.timeHour24 = function (b) {
    return(new Date(b)).getHours() + ":00"
};
Renderer.timeMonthDay = function (b) {
    b = new Date(b);
    return["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"][b.getMonth()] + ". " + b.getDate()
};
Renderer.status=function(b){
	var e = $("<span></span>");
	switch (b)
	{
		case "located": var StaName="afiStatus1";break;
		case "unlocated": var StaName="afiStatus2";break;
		case "locating": var StaName="afiStatus3";break;
	}
    e.addClass(StaName);
    return e;
};
Renderer.state=function(b){
	switch (b){
		case "located": var LocName="locateStatus1";var e=$("<div class='"+LocName+"'>"+Msg.located+"</div>"); return e;
		case "unlocated": var LocName="locateStatus2";var e=$("<div class='"+LocName+"'>"+Msg.unlocated+"</div>"); return e;
		case "locating": var LocName="locateStatus3";var e=$("<div class='"+LocName+"'>"+Msg.locating+"</div>"); return e;
	}
};
Renderer.apModel = function (b) {
	return b;
	//if (b == "BZ2")return"UniFi AP"; else if (b == "BZ2LR")return"UniFi AP-LR"; else if (b == "U2S48")return"UniFi AP"; else if (b == "U2L48")return"UniFi AP-LR"; else if (b == "U2HSR")return"UniFi AP-Outdoor+"; else if (b == "U2O")return"UniFi AP-Outdoor"; else if (b == "U5O")return"UniFi AP-Outdoor 5G"; else if (b == "U7P")return"UniFi AP-Pro"; else if (b == "U2M")return"UniFi AP-Mini"; else if (b == "p2N")return"PicoStation M2"; else if (b == "U7E")return"UniFi AP-AC"; else if (b == "U7O")return"UniFi AP-AC Outdoor";
    //else if (b == "U7Ev2")return"UniFi AP-AC v2"
};
Renderer.apIcon = function (b) {
    var e = $("<span></span>");
    e.addClass("dev-model").addClass("model-" + b);
    return e
};
Renderer.DeviceIcon = function (b) {
	
	var titleInfo=eval('Msg.'+b);
    var e = $("<a title='"+titleInfo+"'><div class='"+b+"'></div></a>");
    return e
};
var stats = new Array(Msg.WTPStatSulking, Msg.WTPStatDiscovery, Msg.WTPStatJoin, 
	Msg.WTPStatConfigure, Msg.WTPStatDataCheck, Msg.WTPStatRun, 
	Msg.WTPStatReset, Msg.WTPStatQuit, Msg.WTPStatImageData, Msg.WTPStatBakRun, 
	Msg.WTPStatOffline, Msg.WTPStatRejoin, Msg.WTPStatAdopting, Msg.WTPStatUpgrading);/*used for this and *manage*.js  file */
Renderer.apStatusType = function (b) {
    b = parseInt(b) || 0;
    switch (b) {
        case 3:/*conifgure*/
        case 4:/*datacheck*/
        case 5:/*run*/
        case 8:/*imagedata*/
        case 9:/*bakrun*/
            return"ok";
        case 1:/*discovery*/
        case 2:/*join*/
        case 11:/*rejoin*/
        case 12:/*init  */
        case 13:/*upgrading */
            return"pending";
        case 0:/*sulking*/
        case 6:/*reset*/
        case 7:/*quit*/
		case 10:/*offline*/
            return"error";
        default:
            return"error"
    }
};
Renderer.apStatusType2 = function (b) {
    b = parseInt(b) || 0;
    switch (b) {
        case 1:
        case 4:
        case 5:
            return"ok";
        case 2:
        case 7:
        case 12:
        case 13:
            return"pending";
        case 0:
        case 3:
        case 9:
        case 10:
        case 11:
            return"error";
        default:
            return"error"
    }
};
Renderer.mapId = function (b) {
    return b
};

Renderer.apStatus = function (b, e) {
    var a = $("<span></span>"), c;
    b = parseInt(b);
	/*
    if (b == 0)c = Msg.WTPStatSulking;
	else if (b == 1)c = Msg.WTPStatDiscovery;
	else if (b == 2)c = Msg.WTPStatJoin; 
	else if (b == 3)c = Msg.WTPStatConfigure;
	else if (b == 4)c = Msg.WTPStatDataCheck;
	else if (b == 5)c = Msg.WTPStatRun; 
	else if (b == 6)c = Msg.WTPStatReset;
	else if (b == 7)c = Msg.WTPStatQuit; 
	else if (b == 8)c = Msg.WTPStatImageData;
	else if (b == 9)c = Msg.WTPStatBakRun;*/
	c = stats[b];
	
    a.addClass(Renderer.apStatusType(b));
    a.html(c);
    return a
};

Renderer.apStatus2 = function (b, e)
{
    var a = $("<span></span>"), c;
    b = parseInt(b);
    if (b == 1)
		c = Msg.APConnected;
	else if (b == 0)
		c = Msg.APDisconnected;
	else if (b == 3)
		c = Msg.APDisconnected;
	else if (b == 4)
		c = Msg.APUpgrading;
	else if (b == 5)
		c = Msg.APProvisioning;
	else if (b == 2)
		c = e["default"] ? Msg.APPending : Msg.APManagedByOthers;
	else if (b == 6)
		c = Msg.APHeartbeatMissed;
	else if (b == 7)
		c = Msg.APAdopting;
	else if (b == 8)
		c = Msg.APDeleting;
	else if (b == 9)
		c = Msg.APManagedByOthers;
	else if (b == 10)
		c = Msg.APAdoptFailed;
	else if (b == 11)
		c = Msg.APIsolated;
	else if (b == 12)
		c = Msg.APIsolatedPending;
	else if (b == 13)
		c = Msg.APWirelessAdopting;
    if ((b == 1 || b == 7) && e && e.uplink && e.uplink.name == "ath0")
		c += " (wireless)";
    a.addClass(Renderer.apStatusType(b));
    a.html(c);
    return a
};

Renderer.apChannel = function (b, e)
{
    if (!e.radio_table)
		return "";
    var a = [];
    $.each(e.radio_table, function (c, d) {
        if (d.channel == 0)
			return true;
        if ((d.state || "RUN") != "RUN")
			return true;
        var f = d.channel + " (" + d.radio + ")";
        if ($.inArray(f, a) < 0)
			if (d.radio == "ng")
				a = [f].concat(a);
			else
				a.push(f)
    });
    return a.join(", ")
};

Renderer.apChannel.className = "number";

Renderer.apXRate = function (b)
{
    if (isNaN(b))
		return"N/A";
    return parseInt(b) / 1E3 + "M"
};

Renderer.apName = function (b)
{
    return $("#model-aps").data("ap_names")[b] || b
};

Renderer.apDetailLink = function (b, n)
{
	var e = $("#model-aps").data("ap_names")[b];
	var t = b;
    //e = e ?( e + " / " + b) : b;
    if (n && n.mac == b)
	{
		t = "ID " + n._id + " MAC " + t;
	}
	if (n && n.roam_ap && (2 == n.roam))
	{
		e = e + " roam from " + Renderer.apName(n.roam_ap);
	}
    return $('<a href="javascript:void(0);" class="ap-link" title="' + t + '"></a>').text(e)
};

Renderer.apDetailLink.className = "ap-name";

Renderer.staDetailLink = function (b)
{
    var e = $("#model-stas").data("sta_names")[b] || b;
    return $('<a href="javascript:void(0);" class="sta-link" title="' + b + '"></a>').text(e)
};

Renderer.staDetailLinkWithName = function (b, e)
{
    return $('<a href="javascript:void(0);" class="sta-link" title="' + b + '"></a>').text(e)
};

Renderer.eventMessage = function (b, e)
{
    if (!b)
		return "";
    b = b.replace(/AP\[([0-9a-fA-F:]+)((\,[0-9.]+)?)\]/g, 'AP[<a href="javascript:void(0);" title="$1" class="ap-link">$2</a>]');
    b = b.replace(/User\[([0-9a-fA-F:]+)((\,[0-9.]+)?)\]/g, 'User[<a href="javascript:void(0);" title="$1" class="sta-link">$2</a>]');
    b = b.replace(/Guest\[([0-9a-fA-F:]+)((\,[0-9.]+)?)\]/g, 'Guest[<a href="javascript:void(0);" title="$1" class="sta-link">$2</a>]');
    b = b.replace(/Client\[([0-9a-fA-F:]+)((\,[0-9.]+)?)\]/g, 'Client[<a href="javascript:void(0);" title="$1" class="sta-link">$2</a>]');
    var a = $("<span></span>").html(b);
    a.find("a.ap-link").each(function () {
        var c = $(this).attr("title");
		var txt = $(this).text()||"";
		c = c.toLowerCase();
		$(this).attr("title", c.toLowerCase());
        c = $("#model-aps").data("ap_names")[c] || c;
        $(this).text(c+txt);
    });
    a.find("a.sta-link").each(function () {
        var c = $(this).attr("title");
		var txt = $(this).text()||"";
		c = c.toLowerCase();
		$(this).attr("title", c.toLowerCase());
        c = $("#model-stas").data("sta_names")[c] || e.hostname || c;
        $(this).text(c+txt);
    });
    return a
};
Renderer.ccq = function (b) {
    if (b == 0 || isNaN(b))return"N/A";
    return b / 10 + "%"
};
Renderer.ccq.className = "number";
Renderer.uptime = function (b) {
    var e = [];
    if (isNaN(b) || b == 0)return"0s";
    var a = ["d", "h", "m", "s"], c = [86400, 3600, 60, 1];
    b = parseInt(b);
    for (var d = 0; d < a.length; d++) {
        var f = Math.floor(b / c[d]);
        if (f) {
            e.push(f + a[d]);
            b %= c[d]
        }
        if (e.length > 2)break
    }
    return e.join(" ")
};
Renderer.uptime.className = "number";
Renderer.amount = function (b, e) {
    if (b) {
        var a = $(e).attr_get("currency", "USD");
        switch (a) {
            case "USD":
                return"$" + b;
            case "EUR":
                return"&#8364;" + b
        }
        return b + "(" + a + ")"
    }
    return b
};
window.V = function () {
};
V.reWhitespace = /^\s+$/;
V.reAlphanumeric = /^[a-zA-Z0-9]+$/;
V.reInteger = /^\d+$/;
V.reSsid = /^([\x20-\x7E]){1,32}$/;
V.rePassphrase = /(^([\x20-\x7E]){8,63}$)/;
V.reWEP64 = /^([0-9a-fA-F]){10}$/;
V.reWEP128 = /^([0-9a-fA-F]){26}$/;
V.reWEP = /^([0-9a-fA-F]){10}$|^([0-9a-fA-F]){26}$|^([\x20-\x7E]){5}$|^([\x20-\x7E]){13}$/;
V.reAdminName = /^[a-zA-Z0-9]+$/;
V.reAdminPassword = /^[a-zA-Z0-9][\x21-\x7E]+$/;
V.reIPv4Loose = /^\d+.\d+.\d+.\d+$/;
V.reSubnetSpec = /^\d+.\d+.\d+.\d+\/\d+$/;
V.reEmail = /^[\w\-\.\+]+\@[a-zA-Z0-9\.\-]+\.[a-zA-z0-9]{2,10}$/;
V.reHostname = /^[a-zA-Z0-9.-]+$/;
V.isNumber = function (b) {
    if (typeof b === "boolean")return false;
    return b != null && !isNaN(b)
};
V.ip2long = function (b) {
    var e = false, a = [];
    if (b.match(/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/)) {
        a = b.split(".");
        e = a[0] * 16777216 + a[1] * 65536 + a[2] * 256 + a[3] * 1
    }
    return e
};
V.isIP = function (b, e) {
    if (V.reIPv4Loose.test(b)) {
        var a = b.match(/[0-9]+/g);
        if (a.length == 4) {
            if (a[0] == 0 || a[0] > parseInt(e))return false;
            for (var c = 1; c < 4; c++)if (a[c] > 255)return false;
            return true
        }
    }
    return false
};
V.error = function (b, e) {
    b = $(b).get(0);
    if (!$.data(b, "v_error")) {
        $(b).wrap('<span class="v-error"></span>');
        $.data(b, "v_error", true);
        $(b).select();
        $(b).focus();
        alert(e)
    }
};
V.normal = function (b) {
    b = $(b).get(0);
    if ($.data(b, "v_error")) {
        $.removeData(b, "v_error");
        var e = $(b).parent();
        $(b).insertAfter(e);
        e.remove()
    }
};
V.getval = function (b) {
    return b.data("jpass") ? b.jpass("val") : b.val()
};
V.checkSsid = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    /*if (!V.reSsid.test(a)) {
        V.error(b, V.localize(Msg.E_InvalidSSID, e));
        return false
    }*/
    return true
};
V.checkPassphrase = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (!V.rePassphrase.test(a)) {
        V.error(b, V.localize(Msg.E_InvalidPassphrase, e));
        return false
    }
    return true
};
V.checkAdminName = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (!V.reAdminName.test(a)) {
        V.error(b, V.localize(Msg.E_InvalidAdminName, e));
        return false
    }
    return true
};
V.checkOldPasswords = function(op)
{
  V.normal(op);
  var opv = op.val();
  //alert(ouv);alert(opv);
  if ($.trim(opv) == "") {
      V.error(op, V.localize(Msg.E_CannotBeEmpty, Msg.OldPassword));
      return false
  }
  
	return true;
}
V.checkAdminPasswords = function (b, e, a, c) {
    V.normal(b);
    V.normal(e);
    var d = b.val(), f = e.val();
    if ($.trim(d) == "") {
        V.error(b, V.localize(Msg.E_CannotBeEmpty, a));
        return false
    }
    if (!V.reAdminPassword.test(d)) {
        V.error(b, V.localize(Msg.E_InvalidAdminPassword, a));
        return false
    }
    if (d != f) {
        V.error(e, V.localize(Msg.E_DifferentPass, a, c));
        return false
    }
    return true
};
V.checkEmpty = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if ($.trim(a) == "") {
        V.error(b, V.localize(Msg.E_CannotBeEmpty, e));
        return false
    }
    return true
};
V.checkRE = function (b, e, a, c) {
    V.normal(b);
    var d = V.getval(b);
    if (!a.test(d)) {
        V.error(b, V.localize(c, e));
        return false
    }
    return true
};
V.checkLength = function (b, e, a, c) {
    V.normal(b);
    var d = V.getval(b);
    if (d.length < e || d.length > a) {
        V.error(b, V.localize(Msg.E_IncorrectLength, c, e, a));
        return false
    }
    return true
};
V.checkMinMax = function (b, e, a, c) {
    V.normal(b);
    var d = V.getval(b);
    if (!V.isNumber(d) || d > a || d < e) {
        V.error(b, V.localize(Msg.E_IncorrectNumberRance, c, e, a));
        return false
    }
    return true
};
V.checkIP = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (!V.isIP(a, 223)) {
        V.error(b, V.localize(Msg.E_IncorrectIPAddress, e));
        return false
    }
    return true
};
V.checkNetmask = function (b, e, a) {
    V.normal(b);
    var c = V.getval(b), d = V.ip2long(c);
    a = V.ip2long(a);
    var f = ~parseInt(d) & 4294967295;
    if ((f + 1 & f) != 0 || !V.isIP(c, 255)) {
        V.error(b, V.localize(Msg.E_IncorrectNetmask, e));
        return false
    }
    if (!(a ^ a & d)) {
        V.error(b, V.localize(Msg.E_IncorrectIPMaskCombination, e));
        return false
    }
    return true
};
V.checkSubnetSpec = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (V.reSubnetSpec.test(a)) {
        var c = a.indexOf("/"), d = a.substring(0, c);
        a = parseInt(a.substring(c + 1));
        if (V.isIP(d, 255))if (!(a > 32))return true
    }
    V.error(b, V.localize(Msg.E_IncorrectSubnetSpec, e));
    return false
};
V.checkEmail = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (!V.reEmail.test(a)) {
        V.error(b, V.localize(Msg.E_IncorrectEmailAddress, e));
        return false
    }
    return true
};
V.checkInteger = function (b, e) {
    V.normal(b);
    var a = V.getval(b);
    if (!V.reInteger.test(a)) {
        V.error(b, V.localize(Msg.E_IncorrectIntegerType, e));
        return false
    }
    return true
};
V.localizer_re = [];
for (var i = 0; i <= 16; i++)V.localizer_re.push(eval("/\\{" + i + "\\}/g"));
V.localize = function (b) {
    for (var e = b, a = arguments.length - 1, c = 1; c <= a; c++)e = e.replace(V.localizer_re[c], arguments[c]);
    return e
};
V.localizeMsg = function (b, e) {
    var a = b.substring(0);
    do {
        var c = a.match(/{([a-z]+)}/);
        if (!c)break;
        var d = c[1], f = e[d];
        if (d == "timestamp")f = Renderer.timestamp(f);
        a = a.replace(c[0], f)
    } while (1);
    return a
};
window.ApiMsg = function () {
};
ApiMsg["api.err.InvalidTarget"] = Msg.E_InvalidTarget;
ApiMsg["api.err.NoUplinkAvailable"] = Msg.E_NoUplinkAvailable;
ApiMsg["api.err.WirelessUplinkDisabled"] = Msg.E_WirelessUplinkDisabled;
ApiMsg["api.err.InvalidBackup"] = Msg.E_InvalidBackup;
ApiMsg["api.err.InvalidBackupVersion"] = Msg.E_InvalidBackupVersion;
ApiMsg["api.err.SmtpAuthFailed"] = Msg.E_SmtpAuthFailed;
ApiMsg["api.err.SmtpNoSuchRcpt"] = Msg.E_SmtpNoSuchRcpt;
ApiMsg["api.err.SmtpSendFailed"] = Msg.E_SmtpSendFailed;
ApiMsg["api.err.SmtpUnknownFailed"] = Msg.E_SmtpUnknownFailed;
ApiMsg["api.err.ObjectReferredBy"] = Msg.E_ObjectReferredBy;
ApiMsg["api.err.InvalidApiKey"] = Msg.E_InvalidApiKey;
ApiMsg["api.err.OldPasswordError"] = Msg.E_OldPasswordError;
ApiMsg["api.err.AddWirelessNetworkFailed"] = Msg.E_AddWirelessNetworkFailed;
ApiMsg["api.err.UpdateWirelessNetworkFailed"] = Msg.E_UpdateWirelessNetworkFailed;
ApiMsg["api.err.DeleteWirelessNetworkFailed"] = Msg.E_DeleteWirelessNetworkFailed;
ApiMsg["api.err.GetWirelessNetworkFailed"] = Msg.E_GetWirelessNetworkFailed;
ApiMsg["api.err.OperationFailed"] = Msg.E_OperationFailed;
ApiMsg["api.err.OperationSuccess"] = Msg.E_OperationSuccess;
ApiMsg["api.err.UserNameExists"] = Msg.E_UserNameExists;
ApiMsg["api.err.UpgradeURLParseFailed"] = Msg.E_UpgradeURLParseFailed;
ApiMsg["api.err.UpgradeDownloadFailed"] = Msg.E_UpgradeDownloadFailed;
ApiMsg["api.err.UpgradeLoadFailed"] = Msg.E_UpgradeLoadFailed;
ApiMsg["api.err.RestoringUploadFailed"] = Msg.E_RestoringUploadFailed;
ApiMsg["api.err.RestoringLoadFailed"] = Msg.E_RestoringLoadFailed;
LOGGING_ENABLED = false;
