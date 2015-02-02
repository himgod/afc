TotalInfo=new Array();
DeTotalInfo=new Array();
NWTotalInfo=new Array();
NWDeTotalInfo=new Array();
NWLastDownFlow=new Array();
NWLastUpFlow=new Array();
APLastDownFlow=new Array();
APLastUpFlow=new Array();
KeyFlowUpB=0,KeyFlowUpP=0,KeyFlowDownB=0,KeyFlowDownP=0;
lastDelay=0;
afidata=[0],wlandata=[0];
$(function(){
	$(".header-nav").tabs();
	});

	$(".navigation-tabs li").click(function() {
		//for(var i = 1; i <= $(".navigation-tabs li").length; i++){
			$(".index-navi-active").removeClass('index-navi-active');
			//$('#'+i).find('a').css("color","#333");
		//}
		var id = $(this).attr('id');
		$(this).addClass('index-navi-active');
		//$(this).find('a').css("color","#fff");
	});
$(document).ready(function() {
		$(".system-page .header-nav-ul li").first().attr('id','choose-button');
		
		$('.overview3 .business-button-position li').first().click(function(){
			$('.overview3 .business-button-position li').first().attr('id','');
		   $('.overview3 .business-button-position li').last().attr('id','');
		   $(this).attr('id','choose-button');	
		   $("#AccessPoint").css("display","block");
		   $("#NetworkSignal").css("display","none");
		   
		   $(window).scrollTop(1140);
		  
		});
		$('.overview3 .business-button-position li').last().click(function(){
			$('.overview3 .business-button-position li').first().attr('id','');
		   $('.overview3 .business-button-position li').last().attr('id','');
		   $(this).attr('id','choose-button');	
		   
		   $("#AccessPoint").css("display","none");
		   $("#NetworkSignal").css("display","block");
		   $(window).scrollTop(1140);
		  
		});
	 	$(".system-page .system-nav-ul li").click(function() {		
		   $(".system-page .system-nav-ul li").first().attr('id','');
		   $(".system-page .system-nav-ul li").first().next().attr('id','');
		   $(".system-page .system-nav-ul li").first().next().next().attr('id','');
		   $(".system-page .system-nav-ul li").last().attr('id','');
		   $(this).attr('id','choose-button');			
		});
		$("#Support .support-nav-ul li").click(function() {		
		   $("#Support .support-nav-ul li").first().attr('id','');
		   $("#Support .support-nav-ul li").first().next().attr('id','');
		   $(this).attr('id','choose-button');			
		});
		$("#App .app-nal-ul li").click(function() {		
		   $("#App .app-nal-ul li").first().attr('id','');
		   $("#App .app-nal-ul li").first().next().attr('id','');
		   $(this).attr('id','choose-button');			
		});
		$('#overlays').draggable();				
        });
		
$(function(){
			$(".history-close").click(function(){
				$(".history-page").css("display","none");
				$(".overview-cover").hide();
			});
			$("#system-health-viewHistory").click(function(){
				
						
			$(".history-page").css("display","block");
			});
			$('#viewHistory2-1').click(function(){
				 changeClass($('.time-button-position li').first());
				 drawCurveGraph("../../api/history/wifi_history_list.php",'#chart1','','0','');
				 $('.time-button-position li').unbind('click');
				$('.time-button-position li').click(function(){
					drawChart(this,'wifi_history_list.php');
				});
				$(".history-page").css("display","block");
				$(".overview-cover").show();
			});
			$('#viewHistory2-2').click(function(){
				changeClass($('.time-button-position li').first());
				 drawCurveGraph("../../api/history/terminal_history_list.php",'#chart1','','0','');
				 $('.time-button-position li').unbind('click');
				$('.time-button-position li').click(function(){
					drawChart(this,'terminal_history_list.php');
				});
				$(".history-page").css("display","block");
				$(".overview-cover").show();
			});
			$('#viewHistory2-3').click(function(){
				changeClass($('.time-button-position li').first());
				 drawCurveGraph("../../api/history/ap_history_list.php",'#chart1','','0','');
				$('.time-button-position li').unbind('click');
				$('.time-button-position li').click(function(){
					drawChart(this,'ap_history_list.php');
				});
			$(".history-page").css("display","block");
			$(".overview-cover").show();
			});
			$('#viewHistory2-4').click(function(){
				changeClass($('.time-button-position li').first());
				 drawCurveGraph("../../api/history/flow_history_list.php",'#chart1','','0','');
				 $('.time-button-position li').unbind('click');
				$('.time-button-position li').click(function(){
					drawChart(this,'flow_history_list.php');
				});
				$(".history-page").css("display","block");
				$(".overview-cover").show();
			});
				$('#viewHistory2').click(function(){
				changeClass($('.time-button-position li').first());
				 drawCurveGraph("../../api/history/wireless_history_list.php",'#chart1','','0','test');
				 $('.time-button-position li').unbind('click');
				$('.time-button-position li').click(function(){
					drawChart(this,'wireless_history_list.php');
				});
				$(".history-page").css("display","block");
				$(".overview-cover").show();
			});
			});
			
