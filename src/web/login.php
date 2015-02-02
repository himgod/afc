<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<?php 	
	//session_save_path($_SERVER['DOCUMENT_ROOT']."/session");
	//echo session_save_path();
	if(!isset($_SESSION)){
		$sessPath = $_SERVER['DOCUMENT_ROOT']."/session";
		if(!file_exists($sessPath))
		{
			exec("sudo mkdir -p ".$sessPath." >/dev/null 2>/dev/null");
		}
		if(is_dir($sessPath))
		{
			exec(" sudo chown www-data:www-data ".$sessPath." >/dev/null 2>/dev/null; sudo chmod 775 ".$sessPath." >/dev/null 2>/dev/null");
		}
		
		include($_SERVER["DOCUMENT_ROOT"]."/session_start.php");
	}
	include("init_lang.php");
	include("publichead.php");
	include("message.php"); 
	$_SESSION["admin_count"] = "";/* to clear the cache for check admin count */
	if(ToWizardIfAdminNotExists())
	{
		return;
	}
	
?>
<title><?php echo getSoftwareName();?> - login</title>
<link href="/library/css/login.css" rel="stylesheet" type="text/css" />

<!--[if IE 6]>
<style type="text/css">
#Logo {
	top: 20px;
    width: 138px;
    heigth:32px;
    background-image: url("http://192.168.1.241/pic/login-logo.png") ;
}
</style>
<![endif]-->

<!--[if lt IE 8]>
<style type="text/css">
.remember LABEL{
	top:-1px;
    }
</style>
<![endif]-->


<!--[if lte IE 8]>
<style type="text/css">
.login-button{
	background:url('/pic/login-button.png') no-repeat;
    border:none;
    width:141px;
    height:45px;
    
}
.login-button:hover{
	background:url('/pic/login-button-hover.png') no-repeat;
}

.input-box{
	background:url('/pic/input-border.png') no-repeat;
	width:280px;
    height:45px;    
    border:none;
    line-height:45px;
}
.input-box:hover{
	border:none;
    background:url('/pic/input-border-hover.png') no-repeat;
    
    }
.login-page{
	background-color:#FFF;
	behavior: url(/library/css/PIE-1.0.0/PIE.php);
    border:2px #eee solid;
}
</style>
<![endif]-->

<!--[if IE 9]>
<style type="text/css">
.login-button{
	background-color: #2DA8F8;
}
.input-box{
	width: 264px;
	margin-bottom: 7px;
	height: 41px;
	border-radius: 10px;
	border: #e2e2e2 1px solid;
}
</style>
<![endif]-->

<script type="text/javascript" src="/lib/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="/library/js/md5.js"></script>
<script type="text/javascript">
	function encryptPasswd()
	{
		if("" == document.getElementById("user_pass").value)
		{
			document.getElementById("user_pass").focus();
			return false;
		}
		//if(document.getElementById("user_pass").value.length != 32)
		{
			document.getElementById("user_pass").value = hex_md5(document.getElementById("user_pass").value);
		}
                return true;
        }
	function getCookie(name) 
	{ 
    		var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)", "m");
    		if(arr=document.cookie.match(reg))
		{
        		return unescape(arr[2]); 
		}    
		else 
		{
        		return null; 
		}
	}
        function forOnload()
        {
                var cookieUser = getCookie("username");
		//alert(cookieUser);
                if(cookieUser != "" && cookieUser != null)
                {
                        document.getElementById("user_name").value = cookieUser;
                        document.getElementById("user_pass").focus();
                }
                else
                {
                        document.getElementById("user_name").focus();
                }
        }

	var xf="";
	var i=0;
	var len="";
	var dq ="";
	  $(document).ready(function(){	
		  len=$(".thnav li").length;
		  lunbo(i);
		$(".thnav li").hover(function(){	
			  clearTimeout(xf);		  
			  dq = $(this).index(); 
		  	if( (dq+1) %len != i){
				$(".pic li").hide().eq(dq).fadeIn(2000);
				$(".thnav li").removeClass("navli").eq(dq).addClass("navli");
			}
			},
			function(){
			i= dq + 1 < len ? dq + 1 : 0;
			xf = setTimeout("lunbo(" + i + ")", 4000);
		});
		$(".pic li").hover(function(){
			clearTimeout(xf);		  
			  dq = $(this).index(); 
		  	if( (dq+1) %len != i){
				$(".pic li").hide().eq(dq).fadeIn(2000);
				$(".thnav li").removeClass("navli").eq(dq).addClass("navli");
			}
			},
			function(){
			i= dq + 1 < len ? dq + 1 : 0;
			xf = setTimeout("lunbo(" + i + ")", 4000);
			});
		}); 
	
	function lunbo(num){
	  i=num;
	 $(".pic li").hide().eq(i).fadeIn(2000);
	$(".thnav li").removeClass("navli").eq(i).addClass("navli"); 
	 i = i + 1 < len ? i + 1 : 0;
	xf=setTimeout("lunbo(" + i + ")", 4000);
	}	

