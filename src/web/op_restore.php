<html>
<head>
<title>upgrading
</title>
<script type="text/javascript" >
function toLogin()
{
	top.location.href="login.php";
}
function afterload()
{
setTimeout("toLogin()",30000);
processing();
}

var i = 0;
var v = new Array(".", "..", "...");
function processing()
{
var s=document.getElementById("divrestore");
s.innerHTML="<br/><br/><br/>&nbsp;&nbsp;&nbsp;Restoring"+v[i]+"<br/><br/><br/>";
i++;
if(i>=3)i=0;
setTimeout("processing()", 1000);
}
</script>
</head>
<body onload="afterload();">
<div  style="position:absolute; width:300px;heigth:200px;background-color:#336699;left:600px;top:300px;">
<span id="divrestore" >Restoring...</span>
</div>
</body>
</html>
