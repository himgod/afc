<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
    <style type="text/css">
      html { height: 100% }
      body { height: 100%; margin: 0; padding: 0 }
      #map_canvas { height: 100% }
    </style>
    <script type="text/javascript"
      src="https://maps.googleapis.com/maps/api/js?sensor=false">
    </script>
    <script type="text/javascript">
      var map;
      var _geocoder;
      function initialize() {alert("init");
        var myOptions = {
          center: new google.maps.LatLng(37.410639, -121.942494),
          zoom: 14,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        try {
          map = new google.maps.Map(document.getElementById("map_canvas"),
              myOptions);
          _geocoder = new google.maps.Geocoder();
          top.gmap_inited();
        }
        catch (e) {
          top.gmap_failed();
        }
      }
      function set_location(address) {
        _geocoder.geocode( {'address': address}, function(results, status) {
          if (status == google.maps.GeocoderStatus.OK) {
            map.setCenter(results[0].geometry.location);
          }
        });
      }
    </script>
  </head>
  <body onload="initialize()">
    <div id="map_canvas" style="width:100%; height:100%"></div>
  </body>
</html>