</script>
<!--[if IE 6]>
<script type="text/javascript"> 
function correctPNG() // correctly handle PNG transparency in Win IE 5.5 & 6. 
{ 
    var arVersion = navigator.appVersion.split("MSIE") 
    var version = parseFloat(arVersion[1]) 
    if ((version >= 5.5) && (document.body.filters)) 
    { 
       for(var j=0; j<document.images.length; j++) 
       { 
          var img = document.images[j] 
          var imgName = img.src.toUpperCase() 
          if (imgName.substring(imgName.length-3, imgName.length) == "PNG") 
          { 
             var imgID = (img.id) ? "id='" + img.id + "' " : "" 
             var imgClass = (img.className) ? "class='" + img.className + "' " : "" 
             var imgTitle = (img.title) ? "title='" + img.title + "' " : "title='" + img.alt + "' " 
             var imgStyle = "display:inline-block;" + img.style.cssText 
             if (img.align == "left") imgStyle = "float:left;" + imgStyle 
             if (img.align == "right") imgStyle = "float:right;" + imgStyle 
             if (img.parentElement.href) imgStyle = "cursor:hand;" + imgStyle 
             var strNewHTML = "<span " + imgID + imgClass + imgTitle 
             + " style=\"" + "width:" + img.width + "px; height:" + img.height + "px;" + imgStyle + ";" 
             + "filter:progid:DXImageTransform.Microsoft.AlphaImageLoader" 
             + "(src=\'" + img.src + "\', sizingMethod='scale');\"></span>" 
             img.outerHTML = strNewHTML 
             j = j-1 
          } 
       } 
    }    
} 
window.attachEvent("onload", correctPNG); 
</script>
<![endif]-->

</head>

<body class="login-page" onload='forOnload();'>
	<div class="head">
    	<div class='' id="Logo"  ></div>        
    </div>
    <div class="main">
        <div class="slide">
          <div class="pic">
            <ul>
                <li><a href="#"><img src="/pic/AE5000.png"/></a></li>
                <li><a href="#"><img src="/pic/banner-E2CN2.png"/></a></li>
                <li><a href="#"><img src="/pic/banner-E3CN3T.png"/></a></li>
                <li><a href="#"><img src="/pic/banner-E2CN2.png"/></a></li>
            </ul>
            </div>
            <div class="thumb">
            <div class="thnav">
              <ul>
                <li></li>
                <li></li>
                <li></li>
                <li></li>
               <!--也可以放缩略图,需要修改上面的CSS-->
               </ul>
              </div>
        </div>
         <div class="clear"></div>	
        </div>
        <div class="login">
       		<form method="post" action="check_password.php" onsubmit='return encryptPasswd();'>
            	<p><input type="text"  id="user_name" class="input-box" name="user_name" <?php if(isset($_SESSION["username"])){
					echo  "value='".$_SESSION["username"]."'";}
					else{ echo  "placeholder='".search($lpublic, "account")."'";}?>/></p>
                <p><input type="password" name="user_pass" class="input-box" id="user_pass" placeholder='<?php echo search($lpublic, "password");?>'/></p>
                <p class="remember"><input type="checkbox" name="remember_name" id="RememberMe" /><label for="RememberMe"><?php echo search($lpublic, "remember_account");?></label></p>
                <p class="logbut"><input type='submit' value='<?php echo search($lpublic, "login");?>' class="login-button"/></p>
           </form>
        </div> 
    </div>
</body>
</html>
