<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />	
<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=GfiprAVPGB7xUTkGySy0m5Vw">
	//src="http://api.map.baidu.com/api?v=2.0&ak=GfiprAVPGB7xUTkGySy0m5Vw"
	//src='/library/js/getscript.js'
</script>
<title>Baidu Map</title>
<style type="text/css">
			html { height: 100% }
      body { height: 100%; margin: 0; padding: 0 }
      #map_canvas { height: 100% }
</style>
<script type="text/javascript" >
	var map;
	var myGeo;

	function set_location(address) {
		myGeo.getPoint(address, function(point){
		  if (point) {
			   map.centerAndZoom(point, 16);
			   //map.addOverlay(new BMap.Marker(point));
		  }
		});
	}  
	function initialize()
	{  
		try{
			var localCity = new BMap.LocalCity();
			map = new BMap.Map("map_canvas");   
			//alert(BMAP_PERSPECTIVE_MAP);  3D
			//var point = new BMap.Point(116.404, 39.915); 
			//map.centerAndZoom(point,15); 
			localCity.get(function (r) {
			   map.centerAndZoom(r.center, r.level);
			});                   
			map.enableScrollWheelZoom();
			myGeo = new BMap.Geocoder();
	    top.gmap_inited();
  	}  
    catch (e) {
      top.gmap_failed();
    }
	}
      
</script>    
</head>
<body onload='initialize();'>
<div id="map_canvas" style="width:100%; height:100%"></div>
</body>
</html>
