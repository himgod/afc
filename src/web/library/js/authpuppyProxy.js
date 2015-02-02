/*
 * the authpuppy server url location.
 */
var auth_root = "";

/*
 * The username of admin user for authpuppy server.
 */
var admin_user = '';

/*
 * The password of admin user for authpuppy server.
 */
var admin_password = '';

/*
 * Init the admin account for change auth server config.
 */
function authInitConfig(authServer, admin, passwd){
	auth_root = authServer;
	admin_user = admin;
	admin_password = passwd;
}

function authLogin(){
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'logindoctrine',
		data: {
			"signin[username]": admin_user,
			"signin[password]": admin_password,
			"signin[_csrf_token]": "CSRFSecret"
		},
		success: function(){
			return true;
		},
		error: function(){
			return false;
		}
	});
}

function authLogout(){
	$.ajax({
		async: false,
		url: auth_root + 'logoutdoctrine',
		success: function(){
			return true;
		},
		error: function(){
			return false;
		}
	});
}

/**
 * Switch off all of the authpuppy plugin.
 * 
 */
function authSwitchOffAllPlugins(){
	authLogin();
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'plugin/manage/save',
		data: "",
		success: function(){
			//return true;
		},
		error: function(){
			//return false;
		}
	});
	authLogout();
}

/**
 * Switch the way of users to auth and connect the network to ApAuthLocalUser.
 * 
 */
function authSwitchApAuthLocalUserPlugin(){
	authLogin();
	switchApAuthLocalUserPlugin();
	authLogout();
}

function switchApAuthLocalUserPlugin(){
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'plugin/manage/save',
		data: {
			'apPluginManagerList[apAuthLocalUserPlugin]': 'on',
			'apPluginManagerList[apExternalCMSPlugin]': 'on'
		},
		success: function(){
			//return true;
		},
		error: function(){
			//return false;
		}
	});
}

/**
 * Switch the way of users to auth and connect the network to ApAuthSplashOnly.
 * 
 */
function authSwitchApAuthSplashOnlyPlugin(){
	authLogin();
	switchApAuthSplashOnlyPlugin();
	authLogout();
}

function switchApAuthSplashOnlyPlugin(){
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'plugin/manage/save',
		data: {
			'apPluginManagerList[apAuthSplashOnlyPlugin]': 'on',
			'apPluginManagerList[apExternalCMSPlugin]': 'on'
		},
		success: function(){
			//return true;
		},
		error: function(){
			//return false;
		}
	});
	authLogout();
}

	/**
	* This function allow admin to add user that allow to connect the network for ApAuthLocalUser way.
	* @return: 
	* 		1  : add success.
	* 		0  : username has existd.
	* 		-1 : maybe network error.
	*		2  : invalid password.
	*/
function authAddUser(username, password, email, status){
	authLogin();
	var res = '';
	var id = authGetUserId(username);
	if(id == '-1')
		return '-1';
	if(id !='' && id != undefined)
		return '0';//the username has existd.
	if(status == undefined || status == '')
		status = '1';
	if(email == undefined || email == ''){
		var timestamp = new Date().getTime();
		email = timestamp + '@autelan.com';
	}
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'authlocaluser/create',
		data:{
			'ap_user[_csrf_token]': 'CSRFSecret', 
			'ap_user[email]': email,
			'ap_user[password]': password, 
			'ap_user[status]': status, 
			'ap_user[username]': username, 
			'ap_user[username_lower]': ''
		},
		success: function(){
			res = '1';//add success.
		},
		error: function(){
			res = '-1';//the network error.
		}
	});
	authLogout();
	return res;
}

/**
 * Set the time in minute before a connection expires because of inactivity.
 * 
 */
function authSetAuthEndTimeminute(expiryTime){
	authLogin();
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'configure',
		data: {
			'asubmit[save]': 'Save', 
			'apconfigure[connection_expiry]': expiryTime,
			'apconfigure[site_name]': 'AuthSite'
			/*'apconfigure[main_url]',
			'apconfigure[show_administrative_login_link]':'',
			'apconfigure[show_network_login_link]':'',
			'apconfigure[show_node_info]':'',
			'apconfigure[support_link]':'',
			'apconfigure[support_text]':'',
			'apconfigure[email_from]':'',
			'apconfigure[name_from]':'',
			'apconfigure[available_languages]':''*/
		},
		success: function(){
			//return true;
		},
		error: function(){
			//return false;
		}
	});
	authLogout();
}

/**
 * Set redirect portal page.
 * 
 */
function authSetPortalPage(portalUrl){
	authLogin();
	switchApAuthLocalUserPlugin();
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'plugin/configure/apExternalCMSPlugin',
		data: {
			'apExternalCMS[redirect_portal_page]': '1',
			'apExternalCMS[portal_page]': portalUrl
		},
		success: function(){
			//return true;
		},
		error: function(){
			//return false;
		}
	});
	authLogout();
}

/**
 * Close redirect portal page.
 * 
 */
function authClosePortalPage(portalUrl){
	authLogin();
	res = false;
	switchApAuthLocalUserPlugin();
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'plugin/configure/apExternalCMSPlugin',
		data: {
			'apExternalCMS[redirect_portal_page]': 'portalUrl'
		},
		success: function(){
			res = true;
		},
		error: function(){
			res = false;
		}
	});
	authLogout();
	return res;
}

/**
 * delete a user by username.
 * @return: 
 * 		1: delete success.
 * 		0: username doesn't exist.
 * 		-1: maybe network error. 
 */
function authDeleteUser(username){
	authLogin();
	var res;
	var id = authGetUserId(username);
	if(id == '-1')
		return '-1';
	if(id =='' || id == undefined)
		return '0';//the username has existd.
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'authlocaluser/delete/'+id,
		data: {
			"sf_method": "delete"
		},
		success: function(){
			res = '1';//delete success.
		},
		error: function(){
			res = '-1';//delete error,maybe network error. 
		}
	});
	authLogout();
	return res;
}

/**
 * search the user that added.
 * The function is supporting fuzzy matching.
 */
function authSearchUser(keyword){
	authLogin();
	var res = '';
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'authlocaluser/search',
		dataType: 'html',
		data: "keyword="+keyword,
		success: function(resMsg){
			res = resMsg;
/*			var ss = $('#resMsg');
			//$('#resMsg').html(resMsg);
			$("#resMsg").load(auth_root + 'authlocaluser/search');*/
		},
		error: function(){
			res = '-1';
		}
	});
	authLogout();
	return res;
}

	/*
	 * Get the user id from db
	 */
function authGetUserId(username){
	var res = '';
	$.ajax({
		type: 'post',
		async: false,
		url: auth_root + 'authlocaluser/getid',
		dataType: 'html',
		data: "username="+username,
		success: function(resMsg){
			res = resMsg;
		},
		error: function(){
			res = '-1';
		}
	});
	return res;
}
