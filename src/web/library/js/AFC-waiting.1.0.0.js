$(function () {
	function updateProgress(sMsg, progress) 
    {  
    	if(progress > 100)
    	{
    		progress = 100;
    	}
        $("#status").innerHTML = sMsg; 
        $("#progress").css("width", Math.round(progress*3) + "px"); 
    }
	var drawprogress = function(total, passed)
	{
		var ii = 0
		var progress = "[";
		passed = passed || 0;
		if(total)
		{
			if(passed > total)
			{
				progress += (1*100).toFixed(2)+"%";
			}
			else
			{
				progress += ((passed/total)*100).toFixed(2)+"%";
			}
		}
		else
		{
			progress += "0.00%";
		}
		progress += "]";
		return progress;
		//return "";
	};
    var c = true,
    //b = location.href.substring(location.href.lastIndexOf("/") + 1);
    b = $("#waiting_cmd").val();
	var percent = $("#percent").val();
	var points = new Array(".", "..", "...", "....", ".....");
    //alert(b);
    var e = {upgrade: {head: Msg.I_BeingUpgraded, 
							step1: Msg.I_UpgradingDownloading, 
							step2: Msg.I_UpgradingLoading
					}, 
			restore: {head: Msg.I_BeingRestored, 
							step1: Msg.I_RestoringUploading,
							step2: Msg.I_RestoringLoading
					}
    };
	var msg = {upgrade: {
					download_failed: Msg.E_UpgradeDownloadFailed, 
					url_parse_failed: Msg.E_UpgradeURLParseFailed, 
					version_load_failed: Msg.E_UpgradeLoadFailed,
					upgrade_failed: Msg.E_UpgradeLoadFailed,
					upgraded: Msg.I_UpgradedSuccess
					}, 
				restore: {
					upload_failed: Msg.E_RestoringUploadFailed, 
					restore_failed: Msg.E_RestoringLoadFailed
					}
				};
	if(percent)
	{
		var date = new Date();
		var i = (date.getSeconds())%(points.length);
    	$("div.progress").html(e[b].step2+drawprogress( 100, parseFloat(percent))+points[i]);
	}
	else
	{
    	$("div.progress").html(e[b].step1);
    }
	f = function () {
		if(c)
		{
			//setTimeout(f, 1E3);
			c = false;
			//return ;
		}
		if(b == "upgrade")
		{
			var wait = 1;
	        $.post("/status.php", {cmd: "upgrade"}, function (a, g, h) {//alert($.toJSON(h));
	            //alert("h "+h.status + " "+a.meta.up);
	            var over = false;				
	            if(h.status == 200 && a && a.meta && a.meta.upgrade_status)
	            {
	            	if(a.meta.upgrade_status == "upgraded") 
	            	{
	            		var finish = "";
						var date = new Date();
						var i = (date.getSeconds())%(points.length);
						if(a.meta && a.meta.totalsecond)
						{
							finish = drawprogress(a.meta.totalsecond, a.meta.totalsecond);
						}
						$("div.progress").html(e[b].step2+finish+points[i]);
						updateProgress("", 100);
	            		alert(msg[b][a.meta.upgrade_status]);
	            		over = true;
	            	}
					else if((a.meta.upgrade_status == "download_failed")
						|| (a.meta.upgrade_status == "url_parse_failed"))
	            	{
	            		alert(msg[b][a.meta.upgrade_status]);
	            		over = true;
	            	}
					else if((a.meta.upgrade_status == "downloaded")
						|| (a.meta.upgrade_status == "upgrade_failed"))
					{
						wait = 1;
						var progress = "";
						var finish = "";
						var date = new Date();
						var i = (date.getSeconds())%(points.length);
						if(a.meta && a.meta.totalsecond)
						{
							finish = drawprogress(a.meta.totalsecond, a.meta.passedsecond);
							//wait = a.meta.passedsecond;
							updateProgress("", (a.meta.passedsecond/a.meta.totalsecond)*100);
						}
						$("div.progress").html(e[b].step2+finish+points[i]);
						if(0 == a.meta.totalsecond || (a.meta.passedsecond > a.meta.totalsecond+3)
							|| (a.meta.upgrade_status == "upgrade_failed"))
						{
							alert(msg[b]["version_load_failed"]);
	            			over = true;
						}
					}
					else
					{
						if(DEBUG_ENABLED)
						{
							
							a.meta.upgrade_status && alert("else upgrade_status "+a.meta.upgrade_status);
						}
						if(a.meta.upgrade_status == "downloading")
						{
							var date = new Date();
							var i = (date.getSeconds())%(points.length);
							$("div.progress").html(e[b].step1+points[i]);
						}
						else if(0 == a.meta.totalsecond || (a.meta.passedsecond > a.meta.totalsecond+3))
						{
							alert(msg[b]["upgrade_failed"]);
	                                                over = true;
						}
					}
		            if (over){location.href = "/index.php";}
		            else {setTimeout(f, wait*1E3);}
	            }
				else
				{
					if(DEBUG_ENABLED)
					{
						//alert("h.status "+h.status);
					}
					if(h.status == 200)
					{
						alert(msg[b]["upgrade_failed"]);
		            	location.href = "/index.php";
					}
					else
					{
		            	setTimeout(f, 1E3);
					}
				}
	        }, 'json');
			
		}
		else if(b == "restore")
		{
	        $.post("/status.php", {cmd: "restore"}, function (a, g, h) {
				
	            var over = false;
	            if(h.status == 200 && a && a.meta && a.meta.restore_status)
	            {
	            	if(a.meta.restore_status == "restored") 
	            	{
	            		over = true;
	            	}
					if((a.meta.restore_status == "upload_failed")
						|| (a.meta.restore_status == "restore_failed"))
	            	{
	            		alert(msg[b][a.meta.restore_status]);
	            		over = true;
	            	}
					else if(a.meta.restore_status == "uploaded")
					{
						$("div.progress").html(e[b].step2);
					}
		            if (over){location.href = "/index.php"; }
		            else setTimeout(f, 1E3)
	            }
				else
				{
					if(DEBUG_ENABLED)
					{
						//alert("h.status "+h.status);
					}
					setTimeout(f, 5E3);
				}
	        }, 'json');
		}
		else
		{
			location.href = "/index.php"; 
		}
    };
    $.jpost.fnReflectConnectionStatus = function (a) {//alert("a"+a);
       //a || (c && f());
    };
    $("h2.header").html(e[b].head);
    setTimeout(function () {
        $.jcmd("system", b, {}, function () {//alert(d);
            c && f()
        })
    }, 1E3)
});
