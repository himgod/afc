<?php   
    class elin_actions{   
        function __construct($ok){   
            $ok->register("elin", $this,"go");   
  
        }   
           
        function go($url){
            //header('Location:'.$url);   
			//header('Location:'.$url);
			echo $url;
			echo "<br>";
        }   
       
           
    }
 //这个插件的功能就是做header跳转！  
?>  