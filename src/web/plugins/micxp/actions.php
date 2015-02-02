<?php  

include ("dynamic_channel_selection.php");
    class micxp_actions{ 
	
        function __construct($ok){   
            $ok->register("micxp", $this,"goaddress");   
        }   
           
        function goaddress(&$data) {
			dynamtic_channel_selection($data);
        }      
    }  
?>  