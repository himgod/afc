<html>
<head>
<title>upgrading
</title>
<script type="text/javascript" >
	function toLogin()
	{
		top.location.href="login.php";
	}

	function toLogout()
	{
		top.location.href="logout.php";
	}
	function afterload()
	{
		setTimeout("toLogout()",30000);
		upgrading();
	}

	var i = 0;
	var v = new Array(".", "..", "...");
	function upgrading()
	{
		var s=document.getElementById("diva");
		s.innerHTML="<br/><br/><br/>&nbsp;&nbsp;&nbsp;Upgrading"+v[i]+"<br/><br/><br/>";
		i++;
		if(i>=3)i=0;
		setTimeout("upgrading()", 1000);
	}
</script>
</head>
<body onload="afterload();">
<div  style="position:absolute; width:300px;heigth:200px;background-color:#336699;left:600px;top:300px;">
<span id="diva" >Upgrading...</span>
</div>
</body>
</html>
