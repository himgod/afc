			function HashTable(key,value)
			{
				this.hashtable = new Object();
				
				this.add = function(key,value)
				{
				       // ?D??key��?��?��??��
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
						alert("��?3y��??��?��2?��??��");
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
				this.hashtable = new HashTable();//��?���y?��??��y��??��2?��?�̣�??��??����??a��?��?��騢?������?�̣�??��?��?��?
				//?����?x?��
				this.xAxis = data.xAxisCategories;

				this.lineWidth = data.lineWidth;
				this.markLineWidth = data.markLineWidth;

				//?����?y?��
				this.yAxis = new Array();	
				//?����?sereisData
				this.seriesArray = new Array();
				//��?3?y?����y?Y
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
				
				//��?3?series��y?Y,??????????y?��o���̣�?��
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
						"mainTitle":"APָ�����",
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
								"name": "ƽ��ʱ��",
								"data": tmpdata.data.TmDelay,
								"valueSuffix":"s"
								},
								{
								 "name": "�����д���",
								 "data": tmpdata.data.DownUpBW,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "�ź�ǿ��",
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
						"mainTitle":"WIFIָ�����",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"�ٷֱ�(%)",
								"label":"%"
							},
							{
								"title":"�ź�ǿ��(dbm)",
								"label":"dbm"
							}
						], 
						"series":
							[
								{
								"name": "�����",
								"data": tmpdata.data.SignalNoiseRate,
								"valueSuffix":"%"
								},
								{
								 "name": "�ŵ�������",
								 "data":tmpdata.data.ChanUtilize,
								 "valueSuffix":"%"		  		
								},
								{
									"name": "�źŸ���ǿ��",
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
						"mainTitle":"�ն�ָ�����",
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
								"name": "��������",
								"data": tmpdata.data.AccessRate,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "Э������",
								 "data": tmpdata.data.NegotiateRate,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "�ź�ǿ��",
									"data": tmpdata.data.SignalIntens,
									"valueSuffix":"dbm"
							    },
								{
									"name": "�����ش���",
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
						"mainTitle":"����ͳ�Ƴ���",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Packets(����)",
								"label":"Mbs"
							},
							{
								"title":"Bytes���ֽ�����",
								"label":"Kbs"
							}
						], 
						"series":
							[
								{
								"name": "���а���",
								"data": tmpdata.data.ULnkPackNum,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "���а���",
								 "data": tmpdata.data.DLnkPackNum,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "�����ֽ���",
									"data": tmpdata.data.ULnkByteNum,
									"valueSuffix":"Kbs"
							    },
								{
									"name": "�����ֽ���",
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
						"mainTitle":"����ҵ��ͳ�Ƴ���",
						"xAxisCategories":tmpdata.TmStmp,
						"yAxisTitle":[
							{
								"title":"Packets(����)",
								"label":"Mbs"
							},
							{
								"title":"Bytes���ֽ�����",
								"label":"Kbs"
							},
							{
								"title":"�����û�",
								"label":"(��)"
							}
						], 
						"series":
							[
								{
								"name": "�û�����",
								"data": tmpdata.data.OnUsrCnt,
								"valueSuffix":""
								},
								{
								"name": "���а���",
								"data": tmpdata.data.ULnkPackNum,
								"valueSuffix":"Mbs"
								},
								{
								 "name": "���а���",
								 "data": tmpdata.data.DLnkPackNum,
								 "valueSuffix":"Mbs"		  		
								},
								{
									"name": "�����ֽ���",
									"data": tmpdata.data.ULnkByteNum,
									"valueSuffix":"Kbs"
							    },
								{
									"name": "�����ֽ���",
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
			
			//?????��??��?o����y��?2?��yjson_url?a??��?��y?Y��?url��??����?div_id������?html??��?????��?��?����??��?id�̨�
			//json_param��ʾ�������̨����Ϊjson��ʽ
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
				if(json_param.time_type == "1")//��Ĭ��0��ʾ24Сʱ���ݣ�1Ϊ��ʾ30������
				{
					xAxisInterval = 2;
				}
				if( chart_type == "" ) //Ĭ�ϱ������Ϊ����ͼ
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