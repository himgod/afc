<?php   
    class elin_actions{   
        function __construct($ok){   
            $ok->register("elin", $this,"go");   
  
        }   
           
        function go($url){
			echo "Perform here <br>";
            //header('Location:'.$url);   
			header($url);
        }   
       
           
    }
 //这个插件的功能就是做header跳转！  
?>  