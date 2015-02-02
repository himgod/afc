			function HashTable(key,value)
			{
				this.hashtable = new Object();
				
				this.add = function(key,value)
				{
				       // ?D??keyê?·?′??ú
				       if((key in this.hashtable) == false)
				       {
				    	   this.hashtable[key] = value;
				       }
				       
				};
				this.del = function(key)
				{
					if(key in this.hashtable)
					{
						delete(this.hashtable[key]);
					}
					else
					{
						alert("é?3yμ??ü?μ2?′??ú");
					}
				};
				
				this.getValue = function(key)
				{
					return this.hashtable[key];
				};
				this.isExistHashTable = function(key)
				{
					if((key in this.hashtable) == true)
					{
						return true;
					}
					else
					{
						return false;
					}
				};
				
				this.print = function(key)
				{
					alert(this.hashtable[key]);
				};
				this.length = function()
				{
					var len = 0;
					for(var k in this.hashtable)
					{
						len++;
					}
					return len;
				};
			}
			function randomColor()
			{
				return '#'+('00000'+(Math.random()*0x1000000<<0).toString(16)).slice(-6);
			}
			//alert(randomColor());
			//Chart class describe a Chart's property
			var colorArray = ['#32D2C9','#FB6362','#F8A20F','#36B92A'];
			function Chart(data,chart_type)
			{
				this.title = data.mainTitle;
				this.subtitle = data.subTitle;
				this.chartType = chart_type;
				this.hashtable = new HashTable();//′?′￠y?á??êy￡??′2?í?μ￥??μ??á￡??aà?′?′￠á?±íê?μ￥??μ?×?·?
				//?¨ò?x?á
				this.xAxis = data.xAxisCategories;

				this.lineWidth = data.lineWidth;
				this.markLineWidth = data.markLineWidth;

				//?¨ò?y?á
				this.yAxis = new Array();	
				//?¨ò?sereisData
				this.seriesArray = new Array();
				//ì?3?y?áêy?Y
				this.fillYAxisArray = function()
				{
					for(var i=0;i<data.yAxisTitle.length;i++)
					{
						//alert(data.yAxisTitle[i].title+data.yAxisTitle[i].label);
						var tmpLabel = data.yAxisTitle[i].label;
						var tmpYAxisDataOne;
						if( i == 0 )
					    {
							var tmpLabel = data.yAxisTitle[0].label;
							tmpYAxisDataOne = {
									"title": {
										"text":data.yAxisTitle[i].title
									},
									"labels":{
										"format": '{value} '+tmpLabel,
										"style":{
											"color":colorArray[i]
										}
									}							
							};
					    }
						else
						{
							tmpYAxisDataOne = {
									"title": {
										"text":data.yAxisTitle[i].title
									},
									"labels":{
										"format": '{value} '+tmpLabel,
										"style":{
											"color":colorArray[i]
										}
									},
									opposite: true
							};  
						}
						if(this.hashtable.isExistHashTable(data.yAxisTitle[i].label) == false )
						{
							this.hashtable.add(i+1,data.yAxisTitle[i].label);
						}
						this.yAxis.push(tmpYAxisDataOne);
						//alert(this.yAxis[i].title);
					}
					//alert(this.hashtable.getValue(1));
				};
				
				//ì?3?seriesêy?Y,??????????y?áoíμ￥?á
				this.fillSeriesArray = function()
				{
					//var tmpValueSuffix = "";
					//alert(this.hashtable.getValue(2));
					for(i=0;i<data.series.length;i++)
					{
						if(data.series[i].valueSuffix == this.hashtable.getValue(1))
						{
							var tmpSeriesDataOne = {
									"name":data.series[i].name,
									"color":colorArray[0],
									"data":data.series[i].data,
									"tooltip":{
										"valueSuffix":data.series[i].valueSuffix
									}
								};
							//alert("hello" + this.hashtable.getValue(1));
						}
						else if(data.series[i].valueSuffix == this.hashtable.getValue(2))
						{
							var tmpSeriesDataOne = {
									"name":data.series[i].name,
									"color":colorArray[1],
									"data":data.series[i].data,
									"yAxis":1,
									"tooltip":{
										"valueSuffix":data.series[i].valueSuffix
									}
								};
							//alert("hello" + this.hashtable.getValue(2));
						}
						else
						{
							var tmpSeriesDataOne = {
									"name":data.series[i].name,
									"color":colorArray[2],
									"data":data.series[i].data,
									"yAxis":2,
									"tooltip":{
										"valueSuffix":data.series[i].valueSuffix
									}
								};
							//alert("hello else");
						}
						this.seriesArray.push(tmpSeriesDataOne);
					}
				 };
			}


			function judgeWhichInfoByUrl(json_url,json_param)
			{

				var tmpdata;
				$.ajax({ 
					async: false, 
					type : "POST", 
					url : json_url, 
					data:{time_type:json_param.time_type,wireless_type:json_param.wireless_type},
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
						//alert(tmpdata);
					} 
				});
				if(tmpdata != "")
				{
					var chartdata;  
					if(json_url == "../../api/history/ap_history_list.php")
					{
						chartdata = {
						"mainTitle":"AP指标呈现",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Times(s)",
								"label":"s"
							},
							{
								"title":"DSI(dbm)",
								"label":"dbm"
							},
							{
								"title":"NetSpeed",
								"label":"Mbs"
							}
						], 
						"series":
							[
								{                       
								"name": "平均时延",
								"data": tmpdata.data.TmDelay,
								"valueSuffix":"s"
								},
								{
								 "name": "上下行带宽",
								 "data": tmpdata.data.DownUpBW,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "信号强度",
									"data": tmpdata.data.SignalIntens,
									"valueSuffix":"dbm"
							    }
						
								],
							"lineWidth":4,
							"markLineWidth":2
						}
				}
				else if(json_url == "../../api/history/wifi_history_list.php")
				{
					chartdata = {
						"mainTitle":"WIFI指标呈现",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"百分比(%)",
								"label":"%"
							},
							{
								"title":"信号强度(dbm)",
								"label":"dbm"
							}
						], 
						"series":
							[
								{
								"name": "信噪比",
								"data": tmpdata.data.SignalNoiseRate,
								"valueSuffix":"%"
								},
								{
								 "name": "信道利用率",
								 "data":tmpdata.data.ChanUtilize,
								 "valueSuffix":"%"		  		
								},
								{
									"name": "信号干扰强度",
									"data": tmpdata.data.SignalInter,
										"valueSuffix":"dbm"
								    }
								],
							"lineWidth":4,
							"markLineWidth":2
						}
					}
					else if(json_url == "../../api/history/terminal_history_list.php")
					{
					chartdata = {
						"mainTitle":"终端指标呈现",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Percentage(%)",
								"label":"%"
							},
							{
								"title":"NetSpeed",
								"label":"Mbs"
							},
							{
								"title":"DSI(dbm)",
								"label":"dbm"
							}
						], 
						"series":
							[
								{
								"name": "接入速率",
								"data": tmpdata.data.AccessRate,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "协商速率",
								 "data": tmpdata.data.NegotiateRate,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "信号强度",
									"data": tmpdata.data.SignalIntens,
									"valueSuffix":"dbm"
							    },
								{
									"name": "无限重传率",
									"data": tmpdata.data.RetransRate,
									"valueSuffix":"%"
							    }
							
								],
							"lineWidth":3,
							"markLineWidth":1
						}
					}
					else if(json_url == "../../api/history/flow_history_list.php")
					{
					chartdata = {
						"mainTitle":"流量统计呈现",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Packets(包数)",
								"label":"Mbs"
							},
							{
								"title":"Bytes（字节数）",
								"label":"Kbs"
							}
						], 
						"series":
							[
								{
								"name": "上行包数",
								"data": tmpdata.data.ULnkPackNum,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "下行包数",
								 "data": tmpdata.data.DLnkPackNum,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "上行字节数",
									"data": tmpdata.data.ULnkByteNum,
									"valueSuffix":"Kbs"
							    },
								{
									"name": "下行字节数",
									"data": tmpdata.data.DLnkByteNum,
									"valueSuffix":"Kbs"
								    }
							
								],
							"lineWidth":3,
							"markLineWidth":1
						}
					}
					else if( json_url == "../../api/history/wireless_history_list.php")
					{
					chartdata = {
						"mainTitle":"无线业务统计呈现",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Packets(包数)",
								"label":"Mbs"
							},
							{
								"title":"Bytes（字节数）",
								"label":"Kbs"
							},
							{
								"title":"在线用户",
								"label":"(人)"
							}
						], 
						"series":
							[
								{
								"name": "用户数量",
								"data": tmpdata.data.OnUsrCnt,
								"valueSuffix":""
								},
								{
								"name": "上行包数",
								"data": tmpdata.data.ULnkPackNum,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "下行包数",
								 "data": tmpdata.data.DLnkPackNum,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "上行字节数",
									"data": tmpdata.data.ULnkByteNum,
									"valueSuffix":"Kbs"
							    },
								{
									"name": "下行字节数",
									"data": tmpdata.data.DLnkByteNum,
									"valueSuffix":"Kbs"
							    }
							
								],
							"lineWidth":3,
							"markLineWidth":1
						}
					}
					else if(json_url == "../../api/history/sys_history_list.php")
					{
						//do nothing
					}
					else
					{
						
					}
					return chartdata;
				}
				else
				{
					return "";
				}
				
			}
			
			//?????ú??í?oˉêy￡?2?êyjson_url?a??è?êy?Yμ?urlμ??·￡?div_id±íê?html??ê?????￡?è?±ê??μ?idμè
			//json_param表示传入给后台参数为json格式
			function drawCurveGraph(json_url,div_id,chart_type,time_type,wireless_type)
			{
				var json_param = {
					"time_type":time_type, 
					"wireless_type":wireless_type 
				};
				var testdata = judgeWhichInfoByUrl(json_url,json_param);
				/*$.ajax({ 
					async: false, 
					type : "POST", 
					url : json_url, 
					data:{data_type:time_type},
					dataType : 'json', 
					success : function(data) { 
						testdata = data; 
						//alert(testdata);
					} 
				});*/

			    var xAxisInterval = 1;
				if(json_param.time_type == "1")//，默认0表示24小时数据，1为表示30天数据
				{
					xAxisInterval = 2;
				}
				if( chart_type == "" ) //默认表格类型为曲线图
				{
					chart_type = "spline";
				}
				var chart = new Chart(testdata,chart_type);

				chart.fillYAxisArray();
				chart.fillSeriesArray();
				//alert(chart.yAxis[0].title);
				$(function () {
				    $(div_id).highcharts({
				    	chart: {
					            zoomType: 'xy',
					            type:chart.chartType
					        },
				        title: {
				            text: chart.title,
				            x: -20 //center
				        },
				        xAxis: [{
				        	gridLineWidth: 1,
							//lineColor: '#000',
							tickInterval: xAxisInterval,
				            categories: chart.xAxis
				        }],
				        plotOptions: {
				            series: {
				                lineWidth:chart.lineWidth, 
				                marker: {
                    				fillColor: '#FFFFFF',
				                    lineWidth: chart.markLineWidth,
				                    lineColor: null // inherit from series
                				}
				            }
				        },
				        yAxis: chart.yAxis,
				        tooltip: {
				            //shared: true
				            backgroundColor:'#FCFFC5'
				        },
						credits: {
							enabled: false
						},
						exporting: {
							enabled: false
						},
				        legend: {
				            layout: 'horizontal',
				            //x: 400,
            				y: 0,
				            align: 'top',
				            verticalAlign: 'bottom',
				            borderWidth: 0
				        },
				        
				        series: chart.seriesArray
				    });
				});
			}