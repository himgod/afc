
 $(function(){
  
	 var maps = $(".map");
	 var map_show = function(){
	    var map = $(this);
	    var main = map.find(".map-mainpanel");
	    var cont = map.find(".map-content");
	    var zoomsets = map.find(".map-zoomset" );
	    var setscale = map.find(".map-setscale");
		var map_image = cont.find(".map-images");
		var scale = main.find(".map-scale");
		var hide = main.find("div.map-unplacedhide");
		var unp = main.find(".map-unplacedlist");
		var scaletool = main.find(".map-scaletool");
		var scale_canvas = $(".map .scale-canvas");
		var cover = main.find(".map-cover");
		//var ruler = map.find(".map-ruler");
	  	var unpwidth = unp.outerWidth();
	  	var centimeter = scale.width();
	    var unpishide = false;
		window.MapOpMsg = function(){};
		window.Msg = function(){};
		MapOpMsg["api.err.MapFileRestoreFailed"] = Msg.MapFileRestoreFailed;
		MapOpMsg["api.err.MapFileUploadFailed"] = Msg.MapFileUploadFailed;
		MapOpMsg["api.err.MapFileTooLarge"] = Msg.MapFileTooLarge;
		MapOpMsg["api.err.MapFileExists"] = Msg.MapFileExists;
		MapOpMsg["api.err.InvalidMapFileUpload"] = Msg.InvalidMapFileUpload;
		MapOpMsg["api.err.MapFileRenameFailed"] = Msg.MapFileRenameFailed;
		MapOpMsg["api.err.MapFileDeleteFailed"] = Msg.MapFileDeleteFailed;
  		map_image.data("map-count", 0);
  		map_image.data("map-showidx", -1);
  		map_image.remove(".map-imagecontainer");
		scale_canvas.hide();
		
	  	 var hasSVG = function(){
	  		var doc = document;
	  		SVG_NS = 'http://www.w3.org/2000/svg';
			return !!doc.createElementNS && !!doc.createElementNS(SVG_NS, 'svg').createSVGRect;
		}


	  	var clearLine = function(container){
	  		container.empty();
	  	}
		var drawLine = function(option)
		{
			if(hasSVG())
			{
				/*var svgnode = ('<svg xmlns="http:\/\/www.w3.org\/2000/svg" version="1.1" style="position:absolute;left:0;top:0;width:100%;height:100%;">'
					+'<line x1="'+Math.round(option.x0)+'" y1="'+Math.round(option.y0)
					+'" x2="'+Math.round(option.x)
					+'" y2="'+Math.round(option.y)+'" style="stroke:'
					+option.color+';stroke-width:'+option.weight+'" stroke-opacity="'+(option.opacity||1)+'" '+(option.dash ? 'stroke-dasharray="10,10"':'')+'></line></svg>');*/
				if(option.points)
				{
					var svgnode = ('<svg xmlns="http:\/\/www.w3.org\/2000/svg" version="1.1" style="position:absolute;left:0;top:0;width:100%;height:100%;">'
						+'<polyline fill="none" stroke-miterlimit="5" points="');
					var ii = 0;
					var len = option.points.length;
					for(ii = 0; ii < len;ii++)
					{
						svgnode += (((ii%2) == 0) ? " ":",") + Math.round(option.points[ii]);
					}
					svgnode += '" style="stroke:'+option.color+';stroke-width:'
						+option.weight+'" opacity="'+(option.opacity||1)+'" '
						+(option.dash ? 'stroke-dasharray="10,10"':'')
						+'></polyline></svg>';
					return option.container ? option.container.html(svgnode) : svgnode;
					//option.container.find("svg").width("100%");
					//option.container.find("svg").height("100%");
				}
			}
			else
			{
				//var vmlnode = ('<v:line from="'+option.x0+','+option.y0
				//	+'" to="'+option.x+','+option.y
				//	+'" strokeWeight="'+option.weight+'" strokeColor="'+option.color+'">'+
				//	'<v:stroke '+(option.dash ? 'dashstyle="Dash"':'')+'/>'+
				//	'</v:line>');
				if(option.points)
				{
					var vmlnode = ('<v:polyline filled="false" points="');
					var ii = 0;
					var len = option.points.length;
					for(ii = 0; ii < len;ii++)
					{
						vmlnode += (ii > 0 ? ",":"") + Math.round(option.points[ii]);
					}
					vmlnode += '" strokeWeight="'+option.weight+'" strokeColor="'+option.color+'">'+
						'<v:stroke '+(option.dash ? 'dashstyle="Dash"':'')+'/>'+
						'</v:polyline>';
					return option.container ? option.container.html(vmlnode) : vmlnode;
				}
			}
			//container.html(svgnode+vmlnode);
			//var paper = Raphael(container.get(0), container.width(), container.height());
			//paper.path("M"+Math.round(x0)+" "+Math.round(y0)+"L"+Math.round(x)+" "+Math.round(y)).attr({stroke:"red","stroke-width": 2});
			//container.find("div").mousemove(function(){return true;});
		};
		var drawRoundLine = function(option)
		{
			if(hasSVG())
			{
				if(option.points && option.points.length > 2)
				{
					var x0 = option.points[0];
					var y0 = option.points[1];
					
					var svgnode = ('<svg xmlns="http:\/\/www.w3.org\/2000/svg" version="1.1" style="position:absolute;left:0;top:0;width:100%;height:100%;">'
						+'<path fill="none" d="M'+Math.round(x0)+","+Math.round(y0)+" L");
					var ii = 0;
					var len = option.points.length - 2;
					var x1 = 0;
					var y1 = 0;
					for(ii = 0; ii < len-2;ii+=2)
					{
						x = Math.round(option.points[2+ii]);
						y = Math.round(option.points[2+ii+1]);
						x1 = Math.round(option.points[2+ii+2]);
						y1 = Math.round(option.points[2+ii+3]);
						svgnode += Math.round(x0+(x-x0)*0.9)+","+Math.round(y0+(y-y0)*0.9)+
							" C"+x+","+y+","+x+","+y+","+
							Math.round(x+(x1-x)*0.1)+","+Math.round(y+(y1-y)*0.1)+" L";
						x0 = x;
						y0 = y;
					}
					x = Math.round(option.points[len]);
					y = Math.round(option.points[len+1]);
					svgnode += x+","+y;
					
					svgnode += '" style="stroke:'+option.color+';stroke-width:'
						+option.weight+'" opacity="'+(option.opacity||1)+'" '
						+(option.dash ? 'stroke-dasharray="5,5"':'')
						+'></path></svg>';
					return option.container ? option.container.html(svgnode) : svgnode;
				}
			}
			else
			{
				var x0 = option.points[0];
				var y0 = option.points[1];
				if(option.points && option.points.length > 2)
				{
					var width = option.container.width();
					var height = option.container.height();
					
					var vmlnode = ('<v:shape style="position:absolute;left:0%;top:0%;width:100%;height:100%" filled="false" coordsize="'+width+","+height+'" path="M'+Math.round(x0)+","+Math.round(y0)+" L");
					var ii = 0;
					var len = option.points.length - 2;
					var x1 = 0;
					var y1 = 0;
					for(ii = 0; ii < len-2;ii+=2)
					{
						x = Math.round(option.points[2+ii]);
						y = Math.round(option.points[2+ii+1]);
						x1 = Math.round(option.points[2+ii+2]);
						y1 = Math.round(option.points[2+ii+3]);
						vmlnode += Math.round(x0+(x-x0)*0.9)+","+Math.round(y0+(y-y0)*0.9)+
							" C"+x+","+y+","+x+","+y+","+
							Math.round(x+(x1-x)*0.1)+","+Math.round(y+(y1-y)*0.1)+" L";
						x0 = x;
						y0 = y;
					}
					x = Math.round(option.points[len]);
					y = Math.round(option.points[len+1]);
					vmlnode += x+","+y;
					vmlnode += ' e " strokeWeight="'+option.weight+'" strokeColor="'+option.color+'" >'+
						'<v:stroke '+(option.dash ? 'dashstyle="shortDot"':'')+' ></v:stroke>'+
						'</v:shape>';
					return option.container ? option.container.html(vmlnode) : vmlnode;
				}
			}
		};
		var getUserRandomAngle = function(user)
		{
			var now = new Date();
			var min = Math.round(now.getTime()/(300*1000)); /* change per 5 minutes */
			var mac = user["mac"];
			if(mac)
			{
				var macint = parseInt(mac.substr(0,2), 16)^parseInt(mac.substr(3,2), 16)
					^parseInt(mac.substr(6,2), 16)^parseInt(mac.substr(9,2), 16)
					^parseInt(mac.substr(12,2), 16)^parseInt(mac.substr(15,2), 16);
				min += macint;
			}
			return min;
		}
		var drawAfiUser = function(map_con, user, afi, zoom)
		{			
			var userNode = $("<div class='userlocation'>");
			var scale = get_map_scale(map_con);
			var offset = (afi.width()/2)+23;
			var radius = Math.round((afi.data("coverage")/scale)*(zoom/100));/*afi coverage radius */
			var userradius = (radius > offset) ? (offset+Math.floor((1-((-user["rssi"]||0)/90))*(radius-offset))):0; /*radius wide*/
			var round = Math.floor(userradius/centimeter);
			var users_container = null;
			if(!afi.data("users_container"))
			{
				var afileft = afi.position().left;
				var afitop = afi.position().top;
				var canvas = map_con.find(".coverage-canvas");
				users_container = $("<div class='users_container'>");
				canvas.append(users_container);
				users_container.css({"left": afileft+"px", "top": afitop+"px"});
				afi.data("users_container", users_container);
			}
			else
			{
				users_container = afi.data("users_container");
			}
			if(userradius < 0 || userradius > radius)
			{
				return false;
			}
			var maxuser = (2*round+1)*6;/* */
			var hash = afi.data("userhash");
			if(!hash[round] || (hash[round] < maxuser))
			{
				var random = getUserRandomAngle(user);//Math.floor(Math.random()*maxuser);
				var x0 = users_container.width()/2;
				var y0 = users_container.height()/2;
				/*calculate paint angle and left blank room in two sides */
				
				if((user["angle"] == undefined)
					|| (undefined == user["random"]) 
					|| user["random"] != random)
				{
					user["random"] = random;
					user["angle"] = (random+Math.floor(Math.random()*maxuser)*2*Math.PI/maxuser);
				}
				var angle = (user["angle"]);//maxuser;
				//Math.round((index/*+0.1+Math.random()*0.8*/)*2*Math.PI/maxuser);
				hash[round] = (hash[round]||0)+1;
				var x = (userradius)*Math.sin(angle);
				var y = -userradius*Math.cos(angle);
				var left = (x+x0);
				var top = (y+y0);
				users_container.append(userNode);
				userNode.html("<div class='userimage'></div>");
				userNode.css({"left" : left+"px", "top" : top+"px"});
				userNode.data("mac", user["mac"]);
				user["name"] && userNode.data("name", user["name"]);
				userNode.attr("title", (user["name"]||user["mac"]));
			}
		}
		var drawAfiUsers = function(map_con, afi, zoom)
		{
			var users = afi.data("users");
			var afileft = afi.position().left;
			var afitop = afi.position().top;
			var canvas = map_con.find(".coverage-canvas");
			var users_container = $("<div class='users_container'>");
			clearAfiUsers(map_con, afi);
			afi.data("userhash", []);/* count of shown user by round */
			users_container.data("afi", afi);
			canvas.append(users_container);
			users_container.css({"left": afileft+"px", "top": afitop+"px"});
			afi.data("users_container", users_container);
			var ii = 0;
			for(ii in users){
				var user = users[ii];
				drawAfiUser(map_con, user, afi, zoom);
			};
		}
		var drawMapUsers = function(map_con, zoom)
		{			
			var afis = map_con.find(".afi-device-set");
			afis.each(function(){
				var afi = $(this);
				drawAfiUsers(map_con, afi, zoom);
			});
		}
		var clearAfiUsers = function(map_con, afi)
		{
			//var canvas = map_con.find(".coverage-canvas");
			//clearMapUsers(map_con);
			afi.data("users_container") && afi.data("users_container").empty();
		}
		var clearMapUsers = function(map_con)
		{
			map_con.find(".users_container").remove();
		}
		var unspread_spread_attaches = function()
		{
	  		var index = map_image.data("map-showidx");
	  		var shown_img_con = map_image.find("div.map-imagecontainer[index="+index+"]").first();
			var spreaddevice = shown_img_con.find(".device-set.spreaded");
			var spreads = spreaddevice.find(".dev-spread");
			spreads.each(
				function(){
					var spread = $(this);
					spread.animate({"left":"0%", "top":"0%"}, "fast",function(){
						spreaddevice.removeClass("spreaded");
						spread.hide();
					});
				}
			);
		}
		var fillCircle = function(x, y, radius, color, opacity, container, stroked)
		{
			if(hasSVG())
			{			
				var svgnode = '<svg style="left:'+(-(radius-x)+"px")+'; top:'+(-(radius-y)+"px")+
					';width:'+(2*radius)+'px;height:'+(2*radius)+'px;" pxxmlns="http://www.w3.org/2000/svg" version="1.1">'+
						'<g>'+
							'<circle cx="'+(radius)+'" cy="'+(radius)+
							'" r="'+radius+'" fill="'+color+'" fill-opacity="'+opacity+'" stroke="'+(stroked ? color:"none")+'" stroke-width="2" stroke-opacity="0.2"/>'
						'</g>'+
					'</svg>';
						
				var svgcontainer = container;				
				svgcontainer.append(svgnode);
			}
			else
			{
				var vmlcontainer = container;
				var vmlnode = '<v:oval stroked="'+(stroked?"true":"false")+'" strokeColor="'+color+'" fillColor="'+color+'" style="width:'+(2*radius)+'px;height:'+(2*radius)+'px;position:absolute;top:'+(-(radius-y))+'px;left:'+(-(radius-x))+'px;">';
				
				vmlnode += '<v:fill type="solid" opacity="'+opacity+'" origin="0.5,0.5" position="0.5,0.5" style="width:'+(2*radius)+'px;height:'+(2*radius)+'px;position:absolute;top:0px;left:0px;"></v:fill>';
				vmlnode += '<v:stroke opacity="0.2" weight="2px"></v:stroke>';
				vmlnode += '</v:oval>';
				
				vmlcontainer.append(vmlnode);
			}
		}
		var clearCircle = function(container)
		{	
			container && container.empty();
		}		
		var show_alert = function(option)
		{
			var common_alert = map.find(".map-common-alert-win");
			var iscover = false;
			option.content = option.content || "系统提示信息";
			option.title = option.title || common_alert.attr("title") || "提示信息";
			common_alert.find(".map-win-content p label").html(option.content);
			common_alert.find(".map-win-title label.map-win-title-label").html(option.title);			
			common_alert.find(".map-win-bottom .map-win-ok-button,.close-icon")
				.on("click", 
				function(){
					if(option.modal)
					{
						cover.removeClass("map-mapcover-for-alert-confirm");
						//(!iscover) && cover.hide();
					}
					common_alert.hide();
					common_alert.off("click");
					option.callback && option.callback();
				}
			);
			if(option.modal)
			{	
				iscover = cover.is(":hidden");
				cover.addClass("map-mapcover-for-alert-confirm");
				//(!iscover) && cover.show();
			}
			setpositioncenter(common_alert);
			common_alert.show();
		}
		var show_confirm = function(option)
		{
			var common_confirm = map.find(".map-common-confirm-win");
			if(!(common_confirm.data("setheaderbottom")))
			{
		    	var header_bottom = map.find(".map-model-win").html();
				common_confirm.append(header_bottom);
				common_confirm.data("setheaderbottom", true)
			}
			option.content = option.content || option.content || "确认要执行此操作吗?";
			option.title = option.title || common_confirm.attr("title") || "确认信息";
			common_confirm.find(".map-win-content p label").html(option.content);
			common_confirm.find(".map-win-title label.map-win-title-label").html(option.title);			
			common_confirm.find(".map-win-bottom .map-win-cancel-button,.close-icon")
				.on("click", 
				function(){
					common_confirm.hide();
					$(this).off("click");
					if(option.modal)
					{
						cover.removeClass("map-mapcover-for-alert-confirm");
						//(!iscover) && cover.hide();
					}
					common_alert.hide();
					common_alert.off("click");
				}
			);		
			common_confirm.find(".map-win-bottom .map-win-confirm-button")
				.on("click", 
				function(){
					option.callback && option.callback(arg);
					common_confirm.hide();
					$(this).off("click");
				}
			);
			if(option.modal)
			{	
				iscover = cover.is(":hidden");
				cover.addClass("map-mapcover-for-alert-confirm");
				//(!iscover) && cover.show();
			}
			setpositioncenter(common_confirm);
			common_confirm.show();
		}
		var get_map_scale = function(map_con)
		{
			var scale = 0;
			var map_img = map_con.find("img.map-mapimage").first();
			var unit = map_img.data("initscaleunit");
			var meter = map_img.data("initscalemeter");
			var swidth = map_img.data("initscalewidth");
			if(unit == "cm")
			{
				swidth *= 1000;
			}
			else if(unit == "km")
			{
				meter *= 1000;
			}
			scale = meter/swidth;
			return scale;
		}
		var refreshCoverage = function(x, y, radiuses, colors, opacities, container, strokeds)
		{			
			var ii = 0;
			var radius = 0;
			var opacity = 0;
			var color = 0;
			var stroked = 0;
			clearCircle(container);
			for(ii in radiuses)
			{
				radius = radiuses[ii];
				opacity = opacities[ii] || opacity;
				color = colors[ii] || color;
				stroked = strokeds[ii]||0;
				fillCircle(x, y, radius, color, opacity, container, stroked);
			}
		}
		var showCoverage = function(map_con, afi, zoom){
			var scale = get_map_scale(map_con);
			var canvas = map_con.find(".coverage-canvas");
			var afileft = afi.position().left;
			var afitop = afi.position().top;
			var radius = Math.round((afi.data("coverage")/scale)*(zoom/100));
			var radiuses = [radius, radius*3/4, radius/2];
			var colors = ["#3CA7EF", "#68C749", "#3CA7EF"];//, "#ff3333"];
			var opacities = [0.05, 0.1, 0.15];
			var strokeds = [true, false, true];
			var coverage_container = $("<div class='coverage_circle'>");
			canvas.append(coverage_container);
			coverage_container.width(afi.width());
			coverage_container.height(afi.height());
			coverage_container.css({"left":afileft,"top":afitop});
			afi.data("coverage_container") && afi.data("coverage_container").remove();
			coverage_container.data("mac", afi.data("mac"));
			afi.data("coverage_container", coverage_container);
			if(afi.data("type") == "afi" && (afi.data("state") == "run"))
			{
				refreshCoverage((afi.width()/2), (afi.height()/2), radiuses, colors, opacities, coverage_container, strokeds);
			}
		}
		var eraseCoverage = function(map_con, afi){
			//var canvas = map_con.find(".coverage-canvas");
			if(afi.data("type") == "afi")
			{
				//clearCircle(afi.data("coverage_container"));
				afi.data("coverage_container") && afi.data("coverage_container").remove();
			}
		}
		var drawScaleLine = function(option)
		{
			var scale_line_color = "red";
			var scale_line_weight = 2;
			drawLine({"points" : [option.x0, option.y0, option.x, option.y], 
					"color" : scale_line_color, "weight" : scale_line_weight, 
					"container" : (option.container||scale_canvas)});
		}
		var aa = true;
		var drawTopologyLine = function(map_con, startdev, enddev, type)
		{
			var weight = 4;
			if(!startdev.length || !enddev.length)
			{
				return false;
			}
			var afc = map_con.find(".afc-device-set").first();
			var afcx = afc.length ? afc.position().left+(afc.width()/2):0;
			var afcy = afc.length ? afc.position().top+(afc.height()/2):0;
			var canvas = map_con.find(".coverage-canvas");
			var x0 = startdev.position().left+(startdev.width()/2);
			var y0 = startdev.position().top+(startdev.height()/2);
			var x = enddev.position().left+(enddev.width()/2);
			var y = enddev.position().top+(enddev.height()/2);
			var topologyNode = $("<div class='topology_line "+type+"'>");
			canvas.append(topologyNode);
			var color = "green";
			var dash = false;
			var cross = null;
			var points = [];
			if(undefined == startdev.data("aa"))
			{
				startdev.data("aa", aa);
				aa = !aa;
			}
			switch(type)
			{
				case "connect":
					color = "#3CA7EF";
					dash = false;
					if(startdev.data("aa"))
					{/*here calculate the location of the map to decide the line endpoint 
					  x, y  is center of the dev, x-15, y-15 is the up left point, the line width is 4 
					  we allow the line endto x: 3, 8, 13, 18, 23 , x-12 means x-15+3,*/
						x = x-12+5*(Math.floor(x0*5/(map_con.width()))%5);
					}
					else
					{
						y = y-12+5*(Math.floor(y0*5/(map_con.height()))%5);
					}
					break;
				case "disconnect":
					color = "#F29C9F";
					dash = false;
					//topologyNode.css({
					//	"filter":"alpha(opacity=50)",
					//  	"-moz-opacity":"0.5",
					//  	"opacity":"0.5"
					//});
					if(startdev.data("aa"))
					{
						x = x-12+5*(Math.floor(x0*5/(map_con.width()))%5);
					}
					else
					{
						y = y-12+5*(Math.floor(y0*5/(map_con.height()))%5);
					}
					var lenx = Math.abs(x-x0);
					var leny = Math.abs(y-y0);
					/*cross  width:12 height:12, x0+(x-x0)/2 , y0+(y-y0)/2 is the center */
					var crossleft = (lenx>leny || 1)? x0+((x-x0)/2)-6 : x0-6;
					var crosstop = (lenx>leny && 0)? y-8 : y0+((y-y0)/2)-6;
					var crosscolor = "#FB1B3F";
					var crossborder = 2;
					/*paint white line as border */
					cross = drawLine({"points" : [crossleft-crossborder,crosstop-crossborder, crossleft+12+crossborder, crosstop+12+crossborder], "color" : "white", "weight" : weight+2*crossborder, 
						"dash" : dash});
					
					cross += drawLine({"points" : [crossleft-crossborder, crosstop+12+crossborder, crossleft+12+crossborder,crosstop-crossborder], "color" : "white", "weight" : weight+2*crossborder, 
						"dash" : dash});
					cross += drawLine({"points" : [crossleft,crosstop, crossleft+12, crosstop+12], "color" : crosscolor, "weight" : weight, 
						"dash" : dash});
					
					cross += drawLine({"points" : [crossleft, crosstop+12, crossleft+12,crosstop], "color" : crosscolor, "weight" : weight, 
						"dash" : dash});
					
					break;
				case "wireless":
					color = "#3CA7EF";
					dash = true;
					if(startdev.data("aa"))
					{
						x0 += (afcx > x) ? (-6) : (+6);
						x += (afcx<x0)? 6 : (-6);
					}
					else
					{
						y0 += (afcy > y) ? (-6) : (6);
						y += (afcy<y0)? 6 : (-6);
					}
					break;
				default:
					break;
			}
			if(startdev.data("aa"))
			{
				points = [x0, y0, x0, (y0+(y-y0)*0.5), x, (y0+(y-y0)*0.5), x, y];
			}
			else
			{
				points = [x0, y0, (x0+(x-x0)*0.5), y0, (x0+(x-x0)*0.5), y, x, y];
			}
			drawRoundLine({"points" : points, "color" : color, "weight" : weight, 
						"container" : topologyNode, "dash" : dash});
			
			cross && topologyNode.append(cross);
		}
		var drawTopologyConnectLine = function(map_con, startdev, enddev)
		{
			drawTopologyLine(map_con, startdev, enddev, "connect");
		}
		var drawTopologyDisconnectLine = function(map_con, startdev, enddev)
		{
			drawTopologyLine(map_con, startdev, enddev, "disconnect");
		}
		var drawTopologyWirelessLine = function(map_con, startdev, enddev)
		{
			drawTopologyLine(map_con, startdev, enddev, "wireless");
		}
		var drawTopology = function(map_con)
		{
			var afc = map_con.find(".afc-device-set").first();
			var afis = map_con.find(".afi-device-set");
			var devhash = map_con.data("devhash");
			clearTopology(map_con);
			
			afis.each(
				function()
				{
					var afi = $(this);
					if(afi.data("uplink") == "wire" && "run" == afi.data("state"))
					{
						afc && afc.length && drawTopologyConnectLine(map_con, afi, afc);
					}
					else 
					{
						afc && afc.length && drawTopologyDisconnectLine(map_con, afi, afc);
						if(afi.data("uplink") == "wireless")
						{
							var afi1 = devhash[afi.data("uplink_mac")];
							
							afi1 && drawTopologyWirelessLine(map_con, afi, afi1);
						}
					}
				}
			);
		}
		var clearTopology = function(map_con)
		{
			var canvas = map_con.find(".coverage-canvas");
			canvas.find(".topology_line,.dicconect-cross").remove();
		}
		var setScaleStartPoint = function(x, y){
			scale_canvas.data("startX", x);
			scale_canvas.data("startY", y);
			scale_canvas.data("lineStart", true);
			drawScaleLine({"x0" : x-1, "y0": y-1, "x" : x+1, "y" : y+1, "container" : scale_canvas});
		};
		var scale_canvasMousedown = function(ev){
			clearLine(scale_canvas);
			setScaleStartPoint(ev.offsetX, ev.offsetY);
			
			scaletool.find(".button.next").attr("disabled", true);
		};
		var scale_canvasMousemove = function(ev)
		{
			
			var x0 = scale_canvas.data("startX");
			var y0 = scale_canvas.data("startY");
			var x = ev.offsetX;
			var y = ev.offsetY;
			if($(this).data("lineStart"))
			{
				clearLine(scale_canvas);
				drawScaleLine({"x0" : x0, "y0": y0, "x" : x, "y" : y, "container" : scale_canvas});
			}
		}
		var scale_canvasMouseup = function(ev){
			var x0 = scale_canvas.data("startX");
			var y0 = scale_canvas.data("startY");
			var x = ev.offsetX;
			var y = ev.offsetY;
			var linepixel = 0;
			
			if(scale_canvas.data("lineStart"))
			{
				
				if($(this) == scale_canvas)
				{
					clearLine(scale_canvas);
					drawScaleLine({"x0" : x0, "y0": y0, "x" : x, "y" : y, "container" : scale_canvas});	
				}
				scale_canvas.data("lineStart", false);
				if(x0 != x || y0 != y)
				{
					linepixel = Math.pow(((x-x0)*(x-x0)+(y-y0)*(y-y0)), 1/2) || 0;
					scaletool.find(".button.next").attr("disabled", false);
				}
				scale_canvas.data("linepixel", Math.round(linepixel));
			}
		}
		var setpositioncenter = function(win)
		{
			var parent = win.parent();
			var width = win.width();
			var height = win.height();
			win.css("left", ((parent.width()-width)/2)+"px");
			win.css("top", ((parent.height()-height)/2)+"px");
		}
    	var refreshScale = function(slidervalue){
	  		var index = map_image.data("map-showidx");
	  		var shown_img_con = map_image.find("div.map-imagecontainer[index="+index+"]").first();
			var shown_img = shown_img_con.find("img.map-mapimage").first();
			var meter = shown_img.data("initscalemeter");
			var width = parseInt(shown_img.data("initscalewidth")*(parseInt(slidervalue)/100));
			var scales=[1, 2, 5, 10, 20, 50, 100, 200, 500, 1000];
			var unit = shown_img.data("initscaleunit");
    		if(width < centimeter)
			{/* scale < 1cm */
				while(width < centimeter)
				{
					if(unit == "cm" && (meter) == 50)
					{
						meter = Math.round(meter/50);
						width *= 2;
						unit = "m";
					}
					else if(unit == "m" && (meter) == 500)
					{
						meter = Math.round(meter/500);
						width *= 2;			
						unit = "km";
					}
					else
					{
						var j = 0;
						while(j < scales.length-1 && (meter) != scales[j])
						{
							j++;
						}
						if((meter) != scales[j])
						{
							return false;
						}
						width *= (scales[j+1]/(meter));
						meter = Math.round(meter*(scales[j+1]/(meter)));
					}
				}
			}
			else
			{				
				var change = 1;
				while(change)
				{
					change = 0;
					var j = 0;
					while(j < scales.length && (meter) != scales[j])
					{
						j++;
					}
					if((meter) != scales[j])
					{
						return false;
					}
					if(0 == (j%2))
					{
						if(width >= 2*centimeter )
						{
							change = 1;
							if(unit == "km" && (meter) == 1)
							{
								meter = 500;
								width /= 2;
								unit = "m";
							}
							else if(unit == "m" && (meter) == 1)
							{
								meter = 50;
								width /= 2;
								unit = "cm";
							}
							else if(!(unit == "cm" && (meter) == 1))
							{
								meter = Math.round(meter/2);
								width /= 2;
							}
							else
							{
								change = 0;
							}
						}
					}
					else
					{
						if(width >= 5*centimeter )
						{
							while(width > 5*centimeter)
							{
								change = 1;
								meter = Math.round(meter/5);
								width /= 5;
							}
						}
					}
				}
			}
			scale.find("span.map-scaletext").html(meter);
			main.find("span.map-scaleunit").html(unit);
			scale.width(Math.round(width));
			return true;
    	}
		var mapafc_refresh_info = function(mapafc, data)
		{
			var name = data.name;
			var mac = data.mac;
			var model = data.model;
			var state = data.state;
			var score = data.score;
			var type = "afc";
			var img = "/media/global/u3030.png";
			//img = Afc.get_img(model, state);
			mapafc.data("mac", mac);
			mapafc.data("name", name);
			mapafc.data("model", model);
			mapafc.data("state", state);
			mapafc.data("score", score);
			mapafc.data("img", img);
			var detail = (mapafc.data("name")||(mapafc.data("mac")))+"<br/><br/>"+
				"Mac:"+mapafc.data("mac")+"<br/>"+
				"State:"+mapafc.data("state")+"<br/>"+
				"Score:"+mapafc.data("score")+"<br/>";
			
			mapafc.find(".labeldiv .info").html(detail);
		}
		var set_afi_light = function(afi, color)
		{
			if(afi.find(".afi-device-light").length)
			{
				switch(color)
				{
					case "blue":
						afi.find(".afi-device-light").get(0).className = "afi-device-light blue";
						break;
					case "green":
						afi.find(".afi-device-light").get(0).className = "afi-device-light green";
						break;
					case "red":
						afi.find(".afi-device-light").get(0).className = "afi-device-light red";
						break;
					case "orange":
					case "yellow":
						afi.find(".afi-device-light").get(0).className = "afi-device-light orange";
						break;
					default:
						afi.find(".afi-device-light").get(0).className = "afi-device-light none";
				}
			}
		}
		var set_afi_state = function (afi, state)
		{
			afi.data("state", state);
			if(afi.data("state") == "run")
			{
				set_afi_light(afi, "blue");
			}
			else
			{
				set_afi_light(afi, "red");
			}
		}
		var mapafi_refresh_info = function(mapafi, data)
		{
			var name = data.name;
			var mac = data.mac;
			var model = data.model;
			var channel = data.channel;
			var num_sta = data.num_sta;
			var img = "";//"/media/global/afi_empty_light.png";
			//img = Afi.get_img(model, state);
			mapafi.data("mac", mac);
			mapafi.data("name", name);
			mapafi.data("channel", channel);
			mapafi.data("num_sta", num_sta);
			mapafi.data("img", img);
			mapafi.data("coverage", 10);
			mapafi.data("uplink", (data.uplink||"wire"));
			//mapafi.data("state", (data.state||"init"));
			var testuser = {"name":"testuser", "mac":"00:12:34:56:78:9A", "rssi":-76};
			var testuser2 = {"name":"testuser2", "mac":"00:12:34:56:78:9B", "rssi":-78};
			var testuser3 = {"name":"testuser3", "mac":"00:12:34:56:78:9C", "rssi":-68};
			var testuser4 = {"name":"testuser4", "mac":"00:12:34:56:78:9F", "rssi":-89};
			mapafi.data("users", [testuser, testuser2, testuser3, testuser4]);
			if((data.uplink == "wireless") && data.uplink_mac)
			{
				mapafi.data("uplink_mac", data.uplink_mac);
			}
			var detail = (mapafi.data("name")||(mapafi.data("mac")))+"<br/><br/>"+
				"Mac:"+mapafi.data("mac")+"<br/>"+
				"Channel:"+mapafi.data("channel")+"<br/>"+
				"Users:"+mapafi.data("num_sta")+"<br/>";
			
			mapafi.find(".labeldiv .info").html(detail);		
			set_afi_state(mapafi, (data.state||"init"));
		}
		var create_unplaced_dev = function(type, data){
			var unplaceddevset = $("<div class='device-set "+type+" "+type+"-device-set map-unplaceddevice'>");
			var unplaceddev = $("<div class='device draggable-handle'>");
			var devlabeldiv = $("<div class='attach move-semi-transparent labeldiv'>");
			var devdetail = ("<label class='draggable-handle label info'>");
			var devlock = $("<div class='attach dev-spread move-semi-transparent lock'>");
			var devconfig = $("<div class='attach dev-spread move-semi-transparent config'>");
			var devdelete = $("<div class='attach dev-spread move-semi-transparent delete'>");
			//var devcover = $("<img class='attach move-semi-transparent coverage' >");
			//devlabeldiv.append(devlabel);
			unplaceddevset.append(unplaceddev);
			unplaceddevset.append(devlabeldiv);
			//devlabeldiv.append(devdetail);
			devlabeldiv.html(devdetail);
			unplaceddevset.append(devlock);
			unplaceddevset.append(devconfig);
			unplaceddevset.append(devdelete);
			unplaceddevset.data("type", type);
			if(type == "afi")
			{
				unplaceddevset.append($("<div class='afi-device-light'>"));
				mapafi_refresh_info(unplaceddevset, data);
			}
			else if (type == "afc")
			{
				mapafc_refresh_info(unplaceddevset, data);
			}
			unplaceddev.css({"background-image":"url("+unplaceddevset.data("img")+")",
				"background-repeat":"no-repeat"});

			//devdetail.show();
			//devlabel.disableSelection();
			devlabeldiv.find("label").disableSelection();
			devlabeldiv.show();
			//devlock.html("<label>锁</label>");
			//devconfig.html("<label>配</label>");
			//devdelete.html("<label>删</label>");
			devlock.hide();
			devconfig.hide();
			devdelete.hide();
			//devcover.hide();
			unplaceddev.show();			
			unplaceddevset.draggable({handle:".draggable-handle", revert: "invalid",
				drag:function(){
					var zoom = map.find(".map-zoom");
					var zoomvalue = zoom.slider("value");
					$(this).data("initleft", ($(this).position().left+($(this).width()/2))*(100/zoomvalue));
					$(this).data("inittop", ($(this).position().top+($(this).height()/2))*(100/zoomvalue));
					$(this).data("coverage_container") && ($(this).data("coverage_container").css({
							"left":$(this).position().left, 
							"top": $(this).position().top}));
					$(this).data("users_container") && ($(this).data("users_container").css({
							"left":$(this).position().left, 
							"top": $(this).position().top}));
					var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
					if($("div.map-tool-show-buttons input.topology").is(":checked"))
					{
						drawTopology(show_img_con);
					}
				}
			});
			
			return unplaceddevset;
		}
		var place_dev_to_map = function(targetdev, targetmap)
		{
			targetdev.find(".attach").hide();
			targetdev.addClass("map-placeddevice");
			targetdev.removeClass("map-unplaceddevice");
			//targetdev.find("*").removeClass("map-unplaceddevice");
			//targetdev.find("*").addClass("map-placeddevice");
			var offsetX = targetdev.parent().offset().left - targetmap.offset().left;
			var offsetY = targetdev.parent().offset().top - targetmap.offset().top;
			targetdev.find(".labeldiv .label").removeClass("draggable-handle");
			targetdev.css({"position":"absolute", "left":(targetdev.position().left+offsetX)+"px", 
				"top":(targetdev.position().top+offsetY)+"px", "z-index":2});
			
			targetdev.appendTo(targetmap);		
			targetdev.draggable( "option", "containment", "parent" ); 

			
			//targetafi.css({"left":(targetafi.position().left+offsetX)+"px", 
			//	"top":(targetafi.position().top+offsetY)+"px", "z-index":2});
			var zoom = map.find(".map-zoom");
			var zoomvalue = zoom.slider("value");
			targetdev.data("initleft", ((targetdev.position().left+(targetdev.width()/2))*(100/zoomvalue)));
			targetdev.data("inittop", ((targetdev.position().top+(targetdev.height()/2))*(100/zoomvalue)));
			var spreaddevice = targetmap.find(".device-set.spreaded");
			targetdev.find(".device").on("mousedown", 
				function(ev)
				{
					var dev = $(this);
					if(dev.parent().hasClass("map-placeddevice"))
					{
						//var afi = afiset.find(".afi-device");
						var width = dev.width();
						var height = dev.height();
						var devleft = dev.position().left;
						var devtop = dev.position().top;
						var i = 0;
						var spreaddevice = targetmap.find(".device-set.spreaded");
						if(dev.parent() != spreaddevice)
						{
							spreaddevice.removeClass("spreaded");
							spreaddevice.find(".dev-spread").css("left", 0);
							spreaddevice.find(".dev-spread").css("top", 0);
							spreaddevice.find(".dev-spread").hide();
						}
						var spreads = dev.parent().find(".dev-spread");
						dev.parent().addClass("spreaded");
						spreads.show();
						spreads.each(
							function(){
								var spread = $(this);
								spread.data("initleft") || spread.data("initleft", spread.position().left);
								spread.data("inittop") || spread.data("inittop", spread.position().top);
								spread.css("left", "0px");
								spread.css("top", "0px");
								var left = spread.data("initleft")+devleft;
								var top = spread.data("inittop")+devtop;
																
								spread.animate({"left":left+"px", "top":top+"px"}, "fast");
								i++;
							}
						);
					}
				}
			);
			if(targetdev != spreaddevice)
			{
				spreaddevice.removeClass("spreaded");
				spreaddevice.find(".dev-spread").css("left", 0);
				spreaddevice.find(".dev-spread").css("top", 0);
				spreaddevice.find(".dev-spread").hide();
				targetdev.find(".device").trigger("mousedown");
			}
			// check and open label coverage topology etc. here 
			targetmap.find("img.map-mapimage").on("mousedown", unspread_spread_attaches);
			targetmap.find(".coverage-canvas").on("mousedown", unspread_spread_attaches);
			var devdelete = function(){mapdev_unplace_from_map(targetmap, targetdev);};
			targetdev.find(".delete").on("click", devdelete);
			targetdev.find(".lock,.unlock").on("click", function(){
				if(!targetdev.data("lock"))
				{
					targetdev.draggable("disable");			
					targetdev.find(".delete").off("click");
					targetdev.find(".config").off("click");
					targetdev.data("lock", true);
					targetdev.find(".lock").removeClass("lock").addClass("unlock");
					//targetdev.find(".lock").html("<label>开</label>");
				}
				else
				{
					targetdev.draggable("enable");				
					targetdev.find(".delete").on("click", devdelete);
					targetdev.find(".config").on("click");
					targetdev.data("lock", false);
					targetdev.find(".unlock").removeClass("unlock").addClass("lock");
					//targetdev.find(".lock").html("<label>锁</label>");
				}
			});
			targetdev.find(".config").on("click", function(){
				//afi_dialog_show(targetdev.data("mac"));
			});
			var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
			if($("div.map-tool-show-buttons input.detail").is(":checked"))
			{
				show_img_con.find(".device-set .attach.labeldiv .label")
					.removeClass("label").addClass("detail").show();
				show_img_con.find(".device-set .attach.labeldiv").show();
			}
			else if($("div.map-tool-show-buttons input.label").is(":checked"))
			{
				show_img_con.find(".device-set .attach.labeldiv .detail")
					.removeClass("detail").addClass("label").show();
				show_img_con.find(".device-set .attach.labeldiv").show();
			}
			else
			{
				show_img_con.find(".device-set .attach.labeldiv").hide();
			}
			if(targetdev.data("type") == "afi")
			{
				if($("div.map-tool-show-buttons input.coverage").is(":checked"))
				{
			    	var zoom = zoomsets.find(".map-zoom");
					showCoverage(show_img_con, targetdev, zoom.slider("value"));
				}
				if($("div.map-tool-show-buttons input.usershow").is(":checked"))
				{
			    	var zoom = zoomsets.find(".map-zoom");
					drawAfiUsers(show_img_con, targetdev, zoom.slider("value"));
				}
				var mac = targetdev.data("mac");
				var devhash = show_img_con.data("devhash");
				mac && devhash && (show_img_con.data("devhash")[mac] = targetdev);
			}
			if($("div.map-tool-show-buttons input.topology").is(":checked"))
			{
				drawTopology(show_img_con);
			}
		}
		var mapdev_unplace_from_map = function(targetmap, targetdev)
		{
			var unplacedcontainer = unp.find(".map-unplaceddevices");
			targetdev.removeClass("map-placeddevice");
			//targetdev.find("*").removeClass("map-placeddevice");
			targetdev.css({"position":"relative", "left":"10px", "top":"5px"});
			targetdev.addClass("map-unplaceddevice");
			//targetdev.find("*").addClass("map-unplaceddevice");
			if(targetdev.hasClass("afc"))
			{
				targetdev.prependTo(unplacedcontainer);
			}
			else
			{
				targetdev.appendTo(unplacedcontainer);
			}
			targetdev.draggable({handle:".draggable-handle", revert: "invalid"});
			targetdev.draggable( "option", "containment", "document" ); 
			targetdev.removeClass("spread");
			targetdev.find(".attach").hide();
			targetdev.find(".attach.labeldiv").show();
			targetdev.find(".attach .detail").removeClass("detail").addClass("label");
			targetdev.find(".attach .label").addClass("draggable-handle");
			//targetdev.find(".labeldiv .label").addClass("draggable-handle");
			targetdev.off("focus");
			targetdev.off("blur");
			//targetdev.off("mousedown");
			targetdev.find(".delete").off("click");
			targetdev.find(".lock").off("click");
			targetdev.find(".config").off("click");
			if(targetdev.data("type") == "afi")
			{
				eraseCoverage(targetmap, targetdev);
				clearAfiUsers(targetmap, targetdev);
				var mac = targetdev.data("mac");
				var devhash = targetmap.data("devhash");
				mac && devhash && (delete targetmap.data("devhash")[mac]);
			}
			if($("div.map-tool-show-buttons input.topology").is(":checked"))
			{
				drawTopology(targetmap);
			}
		}
	    var refreshposition = function(){
			unpwidth = unp.outerWidth();
			
	    	if(!unpishide)
	    	{
				unp.css("left", (main.width()-unpwidth)+"px");
			}
			else
			{
				unp.css("left", (main.width())+"px");
			}
			
			hide.css("left", (parseInt(unp.css("left")) - hide.width()) +"px");
		  	scale.css("bottom", (/*main.height()-scale.height()-*/10)+"px");
		  	
		}
		var map_operation_to_server = function(option, callback){
			
			$.post("/api/map_upload.php", {json: $.toJSON(option)},
				function(f, g, h){
					if(h.status != 200)
					{
						show_alert(MapOpMsg[g.msg]||g.msg, "出错信息");
						return false;
					}
					g = f.meta;
					if("error" == g.rc)
					{
						show_alert(MapOpMsg[g.msg]||g.msg, "出错信息");
					}
					else
					{
						var data = f.data;
						load_mapimg_list(data);
						loadmaplist();
						callback && callback();
					}
				}, 
			'json');
		}
	    zoomsets.each(
		    function(){
		    	var zoomset = $(this);
		    	var zoom = zoomset.find(".map-zoom");
		    	
		    	var setpercent = function(val)
		    	{
		    		zoomset.find("label.map-zoompercent").html(parseInt(val)+"%");
		    	};
		    	var sliderChange = function(a, e)
		    	{
		    		setpercent(parseInt(e.value));
		    		
			  		var index = map_image.data("map-showidx");
			  		var shown_img_con = map_image.find("div.map-imagecontainer[index="+index+"]").first();
			  		var shown_img = shown_img_con.find("img.map-mapimage").first();
					var cleft = parseInt(shown_img_con.parent().width()/2);
					var ctop = parseInt(shown_img_con.parent().height()/2);
					var initleft = shown_img_con.data("initleft");
					var inittop = shown_img_con.data("inittop");
		    		shown_img.width(parseInt(shown_img.data("initwidth")*parseInt(e.value)/100));
					shown_img.height(parseInt(shown_img.data("initheight")*parseInt(e.value)/100));
		    		shown_img_con.width(parseInt(shown_img.data("initwidth")*parseInt(e.value)/100));
					shown_img_con.height(parseInt(shown_img.data("initheight")*parseInt(e.value)/100));
					shown_img.data("zoom", parseInt(e.value));
					shown_img_con.css("left", parseInt(cleft-((cleft-initleft)*(parseInt(e.value)/100)))+"px");
					shown_img_con.css("top", parseInt(ctop-((ctop-inittop)*(parseInt(e.value)/100)))+"px");
					var mapelement = shown_img_con.find("div.map-placeddevice.device-set");
					mapelement.each(
						function(){
							var dev = $(this);
							dev.css({"left":(dev.data("initleft")*(e.value/100)-(dev.width()/2))+"px", 
								"top":(dev.data("inittop")*(e.value/100)-(dev.height()/2))+"px"});
							
							if($("div.map-tool-show-buttons input.coverage").is(":checked") && dev.data("type") == "afi")
							{
								showCoverage(shown_img_con, dev, e.value);
							}
						}
					);
					if($("div.map-tool-show-buttons input.topology").is(":checked"))
					{
						drawTopology(shown_img_con);
					}
					
					if($(("div.map-tool-show-buttons input.usershow")).is(":checked"))
					{
						var zoom = zoomsets.find(".map-zoom");
						drawMapUsers(shown_img_con, zoom.slider("value"));			
					}
					refreshScale(parseInt(e.value));
					return true;
		    	};
		    	zoom.slider({
			      orientation: "vertical",
			      range: "max",
			      min:50,
			      max: 150,
			      value: 100,
			      slide: sliderChange,
			      change: sliderChange
			   });
				setpercent(zoom.slider("value"));/* text  xx% */
	    	}
	    );
	    main.each(function(){
	    	var panel = $(this);
	    	hide.click(function(){
				unpwidth = unp.outerWidth();
	    		var nleft = (unpishide ? (main.width()-unpwidth):main.width());
	    		
	    		unp.animate({left:nleft+"px"}, "slow");				  
				$(this).animate({left: (nleft-hide.width())+"px"}, "slow");
				$(this).find("label")[0].innerHTML = unpishide ? "&gt;&gt;":"&lt;&lt;";
				unpishide = !unpishide;
			});
			unp.resizable({ handles: 'w' });
			var input = scaletool.find(".second-step input.map-distanceinput");
			input.change(
				function(){
					scaletool.find(".button.confirm").attr("disabled", 
						(($(this).val() == "")|| isNaN($(this).val()) ||(0.0 == parseFloat($(this).val()))));
				}
			);
			input.keyup(
				function(ev){
					scaletool.find(".button.confirm").attr("disabled", 
						(($(this).val() == "")|| isNaN($(this).val()) || (0.0 == parseFloat($(this).val()))));
				}
			);

			setscale.click(function(){
				//var cover = main.find(".map-maincover.cover-for-scaletool");
				cover.addClass("map-maincover");
				//cover.show();
				clearLine(scale_canvas);
				scale_canvas.show();
				input.val("");
				scaletool.find(".button.next").attr("disabled", true);
				scaletool.find(".button.confirm").attr("disabled", true);
				scaletool.show();
				scaletool.find(".first-step").show();
				scaletool.find(".second-step").hide();
				//prepare for draw line
				scale_canvas.data("lineStart", false);
				scale_canvas.on("mousedown", scale_canvasMousedown);
				scale_canvas.on("mousemove", scale_canvasMousemove);
				scale_canvas.on("mouseup", scale_canvasMouseup);
				
			});
			scaletool.find(".button.cancel").click(function(){
				//var cover = main.find(".map-maincover.cover-for-scaletool");
				scaletool.hide();
				//cover.hide();
				cover.removeClass("map-maincover");
				scale_canvas.hide();
				scale_canvas.data("lineStart", false);
			});
			scaletool.find(".button.next").click(function(){
				//var cover = main.find(".map-maincover.cover-for-scaletool");
				if(scale_canvas.data("lineStart"))
				{
					return false;
				}
				scaletool.find(".first-step").hide();
				scaletool.find(".button.confirm").attr("disabled", 
					((input.val() == "")|| isNaN(input.val()) || (0.0 == parseFloat(input.val()))));
				scaletool.find(".second-step").show();
				scale_canvas.off("mousedown");
				scale_canvas.off("mousemove");
				scale_canvas.off("mouseup");
				input.focus();
			});
			scaletool.find(".button.back").click(function(){
				scaletool.find(".first-step").show();
				scaletool.find(".second-step").hide();
				//prepare for draw line
				scale_canvas.on("mousedown", scale_canvasMousedown);
				scale_canvas.on("mousemove", scale_canvasMousemove);
				scale_canvas.on("mouseup", scale_canvasMouseup);
			});
			scaletool.find(".button.confirm").click(function(){
				
				var distance = parseFloat($(".map-distanceinput").val());
				var linepixel = scale_canvas.data("linepixel");
				var zoom = map.find(".map-zoom");
				var zoomvalue = zoom.slider("value");
				var result = ((centimeter*distance*zoomvalue)/(100*linepixel));
				//input.val("");
				scaletool.find(".button.confirm").attr("disabled",true);
				input.select().focus();
				//clearLine(scale_canvas);
				if(!(distance && linepixel))
				{
					show_alert({
						"content":"设置参数无效!", 
						"title":"设置出错",
						"modal":true, 
						"callback":function(){input.select().focus();}
					});
					return false;
				}
				var unit = "";
				var width = centimeter;
				if(result > 500)
				{
					result = Math.round(result/1000);
					if(result > 100)
					{
						show_alert({
							"content":"比例尺设置过大!", 
							"title":"设置出错",
							"modal":true, 
							"callback":function(){input.select().focus();}
						});
						return false;
					}
					unit = "km";
				}
				else if(result > 0.5)
				{
					unit = "m";
				}
				else
				{	
					result = Math.round(100*result);
					unit = "cm";
					if(result < 1)
					{
						show_alert({
							"content":"比例尺设置过小!", 
							"title":"设置出错",
							"modal":true, 
							"callback":function(){input.select().focus();}
						});
						return false;
					}
				}
				var scales=[1, 5, 10, 50, 100, 500, 1000];
				var j = 0;
				while(j < scales.length-1 && result > scales[j])
				{
					j++;
				}
				if(result > scales[j])
				{
					width = centimeter;
				}
				else
				{
					width = (scales[j]/result)*centimeter;
					result = scales[j];
				}
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				var show_img = show_img_con.find("img.map-mapimage").first();
				show_img.data("initscalemeter", result);
				show_img.data("initscalewidth", width);
				show_img.data("initscaleunit", unit);
				show_img_con.find(".afi-device-set").each(function()
				{
					var dev = $(this);
					if($("div.map-tool-show-buttons input.coverage").is(":checked") && dev.data("type") == "afi")
					{
				    	var zoom = zoomsets.find(".map-zoom");
						showCoverage(show_img_con, dev, zoom.slider("value"));
					}
				});
				refreshScale(zoom.slider("value"));
				if($("div.map-tool-show-buttons input.usershow").is(":checked"))
				{
					drawMapUsers(show_img_con, zoom.slider("value"));
				}
				scale_canvas.data("linepixel", 0);
				//var cover = main.find(".map-maincover.cover-for-scaletool");
				scaletool.find(".second-step").hide();
				scaletool.hide();
				cover.removeClass("map-maincover");
				//cover.hide();
				scale_canvas.hide();
				return true;
				
			});
	    	$(window).resize(refreshposition);
	    });	    
		
		refreshposition();

		var selec = map.find(".map-tool-map select");
		var setimgconcenter = function(imgcon)
		{
			var img = imgcon.find("img.map-mapimage").first();
  			var offsetx = (map_image.width()-img.width())/2;
  			var offsety = (map_image.height()-img.height())/2;
			var zoom = map.find(".map-zoom");
			var zoomvalue = zoom.slider("value");
			var initleft = (map_image.width() - (img.width()*(100/zoomvalue)))/2;
			var inittop = (map_image.height() - (img.height()*(100/zoomvalue)))/2;
			
  			/*img.css({
  				"position":"absolute", "z-index":"1",
  				"left":"0px",
  				"top":"0px"
  			});*/
			imgcon.css({
  				"left":(offsetx/*+parseInt(map_image.css("left"))*/)+"px",
  				"top":(offsety/*+parseInt(map_image.css("top"))*/)+"px",
  				"width":(img.width()+"px"),
  				"height":(img.height()+"px")
  			});
			setpositioncenter(imgcon);
  			img.data("initwidth", img.width());
  			img.data("initheight", img.height());
  			imgcon.data("initleft", initleft);
  			imgcon.data("inittop", inittop);
				img.show();
  			if(imgcon.attr("index") == map_image.data("map-showidx"))
  			{
				imgcon.show();
  			}
			else
			{
				imgcon.hide();
			}
		}
		var load_mapimg_list = function(data)
		{			
			selec.empty();
			map_image.empty();
		  	if(data.length > 0)
		  	{
		  		var i = 0;
		  		var count = 0;
				var zoom = map.find(".map-zoom");
				zoom.slider("value", 100);
		  		for(i = 0; i < data.length; i++)
		  		{
		  			if(!data[i].url)
		  			{
		  				continue;
		  			}
		  			var img = $("<img class='map-mapimage'>");
					var imgcontainer = $("<div class='map-imagecontainer'>");
					var coverage_container = $('<div class="canvas coverage-canvas">');
		  			img.attr("src", data[i].url);
		  			img.attr("alt", data[i].url);
		  			map_image.append(imgcontainer);
					imgcontainer.append(coverage_container);
					imgcontainer.css({
		  				"left":"0px",
		  				"top":"0px",
		  				"width":"200%",
		  				"height":"200%"
		  			});
					imgcontainer.append(img);
					imgcontainer.data("subfix", getsubfix(data[i].url));
		  			img.data("zoom", 100);
					imgcontainer.attr("index", count);
					imgcontainer.data("devhash", (new Array()));
		  			img.attr("index", count);
		  			imgcontainer.draggable({cancel:".device-set .attach,.device", drag:function(){
							var cleft = parseInt(imgcontainer.parent().width()/2);
							var ctop = parseInt(imgcontainer.parent().height()/2);
							var zoom = map.find(".map-zoom");
							var zoomvalue = zoom.slider("value");
							$(this).data("initleft", (cleft-((cleft-$(this).position().left)*(100/zoomvalue))));
							$(this).data("inittop", (ctop-((ctop-$(this).position().top)*(100/zoomvalue))));
						}
					});
						
					img.data("initscalewidth", centimeter);
		  			img.data("initscalemeter", 1);//load from server 
					img.data("initscaleunit", "m");
		  			selec.append("<option value='"+count+"'>"+(data[i].name ? data[i].name : "null")+"</option>");
					
		  			img.hide();
					
					//imgcontainer.bind("drag", (function(){if($(this).data("candrag")){return true;}else{return false;}}));
					//imgcontainer.data("candrag", true);
		  			
		  			count++;
					imgcontainer.droppable({drop:function(ev,ui){
						if(ui.draggable.hasClass("map-unplaceddevice"))
						{
							place_dev_to_map(ui.draggable, $(this));
						}
					}});
					
					//img.load(function(){setimgconcenter($(this).parent())});
		  		}
		  		
		  		scale.css("bottom"/*"top"*/, /*(main.height()-scale.height()-10)*/10+"px");
		  		if(count > 0)
		  		{
			  		map_image.data("map-count", count);
			  		map_image.data("map-showidx", 0);
			  		selec.val(0);
			  	}
		  		
	  			var after_load = function(){
					var container = map_image.find(".map-imagecontainer");
	  				container.each(function(){
	  					var imgcon = $(this);
						setimgconcenter(imgcon);
		  			});
	  			};
	  			
	  			if(count > 0)
				{
					setTimeout(after_load, count*1500);
					//$(window).load(after_load);
				}
			}
		}
		var getsubfix = function (filename)
		{ /* get the filename of the url */
			var match = filename.match(/.*\.([a-zA-Z\-]+)\s*$/);
		    return match[1] ? match[1] : "";
		}
		$.post("/api/list_map.php", {json:""},
			function (a, g, h) {
			  	var data = a.data;
				if(h.status != 200)
				{
					return false;
				}
			  	load_mapimg_list(data);
			}
			,"json"
		);
		var unplacedcontainer = unp.find(".map-unplaceddevices");
		var afcdata = {"mac":"00:1f:64:00:00:01", 
			"name":"AFi-Controller", 
			"model":"AFC",
			"state":"Normal", 
			"score":"98"};
		var unplacedafc = create_unplaced_dev("afc", afcdata);
		unplacedcontainer.append(unplacedafc);
		$.post("/api/stat_device.php", {json:""},
			function(a, g, h){
				var data = a.data;
				var i = 0;				
				
				for(i = 0; i < data.length; i++)
				{
					if(!(data[i].adopted))
					{
						continue;
					}
					var unplacedafi = create_unplaced_dev("afi", data[i]);
					unplacedcontainer.append(unplacedafi);
					//unplacedafi.draggable({handle:".draggable-handle", revert: "invalid"});
				}
				
				var tmpafidata = {"mac":"00:1f:64:00:00:02", 
					"name":"tmpafi", 
					"model":"AFi",
					"channel":"6", 
					"state":"run",
					"num_sta":"2",
					"uplink":"wire"};
				var tmpafi = create_unplaced_dev("afi", tmpafidata);
				unplacedcontainer.append(tmpafi);
				var tmpafidata2 = {"mac":"00:1f:64:00:00:03", 
					"name":"tmpafi2", 
					"model":"AFi",
					"channel":"6", 
					"state":"quit",
					"num_sta":"2",
					"uplink":"wireless",
					"uplink_mac":"00:1f:64:00:00:02"};
				var tmpafi2 = create_unplaced_dev("afi", tmpafidata2);
				tmpafidata = {"mac":"00:1f:64:00:00:07", 
					"model":"AFi",
					"channel":"6",
					"uplink":"wire",
					"state":"run", 
					"num_sta":0,
					"name":"tmp3"};
				var tmpafi3 = create_unplaced_dev("afi", tmpafidata);				
				tmpafidata = {"mac":"00:1f:64:00:00:0B", 
					"model":"AFi",
					"channel":"6",
					"uplink":"wire",
					"state":"run", 
					"num_sta":0,
					"name":"tmp4"};
				var tmpafi4 = create_unplaced_dev("afi", tmpafidata);
				tmpafidata = {"mac":"00:1f:64:00:00:0F", 
					"model":"AFi",
					"channel":"6",
					"uplink":"wire",
					"state":"run", 
					"num_sta":0,
					"name":"tmp5"};
				var tmpafi5 = create_unplaced_dev("afi", tmpafidata);
				unplacedcontainer.append(tmpafi2);
				unplacedcontainer.append(tmpafi3);
				unplacedcontainer.append(tmpafi4);
				unplacedcontainer.append(tmpafi5);
			}
		, "json");
		selec.change(
			function()
			{
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				var hide_img_con = map_image.find("div.map-imagecontainer[index="+map_image.data("map-showidx")+"]").first();
				var show_img = show_img_con.find("img.map-mapimage").first();
				
				hide_img_con.hide();
				show_img_con.show();
				map_image.data("map-showidx", selec.val());
				
		    	var zoom = map.find(".map-zoomset .map-zoom");
		    	//var zoomvalue = 0;
		    	var zoomvalue = show_img.data("zoom");
		  		//show_img.data("initscalewidth", centimeter);
		  		
		    	zoom.slider("value", zoomvalue?zoomvalue:100);
		    	refreshScale(zoom.slider("value"));
		    	
			}
		);
		cont.mousewheel(
			function(a, d){
				var zoom = map.find(".map-zoom");
				var old = zoom.slider("value");
				zoom.slider("value", old+d);
			}
		);

		    scaletool.disableSelection();
			
			var which = ["label", "detail", "coverage", "usershow", "topology"];
			var i = 0;
			while(i < which.length)
			{
				if(!map.find("div.map-tool-show-buttons input."+which[i]).attr("id"))
				{
					var j = 0;
					while($("#checkbox_"+which[i]+(j||"")).length)
					{
						j++;
					}
					map.find("div.map-tool-show-buttons input."+which[i]).attr("id", "checkbox_"+which[i]+(j||""))
					map.find("div.map-tool-show-buttons label.checkbox-"+which[i]).attr("for", "checkbox_"+which[i]+(j||""))
				}
				i++;
			}
			map.find("div.map-tool-show-buttons input.map-tool-checkbox").button();
			map.find("div.map-tool-show-buttons input.label").click(function(){
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				if(($(this).is(":checked")) && map.find("div.map-tool-show-buttons input.detail").is(":checked"))
				{
					map.find("div.map-tool-show-buttons input.detail").click();
				}
				if($(this).is(":checked"))
				{
					show_img_con.find(".device-set .attach.labeldiv .detail")
						.removeClass("detail").addClass("label").show();
					show_img_con.find(".device-set .attach.labeldiv").show();
				}
				else
				{
					show_img_con.find(".device-set .attach.labeldiv").hide();
				}
			});	
			map.find("div.map-tool-show-buttons input.detail").click(function(){
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				if(($(this).is(":checked")) && map.find("div.map-tool-show-buttons input.label").is(":checked"))
				{
					map.find("div.map-tool-show-buttons input.label").click();
				}
				if($(this).is(":checked"))
				{
					show_img_con.find(".device-set .attach.labeldiv .label")
						.removeClass("label").addClass("detail").show();
					show_img_con.find(".device-set .attach.labeldiv").show();
				}
				else
				{
					show_img_con.find(".device-set .attach.labeldiv").hide();
				}
			});	
			map.find("div.map-tool-show-buttons input.coverage").click(function()
			{
				if($(this).is(":checked"))
				{
					var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
					
					show_img_con.find(".afi-device-set").each(
						function()
						{
							var afi = $(this);
					    	var zoom = zoomsets.find(".map-zoom");
							showCoverage(show_img_con, afi, zoom.slider("value"));
						}
					);
				}
				else
				{
					var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
					
					show_img_con.find(".afi-device-set").each(
						function()
						{
							var afi = $(this);
							eraseCoverage(show_img_con, afi);
						}
					);
				}
			});
			map.find("div.map-tool-show-buttons input.usershow").click(function(){
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				if($(this).is(":checked"))
				{
					var zoom = zoomsets.find(".map-zoom");
					drawMapUsers(show_img_con, zoom.slider("value"));			
				}
				else
				{
					clearMapUsers(show_img_con);
				}
			});
			map.find("div.map-tool-show-buttons input.topology").click(function()
			{
				var show_img_con = map_image.find("div.map-imagecontainer[index="+selec.val()+"]").first();
				if($(this).is(":checked"))
				{
					drawTopology(show_img_con);		
				}
				else
				{
					clearTopology(show_img_con);
				}
			});		
			
		    var header_bottom = map.find(".map-model-win").html();
			var header = map.find(".map-model-win .map-win-title").get(0).outerHTML;						
			
			var add_win = map.find(".map-add-win").append(header_bottom);
			var config_win = map.find(".map-config-win").append(header);
			var edit_win = map.find(".map-edit-win").append(header_bottom);
			var delete_confirm_win = map.find(".map-delete-confirm-win").append(header_bottom);
			//add_win.find("label.map-win-title-label").html(add_win.attr("title"));
			//edit_win.find("label.map-win-title-label").html(edit_win.attr("title"));
			//delete_confirm_win.find("label.map-win-title-label").html(delete_confirm_win.attr("title"));
			map.find("label.map-win-title-label").each(function(){
				$(this).html($(this).parent().parent().attr("title"));
			});
			
			var iscovered = false;
			
			var wins = map.find(".map-win");
			wins.each(function(){var win = $(this); win.on("show", setpositioncenter(win));});
			var loadmaplist = function()
			{
				var i = 0;
				var maplist = map.find(".map-mapname-list");
				maplist.empty();
				for(i = 0; i < selec.find("option").length; i++)
				{
					var mapname = selec.find("option[value='"+i+"']").text();
					var mapitem = $('<div class="map-mapname-item">');
					mapitem.data("mapname", mapname);
					if(mapname.length > 32)
					{
						mapname = mapname.substr(0,31)+"...";
					}
					var mapitemcontent = '<label class="map-mapname-label">'+
							mapname+'</label>'+
							'<div class="edit-icon"></div>'+
							'<div class="delete-icon"></div>';
					var map_con = map_image.find("div.map-imagecontainer[index="+i+"]").first();
					var subfix = map_con.data("subfix");
					
					maplist.append(mapitem);
					mapitem.append(mapitemcontent);
					mapitem.data("subfix", subfix);
				}
				var mapitems = map.find(".map-mapname-list .map-mapname-item");
				mapitems.each(function(){
					var thisitem = $(this);
					thisitem.find(".edit-icon").click(function(){
						edit_win.find(".map-win-content p input.map-editmapnametext").val(thisitem.data("mapname"));
						edit_win.data("mapname", thisitem.data("mapname"));
						edit_win.data("subfix", thisitem.data("subfix"));
						edit_win.show();
						cover.addClass("map-mapcover-foredit");});
					thisitem.find(".delete-icon").click(function(){
						var confirm = "确认要删除地图"+thisitem.find(".map-mapname-label").html()+"吗?";
						delete_confirm_win.find(".map-win-content p label").html(confirm);
						delete_confirm_win.data("mapname", thisitem.data("mapname"));
						delete_confirm_win.show();
						cover.addClass("map-mapcover-foredit");
					});
				});
			}
				
			map.find(".map-draggable-win").draggable({handle:".map-win-title", cancel:".close-icon", containment:"parent"});
			
			map.find(".map-tool-map input.button").click(function(){
					//iscovered = !cover.is(":hidden");
					cover.addClass("map-mapcover");
					//cover.show();
					loadmaplist();
					setpositioncenter(config_win);
					config_win.show();
				}
			);
			
			map.find(".map-newmapadd-panel").click(
				function()
				{
					//add_win.dialog("open");
					add_win.show();
					map.find(".map-config-win").hide();
				}
			);
			map.find(".map-config-win .map-win-title .close-icon").click(function(){
				map.find(".map-config-win").hide();
				cover.removeClass("map-mapcover");
				//if(!iscovered)cover.hide();
			});
			//map.find(".map-config-win").hide();
			
			
			add_win.find(".map-win-cancel-button,.close-icon").click(function(){
				config_win.show();
				add_win.hide();
			});			
			
			edit_win.find(".map-win-cancel-button,.close-icon").click(function(){
					cover.removeClass("map-mapcover-foredit");
					edit_win.hide();
				}
			);
			delete_confirm_win.find(".map-win-cancel-button,.close-icon").click(function(){
					cover.removeClass("map-mapcover-foredit");
					delete_confirm_win.hide();
				}
			);
			add_win.find(".map-win-confirm-button").click(
				function(){
					//add new map 
					//refresh maplist and select option and config win 
					var ifram = $("iframe.map-upload-iframe");
					var uploadform = ifram.contents().find("form#map-upload-form");
					var mapname = uploadform.find("input.map-newmapnameinput");
					var mappath = uploadform.find("input.map-upload");
					var pathtext = uploadform.find("input.map-upload-text");
					if(mapname.val() != "" && mappath.val() != "")
					{
						uploadform.submit();		
						//config_win.show();
						//add_win.hide();
					}
					else
					{
						show_alert({"content":"请输入地图名称并选择要上传的文件!",
							"title":"参数错误"});
					}					
				}
			);
			
			edit_win.find(".map-win-confirm-button")
				.click(function(){
					//submit modify 					
					var mapname = $("input.map-editmapnametext");
					var mapoldname = edit_win.data("mapname");
					var imgsubfix = edit_win.data("subfix");
					var callback = function(){
						cover.removeClass("map-mapcover-foredit");
						edit_win.hide();
					}
					if(mapname.val() != "" && mapname.val() != mapoldname)
					{
						//$("#map-upload-form").submit();
						map_operation_to_server({cmd: "rename", name: mapname.val(), 
							oldname: mapoldname, subfix: imgsubfix}, callback);
					}
					else if(mapname.val() == "" )
					{
						show_alert({"content":"请输入新的地图名称!",
							"title":"参数错误"});
					}
					else
					{
						callback && callback();
					}
				}
			);
			
			delete_confirm_win.find(".map-win-confirm-button")
				.click(function(){
				var callback = function(){
					cover.removeClass("map-mapcover-foredit");
					delete_confirm_win.hide();
				}
				if(delete_confirm_win.data("mapname") != "")
				{
					//$("#map-upload-form").submit();
					map_operation_to_server({cmd: "delete", 
						name: delete_confirm_win.data("mapname")}, callback);
				}
				else
				{
					callback && callback();
				}
			});
			//$(document).disableSelection();
			map.disableSelection();
			//$(document).mousewheel(function(){return false;});
			$(document).on("mousewheel", function(){return false;});
	  };	 
      maps.data("fn_shown", function(){maps.each(map_show);});
	  var fn_shown = maps.data("fn_shown");
	  fn_shown && fn_shown();
  });
