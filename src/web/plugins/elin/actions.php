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
 //�������Ĺ��ܾ�����header��ת��  
?>  