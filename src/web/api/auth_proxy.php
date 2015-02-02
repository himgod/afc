<?php

$AUTH_ROOT = "";
$AUTH_ADMIN = "";

$authAdminAccount = "admin";
$authAdminPasswd = "autelan";
$AUTH_ROOT = "http://192.168.6.11:9090/";
$AUTH_ADMIN = "signin%5Busername%5D=".$authAdminAccount."&signin%5Bpassword%5D=".$authAdminPasswd."&signin%5B_csrf_token%5D=CSRFSecret";

$action = "";
$data = "";
$data2 = "";
if(isset($_POST["action"]) &&!empty($_POST["action"])){
	$action = $_POST['action'];
	$data = $_POST['data'];
	if($action == "authAddUser"){
		$data = $_POST['username'];
		$data2 = $_POST['password'];
	}
}

switch ($action) {
	case "authSwitchOffAllPlugins":
	    authSwitchOffAllPlugins($AUTH_ADMIN, $AUTH_ROOT);
	    break;
	case "authSwitchApAuthLocalUserPlugin":
	    authSwitchApAuthLocalUserPlugin($AUTH_ADMIN, $AUTH_ROOT);
	    break;
	case "authSwitchApAuthSplashOnlyPlugin":
	    authSwitchApAuthSplashOnlyPlugin($AUTH_ADMIN, $AUTH_ROOT);
	    break;
	case "authSetAuthEndTimeminute":
	    authSetAuthEndTimeminute($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
	case "authClosePortalPage":
	    authClosePortalPage($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
	case "authSetPortalPage":
	    authSetPortalPage($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
	case "authSearchUser":
	    authSearchUser($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
	case "authAddUser":
	    authAddUser($AUTH_ADMIN, $AUTH_ROOT, $data, $data2);
	    break;
	case "authDeleteUser":
	    authDeleteUser($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
	case "authGetUserId":
	    authGetUserId($AUTH_ADMIN, $AUTH_ROOT, $data);
	    break;
}

function authSwitchOffAllPlugins($authInfo, $authRoot){
	$action = $authRoot."plugin/manage/save";
	$postData = "";
	httpPost($authInfo, $action, $postData);
}

function authSwitchApAuthLocalUserPlugin($authInfo, $authRoot){
	$action = $authRoot."plugin/manage/save";
	$postData = "apPluginManagerList%5BapExternalCMSPlugin%5D=on&apPluginManagerList%5BapAuthLocalUserPlugin%5D=on";
	httpPost($authInfo, $action, $postData);
}

function authSwitchApAuthSplashOnlyPlugin($authInfo, $authRoot){
	$action = $authRoot."plugin/manage/save";
	$postData = "apPluginManagerList%5BapExternalCMSPlugin%5D=on&apPluginManagerList%5BapAuthSplashOnlyPlugin%5D=on";
	httpPost($authInfo, $action, $postData);
}

function authSetAuthEndTimeminute($authInfo, $authRoot, $expiryTime){
	$action = $authRoot."configure";
	$postData = "submit%5BSave%5D=Save&apconfigure%5Bconnection_expiry%5D=".$expiryTime."&apconfigure%5Bsite_name%5D=AuthSite";
	httpPost($authInfo, $action, $postData);
}

function authClosePortalPage($authInfo, $authRoot, $portalUrl){
	$action = $authRoot."plugin/configure/apExternalCMSPlugin";
	$postData = "apExternalCMS%5Bportal_page%5D=".$portalUrl;
	httpPost($authInfo, $action, $postData);
}

function authSetPortalPage($authInfo, $authRoot, $portalUrl){
	$action = $authRoot."plugin/configure/apExternalCMSPlugin";
	$postData = "apExternalCMS%5Bredirect_portal_page%5D=1&apExternalCMS%5Bportal_page%5D=".$portalUrl;
	httpPost($authInfo, $action, $postData);
}

function authSearchUser($authInfo, $authRoot, $keyword){
	$action = $authRoot."authlocaluser/search";
	$postData = "keyword=".$keyword;
	echo json_encode(getResponseContent(httpPost($authInfo, $action, $postData)));
}

function authAddUser($authInfo, $authRoot, $username, $passwd){
	$userId = authGetUserId($authInfo, $authRoot, $username);
	if($userId !== ""){
		echo "0";
		return "";
	}
	$userId = authGetUserId($authInfo, $authRoot, $username);
	$action = $authRoot."authlocaluser/create";
	$email = time()."@autelan.com";
	$status='1';
	$postData = "ap_user%5B_csrf_token%5D=CSRFSecret&ap_user%5Bemail%5D=".$email."&ap_user%5Bpassword%5D=".$passwd."&ap_user%5Busername%5D=".$username."&ap_user%5Bstatus%5D=".$status."&ap_user%5Busername_lower%5D";
	httpPost($authInfo, $action, $postData);
	if($userId !== ""){
		echo "1";
		return "";
	}
	echo "2";
}

function authDeleteUser($authInfo, $authRoot, $username){
	$userId = authGetUserId($authInfo, $authRoot, $username);
	if($userId === ""){
		echo "0";
		return "";
	}
	$action = $authRoot."authlocaluser/delete/".$userId;
	$postData = "sf_method=delete";
	$res = httpPost($authInfo, $action, $postData);
	$userId = authGetUserId($authInfo, $authRoot, $username);
	if($userId === ""){
		echo "1";
		return "";
	}
	echo "2";
}

function authGetUserId($authInfo, $authRoot, $username){
	$action = $authRoot."authlocaluser/getid";
	$postData = "username=".$username;
	$res = getResponseContent(httpPost($authInfo, $action, $postData));
	return $res;
} 

function httpPost($authInfo, $url, $postData){
	$cookie_file = dirname(__FILE__).'/cookie.txt';
	$ch = curl_init(); 
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	
	curl_setopt($ch, CURLOPT_POSTFIELDS, $authInfo); 
	curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file); //使用上面获取的cookies
	$output = curl_exec($ch);

	curl_setopt($ch, CURLOPT_POSTFIELDS, $postData); 
	curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie_file); //使用上面获取的cookies
	$output = curl_exec($ch);
	curl_close($ch);
	//echo $output;
	return trim($output);
}

function getResponseContent($response){
	$startPos = strpos($response, 'id="content"');
	$endPos = strpos($response, 'id="menu"', $startPos);
	$res = substr($response, $startPos+29, $endPos-$startPos-50);
	return trim($res);
}
?>