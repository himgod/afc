<?php   
 class PluginManager {     
    private $_listeners = array();
	
    public function __construct() {     
        $plugins = array(//这里其实可以到数据库查询到相应的插件信息。这里只是为了方便定义了两组插件。  
            array(     
                'name' => 'elin',     
				'directory'=>'elin'   
                ),   
            array(     
                'name' => 'micxp',     
                'directory'=>'micxp'   
                )     
        );     
		
        if($plugins) {     
            foreach($plugins as $plugin) {   

                if (@file_exists('plugins/'.$plugin['directory'].'/actions.php'))  {  
                        include_once('plugins/'.$plugin['directory'].'/actions.php');     
                        $class = $plugin['name'].'_actions'; 
						
                        if (class_exists($class)){
							//初始化所有插件						
                            new $class($this);     
                        }     
                }     
            }     
        }     
  
    }     
  
       
    /** 
	* 注册需要监听的插件方法（钩子） 
	* 
	* @param string $hook 
	* @param object $reference 
	* @param string $method 
	*/ 
    function register($hook, &$reference, $method)  
	{     
        $key = get_class($reference)."->".$method;     
        $this->_listeners[$hook][$key] = array(&$reference, $method);  		
    }
	function unregister($hook, &$reference, $method) 
	{
		$key = get_class($reference).'->'.$method;
		$method = '';
		$this->_listeners[$hook][$key] = array(&$reference, $method);
	}
  
	/** 
	* 触发一个钩子 
	* 
	* @param string $hook 钩子的名称 
	* @param mixed $data 钩子的入参 
	* @return mixed 
	*/ 

    function trigger($hook, &$data='')  
	{     
        $result = '';     
//查看要实现的钩子，是否在监听数组之中    
        if (isset($this->_listeners[$hook]) && is_array($this->_listeners[$hook]) && count($this->_listeners[$hook]) > 0)  {     
            foreach ($this->_listeners[$hook] as $listener){     
        // 取出插件对象的引用和方法    
                $class  = &$listener[0];     
                $method = $listener[1];     
                if(method_exists($class,$method)) {     
                    // 动态调用插件的方法    
                    $result .= $class->$method($data);     
                }     
            }     
        }
		else
			echo "not register";
#此处做些日志记录方面的东西  
		echo $result;
        return $result;     
    }     
}   

$b= new PluginManager();
$data = array();
$b->trigger("micxp", $data);

//这个方法就是在需要调用的地方插入即可！
	echo "After Dynamic Channel Selection: <br>";
	for ($i = 0; isset($data[$i]); $i++)
	{
		echo "<br>";
		echo "RadioMac: ";
		echo $data[$i]["RadioMac"];
		echo "   ";
		echo "CHANNEL: ";
		echo $data[$i]["Channel"];
		echo "<br>";
		$ret = ext_wtp("dynamic_select_radio_channel", $data[$i]["RadioMac"], $data[$i]["Channel"]);
	}
//上面是NEW一个插件管理类！    
?> 