$(function() {	
		$('#QrCode a.custom-width').cluetip({width: '100px', showTitle: false, cluetipClass: 'cluetip'});
		AjaxPost();
		$("#RefreshSelect").bind("jtimer_due",AjaxPost);		
	    $("#downbutton").click(function(){
		  var othersup=$("#othersupport").css('display');
		  if( othersup=="none") {$("#othersupport").show();  $(this).attr("class","support-up");}
		  else {$("#othersupport").hide();  $(this).attr("class","support-down");}
		});
		$('#installed .install-APP .app-radio-style').click(function(){
		   var appc=$(this).attr("class");
		   if(appc=="app-radio-style radio-unselect")
		   $(this).attr("class","app-radio-style radio-select");
		   else 	$(this).attr("class","app-radio-style radio-unselect");
		});

		//清空表单内容problemFeedback-tab-content.php(不用加id，直接用class)
        //提交反馈，数据写入数据库？？
		$(".feedbackButton").children().click(function(){
			$("#othersupport").find('input[type="checkbox"]').attr("checked",false);
			$("#othersupport").find("textarea").val("");
			$("#othersupport").find("input").val("");
		});	
		
			
 });
 function AjaxPost()
 {
	var tempdata,healthydata;
	 	$.ajax({ 
				async: true, 
				type : "POST", 
				url : "/api/realtime_data.php", 
				dataType : 'json', 
				success : function(showdata) { 
					tempdata=showdata['key_index'];
					healthydata=showdata['sys_healthy'];
					afidata=showdata['wireless']['wireless_as_afi'];
					wlandata=showdata['wireless']['wireless_as_wlan'];
					drawPolar("#scoreOverwiewContainer",healthydata );
					showKeyData(tempdata);
					if(afidata!="") {
						showOptionAP(afidata);
						showWirelessAPData(afidata,0);
					}
					if(wlandata!="") {
						showOptionNW(wlandata);
						//var wlanIndex=$("#en-select-AFI select").find("option[selected='selected']").index();
						showWirelessNWData(wlandata,0);}
			}
		});
	
 }
