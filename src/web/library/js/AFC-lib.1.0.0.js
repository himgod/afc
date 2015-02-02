(function (c, b) {
    function e() {
        if (!i.isReady) {
            try {
                D.documentElement.doScroll("left")
            } catch (d) {
                setTimeout(e, 1);
                return
            }
            i.ready()
        }
    }

    function f(d, j) {
        j.src ? i.ajax({url: j.src, async: false, dataType: "script"}) : i.globalEval(j.text || j.textContent || j.innerHTML || "");
        j.parentNode && j.parentNode.removeChild(j)
    }

    function a(d, j, k, m, n, w) {
        var x = d.length;
        if (typeof j === "object") {
            for (var E in j)a(d, E, j[E], m, n, k);
            return d
        }
        if (k !== b) {
            m = !w && m && i.isFunction(k);
            for (E = 0; E < x; E++)n(d[E], j, m ? k.call(d[E], E, n(d[E], j)) : k, w);
            return d
        }
        return x ?
            n(d[0], j) : b
    }

    function g() {
        return(new Date).getTime()
    }

    function h() {
        return false
    }

    function p() {
        return true
    }

    function q(d, j, k) {
        k[0].type = d;
        return i.event.handle.apply(j, k)
    }

    function o(d) {
        var j, k = [], m = [], n = arguments, w, x, E, B, F, I;
        x = i.data(this, "events");
        if (!(d.liveFired === this || !x || !x.live || d.button && d.type === "click")) {
            d.liveFired = this;
            var N = x.live.slice(0);
            for (B = 0; B < N.length; B++) {
                x = N[B];
                x.origType.replace(ra, "") === d.type ? m.push(x.selector) : N.splice(B--, 1)
            }
            w = i(d.target).closest(m, d.currentTarget);
            F = 0;
            for (I =
                     w.length; F < I; F++)for (B = 0; B < N.length; B++) {
                x = N[B];
                if (w[F].selector === x.selector) {
                    E = w[F].elem;
                    m = null;
                    if (x.preType === "mouseenter" || x.preType === "mouseleave")m = i(d.relatedTarget).closest(x.selector)[0];
                    if (!m || m !== E)k.push({elem: E, handleObj: x})
                }
            }
            F = 0;
            for (I = k.length; F < I; F++) {
                w = k[F];
                d.currentTarget = w.elem;
                d.data = w.handleObj.data;
                d.handleObj = w.handleObj;
                if (w.handleObj.origHandler.apply(w.elem, n) === false) {
                    j = false;
                    break
                }
            }
            return j
        }
    }

    function l(d, j) {
        return"live." + (d && d !== "*" ? d + "." : "") + j.replace(/\./g, "`").replace(/ /g,
            "&")
    }

    function r(d, j) {
        var k = 0;
        j.each(function () {
            if (this.nodeName === (d[k] && d[k].nodeName)) {
                var m = i.data(d[k++]), n = i.data(this, m);
                if (m = m && m.events) {
                    delete n.handle;
                    n.events = {};
                    for (var w in m)for (var x in m[w])i.event.add(this, w, m[w][x], m[w][x].data)
                }
            }
        })
    }

    function u(d, j, k) {
        var m, n, w;
        j = j && j[0] ? j[0].ownerDocument || j[0] : D;
        if (d.length === 1 && typeof d[0] === "string" && d[0].length < 512 && j === D && !Ja.test(d[0]) && (i.support.checkClone || !Ka.test(d[0]))) {
            n = true;
            if (w = i.fragments[d[0]])if (w !== 1)m = w
        }
        if (!m) {
            m = j.createDocumentFragment();
            i.clean(d, j, m, k)
        }
        if (n)i.fragments[d[0]] = w ? m : 1;
        return{fragment: m, cacheable: n}
    }

    function y(d, j) {
        var k = {};
        i.each(La.concat.apply([], La.slice(0, j)), function () {
            k[this] = d
        });
        return k
    }

    function A(d) {
        return"scrollTo"in d && d.document ? d : d.nodeType === 9 ? d.defaultView || d.parentWindow : false
    }

    var i = function (d, j) {
            return new i.fn.init(d, j)
        }, J = c.jQuery, K = c.$, D = c.document, P, X = /^[^<]*(<[\w\W]+>)[^>]*$|^#([\w-]+)$/, ea = /^.[^:#\[\.,]*$/, Y = /\S/, S = /^(\s|\u00A0)+|(\s|\u00A0)+$/g, $ = /^<(\w+)\s*\/?>(?:<\/\1>)?$/, ga = navigator.userAgent,
        sa = false, ha = [], fa, na = Object.prototype.toString, ia = Object.prototype.hasOwnProperty, oa = Array.prototype.push, U = Array.prototype.slice, aa = Array.prototype.indexOf;
    i.fn = i.prototype = {init: function (d, j) {
        var k, m;
        if (!d)return this;
        if (d.nodeType) {
            this.context = this[0] = d;
            this.length = 1;
            return this
        }
        if (d === "body" && !j) {
            this.context = D;
            this[0] = D.body;
            this.selector = "body";
            this.length = 1;
            return this
        }
        if (typeof d === "string")if ((k = X.exec(d)) && (k[1] || !j))if (k[1]) {
            m = j ? j.ownerDocument || j : D;
            if (d = $.exec(d))if (i.isPlainObject(j)) {
                d =
                    [D.createElement(d[1])];
                i.fn.attr.call(d, j, true)
            } else d = [m.createElement(d[1])]; else {
                d = u([k[1]], [m]);
                d = (d.cacheable ? d.fragment.cloneNode(true) : d.fragment).childNodes
            }
            return i.merge(this, d)
        } else {
            if (j = D.getElementById(k[2])) {
                if (j.id !== k[2])return P.find(d);
                this.length = 1;
                this[0] = j
            }
            this.context = D;
            this.selector = d;
            return this
        } else if (!j && /^\w+$/.test(d)) {
            this.selector = d;
            this.context = D;
            d = D.getElementsByTagName(d);
            return i.merge(this, d)
        } else return!j || j.jquery ? (j || P).find(d) : i(j).find(d); else if (i.isFunction(d))return P.ready(d);
        if (d.selector !== b) {
            this.selector = d.selector;
            this.context = d.context
        }
        return i.makeArray(d, this)
    }, selector: "", jquery: "1.4.2", length: 0, size: function () {
        return this.length
    }, toArray: function () {
        return U.call(this, 0)
    }, get: function (d) {
        return d == null ? this.toArray() : d < 0 ? this.slice(d)[0] : this[d]
    }, pushStack: function (d, j, k) {
        var m = i();
        i.isArray(d) ? oa.apply(m, d) : i.merge(m, d);
        m.prevObject = this;
        m.context = this.context;
        if (j === "find")m.selector = this.selector + (this.selector ? " " : "") + k; else if (j)m.selector = this.selector +
            "." + j + "(" + k + ")";
        return m
    }, each: function (d, j) {
        return i.each(this, d, j)
    }, ready: function (d) {
        i.bindReady();
        if (i.isReady)d.call(D, i); else ha && ha.push(d);
        return this
    }, eq: function (d) {
        return d === -1 ? this.slice(d) : this.slice(d, +d + 1)
    }, first: function () {
        return this.eq(0)
    }, last: function () {
        return this.eq(-1)
    }, slice: function () {
        return this.pushStack(U.apply(this, arguments), "slice", U.call(arguments).join(","))
    }, map: function (d) {
        return this.pushStack(i.map(this, function (j, k) {
            return d.call(j, k, j)
        }))
    }, end: function () {
        return this.prevObject ||
            i(null)
    }, push: oa, sort: [].sort, splice: [].splice};
    i.fn.init.prototype = i.fn;
    i.extend = i.fn.extend = function () {
        var d = arguments[0] || {}, j = 1, k = arguments.length, m = false, n, w, x, E;
        if (typeof d === "boolean") {
            m = d;
            d = arguments[1] || {};
            j = 2
        }
        if (typeof d !== "object" && !i.isFunction(d))d = {};
        if (k === j) {
            d = this;
            --j
        }
        for (; j < k; j++)if ((n = arguments[j]) != null)for (w in n) {
            x = d[w];
            E = n[w];
            if (d !== E)if (m && E && (i.isPlainObject(E) || i.isArray(E))) {
                x = x && (i.isPlainObject(x) || i.isArray(x)) ? x : i.isArray(E) ? [] : {};
                d[w] = i.extend(m, x, E)
            } else if (E !== b)d[w] =
                E
        }
        return d
    };
    i.extend({noConflict: function (d) {
        c.$ = K;
        if (d)c.jQuery = J;
        return i
    }, isReady: false, ready: function () {
        if (!i.isReady) {
            if (!D.body)return setTimeout(i.ready, 13);
            i.isReady = true;
            if (ha) {
                for (var d, j = 0; d = ha[j++];)d.call(D, i);
                ha = null
            }
            i.fn.triggerHandler && i(D).triggerHandler("ready")
        }
    }, bindReady: function () {
        if (!sa) {
            sa = true;
            if (D.readyState === "complete")return i.ready();
            if (D.addEventListener) {
                D.addEventListener("DOMContentLoaded", fa, false);
                c.addEventListener("load", i.ready, false)
            } else if (D.attachEvent) {
                D.attachEvent("onreadystatechange",
                    fa);
                c.attachEvent("onload", i.ready);
                var d = false;
                try {
                    d = c.frameElement == null
                } catch (j) {
                }
                D.documentElement.doScroll && d && e()
            }
        }
    }, isFunction: function (d) {
        return na.call(d) === "[object Function]"
    }, isArray: function (d) {
        return na.call(d) === "[object Array]"
    }, isPlainObject: function (d) {
        if (!d || na.call(d) !== "[object Object]" || d.nodeType || d.setInterval)return false;
        if (d.constructor && !ia.call(d, "constructor") && !ia.call(d.constructor.prototype, "isPrototypeOf"))return false;
        var j;
        for (j in d);
        return j === b || ia.call(d, j)
    },
        isEmptyObject: function (d) {
            for (var j in d)return false;
            return true
        }, error: function (d) {
            throw d;
        }, parseJSON: function (d) {
            if (typeof d !== "string" || !d)return null;
            d = i.trim(d);
            if (/^[\],:{}\s]*$/.test(d.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, "@").replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, "]").replace(/(?:^|:|,)(?:\s*\[)+/g, "")))return c.JSON && c.JSON.parse ? c.JSON.parse(d) : (new Function("return " + d))(); else i.error("Invalid JSON: " + d)
        }, noop: function () {
        }, globalEval: function (d) {
            if (d &&
                Y.test(d)) {
                var j = D.getElementsByTagName("head")[0] || D.documentElement, k = D.createElement("script");
                k.type = "text/javascript";
                if (i.support.scriptEval)k.appendChild(D.createTextNode(d)); else k.text = d;
                j.insertBefore(k, j.firstChild);
                j.removeChild(k)
            }
        }, nodeName: function (d, j) {
            return d.nodeName && d.nodeName.toUpperCase() === j.toUpperCase()
        }, each: function (d, j, k) {
            var m, n = 0, w = d.length, x = w === b || i.isFunction(d);
            if (k)if (x)for (m in d) {
                if (j.apply(d[m], k) === false)break
            } else for (; n < w;) {
                if (j.apply(d[n++], k) === false)break
            } else if (x)for (m in d) {
                if (j.call(d[m],
                    m, d[m]) === false)break
            } else for (k = d[0]; n < w && j.call(k, n, k) !== false; k = d[++n]);
            return d
        }, trim: function (d) {
            return(d || "").replace(S, "")
        }, makeArray: function (d, j) {
            j = j || [];
            if (d != null)d.length == null || typeof d === "string" || i.isFunction(d) || typeof d !== "function" && d.setInterval ? oa.call(j, d) : i.merge(j, d);
            return j
        }, inArray: function (d, j) {
            if (j.indexOf)return j.indexOf(d);
            for (var k = 0, m = j.length; k < m; k++)if (j[k] === d)return k;
            return-1
        }, merge: function (d, j) {
            var k = d.length, m = 0;
            if (typeof j.length === "number")for (var n = j.length; m <
                n; m++)d[k++] = j[m]; else for (; j[m] !== b;)d[k++] = j[m++];
            d.length = k;
            return d
        }, grep: function (d, j, k) {
            for (var m = [], n = 0, w = d.length; n < w; n++)!k !== !j(d[n], n) && m.push(d[n]);
            return m
        }, map: function (d, j, k) {
            for (var m = [], n, w = 0, x = d.length; w < x; w++) {
                n = j(d[w], w, k);
                if (n != null)m[m.length] = n
            }
            return m.concat.apply([], m)
        }, guid: 1, proxy: function (d, j, k) {
            if (arguments.length === 2)if (typeof j === "string") {
                k = d;
                d = k[j];
                j = b
            } else if (j && !i.isFunction(j)) {
                k = j;
                j = b
            }
            if (!j && d)j = function () {
                return d.apply(k || this, arguments)
            };
            if (d)j.guid = d.guid =
                d.guid || j.guid || i.guid++;
            return j
        }, uaMatch: function (d) {
            d = d.toLowerCase();
            d = /(webkit)[ \/]([\w.]+)/.exec(d) || /(opera)(?:.*version)?[ \/]([\w.]+)/.exec(d) || /(msie) ([\w.]+)/.exec(d) || !/compatible/.test(d) && /(mozilla)(?:.*? rv:([\w.]+))?/.exec(d) || [];
            return{browser: d[1] || "", version: d[2] || "0"}
        }, browser: {}});
    ga = i.uaMatch(ga);
    if (ga.browser) {
        i.browser[ga.browser] = true;
        i.browser.version = ga.version
    }
    if (i.browser.webkit)i.browser.safari = true;
    if (aa)i.inArray = function (d, j) {
        return aa.call(j, d)
    };
    P = i(D);
    if (D.addEventListener)fa =
        function () {
            D.removeEventListener("DOMContentLoaded", fa, false);
            i.ready()
        }; else if (D.attachEvent)fa = function () {
        if (D.readyState === "complete") {
            D.detachEvent("onreadystatechange", fa);
            i.ready()
        }
    };
    (function () {
        i.support = {};
        var d = D.documentElement, j = D.createElement("script"), k = D.createElement("div"), m = "script" + g();
        k.style.display = "none";
        k.innerHTML = "   <link/><table></table><a href='/a' style='color:red;float:left;opacity:.55;'>a</a><input type='checkbox'/>";
        var n = k.getElementsByTagName("*"), w = k.getElementsByTagName("a")[0];
        if (!(!n || !n.length || !w)) {
            i.support = {leadingWhitespace: k.firstChild.nodeType === 3, tbody: !k.getElementsByTagName("tbody").length, htmlSerialize: !!k.getElementsByTagName("link").length, style: /red/.test(w.getAttribute("style")), hrefNormalized: w.getAttribute("href") === "/a", opacity: /^0.55$/.test(w.style.opacity), cssFloat: !!w.style.cssFloat, checkOn: k.getElementsByTagName("input")[0].value === "on", optSelected: D.createElement("select").appendChild(D.createElement("option")).selected, parentNode: k.removeChild(k.appendChild(D.createElement("div"))).parentNode ===
                null, deleteExpando: true, checkClone: false, scriptEval: false, noCloneEvent: true, boxModel: null};
            j.type = "text/javascript";
            try {
                j.appendChild(D.createTextNode("window." + m + "=1;"))
            } catch (x) {
            }
            d.insertBefore(j, d.firstChild);
            if (c[m]) {
                i.support.scriptEval = true;
                delete c[m]
            }
            try {
                delete j.test
            } catch (E) {
                i.support.deleteExpando = false
            }
            d.removeChild(j);
            if (k.attachEvent && k.fireEvent) {
                k.attachEvent("onclick", function B() {
                    i.support.noCloneEvent = false;
                    k.detachEvent("onclick", B)
                });
                k.cloneNode(true).fireEvent("onclick")
            }
            k = D.createElement("div");
            k.innerHTML = "<input type='radio' name='radiotest' checked='checked'/>";
            d = D.createDocumentFragment();
            d.appendChild(k.firstChild);
            i.support.checkClone = d.cloneNode(true).cloneNode(true).lastChild.checked;
            i(function () {
                var B = D.createElement("div");
                B.style.width = B.style.paddingLeft = "1px";
                D.body.appendChild(B);
                i.boxModel = i.support.boxModel = B.offsetWidth === 2;
                D.body.removeChild(B).style.display = "none"
            });
            d = function (B) {
                var F = D.createElement("div");
                B = "on" + B;
                var I = B in F;
                if (!I) {
                    F.setAttribute(B, "return;");
                    I = typeof F[B] ===
                        "function"
                }
                return I
            };
            i.support.submitBubbles = d("submit");
            i.support.changeBubbles = d("change");
            d = j = k = n = w = null
        }
    })();
    i.props = {"for": "htmlFor", "class": "className", readonly: "readOnly", maxlength: "maxLength", cellspacing: "cellSpacing", rowspan: "rowSpan", colspan: "colSpan", tabindex: "tabIndex", usemap: "useMap", frameborder: "frameBorder"};
    var Z = "jQuery" + g(), V = 0, ta = {};
    i.extend({cache: {}, expando: Z, noData: {embed: true, object: true, applet: true}, data: function (d, j, k) {
        if (!(d.nodeName && i.noData[d.nodeName.toLowerCase()])) {
            d =
                    d == c ? ta : d;
            var m = d[Z], n = i.cache;
            if (!m && typeof j === "string" && k === b)return null;
            m || (m = ++V);
            if (typeof j === "object") {
                d[Z] = m;
                n[m] = i.extend(true, {}, j)
            } else if (!n[m]) {
                d[Z] = m;
                n[m] = {}
            }
            d = n[m];
            if (k !== b)d[j] = k;
            return typeof j === "string" ? d[j] : d
        }
    }, removeData: function (d, j) {
        if (!(d.nodeName && i.noData[d.nodeName.toLowerCase()])) {
            d = d == c ? ta : d;
            var k = d[Z], m = i.cache, n = m[k];
            if (j) {
                if (n) {
                    delete n[j];
                    i.isEmptyObject(n) && i.removeData(d)
                }
            } else {
                if (i.support.deleteExpando)delete d[i.expando]; else d.removeAttribute && d.removeAttribute(i.expando);
                delete m[k]
            }
        }
    }});
    i.fn.extend({data: function (d, j) {
        if (typeof d === "undefined" && this.length)return i.data(this[0]); else if (typeof d === "object")return this.each(function () {
            i.data(this, d)
        });
        var k = d.split(".");
        k[1] = k[1] ? "." + k[1] : "";
        if (j === b) {
            var m = this.triggerHandler("getData" + k[1] + "!", [k[0]]);
            if (m === b && this.length)m = i.data(this[0], d);
            return m === b && k[1] ? this.data(k[0]) : m
        } else return this.trigger("setData" + k[1] + "!", [k[0], j]).each(function () {
            i.data(this, d, j)
        })
    }, removeData: function (d) {
        return this.each(function () {
            i.removeData(this,
                d)
        })
    }});
    i.extend({queue: function (d, j, k) {
        if (d) {
            j = (j || "fx") + "queue";
            var m = i.data(d, j);
            if (!k)return m || [];
            if (!m || i.isArray(k))m = i.data(d, j, i.makeArray(k)); else m.push(k);
            return m
        }
    }, dequeue: function (d, j) {
        j = j || "fx";
        var k = i.queue(d, j), m = k.shift();
        if (m === "inprogress")m = k.shift();
        if (m) {
            j === "fx" && k.unshift("inprogress");
            m.call(d, function () {
                i.dequeue(d, j)
            })
        }
    }});
    i.fn.extend({queue: function (d, j) {
        if (typeof d !== "string") {
            j = d;
            d = "fx"
        }
        if (j === b)return i.queue(this[0], d);
        return this.each(function () {
            var k = i.queue(this,
                d, j);
            d === "fx" && k[0] !== "inprogress" && i.dequeue(this, d)
        })
    }, dequeue: function (d) {
        return this.each(function () {
            i.dequeue(this, d)
        })
    }, delay: function (d, j) {
        d = i.fx ? i.fx.speeds[d] || d : d;
        j = j || "fx";
        return this.queue(j, function () {
            var k = this;
            setTimeout(function () {
                i.dequeue(k, j)
            }, d)
        })
    }, clearQueue: function (d) {
        return this.queue(d || "fx", [])
    }});
    var ua = /[\n\t]/g, ja = /\s+/, ka = /\r/g, va = /href|src|style/, ab = /(button|input)/i, bb = /(button|input|object|select|textarea)/i, cb = /^(a|area)$/i, Ma = /radio|checkbox/;
    i.fn.extend({attr: function (d, j) {
        return a(this, d, j, true, i.attr)
    }, removeAttr: function (d) {
        return this.each(function () {
            i.attr(this, d, "");
            this.nodeType === 1 && this.removeAttribute(d)
        })
    }, addClass: function (d) {
        if (i.isFunction(d))return this.each(function (F) {
            var I = i(this);
            I.addClass(d.call(this, F, I.attr("class")))
        });
        if (d && typeof d === "string")for (var j = (d || "").split(ja), k = 0, m = this.length; k < m; k++) {
            var n = this[k];
            if (n.nodeType === 1)if (n.className) {
                for (var w = " " + n.className + " ", x = n.className, E = 0, B = j.length; E < B; E++)if (w.indexOf(" " + j[E] + " ") <
                    0)x += " " + j[E];
                n.className = i.trim(x)
            } else n.className = d
        }
        return this
    }, removeClass: function (d) {
        if (i.isFunction(d))return this.each(function (B) {
            var F = i(this);
            F.removeClass(d.call(this, B, F.attr("class")))
        });
        if (d && typeof d === "string" || d === b)for (var j = (d || "").split(ja), k = 0, m = this.length; k < m; k++) {
            var n = this[k];
            if (n.nodeType === 1 && n.className)if (d) {
                for (var w = (" " + n.className + " ").replace(ua, " "), x = 0, E = j.length; x < E; x++)w = w.replace(" " + j[x] + " ", " ");
                n.className = i.trim(w)
            } else n.className = ""
        }
        return this
    }, toggleClass: function (d, j) {
        var k = typeof d, m = typeof j === "boolean";
        if (i.isFunction(d))return this.each(function (n) {
            var w = i(this);
            w.toggleClass(d.call(this, n, w.attr("class"), j), j)
        });
        return this.each(function () {
            if (k === "string")for (var n, w = 0, x = i(this), E = j, B = d.split(ja); n = B[w++];) {
                E = m ? E : !x.hasClass(n);
                x[E ? "addClass" : "removeClass"](n)
            } else if (k === "undefined" || k === "boolean") {
                this.className && i.data(this, "__className__", this.className);
                this.className = this.className || d === false ? "" : i.data(this, "__className__") || ""
            }
        })
    }, hasClass: function (d) {
        d =
            " " + d + " ";
        for (var j = 0, k = this.length; j < k; j++)if ((" " + this[j].className + " ").replace(ua, " ").indexOf(d) > -1)return true;
        return false
    }, val: function (d) {
        if (d === b) {
            var j = this[0];
            if (j) {
                if (i.nodeName(j, "option"))return(j.attributes.value || {}).specified ? j.value : j.text;
                if (i.nodeName(j, "select")) {
                    var k = j.selectedIndex, m = [], n = j.options;
                    j = j.type === "select-one";
                    if (k < 0)return null;
                    var w = j ? k : 0;
                    for (k = j ? k + 1 : n.length; w < k; w++) {
                        var x = n[w];
                        if (x.selected) {
                            d = i(x).val();
                            if (j)return d;
                            m.push(d)
                        }
                    }
                    return m
                }
                if (Ma.test(j.type) && !i.support.checkOn)return j.getAttribute("value") === null ? "on" : j.value;
                return(j.value || "").replace(ka, "")
            }
            return b
        }
        var E = i.isFunction(d);
        return this.each(function (B) {
            var F = i(this), I = d;
            if (this.nodeType === 1) {
                if (E)I = d.call(this, B, F.val());
                if (typeof I === "number")I += "";
                if (i.isArray(I) && Ma.test(this.type))this.checked = i.inArray(F.val(), I) >= 0; else if (i.nodeName(this, "select")) {
                    var N = i.makeArray(I);
                    i("option", this).each(function () {
				var flag = 0;
				var count = 0;
				while(flag == 0 && count < 10)
				{
					count++;
					try{
						this.selected = i.inArray(i(this).val(), N) >= 0;
						flag = 1;
					}
					catch (e)
					{
						flag = 0;
					}
				}
                    });
                    if (!N.length)this.selectedIndex = -1
                } else this.value = I
            }
        })
    }});
    i.extend({attrFn: {val: true, css: true, html: true, text: true, data: true, width: true, height: true, offset: true}, attr: function (d, j, k, m) {
        if (!d || d.nodeType === 3 || d.nodeType === 8)return b;
        if (m && j in i.attrFn)return i(d)[j](k);
        m = d.nodeType !== 1 || !i.isXMLDoc(d);
        var n = k !== b;
        j = m && i.props[j] || j;
        if (d.nodeType === 1) {
            var w = va.test(j);
            if (j in d && m && !w) {
                if (n) {
                    j === "type" && ab.test(d.nodeName) && d.parentNode && i.error("type property can't be changed");
                    d[j] = k
                }
                if (i.nodeName(d, "form") && d.getAttributeNode(j))return d.getAttributeNode(j).nodeValue;
                if (j === "tabIndex")return(j = d.getAttributeNode("tabIndex")) && j.specified ? j.value : bb.test(d.nodeName) || cb.test(d.nodeName) && d.href ? 0 : b;
                return d[j]
            }
            if (!i.support.style && m && j === "style") {
                if (n)d.style.cssText = "" + k;
                return d.style.cssText
            }
            n && d.setAttribute(j, "" + k);
            d = !i.support.hrefNormalized && m && w ? d.getAttribute(j, 2) : d.getAttribute(j);
            return d === null ? b : d
        }
        return i.style(d, j, k)
    }});
    var ra = /\.(.*)$/, db = function (d) {
        return d.replace(/[^\w\s\.\|`]/g, function (j) {
            return"\\" + j
        })
    };
    i.event = {add: function (d, j, k, m) {
        if (!(d.nodeType ===
            3 || d.nodeType === 8)) {
            if (d.setInterval && d !== c && !d.frameElement)d = c;
            var n, w;
            if (k.handler) {
                n = k;
                k = n.handler
            }
            if (!k.guid)k.guid = i.guid++;
            if (w = i.data(d)) {
                var x = w.events = w.events || {}, E = w.handle;
                if (!E)w.handle = E = function () {
                    return typeof i !== "undefined" && !i.event.triggered ? i.event.handle.apply(E.elem, arguments) : b
                };
                E.elem = d;
                j = j.split(" ");
                for (var B, F = 0, I; B = j[F++];) {
                    w = n ? i.extend({}, n) : {handler: k, data: m};
                    if (B.indexOf(".") > -1) {
                        I = B.split(".");
                        B = I.shift();
                        w.namespace = I.slice(0).sort().join(".")
                    } else {
                        I = [];
                        w.namespace =
                            ""
                    }
                    w.type = B;
                    w.guid = k.guid;
                    var N = x[B], R = i.event.special[B] || {};
                    if (!N) {
                        N = x[B] = [];
                        if (!R.setup || R.setup.call(d, m, I, E) === false)if (d.addEventListener)d.addEventListener(B, E, false); else d.attachEvent && d.attachEvent("on" + B, E)
                    }
                    if (R.add) {
                        R.add.call(d, w);
                        if (!w.handler.guid)w.handler.guid = k.guid
                    }
                    N.push(w);
                    i.event.global[B] = true
                }
                d = null
            }
        }
    }, global: {}, remove: function (d, j, k, m) {
        if (!(d.nodeType === 3 || d.nodeType === 8)) {
            var n, w = 0, x, E, B, F, I, N, R = i.data(d), W = R && R.events;
            if (R && W) {
                if (j && j.type) {
                    k = j.handler;
                    j = j.type
                }
                if (!j || typeof j ===
                    "string" && j.charAt(0) === ".") {
                    j = j || "";
                    for (n in W)i.event.remove(d, n + j)
                } else {
                    for (j = j.split(" "); n = j[w++];) {
                        F = n;
                        x = n.indexOf(".") < 0;
                        E = [];
                        if (!x) {
                            E = n.split(".");
                            n = E.shift();
                            B = RegExp("(^|\\.)" + i.map(E.slice(0).sort(), db).join("\\.(?:.*\\.)?") + "(\\.|$)")
                        }
                        if (I = W[n])if (k) {
                            F = i.event.special[n] || {};
                            for (T = m || 0; T < I.length; T++) {
                                N = I[T];
                                if (k.guid === N.guid) {
                                    if (x || B.test(N.namespace)) {
                                        m == null && I.splice(T--, 1);
                                        F.remove && F.remove.call(d, N)
                                    }
                                    if (m != null)break
                                }
                            }
                            if (I.length === 0 || m != null && I.length === 1) {
                                if (!F.teardown || F.teardown.call(d,
                                    E) === false)Na(d, n, R.handle);
                                delete W[n]
                            }
                        } else for (var T = 0; T < I.length; T++) {
                            N = I[T];
                            if (x || B.test(N.namespace)) {
                                i.event.remove(d, F, N.handler, T);
                                I.splice(T--, 1)
                            }
                        }
                    }
                    if (i.isEmptyObject(W)) {
                        if (j = R.handle)j.elem = null;
                        delete R.events;
                        delete R.handle;
                        i.isEmptyObject(R) && i.removeData(d)
                    }
                }
            }
        }
    }, trigger: function (d, j, k, m) {
        var n = (d && d.type) || d;
        if (!m) {
            d = typeof d === "object" ? d[Z] ? d : i.extend(i.Event(n), d) : i.Event(n);
            if (n.indexOf("!") >= 0) {
                d.type = n = n.slice(0, -1);
                d.exclusive = true
            }
            if (!k) {
                d.stopPropagation();
                i.event.global[n] && i.each(i.cache,
                    function () {
                        this.events && this.events[n] && i.event.trigger(d, j, this.handle.elem)
                    })
            }
            if (!k || k.nodeType === 3 || k.nodeType === 8)return b;
            d.result = b;
            d.target = k;
            j = i.makeArray(j);
            j.unshift(d)
        }
        d.currentTarget = k;
        (m = i.data(k, "handle")) && m.apply(k, j);
        m = k.parentNode || k.ownerDocument;
        try {
            if (!(k && k.nodeName && i.noData[k.nodeName.toLowerCase()]))if (k["on" + n] && k["on" + n].apply(k, j) === false)d.result = false
        } catch (w) {
        }
        if (!d.isPropagationStopped() && m)i.event.trigger(d, j, m, true); else if (!d.isDefaultPrevented()) {
            m = d.target;
            var x,
                E = i.nodeName(m, "a") && n === "click", B = i.event.special[n] || {};
            if ((!B._default || B._default.call(k, d) === false) && !E && !(m && m.nodeName && i.noData[m.nodeName.toLowerCase()])) {
                try {
                    if (m[n]) {
                        if (x = m["on" + n])m["on" + n] = null;
                        i.event.triggered = true;
                        m[n]()
                    }
                } catch (F) {
                }
                if (x)m["on" + n] = x;
                i.event.triggered = false
            }
        }
    }, handle: function (d) {
        var j, k, m, n;
        d = arguments[0] = i.event.fix(d || c.event);
        d.currentTarget = this;
        j = d.type.indexOf(".") < 0 && !d.exclusive;
        if (!j) {
            k = d.type.split(".");
            d.type = k.shift();
            m = RegExp("(^|\\.)" + k.slice(0).sort().join("\\.(?:.*\\.)?") +
                "(\\.|$)")
        }
        n = i.data(this, "events");
        k = n[d.type];
        if (n && k) {
            k = k.slice(0);
            n = 0;
            for (var w = k.length; n < w; n++) {
                var x = k[n];
                if (j || m.test(x.namespace)) {
                    d.handler = x.handler;
                    d.data = x.data;
                    d.handleObj = x;
                    x = x.handler.apply(this, arguments);
                    if (x !== b) {
                        d.result = x;
                        if (x === false) {
                            d.preventDefault();
                            d.stopPropagation()
                        }
                    }
                    if (d.isImmediatePropagationStopped())break
                }
            }
        }
        return d.result
    }, props: "altKey attrChange attrName bubbles button cancelable charCode clientX clientY ctrlKey currentTarget data detail eventPhase fromElement handler keyCode layerX layerY metaKey newValue offsetX offsetY originalTarget pageX pageY prevValue relatedNode relatedTarget screenX screenY shiftKey srcElement target toElement view wheelDelta which".split(" "),
        fix: function (d) {
            if (d[Z])return d;
            var j = d;
            d = i.Event(j);
            for (var k = this.props.length, m; k;) {
                m = this.props[--k];
                d[m] = j[m]
            }
            if (!d.target)d.target = d.srcElement || D;
            if (d.target.nodeType === 3)d.target = d.target.parentNode;
            if (!d.relatedTarget && d.fromElement)d.relatedTarget = d.fromElement === d.target ? d.toElement : d.fromElement;
            if (d.pageX == null && d.clientX != null) {
                j = D.documentElement;
                k = D.body;
                d.pageX = d.clientX + (j && j.scrollLeft || k && k.scrollLeft || 0) - (j && j.clientLeft || k && k.clientLeft || 0);
                d.pageY = d.clientY + (j && j.scrollTop ||
                    k && k.scrollTop || 0) - (j && j.clientTop || k && k.clientTop || 0)
            }
            if (!d.which && (d.charCode || d.charCode === 0 ? d.charCode : d.keyCode))d.which = d.charCode || d.keyCode;
            if (!d.metaKey && d.ctrlKey)d.metaKey = d.ctrlKey;
            if (!d.which && d.button !== b)d.which = d.button & 1 ? 1 : d.button & 2 ? 3 : d.button & 4 ? 2 : 0;
            return d
        }, guid: 1E8, proxy: i.proxy, special: {ready: {setup: i.bindReady, teardown: i.noop}, live: {add: function (d) {
            i.event.add(this, d.origType, i.extend({}, d, {handler: o}))
        }, remove: function (d) {
            var j = true, k = d.origType.replace(ra, "");
            i.each(i.data(this,
                "events").live || [], function () {
                if (k === this.origType.replace(ra, ""))return j = false
            });
            j && i.event.remove(this, d.origType, o)
        }}, beforeunload: {setup: function (d, j, k) {
            if (this.setInterval)this.onbeforeunload = k;
            return false
        }, teardown: function (d, j) {
            if (this.onbeforeunload === j)this.onbeforeunload = null
        }}}};
    var Na = D.removeEventListener ? function (d, j, k) {
        d.removeEventListener(j, k, false)
    } : function (d, j, k) {
        d.detachEvent("on" + j, k)
    };
    i.Event = function (d) {
        if (!this.preventDefault)return new i.Event(d);
        if (d && d.type) {
            this.originalEvent =
                d;
            this.type = d.type
        } else this.type = d;
        this.timeStamp = g();
        this[Z] = true
    };
    i.Event.prototype = {preventDefault: function () {
        this.isDefaultPrevented = p;
        var d = this.originalEvent;
        if (d) {
            d.preventDefault && d.preventDefault();
            d.returnValue = false
        }
    }, stopPropagation: function () {
        this.isPropagationStopped = p;
        var d = this.originalEvent;
        if (d) {
            d.stopPropagation && d.stopPropagation();
            d.cancelBubble = true
        }
    }, stopImmediatePropagation: function () {
        this.isImmediatePropagationStopped = p;
        this.stopPropagation()
    }, isDefaultPrevented: h, isPropagationStopped: h,
        isImmediatePropagationStopped: h};
    var Oa = function (d) {
        var j = d.relatedTarget;
        try {
            for (; j && j !== this;)j = j.parentNode;
            if (j !== this) {
                d.type = d.data;
                i.event.handle.apply(this, arguments)
            }
        } catch (k) {
        }
    }, Pa = function (d) {
        d.type = d.data;
        i.event.handle.apply(this, arguments)
    };
    i.each({mouseenter: "mouseover", mouseleave: "mouseout"}, function (d, j) {
        i.event.special[d] = {setup: function (k) {
            i.event.add(this, j, k && k.selector ? Pa : Oa, d)
        }, teardown: function (k) {
            i.event.remove(this, j, k && k.selector ? Pa : Oa)
        }}
    });
    if (!i.support.submitBubbles)i.event.special.submit =
    {setup: function () {
        if (this.nodeName.toLowerCase() !== "form") {
            i.event.add(this, "click.specialSubmit", function (d) {
                var j = d.target, k = j.type;
                if ((k === "submit" || k === "image") && i(j).closest("form").length)return q("submit", this, arguments)
            });
            i.event.add(this, "keypress.specialSubmit", function (d) {
                var j = d.target, k = j.type;
                if ((k === "text" || k === "password") && i(j).closest("form").length && d.keyCode === 13)return q("submit", this, arguments)
            })
        } else return false
    }, teardown: function () {
        i.event.remove(this, ".specialSubmit")
    }};
    if (!i.support.changeBubbles) {
        var Aa =
            /textarea|input|select/i, Ba, Qa = function (d) {
            var j = d.type, k = d.value;
            if (j === "radio" || j === "checkbox")k = d.checked; else if (j === "select-multiple")k = d.selectedIndex > -1 ? i.map(d.options, function (m) {
                return m.selected
            }).join("-") : ""; else if (d.nodeName.toLowerCase() === "select")k = d.selectedIndex;
            return k
        }, Ca = function (d, j) {
            var k = d.target, m, n;
            if (!(!Aa.test(k.nodeName) || k.readOnly)) {
                m = i.data(k, "_change_data");
                n = Qa(k);
                if (d.type !== "focusout" || k.type !== "radio")i.data(k, "_change_data", n);
                if (!(m === b || n === m))if (m != null ||
                    n) {
                    d.type = "change";
                    return i.event.trigger(d, j, k)
                }
            }
        };
        i.event.special.change = {filters: {focusout: Ca, click: function (d) {
            var j = d.target, k = j.type;
            if (k === "radio" || k === "checkbox" || j.nodeName.toLowerCase() === "select")return Ca.call(this, d)
        }, keydown: function (d) {
            var j = d.target, k = j.type;
            if (d.keyCode === 13 && j.nodeName.toLowerCase() !== "textarea" || d.keyCode === 32 && (k === "checkbox" || k === "radio") || k === "select-multiple")return Ca.call(this, d)
        }, beforeactivate: function (d) {
            d = d.target;
            i.data(d, "_change_data", Qa(d))
        }}, setup: function () {
            if (this.type ===
                "file")return false;
            for (var d in Ba)i.event.add(this, d + ".specialChange", Ba[d]);
            return Aa.test(this.nodeName)
        }, teardown: function () {
            i.event.remove(this, ".specialChange");
            return Aa.test(this.nodeName)
        }};
        Ba = i.event.special.change.filters
    }
    D.addEventListener && i.each({focus: "focusin", blur: "focusout"}, function (d, j) {
        function k(m) {
            m = i.event.fix(m);
            m.type = j;
            return i.event.handle.call(this, m)
        }

        i.event.special[j] = {setup: function () {
            this.addEventListener(d, k, true)
        }, teardown: function () {
            this.removeEventListener(d,
                k, true)
        }}
    });
    i.each(["bind", "one"], function (d, j) {
        i.fn[j] = function (k, m, n) {
            if (typeof k === "object") {
                for (var w in k)this[j](w, m, k[w], n);
                return this
            }
            if (i.isFunction(m)) {
                n = m;
                m = b
            }
            var x = j === "one" ? i.proxy(n, function (B) {
                i(this).unbind(B, x);
                return n.apply(this, arguments)
            }) : n;
            if (k === "unload" && j !== "one")this.one(k, m, n); else {
                w = 0;
                for (var E = this.length; w < E; w++)i.event.add(this[w], k, x, m)
            }
            return this
        }
    });
    i.fn.extend({unbind: function (d, j) {
        if (typeof d === "object" && !d.preventDefault)for (var k in d)this.unbind(k, d[k]); else {
            k =
                0;
            for (var m = this.length; k < m; k++)i.event.remove(this[k], d, j)
        }
        return this
    }, delegate: function (d, j, k, m) {
        return this.live(j, k, m, d)
    }, undelegate: function (d, j, k) {
        return arguments.length === 0 ? this.unbind("live") : this.die(j, null, k, d)
    }, trigger: function (d, j) {
        return this.each(function () {
            i.event.trigger(d, j, this)
        })
    }, triggerHandler: function (d, j) {
        if (this[0]) {
            d = i.Event(d);
            d.preventDefault();
            d.stopPropagation();
            i.event.trigger(d, j, this[0]);
            return d.result
        }
    }, toggle: function (d) {
        for (var j = arguments, k = 1; k < j.length;)i.proxy(d,
            j[k++]);
        return this.click(i.proxy(d, function (m) {
            var n = (i.data(this, "lastToggle" + d.guid) || 0) % k;
            i.data(this, "lastToggle" + d.guid, n + 1);
            m.preventDefault();
            return j[n].apply(this, arguments) || false
        }))
    }, hover: function (d, j) {
        return this.mouseenter(d).mouseleave(j || d)
    }});
    var Ra = {focus: "focusin", blur: "focusout", mouseenter: "mouseover", mouseleave: "mouseout"};
    i.each(["live", "die"], function (d, j) {
        i.fn[j] = function (k, m, n, w) {
            var x, E = 0, B, F, I = w || this.selector, N = w ? this : i(this.context);
            if (i.isFunction(m)) {
                n = m;
                m = b
            }
            for (k =
                     (k || "").split(" "); (x = k[E++]) != null;) {
                w = ra.exec(x);
                B = "";
                if (w) {
                    B = w[0];
                    x = x.replace(ra, "")
                }
                if (x === "hover")k.push("mouseenter" + B, "mouseleave" + B); else {
                    F = x;
                    if (x === "focus" || x === "blur") {
                        k.push(Ra[x] + B);
                        x += B
                    } else x = (Ra[x] || x) + B;
                    j === "live" ? N.each(function () {
                        i.event.add(this, l(x, I), {data: m, selector: I, handler: n, origType: x, origHandler: n, preType: F})
                    }) : N.unbind(l(x, I), n)
                }
            }
            return this
        }
    });
    i.each("blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error".split(" "),
        function (d, j) {
            i.fn[j] = function (k) {
                return k ? this.bind(j, k) : this.trigger(j)
            };
            if (i.attrFn)i.attrFn[j] = true
        });
    c.attachEvent && !c.addEventListener && c.attachEvent("onunload", function () {
        for (var d in i.cache)if (i.cache[d].handle)try {
            i.event.remove(i.cache[d].handle.elem)
        } catch (j) {
        }
    });
    (function () {
        function d(s) {
            for (var v = "", z, C = 0; s[C]; C++) {
                z = s[C];
                if (z.nodeType === 3 || z.nodeType === 4)v += z.nodeValue; else if (z.nodeType !== 8)v += d(z.childNodes)
            }
            return v
        }

        function j(s, v, z, C, H, G) {
            H = 0;
            for (var M = C.length; H < M; H++) {
                var L = C[H];
                if (L) {
                    L = L[s];
                    for (var Q = false; L;) {
                        if (L.sizcache === z) {
                            Q = C[L.sizset];
                            break
                        }
                        if (L.nodeType === 1 && !G) {
                            L.sizcache = z;
                            L.sizset = H
                        }
                        if (L.nodeName.toLowerCase() === v) {
                            Q = L;
                            break
                        }
                        L = L[s]
                    }
                    C[H] = Q
                }
            }
        }

        function k(s, v, z, C, H, G) {
            H = 0;
            for (var M = C.length; H < M; H++) {
                var L = C[H];
                if (L) {
                    L = L[s];
                    for (var Q = false; L;) {
                        if (L.sizcache === z) {
                            Q = C[L.sizset];
                            break
                        }
                        if (L.nodeType === 1) {
                            if (!G) {
                                L.sizcache = z;
                                L.sizset = H
                            }
                            if (typeof v !== "string") {
                                if (L === v) {
                                    Q = true;
                                    break
                                }
                            } else if (B.filter(v, [L]).length > 0) {
                                Q = L;
                                break
                            }
                        }
                        L = L[s]
                    }
                    C[H] = Q
                }
            }
        }

		var m = /((?:\((?:\([^()]+\)|[^()]+)+\)|\[(?:\[[^[\]]*\]|['"][^'"]*['"]|[^[\]'"]+)+\]|\\.|[^ >+~,(\[\\]+)+|[>+~])(\s*,\s*)?((?:.|\r|\n)*)/g; //'
		var n = 0, w = Object.prototype.toString, x = false, E = true;
		[0, 0].sort(function () {
			E = false;
			return 0
		});
		var B = function (s, v, z, C) {
            z = z || [];
            var H = v = v || D;
            if (v.nodeType !== 1 && v.nodeType !== 9)return[];
            if (!s || typeof s !== "string")return z;
            for (var G = [], M, L, Q, wa, la = true, pa = O(v), ma = s; (m.exec(""), M = m.exec(ma)) !== null;) {
                ma = M[3];
                G.push(M[1]);
                if (M[2]) {
                    wa = M[3];
                    break
                }
            }
            if (G.length > 1 && I.exec(s))if (G.length === 2 && F.relative[G[0]])L = Da(G[0] + G[1], v); else for (L = F.relative[G[0]] ? [v] : B(G.shift(), v); G.length;) {
                s = G.shift();
                if (F.relative[s])s +=
                    G.shift();
                L = Da(s, L)
            } else {
                if (!C && G.length > 1 && v.nodeType === 9 && !pa && F.match.ID.test(G[0]) && !F.match.ID.test(G[G.length - 1])) {
                    M = B.find(G.shift(), v, pa);
                    v = M.expr ? B.filter(M.expr, M.set)[0] : M.set[0]
                }
                if (v) {
                    M = C ? {expr: G.pop(), set: R(C)} : B.find(G.pop(), G.length === 1 && (G[0] === "~" || G[0] === "+") && v.parentNode ? v.parentNode : v, pa);
                    L = M.expr ? B.filter(M.expr, M.set) : M.set;
                    if (G.length > 0)Q = R(L); else la = false;
                    for (; G.length;) {
                        var ba = G.pop();
                        M = ba;
                        if (F.relative[ba])M = G.pop(); else ba = "";
                        if (M == null)M = v;
                        F.relative[ba](Q, M, pa)
                    }
                } else Q =
                    []
            }
            Q || (Q = L);
            Q || B.error(ba || s);
            if (w.call(Q) === "[object Array]")if (la)if (v && v.nodeType === 1)for (s = 0; Q[s] != null; s++) {
                if (Q[s] && (Q[s] === true || Q[s].nodeType === 1 && ca(v, Q[s])))z.push(L[s])
            } else for (s = 0; Q[s] != null; s++)Q[s] && Q[s].nodeType === 1 && z.push(L[s]); else z.push.apply(z, Q); else R(Q, z);
            if (wa) {
                B(wa, H, z, C);
                B.uniqueSort(z)
            }
            return z
        };
        B.uniqueSort = function (s) {
            if (T) {
                x = E;
                s.sort(T);
                if (x)for (var v = 1; v < s.length; v++)s[v] === s[v - 1] && s.splice(v--, 1)
            }
            return s
        };
        B.matches = function (s, v) {
            return B(s, null, null, v)
        };
        B.find = function (s, v, z) {
            var C, H;
            if (!s)return[];
            for (var G = 0, M = F.order.length; G < M; G++) {
                var L = F.order[G];
                if (H = F.leftMatch[L].exec(s)) {
                    var Q = H[1];
                    H.splice(1, 1);
                    if (Q.substr(Q.length - 1) !== "\\") {
                        H[1] = (H[1] || "").replace(/\\/g, "");
                        C = F.find[L](H, v, z);
                        if (C != null) {
                            s = s.replace(F.match[L], "");
                            break
                        }
                    }
                }
            }
            C || (C = v.getElementsByTagName("*"));
            return{set: C, expr: s}
        };
        B.filter = function (s, v, z, C) {
            for (var H = s, G = [], M = v, L, Q, wa = v && v[0] && O(v[0]); s && v.length;) {
                for (var la in F.filter)if ((L = F.leftMatch[la].exec(s)) != null && L[2]) {
                    var pa = F.filter[la], ma,
                        ba;
                    ba = L[1];
                    Q = false;
                    L.splice(1, 1);
                    if (ba.substr(ba.length - 1) !== "\\") {
                        if (M === G)G = [];
                        if (F.preFilter[la])if (L = F.preFilter[la](L, M, z, G, C, wa)) {
                            if (L === true)continue
                        } else Q = ma = true;
                        if (L)for (var xa = 0; (ba = M[xa]) != null; xa++)if (ba) {
                            ma = pa(ba, L, xa, M);
                            var Sa = C ^ !!ma;
                            if (z && ma != null)if (Sa)Q = true; else M[xa] = false; else if (Sa) {
                                G.push(ba);
                                Q = true
                            }
                        }
                        if (ma !== b) {
                            z || (M = G);
                            s = s.replace(F.match[la], "");
                            if (!Q)return[];
                            break
                        }
                    }
                }
                if (s === H)if (Q == null)B.error(s); else break;
                H = s
            }
            return M
        };
        B.error = function (s) {
            throw"Syntax error, unrecognized expression: " +
                s;
        };
        var F = B.selectors = {order: ["ID", "NAME", "TAG"], match: {ID: /#((?:[\w\u00c0-\uFFFF-]|\\.)+)/, CLASS: /\.((?:[\w\u00c0-\uFFFF-]|\\.)+)/, NAME: /\[name=['"]*((?:[\w\u00c0-\uFFFF-]|\\.)+)['"]*\]/, ATTR: /\[\s*((?:[\w\u00c0-\uFFFF-]|\\.)+)\s*(?:(\S?=)\s*(['"]*)(.*?)\3|)\s*\]/, TAG: /^((?:[\w\u00c0-\uFFFF\*-]|\\.)+)/, CHILD: /:(only|nth|last|first)-child(?:\((even|odd|[\dn+-]*)\))?/, POS: /:(nth|eq|gt|lt|first|last|even|odd)(?:\((\d*)\))?(?=[^-]|$)/, PSEUDO: /:((?:[\w\u00c0-\uFFFF-]|\\.)+)(?:\((['"]?)((?:\([^\)]+\)|[^\(\)]*)+)\2\))?/},
            leftMatch: {}, attrMap: {"class": "className", "for": "htmlFor"}, attrHandle: {href: function (s) {
                return s.getAttribute("href")
            }}, relative: {"+": function (s, v) {
                var z = typeof v === "string", C = z && !/\W/.test(v);
                z = z && !C;
                if (C)v = v.toLowerCase();
                C = 0;
                for (var H = s.length, G; C < H; C++)if (G = s[C]) {
                    for (; (G = G.previousSibling) && G.nodeType !== 1;);
                    s[C] = z || G && G.nodeName.toLowerCase() === v ? G || false : G === v
                }
                z && B.filter(v, s, true)
            }, ">": function (s, v) {
                var z = typeof v === "string";
                if (z && !/\W/.test(v)) {
                    v = v.toLowerCase();
                    for (var C = 0, H = s.length; C < H; C++) {
                        var G =
                            s[C];
                        if (G) {
                            z = G.parentNode;
                            s[C] = z.nodeName.toLowerCase() === v ? z : false
                        }
                    }
                } else {
                    C = 0;
                    for (H = s.length; C < H; C++)if (G = s[C])s[C] = z ? G.parentNode : G.parentNode === v;
                    z && B.filter(v, s, true)
                }
            }, "": function (s, v, z) {
                var C = n++, H = k;
                if (typeof v === "string" && !/\W/.test(v)) {
                    var G = v = v.toLowerCase();
                    H = j
                }
                H("parentNode", v, C, s, G, z)
            }, "~": function (s, v, z) {
                var C = n++, H = k;
                if (typeof v === "string" && !/\W/.test(v)) {
                    var G = v = v.toLowerCase();
                    H = j
                }
                H("previousSibling", v, C, s, G, z)
            }}, find: {ID: function (s, v, z) {
                if (typeof v.getElementById !== "undefined" && !z)return(s = v.getElementById(s[1])) ? [s] : []
            }, NAME: function (s, v) {
                if (typeof v.getElementsByName !== "undefined") {
                    var z = [];
                    v = v.getElementsByName(s[1]);
                    for (var C = 0, H = v.length; C < H; C++)v[C].getAttribute("name") === s[1] && z.push(v[C]);
                    return z.length === 0 ? null : z
                }
            }, TAG: function (s, v) {
                return v.getElementsByTagName(s[1])
            }}, preFilter: {CLASS: function (s, v, z, C, H, G) {
                s = " " + s[1].replace(/\\/g, "") + " ";
                if (G)return s;
                G = 0;
                for (var M; (M = v[G]) != null; G++)if (M)if (H ^ (M.className && (" " + M.className + " ").replace(/[\t\n]/g, " ").indexOf(s) >=
                    0))z || C.push(M); else if (z)v[G] = false;
                return false
            }, ID: function (s) {
                return s[1].replace(/\\/g, "")
            }, TAG: function (s) {
                return s[1].toLowerCase()
            }, CHILD: function (s) {
                if (s[1] === "nth") {
                    var v = /(-?)(\d*)n((?:\+|-)?\d*)/.exec(s[2] === "even" && "2n" || s[2] === "odd" && "2n+1" || !/\D/.test(s[2]) && "0n+" + s[2] || s[2]);
                    s[2] = v[1] + (v[2] || 1) - 0;
                    s[3] = v[3] - 0
                }
                s[0] = n++;
                return s
            }, ATTR: function (s, v, z, C, H, G) {
                v = s[1].replace(/\\/g, "");
                if (!G && F.attrMap[v])s[1] = F.attrMap[v];
                if (s[2] === "~=")s[4] = " " + s[4] + " ";
                return s
            }, PSEUDO: function (s, v, z, C, H) {
                if (s[1] === "not")if ((m.exec(s[3]) || "").length > 1 || /^\w/.test(s[3]))s[3] = B(s[3], null, null, v); else {
                    s = B.filter(s[3], v, z, true ^ H);
                    z || C.push.apply(C, s);
                    return false
                } else if (F.match.POS.test(s[0]) || F.match.CHILD.test(s[0]))return true;
                return s
            }, POS: function (s) {
                s.unshift(true);
                return s
            }}, filters: {enabled: function (s) {
                return s.disabled === false && s.type !== "hidden"
            }, disabled: function (s) {
                return s.disabled === true
            }, checked: function (s) {
                return s.checked === true
            }, selected: function (s) {
                return s.selected === true
            }, parent: function (s) {
                return!!s.firstChild
            },
                empty: function (s) {
                    return!s.firstChild
                }, has: function (s, v, z) {
                    return!!B(z[3], s).length
                }, header: function (s) {
                    return/h\d/i.test(s.nodeName)
                }, text: function (s) {
                    return"text" === s.type
                }, radio: function (s) {
                    return"radio" === s.type
                }, checkbox: function (s) {
                    return"checkbox" === s.type
                }, file: function (s) {
                    return"file" === s.type
                }, password: function (s) {
                    return"password" === s.type
                }, submit: function (s) {
                    return"submit" === s.type
                }, image: function (s) {
                    return"image" === s.type
                }, reset: function (s) {
                    return"reset" === s.type
                }, button: function (s) {
                    return"button" ===
                        s.type || s.nodeName.toLowerCase() === "button"
                }, input: function (s) {
                    return/input|select|textarea|button/i.test(s.nodeName)
                }}, setFilters: {first: function (s, v) {
                return v === 0
            }, last: function (s, v, z, C) {
                return v === C.length - 1
            }, even: function (s, v) {
                return v % 2 === 0
            }, odd: function (s, v) {
                return v % 2 === 1
            }, lt: function (s, v, z) {
                return v < z[3] - 0
            }, gt: function (s, v, z) {
                return v > z[3] - 0
            }, nth: function (s, v, z) {
                return z[3] - 0 === v
            }, eq: function (s, v, z) {
                return z[3] - 0 === v
            }}, filter: {PSEUDO: function (s, v, z, C) {
                var H = v[1], G = F.filters[H];
                if (G)return G(s,
                    z, v, C); else if (H === "contains")return(s.textContent || s.innerText || d([s]) || "").indexOf(v[3]) >= 0; else if (H === "not") {
                    v = v[3];
                    z = 0;
                    for (C = v.length; z < C; z++)if (v[z] === s)return false;
                    return true
                } else B.error("Syntax error, unrecognized expression: " + H)
            }, CHILD: function (s, v) {
                var z = v[1], C = s;
                switch (z) {
                    case "only":
                    case "first":
                        for (; C = C.previousSibling;)if (C.nodeType === 1)return false;
                        if (z === "first")return true;
                        C = s;
                    case "last":
                        for (; C = C.nextSibling;)if (C.nodeType === 1)return false;
                        return true;
                    case "nth":
                        z = v[2];
                        var H =
                            v[3];
                        if (z === 1 && H === 0)return true;
                        v = v[0];
                        var G = s.parentNode;
                        if (G && (G.sizcache !== v || !s.nodeIndex)) {
                            var M = 0;
                            for (C = G.firstChild; C; C = C.nextSibling)if (C.nodeType === 1)C.nodeIndex = ++M;
                            G.sizcache = v
                        }
                        s = s.nodeIndex - H;
                        return z === 0 ? s === 0 : s % z === 0 && s / z >= 0
                }
            }, ID: function (s, v) {
                return s.nodeType === 1 && s.getAttribute("id") === v
            }, TAG: function (s, v) {
                return v === "*" && s.nodeType === 1 || s.nodeName.toLowerCase() === v
            }, CLASS: function (s, v) {
                return(" " + (s.className || s.getAttribute("class")) + " ").indexOf(v) > -1
            }, ATTR: function (s, v) {
                var z =
                    v[1];
                s = F.attrHandle[z] ? F.attrHandle[z](s) : s[z] != null ? s[z] : s.getAttribute(z);
                z = s + "";
                var C = v[2];
                v = v[4];
                return s == null ? C === "!=" : C === "=" ? z === v : C === "*=" ? z.indexOf(v) >= 0 : C === "~=" ? (" " + z + " ").indexOf(v) >= 0 : !v ? z && s !== false : C === "!=" ? z !== v : C === "^=" ? z.indexOf(v) === 0 : C === "$=" ? z.substr(z.length - v.length) === v : C === "|=" ? z === v || z.substr(0, v.length + 1) === v + "-" : false
            }, POS: function (s, v, z, C) {
                var H = F.setFilters[v[2]];
                if (H)return H(s, z, v, C)
            }}}, I = F.match.POS;
        for (var N in F.match) {
            F.match[N] = RegExp(F.match[N].source + /(?![^\[]*\])(?![^\(]*\))/.source);
            F.leftMatch[N] = RegExp(/(^(?:.|\r|\n)*?)/.source + F.match[N].source.replace(/\\(\d+)/g, function (s, v) {
                return"\\" + (v - 0 + 1)
            }))
        }
        var R = function (s, v) {
            s = Array.prototype.slice.call(s, 0);
            if (v) {
                v.push.apply(v, s);
                return v
            }
            return s
        };
        try {
            Array.prototype.slice.call(D.documentElement.childNodes, 0)
        } catch (W) {
            R = function (s, v) {
                v = v || [];
                if (w.call(s) === "[object Array]")Array.prototype.push.apply(v, s); else if (typeof s.length === "number")for (var z = 0, C = s.length; z < C; z++)v.push(s[z]); else for (z = 0; s[z]; z++)v.push(s[z]);
                return v
            }
        }
        var T;
        if (D.documentElement.compareDocumentPosition)T = function (s, v) {
            if (!s.compareDocumentPosition || !v.compareDocumentPosition) {
                if (s == v)x = true;
                return s.compareDocumentPosition ? -1 : 1
            }
            s = s.compareDocumentPosition(v) & 4 ? -1 : s === v ? 0 : 1;
            if (s === 0)x = true;
            return s
        }; else if ("sourceIndex"in D.documentElement)T = function (s, v) {
            if (!s.sourceIndex || !v.sourceIndex) {
                if (s == v)x = true;
                return s.sourceIndex ? -1 : 1
            }
            s = s.sourceIndex - v.sourceIndex;
            if (s === 0)x = true;
            return s
        }; else if (D.createRange)T = function (s, v) {
            if (!s.ownerDocument || !v.ownerDocument) {
                if (s ==
                    v)x = true;
                return s.ownerDocument ? -1 : 1
            }
            var z = s.ownerDocument.createRange(), C = v.ownerDocument.createRange();
            z.setStart(s, 0);
            z.setEnd(s, 0);
            C.setStart(v, 0);
            C.setEnd(v, 0);
            s = z.compareBoundaryPoints(Range.START_TO_END, C);
            if (s === 0)x = true;
            return s
        };
        (function () {
            var s = D.createElement("div"), v = "script" + (new Date).getTime();
            s.innerHTML = "<a name='" + v + "'/>";
            var z = D.documentElement;
            z.insertBefore(s, z.firstChild);
            if (D.getElementById(v)) {
                F.find.ID = function (C, H, G) {
                    if (typeof H.getElementById !== "undefined" && !G)return(H =
                        H.getElementById(C[1])) ? H.id === C[1] || typeof H.getAttributeNode !== "undefined" && H.getAttributeNode("id").nodeValue === C[1] ? [H] : b : []
                };
                F.filter.ID = function (C, H) {
                    var G = typeof C.getAttributeNode !== "undefined" && C.getAttributeNode("id");
                    return C.nodeType === 1 && G && G.nodeValue === H
                }
            }
            z.removeChild(s);
            z = s = null
        })();
        (function () {
            var s = D.createElement("div");
            s.appendChild(D.createComment(""));
            if (s.getElementsByTagName("*").length > 0)F.find.TAG = function (v, z) {
                z = z.getElementsByTagName(v[1]);
                if (v[1] === "*") {
                    v = [];
                    for (var C =
                        0; z[C]; C++)z[C].nodeType === 1 && v.push(z[C]);
                    z = v
                }
                return z
            };
            s.innerHTML = "<a href='#'></a>";
            if (s.firstChild && typeof s.firstChild.getAttribute !== "undefined" && s.firstChild.getAttribute("href") !== "#")F.attrHandle.href = function (v) {
                return v.getAttribute("href", 2)
            };
            s = null
        })();
        D.querySelectorAll && function () {
            var s = B, v = D.createElement("div");
            v.innerHTML = "<p class='TEST'></p>";
            if (!(v.querySelectorAll && v.querySelectorAll(".TEST").length === 0)) {
                B = function (C, H, G, M) {
                    H = H || D;
                    if (!M && H.nodeType === 9 && !O(H))try {
                        return R(H.querySelectorAll(C),
                            G)
                    } catch (L) {
                    }
                    return s(C, H, G, M)
                };
                for (var z in s)B[z] = s[z];
                v = null
            }
        }();
        (function () {
            var s = D.createElement("div");
            s.innerHTML = "<div class='test e'></div><div class='test'></div>";
            if (!(!s.getElementsByClassName || s.getElementsByClassName("e").length === 0)) {
                s.lastChild.className = "e";
                if (s.getElementsByClassName("e").length !== 1) {
                    F.order.splice(1, 0, "CLASS");
                    F.find.CLASS = function (v, z, C) {
                        if (typeof z.getElementsByClassName !== "undefined" && !C)return z.getElementsByClassName(v[1])
                    };
                    s = null
                }
            }
        })();
        var ca = D.compareDocumentPosition ?
            function (s, v) {
                return!!(s.compareDocumentPosition(v) & 16)
            } : function (s, v) {
            return s !== v && (s.contains ? s.contains(v) : true)
        }, O = function (s) {
            return(s = (s ? s.ownerDocument || s : 0).documentElement) ? s.nodeName !== "HTML" : false
        }, Da = function (s, v) {
            var z = [], C = "", H;
            for (v = v.nodeType ? [v] : v; H = F.match.PSEUDO.exec(s);) {
                C += H[0];
                s = s.replace(F.match.PSEUDO, "")
            }
            s = F.relative[s] ? s + "*" : s;
            H = 0;
            for (var G = v.length; H < G; H++)B(s, v[H], z);
            return B.filter(C, z)
        };
        i.find = B;
        i.expr = B.selectors;
        i.expr[":"] = i.expr.filters;
        i.unique = B.uniqueSort;
        i.text =
            d;
        i.isXMLDoc = O;
        i.contains = ca
    })();
    var eb = /Until$/, fb = /^(?:parents|prevUntil|prevAll)/, gb = /,/;
    U = Array.prototype.slice;
    var Ta = function (d, j, k) {
        if (i.isFunction(j))return i.grep(d, function (n, w) {
            return!!j.call(n, w, n) === k
        }); else if (j.nodeType)return i.grep(d, function (n) {
            return n === j === k
        }); else if (typeof j === "string") {
            var m = i.grep(d, function (n) {
                return n.nodeType === 1
            });
            if (ea.test(j))return i.filter(j, m, !k); else j = i.filter(j, m)
        }
        return i.grep(d, function (n) {
            return i.inArray(n, j) >= 0 === k
        })
    };
    i.fn.extend({find: function (d) {
        for (var j =
            this.pushStack("", "find", d), k = 0, m = 0, n = this.length; m < n; m++) {
            k = j.length;
            i.find(d, this[m], j);
            if (m > 0)for (var w = k; w < j.length; w++)for (var x = 0; x < k; x++)if (j[x] === j[w]) {
                j.splice(w--, 1);
                break
            }
        }
        return j
    }, has: function (d) {
        var j = i(d);
        return this.filter(function () {
            for (var k = 0, m = j.length; k < m; k++)if (i.contains(this, j[k]))return true
        })
    }, not: function (d) {
        return this.pushStack(Ta(this, d, false), "not", d)
    }, filter: function (d) {
        return this.pushStack(Ta(this, d, true), "filter", d)
    }, is: function (d) {
        return!!d && i.filter(d, this).length >
            0
    }, closest: function (d, j) {
        if (i.isArray(d)) {
            var k = [], m = this[0], n, w = {}, x;
            if (m && d.length) {
                n = 0;
                for (var E = d.length; n < E; n++) {
                    x = d[n];
                    w[x] || (w[x] = i.expr.match.POS.test(x) ? i(x, j || this.context) : x)
                }
                for (; m && m.ownerDocument && m !== j;) {
                    for (x in w) {
                        n = w[x];
                        if (n.jquery ? n.index(m) > -1 : i(m).is(n)) {
                            k.push({selector: x, elem: m});
                            delete w[x]
                        }
                    }
                    m = m.parentNode
                }
            }
            return k
        }
        var B = i.expr.match.POS.test(d) ? i(d, j || this.context) : null;
        return this.map(function (F, I) {
            for (; I && I.ownerDocument && I !== j;) {
                if (B ? B.index(I) > -1 : i(I).is(d))return I;
                I = I.parentNode
            }
            return null
        })
    }, index: function (d) {
        if (!d || typeof d === "string")return i.inArray(this[0], d ? i(d) : this.parent().children());
        return i.inArray(d.jquery ? d[0] : d, this)
    }, add: function (d, j) {
        d = typeof d === "string" ? i(d, j || this.context) : i.makeArray(d);
        j = i.merge(this.get(), d);
        return this.pushStack(!d[0] || !d[0].parentNode || d[0].parentNode.nodeType === 11 || !j[0] || !j[0].parentNode || j[0].parentNode.nodeType === 11 ? j : i.unique(j))
    }, andSelf: function () {
        return this.add(this.prevObject)
    }});
    i.each({parent: function (d) {
        return(d =
            d.parentNode) && d.nodeType !== 11 ? d : null
    }, parents: function (d) {
        return i.dir(d, "parentNode")
    }, parentsUntil: function (d, j, k) {
        return i.dir(d, "parentNode", k)
    }, next: function (d) {
        return i.nth(d, 2, "nextSibling")
    }, prev: function (d) {
        return i.nth(d, 2, "previousSibling")
    }, nextAll: function (d) {
        return i.dir(d, "nextSibling")
    }, prevAll: function (d) {
        return i.dir(d, "previousSibling")
    }, nextUntil: function (d, j, k) {
        return i.dir(d, "nextSibling", k)
    }, prevUntil: function (d, j, k) {
        return i.dir(d, "previousSibling", k)
    }, siblings: function (d) {
        return i.sibling(d.parentNode.firstChild,
            d)
    }, children: function (d) {
        return i.sibling(d.firstChild)
    }, contents: function (d) {
        return i.nodeName(d, "iframe") ? d.contentDocument || d.contentWindow.document : i.makeArray(d.childNodes)
    }}, function (d, j) {
        i.fn[d] = function (k, m) {
            var n = i.map(this, j, k);
            eb.test(d) || (m = k);
            if (m && typeof m === "string")n = i.filter(m, n);
            n = this.length > 1 ? i.unique(n) : n;
            if ((this.length > 1 || gb.test(m)) && fb.test(d))n = n.reverse();
            return this.pushStack(n, d, U.call(arguments).join(","))
        }
    });
    i.extend({filter: function (d, j, k) {
        if (k)d = ":not(" + d + ")";
        return i.find.matches(d,
            j)
    }, dir: function (d, j, k) {
        var m = [];
        for (d = d[j]; d && d.nodeType !== 9 && (k === b || d.nodeType !== 1 || !i(d).is(k));) {
            d.nodeType === 1 && m.push(d);
            d = d[j]
        }
        return m
    }, nth: function (d, j, k) {
        j = j || 1;
        for (var m = 0; d; d = d[k])if (d.nodeType === 1 && ++m === j)break;
        return d
    }, sibling: function (d, j) {
        for (var k = []; d; d = d.nextSibling)d.nodeType === 1 && d !== j && k.push(d);
        return k
    }});
    var Ua = / jQuery\d+="(?:\d+|null)"/g, ya = /^\s+/, Va = /(<([\w:]+)[^>]*?)\/>/g, hb = /^(?:area|br|col|embed|hr|img|input|link|meta|param)$/i, Wa = /<([\w:]+)/, ib = /<tbody/i, jb = /<|&#?\w+;/,
        Ja = /<script|<object|<embed|<option|<style/i, Ka = /checked\s*(?:[^=]|=\s*.checked.)/i, Xa = function (d, j, k) {
            return hb.test(k) ? d : j + "></" + k + ">"
        }, da = {option: [1, "<select multiple='multiple'>", "</select>"], legend: [1, "<fieldset>", "</fieldset>"], thead: [1, "<table>", "</table>"], tr: [2, "<table><tbody>", "</tbody></table>"], td: [3, "<table><tbody><tr>", "</tr></tbody></table>"], col: [2, "<table><tbody></tbody><colgroup>", "</colgroup></table>"], area: [1, "<map>", "</map>"], _default: [0, "", ""]};
    da.optgroup = da.option;
    da.tbody =
        da.tfoot = da.colgroup = da.caption = da.thead;
    da.th = da.td;
    if (!i.support.htmlSerialize)da._default = [1, "div<div>", "</div>"];
    i.fn.extend({text: function (d) {
        if (i.isFunction(d))return this.each(function (j) {
            var k = i(this);
            k.text(d.call(this, j, k.text()))
        });
        if (typeof d !== "object" && d !== b)return this.empty().append((this[0] && this[0].ownerDocument || D).createTextNode(d));
        return i.text(this)
    }, wrapAll: function (d) {
        if (i.isFunction(d))return this.each(function (k) {
            i(this).wrapAll(d.call(this, k))
        });
        if (this[0]) {
            var j = i(d, this[0].ownerDocument).eq(0).clone(true);
            this[0].parentNode && j.insertBefore(this[0]);
            j.map(function () {
                for (var k = this; k.firstChild && k.firstChild.nodeType === 1;)k = k.firstChild;
                return k
            }).append(this)
        }
        return this
    }, wrapInner: function (d) {
        if (i.isFunction(d))return this.each(function (j) {
            i(this).wrapInner(d.call(this, j))
        });
        return this.each(function () {
            var j = i(this), k = j.contents();
            k.length ? k.wrapAll(d) : j.append(d)
        })
    }, wrap: function (d) {
        return this.each(function () {
            i(this).wrapAll(d)
        })
    }, unwrap: function () {
        return this.parent().each(function () {
            i.nodeName(this,
                "body") || i(this).replaceWith(this.childNodes)
        }).end()
    }, append: function () {
        return this.domManip(arguments, true, function (d) {
            this.nodeType === 1 && this.appendChild(d)
        })
    }, prepend: function () {
        return this.domManip(arguments, true, function (d) {
            this.nodeType === 1 && this.insertBefore(d, this.firstChild)
        })
    }, before: function () {
        if (this[0] && this[0].parentNode)return this.domManip(arguments, false, function (j) {
            this.parentNode.insertBefore(j, this)
        }); else if (arguments.length) {
            var d = i(arguments[0]);
            d.push.apply(d, this.toArray());
            return this.pushStack(d, "before", arguments)
        }
    }, after: function () {
        if (this[0] && this[0].parentNode)return this.domManip(arguments, false, function (j) {
            this.parentNode.insertBefore(j, this.nextSibling)
        }); else if (arguments.length) {
            var d = this.pushStack(this, "after", arguments);
            d.push.apply(d, i(arguments[0]).toArray());
            return d
        }
    }, remove: function (d, j) {
        for (var k = 0, m; (m = this[k]) != null; k++)if (!d || i.filter(d, [m]).length) {
            if (!j && m.nodeType === 1) {
                i.cleanData(m.getElementsByTagName("*"));
                i.cleanData([m])
            }
            m.parentNode &&
            m.parentNode.removeChild(m)
        }
        return this
    }, empty: function () {
        for (var d = 0, j; (j = this[d]) != null; d++)for (j.nodeType === 1 && i.cleanData(j.getElementsByTagName("*")); j.firstChild;)j.removeChild(j.firstChild);
        return this
    }, clone: function (d) {
        var j = this.map(function () {
            if (!i.support.noCloneEvent && !i.isXMLDoc(this)) {
                var k = this.outerHTML, m = this.ownerDocument;
                if (!k) {
                    k = m.createElement("div");
                    k.appendChild(this.cloneNode(true));
                    k = k.innerHTML
                }
                return i.clean([k.replace(Ua, "").replace(/=([^="'>\s]+\/)>/g, '="$1">').replace(ya,
                    "")], m)[0];//'";// just for source insight parse 
            } else return this.cloneNode(true)
        });
        if (d === true) {
            r(this, j);
            r(this.find("*"), j.find("*"))
        }
        return j
    }, html: function (d) {
        if (d === b)return this[0] && this[0].nodeType === 1 ? this[0].innerHTML.replace(Ua, "") : null; else if (typeof d === "string" && !Ja.test(d) && (i.support.leadingWhitespace || !ya.test(d)) && !da[(Wa.exec(d) || ["", ""])[1].toLowerCase()]) {
            d = d.replace(Va, Xa);
            try {
                for (var j = 0, k = this.length; j < k; j++)if (this[j].nodeType === 1) {
                    i.cleanData(this[j].getElementsByTagName("*"));
                    this[j].innerHTML = d
                }
            } catch (m) {
                this.empty().append(d)
            }
        } else i.isFunction(d) ?
            this.each(function (n) {
                var w = i(this), x = w.html();
                w.empty().append(function () {
                    return d.call(this, n, x)
                })
            }) : this.empty().append(d);
        return this
    }, replaceWith: function (d) {
        if (this[0] && this[0].parentNode) {
            if (i.isFunction(d))return this.each(function (j) {
                var k = i(this), m = k.html();
                k.replaceWith(d.call(this, j, m))
            });
            if (typeof d !== "string")d = i(d).detach();
            return this.each(function () {
                var j = this.nextSibling, k = this.parentNode;
                i(this).remove();
                j ? i(j).before(d) : i(k).append(d)
            })
        } else return this.pushStack(i(i.isFunction(d) ?
            d() : d), "replaceWith", d)
    }, detach: function (d) {
        return this.remove(d, true)
    }, domManip: function (d, j, k) {
        var m, n, w = d[0], x = [], E;
        if (!i.support.checkClone && arguments.length === 3 && typeof w === "string" && Ka.test(w))return this.each(function () {
            i(this).domManip(d, j, k, true)
        });
        if (i.isFunction(w))return this.each(function (F) {
            var I = i(this);
            d[0] = w.call(this, F, j ? I.html() : b);
            I.domManip(d, j, k)
        });
        if (this[0]) {
            m = w && w.parentNode;
            m = i.support.parentNode && m && m.nodeType === 11 && m.childNodes.length === this.length ? {fragment: m} : u(d, this,
                x);
            E = m.fragment;
            if (n = E.childNodes.length === 1 ? E = E.firstChild : E.firstChild) {
                j = j && i.nodeName(n, "tr");
                n = 0;
                for (var B = this.length; n < B; n++)k.call(j ? i.nodeName(this[n], "table") ? this[n].getElementsByTagName("tbody")[0] || this[n].appendChild(this[n].ownerDocument.createElement("tbody")) : this[n] : this[n], n > 0 || m.cacheable || this.length > 1 ? E.cloneNode(true) : E)
            }
            x.length && i.each(x, f)
        }
        return this
    }});
    i.fragments = {};
    i.each({appendTo: "append", prependTo: "prepend", insertBefore: "before", insertAfter: "after", replaceAll: "replaceWith"},
        function (d, j) {
            i.fn[d] = function (k) {
                var m = [];
                k = i(k);
                var n = this.length === 1 && this[0].parentNode;
                if (n && n.nodeType === 11 && n.childNodes.length === 1 && k.length === 1) {
                    k[j](this[0]);
                    return this
                } else {
                    n = 0;
                    for (var w = k.length; n < w; n++) {
                        var x = (n > 0 ? this.clone(true) : this).get();
                        i.fn[j].apply(i(k[n]), x);
                        m = m.concat(x)
                    }
                    return this.pushStack(m, d, k.selector)
                }
            }
        });
    i.extend({clean: function (d, j, k, m) {
        j = j || D;
        if (typeof j.createElement === "undefined")j = j.ownerDocument || j[0] && j[0].ownerDocument || D;
        for (var n = [], w = 0, x; (x = d[w]) != null; w++) {
            if (typeof x ===
                "number")x += "";
            if (x) {
                if (typeof x === "string" && !jb.test(x))x = j.createTextNode(x); else if (typeof x === "string") {
                    x = x.replace(Va, Xa);
                    var E = (Wa.exec(x) || ["", ""])[1].toLowerCase(), B = da[E] || da._default, F = B[0], I = j.createElement("div");
                    for (I.innerHTML = B[1] + x + B[2]; F--;)I = I.lastChild;
                    if (!i.support.tbody) {
                        F = ib.test(x);
                        E = E === "table" && !F ? I.firstChild && I.firstChild.childNodes : B[1] === "<table>" && !F ? I.childNodes : [];
                        for (B = E.length - 1; B >= 0; --B)i.nodeName(E[B], "tbody") && !E[B].childNodes.length && E[B].parentNode.removeChild(E[B])
                    }
                    !i.support.leadingWhitespace &&
                    ya.test(x) && I.insertBefore(j.createTextNode(ya.exec(x)[0]), I.firstChild);
                    x = I.childNodes
                }
                if (x.nodeType)n.push(x); else n = i.merge(n, x)
            }
        }
        if (k)for (w = 0; n[w]; w++)if (m && i.nodeName(n[w], "script") && (!n[w].type || n[w].type.toLowerCase() === "text/javascript"))m.push(n[w].parentNode ? n[w].parentNode.removeChild(n[w]) : n[w]); else {
            n[w].nodeType === 1 && n.splice.apply(n, [w + 1, 0].concat(i.makeArray(n[w].getElementsByTagName("script"))));
            k.appendChild(n[w])
        }
        return n
    }, cleanData: function (d) {
        for (var j, k, m = i.cache, n = i.event.special,
                 w = i.support.deleteExpando, x = 0, E; (E = d[x]) != null; x++)if (k = E[i.expando]) {
            j = m[k];
            if (j.events)for (var B in j.events)n[B] ? i.event.remove(E, B) : Na(E, B, j.handle);
            if (w)delete E[i.expando]; else E.removeAttribute && E.removeAttribute(i.expando);
            delete m[k]
        }
    }});
    var kb = /z-?index|font-?weight|opacity|zoom|line-?height/i, Ya = /alpha\([^)]*\)/, Za = /opacity=([^)]*)/, Ea = /float/i, Fa = /-([a-z])/ig, lb = /([A-Z])/g, mb = /^-?\d+(?:px)?$/i, nb = /^-?\d/, ob = {position: "absolute", visibility: "hidden", display: "block"}, pb = ["Left", "Right"],
        qb = ["Top", "Bottom"], rb = D.defaultView && D.defaultView.getComputedStyle, $a = i.support.cssFloat ? "cssFloat" : "styleFloat", Ga = function (d, j) {
            return j.toUpperCase()
        };
    i.fn.css = function (d, j) {
        return a(this, d, j, true, function (k, m, n) {
            if (n === b)return i.curCSS(k, m);
            if (typeof n === "number" && !kb.test(m))n += "px";
            i.style(k, m, n)
        })
    };
    i.extend({style: function (d, j, k) {
        if (!d || d.nodeType === 3 || d.nodeType === 8)return b;
        if ((j === "width" || j === "height") && parseFloat(k) < 0)k = b;
        var m = d.style || d, n = k !== b;
        if (!i.support.opacity && j === "opacity") {
            if (n) {
                m.zoom =
                    1;
                j = parseInt(k, 10) + "" === "NaN" ? "" : "alpha(opacity=" + k * 100 + ")";
                d = m.filter || i.curCSS(d, "filter") || "";
                m.filter = Ya.test(d) ? d.replace(Ya, j) : j
            }
            return m.filter && m.filter.indexOf("opacity=") >= 0 ? parseFloat(Za.exec(m.filter)[1]) / 100 + "" : ""
        }
        if (Ea.test(j))j = $a;
        j = j.replace(Fa, Ga);
        if (n)m[j] = k;
        return m[j]
    }, css: function (d, j, k, m) {
        if (j === "width" || j === "height") {
            var n, w = j === "width" ? pb : qb;
            k = function () {
                n = j === "width" ? d.offsetWidth : d.offsetHeight;
                m !== "border" && i.each(w, function () {
                    m || (n -= parseFloat(i.curCSS(d, "padding" + this,
                        true)) || 0);
                    if (m === "margin")n += parseFloat(i.curCSS(d, "margin" + this, true)) || 0; else n -= parseFloat(i.curCSS(d, "border" + this + "Width", true)) || 0
                })
            };
            d.offsetWidth !== 0 ? k() : i.swap(d, ob, k);
            return Math.max(0, Math.round(n))
        }
        return i.curCSS(d, j, k)
    }, curCSS: function (d, j, k) {
        var m, n = d.style;
        if (!i.support.opacity && j === "opacity" && d.currentStyle) {
            m = Za.test(d.currentStyle.filter || "") ? parseFloat(RegExp.$1) / 100 + "" : "";
            return m === "" ? "1" : m
        }
        if (Ea.test(j))j = $a;
        if (!k && n && n[j])m = n[j]; else if (rb) {
            if (Ea.test(j))j = "float";
            j = j.replace(lb,
                "-$1").toLowerCase();
            n = d.ownerDocument.defaultView;
            if (!n)return null;
            if (d = n.getComputedStyle(d, null))m = d.getPropertyValue(j);
            if (j === "opacity" && m === "")m = "1"
        } else if (d.currentStyle) {
            k = j.replace(Fa, Ga);
            m = d.currentStyle[j] || d.currentStyle[k];
            if (!mb.test(m) && nb.test(m)) {
                j = n.left;
                var w = d.runtimeStyle.left;
                d.runtimeStyle.left = d.currentStyle.left;
                n.left = k === "fontSize" ? "1em" : m || 0;
                m = n.pixelLeft + "px";
                n.left = j;
                d.runtimeStyle.left = w
            }
        }
        return m
    }, swap: function (d, j, k) {
        var m = {};
        for (var n in j) {
            m[n] = d.style[n];
            d.style[n] =
                j[n]
        }
        k.call(d);
        for (n in j)d.style[n] = m[n]
    }});
    if (i.expr && i.expr.filters) {
        i.expr.filters.hidden = function (d) {
            var j = d.offsetWidth, k = d.offsetHeight, m = d.nodeName.toLowerCase() === "tr";
            return j === 0 && k === 0 && !m ? true : j > 0 && k > 0 && !m ? false : i.curCSS(d, "display") === "none"
        };
        i.expr.filters.visible = function (d) {
            return!i.expr.filters.hidden(d)
        }
    }
    var sb = g(), tb = /<script(.|\s)*?\/script>/gi, ub = /select|textarea/i, vb = /color|date|datetime|email|hidden|month|number|password|range|search|tel|text|time|url|week/i, qa = /=\?(&|$)/,
        Ha = /\?/, wb = /(\?|&)_=.*?(&|$)/, xb = /^(\w+:)?\/\/([^\/?#]+)/, yb = /%20/g, zb = i.fn.load;
    i.fn.extend({load: function (d, j, k) {
        if (typeof d !== "string")return zb.call(this, d); else if (!this.length)return this;
        var m = d.indexOf(" ");
        if (m >= 0) {
            var n = d.slice(m, d.length);
            d = d.slice(0, m)
        }
        m = "GET";
        if (j)if (i.isFunction(j)) {
            k = j;
            j = null
        } else if (typeof j === "object") {
            j = i.param(j, i.ajaxSettings.traditional);
            m = "POST"
        }
        var w = this;
        i.ajax({url: d, type: m, dataType: "html", data: j, complete: function (x, E) {
            if (E === "success" || E === "notmodified")w.html(n ?
                i("<div />").append(x.responseText.replace(tb, "")).find(n) : x.responseText);
            k && w.each(k, [x.responseText, E, x])
        }});
        return this
    }, serialize: function () {
        return i.param(this.serializeArray())
    }, serializeArray: function () {
        return this.map(function () {
            return this.elements ? i.makeArray(this.elements) : this
        }).filter(function () {
            return this.name && !this.disabled && (this.checked || ub.test(this.nodeName) || vb.test(this.type))
        }).map(function (d, j) {
            d = i(this).val();
            return d == null ? null : i.isArray(d) ? i.map(d, function (k) {
                return{name: j.name,
                    value: k}
            }) : {name: j.name, value: d}
        }).get()
    }});
    i.each("ajaxStart ajaxStop ajaxComplete ajaxError ajaxSuccess ajaxSend".split(" "), function (d, j) {
        i.fn[j] = function (k) {
            return this.bind(j, k)
        }
    });
    i.extend({get: function (d, j, k, m) {
        if (i.isFunction(j)) {
            m = m || k;
            k = j;
            j = null
        }
        return i.ajax({type: "GET", url: d, data: j, success: k, dataType: m})
    }, getScript: function (d, j) {
        return i.get(d, null, j, "script")
    }, getJSON: function (d, j, k) {
        return i.get(d, j, k, "json")
    }, post: function (d, j, k, m) {
        if (i.isFunction(j)) {
            m = m || k;
            k = j;
            j = {}
        }
        return i.ajax({type: "POST",
            url: d, data: j, success: k, dataType: m})
    }, ajaxSetup: function (d) {
        i.extend(i.ajaxSettings, d)
    }, ajaxSettings: {url: location.href, global: true, type: "GET", contentType: "application/x-www-form-urlencoded", processData: true, async: true, xhr: c.XMLHttpRequest && (c.location.protocol !== "file:" || !c.ActiveXObject) ? function () {
        return new c.XMLHttpRequest
    } : function () {
        try {
            return new c.ActiveXObject("Microsoft.XMLHTTP")
        } catch (d) {
        }
    }, accepts: {xml: "application/xml, text/xml", html: "text/html", script: "text/javascript, application/javascript",
        json: "application/json, text/javascript", text: "text/plain", _default: "*/*"}}, lastModified: {}, etag: {}, ajax: function (d) {
        function j() {
            n.success && n.success.call(B, E, x, O);
            n.global && m("ajaxSuccess", [O, n])
        }

        function k() {
            n.complete && n.complete.call(B, O, x);
            n.global && m("ajaxComplete", [O, n]);
            n.global && !--i.active && i.event.trigger("ajaxStop")
        }

        function m(H, G) {
            (n.context ? i(n.context) : i.event).trigger(H, G)
        }

        var n = i.extend(true, {}, i.ajaxSettings, d), w, x, E, B = d && d.context || n, F = n.type.toUpperCase();
        if (n.data && n.processData &&
            typeof n.data !== "string")n.data = i.param(n.data, n.traditional);
        if (n.dataType === "jsonp") {
            if (F === "GET")qa.test(n.url) || (n.url += (Ha.test(n.url) ? "&" : "?") + (n.jsonp || "callback") + "=?"); else if (!n.data || !qa.test(n.data))n.data = (n.data ? n.data + "&" : "") + (n.jsonp || "callback") + "=?";
            n.dataType = "json"
        }
        if (n.dataType === "json" && (n.data && qa.test(n.data) || qa.test(n.url))) {
            w = n.jsonpCallback || "jsonp" + sb++;
            if (n.data)n.data = (n.data + "").replace(qa, "=" + w + "$1");
            n.url = n.url.replace(qa, "=" + w + "$1");
            n.dataType = "script";
            c[w] = c[w] ||
                function (H) {
                    E = H;
                    j();
                    k();
                    c[w] = b;
                    try {
                        delete c[w]
                    } catch (G) {
                    }
                    R && R.removeChild(W)
                }
        }
        if (n.dataType === "script" && n.cache === null)n.cache = false;
        if (n.cache === false && F === "GET") {
            var I = g(), N = n.url.replace(wb, "$1_=" + I + "$2");
            n.url = N + (N === n.url ? (Ha.test(n.url) ? "&" : "?") + "_=" + I : "")
        }
        if (n.data && F === "GET")n.url += (Ha.test(n.url) ? "&" : "?") + n.data;
        n.global && !i.active++ && i.event.trigger("ajaxStart");
        I = (I = xb.exec(n.url)) && (I[1] && I[1] !== location.protocol || I[2] !== location.host);
        if (n.dataType === "script" && F === "GET" && I) {
            var R = D.getElementsByTagName("head")[0] ||
                D.documentElement, W = D.createElement("script");
            W.src = n.url;
            if (n.scriptCharset)W.charset = n.scriptCharset;
            if (!w) {
                var T = false;
                W.onload = W.onreadystatechange = function () {
                    if (!T && (!this.readyState || this.readyState === "loaded" || this.readyState === "complete")) {
                        T = true;
                        j();
                        k();
                        W.onload = W.onreadystatechange = null;
                        R && W.parentNode && R.removeChild(W)
                    }
                }
            }
            R.insertBefore(W, R.firstChild);
            return b
        }
        var ca = false, O = n.xhr();
        if (O) {
            n.username ? O.open(F, n.url, n.async, n.username, n.password) : O.open(F, n.url, n.async);
            try {
                if (n.data || d &&
                    d.contentType)O.setRequestHeader("Content-Type", n.contentType);
                if (n.ifModified) {
                    i.lastModified[n.url] && O.setRequestHeader("If-Modified-Since", i.lastModified[n.url]);
                    i.etag[n.url] && O.setRequestHeader("If-None-Match", i.etag[n.url])
                }
                I || O.setRequestHeader("X-Requested-With", "XMLHttpRequest");
                O.setRequestHeader("Accept", n.dataType && n.accepts[n.dataType] ? n.accepts[n.dataType] + ", */*" : n.accepts._default)
            } catch (Da) {
            }
            if (n.beforeSend && n.beforeSend.call(B, O, n) === false) {
                n.global && !--i.active && i.event.trigger("ajaxStop");
                O.abort();
                return false
            }
            n.global && m("ajaxSend", [O, n]);
            var s = O.onreadystatechange = function (H) {
                if (!O || O.readyState === 0 || H === "abort") {
                    ca || k();
                    ca = true;
                    if (O)O.onreadystatechange = i.noop
                } else if (!ca && O && (O.readyState === 4 || H === "timeout")) {
                    ca = true;
                    O.onreadystatechange = i.noop;
                    x = H === "timeout" ? "timeout" : !i.httpSuccess(O) ? "error" : n.ifModified && i.httpNotModified(O, n.url) ? "notmodified" : "success";
                    var G;
                    if (x === "success")try {
                        E = i.httpData(O, n.dataType, n)
                    } catch (M) {
                        x = "parsererror";
                        G = M
                    }
                    if (x === "success" || x === "notmodified")w ||
                    j(); else i.handleError(n, O, x, G);
                    k();
                    H === "timeout" && O.abort();
                    if (n.async)O = null
                }
            };
            try {
                var v = O.abort;
                O.abort = function () {
                    O && v.call(O);
                    s("abort")
                }
            } catch (z) {
            }
            n.async && n.timeout > 0 && setTimeout(function () {
                O && !ca && s("timeout")
            }, n.timeout);
            try {
                O.send(F === "POST" || F === "PUT" || F === "DELETE" ? n.data : null)
            } catch (C) {
                i.handleError(n, O, null, C);
                k()
            }
            n.async || s();
            return O
        }
    }, handleError: function (d, j, k, m) {
        if (d.error)d.error.call(d.context || d, j, k, m);
        if (d.global)(d.context ? i(d.context) : i.event).trigger("ajaxError", [j, d, m])
    },
        active: 0, httpSuccess: function (d) {
            try {
                return!d.status && location.protocol === "file:" || d.status >= 200 && d.status < 300 || d.status === 304 || d.status === 1223 || d.status === 0
            } catch (j) {
            }
            return false
        }, httpNotModified: function (d, j) {
            var k = d.getResponseHeader("Last-Modified"), m = d.getResponseHeader("Etag");
            if (k)i.lastModified[j] = k;
            if (m)i.etag[j] = m;
            return d.status === 304 || d.status === 0
        }, httpData: function (d, j, k) {
            var m = d.getResponseHeader("content-type") || "", n = j === "xml" || !j && m.indexOf("xml") >= 0;
            d = n ? d.responseXML : d.responseText;
            n && d.documentElement.nodeName === "parsererror" && i.error("parsererror");
            if (k && k.dataFilter)d = k.dataFilter(d, j);
            if (typeof d === "string")if (j === "json" || !j && m.indexOf("json") >= 0)d = i.parseJSON(d); else if (j === "script" || !j && m.indexOf("javascript") >= 0)i.globalEval(d);
            return d
        }, param: function (d, j) {
            function k(x, E) {
                if (i.isArray(E))i.each(E, function (B, F) {
                    j || /\[\]$/.test(x) ? m(x, F) : k(x + "[" + (typeof F === "object" || i.isArray(F) ? B : "") + "]", F)
                }); else!j && E != null && typeof E === "object" ? i.each(E, function (B, F) {
                    k(x + "[" + B + "]",
                        F)
                }) : m(x, E)
            }

            function m(x, E) {
                E = i.isFunction(E) ? E() : E;
                n[n.length] = encodeURIComponent(x) + "=" + encodeURIComponent(E)
            }

            var n = [];
            if (j === b)j = i.ajaxSettings.traditional;
            if (i.isArray(d) || d.jquery)i.each(d, function () {
                m(this.name, this.value)
            }); else for (var w in d)k(w, d[w]);
            return n.join("&").replace(yb, "+")
        }});
    var Ia = {}, Ab = /toggle|show|hide/, Bb = /^([+-]=)?([\d+-.]+)(.*)$/, za, La = [
        ["height", "marginTop", "marginBottom", "paddingTop", "paddingBottom"],
        ["width", "marginLeft", "marginRight", "paddingLeft", "paddingRight"],
        ["opacity"]
    ];
    i.fn.extend({show: function (d, j) {
        if (d || d === 0)return this.animate(y("show", 3), d, j); else {
            d = 0;
            for (j = this.length; d < j; d++) {
                var k = i.data(this[d], "olddisplay");
                this[d].style.display = k || "";
                if (i.css(this[d], "display") === "none") {
                    k = this[d].nodeName;
                    var m;
                    if (Ia[k])m = Ia[k]; else {
                        var n = i("<" + k + " />").appendTo("body");
                        m = n.css("display");
                        if (m === "none")m = "block";
                        n.remove();
                        Ia[k] = m
                    }
                    i.data(this[d], "olddisplay", m)
                }
            }
            d = 0;
            for (j = this.length; d < j; d++)this[d].style.display = i.data(this[d], "olddisplay") || "";
            return this
        }
    },
        hide: function (d, j) {
            if (d || d === 0)return this.animate(y("hide", 3), d, j); else {
                d = 0;
                for (j = this.length; d < j; d++) {
                    var k = i.data(this[d], "olddisplay");
                    !k && k !== "none" && i.data(this[d], "olddisplay", i.css(this[d], "display"))
                }
                d = 0;
                for (j = this.length; d < j; d++)this[d].style.display = "none";
                return this
            }
        }, _toggle: i.fn.toggle, toggle: function (d, j) {
            var k = typeof d === "boolean";
            if (i.isFunction(d) && i.isFunction(j))this._toggle.apply(this, arguments); else d == null || k ? this.each(function () {
                var m = k ? d : i(this).is(":hidden");
                i(this)[m ? "show" :
                    "hide"]()
            }) : this.animate(y("toggle", 3), d, j);
            return this
        }, fadeTo: function (d, j, k) {
            return this.filter(":hidden").css("opacity", 0).show().end().animate({opacity: j}, d, k)
        }, animate: function (d, j, k, m) {
            var n = i.speed(j, k, m);
            if (i.isEmptyObject(d))return this.each(n.complete);
            return this[n.queue === false ? "each" : "queue"](function () {
                var w = i.extend({}, n), x, E = this.nodeType === 1 && i(this).is(":hidden"), B = this;
                for (x in d) {
                    var F = x.replace(Fa, Ga);
                    if (x !== F) {
                        d[F] = d[x];
                        delete d[x];
                        x = F
                    }
                    if (d[x] === "hide" && E || d[x] === "show" && !E)return w.complete.call(this);
                    if ((x === "height" || x === "width") && this.style) {
                        w.display = i.css(this, "display");
                        w.overflow = this.style.overflow
                    }
                    if (i.isArray(d[x])) {
                        (w.specialEasing = w.specialEasing || {})[x] = d[x][1];
                        d[x] = d[x][0]
                    }
                }
                if (w.overflow != null)this.style.overflow = "hidden";
                w.curAnim = i.extend({}, d);
                i.each(d, function (I, N) {
                    var R = new i.fx(B, w, I);
                    if (Ab.test(N))R[N === "toggle" ? E ? "show" : "hide" : N](d); else {
                        var W = Bb.exec(N), T = R.cur(true) || 0;
                        if (W) {
                            N = parseFloat(W[2]);
                            var ca = W[3] || "px";
                            if (ca !== "px") {
                                B.style[I] = (N || 1) + ca;
                                T = (N || 1) / R.cur(true) * T;
                                B.style[I] =
                                    T + ca
                            }
                            if (W[1])N = (W[1] === "-=" ? -1 : 1) * N + T;
                            R.custom(T, N, ca)
                        } else R.custom(T, N, "")
                    }
                });
                return true
            })
        }, stop: function (d, j) {
            var k = i.timers;
            d && this.queue([]);
            this.each(function () {
                for (var m = k.length - 1; m >= 0; m--)if (k[m].elem === this) {
                    j && k[m](true);
                    k.splice(m, 1)
                }
            });
            j || this.dequeue();
            return this
        }});
    i.each({slideDown: y("show", 1), slideUp: y("hide", 1), slideToggle: y("toggle", 1), fadeIn: {opacity: "show"}, fadeOut: {opacity: "hide"}}, function (d, j) {
        i.fn[d] = function (k, m) {
            return this.animate(j, k, m)
        }
    });
    i.extend({speed: function (d, j, k) {
        var m = d && typeof d === "object" ? d : {complete: k || !k && j || i.isFunction(d) && d, duration: d, easing: k && j || j && !i.isFunction(j) && j};
        m.duration = i.fx.off ? 0 : typeof m.duration === "number" ? m.duration : i.fx.speeds[m.duration] || i.fx.speeds._default;
        m.old = m.complete;
        m.complete = function () {
            m.queue !== false && i(this).dequeue();
            i.isFunction(m.old) && m.old.call(this)
        };
        return m
    }, easing: {linear: function (d, j, k, m) {
        return k + m * d
    }, swing: function (d, j, k, m) {
        return(-Math.cos(d * Math.PI) / 2 + 0.5) * m + k
    }}, timers: [], fx: function (d, j, k) {
        this.options =
            j;
        this.elem = d;
        this.prop = k;
        if (!j.orig)j.orig = {}
    }});
    i.fx.prototype = {update: function () {
        this.options.step && this.options.step.call(this.elem, this.now, this);
        (i.fx.step[this.prop] || i.fx.step._default)(this);
        if ((this.prop === "height" || this.prop === "width") && this.elem.style)this.elem.style.display = "block"
    }, cur: function (d) {
        if (this.elem[this.prop] != null && (!this.elem.style || this.elem.style[this.prop] == null))return this.elem[this.prop];
        return(d = parseFloat(i.css(this.elem, this.prop, d))) && d > -10000 ? d : parseFloat(i.curCSS(this.elem,
            this.prop)) || 0
    }, custom: function (d, j, k) {
        function m(w) {
            return n.step(w)
        }

        this.startTime = g();
        this.start = d;
        this.end = j;
        this.unit = k || this.unit || "px";
        this.now = this.start;
        this.pos = this.state = 0;
        var n = this;
        m.elem = this.elem;
        if (m() && i.timers.push(m) && !za)za = setInterval(i.fx.tick, 13)
    }, show: function () {
        this.options.orig[this.prop] = i.style(this.elem, this.prop);
        this.options.show = true;
        this.custom(this.prop === "width" || this.prop === "height" ? 1 : 0, this.cur());
        i(this.elem).show()
    }, hide: function () {
        this.options.orig[this.prop] =
            i.style(this.elem, this.prop);
        this.options.hide = true;
        this.custom(this.cur(), 0)
    }, step: function (d) {
        var j = g(), k = true;
        if (d || j >= this.options.duration + this.startTime) {
            this.now = this.end;
            this.pos = this.state = 1;
            this.update();
            this.options.curAnim[this.prop] = true;
            for (var m in this.options.curAnim)if (this.options.curAnim[m] !== true)k = false;
            if (k) {
                if (this.options.display != null) {
                    this.elem.style.overflow = this.options.overflow;
                    d = i.data(this.elem, "olddisplay");
                    this.elem.style.display = d ? d : this.options.display;
                    if (i.css(this.elem,
                        "display") === "none")this.elem.style.display = "block"
                }
                this.options.hide && i(this.elem).hide();
                if (this.options.hide || this.options.show)for (var n in this.options.curAnim)i.style(this.elem, n, this.options.orig[n]);
                this.options.complete.call(this.elem)
            }
            return false
        } else {
            n = j - this.startTime;
            this.state = n / this.options.duration;
            d = this.options.easing || (i.easing.swing ? "swing" : "linear");
            this.pos = i.easing[this.options.specialEasing && this.options.specialEasing[this.prop] || d](this.state, n, 0, 1, this.options.duration);
            this.now = this.start + (this.end - this.start) * this.pos;
            this.update()
        }
        return true
    }};
    i.extend(i.fx, {tick: function () {
        for (var d = i.timers, j = 0; j < d.length; j++)d[j]() || d.splice(j--, 1);
        d.length || i.fx.stop()
    }, stop: function () {
        clearInterval(za);
        za = null
    }, speeds: {slow: 600, fast: 200, _default: 400}, step: {opacity: function (d) {
        i.style(d.elem, "opacity", d.now)
    }, _default: function (d) {
        if (d.elem.style && d.elem.style[d.prop] != null)d.elem.style[d.prop] = (d.prop === "width" || d.prop === "height" ? Math.max(0, d.now) : d.now) + d.unit; else d.elem[d.prop] =
            d.now
    }}});
    if (i.expr && i.expr.filters)i.expr.filters.animated = function (d) {
        return i.grep(i.timers, function (j) {
            return d === j.elem
        }).length
    };
    i.fn.offset = "getBoundingClientRect"in D.documentElement ? function (d) {
        var j = this[0];
        if (d)return this.each(function (n) {
            i.offset.setOffset(this, d, n)
        });
        if (!j || !j.ownerDocument)return null;
        if (j === j.ownerDocument.body)return i.offset.bodyOffset(j);
        var k = j.getBoundingClientRect(), m = j.ownerDocument;
        j = m.body;
        m = m.documentElement;
        return{top: k.top + (self.pageYOffset || i.support.boxModel &&
            m.scrollTop || j.scrollTop) - (m.clientTop || j.clientTop || 0), left: k.left + (self.pageXOffset || i.support.boxModel && m.scrollLeft || j.scrollLeft) - (m.clientLeft || j.clientLeft || 0)}
    } : function (d) {
        var j = this[0];
        if (d)return this.each(function (I) {
            i.offset.setOffset(this, d, I)
        });
        if (!j || !j.ownerDocument)return null;
        if (j === j.ownerDocument.body)return i.offset.bodyOffset(j);
        i.offset.initialize();
        var k = j.offsetParent, m = j, n = j.ownerDocument, w, x = n.documentElement, E = n.body;
        m = (n = n.defaultView) ? n.getComputedStyle(j, null) : j.currentStyle;
        for (var B = j.offsetTop, F = j.offsetLeft; (j = j.parentNode) && j !== E && j !== x;) {
            if (i.offset.supportsFixedPosition && m.position === "fixed")break;
            w = n ? n.getComputedStyle(j, null) : j.currentStyle;
            B -= j.scrollTop;
            F -= j.scrollLeft;
            if (j === k) {
                B += j.offsetTop;
                F += j.offsetLeft;
                if (i.offset.doesNotAddBorder && !(i.offset.doesAddBorderForTableAndCells && /^t(able|d|h)$/i.test(j.nodeName))) {
                    B += parseFloat(w.borderTopWidth) || 0;
                    F += parseFloat(w.borderLeftWidth) || 0
                }
                m = k;
                k = j.offsetParent
            }
            if (i.offset.subtractsBorderForOverflowNotVisible && w.overflow !==
                "visible") {
                B += parseFloat(w.borderTopWidth) || 0;
                F += parseFloat(w.borderLeftWidth) || 0
            }
            m = w
        }
        if (m.position === "relative" || m.position === "static") {
            B += E.offsetTop;
            F += E.offsetLeft
        }
        if (i.offset.supportsFixedPosition && m.position === "fixed") {
            B += Math.max(x.scrollTop, E.scrollTop);
            F += Math.max(x.scrollLeft, E.scrollLeft)
        }
        return{top: B, left: F}
    };
    i.offset = {initialize: function () {
        var d = D.body, j = D.createElement("div"), k, m, n, w = parseFloat(i.curCSS(d, "marginTop", true)) || 0;
        i.extend(j.style, {position: "absolute", top: 0, left: 0, margin: 0,
            border: 0, width: "1px", height: "1px", visibility: "hidden"});
        j.innerHTML = "<div style='position:absolute;top:0;left:0;margin:0;border:5px solid #000;padding:0;width:1px;height:1px;'><div></div></div><table style='position:absolute;top:0;left:0;margin:0;border:5px solid #000;padding:0;width:1px;height:1px;' cellpadding='0' cellspacing='0'><tr><td></td></tr></table>";
        d.insertBefore(j, d.firstChild);
        k = j.firstChild;
        m = k.firstChild;
        n = k.nextSibling.firstChild.firstChild;
        this.doesNotAddBorder = m.offsetTop !== 5;
        this.doesAddBorderForTableAndCells =
            n.offsetTop === 5;
        m.style.position = "fixed";
        m.style.top = "20px";
        this.supportsFixedPosition = m.offsetTop === 20 || m.offsetTop === 15;
        m.style.position = m.style.top = "";
        k.style.overflow = "hidden";
        k.style.position = "relative";
        this.subtractsBorderForOverflowNotVisible = m.offsetTop === -5;
        this.doesNotIncludeMarginInBodyOffset = d.offsetTop !== w;
        d.removeChild(j);
        i.offset.initialize = i.noop
    }, bodyOffset: function (d) {
        var j = d.offsetTop, k = d.offsetLeft;
        i.offset.initialize();
        if (i.offset.doesNotIncludeMarginInBodyOffset) {
            j += parseFloat(i.curCSS(d,
                "marginTop", true)) || 0;
            k += parseFloat(i.curCSS(d, "marginLeft", true)) || 0
        }
        return{top: j, left: k}
    }, setOffset: function (d, j, k) {
        if (/static/.test(i.curCSS(d, "position")))d.style.position = "relative";
        var m = i(d), n = m.offset(), w = parseInt(i.curCSS(d, "top", true), 10) || 0, x = parseInt(i.curCSS(d, "left", true), 10) || 0;
        if (i.isFunction(j))j = j.call(d, k, n);
        k = {top: j.top - n.top + w, left: j.left - n.left + x};
        "using"in j ? j.using.call(d, k) : m.css(k)
    }};
    i.fn.extend({position: function () {
        if (!this[0])return null;
        var d = this[0], j = this.offsetParent(),
            k = this.offset(), m = /^body|html$/i.test(j[0].nodeName) ? {top: 0, left: 0} : j.offset();
        k.top -= parseFloat(i.curCSS(d, "marginTop", true)) || 0;
        k.left -= parseFloat(i.curCSS(d, "marginLeft", true)) || 0;
        m.top += parseFloat(i.curCSS(j[0], "borderTopWidth", true)) || 0;
        m.left += parseFloat(i.curCSS(j[0], "borderLeftWidth", true)) || 0;
        return{top: k.top - m.top, left: k.left - m.left}
    }, offsetParent: function () {
        return this.map(function () {
            for (var d = this.offsetParent || D.body; d && !/^body|html$/i.test(d.nodeName) && i.css(d, "position") === "static";)d =
                d.offsetParent;
            return d
        })
    }});
    i.each(["Left", "Top"], function (d, j) {
        var k = "scroll" + j;
        i.fn[k] = function (m) {
            var n = this[0], w;
            if (!n)return null;
            return m !== b ? this.each(function () {
                if (w = A(this))w.scrollTo(!d ? m : i(w).scrollLeft(), d ? m : i(w).scrollTop()); else this[k] = m
            }) : (w = A(n)) ? "pageXOffset"in w ? w[d ? "pageYOffset" : "pageXOffset"] : i.support.boxModel && w.document.documentElement[k] || w.document.body[k] : n[k]
        }
    });
    i.each(["Height", "Width"], function (d, j) {
        var k = j.toLowerCase();
        i.fn["inner" + j] = function () {
            return this[0] ? i.css(this[0],
                k, false, "padding") : null
        };
        i.fn["outer" + j] = function (m) {
            return this[0] ? i.css(this[0], k, false, m ? "margin" : "border") : null
        };
        i.fn[k] = function (m) {
            var n = this[0];
            if (!n)return m == null ? null : this;
            if (i.isFunction(m))return this.each(function (w) {
                var x = i(this);
                x[k](m.call(this, w, x[k]()))
            });
            return"scrollTo"in n && n.document ? n.document.compatMode === "CSS1Compat" && n.document.documentElement["client" + j] || n.document.body["client" + j] : n.nodeType === 9 ? Math.max(n.documentElement["client" + j], n.body["scroll" + j], n.documentElement["scroll" +
                j], n.body["offset" + j], n.documentElement["offset" + j]) : m === b ? i.css(n, k) : this.css(k, typeof m === "string" ? m : m + "px")
        }
    });
    c.jQuery = c.$ = i
})(window);
(function (c) {
    c.toJSON = function (f) {
        if (typeof JSON == "object" && JSON.stringify)return JSON.stringify(f);
        var a = typeof f;
        if (f === null)return"null";
        if (a != "undefined") {
            if (a == "number" || a == "boolean")return f + "";
            if (a == "string")return c.quoteString(f);
            if (a == "object") {
                if (typeof f.toJSON == "function")return c.toJSON(f.toJSON());
                if (f.constructor === Date) {
                    var g = f.getUTCMonth() + 1;
                    if (g < 10)g = "0" + g;
                    var h = f.getUTCDate();
                    if (h < 10)h = "0" + h;
                    a = f.getUTCFullYear();
                    var p = f.getUTCHours();
                    if (p < 10)p = "0" + p;
                    var q = f.getUTCMinutes();
                    if (q <
                        10)q = "0" + q;
                    var o = f.getUTCSeconds();
                    if (o < 10)o = "0" + o;
                    f = f.getUTCMilliseconds();
                    if (f < 100)f = "0" + f;
                    if (f < 10)f = "0" + f;
                    return'"' + a + "-" + g + "-" + h + "T" + p + ":" + q + ":" + o + "." + f + 'Z"'
                }
                if (f.constructor === Array) {
                    g = [];
                    for (h = 0; h < f.length; h++)g.push(c.toJSON(f[h]) || "null");
                    return"[" + g.join(",") + "]"
                }
                g = [];
                for (h in f) {
                    a = typeof h;
                    if (a == "number")a = '"' + h + '"'; else if (a == "string")a = c.quoteString(h); else continue;
                    if (typeof f[h] != "function") {
                        p = c.toJSON(f[h]);
                        g.push(a + ":" + p)
                    }
                }
                return"{" + g.join(", ") + "}"
            }
        }
    };
    c.evalJSON = function (f) {
        if (typeof JSON ==
            "object" && JSON.parse)return JSON.parse(f);
        return eval("(" + f + ")")
    };
    c.secureEvalJSON = function (f) {
        if (typeof JSON == "object" && JSON.parse)return JSON.parse(f);
        var a = f;
        a = a.replace(/\\["\\\/bfnrtu]/g, "@");
        a = a.replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, "]");//"
        a = a.replace(/(?:^|:|,)(?:\s*\[)+/g, "");
        if (/^[\],:{}\s]*$/.test(a))return eval("(" + f + ")"); else throw new SyntaxError("Error parsing JSON, source is not valid.");
    };
    c.quoteString = function (f) {
        if (f.match(b))return'"' + f.replace(b,
            function (a) {
                var g = e[a];
                if (typeof g === "string")return g;
                g = a.charCodeAt();
                return"\\u00" + Math.floor(g / 16).toString(16) + (g % 16).toString(16)
            }) + '"';
        return'"' + f + '"'
    };
	var b = /["\\\x00-\x1f\x7f-\x9f]/g, e = {"\u0008": "\\b", "\t": "\\t", "\n": "\\n", "\u000c": "\\f", "\r": "\\r", '"': '\\"', "\\": "\\\\"};//'"
})(jQuery);
jQuery.cookie = function (c, b, e) {
    if (arguments.length > 1 && String(b) !== "[object Object]") {
        e = jQuery.extend({}, e);
        if (b === null || b === undefined)e.expires = -1;
        if (typeof e.expires === "number") {
            var f = e.expires, a = e.expires = new Date;
            a.setDate(a.getDate() + f)
        }
        b = String(b);
        return document.cookie = [encodeURIComponent(c), "=", e.raw ? b : encodeURIComponent(b), e.expires ? "; expires=" + e.expires.toUTCString() : "", e.path ? "; path=" + e.path : "", e.domain ? "; domain=" + e.domain : "", e.secure ? "; secure" : ""].join("")
    }
    e = b || {};
    a = e.raw ? function (g) {
        return g
    } :
        decodeURIComponent;
    return(f = RegExp("(?:^|; )" + encodeURIComponent(c) + "=([^;]*)").exec(document.cookie)) ? a(f[1]) : null
};
(function (c) {
    c.ui = c.ui || {};
    if (!c.ui.version) {
        c.extend(c.ui, {version: "1.8.2", plugin: {add: function (b, e, f) {
            b = c.ui[b].prototype;
            for (var a in f) {
                b.plugins[a] = b.plugins[a] || [];
                b.plugins[a].push([e, f[a]])
            }
        }, call: function (b, e, f) {
            if ((e = b.plugins[e]) && b.element[0].parentNode)for (var a = 0; a < e.length; a++)b.options[e[a][0]] && e[a][1].apply(b.element, f)
        }}, contains: function (b, e) {
            return document.compareDocumentPosition ? b.compareDocumentPosition(e) & 16 : b !== e && b.contains(e)
        }, hasScroll: function (b, e) {
            if (c(b).css("overflow") ==
                "hidden")return false;
            e = e && e == "left" ? "scrollLeft" : "scrollTop";
            var f = false;
            if (b[e] > 0)return true;
            b[e] = 1;
            f = b[e] > 0;
            b[e] = 0;
            return f
        }, isOverAxis: function (b, e, f) {
            return b > e && b < e + f
        }, isOver: function (b, e, f, a, g, h) {
            return c.ui.isOverAxis(b, f, g) && c.ui.isOverAxis(e, a, h)
        }, keyCode: {ALT: 18, BACKSPACE: 8, CAPS_LOCK: 20, COMMA: 188, COMMAND: 91, COMMAND_LEFT: 91, COMMAND_RIGHT: 93, CONTROL: 17, DELETE: 46, DOWN: 40, END: 35, ENTER: 13, ESCAPE: 27, HOME: 36, INSERT: 45, LEFT: 37, MENU: 93, NUMPAD_ADD: 107, NUMPAD_DECIMAL: 110, NUMPAD_DIVIDE: 111, NUMPAD_ENTER: 108,
            NUMPAD_MULTIPLY: 106, NUMPAD_SUBTRACT: 109, PAGE_DOWN: 34, PAGE_UP: 33, PERIOD: 190, RIGHT: 39, SHIFT: 16, SPACE: 32, TAB: 9, UP: 38, WINDOWS: 91}});
        c.fn.extend({_focus: c.fn.focus, focus: function (b, e) {
            return typeof b === "number" ? this.each(function () {
                var f = this;
                setTimeout(function () {
                    c(f).focus();
                    e && e.call(f)
                }, b)
            }) : this._focus.apply(this, arguments)
        }, enableSelection: function () {
            return this.attr("unselectable", "off").css("MozUserSelect", "")
        }, disableSelection: function () {
            return this.attr("unselectable", "on").css("MozUserSelect",
                "none")
        }, scrollParent: function () {
            var b;
            b = c.browser.msie && /(static|relative)/.test(this.css("position")) || /absolute/.test(this.css("position")) ? this.parents().filter(function () {
                return/(relative|absolute|fixed)/.test(c.curCSS(this, "position", 1)) && /(auto|scroll)/.test(c.curCSS(this, "overflow", 1) + c.curCSS(this, "overflow-y", 1) + c.curCSS(this, "overflow-x", 1))
            }).eq(0) : this.parents().filter(function () {
                return/(auto|scroll)/.test(c.curCSS(this, "overflow", 1) + c.curCSS(this, "overflow-y", 1) + c.curCSS(this, "overflow-x",
                    1))
            }).eq(0);
            return/fixed/.test(this.css("position")) || !b.length ? c(document) : b
        }, zIndex: function (b) {
            if (b !== undefined)return this.css("zIndex", b);
            if (this.length) {
                b = c(this[0]);
                for (var e; b.length && b[0] !== document;) {
                    e = b.css("position");
                    if (e == "absolute" || e == "relative" || e == "fixed") {
                        e = parseInt(b.css("zIndex"));
                        if (!isNaN(e) && e != 0)return e
                    }
                    b = b.parent()
                }
            }
            return 0
        }});
        c.extend(c.expr[":"], {data: function (b, e, f) {
            return!!c.data(b, f[3])
        }, focusable: function (b) {
            var e = b.nodeName.toLowerCase(), f = c.attr(b, "tabindex");
            return(/input|select|textarea|button|object/.test(e) ?
                !b.disabled : "a" == e || "area" == e ? b.href || !isNaN(f) : !isNaN(f)) && !c(b)["area" == e ? "parents" : "closest"](":hidden").length
        }, tabbable: function (b) {
            var e = c.attr(b, "tabindex");
            return(isNaN(e) || e >= 0) && c(b).is(":focusable")
        }})
    }
})(jQuery);
(function (c) {
    var b = c.fn.remove;
    c.fn.remove = function (e, f) {
        return this.each(function () {
            if (!f)if (!e || c.filter(e, [this]).length)c("*", this).add(this).each(function () {
                c(this).triggerHandler("remove")
            });
            return b.call(c(this), e, f)
        })
    };
    c.widget = function (e, f, a) {
        var g = e.split(".")[0], h;
        e = e.split(".")[1];
        h = g + "-" + e;
        if (!a) {
            a = f;
            f = c.Widget
        }
        c.expr[":"][h] = function (p) {
            return!!c.data(p, e)
        };
        c[g] = c[g] || {};
        c[g][e] = function (p, q) {
            arguments.length && this._createWidget(p, q)
        };
        f = new f;
        f.options = c.extend({}, f.options);
        c[g][e].prototype =
            c.extend(true, f, {namespace: g, widgetName: e, widgetEventPrefix: c[g][e].prototype.widgetEventPrefix || e, widgetBaseClass: h}, a);
        c.widget.bridge(e, c[g][e])
    };
    c.widget.bridge = function (e, f) {
        c.fn[e] = function (a) {
            var g = typeof a === "string", h = Array.prototype.slice.call(arguments, 1), p = this;
            a = !g && h.length ? c.extend.apply(null, [true, a].concat(h)) : a;
            if (g && a.substring(0, 1) === "_")return p;
            g ? this.each(function () {
                var q = c.data(this, e), o = q && c.isFunction(q[a]) ? q[a].apply(q, h) : q;
                if (o !== q && o !== undefined) {
                    p = o;
                    return false
                }
            }) : this.each(function () {
                var q =
                    c.data(this, e);
                if (q) {
                    a && q.option(a);
                    q._init()
                } else c.data(this, e, new f(a, this))
            });
            return p
        }
    };
    c.Widget = function (e, f) {
        arguments.length && this._createWidget(e, f)
    };
    c.Widget.prototype = {widgetName: "widget", widgetEventPrefix: "", options: {disabled: false}, _createWidget: function (e, f) {
        this.element = c(f).data(this.widgetName, this);
        this.options = c.extend(true, {}, this.options, c.metadata && c.metadata.get(f)[this.widgetName], e);
        var a = this;
        this.element.bind("remove." + this.widgetName, function () {
            a.destroy()
        });
        this._create();
        this._init()
    }, _create: function () {
    }, _init: function () {
    }, destroy: function () {
        this.element.unbind("." + this.widgetName).removeData(this.widgetName);
        this.widget().unbind("." + this.widgetName).removeAttr("aria-disabled").removeClass(this.widgetBaseClass + "-disabled ui-state-disabled")
    }, widget: function () {
        return this.element
    }, option: function (e, f) {
        var a = e, g = this;
        if (arguments.length === 0)return c.extend({}, g.options);
        if (typeof e === "string") {
            if (f === undefined)return this.options[e];
            a = {};
            a[e] = f
        }
        c.each(a, function (h, p) {
            g._setOption(h, p)
        });
        return g
    }, _setOption: function (e, f) {
        this.options[e] = f;
        if (e === "disabled")this.widget()[f ? "addClass" : "removeClass"](this.widgetBaseClass + "-disabled ui-state-disabled").attr("aria-disabled", f);
        return this
    }, enable: function () {
        return this._setOption("disabled", false)
    }, disable: function () {
        return this._setOption("disabled", true)
    }, _trigger: function (e, f, a) {
        var g = this.options[e];
        f = c.Event(f);
        f.type = (e === this.widgetEventPrefix ? e : this.widgetEventPrefix + e).toLowerCase();
        a = a || {};
        if (f.originalEvent) {
            e =
                c.event.props.length;
            for (var h; e;) {
                h = c.event.props[--e];
                f[h] = f.originalEvent[h]
            }
        }
        this.element.trigger(f, a);
        return!(c.isFunction(g) && g.call(this.element[0], f, a) === false || f.isDefaultPrevented())
    }}
})(jQuery);
(function (c) {
    c.widget("ui.mouse", {options: {cancel: ":input,option", distance: 1, delay: 0}, _mouseInit: function () {
        var b = this;
        this.element.bind("mousedown." + this.widgetName, function (e) {
            return b._mouseDown(e)
        }).bind("click." + this.widgetName, function (e) {
            if (b._preventClickEvent) {
                b._preventClickEvent = false;
                e.stopImmediatePropagation();
                return false
            }
        });
        this.started = false
    }, _mouseDestroy: function () {
        this.element.unbind("." + this.widgetName)
    }, _mouseDown: function (b) {
        b.originalEvent = b.originalEvent || {};
        if (!b.originalEvent.mouseHandled) {
            this._mouseStarted &&
            this._mouseUp(b);
            this._mouseDownEvent = b;
            var e = this, f = b.which == 1, a = typeof this.options.cancel == "string" ? c(b.target).parents().add(b.target).filter(this.options.cancel).length : false;
            if (!f || a || !this._mouseCapture(b))return true;
            this.mouseDelayMet = !this.options.delay;
            if (!this.mouseDelayMet)this._mouseDelayTimer = setTimeout(function () {
                e.mouseDelayMet = true
            }, this.options.delay);
            if (this._mouseDistanceMet(b) && this._mouseDelayMet(b)) {
                this._mouseStarted = this._mouseStart(b) !== false;
                if (!this._mouseStarted) {
                    b.preventDefault();
                    return true
                }
            }
            this._mouseMoveDelegate = function (g) {
                return e._mouseMove(g)
            };
            this._mouseUpDelegate = function (g) {
                return e._mouseUp(g)
            };
            c(document).bind("mousemove." + this.widgetName, this._mouseMoveDelegate).bind("mouseup." + this.widgetName, this._mouseUpDelegate);
            c.browser.safari || b.preventDefault();
            return b.originalEvent.mouseHandled = true
        }
    }, _mouseMove: function (b) {
        if (c.browser.msie && !b.button)return this._mouseUp(b);
        if (this._mouseStarted) {
            this._mouseDrag(b);
            return b.preventDefault()
        }
        if (this._mouseDistanceMet(b) &&
            this._mouseDelayMet(b))(this._mouseStarted = this._mouseStart(this._mouseDownEvent, b) !== false) ? this._mouseDrag(b) : this._mouseUp(b);
        return!this._mouseStarted
    }, _mouseUp: function (b) {
        c(document).unbind("mousemove." + this.widgetName, this._mouseMoveDelegate).unbind("mouseup." + this.widgetName, this._mouseUpDelegate);
        if (this._mouseStarted) {
            this._mouseStarted = false;
            this._preventClickEvent = b.target == this._mouseDownEvent.target;
            this._mouseStop(b)
        }
        return false
    }, _mouseDistanceMet: function (b) {
        return Math.max(Math.abs(this._mouseDownEvent.pageX -
            b.pageX), Math.abs(this._mouseDownEvent.pageY - b.pageY)) >= this.options.distance
    }, _mouseDelayMet: function () {
        return this.mouseDelayMet
    }, _mouseStart: function () {
    }, _mouseDrag: function () {
    }, _mouseStop: function () {
    }, _mouseCapture: function () {
        return true
    }})
})(jQuery);
(function (c) {
    c.widget("ui.draggable", c.ui.mouse, {widgetEventPrefix: "drag", options: {addClasses: true, appendTo: "parent", axis: false, connectToSortable: false, containment: false, cursor: "auto", cursorAt: false, grid: false, handle: false, helper: "original", iframeFix: false, opacity: false, refreshPositions: false, revert: false, revertDuration: 500, scope: "default", scroll: true, scrollSensitivity: 20, scrollSpeed: 20, snap: false, snapMode: "both", snapTolerance: 20, stack: false, zIndex: false}, _create: function () {
        if (this.options.helper ==
            "original" && !/^(?:r|a|f)/.test(this.element.css("position")))this.element[0].style.position = "relative";
        this.options.addClasses && this.element.addClass("ui-draggable");
        this.options.disabled && this.element.addClass("ui-draggable-disabled");
        this._mouseInit()
    }, destroy: function () {
        if (this.element.data("draggable")) {
            this.element.removeData("draggable").unbind(".draggable").removeClass("ui-draggable ui-draggable-dragging ui-draggable-disabled");
            this._mouseDestroy();
            return this
        }
    }, _mouseCapture: function (b) {
        var e =
            this.options;
        if (this.helper || e.disabled || c(b.target).is(".ui-resizable-handle"))return false;
        this.handle = this._getHandle(b);
        if (!this.handle)return false;
        return true
    }, _mouseStart: function (b) {
        var e = this.options;
        this.helper = this._createHelper(b);
        this._cacheHelperProportions();
        if (c.ui.ddmanager)c.ui.ddmanager.current = this;
        this._cacheMargins();
        this.cssPosition = this.helper.css("position");
        this.scrollParent = this.helper.scrollParent();
        this.offset = this.positionAbs = this.element.offset();
        this.offset = {top: this.offset.top -
            this.margins.top, left: this.offset.left - this.margins.left};
        c.extend(this.offset, {click: {left: b.pageX - this.offset.left, top: b.pageY - this.offset.top}, parent: this._getParentOffset(), relative: this._getRelativeOffset()});
        this.originalPosition = this.position = this._generatePosition(b);
        this.originalPageX = b.pageX;
        this.originalPageY = b.pageY;
        e.cursorAt && this._adjustOffsetFromHelper(e.cursorAt);
        e.containment && this._setContainment();
        if (this._trigger("start", b) === false) {
            this._clear();
            return false
        }
        this._cacheHelperProportions();
        c.ui.ddmanager && !e.dropBehaviour && c.ui.ddmanager.prepareOffsets(this, b);
        this.helper.addClass("ui-draggable-dragging");
        this._mouseDrag(b, true);
        return true
    }, _mouseDrag: function (b, e) {
        this.position = this._generatePosition(b);
        this.positionAbs = this._convertPositionTo("absolute");
        if (!e) {
            e = this._uiHash();
            if (this._trigger("drag", b, e) === false) {
                this._mouseUp({});
                return false
            }
            this.position = e.position
        }
        if (!this.options.axis || this.options.axis != "y")this.helper[0].style.left = this.position.left + "px";
        if (!this.options.axis ||
            this.options.axis != "x")this.helper[0].style.top = this.position.top + "px";
        c.ui.ddmanager && c.ui.ddmanager.drag(this, b);
        return false
    }, _mouseStop: function (b) {
        var e = false;
        if (c.ui.ddmanager && !this.options.dropBehaviour)e = c.ui.ddmanager.drop(this, b);
        if (this.dropped) {
            e = this.dropped;
            this.dropped = false
        }
        if (!this.element[0] || !this.element[0].parentNode)return false;
        if (this.options.revert == "invalid" && !e || this.options.revert == "valid" && e || this.options.revert === true || c.isFunction(this.options.revert) && this.options.revert.call(this.element,
            e)) {
            var f = this;
            c(this.helper).animate(this.originalPosition, parseInt(this.options.revertDuration, 10), function () {
                f._trigger("stop", b) !== false && f._clear()
            })
        } else this._trigger("stop", b) !== false && this._clear();
        return false
    }, cancel: function () {
        this.helper.is(".ui-draggable-dragging") ? this._mouseUp({}) : this._clear();
        return this
    }, _getHandle: function (b) {
        var e = !this.options.handle || !c(this.options.handle, this.element).length ? true : false;
        c(this.options.handle, this.element).find("*").andSelf().each(function () {
            if (this ==
                b.target)e = true
        });
        return e
    }, _createHelper: function (b) {
        var e = this.options;
        b = c.isFunction(e.helper) ? c(e.helper.apply(this.element[0], [b])) : e.helper == "clone" ? this.element.clone() : this.element;
        b.parents("body").length || b.appendTo(e.appendTo == "parent" ? this.element[0].parentNode : e.appendTo);
        b[0] != this.element[0] && !/(fixed|absolute)/.test(b.css("position")) && b.css("position", "absolute");
        return b
    }, _adjustOffsetFromHelper: function (b) {
        if (typeof b == "string")b = b.split(" ");
        if (c.isArray(b))b = {left: +b[0], top: +b[1] ||
            0};
        if ("left"in b)this.offset.click.left = b.left + this.margins.left;
        if ("right"in b)this.offset.click.left = this.helperProportions.width - b.right + this.margins.left;
        if ("top"in b)this.offset.click.top = b.top + this.margins.top;
        if ("bottom"in b)this.offset.click.top = this.helperProportions.height - b.bottom + this.margins.top
    }, _getParentOffset: function () {
        this.offsetParent = this.helper.offsetParent();
        var b = this.offsetParent.offset();
        if (this.cssPosition == "absolute" && this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0],
            this.offsetParent[0])) {
            b.left += this.scrollParent.scrollLeft();
            b.top += this.scrollParent.scrollTop()
        }
        if (this.offsetParent[0] == document.body || this.offsetParent[0].tagName && this.offsetParent[0].tagName.toLowerCase() == "html" && c.browser.msie)b = {top: 0, left: 0};
        return{top: b.top + (parseInt(this.offsetParent.css("borderTopWidth"), 10) || 0), left: b.left + (parseInt(this.offsetParent.css("borderLeftWidth"), 10) || 0)}
    }, _getRelativeOffset: function () {
        if (this.cssPosition == "relative") {
            var b = this.element.position();
            return{top: b.top -
                (parseInt(this.helper.css("top"), 10) || 0) + this.scrollParent.scrollTop(), left: b.left - (parseInt(this.helper.css("left"), 10) || 0) + this.scrollParent.scrollLeft()}
        } else return{top: 0, left: 0}
    }, _cacheMargins: function () {
        this.margins = {left: parseInt(this.element.css("marginLeft"), 10) || 0, top: parseInt(this.element.css("marginTop"), 10) || 0}
    }, _cacheHelperProportions: function () {
        this.helperProportions = {width: this.helper.outerWidth(), height: this.helper.outerHeight()}
    }, _setContainment: function () {
        var b = this.options;
        if (b.containment ==
            "parent")b.containment = this.helper[0].parentNode;
        if (b.containment == "document" || b.containment == "window")this.containment = [0 - this.offset.relative.left - this.offset.parent.left, 0 - this.offset.relative.top - this.offset.parent.top, c(b.containment == "document" ? document : window).width() - this.helperProportions.width - this.margins.left, (c(b.containment == "document" ? document : window).height() || document.body.parentNode.scrollHeight) - this.helperProportions.height - this.margins.top];
        if (!/^(document|window|parent)$/.test(b.containment) &&
            b.containment.constructor != Array) {
            var e = c(b.containment)[0];
            if (e) {
                b = c(b.containment).offset();
                var f = c(e).css("overflow") != "hidden";
                this.containment = [b.left + (parseInt(c(e).css("borderLeftWidth"), 10) || 0) + (parseInt(c(e).css("paddingLeft"), 10) || 0) - this.margins.left, b.top + (parseInt(c(e).css("borderTopWidth"), 10) || 0) + (parseInt(c(e).css("paddingTop"), 10) || 0) - this.margins.top, b.left + (f ? Math.max(e.scrollWidth, e.offsetWidth) : e.offsetWidth) - (parseInt(c(e).css("borderLeftWidth"), 10) || 0) - (parseInt(c(e).css("paddingRight"),
                    10) || 0) - this.helperProportions.width - this.margins.left, b.top + (f ? Math.max(e.scrollHeight, e.offsetHeight) : e.offsetHeight) - (parseInt(c(e).css("borderTopWidth"), 10) || 0) - (parseInt(c(e).css("paddingBottom"), 10) || 0) - this.helperProportions.height - this.margins.top]
            }
        } else if (b.containment.constructor == Array)this.containment = b.containment
    }, _convertPositionTo: function (b, e) {
        if (!e)e = this.position;
        b = b == "absolute" ? 1 : -1;
        var f = this.cssPosition == "absolute" && !(this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0],
            this.offsetParent[0])) ? this.offsetParent : this.scrollParent, a = /(html|body)/i.test(f[0].tagName);
        return{top: e.top + this.offset.relative.top * b + this.offset.parent.top * b - (c.browser.safari && c.browser.version < 526 && this.cssPosition == "fixed" ? 0 : (this.cssPosition == "fixed" ? -this.scrollParent.scrollTop() : a ? 0 : f.scrollTop()) * b), left: e.left + this.offset.relative.left * b + this.offset.parent.left * b - (c.browser.safari && c.browser.version < 526 && this.cssPosition == "fixed" ? 0 : (this.cssPosition == "fixed" ? -this.scrollParent.scrollLeft() :
            a ? 0 : f.scrollLeft()) * b)}
    }, _generatePosition: function (b) {
        var e = this.options, f = this.cssPosition == "absolute" && !(this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0], this.offsetParent[0])) ? this.offsetParent : this.scrollParent, a = /(html|body)/i.test(f[0].tagName), g = b.pageX, h = b.pageY;
        if (this.originalPosition) {
            if (this.containment) {
                if (b.pageX - this.offset.click.left < this.containment[0])g = this.containment[0] + this.offset.click.left;
                if (b.pageY - this.offset.click.top < this.containment[1])h = this.containment[1] +
                    this.offset.click.top;
                if (b.pageX - this.offset.click.left > this.containment[2])g = this.containment[2] + this.offset.click.left;
                if (b.pageY - this.offset.click.top > this.containment[3])h = this.containment[3] + this.offset.click.top
            }
            if (e.grid) {
                h = this.originalPageY + Math.round((h - this.originalPageY) / e.grid[1]) * e.grid[1];
                h = this.containment ? !(h - this.offset.click.top < this.containment[1] || h - this.offset.click.top > this.containment[3]) ? h : !(h - this.offset.click.top < this.containment[1]) ? h - e.grid[1] : h + e.grid[1] : h;
                g = this.originalPageX +
                    Math.round((g - this.originalPageX) / e.grid[0]) * e.grid[0];
                g = this.containment ? !(g - this.offset.click.left < this.containment[0] || g - this.offset.click.left > this.containment[2]) ? g : !(g - this.offset.click.left < this.containment[0]) ? g - e.grid[0] : g + e.grid[0] : g
            }
        }
        return{top: h - this.offset.click.top - this.offset.relative.top - this.offset.parent.top + (c.browser.safari && c.browser.version < 526 && this.cssPosition == "fixed" ? 0 : this.cssPosition == "fixed" ? -this.scrollParent.scrollTop() : a ? 0 : f.scrollTop()), left: g - this.offset.click.left -
            this.offset.relative.left - this.offset.parent.left + (c.browser.safari && c.browser.version < 526 && this.cssPosition == "fixed" ? 0 : this.cssPosition == "fixed" ? -this.scrollParent.scrollLeft() : a ? 0 : f.scrollLeft())}
    }, _clear: function () {
        this.helper.removeClass("ui-draggable-dragging");
        this.helper[0] != this.element[0] && !this.cancelHelperRemoval && this.helper.remove();
        this.helper = null;
        this.cancelHelperRemoval = false
    }, _trigger: function (b, e, f) {
        f = f || this._uiHash();
        c.ui.plugin.call(this, b, [e, f]);
        if (b == "drag")this.positionAbs =
            this._convertPositionTo("absolute");
        return c.Widget.prototype._trigger.call(this, b, e, f)
    }, plugins: {}, _uiHash: function () {
        return{helper: this.helper, position: this.position, originalPosition: this.originalPosition, offset: this.positionAbs}
    }});
    c.extend(c.ui.draggable, {version: "1.8.2"});
    c.ui.plugin.add("draggable", "connectToSortable", {start: function (b, e) {
        var f = c(this).data("draggable"), a = f.options, g = c.extend({}, e, {item: f.element});
        f.sortables = [];
        c(a.connectToSortable).each(function () {
            var h = c.data(this, "sortable");
            if (h && !h.options.disabled) {
                f.sortables.push({instance: h, shouldRevert: h.options.revert});
                h._refreshItems();
                h._trigger("activate", b, g)
            }
        })
    }, stop: function (b, e) {
        var f = c(this).data("draggable"), a = c.extend({}, e, {item: f.element});
        c.each(f.sortables, function () {
            if (this.instance.isOver) {
                this.instance.isOver = 0;
                f.cancelHelperRemoval = true;
                this.instance.cancelHelperRemoval = false;
                if (this.shouldRevert)this.instance.options.revert = true;
                this.instance._mouseStop(b);
                this.instance.options.helper = this.instance.options._helper;
                f.options.helper == "original" && this.instance.currentItem.css({top: "auto", left: "auto"})
            } else {
                this.instance.cancelHelperRemoval = false;
                this.instance._trigger("deactivate", b, a)
            }
        })
    }, drag: function (b, e) {
        var f = c(this).data("draggable"), a = this;
        c.each(f.sortables, function () {
            this.instance.positionAbs = f.positionAbs;
            this.instance.helperProportions = f.helperProportions;
            this.instance.offset.click = f.offset.click;
            if (this.instance._intersectsWith(this.instance.containerCache)) {
                if (!this.instance.isOver) {
                    this.instance.isOver =
                        1;
                    this.instance.currentItem = c(a).clone().appendTo(this.instance.element).data("sortable-item", true);
                    this.instance.options._helper = this.instance.options.helper;
                    this.instance.options.helper = function () {
                        return e.helper[0]
                    };
                    b.target = this.instance.currentItem[0];
                    this.instance._mouseCapture(b, true);
                    this.instance._mouseStart(b, true, true);
                    this.instance.offset.click.top = f.offset.click.top;
                    this.instance.offset.click.left = f.offset.click.left;
                    this.instance.offset.parent.left -= f.offset.parent.left - this.instance.offset.parent.left;
                    this.instance.offset.parent.top -= f.offset.parent.top - this.instance.offset.parent.top;
                    f._trigger("toSortable", b);
                    f.dropped = this.instance.element;
                    f.currentItem = f.element;
                    this.instance.fromOutside = f
                }
                this.instance.currentItem && this.instance._mouseDrag(b)
            } else if (this.instance.isOver) {
                this.instance.isOver = 0;
                this.instance.cancelHelperRemoval = true;
                this.instance.options.revert = false;
                this.instance._trigger("out", b, this.instance._uiHash(this.instance));
                this.instance._mouseStop(b, true);
                this.instance.options.helper =
                    this.instance.options._helper;
                this.instance.currentItem.remove();
                this.instance.placeholder && this.instance.placeholder.remove();
                f._trigger("fromSortable", b);
                f.dropped = false
            }
        })
    }});
    c.ui.plugin.add("draggable", "cursor", {start: function () {
        var b = c("body"), e = c(this).data("draggable").options;
        if (b.css("cursor"))e._cursor = b.css("cursor");
        b.css("cursor", e.cursor)
    }, stop: function () {
        var b = c(this).data("draggable").options;
        b._cursor && c("body").css("cursor", b._cursor)
    }});
    c.ui.plugin.add("draggable", "iframeFix", {start: function () {
        var b =
            c(this).data("draggable").options;
        c(b.iframeFix === true ? "iframe" : b.iframeFix).each(function () {
            c('<div class="ui-draggable-iframeFix" style="background: #fff;"></div>').css({width: this.offsetWidth + "px", height: this.offsetHeight + "px", position: "absolute", opacity: "0.001", zIndex: 1E3}).css(c(this).offset()).appendTo("body")
        })
    }, stop: function () {
        c("div.ui-draggable-iframeFix").each(function () {
            this.parentNode.removeChild(this)
        })
    }});
    c.ui.plugin.add("draggable", "opacity", {start: function (b, e) {
        b = c(e.helper);
        e = c(this).data("draggable").options;
        if (b.css("opacity"))e._opacity = b.css("opacity");
        b.css("opacity", e.opacity)
    }, stop: function (b, e) {
        b = c(this).data("draggable").options;
        b._opacity && c(e.helper).css("opacity", b._opacity)
    }});
    c.ui.plugin.add("draggable", "scroll", {start: function () {
        var b = c(this).data("draggable");
        if (b.scrollParent[0] != document && b.scrollParent[0].tagName != "HTML")b.overflowOffset = b.scrollParent.offset()
    }, drag: function (b) {
        var e = c(this).data("draggable"), f = e.options, a = false;
        if (e.scrollParent[0] != document && e.scrollParent[0].tagName !=
            "HTML") {
            if (!f.axis || f.axis != "x")if (e.overflowOffset.top + e.scrollParent[0].offsetHeight - b.pageY < f.scrollSensitivity)e.scrollParent[0].scrollTop = a = e.scrollParent[0].scrollTop + f.scrollSpeed; else if (b.pageY - e.overflowOffset.top < f.scrollSensitivity)e.scrollParent[0].scrollTop = a = e.scrollParent[0].scrollTop - f.scrollSpeed;
            if (!f.axis || f.axis != "y")if (e.overflowOffset.left + e.scrollParent[0].offsetWidth - b.pageX < f.scrollSensitivity)e.scrollParent[0].scrollLeft = a = e.scrollParent[0].scrollLeft + f.scrollSpeed; else if (b.pageX -
                e.overflowOffset.left < f.scrollSensitivity)e.scrollParent[0].scrollLeft = a = e.scrollParent[0].scrollLeft - f.scrollSpeed
        } else {
            if (!f.axis || f.axis != "x")if (b.pageY - c(document).scrollTop() < f.scrollSensitivity)a = c(document).scrollTop(c(document).scrollTop() - f.scrollSpeed); else if (c(window).height() - (b.pageY - c(document).scrollTop()) < f.scrollSensitivity)a = c(document).scrollTop(c(document).scrollTop() + f.scrollSpeed);
            if (!f.axis || f.axis != "y")if (b.pageX - c(document).scrollLeft() < f.scrollSensitivity)a = c(document).scrollLeft(c(document).scrollLeft() -
                f.scrollSpeed); else if (c(window).width() - (b.pageX - c(document).scrollLeft()) < f.scrollSensitivity)a = c(document).scrollLeft(c(document).scrollLeft() + f.scrollSpeed)
        }
        a !== false && c.ui.ddmanager && !f.dropBehaviour && c.ui.ddmanager.prepareOffsets(e, b)
    }});
    c.ui.plugin.add("draggable", "snap", {start: function () {
        var b = c(this).data("draggable"), e = b.options;
        b.snapElements = [];
        c(e.snap.constructor != String ? e.snap.items || ":data(draggable)" : e.snap).each(function () {
            var f = c(this), a = f.offset();
            this != b.element[0] && b.snapElements.push({item: this,
                width: f.outerWidth(), height: f.outerHeight(), top: a.top, left: a.left})
        })
    }, drag: function (b, e) {
        for (var f = c(this).data("draggable"), a = f.options, g = a.snapTolerance, h = e.offset.left, p = h + f.helperProportions.width, q = e.offset.top, o = q + f.helperProportions.height, l = f.snapElements.length - 1; l >= 0; l--) {
            var r = f.snapElements[l].left, u = r + f.snapElements[l].width, y = f.snapElements[l].top, A = y + f.snapElements[l].height;
            if (r - g < h && h < u + g && y - g < q && q < A + g || r - g < h && h < u + g && y - g < o && o < A + g || r - g < p && p < u + g && y - g < q && q < A + g || r - g < p && p < u + g && y - g < o &&
                o < A + g) {
                if (a.snapMode != "inner") {
                    var i = Math.abs(y - o) <= g, J = Math.abs(A - q) <= g, K = Math.abs(r - p) <= g, D = Math.abs(u - h) <= g;
                    if (i)e.position.top = f._convertPositionTo("relative", {top: y - f.helperProportions.height, left: 0}).top - f.margins.top;
                    if (J)e.position.top = f._convertPositionTo("relative", {top: A, left: 0}).top - f.margins.top;
                    if (K)e.position.left = f._convertPositionTo("relative", {top: 0, left: r - f.helperProportions.width}).left - f.margins.left;
                    if (D)e.position.left = f._convertPositionTo("relative", {top: 0, left: u}).left - f.margins.left
                }
                var P =
                    i || J || K || D;
                if (a.snapMode != "outer") {
                    i = Math.abs(y - q) <= g;
                    J = Math.abs(A - o) <= g;
                    K = Math.abs(r - h) <= g;
                    D = Math.abs(u - p) <= g;
                    if (i)e.position.top = f._convertPositionTo("relative", {top: y, left: 0}).top - f.margins.top;
                    if (J)e.position.top = f._convertPositionTo("relative", {top: A - f.helperProportions.height, left: 0}).top - f.margins.top;
                    if (K)e.position.left = f._convertPositionTo("relative", {top: 0, left: r}).left - f.margins.left;
                    if (D)e.position.left = f._convertPositionTo("relative", {top: 0, left: u - f.helperProportions.width}).left - f.margins.left
                }
                if (!f.snapElements[l].snapping &&
                    (i || J || K || D || P))f.options.snap.snap && f.options.snap.snap.call(f.element, b, c.extend(f._uiHash(), {snapItem: f.snapElements[l].item}));
                f.snapElements[l].snapping = i || J || K || D || P
            } else {
                f.snapElements[l].snapping && f.options.snap.release && f.options.snap.release.call(f.element, b, c.extend(f._uiHash(), {snapItem: f.snapElements[l].item}));
                f.snapElements[l].snapping = false
            }
        }
    }});
    c.ui.plugin.add("draggable", "stack", {start: function () {
        var b = c(this).data("draggable").options;
        b = c.makeArray(c(b.stack)).sort(function (f, a) {
            return(parseInt(c(f).css("zIndex"),
                10) || 0) - (parseInt(c(a).css("zIndex"), 10) || 0)
        });
        if (b.length) {
            var e = parseInt(b[0].style.zIndex) || 0;
            c(b).each(function (f) {
                this.style.zIndex = e + f
            });
            this[0].style.zIndex = e + b.length
        }
    }});
    c.ui.plugin.add("draggable", "zIndex", {start: function (b, e) {
        b = c(e.helper);
        e = c(this).data("draggable").options;
        if (b.css("zIndex"))e._zIndex = b.css("zIndex");
        b.css("zIndex", e.zIndex)
    }, stop: function (b, e) {
        b = c(this).data("draggable").options;
        b._zIndex && c(e.helper).css("zIndex", b._zIndex)
    }})
})(jQuery);
(function (c) {
    c.widget("ui.droppable", {widgetEventPrefix: "drop", options: {accept: "*", activeClass: false, addClasses: true, greedy: false, hoverClass: false, scope: "default", tolerance: "intersect"}, _create: function () {
        var b = this.options, e = b.accept;
        this.isover = 0;
        this.isout = 1;
        this.accept = c.isFunction(e) ? e : function (f) {
            return f.is(e)
        };
        this.proportions = {width: this.element[0].offsetWidth, height: this.element[0].offsetHeight};
        c.ui.ddmanager.droppables[b.scope] = c.ui.ddmanager.droppables[b.scope] || [];
        c.ui.ddmanager.droppables[b.scope].push(this);
        b.addClasses && this.element.addClass("ui-droppable")
    }, destroy: function () {
        for (var b = c.ui.ddmanager.droppables[this.options.scope], e = 0; e < b.length; e++)b[e] == this && b.splice(e, 1);
        this.element.removeClass("ui-droppable ui-droppable-disabled").removeData("droppable").unbind(".droppable");
        return this
    }, _setOption: function (b, e) {
        if (b == "accept")this.accept = c.isFunction(e) ? e : function (f) {
            return f.is(e)
        };
        c.Widget.prototype._setOption.apply(this, arguments)
    }, _activate: function (b) {
        var e = c.ui.ddmanager.current;
        this.options.activeClass &&
        this.element.addClass(this.options.activeClass);
        e && this._trigger("activate", b, this.ui(e))
    }, _deactivate: function (b) {
        var e = c.ui.ddmanager.current;
        this.options.activeClass && this.element.removeClass(this.options.activeClass);
        e && this._trigger("deactivate", b, this.ui(e))
    }, _over: function (b) {
        var e = c.ui.ddmanager.current;
        if (!(!e || (e.currentItem || e.element)[0] == this.element[0]))if (this.accept.call(this.element[0], e.currentItem || e.element)) {
            this.options.hoverClass && this.element.addClass(this.options.hoverClass);
            this._trigger("over", b, this.ui(e))
        }
    }, _out: function (b) {
        var e = c.ui.ddmanager.current;
        if (!(!e || (e.currentItem || e.element)[0] == this.element[0]))if (this.accept.call(this.element[0], e.currentItem || e.element)) {
            this.options.hoverClass && this.element.removeClass(this.options.hoverClass);
            this._trigger("out", b, this.ui(e))
        }
    }, _drop: function (b, e) {
        var f = e || c.ui.ddmanager.current;
        if (!f || (f.currentItem || f.element)[0] == this.element[0])return false;
        var a = false;
        this.element.find(":data(droppable)").not(".ui-draggable-dragging").each(function () {
            var g =
                c.data(this, "droppable");
            if (g.options.greedy && !g.options.disabled && g.options.scope == f.options.scope && g.accept.call(g.element[0], f.currentItem || f.element) && c.ui.intersect(f, c.extend(g, {offset: g.element.offset()}), g.options.tolerance)) {
                a = true;
                return false
            }
        });
        if (a)return false;
        if (this.accept.call(this.element[0], f.currentItem || f.element)) {
            this.options.activeClass && this.element.removeClass(this.options.activeClass);
            this.options.hoverClass && this.element.removeClass(this.options.hoverClass);
            this._trigger("drop",
                b, this.ui(f));
            return this.element
        }
        return false
    }, ui: function (b) {
        return{draggable: b.currentItem || b.element, helper: b.helper, position: b.position, offset: b.positionAbs}
    }});
    c.extend(c.ui.droppable, {version: "1.8.2"});
    c.ui.intersect = function (b, e, f) {
        if (!e.offset)return false;
        var a = (b.positionAbs || b.position.absolute).left, g = a + b.helperProportions.width, h = (b.positionAbs || b.position.absolute).top, p = h + b.helperProportions.height, q = e.offset.left, o = q + e.proportions.width, l = e.offset.top, r = l + e.proportions.height;
        switch (f) {
            case "fit":
                return q <
                    a && g < o && l < h && p < r;
            case "intersect":
                return q < a + b.helperProportions.width / 2 && g - b.helperProportions.width / 2 < o && l < h + b.helperProportions.height / 2 && p - b.helperProportions.height / 2 < r;
            case "pointer":
                return c.ui.isOver((b.positionAbs || b.position.absolute).top + (b.clickOffset || b.offset.click).top, (b.positionAbs || b.position.absolute).left + (b.clickOffset || b.offset.click).left, l, q, e.proportions.height, e.proportions.width);
            case "touch":
                return(h >= l && h <= r || p >= l && p <= r || h < l && p > r) && (a >= q && a <= o || g >= q && g <= o || a < q && g > o);
            default:
                return false
        }
    };
    c.ui.ddmanager = {current: null, droppables: {"default": []}, prepareOffsets: function (b, e) {
        var f = c.ui.ddmanager.droppables[b.options.scope] || [], a = e ? e.type : null, g = (b.currentItem || b.element).find(":data(droppable)").andSelf(), h = 0;
        a:for (; h < f.length; h++)if (!(f[h].options.disabled || b && !f[h].accept.call(f[h].element[0], b.currentItem || b.element))) {
            for (var p = 0; p < g.length; p++)if (g[p] == f[h].element[0]) {
                f[h].proportions.height = 0;
                continue a
            }
            f[h].visible = f[h].element.css("display") != "none";
            if (f[h].visible) {
                f[h].offset =
                    f[h].element.offset();
                f[h].proportions = {width: f[h].element[0].offsetWidth, height: f[h].element[0].offsetHeight};
                a == "mousedown" && f[h]._activate.call(f[h], e)
            }
        }
    }, drop: function (b, e) {
        var f = false;
        c.each(c.ui.ddmanager.droppables[b.options.scope] || [], function () {
            if (this.options) {
                if (!this.options.disabled && this.visible && c.ui.intersect(b, this, this.options.tolerance))f = f || this._drop.call(this, e);
                if (!this.options.disabled && this.visible && this.accept.call(this.element[0], b.currentItem || b.element)) {
                    this.isout = 1;
                    this.isover = 0;
                    this._deactivate.call(this, e)
                }
            }
        });
        return f
    }, drag: function (b, e) {
        b.options.refreshPositions && c.ui.ddmanager.prepareOffsets(b, e);
        c.each(c.ui.ddmanager.droppables[b.options.scope] || [], function () {
            if (!(this.options.disabled || this.greedyChild || !this.visible)) {
                var f = c.ui.intersect(b, this, this.options.tolerance);
                if (f = !f && this.isover == 1 ? "isout" : f && this.isover == 0 ? "isover" : null) {
                    var a;
                    if (this.options.greedy) {
                        var g = this.element.parents(":data(droppable):eq(0)");
                        if (g.length) {
                            a = c.data(g[0], "droppable");
                            a.greedyChild = f == "isover" ? 1 : 0
                        }
                    }
                    if (a && f == "isover") {
                        a.isover = 0;
                        a.isout = 1;
                        a._out.call(a, e)
                    }
                    this[f] = 1;
                    this[f == "isout" ? "isover" : "isout"] = 0;
                    this[f == "isover" ? "_over" : "_out"].call(this, e);
                    if (a && f == "isout") {
                        a.isout = 0;
                        a.isover = 1;
                        a._over.call(a, e)
                    }
                }
            }
        })
    }}
})(jQuery);
(function (c) {
    c.widget("ui.resizable", c.ui.mouse, {widgetEventPrefix: "resize", options: {alsoResize: false, animate: false, animateDuration: "slow", animateEasing: "swing", aspectRatio: false, autoHide: false, containment: false, ghost: false, grid: false, handles: "e,s,se", helper: false, maxHeight: null, maxWidth: null, minHeight: 10, minWidth: 10, zIndex: 1E3}, _create: function () {
        var f = this, a = this.options;
        this.element.addClass("ui-resizable");
        c.extend(this, {_aspectRatio: !!a.aspectRatio, aspectRatio: a.aspectRatio, originalElement: this.element,
            _proportionallyResizeElements: [], _helper: a.helper || a.ghost || a.animate ? a.helper || "ui-resizable-helper" : null});
        if (this.element[0].nodeName.match(/canvas|textarea|input|select|button|img/i)) {
            /relative/.test(this.element.css("position")) && c.browser.opera && this.element.css({position: "relative", top: "auto", left: "auto"});
            this.element.wrap(c('<div class="ui-wrapper" style="overflow: hidden;"></div>').css({position: this.element.css("position"), width: this.element.outerWidth(), height: this.element.outerHeight(),
                top: this.element.css("top"), left: this.element.css("left")}));
            this.element = this.element.parent().data("resizable", this.element.data("resizable"));
            this.elementIsWrapper = true;
            this.element.css({marginLeft: this.originalElement.css("marginLeft"), marginTop: this.originalElement.css("marginTop"), marginRight: this.originalElement.css("marginRight"), marginBottom: this.originalElement.css("marginBottom")});
            this.originalElement.css({marginLeft: 0, marginTop: 0, marginRight: 0, marginBottom: 0});
            this.originalResizeStyle =
                this.originalElement.css("resize");
            this.originalElement.css("resize", "none");
            this._proportionallyResizeElements.push(this.originalElement.css({position: "static", zoom: 1, display: "block"}));
            this.originalElement.css({margin: this.originalElement.css("margin")});
            this._proportionallyResize()
        }
        this.handles = a.handles || (!c(".ui-resizable-handle", this.element).length ? "e,s,se" : {n: ".ui-resizable-n", e: ".ui-resizable-e", s: ".ui-resizable-s", w: ".ui-resizable-w", se: ".ui-resizable-se", sw: ".ui-resizable-sw", ne: ".ui-resizable-ne",
            nw: ".ui-resizable-nw"});
        if (this.handles.constructor == String) {
            if (this.handles == "all")this.handles = "n,e,s,w,se,sw,ne,nw";
            var g = this.handles.split(",");
            this.handles = {};
            for (var h = 0; h < g.length; h++) {
                var p = c.trim(g[h]), q = c('<div class="ui-resizable-handle ' + ("ui-resizable-" + p) + '"></div>');
                /sw|se|ne|nw/.test(p) && q.css({zIndex: ++a.zIndex});
                "se" == p && q.addClass("ui-icon ui-icon-gripsmall-diagonal-se");
                this.handles[p] = ".ui-resizable-" + p;
                this.element.append(q)
            }
        }
        this._renderAxis = function (o) {
            o = o || this.element;
            for (var l in this.handles) {
                if (this.handles[l].constructor ==
                    String)this.handles[l] = c(this.handles[l], this.element).show();
                if (this.elementIsWrapper && this.originalElement[0].nodeName.match(/textarea|input|select|button/i)) {
                    var r = c(this.handles[l], this.element), u = 0;
                    u = /sw|ne|nw|se|n|s/.test(l) ? r.outerHeight() : r.outerWidth();
                    r = ["padding", /ne|nw|n/.test(l) ? "Top" : /se|sw|s/.test(l) ? "Bottom" : /^e$/.test(l) ? "Right" : "Left"].join("");
                    o.css(r, u);
                    this._proportionallyResize()
                }
                c(this.handles[l])
            }
        };
        this._renderAxis(this.element);
        this._handles = c(".ui-resizable-handle", this.element).disableSelection();
        this._handles.mouseover(function () {
            if (!f.resizing) {
                if (this.className)var o = this.className.match(/ui-resizable-(se|sw|ne|nw|n|e|s|w)/i);
                f.axis = o && o[1] ? o[1] : "se"
            }
        });
        if (a.autoHide) {
            this._handles.hide();
            c(this.element).addClass("ui-resizable-autohide").hover(function () {
                c(this).removeClass("ui-resizable-autohide");
                f._handles.show()
            }, function () {
                if (!f.resizing) {
                    c(this).addClass("ui-resizable-autohide");
                    f._handles.hide()
                }
            })
        }
        this._mouseInit()
    }, destroy: function () {
        this._mouseDestroy();
        var f = function (g) {
            c(g).removeClass("ui-resizable ui-resizable-disabled ui-resizable-resizing").removeData("resizable").unbind(".resizable").find(".ui-resizable-handle").remove()
        };
        if (this.elementIsWrapper) {
            f(this.element);
            var a = this.element;
            a.after(this.originalElement.css({position: a.css("position"), width: a.outerWidth(), height: a.outerHeight(), top: a.css("top"), left: a.css("left")})).remove()
        }
        this.originalElement.css("resize", this.originalResizeStyle);
        f(this.originalElement);
        return this
    }, _mouseCapture: function (f) {
        var a = false;
        for (var g in this.handles)if (c(this.handles[g])[0] == f.target)a = true;
        return!this.options.disabled && a
    }, _mouseStart: function (f) {
        var a = this.options, g = this.element.position(),
            h = this.element;
        this.resizing = true;
        this.documentScroll = {top: c(document).scrollTop(), left: c(document).scrollLeft()};
        if (h.is(".ui-draggable") || /absolute/.test(h.css("position")))h.css({position: "absolute", top: g.top, left: g.left});
        c.browser.opera && /relative/.test(h.css("position")) && h.css({position: "relative", top: "auto", left: "auto"});
        this._renderProxy();
        g = b(this.helper.css("left"));
        var p = b(this.helper.css("top"));
        if (a.containment) {
            g += c(a.containment).scrollLeft() || 0;
            p += c(a.containment).scrollTop() || 0
        }
        this.offset =
            this.helper.offset();
        this.position = {left: g, top: p};
        this.size = this._helper ? {width: h.outerWidth(), height: h.outerHeight()} : {width: h.width(), height: h.height()};
        this.originalSize = this._helper ? {width: h.outerWidth(), height: h.outerHeight()} : {width: h.width(), height: h.height()};
        this.originalPosition = {left: g, top: p};
        this.sizeDiff = {width: h.outerWidth() - h.width(), height: h.outerHeight() - h.height()};
        this.originalMousePosition = {left: f.pageX, top: f.pageY};
        this.aspectRatio = typeof a.aspectRatio == "number" ? a.aspectRatio :
            this.originalSize.width / this.originalSize.height || 1;
        a = c(".ui-resizable-" + this.axis).css("cursor");
        c("body").css("cursor", a == "auto" ? this.axis + "-resize" : a);
        h.addClass("ui-resizable-resizing");
        this._propagate("start", f);
        return true
    }, _mouseDrag: function (f) {
        var a = this.helper, g = this.originalMousePosition, h = this._change[this.axis];
        if (!h)return false;
        g = h.apply(this, [f, f.pageX - g.left || 0, f.pageY - g.top || 0]);
        if (this._aspectRatio || f.shiftKey)g = this._updateRatio(g, f);
        g = this._respectSize(g, f);
        this._propagate("resize",
            f);
        a.css({top: this.position.top + "px", left: this.position.left + "px", width: this.size.width + "px", height: this.size.height + "px"});
        !this._helper && this._proportionallyResizeElements.length && this._proportionallyResize();
        this._updateCache(g);
        this._trigger("resize", f, this.ui());
        return false
    }, _mouseStop: function (f) {
        this.resizing = false;
        var a = this.options;
        if (this._helper) {
            var g = this._proportionallyResizeElements, h = g.length && /textarea/i.test(g[0].nodeName);
            g = h && c.ui.hasScroll(g[0], "left") ? 0 : this.sizeDiff.height;
            h = {width: this.size.width - (h ? 0 : this.sizeDiff.width), height: this.size.height - g};
            g = parseInt(this.element.css("left"), 10) + (this.position.left - this.originalPosition.left) || null;
            var p = parseInt(this.element.css("top"), 10) + (this.position.top - this.originalPosition.top) || null;
            a.animate || this.element.css(c.extend(h, {top: p, left: g}));
            this.helper.height(this.size.height);
            this.helper.width(this.size.width);
            this._helper && !a.animate && this._proportionallyResize()
        }
        c("body").css("cursor", "auto");
        this.element.removeClass("ui-resizable-resizing");
        this._propagate("stop", f);
        this._helper && this.helper.remove();
        return false
    }, _updateCache: function (f) {
        this.offset = this.helper.offset();
        if (e(f.left))this.position.left = f.left;
        if (e(f.top))this.position.top = f.top;
        if (e(f.height))this.size.height = f.height;
        if (e(f.width))this.size.width = f.width
    }, _updateRatio: function (f) {
        var a = this.position, g = this.size, h = this.axis;
        if (f.height)f.width = g.height * this.aspectRatio; else if (f.width)f.height = g.width / this.aspectRatio;
        if (h == "sw") {
            f.left = a.left + (g.width - f.width);
            f.top =
                null
        }
        if (h == "nw") {
            f.top = a.top + (g.height - f.height);
            f.left = a.left + (g.width - f.width)
        }
        return f
    }, _respectSize: function (f) {
        var a = this.options, g = this.axis, h = e(f.width) && a.maxWidth && a.maxWidth < f.width, p = e(f.height) && a.maxHeight && a.maxHeight < f.height, q = e(f.width) && a.minWidth && a.minWidth > f.width, o = e(f.height) && a.minHeight && a.minHeight > f.height;
        if (q)f.width = a.minWidth;
        if (o)f.height = a.minHeight;
        if (h)f.width = a.maxWidth;
        if (p)f.height = a.maxHeight;
        var l = this.originalPosition.left + this.originalSize.width, r = this.position.top +
            this.size.height, u = /sw|nw|w/.test(g);
        g = /nw|ne|n/.test(g);
        if (q && u)f.left = l - a.minWidth;
        if (h && u)f.left = l - a.maxWidth;
        if (o && g)f.top = r - a.minHeight;
        if (p && g)f.top = r - a.maxHeight;
        if ((a = !f.width && !f.height) && !f.left && f.top)f.top = null; else if (a && !f.top && f.left)f.left = null;
        return f
    }, _proportionallyResize: function () {
        if (this._proportionallyResizeElements.length)for (var f = this.helper || this.element, a = 0; a < this._proportionallyResizeElements.length; a++) {
            var g = this._proportionallyResizeElements[a];
            if (!this.borderDif) {
                var h =
                    [g.css("borderTopWidth"), g.css("borderRightWidth"), g.css("borderBottomWidth"), g.css("borderLeftWidth")], p = [g.css("paddingTop"), g.css("paddingRight"), g.css("paddingBottom"), g.css("paddingLeft")];
                this.borderDif = c.map(h, function (q, o) {
                    q = parseInt(q, 10) || 0;
                    o = parseInt(p[o], 10) || 0;
                    return q + o
                })
            }
            c.browser.msie && (c(f).is(":hidden") || c(f).parents(":hidden").length) || g.css({height: f.height() - this.borderDif[0] - this.borderDif[2] || 0, width: f.width() - this.borderDif[1] - this.borderDif[3] || 0})
        }
    }, _renderProxy: function () {
        var f =
            this.options;
        this.elementOffset = this.element.offset();
        if (this._helper) {
            this.helper = this.helper || c('<div style="overflow:hidden;"></div>');
            var a = c.browser.msie && c.browser.version < 7, g = a ? 1 : 0;
            a = a ? 2 : -1;
            this.helper.addClass(this._helper).css({width: this.element.outerWidth() + a, height: this.element.outerHeight() + a, position: "absolute", left: this.elementOffset.left - g + "px", top: this.elementOffset.top - g + "px", zIndex: ++f.zIndex});
            this.helper.appendTo("body").disableSelection()
        } else this.helper = this.element
    }, _change: {e: function (f, a) {
        return{width: this.originalSize.width + a}
    }, w: function (f, a) {
        return{left: this.originalPosition.left + a, width: this.originalSize.width - a}
    }, n: function (f, a, g) {
        return{top: this.originalPosition.top + g, height: this.originalSize.height - g}
    }, s: function (f, a, g) {
        return{height: this.originalSize.height + g}
    }, se: function (f, a, g) {
        return c.extend(this._change.s.apply(this, arguments), this._change.e.apply(this, [f, a, g]))
    }, sw: function (f, a, g) {
        return c.extend(this._change.s.apply(this, arguments), this._change.w.apply(this, [f, a,
            g]))
    }, ne: function (f, a, g) {
        return c.extend(this._change.n.apply(this, arguments), this._change.e.apply(this, [f, a, g]))
    }, nw: function (f, a, g) {
        return c.extend(this._change.n.apply(this, arguments), this._change.w.apply(this, [f, a, g]))
    }}, _propagate: function (f, a) {
        c.ui.plugin.call(this, f, [a, this.ui()]);
        f != "resize" && this._trigger(f, a, this.ui())
    }, plugins: {}, ui: function () {
        return{originalElement: this.originalElement, element: this.element, helper: this.helper, position: this.position, size: this.size, originalSize: this.originalSize,
            originalPosition: this.originalPosition}
    }});
    c.extend(c.ui.resizable, {version: "1.8.2"});
    c.ui.plugin.add("resizable", "alsoResize", {start: function () {
        var f = c(this).data("resizable").options, a = function (g) {
            c(g).each(function () {
                c(this).data("resizable-alsoresize", {width: parseInt(c(this).width(), 10), height: parseInt(c(this).height(), 10), left: parseInt(c(this).css("left"), 10), top: parseInt(c(this).css("top"), 10)})
            })
        };
        if (typeof f.alsoResize == "object" && !f.alsoResize.parentNode)if (f.alsoResize.length) {
            f.alsoResize =
                f.alsoResize[0];
            a(f.alsoResize)
        } else c.each(f.alsoResize, function (g) {
            a(g)
        }); else a(f.alsoResize)
    }, resize: function () {
        var f = c(this).data("resizable"), a = f.options, g = f.originalSize, h = f.originalPosition, p = {height: f.size.height - g.height || 0, width: f.size.width - g.width || 0, top: f.position.top - h.top || 0, left: f.position.left - h.left || 0}, q = function (o, l) {
            c(o).each(function () {
                var r = c(this), u = c(this).data("resizable-alsoresize"), y = {};
                c.each((l && l.length ? l : ["width", "height", "top", "left"]) || ["width", "height", "top", "left"],
                    function (A, i) {
                        if ((A = (u[i] || 0) + (p[i] || 0)) && A >= 0)y[i] = A || null
                    });
                if (/relative/.test(r.css("position")) && c.browser.opera) {
                    f._revertToRelativePosition = true;
                    r.css({position: "absolute", top: "auto", left: "auto"})
                }
                r.css(y)
            })
        };
        typeof a.alsoResize == "object" && !a.alsoResize.nodeType ? c.each(a.alsoResize, function (o, l) {
            q(o, l)
        }) : q(a.alsoResize)
    }, stop: function () {
        var f = c(this).data("resizable");
        if (f._revertToRelativePosition && c.browser.opera) {
            f._revertToRelativePosition = false;
            el.css({position: "relative"})
        }
        c(this).removeData("resizable-alsoresize-start")
    }});
    c.ui.plugin.add("resizable", "animate", {stop: function (f) {
        var a = c(this).data("resizable"), g = a.options, h = a._proportionallyResizeElements, p = h.length && /textarea/i.test(h[0].nodeName), q = p && c.ui.hasScroll(h[0], "left") ? 0 : a.sizeDiff.height;
        p = {width: a.size.width - (p ? 0 : a.sizeDiff.width), height: a.size.height - q};
        q = parseInt(a.element.css("left"), 10) + (a.position.left - a.originalPosition.left) || null;
        var o = parseInt(a.element.css("top"), 10) + (a.position.top - a.originalPosition.top) || null;
        a.element.animate(c.extend(p, o &&
            q ? {top: o, left: q} : {}), {duration: g.animateDuration, easing: g.animateEasing, step: function () {
            var l = {width: parseInt(a.element.css("width"), 10), height: parseInt(a.element.css("height"), 10), top: parseInt(a.element.css("top"), 10), left: parseInt(a.element.css("left"), 10)};
            h && h.length && c(h[0]).css({width: l.width, height: l.height});
            a._updateCache(l);
            a._propagate("resize", f)
        }})
    }});
    c.ui.plugin.add("resizable", "containment", {start: function () {
        var f = c(this).data("resizable"), a = f.element, g = f.options.containment;
        if (a = g instanceof
            c ? g.get(0) : /parent/.test(g) ? a.parent().get(0) : g) {
            f.containerElement = c(a);
            if (/document/.test(g) || g == document) {
                f.containerOffset = {left: 0, top: 0};
                f.containerPosition = {left: 0, top: 0};
                f.parentData = {element: c(document), left: 0, top: 0, width: c(document).width(), height: c(document).height() || document.body.parentNode.scrollHeight}
            } else {
                var h = c(a), p = [];
                c(["Top", "Right", "Left", "Bottom"]).each(function (l, r) {
                    p[l] = b(h.css("padding" + r))
                });
                f.containerOffset = h.offset();
                f.containerPosition = h.position();
                f.containerSize = {height: h.innerHeight() -
                    p[3], width: h.innerWidth() - p[1]};
                g = f.containerOffset;
                var q = f.containerSize.height, o = f.containerSize.width;
                o = c.ui.hasScroll(a, "left") ? a.scrollWidth : o;
                q = c.ui.hasScroll(a) ? a.scrollHeight : q;
                f.parentData = {element: a, left: g.left, top: g.top, width: o, height: q}
            }
        }
    }, resize: function (f) {
        var a = c(this).data("resizable"), g = a.options, h = a.containerOffset, p = a.position;
        f = a._aspectRatio || f.shiftKey;
        var q = {top: 0, left: 0}, o = a.containerElement;
        if (o[0] != document && /static/.test(o.css("position")))q = h;
        if (p.left < (a._helper ? h.left :
            0)) {
            a.size.width += a._helper ? a.position.left - h.left : a.position.left - q.left;
            if (f)a.size.height = a.size.width / g.aspectRatio;
            a.position.left = g.helper ? h.left : 0
        }
        if (p.top < (a._helper ? h.top : 0)) {
            a.size.height += a._helper ? a.position.top - h.top : a.position.top;
            if (f)a.size.width = a.size.height * g.aspectRatio;
            a.position.top = a._helper ? h.top : 0
        }
        a.offset.left = a.parentData.left + a.position.left;
        a.offset.top = a.parentData.top + a.position.top;
        g = Math.abs((a._helper ? a.offset.left - q.left : a.offset.left - q.left) + a.sizeDiff.width);
        h =
            Math.abs((a._helper ? a.offset.top - q.top : a.offset.top - h.top) + a.sizeDiff.height);
        p = a.containerElement.get(0) == a.element.parent().get(0);
        q = /relative|absolute/.test(a.containerElement.css("position"));
        if (p && q)g -= a.parentData.left;
        if (g + a.size.width >= a.parentData.width) {
            a.size.width = a.parentData.width - g;
            if (f)a.size.height = a.size.width / a.aspectRatio
        }
        if (h + a.size.height >= a.parentData.height) {
            a.size.height = a.parentData.height - h;
            if (f)a.size.width = a.size.height * a.aspectRatio
        }
    }, stop: function () {
        var f = c(this).data("resizable"),
            a = f.options, g = f.containerOffset, h = f.containerPosition, p = f.containerElement, q = c(f.helper), o = q.offset(), l = q.outerWidth() - f.sizeDiff.width;
        q = q.outerHeight() - f.sizeDiff.height;
        f._helper && !a.animate && /relative/.test(p.css("position")) && c(this).css({left: o.left - h.left - g.left, width: l, height: q});
        f._helper && !a.animate && /static/.test(p.css("position")) && c(this).css({left: o.left - h.left - g.left, width: l, height: q})
    }});
    c.ui.plugin.add("resizable", "ghost", {start: function () {
        var f = c(this).data("resizable"), a = f.options,
            g = f.size;
        f.ghost = f.originalElement.clone();
        f.ghost.css({opacity: 0.25, display: "block", position: "relative", height: g.height, width: g.width, margin: 0, left: 0, top: 0}).addClass("ui-resizable-ghost").addClass(typeof a.ghost == "string" ? a.ghost : "");
        f.ghost.appendTo(f.helper)
    }, resize: function () {
        var f = c(this).data("resizable");
        f.ghost && f.ghost.css({position: "relative", height: f.size.height, width: f.size.width})
    }, stop: function () {
        var f = c(this).data("resizable");
        f.ghost && f.helper && f.helper.get(0).removeChild(f.ghost.get(0))
    }});
    c.ui.plugin.add("resizable", "grid", {resize: function () {
        var f = c(this).data("resizable"), a = f.options, g = f.size, h = f.originalSize, p = f.originalPosition, q = f.axis;
        a.grid = typeof a.grid == "number" ? [a.grid, a.grid] : a.grid;
        var o = Math.round((g.width - h.width) / (a.grid[0] || 1)) * (a.grid[0] || 1);
        a = Math.round((g.height - h.height) / (a.grid[1] || 1)) * (a.grid[1] || 1);
        if (/^(se|s|e)$/.test(q)) {
            f.size.width = h.width + o;
            f.size.height = h.height + a
        } else if (/^(ne)$/.test(q)) {
            f.size.width = h.width + o;
            f.size.height = h.height + a;
            f.position.top = p.top -
                a
        } else {
            if (/^(sw)$/.test(q)) {
                f.size.width = h.width + o;
                f.size.height = h.height + a
            } else {
                f.size.width = h.width + o;
                f.size.height = h.height + a;
                f.position.top = p.top - a
            }
            f.position.left = p.left - o
        }
    }});
    var b = function (f) {
        return parseInt(f, 10) || 0
    }, e = function (f) {
        return!isNaN(parseInt(f, 10))
    }
})(jQuery);
(function (c) {
    c.widget("ui.selectable", c.ui.mouse, {options: {appendTo: "body", autoRefresh: true, distance: 0, filter: "*", tolerance: "touch"}, _create: function () {
        var b = this;
        this.element.addClass("ui-selectable");
        this.dragged = false;
        var e;
        this.refresh = function () {
            e = c(b.options.filter, b.element[0]);
            e.each(function () {
                var f = c(this), a = f.offset();
                c.data(this, "selectable-item", {element: this, $element: f, left: a.left, top: a.top, right: a.left + f.outerWidth(), bottom: a.top + f.outerHeight(), startselected: false, selected: f.hasClass("ui-selected"),
                    selecting: f.hasClass("ui-selecting"), unselecting: f.hasClass("ui-unselecting")})
            })
        };
        this.refresh();
        this.selectees = e.addClass("ui-selectee");
        this._mouseInit();
        this.helper = c("<div class='ui-selectable-helper'></div>")
    }, destroy: function () {
        this.selectees.removeClass("ui-selectee").removeData("selectable-item");
        this.element.removeClass("ui-selectable ui-selectable-disabled").removeData("selectable").unbind(".selectable");
        this._mouseDestroy();
        return this
    }, _mouseStart: function (b) {
        var e = this;
        this.opos = [b.pageX,
            b.pageY];
        if (!this.options.disabled) {
            var f = this.options;
            this.selectees = c(f.filter, this.element[0]);
            this._trigger("start", b);
            c(f.appendTo).append(this.helper);
            this.helper.css({"z-index": 100, position: "absolute", left: b.clientX, top: b.clientY, width: 0, height: 0});
            f.autoRefresh && this.refresh();
            this.selectees.filter(".ui-selected").each(function () {
                var a = c.data(this, "selectable-item");
                a.startselected = true;
                if (!b.metaKey) {
                    a.$element.removeClass("ui-selected");
                    a.selected = false;
                    a.$element.addClass("ui-unselecting");
                    a.unselecting = true;
                    e._trigger("unselecting", b, {unselecting: a.element})
                }
            });
            c(b.target).parents().andSelf().each(function () {
                var a = c.data(this, "selectable-item");
                if (a) {
                    var g = !b.metaKey || !a.$element.hasClass("ui-selected");
                    a.$element.removeClass(g ? "ui-unselecting" : "ui-selected").addClass(g ? "ui-selecting" : "ui-unselecting");
                    a.unselecting = !g;
                    a.selecting = g;
                    (a.selected = g) ? e._trigger("selecting", b, {selecting: a.element}) : e._trigger("unselecting", b, {unselecting: a.element});
                    return false
                }
            })
        }
    }, _mouseDrag: function (b) {
        var e =
            this;
        this.dragged = true;
        if (!this.options.disabled) {
            var f = this.options, a = this.opos[0], g = this.opos[1], h = b.pageX, p = b.pageY;
            if (a > h) {
                var q = h;
                h = a;
                a = q
            }
            if (g > p) {
                q = p;
                p = g;
                g = q
            }
            this.helper.css({left: a, top: g, width: h - a, height: p - g});
            this.selectees.each(function () {
                var o = c.data(this, "selectable-item");
                if (!(!o || o.element == e.element[0])) {
                    var l = false;
                    if (f.tolerance == "touch")l = !(o.left > h || o.right < a || o.top > p || o.bottom < g); else if (f.tolerance == "fit")l = o.left > a && o.right < h && o.top > g && o.bottom < p;
                    if (l) {
                        if (o.selected) {
                            o.$element.removeClass("ui-selected");
                            o.selected = false
                        }
                        if (o.unselecting) {
                            o.$element.removeClass("ui-unselecting");
                            o.unselecting = false
                        }
                        if (!o.selecting) {
                            o.$element.addClass("ui-selecting");
                            o.selecting = true;
                            e._trigger("selecting", b, {selecting: o.element})
                        }
                    } else {
                        if (o.selecting)if (b.metaKey && o.startselected) {
                            o.$element.removeClass("ui-selecting");
                            o.selecting = false;
                            o.$element.addClass("ui-selected");
                            o.selected = true
                        } else {
                            o.$element.removeClass("ui-selecting");
                            o.selecting = false;
                            if (o.startselected) {
                                o.$element.addClass("ui-unselecting");
                                o.unselecting =
                                    true
                            }
                            e._trigger("unselecting", b, {unselecting: o.element})
                        }
                        if (o.selected)if (!b.metaKey && !o.startselected) {
                            o.$element.removeClass("ui-selected");
                            o.selected = false;
                            o.$element.addClass("ui-unselecting");
                            o.unselecting = true;
                            e._trigger("unselecting", b, {unselecting: o.element})
                        }
                    }
                }
            });
            return false
        }
    }, _mouseStop: function (b) {
        var e = this;
        this.dragged = false;
        c(".ui-unselecting", this.element[0]).each(function () {
            var f = c.data(this, "selectable-item");
            f.$element.removeClass("ui-unselecting");
            f.unselecting = false;
            f.startselected =
                false;
            e._trigger("unselected", b, {unselected: f.element})
        });
        c(".ui-selecting", this.element[0]).each(function () {
            var f = c.data(this, "selectable-item");
            f.$element.removeClass("ui-selecting").addClass("ui-selected");
            f.selecting = false;
            f.selected = true;
            f.startselected = true;
            e._trigger("selected", b, {selected: f.element})
        });
        this._trigger("stop", b);
        this.helper.remove();
        return false
    }});
    c.extend(c.ui.selectable, {version: "1.8.2"})
})(jQuery);
(function (c) {
    c.widget("ui.sortable", c.ui.mouse, {widgetEventPrefix: "sort", options: {appendTo: "parent", axis: false, connectWith: false, containment: false, cursor: "auto", cursorAt: false, dropOnEmpty: true, forcePlaceholderSize: false, forceHelperSize: false, grid: false, handle: false, helper: "original", items: "> *", opacity: false, placeholder: false, revert: false, scroll: true, scrollSensitivity: 20, scrollSpeed: 20, scope: "default", tolerance: "intersect", zIndex: 1E3}, _create: function () {
        this.containerCache = {};
        this.element.addClass("ui-sortable");
        this.refresh();
        this.floating = this.items.length ? /left|right/.test(this.items[0].item.css("float")) : false;
        this.offset = this.element.offset();
        this._mouseInit()
    }, destroy: function () {
        this.element.removeClass("ui-sortable ui-sortable-disabled").removeData("sortable").unbind(".sortable");
        this._mouseDestroy();
        for (var b = this.items.length - 1; b >= 0; b--)this.items[b].item.removeData("sortable-item");
        return this
    }, _setOption: function (b, e) {
        if (b === "disabled") {
            this.options[b] = e;
            this.widget()[e ? "addClass" : "removeClass"]("ui-sortable-disabled")
        } else c.Widget.prototype._setOption.apply(this,
            arguments)
    }, _mouseCapture: function (b, e) {
        if (this.reverting)return false;
        if (this.options.disabled || this.options.type == "static")return false;
        this._refreshItems(b);
        var f = null, a = this;
        c(b.target).parents().each(function () {
            if (c.data(this, "sortable-item") == a) {
                f = c(this);
                return false
            }
        });
        if (c.data(b.target, "sortable-item") == a)f = c(b.target);
        if (!f)return false;
        if (this.options.handle && !e) {
            var g = false;
            c(this.options.handle, f).find("*").andSelf().each(function () {
                if (this == b.target)g = true
            });
            if (!g)return false
        }
        this.currentItem =
            f;
        this._removeCurrentsFromItems();
        return true
    }, _mouseStart: function (b, e, f) {
        e = this.options;
        this.currentContainer = this;
        this.refreshPositions();
        this.helper = this._createHelper(b);
        this._cacheHelperProportions();
        this._cacheMargins();
        this.scrollParent = this.helper.scrollParent();
        this.offset = this.currentItem.offset();
        this.offset = {top: this.offset.top - this.margins.top, left: this.offset.left - this.margins.left};
        this.helper.css("position", "absolute");
        this.cssPosition = this.helper.css("position");
        c.extend(this.offset,
            {click: {left: b.pageX - this.offset.left, top: b.pageY - this.offset.top}, parent: this._getParentOffset(), relative: this._getRelativeOffset()});
        this.originalPosition = this._generatePosition(b);
        this.originalPageX = b.pageX;
        this.originalPageY = b.pageY;
        e.cursorAt && this._adjustOffsetFromHelper(e.cursorAt);
        this.domPosition = {prev: this.currentItem.prev()[0], parent: this.currentItem.parent()[0]};
        this.helper[0] != this.currentItem[0] && this.currentItem.hide();
        this._createPlaceholder();
        e.containment && this._setContainment();
        if (e.cursor) {
            if (c("body").css("cursor"))this._storedCursor = c("body").css("cursor");
            c("body").css("cursor", e.cursor)
        }
        if (e.opacity) {
            if (this.helper.css("opacity"))this._storedOpacity = this.helper.css("opacity");
            this.helper.css("opacity", e.opacity)
        }
        if (e.zIndex) {
            if (this.helper.css("zIndex"))this._storedZIndex = this.helper.css("zIndex");
            this.helper.css("zIndex", e.zIndex)
        }
        if (this.scrollParent[0] != document && this.scrollParent[0].tagName != "HTML")this.overflowOffset = this.scrollParent.offset();
        this._trigger("start",
            b, this._uiHash());
        this._preserveHelperProportions || this._cacheHelperProportions();
        if (!f)for (f = this.containers.length - 1; f >= 0; f--)this.containers[f]._trigger("activate", b, this._uiHash(this));
        if (c.ui.ddmanager)c.ui.ddmanager.current = this;
        c.ui.ddmanager && !e.dropBehaviour && c.ui.ddmanager.prepareOffsets(this, b);
        this.dragging = true;
        this.helper.addClass("ui-sortable-helper");
        this._mouseDrag(b);
        return true
    }, _mouseDrag: function (b) {
        this.position = this._generatePosition(b);
        this.positionAbs = this._convertPositionTo("absolute");
        if (!this.lastPositionAbs)this.lastPositionAbs = this.positionAbs;
        if (this.options.scroll) {
            var e = this.options, f = false;
            if (this.scrollParent[0] != document && this.scrollParent[0].tagName != "HTML") {
                if (this.overflowOffset.top + this.scrollParent[0].offsetHeight - b.pageY < e.scrollSensitivity)this.scrollParent[0].scrollTop = f = this.scrollParent[0].scrollTop + e.scrollSpeed; else if (b.pageY - this.overflowOffset.top < e.scrollSensitivity)this.scrollParent[0].scrollTop = f = this.scrollParent[0].scrollTop - e.scrollSpeed;
                if (this.overflowOffset.left +
                    this.scrollParent[0].offsetWidth - b.pageX < e.scrollSensitivity)this.scrollParent[0].scrollLeft = f = this.scrollParent[0].scrollLeft + e.scrollSpeed; else if (b.pageX - this.overflowOffset.left < e.scrollSensitivity)this.scrollParent[0].scrollLeft = f = this.scrollParent[0].scrollLeft - e.scrollSpeed
            } else {
                if (b.pageY - c(document).scrollTop() < e.scrollSensitivity)f = c(document).scrollTop(c(document).scrollTop() - e.scrollSpeed); else if (c(window).height() - (b.pageY - c(document).scrollTop()) < e.scrollSensitivity)f = c(document).scrollTop(c(document).scrollTop() +
                    e.scrollSpeed);
                if (b.pageX - c(document).scrollLeft() < e.scrollSensitivity)f = c(document).scrollLeft(c(document).scrollLeft() - e.scrollSpeed); else if (c(window).width() - (b.pageX - c(document).scrollLeft()) < e.scrollSensitivity)f = c(document).scrollLeft(c(document).scrollLeft() + e.scrollSpeed)
            }
            f !== false && c.ui.ddmanager && !e.dropBehaviour && c.ui.ddmanager.prepareOffsets(this, b)
        }
        this.positionAbs = this._convertPositionTo("absolute");
        if (!this.options.axis || this.options.axis != "y")this.helper[0].style.left = this.position.left +
            "px";
        if (!this.options.axis || this.options.axis != "x")this.helper[0].style.top = this.position.top + "px";
        for (e = this.items.length - 1; e >= 0; e--) {
            f = this.items[e];
            var a = f.item[0], g = this._intersectsWithPointer(f);
            if (g)if (a != this.currentItem[0] && this.placeholder[g == 1 ? "next" : "prev"]()[0] != a && !c.ui.contains(this.placeholder[0], a) && (this.options.type == "semi-dynamic" ? !c.ui.contains(this.element[0], a) : true)) {
                this.direction = g == 1 ? "down" : "up";
                if (this.options.tolerance == "pointer" || this._intersectsWithSides(f))this._rearrange(b,
                    f); else break;
                this._trigger("change", b, this._uiHash());
                break
            }
        }
        this._contactContainers(b);
        c.ui.ddmanager && c.ui.ddmanager.drag(this, b);
        this._trigger("sort", b, this._uiHash());
        this.lastPositionAbs = this.positionAbs;
        return false
    }, _mouseStop: function (b, e) {
        if (b) {
            c.ui.ddmanager && !this.options.dropBehaviour && c.ui.ddmanager.drop(this, b);
            if (this.options.revert) {
                var f = this;
                e = f.placeholder.offset();
                f.reverting = true;
                c(this.helper).animate({left: e.left - this.offset.parent.left - f.margins.left + (this.offsetParent[0] ==
                    document.body ? 0 : this.offsetParent[0].scrollLeft), top: e.top - this.offset.parent.top - f.margins.top + (this.offsetParent[0] == document.body ? 0 : this.offsetParent[0].scrollTop)}, parseInt(this.options.revert, 10) || 500, function () {
                    f._clear(b)
                })
            } else this._clear(b, e);
            return false
        }
    }, cancel: function () {
        if (this.dragging) {
            this._mouseUp();
            this.options.helper == "original" ? this.currentItem.css(this._storedCSS).removeClass("ui-sortable-helper") : this.currentItem.show();
            for (var b = this.containers.length - 1; b >= 0; b--) {
                this.containers[b]._trigger("deactivate",
                    null, this._uiHash(this));
                if (this.containers[b].containerCache.over) {
                    this.containers[b]._trigger("out", null, this._uiHash(this));
                    this.containers[b].containerCache.over = 0
                }
            }
        }
        this.placeholder[0].parentNode && this.placeholder[0].parentNode.removeChild(this.placeholder[0]);
        this.options.helper != "original" && this.helper && this.helper[0].parentNode && this.helper.remove();
        c.extend(this, {helper: null, dragging: false, reverting: false, _noFinalSort: null});
        this.domPosition.prev ? c(this.domPosition.prev).after(this.currentItem) :
            c(this.domPosition.parent).prepend(this.currentItem);
        return this
    }, serialize: function (b) {
        var e = this._getItemsAsjQuery(b && b.connected), f = [];
        b = b || {};
        c(e).each(function () {
            var a = (c(b.item || this).attr(b.attribute || "id") || "").match(b.expression || /(.+)[-=_](.+)/);
            if (a)f.push((b.key || a[1] + "[]") + "=" + (b.key && b.expression ? a[1] : a[2]))
        });
        return f.join("&")
    }, toArray: function (b) {
        var e = this._getItemsAsjQuery(b && b.connected), f = [];
        b = b || {};
        e.each(function () {
            f.push(c(b.item || this).attr(b.attribute || "id") || "")
        });
        return f
    },
        _intersectsWith: function (b) {
            var e = this.positionAbs.left, f = e + this.helperProportions.width, a = this.positionAbs.top, g = a + this.helperProportions.height, h = b.left, p = h + b.width, q = b.top, o = q + b.height, l = this.offset.click.top, r = this.offset.click.left;
            l = a + l > q && a + l < o && e + r > h && e + r < p;
            return this.options.tolerance == "pointer" || this.options.forcePointerForContainers || this.options.tolerance != "pointer" && this.helperProportions[this.floating ? "width" : "height"] > b[this.floating ? "width" : "height"] ? l : h < e + this.helperProportions.width /
                2 && f - this.helperProportions.width / 2 < p && q < a + this.helperProportions.height / 2 && g - this.helperProportions.height / 2 < o
        }, _intersectsWithPointer: function (b) {
            var e = c.ui.isOverAxis(this.positionAbs.top + this.offset.click.top, b.top, b.height);
            b = c.ui.isOverAxis(this.positionAbs.left + this.offset.click.left, b.left, b.width);
            e = e && b;
            b = this._getDragVerticalDirection();
            var f = this._getDragHorizontalDirection();
            if (!e)return false;
            return this.floating ? f && f == "right" || b == "down" ? 2 : 1 : b && (b == "down" ? 2 : 1)
        }, _intersectsWithSides: function (b) {
            var e =
                c.ui.isOverAxis(this.positionAbs.top + this.offset.click.top, b.top + b.height / 2, b.height);
            b = c.ui.isOverAxis(this.positionAbs.left + this.offset.click.left, b.left + b.width / 2, b.width);
            var f = this._getDragVerticalDirection(), a = this._getDragHorizontalDirection();
            return this.floating && a ? a == "right" && b || a == "left" && !b : f && (f == "down" && e || f == "up" && !e)
        }, _getDragVerticalDirection: function () {
            var b = this.positionAbs.top - this.lastPositionAbs.top;
            return b != 0 && (b > 0 ? "down" : "up")
        }, _getDragHorizontalDirection: function () {
            var b =
                this.positionAbs.left - this.lastPositionAbs.left;
            return b != 0 && (b > 0 ? "right" : "left")
        }, refresh: function (b) {
            this._refreshItems(b);
            this.refreshPositions();
            return this
        }, _connectWith: function () {
            var b = this.options;
            return b.connectWith.constructor == String ? [b.connectWith] : b.connectWith
        }, _getItemsAsjQuery: function (b) {
            var e = [], f = [], a = this._connectWith();
            if (a && b)for (b = a.length - 1; b >= 0; b--)for (var g = c(a[b]), h = g.length - 1; h >= 0; h--) {
                var p = c.data(g[h], "sortable");
                if (p && p != this && !p.options.disabled)f.push([c.isFunction(p.options.items) ?
                    p.options.items.call(p.element) : c(p.options.items, p.element).not(".ui-sortable-helper").not(".ui-sortable-placeholder"), p])
            }
            f.push([c.isFunction(this.options.items) ? this.options.items.call(this.element, null, {options: this.options, item: this.currentItem}) : c(this.options.items, this.element).not(".ui-sortable-helper").not(".ui-sortable-placeholder"), this]);
            for (b = f.length - 1; b >= 0; b--)f[b][0].each(function () {
                e.push(this)
            });
            return c(e)
        }, _removeCurrentsFromItems: function () {
            for (var b = this.currentItem.find(":data(sortable-item)"),
                     e = 0; e < this.items.length; e++)for (var f = 0; f < b.length; f++)b[f] == this.items[e].item[0] && this.items.splice(e, 1)
        }, _refreshItems: function (b) {
            this.items = [];
            this.containers = [this];
            var e = this.items, f = [
                [c.isFunction(this.options.items) ? this.options.items.call(this.element[0], b, {item: this.currentItem}) : c(this.options.items, this.element), this]
            ], a = this._connectWith();
            if (a)for (var g = a.length - 1; g >= 0; g--)for (var h = c(a[g]), p = h.length - 1; p >= 0; p--) {
                var q = c.data(h[p], "sortable");
                if (q && q != this && !q.options.disabled) {
                    f.push([c.isFunction(q.options.items) ?
                        q.options.items.call(q.element[0], b, {item: this.currentItem}) : c(q.options.items, q.element), q]);
                    this.containers.push(q)
                }
            }
            for (g = f.length - 1; g >= 0; g--) {
                b = f[g][1];
                a = f[g][0];
                p = 0;
                for (h = a.length; p < h; p++) {
                    q = c(a[p]);
                    q.data("sortable-item", b);
                    e.push({item: q, instance: b, width: 0, height: 0, left: 0, top: 0})
                }
            }
        }, refreshPositions: function (b) {
            if (this.offsetParent && this.helper)this.offset.parent = this._getParentOffset();
            for (var e = this.items.length - 1; e >= 0; e--) {
                var f = this.items[e], a = this.options.toleranceElement ? c(this.options.toleranceElement,
                    f.item) : f.item;
                if (!b) {
                    f.width = a.outerWidth();
                    f.height = a.outerHeight()
                }
                a = a.offset();
                f.left = a.left;
                f.top = a.top
            }
            if (this.options.custom && this.options.custom.refreshContainers)this.options.custom.refreshContainers.call(this); else for (e = this.containers.length - 1; e >= 0; e--) {
                a = this.containers[e].element.offset();
                this.containers[e].containerCache.left = a.left;
                this.containers[e].containerCache.top = a.top;
                this.containers[e].containerCache.width = this.containers[e].element.outerWidth();
                this.containers[e].containerCache.height =
                    this.containers[e].element.outerHeight()
            }
            return this
        }, _createPlaceholder: function (b) {
            var e = b || this, f = e.options;
            if (!f.placeholder || f.placeholder.constructor == String) {
                var a = f.placeholder;
                f.placeholder = {element: function () {
                    var g = c(document.createElement(e.currentItem[0].nodeName)).addClass(a || e.currentItem[0].className + " ui-sortable-placeholder").removeClass("ui-sortable-helper")[0];
                    if (!a)g.style.visibility = "hidden";
                    return g
                }, update: function (g, h) {
                    if (!(a && !f.forcePlaceholderSize)) {
                        h.height() || h.height(e.currentItem.innerHeight() -
                            parseInt(e.currentItem.css("paddingTop") || 0, 10) - parseInt(e.currentItem.css("paddingBottom") || 0, 10));
                        h.width() || h.width(e.currentItem.innerWidth() - parseInt(e.currentItem.css("paddingLeft") || 0, 10) - parseInt(e.currentItem.css("paddingRight") || 0, 10))
                    }
                }}
            }
            e.placeholder = c(f.placeholder.element.call(e.element, e.currentItem));
            e.currentItem.after(e.placeholder);
            f.placeholder.update(e, e.placeholder)
        }, _contactContainers: function (b) {
            for (var e = null, f = null, a = this.containers.length - 1; a >= 0; a--)if (!c.ui.contains(this.currentItem[0],
                this.containers[a].element[0]))if (this._intersectsWith(this.containers[a].containerCache)) {
                if (!(e && c.ui.contains(this.containers[a].element[0], e.element[0]))) {
                    e = this.containers[a];
                    f = a
                }
            } else if (this.containers[a].containerCache.over) {
                this.containers[a]._trigger("out", b, this._uiHash(this));
                this.containers[a].containerCache.over = 0
            }
            if (e)if (this.containers.length === 1) {
                this.containers[f]._trigger("over", b, this._uiHash(this));
                this.containers[f].containerCache.over = 1
            } else if (this.currentContainer != this.containers[f]) {
                e =
                    1E4;
                a = null;
                for (var g = this.positionAbs[this.containers[f].floating ? "left" : "top"], h = this.items.length - 1; h >= 0; h--)if (c.ui.contains(this.containers[f].element[0], this.items[h].item[0])) {
                    var p = this.items[h][this.containers[f].floating ? "left" : "top"];
                    if (Math.abs(p - g) < e) {
                        e = Math.abs(p - g);
                        a = this.items[h]
                    }
                }
                if (a || this.options.dropOnEmpty) {
                    this.currentContainer = this.containers[f];
                    a ? this._rearrange(b, a, null, true) : this._rearrange(b, null, this.containers[f].element, true);
                    this._trigger("change", b, this._uiHash());
                    this.containers[f]._trigger("change",
                        b, this._uiHash(this));
                    this.options.placeholder.update(this.currentContainer, this.placeholder);
                    this.containers[f]._trigger("over", b, this._uiHash(this));
                    this.containers[f].containerCache.over = 1
                }
            }
        }, _createHelper: function (b) {
            var e = this.options;
            b = c.isFunction(e.helper) ? c(e.helper.apply(this.element[0], [b, this.currentItem])) : e.helper == "clone" ? this.currentItem.clone() : this.currentItem;
            b.parents("body").length || c(e.appendTo != "parent" ? e.appendTo : this.currentItem[0].parentNode)[0].appendChild(b[0]);
            if (b[0] ==
                this.currentItem[0])this._storedCSS = {width: this.currentItem[0].style.width, height: this.currentItem[0].style.height, position: this.currentItem.css("position"), top: this.currentItem.css("top"), left: this.currentItem.css("left")};
            if (b[0].style.width == "" || e.forceHelperSize)b.width(this.currentItem.width());
            if (b[0].style.height == "" || e.forceHelperSize)b.height(this.currentItem.height());
            return b
        }, _adjustOffsetFromHelper: function (b) {
            if (typeof b == "string")b = b.split(" ");
            if (c.isArray(b))b = {left: +b[0], top: +b[1] ||
                0};
            if ("left"in b)this.offset.click.left = b.left + this.margins.left;
            if ("right"in b)this.offset.click.left = this.helperProportions.width - b.right + this.margins.left;
            if ("top"in b)this.offset.click.top = b.top + this.margins.top;
            if ("bottom"in b)this.offset.click.top = this.helperProportions.height - b.bottom + this.margins.top
        }, _getParentOffset: function () {
            this.offsetParent = this.helper.offsetParent();
            var b = this.offsetParent.offset();
            if (this.cssPosition == "absolute" && this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0],
                this.offsetParent[0])) {
                b.left += this.scrollParent.scrollLeft();
                b.top += this.scrollParent.scrollTop()
            }
            if (this.offsetParent[0] == document.body || this.offsetParent[0].tagName && this.offsetParent[0].tagName.toLowerCase() == "html" && c.browser.msie)b = {top: 0, left: 0};
            return{top: b.top + (parseInt(this.offsetParent.css("borderTopWidth"), 10) || 0), left: b.left + (parseInt(this.offsetParent.css("borderLeftWidth"), 10) || 0)}
        }, _getRelativeOffset: function () {
            if (this.cssPosition == "relative") {
                var b = this.currentItem.position();
                return{top: b.top -
                    (parseInt(this.helper.css("top"), 10) || 0) + this.scrollParent.scrollTop(), left: b.left - (parseInt(this.helper.css("left"), 10) || 0) + this.scrollParent.scrollLeft()}
            } else return{top: 0, left: 0}
        }, _cacheMargins: function () {
            this.margins = {left: parseInt(this.currentItem.css("marginLeft"), 10) || 0, top: parseInt(this.currentItem.css("marginTop"), 10) || 0}
        }, _cacheHelperProportions: function () {
            this.helperProportions = {width: this.helper.outerWidth(), height: this.helper.outerHeight()}
        }, _setContainment: function () {
            var b = this.options;
            if (b.containment == "parent")b.containment = this.helper[0].parentNode;
            if (b.containment == "document" || b.containment == "window")this.containment = [0 - this.offset.relative.left - this.offset.parent.left, 0 - this.offset.relative.top - this.offset.parent.top, c(b.containment == "document" ? document : window).width() - this.helperProportions.width - this.margins.left, (c(b.containment == "document" ? document : window).height() || document.body.parentNode.scrollHeight) - this.helperProportions.height - this.margins.top];
            if (!/^(document|window|parent)$/.test(b.containment)) {
                var e =
                    c(b.containment)[0];
                b = c(b.containment).offset();
                var f = c(e).css("overflow") != "hidden";
                this.containment = [b.left + (parseInt(c(e).css("borderLeftWidth"), 10) || 0) + (parseInt(c(e).css("paddingLeft"), 10) || 0) - this.margins.left, b.top + (parseInt(c(e).css("borderTopWidth"), 10) || 0) + (parseInt(c(e).css("paddingTop"), 10) || 0) - this.margins.top, b.left + (f ? Math.max(e.scrollWidth, e.offsetWidth) : e.offsetWidth) - (parseInt(c(e).css("borderLeftWidth"), 10) || 0) - (parseInt(c(e).css("paddingRight"), 10) || 0) - this.helperProportions.width -
                    this.margins.left, b.top + (f ? Math.max(e.scrollHeight, e.offsetHeight) : e.offsetHeight) - (parseInt(c(e).css("borderTopWidth"), 10) || 0) - (parseInt(c(e).css("paddingBottom"), 10) || 0) - this.helperProportions.height - this.margins.top]
            }
        }, _convertPositionTo: function (b, e) {
            if (!e)e = this.position;
            b = b == "absolute" ? 1 : -1;
            var f = this.cssPosition == "absolute" && !(this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0], this.offsetParent[0])) ? this.offsetParent : this.scrollParent, a = /(html|body)/i.test(f[0].tagName);
            return{top: e.top +
                this.offset.relative.top * b + this.offset.parent.top * b - (c.browser.safari && this.cssPosition == "fixed" ? 0 : (this.cssPosition == "fixed" ? -this.scrollParent.scrollTop() : a ? 0 : f.scrollTop()) * b), left: e.left + this.offset.relative.left * b + this.offset.parent.left * b - (c.browser.safari && this.cssPosition == "fixed" ? 0 : (this.cssPosition == "fixed" ? -this.scrollParent.scrollLeft() : a ? 0 : f.scrollLeft()) * b)}
        }, _generatePosition: function (b) {
            var e = this.options, f = this.cssPosition == "absolute" && !(this.scrollParent[0] != document && c.ui.contains(this.scrollParent[0],
                this.offsetParent[0])) ? this.offsetParent : this.scrollParent, a = /(html|body)/i.test(f[0].tagName);
            if (this.cssPosition == "relative" && !(this.scrollParent[0] != document && this.scrollParent[0] != this.offsetParent[0]))this.offset.relative = this._getRelativeOffset();
            var g = b.pageX, h = b.pageY;
            if (this.originalPosition) {
                if (this.containment) {
                    if (b.pageX - this.offset.click.left < this.containment[0])g = this.containment[0] + this.offset.click.left;
                    if (b.pageY - this.offset.click.top < this.containment[1])h = this.containment[1] + this.offset.click.top;
                    if (b.pageX - this.offset.click.left > this.containment[2])g = this.containment[2] + this.offset.click.left;
                    if (b.pageY - this.offset.click.top > this.containment[3])h = this.containment[3] + this.offset.click.top
                }
                if (e.grid) {
                    h = this.originalPageY + Math.round((h - this.originalPageY) / e.grid[1]) * e.grid[1];
                    h = this.containment ? !(h - this.offset.click.top < this.containment[1] || h - this.offset.click.top > this.containment[3]) ? h : !(h - this.offset.click.top < this.containment[1]) ? h - e.grid[1] : h + e.grid[1] : h;
                    g = this.originalPageX + Math.round((g -
                        this.originalPageX) / e.grid[0]) * e.grid[0];
                    g = this.containment ? !(g - this.offset.click.left < this.containment[0] || g - this.offset.click.left > this.containment[2]) ? g : !(g - this.offset.click.left < this.containment[0]) ? g - e.grid[0] : g + e.grid[0] : g
                }
            }
            return{top: h - this.offset.click.top - this.offset.relative.top - this.offset.parent.top + (c.browser.safari && this.cssPosition == "fixed" ? 0 : this.cssPosition == "fixed" ? -this.scrollParent.scrollTop() : a ? 0 : f.scrollTop()), left: g - this.offset.click.left - this.offset.relative.left - this.offset.parent.left +
                (c.browser.safari && this.cssPosition == "fixed" ? 0 : this.cssPosition == "fixed" ? -this.scrollParent.scrollLeft() : a ? 0 : f.scrollLeft())}
        }, _rearrange: function (b, e, f, a) {
            f ? f[0].appendChild(this.placeholder[0]) : e.item[0].parentNode.insertBefore(this.placeholder[0], this.direction == "down" ? e.item[0] : e.item[0].nextSibling);
            this.counter = this.counter ? ++this.counter : 1;
            var g = this, h = this.counter;
            window.setTimeout(function () {
                h == g.counter && g.refreshPositions(!a)
            }, 0)
        }, _clear: function (b, e) {
            this.reverting = false;
            var f = [];
            !this._noFinalSort &&
            this.currentItem[0].parentNode && this.placeholder.before(this.currentItem);
            this._noFinalSort = null;
            if (this.helper[0] == this.currentItem[0]) {
                for (var a in this._storedCSS)if (this._storedCSS[a] == "auto" || this._storedCSS[a] == "static")this._storedCSS[a] = "";
                this.currentItem.css(this._storedCSS).removeClass("ui-sortable-helper")
            } else this.currentItem.show();
            this.fromOutside && !e && f.push(function (g) {
                this._trigger("receive", g, this._uiHash(this.fromOutside))
            });
            if ((this.fromOutside || this.domPosition.prev != this.currentItem.prev().not(".ui-sortable-helper")[0] ||
                this.domPosition.parent != this.currentItem.parent()[0]) && !e)f.push(function (g) {
                this._trigger("update", g, this._uiHash())
            });
            if (!c.ui.contains(this.element[0], this.currentItem[0])) {
                e || f.push(function (g) {
                    this._trigger("remove", g, this._uiHash())
                });
                for (a = this.containers.length - 1; a >= 0; a--)if (c.ui.contains(this.containers[a].element[0], this.currentItem[0]) && !e) {
                    f.push(function (g) {
                        return function (h) {
                            g._trigger("receive", h, this._uiHash(this))
                        }
                    }.call(this, this.containers[a]));
                    f.push(function (g) {
                        return function (h) {
                            g._trigger("update",
                                h, this._uiHash(this))
                        }
                    }.call(this, this.containers[a]))
                }
            }
            for (a = this.containers.length - 1; a >= 0; a--) {
                e || f.push(function (g) {
                    return function (h) {
                        g._trigger("deactivate", h, this._uiHash(this))
                    }
                }.call(this, this.containers[a]));
                if (this.containers[a].containerCache.over) {
                    f.push(function (g) {
                        return function (h) {
                            g._trigger("out", h, this._uiHash(this))
                        }
                    }.call(this, this.containers[a]));
                    this.containers[a].containerCache.over = 0
                }
            }
            this._storedCursor && c("body").css("cursor", this._storedCursor);
            this._storedOpacity && this.helper.css("opacity",
                this._storedOpacity);
            if (this._storedZIndex)this.helper.css("zIndex", this._storedZIndex == "auto" ? "" : this._storedZIndex);
            this.dragging = false;
            if (this.cancelHelperRemoval) {
                if (!e) {
                    this._trigger("beforeStop", b, this._uiHash());
                    for (a = 0; a < f.length; a++)f[a].call(this, b);
                    this._trigger("stop", b, this._uiHash())
                }
                return false
            }
            e || this._trigger("beforeStop", b, this._uiHash());
            this.placeholder[0].parentNode.removeChild(this.placeholder[0]);
            this.helper[0] != this.currentItem[0] && this.helper.remove();
            this.helper = null;
            if (!e) {
                for (a =
                         0; a < f.length; a++)f[a].call(this, b);
                this._trigger("stop", b, this._uiHash())
            }
            this.fromOutside = false;
            return true
        }, _trigger: function () {
            c.Widget.prototype._trigger.apply(this, arguments) === false && this.cancel()
        }, _uiHash: function (b) {
            var e = b || this;
            return{helper: e.helper, placeholder: e.placeholder || c([]), position: e.position, originalPosition: e.originalPosition, offset: e.positionAbs, item: e.currentItem, sender: b ? b.element : null}
        }});
    c.extend(c.ui.sortable, {version: "1.8.2"})
})(jQuery);
jQuery.effects || function (c) {
    function b(o) {
        var l;
        if (o && o.constructor == Array && o.length == 3)return o;
        if (l = /rgb\(\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*,\s*([0-9]{1,3})\s*\)/.exec(o))return[parseInt(l[1], 10), parseInt(l[2], 10), parseInt(l[3], 10)];
        if (l = /rgb\(\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*,\s*([0-9]+(?:\.[0-9]+)?)\%\s*\)/.exec(o))return[parseFloat(l[1]) * 2.55, parseFloat(l[2]) * 2.55, parseFloat(l[3]) * 2.55];
        if (l = /#([a-fA-F0-9]{2})([a-fA-F0-9]{2})([a-fA-F0-9]{2})/.exec(o))return[parseInt(l[1],
            16), parseInt(l[2], 16), parseInt(l[3], 16)];
        if (l = /#([a-fA-F0-9])([a-fA-F0-9])([a-fA-F0-9])/.exec(o))return[parseInt(l[1] + l[1], 16), parseInt(l[2] + l[2], 16), parseInt(l[3] + l[3], 16)];
        if (/rgba\(0, 0, 0, 0\)/.exec(o))return h.transparent;
        return h[c.trim(o).toLowerCase()]
    }

    function e() {
        var o = document.defaultView ? document.defaultView.getComputedStyle(this, null) : this.currentStyle, l = {}, r, u;
        if (o && o.length && o[0] && o[o[0]])for (var y = o.length; y--;) {
            r = o[y];
            if (typeof o[r] == "string") {
                u = r.replace(/\-(\w)/g, function (A, i) {
                    return i.toUpperCase()
                });
                l[u] = o[r]
            }
        } else for (r in o)if (typeof o[r] === "string")l[r] = o[r];
        return l
    }

    function f(o) {
        var l, r;
        for (l in o) {
            r = o[l];
            if (r == null || c.isFunction(r) || l in q || /scrollbar/.test(l) || !/color/i.test(l) && isNaN(parseFloat(r)))delete o[l]
        }
        return o
    }

    function a(o, l) {
        var r = {_: 0}, u;
        for (u in l)if (o[u] != l[u])r[u] = l[u];
        return r
    }

    function g(o, l, r, u) {
        if (typeof o == "object") {
            u = l;
            r = null;
            l = o;
            o = l.effect
        }
        if (c.isFunction(l)) {
            u = l;
            r = null;
            l = {}
        }
        if (c.isFunction(r)) {
            u = r;
            r = null
        }
        if (typeof l == "number" || c.fx.speeds[l]) {
            u = r;
            r = l;
            l = {}
        }
        l = l || {};
        r = r || l.duration;
        r = c.fx.off ? 0 : typeof r == "number" ? r : c.fx.speeds[r] || c.fx.speeds._default;
        u = u || l.complete;
        return[o, l, r, u]
    }

    c.effects = {};
    c.each(["backgroundColor", "borderBottomColor", "borderLeftColor", "borderRightColor", "borderTopColor", "color", "outlineColor"], function (o, l) {
        c.fx.step[l] = function (r) {
            if (!r.colorInit) {
                var u;
                u = r.elem;
                var y = l, A;
                do {
                    A = c.curCSS(u, y);
                    if (A != "" && A != "transparent" || c.nodeName(u, "body"))break;
                    y = "backgroundColor"
                } while (u = u.parentNode);
                u = b(A);
                r.start = u;
                r.end = b(r.end);
                r.colorInit = true
            }
            r.elem.style[l] =
                "rgb(" + Math.max(Math.min(parseInt(r.pos * (r.end[0] - r.start[0]) + r.start[0], 10), 255), 0) + "," + Math.max(Math.min(parseInt(r.pos * (r.end[1] - r.start[1]) + r.start[1], 10), 255), 0) + "," + Math.max(Math.min(parseInt(r.pos * (r.end[2] - r.start[2]) + r.start[2], 10), 255), 0) + ")"
        }
    });
    var h = {aqua: [0, 255, 255], azure: [240, 255, 255], beige: [245, 245, 220], black: [0, 0, 0], blue: [0, 0, 255], brown: [165, 42, 42], cyan: [0, 255, 255], darkblue: [0, 0, 139], darkcyan: [0, 139, 139], darkgrey: [169, 169, 169], darkgreen: [0, 100, 0], darkkhaki: [189, 183, 107], darkmagenta: [139,
        0, 139], darkolivegreen: [85, 107, 47], darkorange: [255, 140, 0], darkorchid: [153, 50, 204], darkred: [139, 0, 0], darksalmon: [233, 150, 122], darkviolet: [148, 0, 211], fuchsia: [255, 0, 255], gold: [255, 215, 0], green: [0, 128, 0], indigo: [75, 0, 130], khaki: [240, 230, 140], lightblue: [173, 216, 230], lightcyan: [224, 255, 255], lightgreen: [144, 238, 144], lightgrey: [211, 211, 211], lightpink: [255, 182, 193], lightyellow: [255, 255, 224], lime: [0, 255, 0], magenta: [255, 0, 255], maroon: [128, 0, 0], navy: [0, 0, 128], olive: [128, 128, 0], orange: [255, 165, 0], pink: [255, 192,
        203], purple: [128, 0, 128], violet: [128, 0, 128], red: [255, 0, 0], silver: [192, 192, 192], white: [255, 255, 255], yellow: [255, 255, 0], transparent: [255, 255, 255]}, p = ["add", "remove", "toggle"], q = {border: 1, borderBottom: 1, borderColor: 1, borderLeft: 1, borderRight: 1, borderTop: 1, borderWidth: 1, margin: 1, padding: 1};
    c.effects.animateClass = function (o, l, r, u) {
        if (c.isFunction(r)) {
            u = r;
            r = null
        }
        return this.each(function () {
            var y = c(this), A = y.attr("style") || " ", i = f(e.call(this)), J, K = y.attr("className");
            c.each(p, function (D, P) {
                o[P] && y[P + "Class"](o[P])
            });
            J = f(e.call(this));
            y.attr("className", K);
            y.animate(a(i, J), l, r, function () {
                c.each(p, function (D, P) {
                    o[P] && y[P + "Class"](o[P])
                });
                if (typeof y.attr("style") == "object") {
                    y.attr("style").cssText = "";
                    y.attr("style").cssText = A
                } else y.attr("style", A);
                u && u.apply(this, arguments)
            })
        })
    };
    c.fn.extend({_addClass: c.fn.addClass, addClass: function (o, l, r, u) {
        return l ? c.effects.animateClass.apply(this, [
            {add: o},
            l,
            r,
            u
        ]) : this._addClass(o)
    }, _removeClass: c.fn.removeClass, removeClass: function (o, l, r, u) {
        return l ? c.effects.animateClass.apply(this,
            [
                {remove: o},
                l,
                r,
                u
            ]) : this._removeClass(o)
    }, _toggleClass: c.fn.toggleClass, toggleClass: function (o, l, r, u, y) {
        return typeof l == "boolean" || l === undefined ? r ? c.effects.animateClass.apply(this, [l ? {add: o} : {remove: o}, r, u, y]) : this._toggleClass(o, l) : c.effects.animateClass.apply(this, [
            {toggle: o},
            l,
            r,
            u
        ])
    }, switchClass: function (o, l, r, u, y) {
        return c.effects.animateClass.apply(this, [
            {add: l, remove: o},
            r,
            u,
            y
        ])
    }});
    c.extend(c.effects, {version: "1.8.2", save: function (o, l) {
        for (var r = 0; r < l.length; r++)l[r] !== null && o.data("ec.storage." +
            l[r], o[0].style[l[r]])
    }, restore: function (o, l) {
        for (var r = 0; r < l.length; r++)l[r] !== null && o.css(l[r], o.data("ec.storage." + l[r]))
    }, setMode: function (o, l) {
        if (l == "toggle")l = o.is(":hidden") ? "show" : "hide";
        return l
    }, getBaseline: function (o, l) {
        var r;
        switch (o[0]) {
            case "top":
                r = 0;
                break;
            case "middle":
                r = 0.5;
                break;
            case "bottom":
                r = 1;
                break;
            default:
                r = o[0] / l.height
        }
        switch (o[1]) {
            case "left":
                o = 0;
                break;
            case "center":
                o = 0.5;
                break;
            case "right":
                o = 1;
                break;
            default:
                o = o[1] / l.width
        }
        return{x: o, y: r}
    }, createWrapper: function (o) {
        if (o.parent().is(".ui-effects-wrapper"))return o.parent();
        var l = {width: o.outerWidth(true), height: o.outerHeight(true), "float": o.css("float")}, r = c("<div></div>").addClass("ui-effects-wrapper").css({fontSize: "100%", background: "transparent", border: "none", margin: 0, padding: 0});
        o.wrap(r);
        r = o.parent();
        if (o.css("position") == "static") {
            r.css({position: "relative"});
            o.css({position: "relative"})
        } else {
            c.extend(l, {position: o.css("position"), zIndex: o.css("z-index")});
            c.each(["top", "left", "bottom", "right"], function (u, y) {
                l[y] = o.css(y);
                if (isNaN(parseInt(l[y], 10)))l[y] = "auto"
            });
            o.css({position: "relative", top: 0, left: 0})
        }
        return r.css(l).show()
    }, removeWrapper: function (o) {
        if (o.parent().is(".ui-effects-wrapper"))return o.parent().replaceWith(o);
        return o
    }, setTransition: function (o, l, r, u) {
        u = u || {};
        c.each(l, function (y, A) {
            unit = o.cssUnit(A);
            if (unit[0] > 0)u[A] = unit[0] * r + unit[1]
        });
        return u
    }});
    c.fn.extend({effect: function (o) {
        var l = g.apply(this, arguments);
        l = {options: l[1], duration: l[2], callback: l[3]};
        var r = c.effects[o];
        return r && !c.fx.off ? r.call(this, l) : this
    }, _show: c.fn.show, show: function (o) {
        if (!o ||
            typeof o == "number" || c.fx.speeds[o])return this._show.apply(this, arguments); else {
            var l = g.apply(this, arguments);
            l[1].mode = "show";
            return this.effect.apply(this, l)
        }
    }, _hide: c.fn.hide, hide: function (o) {
        if (!o || typeof o == "number" || c.fx.speeds[o])return this._hide.apply(this, arguments); else {
            var l = g.apply(this, arguments);
            l[1].mode = "hide";
            return this.effect.apply(this, l)
        }
    }, __toggle: c.fn.toggle, toggle: function (o) {
        if (!o || typeof o == "number" || c.fx.speeds[o] || typeof o == "boolean" || c.isFunction(o))return this.__toggle.apply(this,
            arguments); else {
            var l = g.apply(this, arguments);
            l[1].mode = "toggle";
            return this.effect.apply(this, l)
        }
    }, cssUnit: function (o) {
        var l = this.css(o), r = [];
        c.each(["em", "px", "%", "pt"], function (u, y) {
            if (l.indexOf(y) > 0)r = [parseFloat(l), y]
        });
        return r
    }});
    c.easing.jswing = c.easing.swing;
    c.extend(c.easing, {def: "easeOutQuad", swing: function (o, l, r, u, y) {
        return c.easing[c.easing.def](o, l, r, u, y)
    }, easeInQuad: function (o, l, r, u, y) {
        return u * (l /= y) * l + r
    }, easeOutQuad: function (o, l, r, u, y) {
        return-u * (l /= y) * (l - 2) + r
    }, easeInOutQuad: function (o, l, r, u, y) {
        if ((l /= y / 2) < 1)return u / 2 * l * l + r;
        return-u / 2 * (--l * (l - 2) - 1) + r
    }, easeInCubic: function (o, l, r, u, y) {
        return u * (l /= y) * l * l + r
    }, easeOutCubic: function (o, l, r, u, y) {
        return u * ((l = l / y - 1) * l * l + 1) + r
    }, easeInOutCubic: function (o, l, r, u, y) {
        if ((l /= y / 2) < 1)return u / 2 * l * l * l + r;
        return u / 2 * ((l -= 2) * l * l + 2) + r
    }, easeInQuart: function (o, l, r, u, y) {
        return u * (l /= y) * l * l * l + r
    }, easeOutQuart: function (o, l, r, u, y) {
        return-u * ((l = l / y - 1) * l * l * l - 1) + r
    }, easeInOutQuart: function (o, l, r, u, y) {
        if ((l /= y / 2) < 1)return u / 2 * l * l * l * l + r;
        return-u / 2 * ((l -= 2) * l * l * l - 2) +
            r
    }, easeInQuint: function (o, l, r, u, y) {
        return u * (l /= y) * l * l * l * l + r
    }, easeOutQuint: function (o, l, r, u, y) {
        return u * ((l = l / y - 1) * l * l * l * l + 1) + r
    }, easeInOutQuint: function (o, l, r, u, y) {
        if ((l /= y / 2) < 1)return u / 2 * l * l * l * l * l + r;
        return u / 2 * ((l -= 2) * l * l * l * l + 2) + r
    }, easeInSine: function (o, l, r, u, y) {
        return-u * Math.cos(l / y * (Math.PI / 2)) + u + r
    }, easeOutSine: function (o, l, r, u, y) {
        return u * Math.sin(l / y * (Math.PI / 2)) + r
    }, easeInOutSine: function (o, l, r, u, y) {
        return-u / 2 * (Math.cos(Math.PI * l / y) - 1) + r
    }, easeInExpo: function (o, l, r, u, y) {
        return l == 0 ? r : u * Math.pow(2,
                10 * (l / y - 1)) + r
    }, easeOutExpo: function (o, l, r, u, y) {
        return l == y ? r + u : u * (-Math.pow(2, -10 * l / y) + 1) + r
    }, easeInOutExpo: function (o, l, r, u, y) {
        if (l == 0)return r;
        if (l == y)return r + u;
        if ((l /= y / 2) < 1)return u / 2 * Math.pow(2, 10 * (l - 1)) + r;
        return u / 2 * (-Math.pow(2, -10 * --l) + 2) + r
    }, easeInCirc: function (o, l, r, u, y) {
        return-u * (Math.sqrt(1 - (l /= y) * l) - 1) + r
    }, easeOutCirc: function (o, l, r, u, y) {
        return u * Math.sqrt(1 - (l = l / y - 1) * l) + r
    }, easeInOutCirc: function (o, l, r, u, y) {
        if ((l /= y / 2) < 1)return-u / 2 * (Math.sqrt(1 - l * l) - 1) + r;
        return u / 2 * (Math.sqrt(1 - (l -= 2) *
            l) + 1) + r
    }, easeInElastic: function (o, l, r, u, y) {
        var A = 0, i = u;
        if (l == 0)return r;
        if ((l /= y) == 1)return r + u;
        A || (A = y * 0.3);
        if (i < Math.abs(u)) {
            i = u;
            o = A / 4
        } else o = A / (2 * Math.PI) * Math.asin(u / i);
        return-(i * Math.pow(2, 10 * (l -= 1)) * Math.sin((l * y - o) * 2 * Math.PI / A)) + r
    }, easeOutElastic: function (o, l, r, u, y) {
        var A = 0, i = u;
        if (l == 0)return r;
        if ((l /= y) == 1)return r + u;
        A || (A = y * 0.3);
        if (i < Math.abs(u)) {
            i = u;
            o = A / 4
        } else o = A / (2 * Math.PI) * Math.asin(u / i);
        return i * Math.pow(2, -10 * l) * Math.sin((l * y - o) * 2 * Math.PI / A) + u + r
    }, easeInOutElastic: function (o, l, r, u, y) {
        var A =
            0, i = u;
        if (l == 0)return r;
        if ((l /= y / 2) == 2)return r + u;
        A || (A = y * 0.3 * 1.5);
        if (i < Math.abs(u)) {
            i = u;
            o = A / 4
        } else o = A / (2 * Math.PI) * Math.asin(u / i);
        if (l < 1)return-0.5 * i * Math.pow(2, 10 * (l -= 1)) * Math.sin((l * y - o) * 2 * Math.PI / A) + r;
        return i * Math.pow(2, -10 * (l -= 1)) * Math.sin((l * y - o) * 2 * Math.PI / A) * 0.5 + u + r
    }, easeInBack: function (o, l, r, u, y, A) {
        if (A == undefined)A = 1.70158;
        return u * (l /= y) * l * ((A + 1) * l - A) + r
    }, easeOutBack: function (o, l, r, u, y, A) {
        if (A == undefined)A = 1.70158;
        return u * ((l = l / y - 1) * l * ((A + 1) * l + A) + 1) + r
    }, easeInOutBack: function (o, l, r, u, y, A) {
        if (A ==
            undefined)A = 1.70158;
        if ((l /= y / 2) < 1)return u / 2 * l * l * (((A *= 1.525) + 1) * l - A) + r;
        return u / 2 * ((l -= 2) * l * (((A *= 1.525) + 1) * l + A) + 2) + r
    }, easeInBounce: function (o, l, r, u, y) {
        return u - c.easing.easeOutBounce(o, y - l, 0, u, y) + r
    }, easeOutBounce: function (o, l, r, u, y) {
        return(l /= y) < 1 / 2.75 ? u * 7.5625 * l * l + r : l < 2 / 2.75 ? u * (7.5625 * (l -= 1.5 / 2.75) * l + 0.75) + r : l < 2.5 / 2.75 ? u * (7.5625 * (l -= 2.25 / 2.75) * l + 0.9375) + r : u * (7.5625 * (l -= 2.625 / 2.75) * l + 0.984375) + r
    }, easeInOutBounce: function (o, l, r, u, y) {
        if (l < y / 2)return c.easing.easeInBounce(o, l * 2, 0, u, y) * 0.5 + r;
        return c.easing.easeOutBounce(o,
                l * 2 - y, 0, u, y) * 0.5 + u * 0.5 + r
    }})
}(jQuery);
(function (c) {
    c.effects.blind = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left"], a = c.effects.setMode(e, b.options.mode || "hide"), g = b.options.direction || "vertical";
            c.effects.save(e, f);
            e.show();
            var h = c.effects.createWrapper(e).css({overflow: "hidden"}), p = g == "vertical" ? "height" : "width";
            g = g == "vertical" ? h.height() : h.width();
            a == "show" && h.css(p, 0);
            var q = {};
            q[p] = a == "show" ? g : 0;
            h.animate(q, b.duration, b.options.easing, function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(e[0], arguments);
                e.dequeue()
            })
        })
    }
})(jQuery);
(function (c) {
    c.effects.bounce = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left"], a = c.effects.setMode(e, b.options.mode || "effect"), g = b.options.direction || "up", h = b.options.distance || 20, p = b.options.times || 5, q = b.duration || 250;
            /show|hide/.test(a) && f.push("opacity");
            c.effects.save(e, f);
            e.show();
            c.effects.createWrapper(e);
            var o = g == "up" || g == "down" ? "top" : "left";
            g = g == "up" || g == "left" ? "pos" : "neg";
            h = b.options.distance || (o == "top" ? e.outerHeight({margin: true}) / 3 : e.outerWidth({margin: true}) /
                3);
            if (a == "show")e.css("opacity", 0).css(o, g == "pos" ? -h : h);
            if (a == "hide")h /= p * 2;
            a != "hide" && p--;
            if (a == "show") {
                var l = {opacity: 1};
                l[o] = (g == "pos" ? "+=" : "-=") + h;
                e.animate(l, q / 2, b.options.easing);
                h /= 2;
                p--
            }
            for (l = 0; l < p; l++) {
                var r = {}, u = {};
                r[o] = (g == "pos" ? "-=" : "+=") + h;
                u[o] = (g == "pos" ? "+=" : "-=") + h;
                e.animate(r, q / 2, b.options.easing).animate(u, q / 2, b.options.easing);
                h = a == "hide" ? h * 2 : h / 2
            }
            if (a == "hide") {
                l = {opacity: 0};
                l[o] = (g == "pos" ? "-=" : "+=") + h;
                e.animate(l, q / 2, b.options.easing, function () {
                    e.hide();
                    c.effects.restore(e, f);
                    c.effects.removeWrapper(e);
                    b.callback && b.callback.apply(this, arguments)
                })
            } else {
                r = {};
                u = {};
                r[o] = (g == "pos" ? "-=" : "+=") + h;
                u[o] = (g == "pos" ? "+=" : "-=") + h;
                e.animate(r, q / 2, b.options.easing).animate(u, q / 2, b.options.easing, function () {
                    c.effects.restore(e, f);
                    c.effects.removeWrapper(e);
                    b.callback && b.callback.apply(this, arguments)
                })
            }
            e.queue("fx", function () {
                e.dequeue()
            });
            e.dequeue()
        })
    }
})(jQuery);
(function (c) {
    c.effects.clip = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left", "height", "width"], a = c.effects.setMode(e, b.options.mode || "hide"), g = b.options.direction || "vertical";
            c.effects.save(e, f);
            e.show();
            var h = c.effects.createWrapper(e).css({overflow: "hidden"});
            h = e[0].tagName == "IMG" ? h : e;
            var p = {size: g == "vertical" ? "height" : "width", position: g == "vertical" ? "top" : "left"};
            g = g == "vertical" ? h.height() : h.width();
            if (a == "show") {
                h.css(p.size, 0);
                h.css(p.position, g / 2)
            }
            var q = {};
            q[p.size] =
                    a == "show" ? g : 0;
            q[p.position] = a == "show" ? 0 : g / 2;
            h.animate(q, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(e[0], arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.drop = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left", "opacity"], a = c.effects.setMode(e, b.options.mode || "hide"), g = b.options.direction || "left";
            c.effects.save(e, f);
            e.show();
            c.effects.createWrapper(e);
            var h = g == "up" || g == "down" ? "top" : "left";
            g = g == "up" || g == "left" ? "pos" : "neg";
            var p = b.options.distance || (h == "top" ? e.outerHeight({margin: true}) / 2 : e.outerWidth({margin: true}) / 2);
            if (a == "show")e.css("opacity", 0).css(h, g == "pos" ? -p : p);
            var q = {opacity: a == "show" ? 1 :
                0};
            q[h] = (a == "show" ? g == "pos" ? "+=" : "-=" : g == "pos" ? "-=" : "+=") + p;
            e.animate(q, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(this, arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.explode = function (b) {
        return this.queue(function () {
            var e = b.options.pieces ? Math.round(Math.sqrt(b.options.pieces)) : 3, f = b.options.pieces ? Math.round(Math.sqrt(b.options.pieces)) : 3;
            b.options.mode = b.options.mode == "toggle" ? c(this).is(":visible") ? "hide" : "show" : b.options.mode;
            var a = c(this).show().css("visibility", "hidden"), g = a.offset();
            g.top -= parseInt(a.css("marginTop"), 10) || 0;
            g.left -= parseInt(a.css("marginLeft"), 10) || 0;
            for (var h = a.outerWidth(true), p = a.outerHeight(true), q = 0; q < e; q++)for (var o =
                0; o < f; o++)a.clone().appendTo("body").wrap("<div></div>").css({position: "absolute", visibility: "visible", left: -o * (h / f), top: -q * (p / e)}).parent().addClass("ui-effects-explode").css({position: "absolute", overflow: "hidden", width: h / f, height: p / e, left: g.left + o * (h / f) + (b.options.mode == "show" ? (o - Math.floor(f / 2)) * (h / f) : 0), top: g.top + q * (p / e) + (b.options.mode == "show" ? (q - Math.floor(e / 2)) * (p / e) : 0), opacity: b.options.mode == "show" ? 0 : 1}).animate({left: g.left + o * (h / f) + (b.options.mode == "show" ? 0 : (o - Math.floor(f / 2)) * (h / f)), top: g.top +
                q * (p / e) + (b.options.mode == "show" ? 0 : (q - Math.floor(e / 2)) * (p / e)), opacity: b.options.mode == "show" ? 1 : 0}, b.duration || 500);
            setTimeout(function () {
                b.options.mode == "show" ? a.css({visibility: "visible"}) : a.css({visibility: "visible"}).hide();
                b.callback && b.callback.apply(a[0]);
                a.dequeue();
                c("div.ui-effects-explode").remove()
            }, b.duration || 500)
        })
    }
})(jQuery);
(function (c) {
    c.effects.fade = function (b) {
        return this.queue(function () {
            var e = c(this), f = c.effects.setMode(e, b.options.mode || "hide");
            e.animate({opacity: f}, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                b.callback && b.callback.apply(this, arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.fold = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left"], a = c.effects.setMode(e, b.options.mode || "hide"), g = b.options.size || 15, h = !!b.options.horizFirst, p = b.duration ? b.duration / 2 : c.fx.speeds._default / 2;
            c.effects.save(e, f);
            e.show();
            var q = c.effects.createWrapper(e).css({overflow: "hidden"}), o = a == "show" != h, l = o ? ["width", "height"] : ["height", "width"];
            o = o ? [q.width(), q.height()] : [q.height(), q.width()];
            var r = /([0-9]+)%/.exec(g);
            if (r)g = parseInt(r[1], 10) / 100 *
                o[a == "hide" ? 0 : 1];
            if (a == "show")q.css(h ? {height: 0, width: g} : {height: g, width: 0});
            h = {};
            r = {};
            h[l[0]] = a == "show" ? o[0] : g;
            r[l[1]] = a == "show" ? o[1] : 0;
            q.animate(h, p, b.options.easing).animate(r, p, b.options.easing, function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(e[0], arguments);
                e.dequeue()
            })
        })
    }
})(jQuery);
(function (c) {
    c.effects.highlight = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["backgroundImage", "backgroundColor", "opacity"], a = c.effects.setMode(e, b.options.mode || "show"), g = {backgroundColor: e.css("backgroundColor")};
            if (a == "hide")g.opacity = 0;
            c.effects.save(e, f);
            e.show().css({backgroundImage: "none", backgroundColor: b.options.color || "#ffff99"}).animate(g, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                a == "show" && !c.support.opacity &&
                this.style.removeAttribute("filter");
                b.callback && b.callback.apply(this, arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.pulsate = function (b) {
        return this.queue(function () {
            var e = c(this), f = c.effects.setMode(e, b.options.mode || "show");
            times = (b.options.times || 5) * 2 - 1;
            duration = b.duration ? b.duration / 2 : c.fx.speeds._default / 2;
            isVisible = e.is(":visible");
            animateTo = 0;
            if (!isVisible) {
                e.css("opacity", 0).show();
                animateTo = 1
            }
            if (f == "hide" && isVisible || f == "show" && !isVisible)times--;
            for (f = 0; f < times; f++) {
                e.animate({opacity: animateTo}, duration, b.options.easing);
                animateTo = (animateTo + 1) % 2
            }
            e.animate({opacity: animateTo}, duration,
                b.options.easing, function () {
                    animateTo == 0 && e.hide();
                    b.callback && b.callback.apply(this, arguments)
                });
            e.queue("fx", function () {
                e.dequeue()
            }).dequeue()
        })
    }
})(jQuery);
(function (c) {
    c.effects.puff = function (b) {
        return this.queue(function () {
            var e = c(this), f = c.effects.setMode(e, b.options.mode || "hide"), a = parseInt(b.options.percent, 10) || 150, g = a / 100, h = {height: e.height(), width: e.width()};
            c.extend(b.options, {fade: true, mode: f, percent: f == "hide" ? a : 100, from: f == "hide" ? h : {height: h.height * g, width: h.width * g}});
            e.effect("scale", b.options, b.duration, b.callback);
            e.dequeue()
        })
    };
    c.effects.scale = function (b) {
        return this.queue(function () {
            var e = c(this), f = c.extend(true, {}, b.options), a = c.effects.setMode(e,
                    b.options.mode || "effect"), g = parseInt(b.options.percent, 10) || (parseInt(b.options.percent, 10) == 0 ? 0 : a == "hide" ? 0 : 100), h = b.options.direction || "both", p = b.options.origin;
            if (a != "effect") {
                f.origin = p || ["middle", "center"];
                f.restore = true
            }
            p = {height: e.height(), width: e.width()};
            e.from = b.options.from || (a == "show" ? {height: 0, width: 0} : p);
            g = {y: h != "horizontal" ? g / 100 : 1, x: h != "vertical" ? g / 100 : 1};
            e.to = {height: p.height * g.y, width: p.width * g.x};
            if (b.options.fade) {
                if (a == "show") {
                    e.from.opacity = 0;
                    e.to.opacity = 1
                }
                if (a == "hide") {
                    e.from.opacity =
                        1;
                    e.to.opacity = 0
                }
            }
            f.from = e.from;
            f.to = e.to;
            f.mode = a;
            e.effect("size", f, b.duration, b.callback);
            e.dequeue()
        })
    };
    c.effects.size = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left", "width", "height", "overflow", "opacity"], a = ["position", "top", "left", "overflow", "opacity"], g = ["width", "height", "overflow"], h = ["fontSize"], p = ["borderTopWidth", "borderBottomWidth", "paddingTop", "paddingBottom"], q = ["borderLeftWidth", "borderRightWidth", "paddingLeft", "paddingRight"], o = c.effects.setMode(e,
                    b.options.mode || "effect"), l = b.options.restore || false, r = b.options.scale || "both", u = b.options.origin, y = {height: e.height(), width: e.width()};
            e.from = b.options.from || y;
            e.to = b.options.to || y;
            if (u) {
                u = c.effects.getBaseline(u, y);
                e.from.top = (y.height - e.from.height) * u.y;
                e.from.left = (y.width - e.from.width) * u.x;
                e.to.top = (y.height - e.to.height) * u.y;
                e.to.left = (y.width - e.to.width) * u.x
            }
            var A = {from: {y: e.from.height / y.height, x: e.from.width / y.width}, to: {y: e.to.height / y.height, x: e.to.width / y.width}};
            if (r == "box" || r == "both") {
                if (A.from.y !=
                    A.to.y) {
                    f = f.concat(p);
                    e.from = c.effects.setTransition(e, p, A.from.y, e.from);
                    e.to = c.effects.setTransition(e, p, A.to.y, e.to)
                }
                if (A.from.x != A.to.x) {
                    f = f.concat(q);
                    e.from = c.effects.setTransition(e, q, A.from.x, e.from);
                    e.to = c.effects.setTransition(e, q, A.to.x, e.to)
                }
            }
            if (r == "content" || r == "both")if (A.from.y != A.to.y) {
                f = f.concat(h);
                e.from = c.effects.setTransition(e, h, A.from.y, e.from);
                e.to = c.effects.setTransition(e, h, A.to.y, e.to)
            }
            c.effects.save(e, l ? f : a);
            e.show();
            c.effects.createWrapper(e);
            e.css("overflow", "hidden").css(e.from);
            if (r == "content" || r == "both") {
                p = p.concat(["marginTop", "marginBottom"]).concat(h);
                q = q.concat(["marginLeft", "marginRight"]);
                g = f.concat(p).concat(q);
                e.find("*[width]").each(function () {
                    child = c(this);
                    l && c.effects.save(child, g);
                    var i = {height: child.height(), width: child.width()};
                    child.from = {height: i.height * A.from.y, width: i.width * A.from.x};
                    child.to = {height: i.height * A.to.y, width: i.width * A.to.x};
                    if (A.from.y != A.to.y) {
                        child.from = c.effects.setTransition(child, p, A.from.y, child.from);
                        child.to = c.effects.setTransition(child,
                            p, A.to.y, child.to)
                    }
                    if (A.from.x != A.to.x) {
                        child.from = c.effects.setTransition(child, q, A.from.x, child.from);
                        child.to = c.effects.setTransition(child, q, A.to.x, child.to)
                    }
                    child.css(child.from);
                    child.animate(child.to, b.duration, b.options.easing, function () {
                        l && c.effects.restore(child, g)
                    })
                })
            }
            e.animate(e.to, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                e.to.opacity === 0 && e.css("opacity", e.from.opacity);
                o == "hide" && e.hide();
                c.effects.restore(e, l ? f : a);
                c.effects.removeWrapper(e);
                b.callback &&
                b.callback.apply(this, arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.shake = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left"];
            c.effects.setMode(e, b.options.mode || "effect");
            var a = b.options.direction || "left", g = b.options.distance || 20, h = b.options.times || 3, p = b.duration || b.options.duration || 140;
            c.effects.save(e, f);
            e.show();
            c.effects.createWrapper(e);
            var q = a == "up" || a == "down" ? "top" : "left", o = a == "up" || a == "left" ? "pos" : "neg";
            a = {};
            var l = {}, r = {};
            a[q] = (o == "pos" ? "-=" : "+=") + g;
            l[q] = (o == "pos" ? "+=" : "-=") + g * 2;
            r[q] = (o == "pos" ? "-=" : "+=") +
                g * 2;
            e.animate(a, p, b.options.easing);
            for (g = 1; g < h; g++)e.animate(l, p, b.options.easing).animate(r, p, b.options.easing);
            e.animate(l, p, b.options.easing).animate(a, p / 2, b.options.easing, function () {
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(this, arguments)
            });
            e.queue("fx", function () {
                e.dequeue()
            });
            e.dequeue()
        })
    }
})(jQuery);
(function (c) {
    c.effects.slide = function (b) {
        return this.queue(function () {
            var e = c(this), f = ["position", "top", "left"], a = c.effects.setMode(e, b.options.mode || "show"), g = b.options.direction || "left";
            c.effects.save(e, f);
            e.show();
            c.effects.createWrapper(e).css({overflow: "hidden"});
            var h = g == "up" || g == "down" ? "top" : "left";
            g = g == "up" || g == "left" ? "pos" : "neg";
            var p = b.options.distance || (h == "top" ? e.outerHeight({margin: true}) : e.outerWidth({margin: true}));
            if (a == "show")e.css(h, g == "pos" ? -p : p);
            var q = {};
            q[h] = (a == "show" ? g == "pos" ?
                "+=" : "-=" : g == "pos" ? "-=" : "+=") + p;
            e.animate(q, {queue: false, duration: b.duration, easing: b.options.easing, complete: function () {
                a == "hide" && e.hide();
                c.effects.restore(e, f);
                c.effects.removeWrapper(e);
                b.callback && b.callback.apply(this, arguments);
                e.dequeue()
            }})
        })
    }
})(jQuery);
(function (c) {
    c.effects.transfer = function (b) {
        return this.queue(function () {
            var e = c(this), f = c(b.options.to), a = f.offset();
            f = {top: a.top, left: a.left, height: f.innerHeight(), width: f.innerWidth()};
            a = e.offset();
            var g = c('<div class="ui-effects-transfer"></div>').appendTo(document.body).addClass(b.options.className).css({top: a.top, left: a.left, height: e.innerHeight(), width: e.innerWidth(), position: "absolute"}).animate(f, b.duration, b.options.easing, function () {
                g.remove();
                b.callback && b.callback.apply(e[0], arguments);
                e.dequeue()
            })
        })
    }
})(jQuery);
(function (c) {
    c.widget("ui.accordion", {options: {active: 0, animated: "slide", autoHeight: true, clearStyle: false, collapsible: false, event: "click", fillSpace: false, header: "> li > :first-child,> :not(li):even", icons: {header: "ui-icon-triangle-1-e", headerSelected: "ui-icon-triangle-1-s"}, navigation: false, navigationFilter: function () {
        return this.href.toLowerCase() == location.href.toLowerCase()
    }}, _create: function () {
        var b = this.options, e = this;
        this.running = 0;
        this.element.addClass("ui-accordion ui-widget ui-helper-reset");
        this.element.children("li").addClass("ui-accordion-li-fix");
        this.headers = this.element.find(b.header).addClass("ui-accordion-header ui-helper-reset ui-state-default ui-corner-all").bind("mouseenter.accordion", function () {
            c(this).addClass("ui-state-hover")
        }).bind("mouseleave.accordion", function () {
            c(this).removeClass("ui-state-hover")
        }).bind("focus.accordion", function () {
            c(this).addClass("ui-state-focus")
        }).bind("blur.accordion", function () {
            c(this).removeClass("ui-state-focus")
        });
        this.headers.next().addClass("ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom");
        if (b.navigation) {
            var f = this.element.find("a").filter(b.navigationFilter);
            if (f.length) {
                var a = f.closest(".ui-accordion-header");
                this.active = a.length ? a : f.closest(".ui-accordion-content").prev()
            }
        }
        this.active = this._findActive(this.active || b.active).toggleClass("ui-state-default").toggleClass("ui-state-active").toggleClass("ui-corner-all").toggleClass("ui-corner-top");
        this.active.next().addClass("ui-accordion-content-active");
        this._createIcons();
        this.resize();
        this.element.attr("role", "tablist");
        this.headers.attr("role",
            "tab").bind("keydown", function (g) {
                return e._keydown(g)
            }).next().attr("role", "tabpanel");
        this.headers.not(this.active || "").attr("aria-expanded", "false").attr("tabIndex", "-1").next().hide();
        this.active.length ? this.active.attr("aria-expanded", "true").attr("tabIndex", "0") : this.headers.eq(0).attr("tabIndex", "0");
        c.browser.safari || this.headers.find("a").attr("tabIndex", "-1");
        b.event && this.headers.bind(b.event + ".accordion", function (g) {
            e._clickHandler.call(e, g, this);
            g.preventDefault()
        })
    }, _createIcons: function () {
        var b =
            this.options;
        if (b.icons) {
            c("<span/>").addClass("ui-icon " + b.icons.header).prependTo(this.headers);
            this.active.find(".ui-icon").toggleClass(b.icons.header).toggleClass(b.icons.headerSelected);
            this.element.addClass("ui-accordion-icons")
        }
    }, _destroyIcons: function () {
        this.headers.children(".ui-icon").remove();
        this.element.removeClass("ui-accordion-icons")
    }, destroy: function () {
        var b = this.options;
        this.element.removeClass("ui-accordion ui-widget ui-helper-reset").removeAttr("role").unbind(".accordion").removeData("accordion");
        this.headers.unbind(".accordion").removeClass("ui-accordion-header ui-helper-reset ui-state-default ui-corner-all ui-state-active ui-corner-top").removeAttr("role").removeAttr("aria-expanded").removeAttr("tabIndex");
        this.headers.find("a").removeAttr("tabIndex");
        this._destroyIcons();
        var e = this.headers.next().css("display", "").removeAttr("role").removeClass("ui-helper-reset ui-widget-content ui-corner-bottom ui-accordion-content ui-accordion-content-active");
        if (b.autoHeight || b.fillHeight)e.css("height",
            "");
        return this
    }, _setOption: function (b, e) {
        c.Widget.prototype._setOption.apply(this, arguments);
        b == "active" && this.activate(e);
        if (b == "icons") {
            this._destroyIcons();
            e && this._createIcons()
        }
    }, _keydown: function (b) {
        var e = c.ui.keyCode;
        if (!(this.options.disabled || b.altKey || b.ctrlKey)) {
            var f = this.headers.length, a = this.headers.index(b.target), g = false;
            switch (b.keyCode) {
                case e.RIGHT:
                case e.DOWN:
                    g = this.headers[(a + 1) % f];
                    break;
                case e.LEFT:
                case e.UP:
                    g = this.headers[(a - 1 + f) % f];
                    break;
                case e.SPACE:
                case e.ENTER:
                    this._clickHandler({target: b.target},
                        b.target);
                    b.preventDefault()
            }
            if (g) {
                c(b.target).attr("tabIndex", "-1");
                c(g).attr("tabIndex", "0");
                g.focus();
                return false
            }
            return true
        }
    }, resize: function () {
        var b = this.options, e;
        if (b.fillSpace) {
            if (c.browser.msie) {
                var f = this.element.parent().css("overflow");
                this.element.parent().css("overflow", "hidden")
            }
            e = this.element.parent().height();
            c.browser.msie && this.element.parent().css("overflow", f);
            this.headers.each(function () {
                e -= c(this).outerHeight(true)
            });
            this.headers.next().each(function () {
                c(this).height(Math.max(0,
                        e - c(this).innerHeight() + c(this).height()))
            }).css("overflow", "auto")
        } else if (b.autoHeight) {
            e = 0;
            this.headers.next().each(function () {
                e = Math.max(e, c(this).height())
            }).height(e)
        }
        return this
    }, activate: function (b) {
        this.options.active = b;
        b = this._findActive(b)[0];
        this._clickHandler({target: b}, b);
        return this
    }, _findActive: function (b) {
        return b ? typeof b == "number" ? this.headers.filter(":eq(" + b + ")") : this.headers.not(this.headers.not(b)) : b === false ? c([]) : this.headers.filter(":eq(0)")
    }, _clickHandler: function (b, e) {
        var f =
            this.options;
        if (!f.disabled)if (b.target) {
            b = c(b.currentTarget || e);
            e = b[0] == this.active[0];
            f.active = f.collapsible && e ? false : c(".ui-accordion-header", this.element).index(b);
            if (!(this.running || !f.collapsible && e)) {
                this.active.removeClass("ui-state-active ui-corner-top").addClass("ui-state-default ui-corner-all").find(".ui-icon").removeClass(f.icons.headerSelected).addClass(f.icons.header);
                if (!e) {
                    b.removeClass("ui-state-default ui-corner-all").addClass("ui-state-active ui-corner-top").find(".ui-icon").removeClass(f.icons.header).addClass(f.icons.headerSelected);
                    b.next().addClass("ui-accordion-content-active")
                }
                h = b.next();
                a = this.active.next();
                g = {options: f, newHeader: e && f.collapsible ? c([]) : b, oldHeader: this.active, newContent: e && f.collapsible ? c([]) : h, oldContent: a};
                f = this.headers.index(this.active[0]) > this.headers.index(b[0]);
                this.active = e ? c([]) : b;
                this._toggle(h, a, g, e, f)
            }
        } else if (f.collapsible) {
            this.active.removeClass("ui-state-active ui-corner-top").addClass("ui-state-default ui-corner-all").find(".ui-icon").removeClass(f.icons.headerSelected).addClass(f.icons.header);
            this.active.next().addClass("ui-accordion-content-active");
            var a = this.active.next(), g = {options: f, newHeader: c([]), oldHeader: f.active, newContent: c([]), oldContent: a}, h = this.active = c([]);
            this._toggle(h, a, g)
        }
    }, _toggle: function (b, e, f, a, g) {
        var h = this.options, p = this;
        this.toShow = b;
        this.toHide = e;
        this.data = f;
        var q = function () {
            if (p)return p._completed.apply(p, arguments)
        };
        this._trigger("changestart", null, this.data);
        this.running = e.size() === 0 ? b.size() : e.size();
        if (h.animated) {
            f = {};
            f = h.collapsible && a ? {toShow: c([]),
                toHide: e, complete: q, down: g, autoHeight: h.autoHeight || h.fillSpace} : {toShow: b, toHide: e, complete: q, down: g, autoHeight: h.autoHeight || h.fillSpace};
            if (!h.proxied)h.proxied = h.animated;
            if (!h.proxiedDuration)h.proxiedDuration = h.duration;
            h.animated = c.isFunction(h.proxied) ? h.proxied(f) : h.proxied;
            h.duration = c.isFunction(h.proxiedDuration) ? h.proxiedDuration(f) : h.proxiedDuration;
            a = c.ui.accordion.animations;
            var o = h.duration, l = h.animated;
            if (l && !a[l] && !c.easing[l])l = "slide";
            a[l] || (a[l] = function (r) {
                this.slide(r, {easing: l,
                    duration: o || 700})
            });
            a[l](f)
        } else {
            if (h.collapsible && a)b.toggle(); else {
                e.hide();
                b.show()
            }
            q(true)
        }
        e.prev().attr("aria-expanded", "false").attr("tabIndex", "-1").blur();
        b.prev().attr("aria-expanded", "true").attr("tabIndex", "0").focus()
    }, _completed: function (b) {
        var e = this.options;
        this.running = b ? 0 : --this.running;
        if (!this.running) {
            e.clearStyle && this.toShow.add(this.toHide).css({height: "", overflow: ""});
            this.toHide.removeClass("ui-accordion-content-active");
            this._trigger("change", null, this.data)
        }
    }});
    c.extend(c.ui.accordion,
        {version: "1.8.2", animations: {slide: function (b, e) {
            b = c.extend({easing: "swing", duration: 300}, b, e);
            if (b.toHide.size())if (b.toShow.size()) {
                var f = b.toShow.css("overflow"), a = 0, g = {}, h = {}, p;
                e = b.toShow;
                p = e[0].style.width;
                e.width(parseInt(e.parent().width(), 10) - parseInt(e.css("paddingLeft"), 10) - parseInt(e.css("paddingRight"), 10) - (parseInt(e.css("borderLeftWidth"), 10) || 0) - (parseInt(e.css("borderRightWidth"), 10) || 0));
                c.each(["height", "paddingTop", "paddingBottom"], function (q, o) {
                    h[o] = "hide";
                    q = ("" + c.css(b.toShow[0],
                        o)).match(/^([\d+-.]+)(.*)$/);
                    g[o] = {value: q[1], unit: q[2] || "px"}
                });
                b.toShow.css({height: 0, overflow: "hidden"}).show();
                b.toHide.filter(":hidden").each(b.complete).end().filter(":visible").animate(h, {step: function (q, o) {
                    if (o.prop == "height")a = o.end - o.start === 0 ? 0 : (o.now - o.start) / (o.end - o.start);
                    b.toShow[0].style[o.prop] = a * g[o.prop].value + g[o.prop].unit
                }, duration: b.duration, easing: b.easing, complete: function () {
                    b.autoHeight || b.toShow.css("height", "");
                    b.toShow.css("width", p);
                    b.toShow.css({overflow: f});
                    b.complete()
                }})
            } else b.toHide.animate({height: "hide"},
                b); else b.toShow.animate({height: "show"}, b)
        }, bounceslide: function (b) {
            this.slide(b, {easing: b.down ? "easeOutBounce" : "swing", duration: b.down ? 1E3 : 200})
        }}})
})(jQuery);
(function (c) {
    c.widget("ui.autocomplete", {options: {minLength: 1, delay: 300}, _create: function () {
        var b = this, e = this.element[0].ownerDocument;
        this.element.addClass("ui-autocomplete-input").attr("autocomplete", "off").attr({role: "textbox", "aria-autocomplete": "list", "aria-haspopup": "true"}).bind("keydown.autocomplete", function (f) {
            var a = c.ui.keyCode;
            switch (f.keyCode) {
                case a.PAGE_UP:
                    b._move("previousPage", f);
                    break;
                case a.PAGE_DOWN:
                    b._move("nextPage", f);
                    break;
                case a.UP:
                    b._move("previous", f);
                    f.preventDefault();
                    break;
                case a.DOWN:
                    b._move("next", f);
                    f.preventDefault();
                    break;
                case a.ENTER:
                case a.NUMPAD_ENTER:
                    b.menu.active && f.preventDefault();
                case a.TAB:
                    if (!b.menu.active)return;
                    b.menu.select(f);
                    break;
                case a.ESCAPE:
                    b.element.val(b.term);
                    b.close(f);
                    break;
                case a.LEFT:
                case a.RIGHT:
                case a.SHIFT:
                case a.CONTROL:
                case a.ALT:
                case a.COMMAND:
                case a.COMMAND_RIGHT:
                case a.INSERT:
                case a.CAPS_LOCK:
                case a.END:
                case a.HOME:
                    break;
                default:
                    clearTimeout(b.searching);
                    b.searching = setTimeout(function () {
                        b.search(null, f)
                    }, b.options.delay);
                    break
            }
        }).bind("focus.autocomplete", function () {
            b.selectedItem = null;
            b.previous = b.element.val()
        }).bind("blur.autocomplete", function (f) {
            clearTimeout(b.searching);
            b.closing = setTimeout(function () {
                b.close(f);
                b._change(f)
            }, 150)
        });
        this._initSource();
        this.response = function () {
            return b._response.apply(b, arguments)
        };
        this.menu = c("<ul></ul>").addClass("ui-autocomplete").appendTo("body", e).mousedown(function () {
            setTimeout(function () {
                clearTimeout(b.closing)
            }, 13)
        }).menu({focus: function (f, a) {
            a = a.item.data("item.autocomplete");
            false !== b._trigger("focus", null, {item: a}) && /^key/.test(f.originalEvent.type) && b.element.val(a.value)
        }, selected: function (f, a) {
            a = a.item.data("item.autocomplete");
            false !== b._trigger("select", f, {item: a}) && b.element.val(a.value);
            b.close(f);
            f = b.previous;
            if (b.element[0] !== e.activeElement) {
                b.element.focus();
                b.previous = f
            }
            b.selectedItem = a
        }, blur: function () {
            b.menu.element.is(":visible") && b.element.val(b.term)
        }}).zIndex(this.element.zIndex() + 1).css({top: 0, left: 0}).hide().data("menu");
        c.fn.bgiframe && this.menu.element.bgiframe()
    },
        destroy: function () {
            this.element.removeClass("ui-autocomplete-input").removeAttr("autocomplete").removeAttr("role").removeAttr("aria-autocomplete").removeAttr("aria-haspopup");
            this.menu.element.remove();
            c.Widget.prototype.destroy.call(this)
        }, _setOption: function (b) {
            c.Widget.prototype._setOption.apply(this, arguments);
            b === "source" && this._initSource()
        }, _initSource: function () {
            var b, e;
            if (c.isArray(this.options.source)) {
                b = this.options.source;
                this.source = function (f, a) {
                    a(c.ui.autocomplete.filter(b, f.term))
                }
            } else if (typeof this.options.source ===
                "string") {
                e = this.options.source;
                this.source = function (f, a) {
                    c.getJSON(e, f, a)
                }
            } else this.source = this.options.source
        }, search: function (b, e) {
            b = b != null ? b : this.element.val();
            if (b.length < this.options.minLength)return this.close(e);
            clearTimeout(this.closing);
            if (this._trigger("search") !== false)return this._search(b)
        }, _search: function (b) {
            this.term = this.element.addClass("ui-autocomplete-loading").val();
            this.source({term: b}, this.response)
        }, _response: function (b) {
            if (b.length) {
                b = this._normalize(b);
                this._suggest(b);
                this._trigger("open")
            } else this.close();
            this.element.removeClass("ui-autocomplete-loading")
        }, close: function (b) {
            clearTimeout(this.closing);
            if (this.menu.element.is(":visible")) {
                this._trigger("close", b);
                this.menu.element.hide();
                this.menu.deactivate()
            }
        }, _change: function (b) {
            this.previous !== this.element.val() && this._trigger("change", b, {item: this.selectedItem})
        }, _normalize: function (b) {
            if (b.length && b[0].label && b[0].value)return b;
            return c.map(b, function (e) {
                if (typeof e === "string")return{label: e, value: e};
                return c.extend({label: e.label ||
                    e.value, value: e.value || e.label}, e)
            })
        }, _suggest: function (b) {
            var e = this.menu.element.empty().zIndex(this.element.zIndex() + 1), f;
            this._renderMenu(e, b);
            this.menu.deactivate();
            this.menu.refresh();
            this.menu.element.show().position({my: "left top", at: "left bottom", of: this.element, collision: "none"});
            b = e.width("").width();
            f = this.element.width();
            e.width(Math.max(b, f))
        }, _renderMenu: function (b, e) {
            var f = this;
            c.each(e, function (a, g) {
                f._renderItem(b, g)
            })
        }, _renderItem: function (b, e) {
            return c("<li></li>").data("item.autocomplete",
                e).append("<a>" + e.label + "</a>").appendTo(b)
        }, _move: function (b, e) {
            if (this.menu.element.is(":visible"))if (this.menu.first() && /^previous/.test(b) || this.menu.last() && /^next/.test(b)) {
                this.element.val(this.term);
                this.menu.deactivate()
            } else this.menu[b](e); else this.search(null, e)
        }, widget: function () {
            return this.menu.element
        }});
    c.extend(c.ui.autocomplete, {escapeRegex: function (b) {
        return b.replace(/([\^\$\(\)\[\]\{\}\*\.\+\?\|\\])/gi, "\\$1")
    }, filter: function (b, e) {
        var f = RegExp(c.ui.autocomplete.escapeRegex(e),
            "i");
        return c.grep(b, function (a) {
            return f.test(a.label || a.value || a)
        })
    }})
})(jQuery);
(function (c) {
    c.widget("ui.menu", {_create: function () {
        var b = this;
        this.element.addClass("ui-menu ui-widget ui-widget-content ui-corner-all").attr({role: "listbox", "aria-activedescendant": "ui-active-menuitem"}).click(function (e) {
            if (c(e.target).closest(".ui-menu-item a").length) {
                e.preventDefault();
                b.select(e)
            }
        });
        this.refresh()
    }, refresh: function () {
        var b = this;
        this.element.children("li:not(.ui-menu-item):has(a)").addClass("ui-menu-item").attr("role", "menuitem").children("a").addClass("ui-corner-all").attr("tabindex",
            -1).mouseenter(function (e) {
                b.activate(e, c(this).parent())
            }).mouseleave(function () {
                b.deactivate()
            })
    }, activate: function (b, e) {
        this.deactivate();
        if (this.hasScroll()) {
            var f = e.offset().top - this.element.offset().top, a = this.element.attr("scrollTop"), g = this.element.height();
            if (f < 0)this.element.attr("scrollTop", a + f); else f > g && this.element.attr("scrollTop", a + f - g + e.height())
        }
        this.active = e.eq(0).children("a").addClass("ui-state-hover").attr("id", "ui-active-menuitem").end();
        this._trigger("focus", b, {item: e})
    }, deactivate: function () {
        if (this.active) {
            this.active.children("a").removeClass("ui-state-hover").removeAttr("id");
            this._trigger("blur");
            this.active = null
        }
    }, next: function (b) {
        this.move("next", ".ui-menu-item:first", b)
    }, previous: function (b) {
        this.move("prev", ".ui-menu-item:last", b)
    }, first: function () {
        return this.active && !this.active.prev().length
    }, last: function () {
        return this.active && !this.active.next().length
    }, move: function (b, e, f) {
        if (this.active) {
            b = this.active[b + "All"](".ui-menu-item").eq(0);
            b.length ? this.activate(f, b) : this.activate(f, this.element.children(e))
        } else this.activate(f, this.element.children(e))
    }, nextPage: function (b) {
        if (this.hasScroll())if (!this.active ||
            this.last())this.activate(b, this.element.children(":first")); else {
            var e = this.active.offset().top, f = this.element.height(), a = this.element.children("li").filter(function () {
                var g = c(this).offset().top - e - f + c(this).height();
                return g < 10 && g > -10
            });
            a.length || (a = this.element.children(":last"));
            this.activate(b, a)
        } else this.activate(b, this.element.children(!this.active || this.last() ? ":first" : ":last"))
    }, previousPage: function (b) {
        if (this.hasScroll())if (!this.active || this.first())this.activate(b, this.element.children(":last"));
        else {
            var e = this.active.offset().top, f = this.element.height();
            result = this.element.children("li").filter(function () {
                var a = c(this).offset().top - e + f - c(this).height();
                return a < 10 && a > -10
            });
            result.length || (result = this.element.children(":first"));
            this.activate(b, result)
        } else this.activate(b, this.element.children(!this.active || this.first() ? ":last" : ":first"))
    }, hasScroll: function () {
        return this.element.height() < this.element.attr("scrollHeight")
    }, select: function (b) {
        this._trigger("selected", b, {item: this.active})
    }})
})(jQuery);
(function (c) {
    var b, e = function (a) {
        c(":ui-button", a.target.form).each(function () {
            var g = c(this).data("button");
            setTimeout(function () {
                g.refresh()
            }, 1)
        })
    }, f = function (a) {
        var g = a.name, h = a.form, p = c([]);
        if (g)p = h ? c(h).find("[name='" + g + "']") : c("[name='" + g + "']", a.ownerDocument).filter(function () {
            return!this.form
        });
        return p
    };
    c.widget("ui.button", {options: {text: true, label: null, icons: {primary: null, secondary: null}}, _create: function () {
        this.element.closest("form").unbind("reset.button").bind("reset.button", e);
        this._determineButtonType();
        this.hasTitle = !!this.buttonElement.attr("title");
        var a = this, g = this.options, h = this.type === "checkbox" || this.type === "radio", p = "ui-state-hover" + (!h ? " ui-state-active" : "");
        if (g.label === null)g.label = this.buttonElement.html();
        if (this.element.is(":disabled"))g.disabled = true;
        this.buttonElement.addClass("ui-button ui-widget ui-state-default ui-corner-all").attr("role", "button").bind("mouseenter.button", function () {
            if (!g.disabled) {
                c(this).addClass("ui-state-hover");
                this === b && c(this).addClass("ui-state-active")
            }
        }).bind("mouseleave.button",
            function () {
                g.disabled || c(this).removeClass(p)
            }).bind("focus.button", function () {
                c(this).addClass("ui-state-focus")
            }).bind("blur.button", function () {
                c(this).removeClass("ui-state-focus")
            });
        h && this.element.bind("change.button", function () {
            a.refresh()
        });
        if (this.type === "checkbox")this.buttonElement.bind("click.button", function () {
            if (g.disabled)return false;
            c(this).toggleClass("ui-state-active");
            a.buttonElement.attr("aria-pressed", a.element[0].checked)
        }); else if (this.type === "radio")this.buttonElement.bind("click.button",
            function () {
                if (g.disabled)return false;
                c(this).addClass("ui-state-active");
                a.buttonElement.attr("aria-pressed", true);
                var q = a.element[0];
                f(q).not(q).map(function () {
                    return c(this).button("widget")[0]
                }).removeClass("ui-state-active").attr("aria-pressed", false)
            }); else {
            this.buttonElement.bind("mousedown.button", function () {
                if (g.disabled)return false;
                c(this).addClass("ui-state-active");
                b = this;
                c(document).one("mouseup", function () {
                    b = null
                })
            }).bind("mouseup.button", function () {
                if (g.disabled)return false;
                c(this).removeClass("ui-state-active")
            }).bind("keydown.button",
                function (q) {
                    if (g.disabled)return false;
                    if (q.keyCode == c.ui.keyCode.SPACE || q.keyCode == c.ui.keyCode.ENTER)c(this).addClass("ui-state-active")
                }).bind("keyup.button", function () {
                    c(this).removeClass("ui-state-active")
                });
            this.buttonElement.is("a") && this.buttonElement.keyup(function (q) {
                q.keyCode === c.ui.keyCode.SPACE && c(this).click()
            })
        }
        this._setOption("disabled", g.disabled)
    }, _determineButtonType: function () {
        this.type = this.element.is(":checkbox") ? "checkbox" : this.element.is(":radio") ? "radio" : this.element.is("input") ?
            "input" : "button";
        if (this.type === "checkbox" || this.type === "radio") {
            this.buttonElement = this.element.parents().last().find("[for=" + this.element.attr("id") + "]");
            this.element.addClass("ui-helper-hidden-accessible");
            var a = this.element.is(":checked");
            a && this.buttonElement.addClass("ui-state-active");
            this.buttonElement.attr("aria-pressed", a)
        } else this.buttonElement = this.element
    }, widget: function () {
        return this.buttonElement
    }, destroy: function () {
        this.element.removeClass("ui-helper-hidden-accessible");
        this.buttonElement.removeClass("ui-button ui-widget ui-state-default ui-corner-all ui-state-hover ui-state-active  ui-button-icons-only ui-button-icon-only ui-button-text-icons ui-button-text-icon ui-button-text-only").removeAttr("role").removeAttr("aria-pressed").html(this.buttonElement.find(".ui-button-text").html());
        this.hasTitle || this.buttonElement.removeAttr("title");
        c.Widget.prototype.destroy.call(this)
    }, _setOption: function (a, g) {
        c.Widget.prototype._setOption.apply(this, arguments);
        if (a === "disabled")g ? this.element.attr("disabled", true) : this.element.removeAttr("disabled");
        this._resetButton()
    }, refresh: function () {
        var a = this.element.is(":disabled");
        a !== this.options.disabled && this._setOption("disabled", a);
        if (this.type === "radio")f(this.element[0]).each(function () {
            c(this).is(":checked") ? c(this).button("widget").addClass("ui-state-active").attr("aria-pressed",
                true) : c(this).button("widget").removeClass("ui-state-active").attr("aria-pressed", false)
        }); else if (this.type === "checkbox")this.element.is(":checked") ? this.buttonElement.addClass("ui-state-active").attr("aria-pressed", true) : this.buttonElement.removeClass("ui-state-active").attr("aria-pressed", false)
    }, _resetButton: function () {
        if (this.type === "input")this.options.label && this.element.val(this.options.label); else {
            var a = this.buttonElement.removeClass("ui-button-icons-only ui-button-icon-only ui-button-text-icons ui-button-text-icon ui-button-text-only"),
                g = c("<span></span>").addClass("ui-button-text").html(this.options.label).appendTo(a.empty()).text(), h = this.options.icons, p = h.primary && h.secondary;
            if (h.primary || h.secondary) {
                a.addClass("ui-button-text-icon" + (p ? "s" : ""));
                h.primary && a.prepend("<span class='ui-button-icon-primary ui-icon " + h.primary + "'></span>");
                h.secondary && a.append("<span class='ui-button-icon-secondary ui-icon " + h.secondary + "'></span>");
                if (!this.options.text) {
                    a.addClass(p ? "ui-button-icons-only" : "ui-button-icon-only").removeClass("ui-button-text-icons ui-button-text-icon");
                    this.hasTitle || a.attr("title", g)
                }
            } else a.addClass("ui-button-text-only")
        }
    }});
    c.widget("ui.buttonset", {_create: function () {
        this.element.addClass("ui-buttonset");
        this._init()
    }, _init: function () {
        this.refresh()
    }, _setOption: function (a, g) {
        a === "disabled" && this.buttons.button("option", a, g);
        c.Widget.prototype._setOption.apply(this, arguments)
    }, refresh: function () {
        this.buttons = this.element.find(":button, :submit, :reset, :checkbox, :radio, a, :data(button)").filter(":ui-button").button("refresh").end().not(":ui-button").button().end().map(function () {
            return c(this).button("widget")[0]
        }).removeClass("ui-corner-all ui-corner-left ui-corner-right").filter(":first").addClass("ui-corner-left").end().filter(":last").addClass("ui-corner-right").end().end()
    },
        destroy: function () {
            this.element.removeClass("ui-buttonset");
            this.buttons.map(function () {
                return c(this).button("widget")[0]
            }).removeClass("ui-corner-left ui-corner-right").end().button("destroy");
            c.Widget.prototype.destroy.call(this)
        }})
})(jQuery);
(function (c) {
    function b() {
        this.debug = false;
        this._curInst = null;
        this._keyEvent = false;
        this._disabledInputs = [];
        this._inDialog = this._datepickerShowing = false;
        this._mainDivId = "ui-datepicker-div";
        this._inlineClass = "ui-datepicker-inline";
        this._appendClass = "ui-datepicker-append";
        this._triggerClass = "ui-datepicker-trigger";
        this._dialogClass = "ui-datepicker-dialog";
        this._disableClass = "ui-datepicker-disabled";
        this._unselectableClass = "ui-datepicker-unselectable";
        this._currentClass = "ui-datepicker-current-day";
        this._dayOverClass =
            "ui-datepicker-days-cell-over";
        this.regional = [];
        this.regional[""] = {closeText: "Done", prevText: "Prev", nextText: "Next", currentText: "Today", monthNames: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"], monthNamesShort: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"], dayNames: ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"], dayNamesShort: ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"], dayNamesMin: ["Su",
            "Mo", "Tu", "We", "Th", "Fr", "Sa"], weekHeader: "Wk", dateFormat: "mm/dd/yy", firstDay: 0, isRTL: false, showMonthAfterYear: false, yearSuffix: ""};
        this._defaults = {showOn: "focus", showAnim: "fadeIn", showOptions: {}, defaultDate: null, appendText: "", buttonText: "...", buttonImage: "", buttonImageOnly: false, hideIfNoPrevNext: false, navigationAsDateFormat: false, gotoCurrent: false, changeMonth: false, changeYear: false, yearRange: "c-10:c+10", showOtherMonths: false, selectOtherMonths: false, showWeek: false, calculateWeek: this.iso8601Week, shortYearCutoff: "+10",
            minDate: null, maxDate: null, duration: "fast", beforeShowDay: null, beforeShow: null, onSelect: null, onChangeMonthYear: null, onClose: null, numberOfMonths: 1, showCurrentAtPos: 0, stepMonths: 1, stepBigMonths: 12, altField: "", altFormat: "", constrainInput: true, showButtonPanel: false, autoSize: false};
        c.extend(this._defaults, this.regional[""]);
        this.dpDiv = c('<div id="' + this._mainDivId + '" class="ui-datepicker ui-widget ui-widget-content ui-helper-clearfix ui-corner-all ui-helper-hidden-accessible"></div>')
    }

    function e(a, g) {
        c.extend(a,
            g);
        for (var h in g)if (g[h] == null || g[h] == undefined)a[h] = g[h];
        return a
    }

    c.extend(c.ui, {datepicker: {version: "1.8.2"}});
    var f = (new Date).getTime();
    c.extend(b.prototype, {markerClassName: "hasDatepicker", log: function () {
        this.debug && console.log.apply("", arguments)
    }, _widgetDatepicker: function () {
        return this.dpDiv
    }, setDefaults: function (a) {
        e(this._defaults, a || {});
        return this
    }, _attachDatepicker: function (a, g) {
        var h = null;
        for (var p in this._defaults) {
            var q = a.getAttribute("date:" + p);
            if (q) {
                h = h || {};
                try {
                    h[p] = eval(q)
                } catch (o) {
                    h[p] =
                        q
                }
            }
        }
        p = a.nodeName.toLowerCase();
        q = p == "div" || p == "span";
        if (!a.id) {
            this.uuid += 1;
            a.id = "dp" + this.uuid
        }
        var l = this._newInst(c(a), q);
        l.settings = c.extend({}, g || {}, h || {});
        if (p == "input")this._connectDatepicker(a, l); else q && this._inlineDatepicker(a, l)
    }, _newInst: function (a, g) {
        return{id: a[0].id.replace(/([^A-Za-z0-9_])/g, "\\\\$1"), input: a, selectedDay: 0, selectedMonth: 0, selectedYear: 0, drawMonth: 0, drawYear: 0, inline: g, dpDiv: !g ? this.dpDiv : c('<div class="' + this._inlineClass + ' ui-datepicker ui-widget ui-widget-content ui-helper-clearfix ui-corner-all"></div>')}
    },
        _connectDatepicker: function (a, g) {
            var h = c(a);
            g.append = c([]);
            g.trigger = c([]);
            if (!h.hasClass(this.markerClassName)) {
                this._attachments(h, g);
                h.addClass(this.markerClassName).keydown(this._doKeyDown).keypress(this._doKeyPress).keyup(this._doKeyUp).bind("setData.datepicker", function (p, q, o) {
                    g.settings[q] = o
                }).bind("getData.datepicker", function (p, q) {
                    return this._get(g, q)
                });
                this._autoSize(g);
                c.data(a, "datepicker", g)
            }
        }, _attachments: function (a, g) {
            var h = this._get(g, "appendText"), p = this._get(g, "isRTL");
            g.append &&
            g.append.remove();
            if (h) {
                g.append = c('<span class="' + this._appendClass + '">' + h + "</span>");
                a[p ? "before" : "after"](g.append)
            }
            a.unbind("focus", this._showDatepicker);
            g.trigger && g.trigger.remove();
            h = this._get(g, "showOn");
            if (h == "focus" || h == "both")a.focus(this._showDatepicker);
            if (h == "button" || h == "both") {
                h = this._get(g, "buttonText");
                var q = this._get(g, "buttonImage");
                g.trigger = c(this._get(g, "buttonImageOnly") ? c("<img/>").addClass(this._triggerClass).attr({src: q, alt: h, title: h}) : c('<button type="button"></button>').addClass(this._triggerClass).html(q ==
                    "" ? h : c("<img/>").attr({src: q, alt: h, title: h})));
                a[p ? "before" : "after"](g.trigger);
                g.trigger.click(function () {
                    c.datepicker._datepickerShowing && c.datepicker._lastInput == a[0] ? c.datepicker._hideDatepicker() : c.datepicker._showDatepicker(a[0]);
                    return false
                })
            }
        }, _autoSize: function (a) {
            if (this._get(a, "autoSize") && !a.inline) {
                var g = new Date(2009, 11, 20), h = this._get(a, "dateFormat");
                if (h.match(/[DM]/)) {
                    var p = function (q) {
                        for (var o = 0, l = 0, r = 0; r < q.length; r++)if (q[r].length > o) {
                            o = q[r].length;
                            l = r
                        }
                        return l
                    };
                    g.setMonth(p(this._get(a,
                        h.match(/MM/) ? "monthNames" : "monthNamesShort")));
                    g.setDate(p(this._get(a, h.match(/DD/) ? "dayNames" : "dayNamesShort")) + 20 - g.getDay())
                }
                a.input.attr("size", this._formatDate(a, g).length)
            }
        }, _inlineDatepicker: function (a, g) {
            var h = c(a);
            if (!h.hasClass(this.markerClassName)) {
                h.addClass(this.markerClassName).append(g.dpDiv).bind("setData.datepicker", function (p, q, o) {
                    g.settings[q] = o
                }).bind("getData.datepicker", function (p, q) {
                    return this._get(g, q)
                });
                c.data(a, "datepicker", g);
                this._setDate(g, this._getDefaultDate(g),
                    true);
                this._updateDatepicker(g);
                this._updateAlternate(g)
            }
        }, _dialogDatepicker: function (a, g, h, p, q) {
            a = this._dialogInst;
            if (!a) {
                this.uuid += 1;
                this._dialogInput = c('<input type="text" id="' + ("dp" + this.uuid) + '" style="position: absolute; top: -100px; width: 0px; z-index: -10;"/>');
                this._dialogInput.keydown(this._doKeyDown);
                c("body").append(this._dialogInput);
                a = this._dialogInst = this._newInst(this._dialogInput, false);
                a.settings = {};
                c.data(this._dialogInput[0], "datepicker", a)
            }
            e(a.settings, p || {});
            g = g && g.constructor ==
                Date ? this._formatDate(a, g) : g;
            this._dialogInput.val(g);
            this._pos = q ? q.length ? q : [q.pageX, q.pageY] : null;
            if (!this._pos)this._pos = [document.documentElement.clientWidth / 2 - 100 + (document.documentElement.scrollLeft || document.body.scrollLeft), document.documentElement.clientHeight / 2 - 150 + (document.documentElement.scrollTop || document.body.scrollTop)];
            this._dialogInput.css("left", this._pos[0] + 20 + "px").css("top", this._pos[1] + "px");
            a.settings.onSelect = h;
            this._inDialog = true;
            this.dpDiv.addClass(this._dialogClass);
            this._showDatepicker(this._dialogInput[0]);
            c.blockUI && c.blockUI(this.dpDiv);
            c.data(this._dialogInput[0], "datepicker", a);
            return this
        }, _destroyDatepicker: function (a) {
            var g = c(a), h = c.data(a, "datepicker");
            if (g.hasClass(this.markerClassName)) {
                var p = a.nodeName.toLowerCase();
                c.removeData(a, "datepicker");
                if (p == "input") {
                    h.append.remove();
                    h.trigger.remove();
                    g.removeClass(this.markerClassName).unbind("focus", this._showDatepicker).unbind("keydown", this._doKeyDown).unbind("keypress", this._doKeyPress).unbind("keyup", this._doKeyUp)
                } else if (p == "div" || p == "span")g.removeClass(this.markerClassName).empty()
            }
        },
        _enableDatepicker: function (a) {
            var g = c(a), h = c.data(a, "datepicker");
            if (g.hasClass(this.markerClassName)) {
                var p = a.nodeName.toLowerCase();
                if (p == "input") {
                    a.disabled = false;
                    h.trigger.filter("button").each(function () {
                        this.disabled = false
                    }).end().filter("img").css({opacity: "1.0", cursor: ""})
                } else if (p == "div" || p == "span")g.children("." + this._inlineClass).children().removeClass("ui-state-disabled");
                this._disabledInputs = c.map(this._disabledInputs, function (q) {
                    return q == a ? null : q
                })
            }
        }, _disableDatepicker: function (a) {
            var g =
                c(a), h = c.data(a, "datepicker");
            if (g.hasClass(this.markerClassName)) {
                var p = a.nodeName.toLowerCase();
                if (p == "input") {
                    a.disabled = true;
                    h.trigger.filter("button").each(function () {
                        this.disabled = true
                    }).end().filter("img").css({opacity: "0.5", cursor: "default"})
                } else if (p == "div" || p == "span")g.children("." + this._inlineClass).children().addClass("ui-state-disabled");
                this._disabledInputs = c.map(this._disabledInputs, function (q) {
                    return q == a ? null : q
                });
                this._disabledInputs[this._disabledInputs.length] = a
            }
        }, _isDisabledDatepicker: function (a) {
            if (!a)return false;
            for (var g = 0; g < this._disabledInputs.length; g++)if (this._disabledInputs[g] == a)return true;
            return false
        }, _getInst: function (a) {
            try {
                return c.data(a, "datepicker")
            } catch (g) {
                throw"Missing instance data for this datepicker";
            }
        }, _optionDatepicker: function (a, g, h) {
            var p = this._getInst(a);
            if (arguments.length == 2 && typeof g == "string")return g == "defaults" ? c.extend({}, c.datepicker._defaults) : p ? g == "all" ? c.extend({}, p.settings) : this._get(p, g) : null;
            var q = g || {};
            if (typeof g == "string") {
                q = {};
                q[g] = h
            }
            if (p) {
                this._curInst == p &&
                this._hideDatepicker();
                var o = this._getDateDatepicker(a, true);
                e(p.settings, q);
                this._attachments(c(a), p);
                this._autoSize(p);
                this._setDateDatepicker(a, o);
                this._updateDatepicker(p)
            }
        }, _changeDatepicker: function (a, g, h) {
            this._optionDatepicker(a, g, h)
        }, _refreshDatepicker: function (a) {
            (a = this._getInst(a)) && this._updateDatepicker(a)
        }, _setDateDatepicker: function (a, g) {
            if (a = this._getInst(a)) {
                this._setDate(a, g);
                this._updateDatepicker(a);
                this._updateAlternate(a)
            }
        }, _getDateDatepicker: function (a, g) {
            (a = this._getInst(a)) && !a.inline && this._setDateFromField(a, g);
            return a ? this._getDate(a) : null
        }, _doKeyDown: function (a) {
            var g = c.datepicker._getInst(a.target), h = true, p = g.dpDiv.is(".ui-datepicker-rtl");
            g._keyEvent = true;
            if (c.datepicker._datepickerShowing)switch (a.keyCode) {
                case 9:
                    c.datepicker._hideDatepicker();
                    h = false;
                    break;
                case 13:
                    h = c("td." + c.datepicker._dayOverClass, g.dpDiv).add(c("td." + c.datepicker._currentClass, g.dpDiv));
                    h[0] ? c.datepicker._selectDay(a.target, g.selectedMonth, g.selectedYear, h[0]) : c.datepicker._hideDatepicker();
                    return false;
                case 27:
                    c.datepicker._hideDatepicker();
                    break;
                case 33:
                    c.datepicker._adjustDate(a.target, a.ctrlKey ? -c.datepicker._get(g, "stepBigMonths") : -c.datepicker._get(g, "stepMonths"), "M");
                    break;
                case 34:
                    c.datepicker._adjustDate(a.target, a.ctrlKey ? +c.datepicker._get(g, "stepBigMonths") : +c.datepicker._get(g, "stepMonths"), "M");
                    break;
                case 35:
                    if (a.ctrlKey || a.metaKey)c.datepicker._clearDate(a.target);
                    h = a.ctrlKey || a.metaKey;
                    break;
                case 36:
                    if (a.ctrlKey || a.metaKey)c.datepicker._gotoToday(a.target);
                    h = a.ctrlKey ||
                        a.metaKey;
                    break;
                case 37:
                    if (a.ctrlKey || a.metaKey)c.datepicker._adjustDate(a.target, p ? +1 : -1, "D");
                    h = a.ctrlKey || a.metaKey;
                    if (a.originalEvent.altKey)c.datepicker._adjustDate(a.target, a.ctrlKey ? -c.datepicker._get(g, "stepBigMonths") : -c.datepicker._get(g, "stepMonths"), "M");
                    break;
                case 38:
                    if (a.ctrlKey || a.metaKey)c.datepicker._adjustDate(a.target, -7, "D");
                    h = a.ctrlKey || a.metaKey;
                    break;
                case 39:
                    if (a.ctrlKey || a.metaKey)c.datepicker._adjustDate(a.target, p ? -1 : +1, "D");
                    h = a.ctrlKey || a.metaKey;
                    if (a.originalEvent.altKey)c.datepicker._adjustDate(a.target,
                        a.ctrlKey ? +c.datepicker._get(g, "stepBigMonths") : +c.datepicker._get(g, "stepMonths"), "M");
                    break;
                case 40:
                    if (a.ctrlKey || a.metaKey)c.datepicker._adjustDate(a.target, +7, "D");
                    h = a.ctrlKey || a.metaKey;
                    break;
                default:
                    h = false
            } else if (a.keyCode == 36 && a.ctrlKey)c.datepicker._showDatepicker(this); else h = false;
            if (h) {
                a.preventDefault();
                a.stopPropagation()
            }
        }, _doKeyPress: function (a) {
            var g = c.datepicker._getInst(a.target);
            if (c.datepicker._get(g, "constrainInput")) {
                g = c.datepicker._possibleChars(c.datepicker._get(g, "dateFormat"));
                var h = String.fromCharCode(a.charCode == undefined ? a.keyCode : a.charCode);
                return a.ctrlKey || h < " " || !g || g.indexOf(h) > -1
            }
        }, _doKeyUp: function (a) {
            a = c.datepicker._getInst(a.target);
            if (a.input.val() != a.lastVal)try {
                if (c.datepicker.parseDate(c.datepicker._get(a, "dateFormat"), a.input ? a.input.val() : null, c.datepicker._getFormatConfig(a))) {
                    c.datepicker._setDateFromField(a);
                    c.datepicker._updateAlternate(a);
                    c.datepicker._updateDatepicker(a)
                }
            } catch (g) {
                c.datepicker.log(g)
            }
            return true
        }, _showDatepicker: function (a) {
            a = a.target ||
                a;
            if (a.nodeName.toLowerCase() != "input")a = c("input", a.parentNode)[0];
            if (!(c.datepicker._isDisabledDatepicker(a) || c.datepicker._lastInput == a)) {
                var g = c.datepicker._getInst(a);
                c.datepicker._curInst && c.datepicker._curInst != g && c.datepicker._curInst.dpDiv.stop(true, true);
                var h = c.datepicker._get(g, "beforeShow");
                e(g.settings, h ? h.apply(a, [a, g]) : {});
                g.lastVal = null;
                c.datepicker._lastInput = a;
                c.datepicker._setDateFromField(g);
                if (c.datepicker._inDialog)a.value = "";
                if (!c.datepicker._pos) {
                    c.datepicker._pos = c.datepicker._findPos(a);
                    c.datepicker._pos[1] += a.offsetHeight
                }
                var p = false;
                c(a).parents().each(function () {
                    p |= c(this).css("position") == "fixed";
                    return!p
                });
                if (p && c.browser.opera) {
                    c.datepicker._pos[0] -= document.documentElement.scrollLeft;
                    c.datepicker._pos[1] -= document.documentElement.scrollTop
                }
                h = {left: c.datepicker._pos[0], top: c.datepicker._pos[1]};
                c.datepicker._pos = null;
                g.dpDiv.css({position: "absolute", display: "block", top: "-1000px"});
                c.datepicker._updateDatepicker(g);
                h = c.datepicker._checkOffset(g, h, p);
                g.dpDiv.css({position: c.datepicker._inDialog &&
                    c.blockUI ? "static" : p ? "fixed" : "absolute", display: "none", left: h.left + "px", top: h.top + "px"});
                if (!g.inline) {
                    h = c.datepicker._get(g, "showAnim");
                    var q = c.datepicker._get(g, "duration"), o = function () {
                        c.datepicker._datepickerShowing = true;
                        var l = c.datepicker._getBorders(g.dpDiv);
                        g.dpDiv.find("iframe.ui-datepicker-cover").css({left: -l[0], top: -l[1], width: g.dpDiv.outerWidth(), height: g.dpDiv.outerHeight()})
                    };
                    g.dpDiv.zIndex(c(a).zIndex() + 1);
                    c.effects && c.effects[h] ? g.dpDiv.show(h, c.datepicker._get(g, "showOptions"), q,
                        o) : g.dpDiv[h || "show"](h ? q : null, o);
                    if (!h || !q)o();
                    g.input.is(":visible") && !g.input.is(":disabled") && g.input.focus();
                    c.datepicker._curInst = g
                }
            }
        }, _updateDatepicker: function (a) {
            var g = this, h = c.datepicker._getBorders(a.dpDiv);
            a.dpDiv.empty().append(this._generateHTML(a)).find("iframe.ui-datepicker-cover").css({left: -h[0], top: -h[1], width: a.dpDiv.outerWidth(), height: a.dpDiv.outerHeight()}).end().find("button, .ui-datepicker-prev, .ui-datepicker-next, .ui-datepicker-calendar td a").bind("mouseout", function () {
                c(this).removeClass("ui-state-hover");
                this.className.indexOf("ui-datepicker-prev") != -1 && c(this).removeClass("ui-datepicker-prev-hover");
                this.className.indexOf("ui-datepicker-next") != -1 && c(this).removeClass("ui-datepicker-next-hover")
            }).bind("mouseover", function () {
                if (!g._isDisabledDatepicker(a.inline ? a.dpDiv.parent()[0] : a.input[0])) {
                    c(this).parents(".ui-datepicker-calendar").find("a").removeClass("ui-state-hover");
                    c(this).addClass("ui-state-hover");
                    this.className.indexOf("ui-datepicker-prev") != -1 && c(this).addClass("ui-datepicker-prev-hover");
                    this.className.indexOf("ui-datepicker-next") != -1 && c(this).addClass("ui-datepicker-next-hover")
                }
            }).end().find("." + this._dayOverClass + " a").trigger("mouseover").end();
            h = this._getNumberOfMonths(a);
            var p = h[1];
            p > 1 ? a.dpDiv.addClass("ui-datepicker-multi-" + p).css("width", 17 * p + "em") : a.dpDiv.removeClass("ui-datepicker-multi-2 ui-datepicker-multi-3 ui-datepicker-multi-4").width("");
            a.dpDiv[(h[0] != 1 || h[1] != 1 ? "add" : "remove") + "Class"]("ui-datepicker-multi");
            a.dpDiv[(this._get(a, "isRTL") ? "add" : "remove") + "Class"]("ui-datepicker-rtl");
            a == c.datepicker._curInst && c.datepicker._datepickerShowing && a.input && a.input.is(":visible") && !a.input.is(":disabled") && a.input.focus()
        }, _getBorders: function (a) {
            var g = function (h) {
                return{thin: 1, medium: 2, thick: 3}[h] || h
            };
            return[parseFloat(g(a.css("border-left-width"))), parseFloat(g(a.css("border-top-width")))]
        }, _checkOffset: function (a, g, h) {
            var p = a.dpDiv.outerWidth(), q = a.dpDiv.outerHeight(), o = a.input ? a.input.outerWidth() : 0, l = a.input ? a.input.outerHeight() : 0, r = document.documentElement.clientWidth + c(document).scrollLeft(),
                u = document.documentElement.clientHeight + c(document).scrollTop();
            g.left -= this._get(a, "isRTL") ? p - o : 0;
            g.left -= h && g.left == a.input.offset().left ? c(document).scrollLeft() : 0;
            g.top -= h && g.top == a.input.offset().top + l ? c(document).scrollTop() : 0;
            g.left -= Math.min(g.left, g.left + p > r && r > p ? Math.abs(g.left + p - r) : 0);
            g.top -= Math.min(g.top, g.top + q > u && u > q ? Math.abs(q + l) : 0);
            return g
        }, _findPos: function (a) {
            for (var g = this._get(this._getInst(a), "isRTL"); a && (a.type == "hidden" || a.nodeType != 1);)a = a[g ? "previousSibling" : "nextSibling"];
            a = c(a).offset();
            return[a.left, a.top]
        }, _hideDatepicker: function (a) {
            var g = this._curInst;
            if (!(!g || a && g != c.data(a, "datepicker")))if (this._datepickerShowing) {
                a = this._get(g, "showAnim");
                var h = this._get(g, "duration"), p = function () {
                    c.datepicker._tidyDialog(g);
                    this._curInst = null
                };
                c.effects && c.effects[a] ? g.dpDiv.hide(a, c.datepicker._get(g, "showOptions"), h, p) : g.dpDiv[a == "slideDown" ? "slideUp" : a == "fadeIn" ? "fadeOut" : "hide"](a ? h : null, p);
                a || p();
                if (a = this._get(g, "onClose"))a.apply(g.input ? g.input[0] : null, [g.input ? g.input.val() :
                    "", g]);
                this._datepickerShowing = false;
                this._lastInput = null;
                if (this._inDialog) {
                    this._dialogInput.css({position: "absolute", left: "0", top: "-100px"});
                    if (c.blockUI) {
                        c.unblockUI();
                        c("body").append(this.dpDiv)
                    }
                }
                this._inDialog = false
            }
        }, _tidyDialog: function (a) {
            a.dpDiv.removeClass(this._dialogClass).unbind(".ui-datepicker-calendar")
        }, _checkExternalClick: function (a) {
            if (c.datepicker._curInst) {
                a = c(a.target);
                a[0].id != c.datepicker._mainDivId && a.parents("#" + c.datepicker._mainDivId).length == 0 && !a.hasClass(c.datepicker.markerClassName) && !a.hasClass(c.datepicker._triggerClass) && c.datepicker._datepickerShowing && !(c.datepicker._inDialog && c.blockUI) && c.datepicker._hideDatepicker()
            }
        }, _adjustDate: function (a, g, h) {
            a = c(a);
            var p = this._getInst(a[0]);
            if (!this._isDisabledDatepicker(a[0])) {
                this._adjustInstDate(p, g + (h == "M" ? this._get(p, "showCurrentAtPos") : 0), h);
                this._updateDatepicker(p)
            }
        }, _gotoToday: function (a) {
            a = c(a);
            var g = this._getInst(a[0]);
            if (this._get(g, "gotoCurrent") && g.currentDay) {
                g.selectedDay = g.currentDay;
                g.drawMonth = g.selectedMonth = g.currentMonth;
                g.drawYear = g.selectedYear = g.currentYear
            } else {
                var h = new Date;
                g.selectedDay = h.getDate();
                g.drawMonth = g.selectedMonth = h.getMonth();
                g.drawYear = g.selectedYear = h.getFullYear()
            }
            this._notifyChange(g);
            this._adjustDate(a)
        }, _selectMonthYear: function (a, g, h) {
            a = c(a);
            var p = this._getInst(a[0]);
            p._selectingMonthYear = false;
            p["selected" + (h == "M" ? "Month" : "Year")] = p["draw" + (h == "M" ? "Month" : "Year")] = parseInt(g.options[g.selectedIndex].value, 10);
            this._notifyChange(p);
            this._adjustDate(a)
        }, _clickMonthYear: function (a) {
            a = this._getInst(c(a)[0]);
            a.input && a._selectingMonthYear && !c.browser.msie && a.input.focus();
            a._selectingMonthYear = !a._selectingMonthYear
        }, _selectDay: function (a, g, h, p) {
            var q = c(a);
            if (!(c(p).hasClass(this._unselectableClass) || this._isDisabledDatepicker(q[0]))) {
                q = this._getInst(q[0]);
                q.selectedDay = q.currentDay = c("a", p).html();
                q.selectedMonth = q.currentMonth = g;
                q.selectedYear = q.currentYear = h;
                this._selectDate(a, this._formatDate(q, q.currentDay, q.currentMonth, q.currentYear))
            }
        }, _clearDate: function (a) {
            a = c(a);
            this._getInst(a[0]);
            this._selectDate(a,
                "")
        }, _selectDate: function (a, g) {
            a = this._getInst(c(a)[0]);
            g = g != null ? g : this._formatDate(a);
            a.input && a.input.val(g);
            this._updateAlternate(a);
            var h = this._get(a, "onSelect");
            if (h)h.apply(a.input ? a.input[0] : null, [g, a]); else a.input && a.input.trigger("change");
            if (a.inline)this._updateDatepicker(a); else {
                this._hideDatepicker();
                this._lastInput = a.input[0];
                typeof a.input[0] != "object" && a.input.focus();
                this._lastInput = null
            }
        }, _updateAlternate: function (a) {
            var g = this._get(a, "altField");
            if (g) {
                var h = this._get(a, "altFormat") ||
                    this._get(a, "dateFormat"), p = this._getDate(a), q = this.formatDate(h, p, this._getFormatConfig(a));
                c(g).each(function () {
                    c(this).val(q)
                })
            }
        }, noWeekends: function (a) {
            a = a.getDay();
            return[a > 0 && a < 6, ""]
        }, iso8601Week: function (a) {
            a = new Date(a.getTime());
            a.setDate(a.getDate() + 4 - (a.getDay() || 7));
            var g = a.getTime();
            a.setMonth(0);
            a.setDate(1);
            return Math.floor(Math.round((g - a) / 864E5) / 7) + 1
        }, parseDate: function (a, g, h) {
            if (a == null || g == null)throw"Invalid arguments";
            g = typeof g == "object" ? g.toString() : g + "";
            if (g == "")return null;
            for (var p = (h ? h.shortYearCutoff : null) || this._defaults.shortYearCutoff, q = (h ? h.dayNamesShort : null) || this._defaults.dayNamesShort, o = (h ? h.dayNames : null) || this._defaults.dayNames, l = (h ? h.monthNamesShort : null) || this._defaults.monthNamesShort, r = (h ? h.monthNames : null) || this._defaults.monthNames, u = h = -1, y = -1, A = -1, i = false, J = function (S) {
                (S = ea + 1 < a.length && a.charAt(ea + 1) == S) && ea++;
                return S
            }, K = function (S) {
                J(S);
                S = RegExp("^\\d{1," + (S == "@" ? 14 : S == "!" ? 20 : S == "y" ? 4 : S == "o" ? 3 : 2) + "}");
                S = g.substring(X).match(S);
                if (!S)throw"Missing number at position " +
                    X;
                X += S[0].length;
                return parseInt(S[0], 10)
            }, D = function (S, $, ga) {
                S = J(S) ? ga : $;
                for ($ = 0; $ < S.length; $++)if (g.substr(X, S[$].length) == S[$]) {
                    X += S[$].length;
                    return $ + 1
                }
                throw"Unknown name at position " + X;
            }, P = function () {
                if (g.charAt(X) != a.charAt(ea))throw"Unexpected literal at position " + X;
                X++
            }, X = 0, ea = 0; ea < a.length; ea++)if (i)if (a.charAt(ea) == "'" && !J("'"))i = false; else P(); else switch (a.charAt(ea)) {
                case "d":
                    y = K("d");
                    break;
                case "D":
                    D("D", q, o);
                    break;
                case "o":
                    A = K("o");
                    break;
                case "m":
                    u = K("m");
                    break;
                case "M":
                    u = D("M",
                        l, r);
                    break;
                case "y":
                    h = K("y");
                    break;
                case "@":
                    var Y = new Date(K("@"));
                    h = Y.getFullYear();
                    u = Y.getMonth() + 1;
                    y = Y.getDate();
                    break;
                case "!":
                    Y = new Date((K("!") - this._ticksTo1970) / 1E4);
                    h = Y.getFullYear();
                    u = Y.getMonth() + 1;
                    y = Y.getDate();
                    break;
                case "'":
                    if (J("'"))P(); else i = true;
                    break;
                default:
                    P()
            }
            if (h == -1)h = (new Date).getFullYear(); else if (h < 100)h += (new Date).getFullYear() - (new Date).getFullYear() % 100 + (h <= p ? 0 : -100);
            if (A > -1) {
                u = 1;
                y = A;
                do {
                    p = this._getDaysInMonth(h, u - 1);
                    if (y <= p)break;
                    u++;
                    y -= p
                } while (1)
            }
            Y = this._daylightSavingAdjust(new Date(h,
                    u - 1, y));
            if (Y.getFullYear() != h || Y.getMonth() + 1 != u || Y.getDate() != y)throw"Invalid date";
            return Y
        }, ATOM: "yy-mm-dd", COOKIE: "D, dd M yy", ISO_8601: "yy-mm-dd", RFC_822: "D, d M y", RFC_850: "DD, dd-M-y", RFC_1036: "D, d M y", RFC_1123: "D, d M yy", RFC_2822: "D, d M yy", RSS: "D, d M y", TICKS: "!", TIMESTAMP: "@", W3C: "yy-mm-dd", _ticksTo1970: (718685 + Math.floor(492.5) - Math.floor(19.7) + Math.floor(4.925)) * 24 * 60 * 60 * 1E7, formatDate: function (a, g, h) {
            if (!g)return"";
            var p = (h ? h.dayNamesShort : null) || this._defaults.dayNamesShort, q = (h ?
                h.dayNames : null) || this._defaults.dayNames, o = (h ? h.monthNamesShort : null) || this._defaults.monthNamesShort;
            h = (h ? h.monthNames : null) || this._defaults.monthNames;
            var l = function (J) {
                (J = i + 1 < a.length && a.charAt(i + 1) == J) && i++;
                return J
            }, r = function (J, K, D) {
                K = "" + K;
                if (l(J))for (; K.length < D;)K = "0" + K;
                return K
            }, u = function (J, K, D, P) {
                return l(J) ? P[K] : D[K]
            }, y = "", A = false;
            if (g)for (var i = 0; i < a.length; i++)if (A)if (a.charAt(i) == "'" && !l("'"))A = false; else y += a.charAt(i); else switch (a.charAt(i)) {
                case "d":
                    y += r("d", g.getDate(), 2);
                    break;
                case "D":
                    y += u("D", g.getDay(), p, q);
                    break;
                case "o":
                    y += r("o", (g.getTime() - (new Date(g.getFullYear(), 0, 0)).getTime()) / 864E5, 3);
                    break;
                case "m":
                    y += r("m", g.getMonth() + 1, 2);
                    break;
                case "M":
                    y += u("M", g.getMonth(), o, h);
                    break;
                case "y":
                    y += l("y") ? g.getFullYear() : (g.getYear() % 100 < 10 ? "0" : "") + g.getYear() % 100;
                    break;
                case "@":
                    y += g.getTime();
                    break;
                case "!":
                    y += g.getTime() * 1E4 + this._ticksTo1970;
                    break;
                case "'":
                    if (l("'"))y += "'"; else A = true;
                    break;
                default:
                    y += a.charAt(i)
            }
            return y
        }, _possibleChars: function (a) {
            for (var g = "", h = false,
                     p = function (o) {
                         (o = q + 1 < a.length && a.charAt(q + 1) == o) && q++;
                         return o
                     }, q = 0; q < a.length; q++)if (h)if (a.charAt(q) == "'" && !p("'"))h = false; else g += a.charAt(q); else switch (a.charAt(q)) {
                case "d":
                case "m":
                case "y":
                case "@":
                    g += "0123456789";
                    break;
                case "D":
                case "M":
                    return null;
                case "'":
                    if (p("'"))g += "'"; else h = true;
                    break;
                default:
                    g += a.charAt(q)
            }
            return g
        }, _get: function (a, g) {
            return a.settings[g] !== undefined ? a.settings[g] : this._defaults[g]
        }, _setDateFromField: function (a, g) {
            if (a.input.val() != a.lastVal) {
                var h = this._get(a, "dateFormat"),
                    p = a.lastVal = a.input ? a.input.val() : null, q, o;
                q = o = this._getDefaultDate(a);
                var l = this._getFormatConfig(a);
                try {
                    q = this.parseDate(h, p, l) || o
                } catch (r) {
                    this.log(r);
                    p = g ? "" : p
                }
                a.selectedDay = q.getDate();
                a.drawMonth = a.selectedMonth = q.getMonth();
                a.drawYear = a.selectedYear = q.getFullYear();
                a.currentDay = p ? q.getDate() : 0;
                a.currentMonth = p ? q.getMonth() : 0;
                a.currentYear = p ? q.getFullYear() : 0;
                this._adjustInstDate(a)
            }
        }, _getDefaultDate: function (a) {
            return this._restrictMinMax(a, this._determineDate(a, this._get(a, "defaultDate"), new Date))
        },
        _determineDate: function (a, g, h) {
            var p = function (o) {
                var l = new Date;
                l.setDate(l.getDate() + o);
                return l
            }, q = function (o) {
                try {
                    return c.datepicker.parseDate(c.datepicker._get(a, "dateFormat"), o, c.datepicker._getFormatConfig(a))
                } catch (l) {
                }
                var r = (o.toLowerCase().match(/^c/) ? c.datepicker._getDate(a) : null) || new Date, u = r.getFullYear(), y = r.getMonth();
                r = r.getDate();
                for (var A = /([+-]?[0-9]+)\s*(d|D|w|W|m|M|y|Y)?/g, i = A.exec(o); i;) {
                    switch (i[2] || "d") {
                        case "d":
                        case "D":
                            r += parseInt(i[1], 10);
                            break;
                        case "w":
                        case "W":
                            r += parseInt(i[1],
                                10) * 7;
                            break;
                        case "m":
                        case "M":
                            y += parseInt(i[1], 10);
                            r = Math.min(r, c.datepicker._getDaysInMonth(u, y));
                            break;
                        case "y":
                        case "Y":
                            u += parseInt(i[1], 10);
                            r = Math.min(r, c.datepicker._getDaysInMonth(u, y));
                            break
                    }
                    i = A.exec(o)
                }
                return new Date(u, y, r)
            };
            if (g = (g = g == null ? h : typeof g == "string" ? q(g) : typeof g == "number" ? isNaN(g) ? h : p(g) : g) && g.toString() == "Invalid Date" ? h : g) {
                g.setHours(0);
                g.setMinutes(0);
                g.setSeconds(0);
                g.setMilliseconds(0)
            }
            return this._daylightSavingAdjust(g)
        }, _daylightSavingAdjust: function (a) {
            if (!a)return null;
            a.setHours(a.getHours() > 12 ? a.getHours() + 2 : 0);
            return a
        }, _setDate: function (a, g, h) {
            var p = !g, q = a.selectedMonth, o = a.selectedYear;
            g = this._restrictMinMax(a, this._determineDate(a, g, new Date));
            a.selectedDay = a.currentDay = g.getDate();
            a.drawMonth = a.selectedMonth = a.currentMonth = g.getMonth();
            a.drawYear = a.selectedYear = a.currentYear = g.getFullYear();
            if ((q != a.selectedMonth || o != a.selectedYear) && !h)this._notifyChange(a);
            this._adjustInstDate(a);
            if (a.input)a.input.val(p ? "" : this._formatDate(a))
        }, _getDate: function (a) {
            return!a.currentYear ||
                a.input && a.input.val() == "" ? null : this._daylightSavingAdjust(new Date(a.currentYear, a.currentMonth, a.currentDay))
        }, _generateHTML: function (a) {
            var g = new Date;
            g = this._daylightSavingAdjust(new Date(g.getFullYear(), g.getMonth(), g.getDate()));
            var h = this._get(a, "isRTL"), p = this._get(a, "showButtonPanel"), q = this._get(a, "hideIfNoPrevNext"), o = this._get(a, "navigationAsDateFormat"), l = this._getNumberOfMonths(a), r = this._get(a, "showCurrentAtPos"), u = this._get(a, "stepMonths"), y = l[0] != 1 || l[1] != 1, A = this._daylightSavingAdjust(!a.currentDay ?
                new Date(9999, 9, 9) : new Date(a.currentYear, a.currentMonth, a.currentDay)), i = this._getMinMaxDate(a, "min"), J = this._getMinMaxDate(a, "max");
            r = a.drawMonth - r;
            var K = a.drawYear;
            if (r < 0) {
                r += 12;
                K--
            }
            if (J) {
                var D = this._daylightSavingAdjust(new Date(J.getFullYear(), J.getMonth() - l[0] * l[1] + 1, J.getDate()));
                for (D = i && D < i ? i : D; this._daylightSavingAdjust(new Date(K, r, 1)) > D;) {
                    r--;
                    if (r < 0) {
                        r = 11;
                        K--
                    }
                }
            }
            a.drawMonth = r;
            a.drawYear = K;
            D = this._get(a, "prevText");
            D = !o ? D : this.formatDate(D, this._daylightSavingAdjust(new Date(K, r - u, 1)), this._getFormatConfig(a));
            D = this._canAdjustMonth(a, -1, K, r) ? '<a class="ui-datepicker-prev ui-corner-all" onclick="DP_jQuery_' + f + ".datepicker._adjustDate('#" + a.id + "', -" + u + ", 'M');\" title=\"" + D + '"><span class="ui-icon ui-icon-circle-triangle-' + (h ? "e" : "w") + '">' + D + "</span></a>" : q ? "" : '<a class="ui-datepicker-prev ui-corner-all ui-state-disabled" title="' + D + '"><span class="ui-icon ui-icon-circle-triangle-' + (h ? "e" : "w") + '">' + D + "</span></a>";
            var P = this._get(a, "nextText");
            P = !o ? P : this.formatDate(P, this._daylightSavingAdjust(new Date(K,
                    r + u, 1)), this._getFormatConfig(a));
            q = this._canAdjustMonth(a, +1, K, r) ? '<a class="ui-datepicker-next ui-corner-all" onclick="DP_jQuery_' + f + ".datepicker._adjustDate('#" + a.id + "', +" + u + ", 'M');\" title=\"" + P + '"><span class="ui-icon ui-icon-circle-triangle-' + (h ? "w" : "e") + '">' + P + "</span></a>" : q ? "" : '<a class="ui-datepicker-next ui-corner-all ui-state-disabled" title="' + P + '"><span class="ui-icon ui-icon-circle-triangle-' + (h ? "w" : "e") + '">' + P + "</span></a>";
            u = this._get(a, "currentText");
            P = this._get(a, "gotoCurrent") &&
                a.currentDay ? A : g;
            u = !o ? u : this.formatDate(u, P, this._getFormatConfig(a));
            o = !a.inline ? '<button type="button" class="ui-datepicker-close ui-state-default ui-priority-primary ui-corner-all" onclick="DP_jQuery_' + f + '.datepicker._hideDatepicker();">' + this._get(a, "closeText") + "</button>" : "";
            p = p ? '<div class="ui-datepicker-buttonpane ui-widget-content">' + (h ? o : "") + (this._isInRange(a, P) ? '<button type="button" class="ui-datepicker-current ui-state-default ui-priority-secondary ui-corner-all" onclick="DP_jQuery_' +
                f + ".datepicker._gotoToday('#" + a.id + "');\">" + u + "</button>" : "") + (h ? "" : o) + "</div>" : "";
            o = parseInt(this._get(a, "firstDay"), 10);
            o = isNaN(o) ? 0 : o;
            u = this._get(a, "showWeek");
            P = this._get(a, "dayNames");
            this._get(a, "dayNamesShort");
            var X = this._get(a, "dayNamesMin"), ea = this._get(a, "monthNames"), Y = this._get(a, "monthNamesShort"), S = this._get(a, "beforeShowDay"), $ = this._get(a, "showOtherMonths"), ga = this._get(a, "selectOtherMonths");
            this._get(a, "calculateWeek");
            for (var sa = this._getDefaultDate(a), ha = "", fa = 0; fa < l[0]; fa++) {
                for (var na =
                    "", ia = 0; ia < l[1]; ia++) {
                    var oa = this._daylightSavingAdjust(new Date(K, r, a.selectedDay)), U = " ui-corner-all", aa = "";
                    if (y) {
                        aa += '<div class="ui-datepicker-group';
                        if (l[1] > 1)switch (ia) {
                            case 0:
                                aa += " ui-datepicker-group-first";
                                U = " ui-corner-" + (h ? "right" : "left");
                                break;
                            case l[1] - 1:
                                aa += " ui-datepicker-group-last";
                                U = " ui-corner-" + (h ? "left" : "right");
                                break;
                            default:
                                aa += " ui-datepicker-group-middle";
                                U = "";
                                break
                        }
                        aa += '">'
                    }
                    aa += '<div class="ui-datepicker-header ui-widget-header ui-helper-clearfix' + U + '">' + (/all|left/.test(U) &&
                        fa == 0 ? h ? q : D : "") + (/all|right/.test(U) && fa == 0 ? h ? D : q : "") + this._generateMonthYearHeader(a, r, K, i, J, fa > 0 || ia > 0, ea, Y) + '</div><table class="ui-datepicker-calendar"><thead><tr>';
                    var Z = u ? '<th class="ui-datepicker-week-col">' + this._get(a, "weekHeader") + "</th>" : "";
                    for (U = 0; U < 7; U++) {
                        var V = (U + o) % 7;
                        Z += "<th" + ((U + o + 6) % 7 >= 5 ? ' class="ui-datepicker-week-end"' : "") + '><span title="' + P[V] + '">' + X[V] + "</span></th>"
                    }
                    aa += Z + "</tr></thead><tbody>";
                    Z = this._getDaysInMonth(K, r);
                    if (K == a.selectedYear && r == a.selectedMonth)a.selectedDay =
                        Math.min(a.selectedDay, Z);
                    U = (this._getFirstDayOfMonth(K, r) - o + 7) % 7;
                    Z = y ? 6 : Math.ceil((U + Z) / 7);
                    V = this._daylightSavingAdjust(new Date(K, r, 1 - U));
                    for (var ta = 0; ta < Z; ta++) {
                        aa += "<tr>";
                        var ua = !u ? "" : '<td class="ui-datepicker-week-col">' + this._get(a, "calculateWeek")(V) + "</td>";
                        for (U = 0; U < 7; U++) {
                            var ja = S ? S.apply(a.input ? a.input[0] : null, [V]) : [true, ""], ka = V.getMonth() != r, va = ka && !ga || !ja[0] || i && V < i || J && V > J;
                            ua += '<td class="' + ((U + o + 6) % 7 >= 5 ? " ui-datepicker-week-end" : "") + (ka ? " ui-datepicker-other-month" : "") + (V.getTime() ==
                                oa.getTime() && r == a.selectedMonth && a._keyEvent || sa.getTime() == V.getTime() && sa.getTime() == oa.getTime() ? " " + this._dayOverClass : "") + (va ? " " + this._unselectableClass + " ui-state-disabled" : "") + (ka && !$ ? "" : " " + ja[1] + (V.getTime() == A.getTime() ? " " + this._currentClass : "") + (V.getTime() == g.getTime() ? " ui-datepicker-today" : "")) + '"' + ((!ka || $) && ja[2] ? ' title="' + ja[2] + '"' : "") + (va ? "" : ' onclick="DP_jQuery_' + f + ".datepicker._selectDay('#" + a.id + "'," + V.getMonth() + "," + V.getFullYear() + ', this);return false;"') + ">" + (ka && !$ ? "&#xa0;" :
                                va ? '<span class="ui-state-default">' + V.getDate() + "</span>" : '<a class="ui-state-default' + (V.getTime() == g.getTime() ? " ui-state-highlight" : "") + (V.getTime() == A.getTime() ? " ui-state-active" : "") + (ka ? " ui-priority-secondary" : "") + '" href="#">' + V.getDate() + "</a>") + "</td>";
                            V.setDate(V.getDate() + 1);
                            V = this._daylightSavingAdjust(V)
                        }
                        aa += ua + "</tr>"
                    }
                    r++;
                    if (r > 11) {
                        r = 0;
                        K++
                    }
                    aa += "</tbody></table>" + (y ? "</div>" + (l[0] > 0 && ia == l[1] - 1 ? '<div class="ui-datepicker-row-break"></div>' : "") : "");
                    na += aa
                }
                ha += na
            }
            ha += p + (c.browser.msie &&
                parseInt(c.browser.version, 10) < 7 && !a.inline ? '<iframe src="javascript:false;" class="ui-datepicker-cover" frameborder="0"></iframe>' : "");
            a._keyEvent = false;
            return ha
        }, _generateMonthYearHeader: function (a, g, h, p, q, o, l, r) {
            var u = this._get(a, "changeMonth"), y = this._get(a, "changeYear"), A = this._get(a, "showMonthAfterYear"), i = '<div class="ui-datepicker-title">', J = "";
            if (o || !u)J += '<span class="ui-datepicker-month">' + l[g] + "</span>"; else {
                l = p && p.getFullYear() == h;
                var K = q && q.getFullYear() == h;
                J += '<select class="ui-datepicker-month" onchange="DP_jQuery_' +
                    f + ".datepicker._selectMonthYear('#" + a.id + "', this, 'M');\" onclick=\"DP_jQuery_" + f + ".datepicker._clickMonthYear('#" + a.id + "');\">";
                for (var D = 0; D < 12; D++)if ((!l || D >= p.getMonth()) && (!K || D <= q.getMonth()))J += '<option value="' + D + '"' + (D == g ? ' selected="selected"' : "") + ">" + r[D] + "</option>";
                J += "</select>"
            }
            A || (i += J + (o || !(u && y) ? "&#xa0;" : ""));
            if (o || !y)i += '<span class="ui-datepicker-year">' + h + "</span>"; else {
                r = this._get(a, "yearRange").split(":");
                var P = (new Date).getFullYear();
                l = function (X) {
                    X = X.match(/c[+-].*/) ? h +
                        parseInt(X.substring(1), 10) : X.match(/[+-].*/) ? P + parseInt(X, 10) : parseInt(X, 10);
                    return isNaN(X) ? P : X
                };
                g = l(r[0]);
                r = Math.max(g, l(r[1] || ""));
                g = p ? Math.max(g, p.getFullYear()) : g;
                r = q ? Math.min(r, q.getFullYear()) : r;
                for (i += '<select class="ui-datepicker-year" onchange="DP_jQuery_' + f + ".datepicker._selectMonthYear('#" + a.id + "', this, 'Y');\" onclick=\"DP_jQuery_" + f + ".datepicker._clickMonthYear('#" + a.id + "');\">"; g <= r; g++)i += '<option value="' + g + '"' + (g == h ? ' selected="selected"' : "") + ">" + g + "</option>";
                i += "</select>"
            }
            i +=
                this._get(a, "yearSuffix");
            if (A)i += (o || !(u && y) ? "&#xa0;" : "") + J;
            i += "</div>";
            return i
        }, _adjustInstDate: function (a, g, h) {
            var p = a.drawYear + (h == "Y" ? g : 0), q = a.drawMonth + (h == "M" ? g : 0);
            g = Math.min(a.selectedDay, this._getDaysInMonth(p, q)) + (h == "D" ? g : 0);
            p = this._restrictMinMax(a, this._daylightSavingAdjust(new Date(p, q, g)));
            a.selectedDay = p.getDate();
            a.drawMonth = a.selectedMonth = p.getMonth();
            a.drawYear = a.selectedYear = p.getFullYear();
            if (h == "M" || h == "Y")this._notifyChange(a)
        }, _restrictMinMax: function (a, g) {
            var h = this._getMinMaxDate(a,
                "min");
            a = this._getMinMaxDate(a, "max");
            g = h && g < h ? h : g;
            return a && g > a ? a : g
        }, _notifyChange: function (a) {
            var g = this._get(a, "onChangeMonthYear");
            if (g)g.apply(a.input ? a.input[0] : null, [a.selectedYear, a.selectedMonth + 1, a])
        }, _getNumberOfMonths: function (a) {
            a = this._get(a, "numberOfMonths");
            return a == null ? [1, 1] : typeof a == "number" ? [1, a] : a
        }, _getMinMaxDate: function (a, g) {
            return this._determineDate(a, this._get(a, g + "Date"), null)
        }, _getDaysInMonth: function (a, g) {
            return 32 - (new Date(a, g, 32)).getDate()
        }, _getFirstDayOfMonth: function (a, g) {
            return(new Date(a, g, 1)).getDay()
        }, _canAdjustMonth: function (a, g, h, p) {
            var q = this._getNumberOfMonths(a);
            h = this._daylightSavingAdjust(new Date(h, p + (g < 0 ? g : q[0] * q[1]), 1));
            g < 0 && h.setDate(this._getDaysInMonth(h.getFullYear(), h.getMonth()));
            return this._isInRange(a, h)
        }, _isInRange: function (a, g) {
            var h = this._getMinMaxDate(a, "min");
            a = this._getMinMaxDate(a, "max");
            return(!h || g.getTime() >= h.getTime()) && (!a || g.getTime() <= a.getTime())
        }, _getFormatConfig: function (a) {
            var g = this._get(a, "shortYearCutoff");
            g = typeof g !=
                "string" ? g : (new Date).getFullYear() % 100 + parseInt(g, 10);
            return{shortYearCutoff: g, dayNamesShort: this._get(a, "dayNamesShort"), dayNames: this._get(a, "dayNames"), monthNamesShort: this._get(a, "monthNamesShort"), monthNames: this._get(a, "monthNames")}
        }, _formatDate: function (a, g, h, p) {
            if (!g) {
                a.currentDay = a.selectedDay;
                a.currentMonth = a.selectedMonth;
                a.currentYear = a.selectedYear
            }
            g = g ? typeof g == "object" ? g : this._daylightSavingAdjust(new Date(p, h, g)) : this._daylightSavingAdjust(new Date(a.currentYear, a.currentMonth, a.currentDay));
            return this.formatDate(this._get(a, "dateFormat"), g, this._getFormatConfig(a))
        }});
    c.fn.datepicker = function (a) {
        if (!c.datepicker.initialized) {
            c(document).mousedown(c.datepicker._checkExternalClick).find("body").append(c.datepicker.dpDiv);
            c.datepicker.initialized = true
        }
        var g = Array.prototype.slice.call(arguments, 1);
        if (typeof a == "string" && (a == "isDisabled" || a == "getDate" || a == "widget"))return c.datepicker["_" + a + "Datepicker"].apply(c.datepicker, [this[0]].concat(g));
        if (a == "option" && arguments.length == 2 && typeof arguments[1] ==
            "string")return c.datepicker["_" + a + "Datepicker"].apply(c.datepicker, [this[0]].concat(g));
        return this.each(function () {
            typeof a == "string" ? c.datepicker["_" + a + "Datepicker"].apply(c.datepicker, [this].concat(g)) : c.datepicker._attachDatepicker(this, a)
        })
    };
    c.datepicker = new b;
    c.datepicker.initialized = false;
    c.datepicker.uuid = (new Date).getTime();
    c.datepicker.version = "1.8.2";
    window["DP_jQuery_" + f] = c
})(jQuery);
(function (c) {
    c.widget("ui.dialog", {options: {autoOpen: true, buttons: {}, closeOnEscape: true, closeText: "close", dialogClass: "", draggable: true, hide: null, height: "auto", maxHeight: false, maxWidth: false, minHeight: 150, minWidth: 150, modal: false, position: "center", resizable: true, show: null, stack: true, title: "", width: 300, zIndex: 1E3}, _create: function () {
        this.originalTitle = this.element.attr("title");
        var b = this, e = b.options, f = e.title || b.originalTitle || "&#160;", a = c.ui.dialog.getTitleId(b.element), g = (b.uiDialog = c("<div></div>")).appendTo(document.body).hide().addClass("ui-dialog ui-widget ui-widget-content ui-corner-all " +
            e.dialogClass).css({zIndex: e.zIndex}).attr("tabIndex", -1).css("outline", 0).keydown(function (q) {
            if (e.closeOnEscape && q.keyCode && q.keyCode === c.ui.keyCode.ESCAPE) {
                b.close(q);
                q.preventDefault()
            }
        }).attr({role: "dialog", "aria-labelledby": a}).mousedown(function (q) {
            b.moveToTop(false, q)
        });
        b.element.show().removeAttr("title").addClass("ui-dialog-content ui-widget-content").appendTo(g);
        var h = (b.uiDialogTitlebar = c("<div></div>")).addClass("ui-dialog-titlebar ui-widget-header ui-corner-all ui-helper-clearfix").prependTo(g),
            p = c('<a href="#"></a>').addClass("ui-dialog-titlebar-close ui-corner-all").attr("role", "button").hover(function () {
                p.addClass("ui-state-hover")
            }, function () {
                p.removeClass("ui-state-hover")
            }).focus(function () {
                p.addClass("ui-state-focus")
            }).blur(function () {
                p.removeClass("ui-state-focus")
            }).click(function (q) {
                b.close(q);
                return false
            }).appendTo(h);
        (b.uiDialogTitlebarCloseText = c("<span></span>")).addClass("ui-icon ui-icon-closethick").text(e.closeText).appendTo(p);
        c("<span></span>").addClass("ui-dialog-title").attr("id",
            a).html(f).prependTo(h);
        if (c.isFunction(e.beforeclose) && !c.isFunction(e.beforeClose))e.beforeClose = e.beforeclose;
        h.find("*").add(h).disableSelection();
        e.draggable && c.fn.draggable && b._makeDraggable();
        e.resizable && c.fn.resizable && b._makeResizable();
        b._createButtons(e.buttons);
        b._isOpen = false;
        c.fn.bgiframe && g.bgiframe()
    }, _init: function () {
        this.options.autoOpen && this.open()
    }, destroy: function () {
        this.overlay && this.overlay.destroy();
        this.uiDialog.hide();
        this.element.unbind(".dialog").removeData("dialog").removeClass("ui-dialog-content ui-widget-content").hide().appendTo("body");
        this.uiDialog.remove();
        this.originalTitle && this.element.attr("title", this.originalTitle);
        return this
    }, widget: function () {
        return this.uiDialog
    }, close: function (b) {
        var e = this, f;
        if (false !== e._trigger("beforeClose", b)) {
            e.overlay && e.overlay.destroy();
            e.uiDialog.unbind("keypress.ui-dialog");
            e._isOpen = false;
            if (e.options.hide)e.uiDialog.hide(e.options.hide, function () {
                e._trigger("close", b)
            }); else {
                e.uiDialog.hide();
                e._trigger("close", b)
            }
            c.ui.dialog.overlay.resize();
            if (e.options.modal) {
                f = 0;
                c(".ui-dialog").each(function () {
                    if (this !==
                        e.uiDialog[0])f = Math.max(f, c(this).css("z-index"))
                });
                c.ui.dialog.maxZ = f
            }
            return e
        }
    }, isOpen: function () {
        return this._isOpen
    }, moveToTop: function (b, e) {
        var f = this.options;
        if (f.modal && !b || !f.stack && !f.modal)return this._trigger("focus", e);
        if (f.zIndex > c.ui.dialog.maxZ)c.ui.dialog.maxZ = f.zIndex;
        if (this.overlay) {
            c.ui.dialog.maxZ += 1;
            this.overlay.$el.css("z-index", c.ui.dialog.overlay.maxZ = c.ui.dialog.maxZ)
        }
        b = {scrollTop: this.element.attr("scrollTop"), scrollLeft: this.element.attr("scrollLeft")};
        c.ui.dialog.maxZ +=
            1;
        this.uiDialog.css("z-index", c.ui.dialog.maxZ);
        this.element.attr(b);
        this._trigger("focus", e);
        return this
    }, open: function () {
        if (!this._isOpen) {
            var b = this.options, e = this.uiDialog;
            this.overlay = b.modal ? new c.ui.dialog.overlay(this) : null;
            e.next().length && e.appendTo("body");
            this._size();
            this._position(b.position);
            e.show(b.show);
            this.moveToTop(true);
            b.modal && e.bind("keypress.ui-dialog", function (f) {
                if (f.keyCode === c.ui.keyCode.TAB) {
                    var a = c(":tabbable", this), g = a.filter(":first");
                    a = a.filter(":last");
                    if (f.target ===
                        a[0] && !f.shiftKey) {
                        g.focus(1);
                        return false
                    } else if (f.target === g[0] && f.shiftKey) {
                        a.focus(1);
                        return false
                    }
                }
            });
            c([]).add(e.find(".ui-dialog-content :tabbable:first")).add(e.find(".ui-dialog-buttonpane :tabbable:first")).add(e).filter(":first").focus();
            this._trigger("open");
            this._isOpen = true;
            return this
        }
    }, _createButtons: function (b) {
        var e = this, f = false, a = c("<div></div>").addClass("ui-dialog-buttonpane ui-widget-content ui-helper-clearfix");
        e.uiDialog.find(".ui-dialog-buttonpane").remove();
        typeof b === "object" &&
            b !== null && c.each(b, function () {
            return!(f = true)
        });
        if (f) {
            c.each(b, function (g, h) {
                g = c('<button type="button"></button>').text(g).click(function () {
                    h.apply(e.element[0], arguments)
                }).appendTo(a);
                c.fn.button && g.button()
            });
            a.appendTo(e.uiDialog)
        }
    }, _makeDraggable: function () {
        function b(h) {
            return{position: h.position, offset: h.offset}
        }

        var e = this, f = e.options, a = c(document), g;
        e.uiDialog.draggable({cancel: ".ui-dialog-content, .ui-dialog-titlebar-close", handle: ".ui-dialog-titlebar", containment: "document", start: function (h, p) {
            g = f.height === "auto" ? "auto" : c(this).height();
            c(this).height(c(this).height()).addClass("ui-dialog-dragging");
            e._trigger("dragStart", h, b(p))
        }, drag: function (h, p) {
            e._trigger("drag", h, b(p))
        }, stop: function (h, p) {
            f.position = [p.position.left - a.scrollLeft(), p.position.top - a.scrollTop()];
            c(this).removeClass("ui-dialog-dragging").height(g);
            e._trigger("dragStop", h, b(p));
            c.ui.dialog.overlay.resize()
        }})
    }, _makeResizable: function (b) {
        function e(h) {
            return{originalPosition: h.originalPosition, originalSize: h.originalSize,
                position: h.position, size: h.size}
        }

        b = b === undefined ? this.options.resizable : b;
        var f = this, a = f.options, g = f.uiDialog.css("position");
        b = typeof b === "string" ? b : "n,e,s,w,se,sw,ne,nw";
        f.uiDialog.resizable({cancel: ".ui-dialog-content", containment: "document", alsoResize: f.element, maxWidth: a.maxWidth, maxHeight: a.maxHeight, minWidth: a.minWidth, minHeight: f._minHeight(), handles: b, start: function (h, p) {
            c(this).addClass("ui-dialog-resizing");
            f._trigger("resizeStart", h, e(p))
        }, resize: function (h, p) {
            f._trigger("resize", h, e(p))
        },
            stop: function (h, p) {
                c(this).removeClass("ui-dialog-resizing");
                a.height = c(this).height();
                a.width = c(this).width();
                f._trigger("resizeStop", h, e(p));
                c.ui.dialog.overlay.resize()
            }}).css("position", g).find(".ui-resizable-se").addClass("ui-icon ui-icon-grip-diagonal-se")
    }, _minHeight: function () {
        var b = this.options;
        return b.height === "auto" ? b.minHeight : Math.min(b.minHeight, b.height)
    }, _position: function (b) {
        var e = [], f = [0, 0];
        b = b || c.ui.dialog.prototype.options.position;
        if (typeof b === "string" || typeof b === "object" &&
            "0"in b) {
            e = b.split ? b.split(" ") : [b[0], b[1]];
            if (e.length === 1)e[1] = e[0];
            c.each(["left", "top"], function (a, g) {
                if (+e[a] === e[a]) {
                    f[a] = e[a];
                    e[a] = g
                }
            })
        } else if (typeof b === "object") {
            if ("left"in b) {
                e[0] = "left";
                f[0] = b.left
            } else if ("right"in b) {
                e[0] = "right";
                f[0] = -b.right
            }
            if ("top"in b) {
                e[1] = "top";
                f[1] = b.top
            } else if ("bottom"in b) {
                e[1] = "bottom";
                f[1] = -b.bottom
            }
        }
        (b = this.uiDialog.is(":visible")) || this.uiDialog.show();
        this.uiDialog.css({top: 0, left: 0}).position({my: e.join(" "), at: e.join(" "), offset: f.join(" "), of: window,
            collision: "fit", using: function (a) {
                var g = c(this).css(a).offset().top;
                g < 0 && c(this).css("top", a.top - g)
            }});
        b || this.uiDialog.hide()
    }, _setOption: function (b, e) {
        var f = this.uiDialog, a = f.is(":data(resizable)"), g = false;
        switch (b) {
            case "beforeclose":
                b = "beforeClose";
                break;
            case "buttons":
                this._createButtons(e);
                break;
            case "closeText":
                this.uiDialogTitlebarCloseText.text("" + e);
                break;
            case "dialogClass":
                f.removeClass(this.options.dialogClass).addClass("ui-dialog ui-widget ui-widget-content ui-corner-all " + e);
                break;
            case "disabled":
                e ?
                    f.addClass("ui-dialog-disabled") : f.removeClass("ui-dialog-disabled");
                break;
            case "draggable":
                e ? this._makeDraggable() : f.draggable("destroy");
                break;
            case "height":
                g = true;
                break;
            case "maxHeight":
                a && f.resizable("option", "maxHeight", e);
                g = true;
                break;
            case "maxWidth":
                a && f.resizable("option", "maxWidth", e);
                g = true;
                break;
            case "minHeight":
                a && f.resizable("option", "minHeight", e);
                g = true;
                break;
            case "minWidth":
                a && f.resizable("option", "minWidth", e);
                g = true;
                break;
            case "position":
                this._position(e);
                break;
            case "resizable":
                a && !e && f.resizable("destroy");
                a && typeof e === "string" && f.resizable("option", "handles", e);
                !a && e !== false && this._makeResizable(e);
                break;
            case "title":
                c(".ui-dialog-title", this.uiDialogTitlebar).html("" + (e || "&#160;"));
                break;
            case "width":
                g = true;
                break
        }
        c.Widget.prototype._setOption.apply(this, arguments);
        g && this._size()
    }, _size: function () {
        var b = this.options, e;
        this.element.css({width: "auto", minHeight: 0, height: 0});
        e = this.uiDialog.css({height: "auto", width: b.width}).height();
        this.element.css(b.height === "auto" ? {minHeight: Math.max(b.minHeight -
            e, 0), height: "auto"} : {minHeight: 0, height: Math.max(b.height - e, 0)}).show();
        this.uiDialog.is(":data(resizable)") && this.uiDialog.resizable("option", "minHeight", this._minHeight())
    }});
    c.extend(c.ui.dialog, {version: "1.8.2", uuid: 0, maxZ: 0, getTitleId: function (b) {
        b = b.attr("id");
        if (!b) {
            this.uuid += 1;
            b = this.uuid
        }
        return"ui-dialog-title-" + b
    }, overlay: function (b) {
        this.$el = c.ui.dialog.overlay.create(b)
    }});
    c.extend(c.ui.dialog.overlay, {instances: [], oldInstances: [], maxZ: 0, events: c.map("focus,mousedown,mouseup,keydown,keypress,click".split(","),
        function (b) {
            return b + ".dialog-overlay"
        }).join(" "), create: function (b) {
        if (this.instances.length === 0) {
            setTimeout(function () {
                c.ui.dialog.overlay.instances.length && c(document).bind(c.ui.dialog.overlay.events, function (f) {
                    return c(f.target).zIndex() >= c.ui.dialog.overlay.maxZ
                })
            }, 1);
            c(document).bind("keydown.dialog-overlay", function (f) {
                if (b.options.closeOnEscape && f.keyCode && f.keyCode === c.ui.keyCode.ESCAPE) {
                    b.close(f);
                    f.preventDefault()
                }
            });
            c(window).bind("resize.dialog-overlay", c.ui.dialog.overlay.resize)
        }
        var e =
            (this.oldInstances.pop() || c("<div></div>").addClass("ui-widget-overlay")).appendTo(document.body).css({width: this.width(), height: this.height()});
        c.fn.bgiframe && e.bgiframe();
        this.instances.push(e);
        return e
    }, destroy: function (b) {
        this.oldInstances.push(this.instances.splice(c.inArray(b, this.instances), 1)[0]);
        this.instances.length === 0 && c([document, window]).unbind(".dialog-overlay");
        b.remove();
        var e = 0;
        c.each(this.instances, function () {
            e = Math.max(e, this.css("z-index"))
        });
        this.maxZ = e
    }, height: function () {
        var b,
            e;
        if (c.browser.msie && c.browser.version < 7) {
            b = Math.max(document.documentElement.scrollHeight, document.body.scrollHeight);
            e = Math.max(document.documentElement.offsetHeight, document.body.offsetHeight);
            return b < e ? c(window).height() + "px" : b + "px"
        } else return c(document).height() + "px"
    }, width: function () {
        var b, e;
        if (c.browser.msie && c.browser.version < 7) {
            b = Math.max(document.documentElement.scrollWidth, document.body.scrollWidth);
            e = Math.max(document.documentElement.offsetWidth, document.body.offsetWidth);
            return b <
                e ? c(window).width() + "px" : b + "px"
        } else return c(document).width() + "px"
    }, resize: function () {
        var b = c([]);
        c.each(c.ui.dialog.overlay.instances, function () {
            b = b.add(this)
        });
        b.css({width: 0, height: 0}).css({width: c.ui.dialog.overlay.width(), height: c.ui.dialog.overlay.height()})
    }});
    c.extend(c.ui.dialog.overlay.prototype, {destroy: function () {
        c.ui.dialog.overlay.destroy(this.$el)
    }})
})(jQuery);
(function (c) {
    c.ui = c.ui || {};
    var b = /left|center|right/, e = /top|center|bottom/, f = c.fn.position, a = c.fn.offset;
    c.fn.position = function (g) {
        if (!g || !g.of)return f.apply(this, arguments);
        g = c.extend({}, g);
        var h = c(g.of), p = (g.collision || "flip").split(" "), q = g.offset ? g.offset.split(" ") : [0, 0], o, l, r;
        if (g.of.nodeType === 9) {
            o = h.width();
            l = h.height();
            r = {top: 0, left: 0}
        } else if (g.of.scrollTo && g.of.document) {
            o = h.width();
            l = h.height();
            r = {top: h.scrollTop(), left: h.scrollLeft()}
        } else if (g.of.preventDefault) {
            g.at = "left top";
            o = l =
                0;
            r = {top: g.of.pageY, left: g.of.pageX}
        } else {
            o = h.outerWidth();
            l = h.outerHeight();
            r = h.offset()
        }
        c.each(["my", "at"], function () {
            var u = (g[this] || "").split(" ");
            if (u.length === 1)u = b.test(u[0]) ? u.concat(["center"]) : e.test(u[0]) ? ["center"].concat(u) : ["center", "center"];
            u[0] = b.test(u[0]) ? u[0] : "center";
            u[1] = e.test(u[1]) ? u[1] : "center";
            g[this] = u
        });
        if (p.length === 1)p[1] = p[0];
        q[0] = parseInt(q[0], 10) || 0;
        if (q.length === 1)q[1] = q[0];
        q[1] = parseInt(q[1], 10) || 0;
        if (g.at[0] === "right")r.left += o; else if (g.at[0] === "center")r.left +=
            o / 2;
        if (g.at[1] === "bottom")r.top += l; else if (g.at[1] === "center")r.top += l / 2;
        r.left += q[0];
        r.top += q[1];
        return this.each(function () {
            var u = c(this), y = u.outerWidth(), A = u.outerHeight(), i = c.extend({}, r);
            if (g.my[0] === "right")i.left -= y; else if (g.my[0] === "center")i.left -= y / 2;
            if (g.my[1] === "bottom")i.top -= A; else if (g.my[1] === "center")i.top -= A / 2;
            i.left = parseInt(i.left);
            i.top = parseInt(i.top);
            c.each(["left", "top"], function (J, K) {
                c.ui.position[p[J]] && c.ui.position[p[J]][K](i, {targetWidth: o, targetHeight: l, elemWidth: y,
                    elemHeight: A, offset: q, my: g.my, at: g.at})
            });
            c.fn.bgiframe && u.bgiframe();
            u.offset(c.extend(i, {using: g.using}))
        })
    };
    c.ui.position = {fit: {left: function (g, h) {
        var p = c(window);
        h = g.left + h.elemWidth - p.width() - p.scrollLeft();
        g.left = h > 0 ? g.left - h : Math.max(0, g.left)
    }, top: function (g, h) {
        var p = c(window);
        h = g.top + h.elemHeight - p.height() - p.scrollTop();
        g.top = h > 0 ? g.top - h : Math.max(0, g.top)
    }}, flip: {left: function (g, h) {
        if (h.at[0] !== "center") {
            var p = c(window);
            p = g.left + h.elemWidth - p.width() - p.scrollLeft();
            var q = h.my[0] === "left" ?
                -h.elemWidth : h.my[0] === "right" ? h.elemWidth : 0, o = -2 * h.offset[0];
            g.left += g.left < 0 ? q + h.targetWidth + o : p > 0 ? q - h.targetWidth + o : 0
        }
    }, top: function (g, h) {
        if (h.at[1] !== "center") {
            var p = c(window);
            p = g.top + h.elemHeight - p.height() - p.scrollTop();
            var q = h.my[1] === "top" ? -h.elemHeight : h.my[1] === "bottom" ? h.elemHeight : 0, o = h.at[1] === "top" ? h.targetHeight : -h.targetHeight, l = -2 * h.offset[1];
            g.top += g.top < 0 ? q + h.targetHeight + l : p > 0 ? q + o + l : 0
        }
    }}};
    if (!c.offset.setOffset) {
        c.offset.setOffset = function (g, h) {
            if (/static/.test(c.curCSS(g, "position")))g.style.position =
                "relative";
            var p = c(g), q = p.offset(), o = parseInt(c.curCSS(g, "top", true), 10) || 0, l = parseInt(c.curCSS(g, "left", true), 10) || 0;
            q = {top: h.top - q.top + o, left: h.left - q.left + l};
            "using"in h ? h.using.call(g, q) : p.css(q)
        };
        c.fn.offset = function (g) {
            var h = this[0];
            if (!h || !h.ownerDocument)return null;
            if (g)return this.each(function () {
                c.offset.setOffset(this, g)
            });
            return a.call(this)
        }
    }
})(jQuery);
(function (c) {
    c.widget("ui.progressbar", {options: {value: 0}, _create: function () {
        this.element.addClass("ui-progressbar ui-widget ui-widget-content ui-corner-all").attr({role: "progressbar", "aria-valuemin": this._valueMin(), "aria-valuemax": this._valueMax(), "aria-valuenow": this._value()});
        this.valueDiv = c("<div class='ui-progressbar-value ui-widget-header ui-corner-left'></div>").appendTo(this.element);
        this._refreshValue()
    }, destroy: function () {
        this.element.removeClass("ui-progressbar ui-widget ui-widget-content ui-corner-all").removeAttr("role").removeAttr("aria-valuemin").removeAttr("aria-valuemax").removeAttr("aria-valuenow");
        this.valueDiv.remove();
        c.Widget.prototype.destroy.apply(this, arguments)
    }, value: function (b) {
        if (b === undefined)return this._value();
        this._setOption("value", b);
        return this
    }, _setOption: function (b, e) {
        switch (b) {
            case "value":
                this.options.value = e;
                this._refreshValue();
                this._trigger("change");
                break
        }
        c.Widget.prototype._setOption.apply(this, arguments)
    }, _value: function () {
        var b = this.options.value;
        if (typeof b !== "number")b = 0;
        if (b < this._valueMin())b = this._valueMin();
        if (b > this._valueMax())b = this._valueMax();
        return b
    },
        _valueMin: function () {
            return 0
        }, _valueMax: function () {
            return 100
        }, _refreshValue: function () {
            var b = this.value();
            this.valueDiv[b === this._valueMax() ? "addClass" : "removeClass"]("ui-corner-right").width(b + "%");
            this.element.attr("aria-valuenow", b)
        }});
    c.extend(c.ui.progressbar, {version: "1.8.2"})
})(jQuery);
(function (c) {
    c.widget("ui.slider", c.ui.mouse, {widgetEventPrefix: "slide", options: {animate: false, distance: 0, max: 100, min: 0, orientation: "horizontal", range: false, step: 1, value: 0, values: null}, _create: function () {
        var b = this, e = this.options;
        this._mouseSliding = this._keySliding = false;
        this._animateOff = true;
        this._handleIndex = null;
        this._detectOrientation();
        this._mouseInit();
        this.element.addClass("ui-slider ui-slider-" + this.orientation + " ui-widget ui-widget-content ui-corner-all");
        e.disabled && this.element.addClass("ui-slider-disabled ui-disabled");
        this.range = c([]);
        if (e.range) {
            if (e.range === true) {
                this.range = c("<div></div>");
                if (!e.values)e.values = [this._valueMin(), this._valueMin()];
                if (e.values.length && e.values.length !== 2)e.values = [e.values[0], e.values[0]]
            } else this.range = c("<div></div>");
            this.range.appendTo(this.element).addClass("ui-slider-range");
            if (e.range === "min" || e.range === "max")this.range.addClass("ui-slider-range-" + e.range);
            this.range.addClass("ui-widget-header")
        }
        c(".ui-slider-handle", this.element).length === 0 && c("<a href='#'></a>").appendTo(this.element).addClass("ui-slider-handle");
        if (e.values && e.values.length)for (; c(".ui-slider-handle", this.element).length < e.values.length;)c("<a href='#'></a>").appendTo(this.element).addClass("ui-slider-handle");
        this.handles = c(".ui-slider-handle", this.element).addClass("ui-state-default ui-corner-all");
        this.handle = this.handles.eq(0);
        this.handles.add(this.range).filter("a").click(function (f) {
            f.preventDefault()
        }).hover(function () {
            e.disabled || c(this).addClass("ui-state-hover")
        }, function () {
            c(this).removeClass("ui-state-hover")
        }).focus(function () {
            if (e.disabled)c(this).blur();
            else {
                c(".ui-slider .ui-state-focus").removeClass("ui-state-focus");
                c(this).addClass("ui-state-focus")
            }
        }).blur(function () {
            c(this).removeClass("ui-state-focus")
        });
        this.handles.each(function (f) {
            c(this).data("index.ui-slider-handle", f)
        });
        this.handles.keydown(function (f) {
            var a = true, g = c(this).data("index.ui-slider-handle"), h, p, q;
            if (!b.options.disabled) {
                switch (f.keyCode) {
                    case c.ui.keyCode.HOME:
                    case c.ui.keyCode.END:
                    case c.ui.keyCode.PAGE_UP:
                    case c.ui.keyCode.PAGE_DOWN:
                    case c.ui.keyCode.UP:
                    case c.ui.keyCode.RIGHT:
                    case c.ui.keyCode.DOWN:
                    case c.ui.keyCode.LEFT:
                        a =
                            false;
                        if (!b._keySliding) {
                            b._keySliding = true;
                            c(this).addClass("ui-state-active");
                            h = b._start(f, g);
                            if (h === false)return
                        }
                        break
                }
                q = b.options.step;
                h = b.options.values && b.options.values.length ? p = b.values(g) : p = b.value();
                switch (f.keyCode) {
                    case c.ui.keyCode.HOME:
                        p = b._valueMin();
                        break;
                    case c.ui.keyCode.END:
                        p = b._valueMax();
                        break;
                    case c.ui.keyCode.PAGE_UP:
                        p = b._trimAlignValue(h + (b._valueMax() - b._valueMin()) / 5);
                        break;
                    case c.ui.keyCode.PAGE_DOWN:
                        p = b._trimAlignValue(h - (b._valueMax() - b._valueMin()) / 5);
                        break;
                    case c.ui.keyCode.UP:
                    case c.ui.keyCode.RIGHT:
                        if (h ===
                            b._valueMax())return;
                        p = b._trimAlignValue(h + q);
                        break;
                    case c.ui.keyCode.DOWN:
                    case c.ui.keyCode.LEFT:
                        if (h === b._valueMin())return;
                        p = b._trimAlignValue(h - q);
                        break
                }
                b._slide(f, g, p);
                return a
            }
        }).keyup(function (f) {
            var a = c(this).data("index.ui-slider-handle");
            if (b._keySliding) {
                b._keySliding = false;
                b._stop(f, a);
                b._change(f, a);
                c(this).removeClass("ui-state-active")
            }
        });
        this._refreshValue();
        this._animateOff = false
    }, destroy: function () {
        this.handles.remove();
        this.range.remove();
        this.element.removeClass("ui-slider ui-slider-horizontal ui-slider-vertical ui-slider-disabled ui-widget ui-widget-content ui-corner-all").removeData("slider").unbind(".slider");
        this._mouseDestroy();
        return this
    }, _mouseCapture: function (b) {
        var e = this.options, f, a, g, h, p, q;
        if (e.disabled)return false;
        this.elementSize = {width: this.element.outerWidth(), height: this.element.outerHeight()};
        this.elementOffset = this.element.offset();
        f = {x: b.pageX, y: b.pageY};
        a = this._normValueFromMouse(f);
        g = this._valueMax() - this._valueMin() + 1;
        p = this;
        this.handles.each(function (o) {
            var l = Math.abs(a - p.values(o));
            if (g > l) {
                g = l;
                h = c(this);
                q = o
            }
        });
        if (e.range === true && this.values(1) === e.min) {
            q += 1;
            h = c(this.handles[q])
        }
        if (this._start(b,
            q) === false)return false;
        this._mouseSliding = true;
        p._handleIndex = q;
        h.addClass("ui-state-active").focus();
        e = h.offset();
        this._clickOffset = !c(b.target).parents().andSelf().is(".ui-slider-handle") ? {left: 0, top: 0} : {left: b.pageX - e.left - h.width() / 2, top: b.pageY - e.top - h.height() / 2 - (parseInt(h.css("borderTopWidth"), 10) || 0) - (parseInt(h.css("borderBottomWidth"), 10) || 0) + (parseInt(h.css("marginTop"), 10) || 0)};
        a = this._normValueFromMouse(f);
        this._slide(b, q, a);
        return this._animateOff = true
    }, _mouseStart: function () {
        return true
    },
        _mouseDrag: function (b) {
            var e = this._normValueFromMouse({x: b.pageX, y: b.pageY});
            this._slide(b, this._handleIndex, e);
            return false
        }, _mouseStop: function (b) {
            this.handles.removeClass("ui-state-active");
            this._mouseSliding = false;
            this._stop(b, this._handleIndex);
            this._change(b, this._handleIndex);
            this._clickOffset = this._handleIndex = null;
            return this._animateOff = false
        }, _detectOrientation: function () {
            this.orientation = this.options.orientation === "vertical" ? "vertical" : "horizontal"
        }, _normValueFromMouse: function (b) {
            var e;
            if (this.orientation === "horizontal") {
                e = this.elementSize.width;
                b = b.x - this.elementOffset.left - (this._clickOffset ? this._clickOffset.left : 0)
            } else {
                e = this.elementSize.height;
                b = b.y - this.elementOffset.top - (this._clickOffset ? this._clickOffset.top : 0)
            }
            e = b / e;
            if (e > 1)e = 1;
            if (e < 0)e = 0;
            if (this.orientation === "vertical")e = 1 - e;
            b = this._valueMax() - this._valueMin();
            return this._trimAlignValue(this._valueMin() + e * b)
        }, _start: function (b, e) {
            var f = {handle: this.handles[e], value: this.value()};
            if (this.options.values && this.options.values.length) {
                f.value =
                    this.values(e);
                f.values = this.values()
            }
            return this._trigger("start", b, f)
        }, _slide: function (b, e, f) {
            var a;
            if (this.options.values && this.options.values.length) {
                a = this.values(e ? 0 : 1);
                if (this.options.values.length === 2 && this.options.range === true && (e === 0 && f > a || e === 1 && f < a))f = a;
                if (f !== this.values(e)) {
                    a = this.values();
                    a[e] = f;
                    b = this._trigger("slide", b, {handle: this.handles[e], value: f, values: a});
                    this.values(e ? 0 : 1);
                    b !== false && this.values(e, f, true)
                }
            } else if (f !== this.value()) {
                b = this._trigger("slide", b, {handle: this.handles[e],
                    value: f});
                b !== false && this.value(f)
            }
        }, _stop: function (b, e) {
            var f = {handle: this.handles[e], value: this.value()};
            if (this.options.values && this.options.values.length) {
                f.value = this.values(e);
                f.values = this.values()
            }
            this._trigger("stop", b, f)
        }, _change: function (b, e) {
            if (!this._keySliding && !this._mouseSliding) {
                var f = {handle: this.handles[e], value: this.value()};
                if (this.options.values && this.options.values.length) {
                    f.value = this.values(e);
                    f.values = this.values()
                }
                this._trigger("change", b, f)
            }
        }, value: function (b) {
            if (arguments.length) {
                this.options.value =
                    this._trimAlignValue(b);
                this._refreshValue();
                this._change(null, 0)
            }
            return this._value()
        }, values: function (b, e) {
            var f, a, g;
            if (arguments.length > 1) {
                this.options.values[b] = this._trimAlignValue(e);
                this._refreshValue();
                this._change(null, b)
            }
            if (arguments.length)if (c.isArray(arguments[0])) {
                f = this.options.values;
                a = arguments[0];
                for (g = 0; g < f.length; g += 1) {
                    f[g] = this._trimAlignValue(a[g]);
                    this._change(null, g)
                }
                this._refreshValue()
            } else return this.options.values && this.options.values.length ? this._values(b) : this.value();
            else return this._values()
        }, _setOption: function (b, e) {
            var f, a = 0;
            if (c.isArray(this.options.values))a = this.options.values.length;
            c.Widget.prototype._setOption.apply(this, arguments);
            switch (b) {
                case "disabled":
                    if (e) {
                        this.handles.filter(".ui-state-focus").blur();
                        this.handles.removeClass("ui-state-hover");
                        this.handles.attr("disabled", "disabled");
                        this.element.addClass("ui-disabled")
                    } else {
                        this.handles.removeAttr("disabled");
                        this.element.removeClass("ui-disabled")
                    }
                    break;
                case "orientation":
                    this._detectOrientation();
                    this.element.removeClass("ui-slider-horizontal ui-slider-vertical").addClass("ui-slider-" + this.orientation);
                    this._refreshValue();
                    break;
                case "value":
                    this._animateOff = true;
                    this._refreshValue();
                    this._change(null, 0);
                    this._animateOff = false;
                    break;
                case "values":
                    this._animateOff = true;
                    this._refreshValue();
                    for (f = 0; f < a; f += 1)this._change(null, f);
                    this._animateOff = false;
                    break
            }
        }, _value: function () {
            return this._trimAlignValue(this.options.value)
        }, _values: function (b) {
            var e, f;
            if (arguments.length) {
                e = this.options.values[b];
                return this._trimAlignValue(e)
            } else {
                e = this.options.values.slice();
                for (f = 0; f < e.length; f += 1)e[f] = this._trimAlignValue(e[f]);
                return e
            }
        }, _trimAlignValue: function (b) {
            if (b < this._valueMin())return this._valueMin();
            if (b > this._valueMax())return this._valueMax();
            var e = this.options.step > 0 ? this.options.step : 1, f = b % e;
            b -= f;
            if (Math.abs(f) * 2 >= e)b += f > 0 ? e : -e;
            return parseFloat(b.toFixed(5))
        }, _valueMin: function () {
            return this.options.min
        }, _valueMax: function () {
            return this.options.max
        }, _refreshValue: function () {
            var b = this.options.range,
                e = this.options, f = this, a = !this._animateOff ? e.animate : false, g, h = {}, p, q, o, l;
            if (this.options.values && this.options.values.length)this.handles.each(function (r) {
                g = (f.values(r) - f._valueMin()) / (f._valueMax() - f._valueMin()) * 100;
                h[f.orientation === "horizontal" ? "left" : "bottom"] = g + "%";
                c(this).stop(1, 1)[a ? "animate" : "css"](h, e.animate);
                if (f.options.range === true)if (f.orientation === "horizontal") {
                    if (r === 0)f.range.stop(1, 1)[a ? "animate" : "css"]({left: g + "%"}, e.animate);
                    if (r === 1)f.range[a ? "animate" : "css"]({width: g - p + "%"},
                        {queue: false, duration: e.animate})
                } else {
                    if (r === 0)f.range.stop(1, 1)[a ? "animate" : "css"]({bottom: g + "%"}, e.animate);
                    if (r === 1)f.range[a ? "animate" : "css"]({height: g - p + "%"}, {queue: false, duration: e.animate})
                }
                p = g
            }); else {
                q = this.value();
                o = this._valueMin();
                l = this._valueMax();
                g = l !== o ? (q - o) / (l - o) * 100 : 0;
                h[f.orientation === "horizontal" ? "left" : "bottom"] = g + "%";
                this.handle.stop(1, 1)[a ? "animate" : "css"](h, e.animate);
                if (b === "min" && this.orientation === "horizontal")this.range.stop(1, 1)[a ? "animate" : "css"]({width: g + "%"}, e.animate);
                if (b === "max" && this.orientation === "horizontal")this.range[a ? "animate" : "css"]({width: 100 - g + "%"}, {queue: false, duration: e.animate});
                if (b === "min" && this.orientation === "vertical")this.range.stop(1, 1)[a ? "animate" : "css"]({height: g + "%"}, e.animate);
                if (b === "max" && this.orientation === "vertical")this.range[a ? "animate" : "css"]({height: 100 - g + "%"}, {queue: false, duration: e.animate})
            }
        }});
    c.extend(c.ui.slider, {version: "1.8.2"})
})(jQuery);
(function (c) {
    var b = 0, e = 0;
    c.widget("ui.tabs", {options: {add: null, ajaxOptions: null, cache: false, cookie: null, collapsible: false, disable: null, disabled: [], enable: null, event: "click", fx: null, idPrefix: "ui-tabs-", load: null, panelTemplate: "<div></div>", remove: null, select: null, show: null, spinner: "<em>Loading&#8230;</em>", tabTemplate: '<li><a href="#{href}"><span>#{label}</span></a></li>'}, _create: function () {
        this._tabify(true)
    }, _setOption: function (f, a) {
        if (f == "selected")this.options.collapsible && a == this.options.selected ||
        this.select(a); else {
            this.options[f] = a;
            this._tabify()
        }
    }, _tabId: function (f) {
        return f.title && f.title.replace(/\s/g, "_").replace(/[^A-Za-z0-9\-_:\.]/g, "") || this.options.idPrefix + ++b
    }, _sanitizeSelector: function (f) {
        return f.replace(/:/g, "\\:")
    }, _cookie: function () {
        var f = this.cookie || (this.cookie = this.options.cookie.name || "ui-tabs-" + ++e);
        return c.cookie.apply(null, [f].concat(c.makeArray(arguments)))
    }, _ui: function (f, a) {
        return{tab: f, panel: a, index: this.anchors.index(f)}
    }, _cleanup: function () {
        this.lis.filter(".ui-state-processing").removeClass("ui-state-processing").find("span:data(label.tabs)").each(function () {
            var f =
                c(this);
            f.html(f.data("label.tabs")).removeData("label.tabs")
        })
    }, _tabify: function (f) {
        function a(A, i) {
            A.css({display: ""});
            !c.support.opacity && i.opacity && A[0].style.removeAttribute("filter")
        }

        this.list = this.element.find("ol,ul").eq(0);
        this.lis = c("li:has(a[href])", this.list);
        this.anchors = this.lis.map(function () {
            return c("a", this)[0]
        });
        this.panels = c([]);
        var g = this, h = this.options, p = /^#.+/;
        this.anchors.each(function (A, i) {
            var J = c(i).attr("href"), K = J.split("#")[0], D;
            if (K && (K === location.toString().split("#")[0] ||
                (D = c("base")[0]) && K === D.href)) {
                J = i.hash;
                i.href = J
            }
            if (p.test(J))g.panels = g.panels.add(g._sanitizeSelector(J)); else if (J != "#") {
                c.data(i, "href.tabs", J);
                c.data(i, "load.tabs", J.replace(/#.*$/, ""));
                J = g._tabId(i);
                i.href = "#" + J;
                i = c("#" + J);
                if (!i.length) {
                    i = c(h.panelTemplate).attr("id", J).addClass("ui-tabs-panel ui-widget-content ui-corner-bottom").insertAfter(g.panels[A - 1] || g.list);
                    i.data("destroy.tabs", true)
                }
                g.panels = g.panels.add(i)
            } else h.disabled.push(A)
        });
        if (f) {
            this.element.addClass("ui-tabs ui-widget ui-widget-content ui-corner-all");
            this.list.addClass("ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all");
            this.lis.addClass("ui-state-default ui-corner-top");
            this.panels.addClass("ui-tabs-panel ui-widget-content ui-corner-bottom");
            if (h.selected === undefined) {
                location.hash && this.anchors.each(function (A, i) {
                    if (i.hash == location.hash) {
                        h.selected = A;
                        return false
                    }
                });
                if (typeof h.selected != "number" && h.cookie)h.selected = parseInt(g._cookie(), 10);
                if (typeof h.selected != "number" && this.lis.filter(".ui-tabs-selected").length)h.selected =
                    this.lis.index(this.lis.filter(".ui-tabs-selected"));
                h.selected = h.selected || (this.lis.length ? 0 : -1)
            } else if (h.selected === null)h.selected = -1;
            h.selected = h.selected >= 0 && this.anchors[h.selected] || h.selected < 0 ? h.selected : 0;
            h.disabled = c.unique(h.disabled.concat(c.map(this.lis.filter(".ui-state-disabled"), function (A) {
                return g.lis.index(A)
            }))).sort();
            c.inArray(h.selected, h.disabled) != -1 && h.disabled.splice(c.inArray(h.selected, h.disabled), 1);
            this.panels.addClass("ui-tabs-hide");
            this.lis.removeClass("ui-tabs-selected ui-state-active");
            if (h.selected >= 0 && this.anchors.length) {
                this.panels.eq(h.selected).removeClass("ui-tabs-hide");
                this.lis.eq(h.selected).addClass("ui-tabs-selected ui-state-active");
                g.element.queue("tabs", function () {
                    g._trigger("show", null, g._ui(g.anchors[h.selected], g.panels[h.selected]))
                });
                this.load(h.selected)
            }
            c(window).bind("unload", function () {
                g.lis.add(g.anchors).unbind(".tabs");
                g.lis = g.anchors = g.panels = null
            })
        } else h.selected = this.lis.index(this.lis.filter(".ui-tabs-selected"));
        this.element[h.collapsible ? "addClass" :
            "removeClass"]("ui-tabs-collapsible");
        h.cookie && this._cookie(h.selected, h.cookie);
        f = 0;
        for (var q; q = this.lis[f]; f++)c(q)[c.inArray(f, h.disabled) != -1 && !c(q).hasClass("ui-tabs-selected") ? "addClass" : "removeClass"]("ui-state-disabled");
        h.cache === false && this.anchors.removeData("cache.tabs");
        this.lis.add(this.anchors).unbind(".tabs");
        if (h.event != "mouseover") {
            var o = function (A, i) {
                i.is(":not(.ui-state-disabled)") && i.addClass("ui-state-" + A)
            };
            this.lis.bind("mouseover.tabs", function () {
                o("hover", c(this))
            });
            this.lis.bind("mouseout.tabs",
                function () {
                    c(this).removeClass("ui-state-hover")
                });
            this.anchors.bind("focus.tabs", function () {
                o("focus", c(this).closest("li"))
            });
            this.anchors.bind("blur.tabs", function () {
                c(this).closest("li").removeClass("ui-state-focus")
            })
        }
        var l, r;
        if (h.fx)if (c.isArray(h.fx)) {
            l = h.fx[0];
            r = h.fx[1]
        } else l = r = h.fx;
        var u = r ? function (A, i) {
            c(A).closest("li").addClass("ui-tabs-selected ui-state-active");
            i.hide().removeClass("ui-tabs-hide").animate(r, r.duration || "normal", function () {
                a(i, r);
                g._trigger("show", null, g._ui(A, i[0]))
            })
        } :
            function (A, i) {
                c(A).closest("li").addClass("ui-tabs-selected ui-state-active");
                i.removeClass("ui-tabs-hide");
                g._trigger("show", null, g._ui(A, i[0]))
            }, y = l ? function (A, i) {
            i.animate(l, l.duration || "normal", function () {
                g.lis.removeClass("ui-tabs-selected ui-state-active");
                i.addClass("ui-tabs-hide");
                a(i, l);
                g.element.dequeue("tabs")
            })
        } : function (A, i) {
            g.lis.removeClass("ui-tabs-selected ui-state-active");
            i.addClass("ui-tabs-hide");
            g.element.dequeue("tabs")
        };
        this.anchors.bind(h.event + ".tabs", function () {
            var A = this,
                i = c(this).closest("li"), J = g.panels.filter(":not(.ui-tabs-hide)"), K = c(g._sanitizeSelector(this.hash));
            if (i.hasClass("ui-tabs-selected") && !h.collapsible || i.hasClass("ui-state-disabled") || i.hasClass("ui-state-processing") || g._trigger("select", null, g._ui(this, K[0])) === false) {
                this.blur();
                return false
            }
            h.selected = g.anchors.index(this);
            g.abort();
            if (h.collapsible)if (i.hasClass("ui-tabs-selected")) {
                h.selected = -1;
                h.cookie && g._cookie(h.selected, h.cookie);
                g.element.queue("tabs", function () {
                    y(A, J)
                }).dequeue("tabs");
                this.blur();
                return false
            } else if (!J.length) {
                h.cookie && g._cookie(h.selected, h.cookie);
                g.element.queue("tabs", function () {
                    u(A, K)
                });
                g.load(g.anchors.index(this));
                this.blur();
                return false
            }
            h.cookie && g._cookie(h.selected, h.cookie);
            if (K.length) {
                J.length && g.element.queue("tabs", function () {
                    y(A, J)
                });
                g.element.queue("tabs", function () {
                    u(A, K)
                });
                g.load(g.anchors.index(this))
            } else throw"jQuery UI Tabs: Mismatching fragment identifier.";
            c.browser.msie && this.blur()
        });
        this.anchors.bind("click.tabs", function () {
            return false
        })
    },
        destroy: function () {
            var f = this.options;
            this.abort();
            this.element.unbind(".tabs").removeClass("ui-tabs ui-widget ui-widget-content ui-corner-all ui-tabs-collapsible").removeData("tabs");
            this.list.removeClass("ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all");
            this.anchors.each(function () {
                var a = c.data(this, "href.tabs");
                if (a)this.href = a;
                var g = c(this).unbind(".tabs");
                c.each(["href", "load", "cache"], function (h, p) {
                    g.removeData(p + ".tabs")
                })
            });
            this.lis.unbind(".tabs").add(this.panels).each(function () {
                c.data(this,
                    "destroy.tabs") ? c(this).remove() : c(this).removeClass("ui-state-default ui-corner-top ui-tabs-selected ui-state-active ui-state-hover ui-state-focus ui-state-disabled ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide")
            });
            f.cookie && this._cookie(null, f.cookie);
            return this
        }, add: function (f, a, g) {
            if (g === undefined)g = this.anchors.length;
            var h = this, p = this.options;
            a = c(p.tabTemplate.replace(/#\{href\}/g, f).replace(/#\{label\}/g, a));
            f = !f.indexOf("#") ? f.replace("#", "") : this._tabId(c("a", a)[0]);
            a.addClass("ui-state-default ui-corner-top").data("destroy.tabs",
                true);
            var q = c("#" + f);
            q.length || (q = c(p.panelTemplate).attr("id", f).data("destroy.tabs", true));
            q.addClass("ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide");
            if (g >= this.lis.length) {
                a.appendTo(this.list);
                q.appendTo(this.list[0].parentNode)
            } else {
                a.insertBefore(this.lis[g]);
                q.insertBefore(this.panels[g])
            }
            p.disabled = c.map(p.disabled, function (o) {
                return o >= g ? ++o : o
            });
            this._tabify();
            if (this.anchors.length == 1) {
                p.selected = 0;
                a.addClass("ui-tabs-selected ui-state-active");
                q.removeClass("ui-tabs-hide");
                this.element.queue("tabs", function () {
                    h._trigger("show", null, h._ui(h.anchors[0], h.panels[0]))
                });
                this.load(0)
            }
            this._trigger("add", null, this._ui(this.anchors[g], this.panels[g]));
            return this
        }, remove: function (f) {
            var a = this.options, g = this.lis.eq(f).remove(), h = this.panels.eq(f).remove();
            if (g.hasClass("ui-tabs-selected") && this.anchors.length > 1)this.select(f + (f + 1 < this.anchors.length ? 1 : -1));
            a.disabled = c.map(c.grep(a.disabled, function (p) {
                return p != f
            }), function (p) {
                return p >= f ? --p : p
            });
            this._tabify();
            this._trigger("remove",
                null, this._ui(g.find("a")[0], h[0]));
            return this
        }, enable: function (f) {
            var a = this.options;
            if (c.inArray(f, a.disabled) != -1) {
                this.lis.eq(f).removeClass("ui-state-disabled");
                a.disabled = c.grep(a.disabled, function (g) {
                    return g != f
                });
                this._trigger("enable", null, this._ui(this.anchors[f], this.panels[f]));
                return this
            }
        }, disable: function (f) {
            var a = this.options;
            if (f != a.selected) {
                this.lis.eq(f).addClass("ui-state-disabled");
                a.disabled.push(f);
                a.disabled.sort();
                this._trigger("disable", null, this._ui(this.anchors[f], this.panels[f]))
            }
            return this
        },
        select: function (f) {
            if (typeof f == "string")f = this.anchors.index(this.anchors.filter("[href$=" + f + "]")); else if (f === null)f = -1;
            if (f == -1 && this.options.collapsible)f = this.options.selected;
            this.anchors.eq(f).trigger(this.options.event + ".tabs");
            return this
        }, load: function (f) {
            var a = this, g = this.options, h = this.anchors.eq(f)[0], p = c.data(h, "load.tabs");
            this.abort();
            if (!p || this.element.queue("tabs").length !== 0 && c.data(h, "cache.tabs"))this.element.dequeue("tabs"); else {
                this.lis.eq(f).addClass("ui-state-processing");
                if (g.spinner) {
                    var q = c("span", h);
                    q.data("label.tabs", q.html()).html(g.spinner)
                }
                this.xhr = c.ajax(c.extend({}, g.ajaxOptions, {url: p, success: function (o, l) {
                    c(a._sanitizeSelector(h.hash)).html(o);
                    a._cleanup();
                    g.cache && c.data(h, "cache.tabs", true);
                    a._trigger("load", null, a._ui(a.anchors[f], a.panels[f]));
                    try {
                        g.ajaxOptions.success(o, l)
                    } catch (r) {
                    }
                }, error: function (o, l) {
                    a._cleanup();
                    a._trigger("load", null, a._ui(a.anchors[f], a.panels[f]));
                    try {
                        g.ajaxOptions.error(o, l, f, h)
                    } catch (r) {
                    }
                }}));
                a.element.dequeue("tabs");
                return this
            }
        },
        abort: function () {
            this.element.queue([]);
            this.panels.stop(false, true);
            this.element.queue("tabs", this.element.queue("tabs").splice(-2, 2));
            if (this.xhr) {
                this.xhr.abort();
                delete this.xhr
            }
            this._cleanup();
            return this
        }, url: function (f, a) {
            this.anchors.eq(f).removeData("cache.tabs").data("load.tabs", a);
            return this
        }, length: function () {
            return this.anchors.length
        }});
    c.extend(c.ui.tabs, {version: "1.8.2"});
    c.extend(c.ui.tabs.prototype, {rotation: null, rotate: function (f, a) {
        var g = this, h = this.options, p = g._rotate || (g._rotate =
            function (q) {
                clearTimeout(g.rotation);
                g.rotation = setTimeout(function () {
                    var o = h.selected;
                    g.select(++o < g.anchors.length ? o : 0)
                }, f);
                q && q.stopPropagation()
            });
        a = g._unrotate || (g._unrotate = !a ? function (q) {
            q.clientX && g.rotate(null)
        } : function () {
            t = h.selected;
            p()
        });
        if (f) {
            this.element.bind("tabsshow", p);
            this.anchors.bind(h.event + ".tabs", a);
            p()
        } else {
            clearTimeout(g.rotation);
            this.element.unbind("tabsshow", p);
            this.anchors.unbind(h.event + ".tabs", a);
            delete this._rotate;
            delete this._unrotate
        }
        return this
    }})
})(jQuery);
