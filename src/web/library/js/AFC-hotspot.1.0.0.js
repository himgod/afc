$(function () {
    $("#model-timefilters").jlistmodel({rows: [
        {_id: 24},
        {_id: 72},
        {_id: 144},
        {_id: 288},
        {_id: 720},
        {_id: 2880}
    ]});
    $("select.time-filter").jselect({model: $("#model-timefilters"), renderer: function (a, b) {
        var c = b._id;
        return c == 24 ? "last 24 hours" : c == 72 ? "3 days" : c == 144 ? "7 days" : c == 288 ? "2 weeks" : c == 720 ? "30 days" : c == 2880 ? "120 days" : "" + c + " hours"
    }, selectionSynchronized: true});
    $("#model-timefilters").jlistmodel("refresh").jlistmodel("setselected", 72);
    var f = $("#Operators").length > 0;
    $("#model-guests").jtablemodel({url: "/stat/guest",
        params: {within: $("#Guests select.time-filter").val()}});
    $("#model-payments").jtablemodel({url: "/stat/payment", params: {within: $("#Payments select.time-filter").val()}});
    $("#model-vouchers").jtablemodel({url: "/stat/voucher", params: {}});
    f && $("#model-operators").jtablemodel({url: "/list/admin", params: {for_hotspot: true}});
    $("#model-timefilters").bind("jlistmodel_select-changed", function () {
        $("#model-guests").jtablemodel("option", "params", {within: $("#Guests select.time-filter").val()});
        $("#model-payments").jtablemodel("option",
            "params", {within: $("#Guests select.time-filter").val()});
        $("#model-guests").jtablemodel("refresh");
        $("#model-payments").jtablemodel("refresh")
    });
    $("#MainPanel").tabs().bind("tabsshow", function (a, b) {
        var c = $("#" + b.panel.id).data("fn_shown");
        c && c()
    }).tabs("select", 0);
    $("#GuestTableContainer").jtable({model: $("#model-guests"), className: "stat"}).jtable("addcol", Msg.Name_MACAddress, "mac", true, function (a, b) {
        var c = b.name || b.hostname || a;
        return c == a ? c : c + " [" + a + "]"
    }).jtable("addcol", "Package", "package").jtable("addcol",
        "Amount", "amount", true, Renderer.amount).jtable("addcol", "Authorized By", "authorized_by", true, function (a, b) {
            if (a == "none")return"None"; else if (a == "password")return"Password"; else if (a == "api")return"API"; else if (a == "voucher")return"Voucher"; else if (a == "credit")return b.payment_cardtype || "Credit Card"; else if (a == "paypal")return"PayPal"
        }).jtable("addcol", "Download", "tx_bytes", true, Renderer.pktsBytes).jtable("addcol", "Upload", "rx_bytes", true, Renderer.pktsBytes).jtable("addcol", "Status", "end", true, function (a, b) {
            if (b.expired)return"Expired"; else {
                var c = parseInt(a) * 1E3;
                (new Date).getTime();
                return"Valid until " + Renderer.timestamp(c)
            }
        }).jtable("addaction", "Disconnect", "terminate", function (a) {
            return!a.expired
        }).jtable("addaction", "Extend", "extend").bind("jtable_terminate", function (a, b) {
            $(a.originalTarget).disable();
            $.jcmd("hotspot", "terminate", {_id: $(b).attr("_id")}, function () {
                $("#GuestTableContainer").jtable("refresh")
            })
        }).bind("jtable_extend", function (a, b) {
            $(a.originalTarget).disable();
            $.jcmd("hotspot", "extend",
                {_id: $(b).attr("_id")}, function () {
                    $("#GuestTableContainer").jtable("refresh")
                })
        });
    $("#GuestTableSearch").jsearch().bind("jsearch_search", function () {
        var a = $("#GuestTableSearch").val();
        $("#GuestTableContainer").jtable("addfilter", "*", a == "" ? null : a).jtable("filter")
    });
    $("#PaymentTableContainer").jtable({model: $("#model-payments"), className: "stat"}).jtable("addcol", "Time", "time", true, Renderer.timestamp).jtable("addcol", "Last Name", "last_name", true).jtable("addcol", "First Name", "first_name", true).jtable("addcol",
        "Package", "package").jtable("addcol", "Amount", "amount", true, Renderer.amount).jtable("addcol", "Extra Info", "info").jtable("addcol", "Status", "status", true, function (a) {
            if (a == "paid")return"Paid"; else if (a == "void")return"Voided/Refunded"
        }).jtable("addaction", "Refund", "refund", function (a) {
            return"void" != a.status
        }).bind("jtable_refund", function (a, b) {
            $(a.originalTarget).disable();
            $.jcmd("hotspot", "refund", {_id: $(b).attr("_id")}, function () {
                $("#PaymentTableContainer").jtable("refresh")
            })
        });
    $("#PaymentTableSearch").jsearch().bind("jsearch_search",
        function () {
            var a = $("#PaymentTableSearch").val();
            $("#PaymentTableContainer").jtable("addfilter", "*", a == "" ? null : a).jtable("filter")
        });
    var d = $("#Vouchers form.create");
    d.find("select[name=expire]").change(function () {
        d.find("span.customExpire").toggle(d.find('[name="expire"]').val() == "custom")
    });
    d.find("select[name=expire]").val("1440").change();
    d.find("input[name=create]").click(function (a) {
        if (V.checkMinMax(d.find("input[name=n]"), 1, 3E3, "Number of vouchers"))if (!(d.find('select[name="expire"]').val() ==
            "custom" && !V.checkMinMax(d.find('input[name="expireNumber"]'), 1, 1E3, "Expire Time"))) {
            $(a.originalTarget).disable();
            $.jcmd("hotspot", "create-voucher", {n: d.find("input[name=n]").val(), quota: d.find("select[name=quota]").val(), expire: d.find('select[name="expire"]').val(), expire_number: d.find('input[name="expireNumber"]').val(), expire_unit: d.find('select[name="expireUnit"]').val(), note: d.find("input[name=note]").val()}, function () {
                $(a.originalTarget).enable();
                $("#model-vouchers").jtablemodel("refresh")
            })
        }
    });
    d.find("input[name=doNote]").change(function () {
        d.find("span.doNote").toggle(d.find("input[name=doNote]").checked())
    });
    $("#VoucherTableContainer").jtable({model: $("#model-vouchers"), className: "stat"}).jtable("addcol", "Code", "code", true, function (a) {
        return a.substring(0, 5) + "-" + a.substring(5)
    }).jtable("addcol", "Create Time", "create_time", true, Renderer.timestamp_sec).jtable("addcol", "Note", "note").jtable("addcol", "Duration", "duration", true, function (a) {
        return Renderer.uptime(a * 60)
    }).jtable("addcol", "Status",
        "used", true, function (a, b) {
            var c = b.end_time - (new Date).getTime() / 1E3;
            return b.used > 0 ? c > 0 ? "Used" + (b.quota != 1 ? " " + b.used + " time(s)" : "") + ", expires in " + Renderer.uptime(c) : "Expired" : "Valid" + (b.quota != 1 ? " for multi-use" : " for one-time use")
        }).jtable("addaction", "Revoke", "delete", function (a) {
            return a.valid != false
        }).jtable("addaction", "Print batch", "print", function (a) {
            return a.valid != false
        }).bind("jtable_delete", function (a, b) {
            $(a.originalTarget).disable();
            $.jcmd("hotspot", "delete-voucher", {_id: b._id}, function () {
                $(a.originalTarget).enable();
                $("#model-vouchers").jtablemodel("refresh")
            })
        }).bind("jtable_print", function (a, b) {
            window.open("/print/hotspot/vouchers?create_time=" + b.create_time, "print_voucher")
        });
    $("#Vouchers input[name=printall]").click(function () {
        window.open("/print/hotspot/vouchers", "print_voucher")
    });
    $("#VoucherTableSearch").jsearch().bind("jsearch_search", function () {
        var a = $("#VoucherTableSearch").val();
        $("#VoucherTableContainer").jtable("addfilter", "*", a == "" ? null : a).jtable("filter")
    });
    if (f) {
        $("#OperatorTableContainer").jtable({model: $("#model-operators"),
            className: "stat"}).jtable("addcol", "Name", "name", true).jtable("addcol", "Password", "x_password", true).jtable("addcol", "Notes", "note", true).jtable("addaction", "Delete", "delete").bind("jtable_delete", function (a, b) {
            confirm("Are you sure you want to delete the object?") && $.jpost("/del/admin/" + b._id, {}, function () {
                $("#model-operators").jtablemodel("refresh")
            })
        });
        var e = $("#Operators form.create");
        e.find("input[name=create]").japply({validate: function () {
            if (!V.checkAdminName(e.find("input[name=name]"), "Account"))return false;
            if (!V.checkAdminPasswords(e.find("input[name=password]"), e.find("input[name=password]"), "Password", "Confirm"))return false;
            return true
        }, get_posts: function () {
            return{url: "/add/admin", payload: {name: e.find("input[name=name]").val(), x_password: e.find("input[name=password]").val(), note: e.find("input[name=note]").val(), for_hotspot: true}}
        }, done: function () {
            $("#model-operators").jtablemodel("refresh")
        }});
        $("#OperatorTableSearch").jsearch().bind("jsearch_search", function () {
            var a = $("#OperatorTableSearch").val();
            $("#OperatorTableContainer").jtable("addfilter", "*", a == "" ? null : a).jtable("filter")
        })
    }
    $("#model-guests").jtablemodel("refresh");
    $("#model-payments").jtablemodel("refresh");
    $("#model-vouchers").jtablemodel("refresh");
    f && $("#model-operators").jtablemodel("refresh");
    $("#RefreshSelect").jtimer().bind("jtimer_due", function () {
        $("#model-guests").jtablemodel("refresh");
        $("#model-payments").jtablemodel("refresh");
        $("#model-vouchers").jtablemodel("refresh");
        f && $("#model-operators").jtablemodel("refresh")
    }).jtimer("start");
    $("#RefreshControl a, .system-dialog a").click(function () {
        $("#RefreshSelect").jtimer("trigger");
        return false
    });
    $("span.label").live("click", function () {
        var a = $(this).prev();
        if (a.is(".radio-input") || a.is(".check-box-input"))
            a.click().change()
    });
    var h = $(".system-dialog");
    h.dialog({autoOpen: false, modal: true, draggable: false, resizable: false, closeOnEscape: false, width: 400, height: 250, closeText: "", dialogClass: "system-warning-dialog"});
    var g = false;
    $.jpost.fnReflectConnectionStatus = function (a) {
        if (!$.jpost.session_expired)if (!a && !g) {
            g = true;
            //h.dialog("open");
        } else if (g && a) {
            g = false;
            h.dialog("close")
        }
    }
});