// overview page key data showing
 function showKeyData(tempdata){
	 var data,attrname;
	 		if(tempdata['signal']!=""){
	 			$("#signal1-1").html(calDif('signal','wifi_snr',0,'#signal1',tempdata));
				$("#signal1").html(tempdata['signal']['wifi_snr']);
				$("#signal1").bind("click mouseover",function(){
					data = tempdata['signal']['snr_list'];
					attrname = [">70dBm:","50~70dBm:","30~50dBm:","<30dBm:"];
					createTable(data, attrname, "#signal1-info", "110px","%");
				});
				if(tempdata['signal']['chan_util']!=""){
				$("#signal2-1").html(calDif('signal','chan_util','ng','#signal2',tempdata));
				$("#signal2").html(tempdata['signal']['chan_util']['ng']);
				$("#signal2").bind("click mouseover",function(){
					data = tempdata['signal']['chan_util_list']['ng'];
					//attrname = [Msg.Channel+'1:',Msg.Channel+'6:',Msg.Channel+'11:'];
					createTableD(data, Msg.Channel,"", "#signal2-info", "100px","%");
					});
				$("#signal3-1").html(calDif('signal','chan_util','na','#signal3',tempdata));
				$("#signal3").html(tempdata['signal']['chan_util']['na']);
				$("#signal3").bind("click mouseover",function(){
					data = tempdata['signal']['chan_util_list']['na'];
					if(data.length<5)  attrname = [Msg.Channel+"36:",Msg.Channel+"40:",Msg.Channel+"44:",Msg.Channel+"48:"];
					else attrname = [Msg.Channel+"36:",Msg.Channel+"40:",Msg.Channel+"44:",Msg.Channel+"48:",Msg.Other+""];
					createTable(data, attrname, "#signal3-info", "100px","%");
				});
				}
				$("#signal4-1").html(calDif('signal','disturb_avr',0,'#signal4',tempdata));
				$("#signal4").html(tempdata['signal']['disturb_avr']);
				$("#signal4").bind("click mouseover",function(){
					data = tempdata['signal']['disturb_list'];
					attrname = ["802.11:",Msg.Signal_disturb_blue+":",Msg.Signal_disturb_oven+":","802.11:","802.11:"];
					createTable(data, attrname, "#signal4-info", "140px","dBm");
				});
			}
				if(tempdata['terminal']!="")  {
				$("#terminal1-1").html(calDif('terminal','signal_intens',0,'#terminal1',tempdata));
				$("#terminal1").html(tempdata['terminal']['signal_intens']);
				$("#terminal1").bind("click mouseover",function(){
					data = tempdata['terminal']['intens_list'];
					attrname = [">70db:","50~70db:","30~50db:","<30db:",Msg.Other+":"];
					createTable(data, attrname, "#terminal1-info", "120px","%");
				});
					if(tempdata['terminal']["access_rate"]!=""){
						$("#terminal2-1").html(calDif('terminal','access_rate','Tx','#terminal2',tempdata));
						$("#terminal2").html(tempdata['terminal']['access_rate']['Tx']);
						$("#terminal2").bind("click mouseover",function(){
							data = tempdata['terminal']['access_rate_list']['Tx'];
							createTableD(data,"","Mbps", "#terminal2-info", "100px","%");
						});
						$("#terminal3-1").html(calDif('terminal','access_rate','Rx','#terminal3',tempdata));
						$("#terminal3").html(tempdata['terminal']['access_rate']['Rx']);
						$("#terminal3").bind("click mouseover",function(){
							data = tempdata['terminal']['access_rate_list']['Rx'];
							//attrname = ["Rx:bps:","Rx:6Mbps:","Rx:11Mbps:","Rx:18Mbps:","Rx:"+Msg.Other+":"];
							createTableD(data,"","Mbps", "#terminal3-info", "100px","%");
						});
					}
					if(tempdata['terminal']["rtry_trans_rate"]!=""){
				$("#terminal4-1").html(calDif('terminal','rtry_trans_rate','Tx','#terminal4',tempdata));
				$("#terminal4").html(tempdata['terminal']['rtry_trans_rate']['Tx']);
				$("#terminal4").bind("click mouseover",function(){
					data = tempdata['terminal']['rtry_trans_list']['Tx'];
					attrname = [">80:","60~80:","30~60:","<30:",Msg.Other+":"];
					createTable(data, attrname, "#terminal4-info", "100px","%");
				});
				$("#terminal5-1").html(calDif('terminal','rtry_trans_rate','Rx','#terminal5',tempdata));
				$("#terminal5").html(tempdata['terminal']['rtry_trans_rate']['Rx']);
				$("#terminal5").bind("click mouseover",function(){
					data = tempdata['terminal']['rtry_trans_list']['Rx'];
					attrname = [">80:","60~80:","30~60:","<30:",Msg.Other+":"];
					createTable(data, attrname, "#terminal5-info", "100px","%");
				});
					}
				}
				if(tempdata['access']!=""){
					var attrname_access=new Array();
					for(var i=0;i<tempdata['access']['afi_name'].length;i++) {attrname_access.push(tempdata['access']['afi_name'][i]+":");}
					if(tempdata['access']['bw']!=""){
				$("#ap1-1").html(calDif('access','bw','up','#ap1',tempdata));
				$("#ap1").html(tempdata['access']['bw']['up']);
				$("#ap1").bind("click mouseover",function(){
					data = tempdata['access']['bw_list']['up'];
					createTable(data, attrname_access, "#ap1-info", "180px","Mbps");
				});
				$("#ap2-1").html(calDif('access','bw','down','#ap2',tempdata));
				$("#ap2").html(tempdata['access']['bw']['down']);
				$("#ap2").bind("click mouseover",function(){
					data = tempdata['access']['bw_list']['down'];
					createTable(data, attrname_access, "#ap2-info", "180px","Mbps");
				});
					}
				
				$("#ap3").html(tempdata['access']['delay_tm']);
				$("#ap3-1").html(calDifF('access','delay_tm',0,lastDelay,tempdata,'#ap3'));
				lastDelay = tempdata['access']['delay_tm'];
				
				$("#ap3").bind("click mouseover",function(){
					data = tempdata['access']['delay_tm_list'];
					createTable(data, attrname_access, "#ap3-info", "180px","ms");
				});
				}
				if(tempdata['flow']!=""){
					var attrname_flow=new Array();
					var afi_Len = tempdata['flow']['afi_name'].length;
					for(var i=0;i<afi_Len;i++) {attrname_flow.push(tempdata['flow']['afi_name'][i]+":");}
					if(tempdata['flow']['bytes']!=""){		
						$("#flow1-1").html(calDifF('flow','bytes','up',KeyFlowUpB,tempdata,'#flow1'));
						$("#flow1").html(dealBytes(tempdata['flow']['bytes']['up'],"#unitflow_1"));
						$("#flow1").bind("click mouseover",function(){
							data = tempdata['flow']['bytes_list']['up'];
							deal_data=new Array();
							for(var k=0;k<afi_Len;k++) {deal_data.push(dealBytes(data[k],""));}
							createTable(deal_data,attrname_flow, "#flow1-info", "150px","");
						});
						KeyFlowUpB=tempdata['flow']['bytes']['up'];
						$("#flow3-1").html(calDifF('flow','bytes','down',KeyFlowDownB,tempdata,'#flow3'));
						$("#flow3").html(dealBytes(tempdata['flow']['bytes']['down'],"#unitflow_2"));
						$("#flow3").bind("click mouseover",function(){
							data = tempdata['flow']['bytes_list']['down'];
							deal_data=new Array();
							for(var k=0;k<afi_Len;k++) {deal_data.push(dealBytes(data[k],""));}
							createTable(deal_data, attrname_flow, "#flow3-info", "140px","");
						});
						KeyFlowDownB=tempdata['flow']['bytes']['down'];
					}
					if(tempdata['flow']['packets']!=""){
						$("#flow2-1").html(calDif('flow','packets','up','#flow2',tempdata));
						$("#flow2").html(tempdata['flow']['packets']['up']);
						$("#flow2").bind("click mouseover",function(){
							data = tempdata['flow']['packets_list']['up'];	
							createTable(data,attrname_flow, "#flow2-info", "140px","");
						});
						$("#flow4-1").html(calDif('flow','packets','down','#flow4',tempdata));
						$("#flow4").html(tempdata['flow']['packets']['down']);
						$("#flow4").bind("click mouseover",function(){
							data = tempdata['flow']['packets_list']['down'];
							createTable(data,attrname_flow, "#flow4-info", "140px","");
						});	
					}
				}
				
				//} 
			//	});
						
 }
 function showOptionAP(dataAP)
{
	$("#AccessPoint #en-select-AFI2 select").empty();
	if(dataAP!=""){
	$("#AccessPoint #en-select-AFI2 select").append($('<option  value="'+dataAP[0]['afi_name']+'">'+dataAP[0]['afi_name']+'</option>'));
	for(var i=1;i<dataAP.length;i++)
		$("#AccessPoint #en-select-AFI2 select").append($('<option value="'+dataAP[i]['afi_name']+'">'+dataAP[i]['afi_name']+'</option>'));
	}
	$("#en-select-AFI2 select option").click(function(){
			var rapIndex=$(this).index();
			showWirelessAPData(afidata,rapIndex);
	});	
}
 function showOptionNW(dataNW)
 {
	 $("#NetworkSignal #en-select-AFI select").empty();
	 if(dataNW!=""){
	$("#NetworkSignal #en-select-AFI select").append($('<option  value="'+dataNW[0]['wlan_name']+'">'+dataNW[0]['wlan_name']+'</option>'));
	for(var j=1;j<dataNW.length;j++)
		$("#NetworkSignal #en-select-AFI select").append($('<option value="'+dataNW[j]['wlan_name']+'">'+dataNW[j]['wlan_name']+'</option>'));
	}
	$("#en-select-AFI select option").click(function(){
		var rwlanIndex=$(this).index();
		showWirelessNWData(wlandata,rwlanIndex);
	});	
 }
 function showWirelessAPData(data,index){
	 var tempdata=data[index];
		   var viewHistoryText=$('#AccessPoint #viewHistory3').html();
		   var networkData=$('#AccessPoint .data3').find('p').first();
		   var numUsersText=networkData.find('span').first().text();
		   var uplinkTextB=$("#wireless_ap2-info").prev().html();
		   var uplinkTextP=$("#wireless_ap3-info").prev().html();
		   var downlinkTextB=$("#wireless_ap4-info").prev().html();
		   var downlinkTextP=$("#wireless_ap5-info").prev().html();
		   var c=$("#AccessPoint .singal-data");
		   c.empty();
		   if(tempdata!=""){

		   if(tempdata['total_info']!=""){
   	 var APUpFlow=tempdata['total_info']['up_flow'];
   	 var APDownFlow=tempdata['total_info']['down_flow'];
	 TotalInfo[index] = [tempdata['total_info']['usr_nums'], APUpFlow['bytes'],APUpFlow['packets'],APDownFlow['bytes'],APDownFlow['packets']];
	 
	 $("#wireless_ap1-1").html(calDifWire('#wireless_ap1',TotalInfo[index],0));
	 $("#wireless_ap1").html(tempdata['total_info']['usr_nums']);	
	
	 
	 $("#wireless_ap2-1").html(calDifAP(TotalInfo[index][1],APLastUpFlow,index,'#wireless_ap2'));
	 $("#wireless_ap2").html(dealBytes(APUpFlow['bytes'],"#wireless_ap2_unit"));
	 APLastUpFlow[index]=APUpFlow['bytes'];
	
	 $("#wireless_ap4-1").html(calDifAP(TotalInfo[index][3],APLastDownFlow,index,'#wireless_ap4'));
	 $("#wireless_ap4").html(dealBytes(APDownFlow['bytes'],"#wireless_ap4_unit"));
	 APLastDownFlow[index]=APDownFlow['bytes'];
	 
	 $("#wireless_ap3-1").html(calDifWire('#wireless_ap3',TotalInfo[index],2));
	 $("#wireless_ap3").html(APUpFlow['packets']);
	
	 $("#wireless_ap5-1").html(calDifWire('#wireless_ap5',TotalInfo[index],4));
	 $("#wireless_ap5").html(APDownFlow['packets']);
	 
	
	 
	 for(var i=0;i<tempdata['wlan_info'].length;i++)
	 {
		
		 c.append($('<div class="indicator"><div class="total-indicator-pic-little"><a class="moreinfo" id="APInfo_'+i+'"><span>'+decideLen(tempdata['wlan_info'][i]['wlan_name'])+'</span></a></div><div class="data data4 data_'+i+'"></div><a id="viewHistory_'+i+'" class="view-history view-history4">'+viewHistoryText+'</a></div>'));
		 captureStr(tempdata['wlan_info'][i]['wlan_name'],"#APInfo_"+i,"140px");
		 c.find(".data_"+i).append($('<p><span>'+ numUsersText+'</span><a class="usernum" id="APUserInfo_'+i+'"><span id="numUsers_'+i+'"></span></a><span class="moreinfo" id="numUsers_'+i+'-1"></span></p>'));
		
		 c.find(".data_"+i).append($('<p><span>'+ uplinkTextB+'</span><span class="moreinfo"id="upFlowB_'+i+'"></span><span id="upFlowB_'+i+'_unit"></span><span id="upFlowB_'+i+'-1"></span><br/>'+uplinkTextP+'<span class="moreinfo" id="upFlowP_'+i+'"></span><span id="upFlowP_'+i+'-1"></span></p>'));
		 c.find(".data_"+i).append($('<p><span>'+ downlinkTextB+'</span><span class="moreinfo" id="downFlowB_'+i+'"></span><span id="downFlowB_'+i+'_unit"></span><span id="downFlowB_'+i+'-1"></span><br/>'+downlinkTextP+'<span class="moreinfo" id="downFlowP_'+i+'"></span><span id="downFlowP_'+i+'-1"></span></p>'));
		var APWlan_info=tempdata['wlan_info'][i];
		DeTotalInfo[i]=[APWlan_info['usr_num'],APWlan_info['up_bytes'],APWlan_info['up_packets'],APWlan_info['down_bytes'],APWlan_info['down_packets']];	
	 $("#numUsers_"+i+"-1").html(calDifWire("#numUsers_"+i,DeTotalInfo[i],0));
	 $("#numUsers_"+i).html(APWlan_info['usr_num']);
	 if(APWlan_info['usr_list']!="")  {userCluetip(APWlan_info['usr_list'],"#APUserInfo_"+i, "100px");}
	
	 $("#upFlowB_"+i+"-1").html(calDifWireB(DeTotalInfo[i],1,"#upFlowB_"+i,1));
	 $("#upFlowB_"+i).html(dealBytes(APWlan_info['up_bytes'],'#upFlowB_'+i+'_unit'));
	 
	 $("#upFlowP_"+i+"-1").html(calDifWire("#upFlowP_"+i,DeTotalInfo[i],2));
	 $("#upFlowP_"+i).html(APWlan_info['up_packets']);
	
	 $("#downFlowB_"+i+"-1").html(calDifWireB(DeTotalInfo[i],3,"#downFlowB_"+i),1);
	 $("#downFlowB_"+i).html(dealBytes(APWlan_info['down_bytes'],'#downFlowB_'+i+'_unit'));
	 
	 $("#downFlowP_"+i+"-1").html(calDifWire("#downFlowP_"+i,DeTotalInfo[i],4));
	 $("#downFlowP_"+i).html(APWlan_info['down_packets']);

	  }
	}
}
 } 
 function showWirelessNWData(data,index)
 {
	 var tempData=data[index];
	 	   var viewHistoryText=$('#AccessPoint #viewHistory3').html();
		   var networkData=$('#AccessPoint .data3').find('p').first();
		   var numUsersText=networkData.find('span').first().text();
		   var uplinkTextB=$("#wireless_ap2-info").prev().html();
		   var uplinkTextP=$("#wireless_ap3-info").prev().html();
		   var downlinkTextB=$("#wireless_ap4-info").prev().html();
		   var downlinkTextP=$("#wireless_ap5-info").prev().html();
		   var c=$("#NetworkSignal .singal-data");
		   c.empty();
		   if(tempData!="")	{	
  
	 if(tempData['total_info']!=""){
	  var NWUpFlow=tempData['total_info']['up_flow'];
	  var NWDownFlow=tempData['total_info']['down_flow'];
	  NWTotalInfo[index]=[tempData['total_info']['usr_nums'],NWUpFlow['bytes'],NWUpFlow['packets'],NWDownFlow['bytes'],NWDownFlow['packets']];
	 $("#wireless_nw1-1").html(calDifWire('#wireless_nw1',NWTotalInfo[index],0));
	 $("#wireless_nw1").html(tempData['total_info']['usr_nums']);	 
	 	
	
	 
	 $("#wireless_nw2-1").html(calDifAP(NWTotalInfo[index][1],NWLastUpFlow,index,'#wireless_nw2'));
	 $("#wireless_nw2").html(dealBytes(NWUpFlow['bytes'],"#wireless_nw2_unit"));
	 NWLastUpFlow[index]=NWUpFlow['bytes'];
	
	 $("#wireless_nw3-1").html(calDifWire('#wireless_nw3',NWTotalInfo[index],2));
	 $("#wireless_nw3").html(tempData['total_info']['up_flow']['packets']);
	 
	 
	 
	 $("#wireless_nw4-1").html(calDifAP(NWTotalInfo[index][3],NWLastDownFlow,index,'#wireless_nw4'));
	 $("#wireless_nw4").html(dealBytes(NWDownFlow['bytes'],"#wireless_nw4_unit"));
	 NWLastDownFlow[index]=NWDownFlow['bytes'];
	 
	 $("#wireless_nw5-1").html(calDifWire('#wireless_nw5',NWTotalInfo[index],4));
	 $("#wireless_nw5").html(tempData['total_info']['down_flow']['packets']);
	 

	 var NWLen=tempData['afi_info'].length;	
	 for(var i=0;i<NWLen;i++)
	 {
		 c.append($('<div class="indicator"><div class="total-indicator-pic-little"><a class="moreinfo" id="NWInfo_'+i+'"><span>'+decideLen(tempData['afi_info'][i]['afi_name'])+'</span></a></div><div class="data data4 datanw_'+i+'"></div><a id="NWviewHistory_'+i+'" class="view-history view-history4">'+viewHistoryText+'</a></div>'));
		 captureStr(tempData['afi_info'][i]['afi_name'],"#NWInfo_"+i,"140px");
		 c.find(".datanw_"+i).append($('<p><span>'+ numUsersText+'</span><a class="usernum" id="NWUserInfo_'+i+'"><span  id="NWnumUsers_'+i+'"></span></a><span id="NWnumUsers_'+i+'-1"></span></p>'));
		 c.find(".datanw_"+i).append($('<p><span>'+ uplinkTextB+'</span><span class="moreinfo" id="NWupFlowB_'+i+'"></span><span id="NWupFlowB_'+i+'_unit"></span><span id="NWupFlowB_'+i+'-1"></span><br/>'+uplinkTextP+'<span  class="moreinfo" id="NWupFlowP_'+i+'"></span><span id="NWupFlowP_'+i+'-1"></span></p>'));
		 c.find(".datanw_"+i).append($('<p><span>'+ downlinkTextB+'</span><span class="moreinfo" id="NWdownFlowB_'+i+'"></span><span id="NWdownFlowB_'+i+'_unit"></span><span id="NWdownFlowB_'+i+'-1"></span><br/>'+downlinkTextP+'<span class="moreinfo" id="NWdownFlowP_'+i+'"></span><span id="NWdownFlowP_'+i+'-1"></span></p>'));
			 
	var NWWlan_info=tempData['afi_info'][i];
	
	NWDeTotalInfo[i]=[NWWlan_info['usr_num'],NWWlan_info['up_bytes'],NWWlan_info['up_packets'],NWWlan_info['down_bytes'],NWWlan_info['down_packets']];
	 $("#NWnumUsers_"+i+"-1").html(calDifWire("#NWnumUsers_"+i,NWDeTotalInfo[i],0));
	 $("#NWnumUsers_"+i).html(NWWlan_info['usr_num']);
	 if(NWWlan_info['usr_list']!="")  {userCluetip(NWWlan_info['usr_list'],"#NWUserInfo_"+i, "100px");}
	
	 $("#NWupFlowB_"+i+"-1").html(calDifWireB(NWDeTotalInfo[i],1,"#NWupFlowB_"+i,1));	 
	 $("#NWupFlowB_"+i).html(dealBytes(NWWlan_info['up_bytes'],'#NWupFlowB_'+i+'_unit'));
	 
	 $("#NWupFlowP_"+i+"-1").html(calDifWire("#NWupFlowP_"+i,NWDeTotalInfo[i],2));
	 $("#NWupFlowP_"+i).html(NWWlan_info['up_packets']);
	 
	 $("#NWdownFlowB_"+i+"-1").html(calDifWireB(NWDeTotalInfo[i],3,"#NWdownFlowB_"+i,1));
	 $("#NWdownFlowB_"+i).html(dealBytes(NWWlan_info['down_bytes'],'#NWdownFlowB_'+i+'_unit'));
	 
	 $("#NWdownFlowP_"+i+"-1").html(calDifWire("#NWdownFlowP_"+i,NWDeTotalInfo[i],4));
	 $("#NWdownFlowP_"+i).html(NWWlan_info['down_packets']);
 	
	  }
	}
		   }
 }
