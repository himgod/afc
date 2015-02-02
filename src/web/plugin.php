<?php   
 class PluginManager {     
    private $_listeners = array();
	
    public function __construct() {     
        $plugins = array(//������ʵ���Ե����ݿ��ѯ����Ӧ�Ĳ����Ϣ������ֻ��Ϊ�˷��㶨������������  
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
							//��ʼ�����в��						
                            new $class($this);     
                        }     
                }     
            }     
        }     
  
    }     
  
       
    /** 
	* ע����Ҫ�����Ĳ�����������ӣ� 
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
	* ����һ������ 
	* 
	* @param string $hook ���ӵ����� 
	* @param mixed $data ���ӵ���� 
	* @return mixed 
	*/ 

    function trigger($hook, &$data='')  
	{     
        $result = '';     
//�鿴Ҫʵ�ֵĹ��ӣ��Ƿ��ڼ�������֮��    
        if (isset($this->_listeners[$hook]) && is_array($this->_listeners[$hook]) && count($this->_listeners[$hook]) > 0)  {     
            foreach ($this->_listeners[$hook] as $listener){     
        // ȡ�������������úͷ���    
                $class  = &$listener[0];     
                $method = $listener[1];     
                if(method_exists($class,$method)) {     
                    // ��̬���ò���ķ���    
                    $result .= $class->$method($data);     
                }     
            }     
        }
		else
			echo "not register";
#�˴���Щ��־��¼����Ķ���  
		echo $result;
        return $result;     
    }     
}   

$b= new PluginManager();
$data = array();
$b->trigger("micxp", $data);

//���������������Ҫ���õĵط����뼴�ɣ�
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
//������NEWһ����������࣡    
?> 
