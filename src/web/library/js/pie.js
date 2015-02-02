$(document).ready(function(){
	//drawPie("#container");
	//drawPolar("#scoreOverwiewContainer", [100, 100, 100, 100, 100],[100, 100, 100, 100, 100]);
	//setInterval("drawPolar('#scoreOverwiewContainer')", 10000);
});

$view = $('#view');
var width = 0;
var height = 0;

function num() {
	var now_score = Number($("#view").html());
	if (Number($("#view").html()) >= Number(score_tmp)) {
		clearInterval(timerID)
	} else {

		if ((Number($("#view").html() - 0) + 1) < 10) {
			$("#view").html("0" + (Number($("#view").html()) + 1));
		} else {
			$("#view").html((Number($("#view").html()) + 1));
		}

		var score_color = '#ff3300';
		if(now_score == 79){
			score_color = '#009900';
			$view.css({
				'color': score_color
			});
		}else if(now_score == 59){
			score_color = '#ffcc00';
			$view.css({
				'color': score_color
			});
		}

	}
}

function drawPolar(div_id,sys_health_data)
{
    var bgData =[100,100,100,100,100]; //background's data
	var iCategory =0;
	score_tmp = 0;
	var rScore_array = [];

	var score_array = [];
	score_array.push(sys_health_data.usr_num_info.usr_score);
	score_array.push(sys_health_data.usr_rssi_info.rssi_score);
	score_array.push(sys_health_data.net_stab_info.stab_score);
	score_array.push(sys_health_data.net_conn_info.conn_score);
	score_array.push(sys_health_data.disturb_info.disturb_score);
	for(var j = 0 ; j < 5;j++)
	{
		if( typeof( score_array[j] ) != "number" )
		{
			score_array[j] = 100;
		}
		else
		{
			if( score_array[j] < 0 || score_array[j] > 100 )
			{
				score_array[j] = 100;
			}
		}
		var rData = 100 - score_array[j];
		rScore_array.push(rData);
		var score_data = [[Msg.OnlineUsrNum,score_array[0]],[Msg.UserSigQuality,score_array[1]],[Msg.NetStable,score_array[2]],[Msg.NetLinked,score_array[3]],[Msg.InterferSigStrength,score_array[4]]];
		score_tmp += Math.ceil(score_array[j]/5);
	}
	$(function () {
    // Parse the data from an inline table using the Highcharts Data plugin
			$(div_id).highcharts({
				/*data: {
					table: 'freq',
					startRow: 1,
					endRow: 6,
					endColumn: 2
				},*/
				chart: {
					polar: true
				},
				
				title: {
					text: null
				},
				
				subtitle: {
					text: ''
				},
				credits: {
					enabled: false
				},
				exporting: {
					enabled: false
				},
				pane: {
					size:"95%"
				},
				legend: {
					enabled: false
				},
				xAxis: {
					categories: ['', '', '', '', ''], //['网络稳定性', '用户信号质量', '在线用户数', '干扰信号强度', '网络连通性'],
					//tickmarkPlacement: 'on',
					enabled: true,
					tickInterval: 72,
					min: 0,
					max: 360,
					labels: {
						formatter: function () {
						return  '';
						}
					}
				},
				yAxis: {
					min: 0,
					endOnTick: false,
					//showLastLabel: true,
					//reversedStacks: true,
					title: {
						//text: 'Frequency (%)'
					},
					labels: {
						formatter: function() {
							//return this.value + '%';
						}
					}
				},
				tooltip: {
					enabled:true,
					backgroundColor: '#FCFFC5',
					borderColor: '#000000'
						//valueSuffix: '%',
						//followPointer: true
				},

				plotOptions: {
					series: {
						stacking: 'normal',
						shadow: false,
						//grouping:true,
						groupPadding: 0,
						pointPlacement: 'on',
						pointPadding: 0.005,
						//pointStart: 0,
						//pointInterval: 0,
						colors: ["#DD1D00", "#FB6362", "#F8A20F", "#26881D", "#3CA7EF"],
						colorByPoint: true
					},
					pie: {
						enableMouseTracking:false,
						shadow: {
							opacity:1
						},
						center: ['50%', '50%'],
						startAngle: 0,
						animation: false,
						dataLabels: {
							enabled: true,
							crop:false,
							overflow: 'none',
							connectorPadding: 6,
							connectorWidth: 2.5,
							zIndex: 3
						}
					},
					column: {
						pointPadding: 0,
						groupPadding: 0,
						pointStart: -36,
						pointInterval: 72,
						dataLabels: {
							enabled: false
						}
						//threshold:30
					}
				}
				,
				series: [{
						type: 'column',
						enableMouseTracking:false,
						borderWidth: 0,
						data: rScore_array,
						colors: ["#F6BC58", "#F88685", "#F04F37", "#63B8F2", "#4B9E44"],
						animation: {
							duration: 3000
						},
						zIndex: 3
				},
				{
					type:'pie',
					data:bgData,
					dataLabels: {
						enabled:false,
						useHtml:true,
						style:{
							fontWeight:'bold'
						},
						formatter: function() {
							var category = [ '用户信号质量','网络稳定性','网络连通性','干扰信号强度','在线用户数'];
							var tmp = category[iCategory];
							iCategory++;
							
							var labelData = score_array[(iCategory%5)];//polarData[i][(iCategory%5)];
							return this.percentage > 1 ? '<b>'+ tmp +':</b> '+ labelData : null;
							},
               			distance: 85
				        //format: hashtable.getValue('\{point.name\}')
				     },
					colors:["#F88685","#F04F37","#63B8F2","#4B9E44","#F6BC58"],
					size:'102%',
					zIndex:2
				},
				{
					type: 'column',
					name: Msg.EvaluateScore,
					borderWidth:0,
					data: score_data, 
					colors:["#F8A20F","#FB6362","#DD1D00","#3CA7EF","#26881D"],
					animation: {
			            duration: 2000
			            },
					zIndex:3
				}
				]
				/*{
                name:'100',
                type:'pie',
                data:[10],
                colors:["#FFFFFF"],
                size:'25%',
				animation: {
			            duration: 2000
			            }
				}]*/
			},
			function(chart)
			{
			    chart.renderer.circle(chart.chartWidth/2-1,chart.chartHeight/2-1,100)
				    	.attr({
							 'zIndex':3,
				    		'fill':'#FFFFFF',
				    		'stroke-width': 0,
				            'stroke': '#000000',
				            'stroke-dasharray':0,
				             'stroke-opacity':0.5
				    	})
				    	.add();
				chart.renderer.path(['M', chart.chartWidth/2+100, chart.chartHeight/2-135, 'L', chart.chartWidth/2+340,chart.chartHeight/2-135])
				            .attr({
				                'stroke-width': 1,
				                'stroke': '#F04F37'
				            })
				          .add();
				chart.renderer.circle(chart.chartWidth/2+340,chart.chartHeight/2-135,10)
				    	.attr({
							'fill':'#F04F37',
							'stroke-width': 1
				    	})
				    	.add();
				chart.renderer.circle(chart.chartWidth/2+340,chart.chartHeight/2-135,5)
				    	.attr({
							'fill':'#FFFFFF',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.text(Msg.UserSigQuality + sys_health_data.usr_rssi_info.usr_rssi, chart.chartWidth/2+200, chart.chartHeight/2-142)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'16px',
								fontFamily:'微软雅黑'
					        })
					        .add();							
				chart.renderer.path(['M', chart.chartWidth/2-170, chart.chartHeight/2+10, 'L', chart.chartWidth/2-350,chart.chartHeight/2+10])
				            .attr({
				                'stroke-width': 1,
				                 'stroke': '#26881D'
				            })
				            .add();
				chart.renderer.circle(chart.chartWidth/2-340,chart.chartHeight/2+10,10)
				    	.attr({
							'fill':'#26881D',
							'stroke-width': 1
				    	})
				    	.add();
				chart.renderer.circle(chart.chartWidth/2-340,chart.chartHeight/2+10,5)
				    	.attr({
							'fill':'#FFFFFF',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.text(Msg.InterferSigStrength + sys_health_data.disturb_info.disturb_avr + "dBm", chart.chartWidth/2-330, chart.chartHeight/2)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'16px',
								fontFamily:'微软雅黑'
					        })
					        .add();
				chart.renderer.path(['M', chart.chartWidth/2-50, chart.chartHeight/2+150, 'L', chart.chartWidth/2-320,chart.chartHeight/2+150])
				            .attr({
				                'stroke-width': 1,
				                 'stroke': '#3CA7EF'
				            })
				            .add();
				chart.renderer.circle(chart.chartWidth/2-320,chart.chartHeight/2+150,10)
				    	.attr({
							'fill':'#3CA7EF',
							'stroke-width': 1
				    	})
				    	.add();
				chart.renderer.circle(chart.chartWidth/2-320,chart.chartHeight/2+150,5)
				    	.attr({
							'fill':'#FFFFFF',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.text(Msg.NetLinked, chart.chartWidth/2-305, chart.chartHeight/2+141)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'16px',
								fontFamily:'微软雅黑'
					        })
					        .add();
				chart.renderer.text(Msg.LossPackRate + sys_health_data.net_conn_info.pkg_loss_rate +"%", chart.chartWidth/2-220, chart.chartHeight/2+145)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'13px',
								fontFamily:'微软雅黑',
								color:'#393d49'
					        })
					        .add();
				chart.renderer.text(Msg.DelayTm + sys_health_data.net_conn_info.delay_tm + "s", chart.chartWidth/2-220, chart.chartHeight/2+115)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'13px',
								fontFamily:'微软雅黑',
								color:'#393d49'
					        })
					        .add();
				chart.renderer.path(['M', chart.chartWidth/2+100, chart.chartHeight/2+115, 'L', chart.chartWidth/2+340,chart.chartHeight/2+115])
				            .attr({
				                'stroke-width': 1,
				                 'stroke': '#DD1D00'
				            })
				            .add();
				chart.renderer.circle(chart.chartWidth/2+340,chart.chartHeight/2+115,10)
				    	.attr({
							'fill':'#DD1D00',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.circle(chart.chartWidth/2+340,chart.chartHeight/2+115,5)
				    	.attr({
							'fill':'#FFFFFF',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.text(Msg.NetStable , chart.chartWidth/2+140, chart.chartHeight/2+111)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'16px',
								fontFamily:'微软雅黑'
					        })
					        .add();
				chart.renderer.text(Msg.reTryRate + sys_health_data.net_stab_info.rtry_rate +"%", chart.chartWidth/2+232, chart.chartHeight/2+81)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'13px',
								fontFamily:'微软雅黑',
								color:'#393d49'
					        })
					        .add();
				chart.renderer.text(Msg.AccessSuccRate + sys_health_data.net_stab_info.access_to_succ +"%", chart.chartWidth/2+232, chart.chartHeight/2+111)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'13px',
								fontFamily:'微软雅黑',
								color:'#393d49'
					        })
					        .add();
				chart.renderer.text(Msg.DropsRate + sys_health_data.net_stab_info.drops_rate +"%", chart.chartWidth/2+232, chart.chartHeight/2+141)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'13px',
								fontFamily:'微软雅黑',
								color:'#393d49'
					        })
					        .add();
				chart.renderer.path(['M', chart.chartWidth/2, chart.chartHeight/2-135, 'L', chart.chartWidth/2-320,chart.chartHeight/2-135])
				            .attr({
				                'stroke-width': 1,
				                 'stroke': '#F8A20F'
				            })
				            .add();
				chart.renderer.circle(chart.chartWidth/2-320,chart.chartHeight/2-135,10)
				    	.attr({
							'fill':'#F8A20F',
							'stroke-width': 1
				    	})
						.add();
				chart.renderer.circle(chart.chartWidth/2-320,chart.chartHeight/2-135,5)
				    	.attr({
							'fill':'#FFFFFF',
							'stroke-width': 1
				    	})
						.add();
			    chart.renderer.text(Msg.OnlineUsrNum +" "+ sys_health_data.usr_num_info.usr_nums, chart.chartWidth/2-305, chart.chartHeight/2-142)
					        .attr({zIndex:3})
					        .css({
					            fontSize:'16px',
								fontFamily:'微软雅黑'
					        })
					        .add();
				width = chart.chartWidth/2-38;
				height = chart.chartHeight/2+23  ;
			}
		);
		if (score_tmp == 100) {
			$view.css({
				'margin-left': '285px'
			});
		}

		/*
		else if (score_tmp < 10) {
			$view.css({
				'margin-left': '335px'
			});
		}*/
		$view = $('#view');
		$view.css({
			'font-family': '微软雅黑',
			'color': '#ff3300',
			'font-size':'86px',
			'position':'absolute',
			'margin-top':'185px',
			'margin-left': '298px',
			'left': '20%'
		});
		var winWidth = 1100;
		if (window.innerWidth)
			winWidth = window.innerWidth;
		else if ((document.body) && (document.body.clientWidth))
			winWidth = document.body.clientWidth;
		
		if(winWidth <= 900){
			$view.css({
				'position':'relative',
				'top':'-200px'
			});
		}
		if(winWidth <= 1040){
			$view.css({
				'left': '21%'
			});
		}
		else{
			$view.css({
				'left': '20%'
			});
		}
		$view.html("00");
		timerID = setInterval("num()", 2000 / score_tmp);

	});
}