function calDifF(a,b,c,d,e,f)
{
	$(f+'-1').removeAttr("class");
	if(d==undefined)  return "";
	if(d==''||d=='0'||d=="0.0")   return '';
	var par=parseFloat(d);
	if(!$.isNumeric(par))  return "";
	if(c==0)  var dif1 = e[a][b]-par;
	else      var dif1 = e[a][b][c]-par;
	var dif2=dealBytes(dif1,"");
	if(b=="delay_tm") {dif2=dif1; if(Math.abs(dif1)<1&&Math.abs(dif1)>0) dif2=dif1.toFixed(2); }
	var value2=((dif1/par)*100).toFixed(1);
	if(dif1>0)  $(f+'-1').attr("class","data-up");
	//else if(dif1==0)  $('$'+d+'-1').addClass();
	else  $(f+'-1').attr("class","data-down");
	var difference=dif2+'('+value2+'%)';
	return difference;
}
function calDifAP(thisData,lastData,index,id)
{
	$(id+'-1').removeAttr("class");
	if(lastData==undefined) return "";
	if(lastData[index]==0) return "";
	var dif1=thisData-lastData[index];
	var value2=((dif1/lastData[index])*100).toFixed(1);
	if(!$.isNumeric(value2))  return "";
	var dif2=dealBytes(dif1,"");
	if(dif1>0)  $(id+'-1').attr("class","data-up");
	//else if(dif1==0)  $('$'+d+'-1').addClass();
	else  $(id+'-1').attr("class","data-down");
	var difference=dif2+'('+value2+'%)';
	return difference;
}
function calDifWireB(thisData,index,id,flag)
{
	$(id+'-1').removeAttr("class");
	var value1=thisData[index];
	var value2=parseFloat($(id).html());
	if(value1=="")  return "";
	if(!$.isNumeric(value2)||value2==0) return "";
	var Dif1=value1-value2;
	if(Dif1>0)  $(id+"-1").attr("class","data-up");
	//else if(dif1==0)  $('$'+d+'-1').addClass();
	else  $(id+"-1").attr("class","data-down");
	if(flag==1) var Dif2=dealBytes(Dif1,id+"_unit");
	else var Dif2=dealBytes(Dif1,"");
	var value3=((Dif1/value2)*100).toFixed(1)+"%"; 
	var difference=Dif2+'('+value3+')';
	return difference;
}

//Wireless business difference
function calDifWire(id,lastData,index)
{
	$(id+'-1').removeAttr("class");
	if(lastData==undefined)  return "";
	if(lastData.length==0||lastData.length==undefined)  return "";
	if($(id).html()==''||$(id).html()=="0")  return "";
	var value1=parseFloat($(id).html());
	if(!$.isNumeric(value1))   return "";
 	var value2=lastData[index];
	if(value2==0||value2==undefined)  return"";  
	var Dif1=value2-value1;
	if(Dif1>0)  $(id+"-1").attr("class","data-up");
	//else if(dif1==0)  $('$'+d+'-1').addClass();
	else  $(id+"-1").attr("class","data-down");
	var value3=((Dif1/value1)*100).toFixed(1)+"%"; 
	var difference=Dif1+'('+value3+')';
	return difference;	
}
 //transfer bytes number to KB,MB
function dealBytes(Bnumber,id)
{
	var i=0;
	if(!$.isNumeric(Bnumber))  return "";
	while(parseInt(Bnumber/1024)!=0)
	{
		Bnumber=(Bnumber/1024).toFixed(2);
		i++;
	}
	switch (i)
	{
	case 0: 
		if(id!="") $(id).html("Bytes");
		if(id=="")	Bnumber+="Bytes"; 
		break;
	case 1: 
		if(id!="") $(id).html("KB");
		if(id=="")	Bnumber+="KB"; 
		break;
	case 2: 
		if(id!="") $(id).html("MB");
		if(id=="")	Bnumber+="MB"; 
		break;
	case 3: 
		if(id!="") $(id).html("GB");
		if(id=="")	Bnumber+="GB"; 
		break;
	case 4: 
		if(id!="") $(id).html("TB");
		if(id=="")	Bnumber+="TB"; 
		break;
	case 5: 
		if(id!="") $(id).html("PB");
		if(id=="")	Bnumber+="PB"; 
		break;
	case 6:
		if(id!="") $(id).html("EB");
		if(id=="")	Bnumber+="EB"; 
		break;
	}
	return Bnumber;
}
 //add tab style
  function changeClass(obj)
	{
	 $(obj).siblings().attr('id','');
	 $(obj).attr('id','choose-button');
	}
//view history function
function drawChart(obj,a,textType)
{
	changeClass(obj);
	var btn1=$('.time-button-position li').first().attr('id');
	var dataType;
	if(btn1=='choose-button')  dataType='0';
	else dataType='1';
	drawCurveGraph("../../api/history/"+a,'#chart1','',dataType,textType);	
}
function calDif(a,b,c,d,e)
{
	$(d+'-1').removeAttr("class");
	var value1 =$(d).html();
	if(value1==''||value1=='0.0'||value1=='0')   return '';
	if(!$.isNumeric(parseFloat(value1)))  return "";
	if(c==0)  var dif1 = e[a][b]-parseFloat(value1);
	else      var dif1 = e[a][b][c]-parseFloat(value1);
	if(dif1>0)  $(d+'-1').attr("class","data-up");
	//else if(dif1==0)  $('$'+d+'-1').addClass();
	else  $(d+'-1').attr("class","data-down");
	var value2=((dif1/parseFloat(value1))*100).toFixed(1)+"%"; 
	var difference=dif1+'('+value2+')';
	return difference;
}
//Wireless Data(if name of name(ap/wlan) is too long,show the cluetip)
function decideLen(Str1)
{
	if(Str1.length>10)
	 {var Str2=Str1.substring(0,7)+"...";
	 }
	else  return Str1;
	return Str2;
}
function captureStr(Str1,index,list_width)
{
	if(Str1.length>10)
	{
	var rest ='|';
	rest+=Str1;
	$(index).attr("title", rest);
	$(index).cluetip({splitTitle: '|', width: list_width, showTitle: false, cluetipClass: 'cluetip',cursor:'pointer'});
	}
}

function createTable(dataset, attrname, index, list_width,units)
{
	var rest ='|';
	var i = 0;
	var aLen=attrname.length;
	for(var key in dataset){
		list_info = attrname[i] + dataset[key];
		rest += list_info+units+"<br/>";
		i++;
		if(i>aLen-1) break;
	}
	$(index).attr("title", rest);
	$(index).cluetip({splitTitle: '|', width: list_width, showTitle: false, cluetipClass: 'cluetip',cursor:'pointer'});
}

function createTableD(dataset,attrname,next, index, list_width,units)
{
	var rest ='|';
	for(var key in dataset){
		list_info = attrname+key+next+":"+ dataset[key];
		rest += list_info+units+"<br/>";
	}
	$(index).attr("title", rest);
	$(index).cluetip({splitTitle: '|', width: list_width, showTitle: false, cluetipClass: 'cluetip',cursor:'pointer'});
}
//Wireless Data(user num cluetip;mi\iPhone\phone)
function userCluetip(dataset,index, list_width)
{
	var rest ='|';
	for(var key in dataset){
		var transKey=eval("Msg."+key);
		list_info = transKey+"("+ dataset[key]+")";
		rest += list_info+"<br/>";
	}
	$(index).attr("title", rest);
	$(index).cluetip({splitTitle: '|', width: list_width, showTitle: false, cluetipClass: 'cluetip',cursor:'pointer'});
}
//support page:show the service code
$(document).ready(function(e) {
    $.ajax({
		method:'POST',
		url:"/api/service_code.php",
		dataType:'json',
		success: function(rst){
			$('#my_service_code').html(rst['service_code']);
		}
	});
});

