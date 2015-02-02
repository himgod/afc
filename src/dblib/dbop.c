#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <mysql.h>
#include <time.h>
#include "dbop.h"

#ifdef OBJECT_ACTIVE_SUPPORT
#if OBJECT_ACTIVE_SUPPORT
/*
typedef enum object_index
{
	ADMIN_CHANGE_PASSWD,
	ADMIN_CHANGE_INFO,
	AP_ADMITED_BY_ADMIN,
	AP_FORGOTEN_BY_ADMIN,
	AP_RESTARTED_BY_ADMIN,
	AP_UPGRADED_BY_ADMIN,
	AP_UPGRADE_FAILED_BY_ADMIN,
	AP_UPGRADE_SUCCESS_BY_ADMIN,
	AP_AUTO_UPGRADE_START,
	AP_AUTO_UPGRADE_FAILED,
	AP_AUTO_UPGRADE_SUCCESS,
	AP_BLOCKED_BY_ADMIN,
	AP_DOWNLOAD_PUBKEY_START,
	AP_DOWNLOAD_PUBKEY_FAILED,
	AP_DOWNLOAD_PUBKEY_SUCCESS,
	USER_KICKED_BY_ADMIN,
	USER_BLOCKED_BY_AMDIN,
	USER_KICKED_BY_ADMIN,
	USER_BLOCKED_BY_ADMIN,
	USER_AUTHORIZED_BY_ADMIN,
	USER_UNAUTHORIZED_BY_ADMIN,	
	
	/*please add befor MAX_OBJECT_ACTIVE_INDEX 
	MAX_OBJECT_ACTIVE_INDEX,
} object_index_e;

*/
object_active_pre known_object_active[MAX_OBJECT_ACTIVE_INDEX] = 
{
	[ADMIN_CHANGE_INFO]    = 	{
									.key = "ADMIN_CHANGE_INFO",
									/* email_alert ? "true":"false" then pass to argv the last %s maybe 'and changed passwd' or '' */
									.msg = WAI_OBJECT_ADMIN" changed user info to name=\"%s\",lang=\"%s\",email_alert=%s,email=\"%s\" %s.",
									//.nullstr = "%s%s%s%s%s",
									.adminIndex = 1,
								},
	[AP_DOWNLOAD_PUBKEY_START] =	{
										.key = "AP_DOWNLOAD_PUBKEY_START",
										.msg = WAI_OBJECT_AP" start to authorize to AFi-Controller.",
										.apIndex = 1,
									},
	[AP_DOWNLOAD_PUBKEY_FAILED] =		{
										.key = "AP_DOWNLOAD_PUBKEY_FAILED",
										.msg = WAI_OBJECT_AP" get permit for authorize to AFi-Controller failed!",
										.apIndex = 1,
									},
	[AP_SAVE_PUBKEY_FAILED] =		{
										.key = "AP_SAVE_PUBKEY_FAILED",
										.msg = WAI_OBJECT_AP" save authorize to AFi-Controller failed!",
										.apIndex = 1,
									},
	[AP_DOWNLOAD_SAVE_PUBKEY_SUCCESS] =		{
										.key = "AP_DOWNLOAD_PUBKEY_SUCCESS",
										.msg = WAI_OBJECT_AP" authorized to AFi-Controller success!",
										.apIndex = 1,
									},
	[AP_WAS_DISCOVERY_PENDING]	=		{
										.key = "AP_WAS_DISCOVERY_PENDING",
										.msg = WAI_OBJECT_AP" was discovered and waiting for adoption.",
										.apIndex = 1,
									},
	[AP_WAS_DISCONNECTED]	=		{
										.key = "AP_WAS_DISCONNECTED",
										.msg = WAI_OBJECT_AP" was disconnected.",
										.apIndex = 1,
									},
	[AP_WAS_OFFLINED]	=			{
										.key = "AP_WAS_OFFLINED",
										.msg = WAI_OBJECT_AP" was offlined.",
										.apIndex = 1,
									},
	[AP_WAS_AUTO_READMITED] =       {
										.key = "AP_WAS_AUTO_READMITED",
										.msg = WAI_OBJECT_AP" was auto readmited.",
										.apIndex = 1,
									},
	[AP_ADMITED_BY_ADMIN]  = 	{
									.key = "AP_ADMITED_BY_ADMIN",
									.msg = WAI_OBJECT_AP" was adopted by "WAI_OBJECT_ADMIN".",
									//.nullstr = "%s%s%s%s%s%s%s%s",
									.apIndex = 1,
									.adminIndex = 2,
								},
	[AP_FORGOTEN_BY_ADMIN] = 	{
									.key = "AP_FORGOTEN_BY_ADMIN",
									.msg = WAI_OBJECT_AP" was forgoten by "WAI_OBJECT_ADMIN".",
									//.nullstr = "%s%s%s%s%s%s%s%s",
									.apIndex = 1,
									.adminIndex = 2,
								},
	[AP_RESTARTED_BY_ADMIN] = 	{
									.key = "AP_RESTARTED_BY_ADMIN",
									.msg = WAI_OBJECT_AP" was restarted by "WAI_OBJECT_ADMIN".",
									//.nullstr = "%s%s%s%s%s%s%s%s",
									.apIndex = 1,
									.adminIndex = 2,
								},
	[AP_BLOCKED_BY_ADMIN] = 	{
									.key = "AP_BLOCKED_BY_ADMIN",
									.msg = WAI_OBJECT_AP" was blocked by "WAI_OBJECT_ADMIN".",
									//.nullstr = "%s%s%s%s%s%s%s%s",
									.apIndex = 1,
									.adminIndex = 2,
								},
	[AP_UPGRADE_BY_ADMIN_START] = 	{
									.key = "AP_UPGRADED_BY_ADMIN",
									.msg = WAI_OBJECT_AP" start to upgrade by "WAI_OBJECT_ADMIN" to version %s.",
									//.nullstr = "%s%s%s%s%s%s%s%s",
									.apIndex = 1,
									.adminIndex = 2,
								},
	[AP_UPGRADE_BY_ADMIN_STEP] = 	{
										.key = "AP_UPGRADE_BY_ADMIN_STEP",
										.msg = WAI_OBJECT_AP" upgrade by "WAI_OBJECT_ADMIN" to version %s info:%s!",
										//.nullstr = "%s%s%s%s%s%s%s%s",
										.apIndex = 1,
										.adminIndex = 2,
									},
	[AP_UPGRADE_BY_ADMIN_FAILED] = 	{
										.key = "AP_UPGRADE_BY_ADMIN_FAILED",
										.msg = WAI_OBJECT_AP" upgrade by "WAI_OBJECT_ADMIN" to version %s failed, info:%s!",
										//.nullstr = "%s%s%s%s%s%s%s%s",
										.apIndex = 1,
										.adminIndex = 2,
									},
	[AP_UPGRADED_BY_ADMIN_SUCCESS] = {
										.key = "AP_UPGRADE_BY_ADMIN_SUCCESS",
										.msg = WAI_OBJECT_AP" was upgraded by "WAI_OBJECT_ADMIN" to newest version %s success!",
										//.nullstr = "%s%s%s%s%s%s%s%s",
										.apIndex = 1,
										.adminIndex = 2,
									},
	[AP_AUTO_UPGRADE_START] =		{
										.key = "AP_AUTO_UPGRADE_START",
										.msg = WAI_OBJECT_AP" start to upgrade %s to version %s.",
										.apIndex = 1,
									},
	[AP_AUTO_UPGRADE_STEP] =		{
										.key = "AP_AUTO_UPGRADE_STEP",
										.msg = WAI_OBJECT_AP" upgrade %s to version %s info:%s!",
										.apIndex = 1,
									},
	[AP_AUTO_UPGRADE_FAILED] =		{
										.key = "AP_AUTO_UPGRADE_FAILED",
										.msg = WAI_OBJECT_AP" upgrade %s to version %s failed, info:%s!",
										.apIndex = 1,
									},
	[AP_AUTO_UPGRADED_SUCCESS] =		{
										.key = "AP_AUTO_UPGRADE_SUCCESS",
										.msg = WAI_OBJECT_AP" was upgraded %s to newest version %s success!",
										.apIndex = 1,
									},
};
#endif
#endif
char * get_db_format_time_from_tm(struct tm * timeinfo, char * timeStr)
{
	//time_t rawtime;
	//struct tm * timeinfo;
	if(!timeStr || !timeinfo)
	{
		syslog(LOG_ERR, "get_db_format_time failed, bad input parameter timeinfo %p timeStr %p!", timeinfo, timeStr);
		return NULL;
	}
	//time ( &rawtime );
	//timeinfo = localtime ( &rawtime );
	
	snprintf (timeStr, (SIMPLE_STR_LEN-1), "%4d-%02d-%02d %02d:%02d:%02d", 
		1900+timeinfo->tm_year, 1+timeinfo->tm_mon,timeinfo->tm_mday,
		timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	
	return timeStr;
}

char * get_db_format_time_from_time_t(time_t rawtime, char * timeStr)
{
	//time_t rawtime;
	struct tm * timeinfo;
	if(!timeStr)
	{
		syslog(LOG_ERR, "get_db_format_time_from_time_t failed, bad input parameter!");
		return NULL;
	}
	//time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	if(!timeinfo)
	{
		syslog(LOG_ERR, "get_db_format_time_from_time_t failed, get localtime filed!");
		return NULL;
	}
	
	snprintf (timeStr, (SIMPLE_STR_LEN-1), "%4d-%02d-%02d %02d:%02d:%02d", 
		1900+timeinfo->tm_year, 1+timeinfo->tm_mon,timeinfo->tm_mday,
		timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	
	return timeStr;
}

struct tm* get_tm_from_db_format_time(struct tm * timeinfo, char * timeStr)
{
	//time_t rawtime;
	//struct tm * timeinfo;
	char * tmpPtr = NULL;
	char tmptimeStr[64] = {0};
	if(!timeStr || !timeinfo)
	{
		syslog(LOG_ERR, "get_tm_from_db_format_time failed, bad input parameter timeinfo %p timeStr %p!", timeinfo, timeStr);
		return NULL;
	}
	strncpy((char *)tmptimeStr, timeStr, 63);
	//time ( &rawtime );
	//timeinfo = localtime ( &rawtime );
	tmpPtr = strtok((char *)tmptimeStr, "-");
	timeinfo->tm_year = strtoul(tmpPtr, NULL, 10);
	timeinfo->tm_year -= 1900;
	tmpPtr = strtok(NULL, "-");
	timeinfo->tm_mon = strtoul(tmpPtr, NULL, 10);
	timeinfo->tm_mon -= 1;
	tmpPtr = strtok(NULL, " ");
	timeinfo->tm_mday = strtoul(tmpPtr, NULL, 10);
	tmpPtr = strtok(NULL, ":");
	timeinfo->tm_hour = strtoul(tmpPtr, NULL, 10);
	tmpPtr = strtok(NULL, ":");
	timeinfo->tm_min = strtoul(tmpPtr, NULL, 10);
	tmpPtr = strtok(NULL, ":");
	timeinfo->tm_sec = strtoul(tmpPtr, NULL, 10);
	
#if	0 && DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "get tm %4d-%02d-%02d %02d:%02d:%02d from tims str '%s'", 
		1900+timeinfo->tm_year, 1+timeinfo->tm_mon,timeinfo->tm_mday,
		timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec, timeStr);
#endif
	
	return timeinfo;
}

time_t get_time_t_from_db_format_time(char * timeStr)
{
	//time_t rawtime;
	struct tm timeinfo;
	struct tm * tmptm = NULL;
	char tmptimeStr[64] = {0};
	time_t timestamp = 0;
	if(!timeStr)
	{
		syslog(LOG_ERR, "get_db_format_time_from_time_t failed, bad input parameter!");
		return 0;
	}
	memset(&timeinfo, 0, sizeof(timeinfo));
	strncpy((char *)tmptimeStr, timeStr, 63);
	tmptm = get_tm_from_db_format_time(&timeinfo, (char *)tmptimeStr);
	if(tmptm)
	{
		timestamp = mktime(tmptm);
	}
	//timestamp = time(NULL);
#if	0 && DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "get time_t %ld from time str '%s'", timestamp, (char *)timeStr);
#endif
	return timestamp;
}

MYSQL * db_connect_to_database(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db)
{/* 0  success , -1 error occured */
	MYSQL *mysqlptr = NULL;
	if(!db)
	{
		syslog(LOG_ERR, "input bad parameter db !\n");
		return NULL;
	}
	mysqlptr = mysql_init(mysql);/* allow  input arg  mysql == NULL */
	if(!mysqlptr)
	{
		syslog(LOG_ERR, "mysql init failed !\n");
		return NULL;
	}
	mysql_options(mysqlptr, MYSQL_READ_DEFAULT_GROUP, "AFi-Controller");
	if(!mysql_real_connect(mysqlptr/*mysql*/, host/*host*/, user/*user*/, passwd/*you understand...*/, 
			db/*database*/, 0/*port*/, NULL/*unix_socket*/, 0/*client_flag*/))
	{
		syslog(LOG_ERR, "Failed to connect to database: %s:%s\n", host, db);
		mysql_error(mysqlptr);
		return NULL;
	}
	return mysqlptr;
}
MYSQL * db_get_database_connection(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db)
{
	return db_connect_to_database(mysql, host, user, passwd, db);
}
int db_disconnect_to_database(MYSQL *mysql)
{
	if(mysql)
	{
		mysql_close(mysql);
		return 0;
	}
	syslog(LOG_ERR, "disconnect database input bad parameter: mysql null pointer !\n");
	return -1;
}
int db_put_database_connection(MYSQL *mysql)
{
	return db_disconnect_to_database(mysql);
}
int db_do_query(MYSQL *mysql, const char *query, int (*call_back)(void *, MYSQL_ROW , int, int), void * result)
{
	int affected_rows = 0;
	
	if (mysql_query(mysql, query) != 0)
	{
		goto err;
	}

	if (mysql_field_count(mysql) > 0)
	{
		MYSQL_RES   *res = NULL;
		MYSQL_ROW    row, end_row;
		int num_fields = 0;
		int num_rows = 0;

		if (!(res = mysql_store_result(mysql)))
			goto err;
		num_fields = mysql_num_fields(res);
		num_rows = mysql_num_rows(res);
		while ((row = mysql_fetch_row(res)))
		{
			if(call_back)
			{
				call_back(result, row, num_rows, num_fields);
			}
		}
		mysql_free_result(res);
		
#if DATABASE_DEBUG_ENABLE
		syslog(LOG_DEBUG, "%s get query result rows: %d\n", query, num_rows);
#endif
		return num_rows;
	}
	else
	{
		affected_rows = mysql_affected_rows(mysql);
#if DATABASE_DEBUG_ENABLE
		syslog(LOG_DEBUG, "%s affected rows: %d\n", query, affected_rows);
#endif
		return affected_rows;
	}
	//return 0;

err:
	syslog(LOG_ERR, "db_do_query failed: %s [%s]", mysql_error(mysql), query);
	return -1;
}

int call_back_get_string_result(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	int i = 0;
	result_string * result = NULL;
	if(!arg || !row || !column_num || !row_num)
	{
		syslog(LOG_ERR, "Bad input parameter: arg %p row %p row num %d column num %d !", arg, row, row_num, column_num);
		return -1;
	}
	result = (result_string *)arg;
	for(i = 0; i < column_num; i++)
	{
		if(!row[i])
		{
			continue;
		}
		if(strlen(row[i])+result->len <= MAX_RESULT_STRING_LEN-3)
		{
			result->len += sprintf((result->result+result->len), "%s |", (char *)row[i]);
		}
		else
		{
			syslog(LOG_ERR, "%s len %d + %d out of range of MAX_RESULT_STRING_LEN \n", __func__, result->len, strlen(row[i])+3 );
			break;
		}
	}
	if(result->len+2 <= MAX_RESULT_STRING_LEN)
	{
		result->len += sprintf((result->result+result->len), "\n");
	}
	return 0;	
}

int call_back_get_array_result(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	int i = 0;
	int malloc_size = 0;
	result_array * result = (result_array *) arg;
	if(!result || !row || !column_num || !row_num)
	{
		syslog(LOG_ERR, "Bad input parameter: arg %p row %p row num %d column num %d !", result, row, row_num, column_num);
		return -1;
	}
	
	if(! result->row_num || !result->result)
	{		
		char ** tmpPtr = NULL;
		if(result->result)
		{
			free(result->result);
		}
		result->result = (char *** )malloc(sizeof(char **)*row_num);
		if(!result->result)
		{
			syslog(LOG_ERR, "%s %d Memory malloc failed!\n", __func__, __LINE__);
			return -1;
		}
		memset(result->result, 0, sizeof(char **)*row_num);
		result->crr_row = 0;
		result->column_num = column_num;
		result->row_num = row_num;
		tmpPtr = (char **)malloc(sizeof(char *)*column_num*row_num);
		memset(tmpPtr, 0, sizeof(char *)*column_num*row_num);
		for(i = 0; i < row_num; i++)
		{
			result->result[i] = tmpPtr+column_num;
		}
	}
	
	for(i = 0; i < column_num; i++)
	{
		if(!row[i])
		{
			continue;
		}
		malloc_size = sizeof(char)*strlen(row[i])+1;
		result->result[result->crr_row][i] = (char *)malloc(malloc_size);
		if(!result->result[result->crr_row][i])
		{
			syslog(LOG_ERR, "%s %d Memory malloc failed!\n", __func__, __LINE__);
			return -1;
		}
		memset(result->result[result->crr_row][i], 0, malloc_size);
		//printf("row [%d] %s\n", i, row[i]);
		strncpy(result->result[result->crr_row][i], (char *)row[i], malloc_size-1);
	}
		
	result->crr_row++;
	
	return 0;	
}

void db_output_string_result(result_string * result)
{
	if(result && (result->len > 0))
	{
		printf("Query result:\n %s \n", result->result);
	}
}

void db_output_array_result(result_array * result)
{
	int i = 0;
	int j = 0;
	if(result && result->result && (result->row_num > 0) && (result->column_num > 0))
	{
		printf("Query result:\n");
		for(i = 0; i < result->row_num; i++)
		{
			for(j = 0; j < result->column_num; j++)
			{
				printf("%s |", (char *)(result->result[i][j]));
			}
			printf("\n");
		}
	}
}

void db_destroy_result_array(result_array * arg)
{
	int i = 0;
	int j = 0;
	result_array * result = arg;
	if(result)
	{
		if(result->result)
		{
			if(result->row_num > 0)
			{
				for(i = 0; i < result->row_num; i++)
				{
					if(result->result[i])
					{
						if(result->column_num > 0)
						{
							for(j = 0; j < result->column_num; j++)
							{
								if(result->result[i][j])
								{
									free(result->result[i][j]);
									result->result[i][j] = NULL;
								}
							}
						}
					}
				}
				free(result->result[0]);
				result->result[0] = NULL;
			}
			free(result->result);
			result->result = NULL;
		}
	}
	return;
}

#ifdef ADMIN_USER_SUPPORT
#if ADMIN_USER_SUPPORT
int call_back_check_user_passwd(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	int * result = (int * )arg;
	*result = FALSE;
	if(row_num > 0 && column_num > 0)
	{
		*result = TRUE;
		return 0;
	}
	return -1;
}
int call_back_get_first_passwd(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	char * result = (char *)arg;
#if DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "get passwd result %p row_num %d column_num %d",
		result, row_num, column_num);
#endif
	if(result && row_num >= 1 && column_num == 1)
	{
		if(result[0] == '\0')
		{
			strncpy(result, (char *)row[0], (USER_PASSWD_LEN-1));
			return 0;
		}
	}
	return -1;
}

int call_back_get_last_passwd(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	char * result = (char *)arg;
#if DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "get passwd result %p row_num %d column_num %d",
		result, row_num, column_num);
#endif
	if(result && row_num >= 1 && column_num == 1)
	{
		memset(result, 0, USER_PASSWD_LEN);
		if(row && row[0])
		{
			strncpy(result, (char *)row[0], (USER_PASSWD_LEN-1));
		}
		return 0;
	}
	return -1;
}
int call_back_get_id_by_name_passwd(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	int * result = (int *)arg;
#if DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "get id by name passwd result %p row_num %d column_num %d",
		result, row_num, column_num);
#endif
	if(result && row_num >= 1 && column_num >= 1)
	{		
		if(row && row[0])
		{
			*result = strtoul(row[0], NULL, 10);
		}
		return 0;
	}
	return -1;
}
int set_userinfo_from_mysql_row(admin_user * userinfo, MYSQL_ROW row)
{
	int i = 0;
	if(!userinfo || !row)
	{
		return -1;
	}
	for(i = 0; i < USERINFO_COLUMN_NUM; i++)
	{
		switch(i)
		{
			case USER_ID:
				if(row[i])
				{
					userinfo->_id = (int)strtoul((char*)row[i], NULL, 10);
				}
				else
				{
					userinfo->_id = -1;
				}
				break;
			case USER_NAME:
				if(row[i])
				{
					strncpy(userinfo->name, (char *)row[i], (USER_NAME_LEN-1));
				}
				else
				{
					strcpy(userinfo->name, "NULL");
				}
				break;
			case USER_PASSWD:
				if(row[i])
				{
					strncpy(userinfo->passwd, (char *)row[i], (USER_PASSWD_LEN-1));
				}
				else
				{
					strcpy(userinfo->passwd, "NULL");
				}
				break;
			case USER_ADMINISTRATOR:
				if(row[i])
				{
					userinfo->administrator = (int)strtoul((char*)row[i], NULL, 10);
				}
				else
				{
					userinfo->administrator = -1;
				}
				break;
			case USER_EMAIL_ALERT:
				if(row[i])
				{
					userinfo->email_alert = (int)strtoul((char*)row[i], NULL, 10);
				}
				else
				{
					userinfo->email_alert = -1;
				}
				break;
			case USER_EMAIL_ADDRESS:
				if(row[i])
				{
					strncpy(userinfo->email_address, (char *)row[i], (USER_EMAIL_LEN-1));
				}
				else
				{
					strcpy(userinfo->email_address, "NULL");
				}
				break;
			case USER_LANGUAGE:
				if(row[i])
				{
					strncpy(userinfo->language, (char *)row[i], (USER_LANGUAGE_LEN-1));
				}
				else
				{
					strcpy(userinfo->language, "NULL");
				}
				break;
			case USER_NOTE:
				if(row[i])
				{
					strncpy(userinfo->note, (char *)row[i], (USER_NOTE_LEN-1));
				}
				else
				{
					strcpy(userinfo->note, "NULL");
				}
				break;
			case USER_LAST_LOGIN_TIME:
				if(row[i])
				{
					strncpy(userinfo->last_login_time, (char *)row[i], (USER_LAST_LOGIN_TIME_LEN-1));
				}
				else
				{
					//strcpy(userinfo->last_login_time, "NULL");
					userinfo->last_login_time[0] = -1;
				}
				break;
			case USER_LAST_LOGIN_IP:
				if(row[i])
				{
					strncpy(userinfo->last_login_ip, (char *)row[i], (USER_LAST_LOGIN_IP_LEN-1));
				}
				else
				{
					strcpy(userinfo->last_login_ip, "NULL");
				}
				break;
			default:
				break;
		}
	}
	return 0;
}
int call_back_get_all_userinfo(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	query_user * result = (query_user *)arg;
	int i = 0;
	
	if(!result)
	{
		syslog(LOG_ERR, "%s line %d Bad input parameter!\n", __func__, __LINE__);
		return -1;
	}
#if DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "result %p row_num %d column_num %d ", result, row_num, column_num);
#endif
	if(result && (result->userinfo == NULL) && row_num > 0 && column_num > 0)
	{
		
		result->userinfo = (admin_user *)malloc(sizeof(admin_user) * row_num);
		if(NULL == result->userinfo)
		{
			syslog(LOG_ERR, "%s line %d memory malloc failed!\n", __func__, __LINE__);
			return -1;
		}
		memset(result->userinfo, 0, sizeof(admin_user) * row_num);
		result->row_num = row_num;
		result->column_num = column_num;
		result->crr_row = 0;
		
	}
	if((result->crr_row < result->row_num) && ((result->userinfo)+(result->crr_row)) && column_num >= USERINFO_COLUMN_NUM)
	{
		set_userinfo_from_mysql_row(((result->userinfo)+(result->crr_row)), row);
		result->crr_row++;
		return 0;
	}
	return -1;
}

//int db_check_user_passwd(char *username, char *passwd, char *userip, int * userid)
int db_check_user_passwd(admin_user *userinfo, int login)
{/*check name and passwd then get userid out */
	MYSQL mysql;
	char * prepareStmt = "select * from %s where name=\"%s\" and passwd=\"%s\"";
	char qeryStmt[256] = {0};
	int result = -1;
	int ret = 0;
	query_user tmpQueryuser;
	memset(&tmpQueryuser, 0, sizeof(query_user));
	if(!userinfo)
	{
		return FALSE;
	}
		
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, userinfo->name, userinfo->passwd);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_userinfo, (void *)&tmpQueryuser);
	
	db_put_database_connection(&mysql);
	
	userinfo->_id = -1;
	if(tmpQueryuser.row_num > 0)
	{
		userinfo->_id = tmpQueryuser.userinfo[0]._id;
	}
	
#if ADMIN_DEBUG_ENABLE
	syslog(LOG_DEBUG, "after check passwd user %s passwd %s result(_id) %d \n", userinfo->name, userinfo->passwd, result);
#endif
	if(tmpQueryuser.row_num > 0)
	{
		if(login)
		{
			db_update_user_login_info_by_id(userinfo);
			
			userinfo->administrator = tmpQueryuser.userinfo[0].administrator;
			memcpy(userinfo->language, tmpQueryuser.userinfo[0].language, USER_LANGUAGE_LEN);
			memcpy(userinfo->last_login_time, tmpQueryuser.userinfo[0].last_login_time, USER_LAST_LOGIN_TIME_LEN);
			memcpy(userinfo->last_login_ip, tmpQueryuser.userinfo[0].last_login_ip, USER_LAST_LOGIN_IP_LEN);
		}
		db_destroy_admin_user(tmpQueryuser.userinfo);
		return TRUE;
	}
	
	syslog(LOG_DEBUG, "check passwd failed! user %s passwd %s \n", userinfo->name, userinfo->passwd);
	
	return FALSE;
}

int db_get_user_passwd(char *username, char *passwd)
{
/*
 *    a call example 
 *	char passwd[64] = {0};
 *    db_get_user_passwd("username", &passwd)
 *	printf("passwd: %s\n", passwd);
 */
	MYSQL mysql;
	char * prepareStmt = "select passwd from %s where name=\"%s\" and passwd=\"%s\"";
	char qeryStmt[256] = {0};
	int ret = 0;
	
	if(!username || !passwd)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, username, passwd);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_first_passwd, passwd);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}


int db_get_all_userinfo_by_name(char *username, query_user *result)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s where name=\"%s\"";
	char qeryStmt[256] = {0};
	int ret = 0;
	
	if(!username || !result)
	{
		return -1;
	}

	memset(result, 0, sizeof(query_user));
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, username);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_userinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_get_all_userinfo_by_id(int _id, query_user *result)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s where _id=%d";
	char qeryStmt[256] = {0};
	int ret = 0;
	
	if(!result)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, _id);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_userinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_get_userinfo_list(query_user *result)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s";
	char qeryStmt[256] = {0};
	int ret = 0;
	
	if(!result)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_userinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_output_userinfo(admin_user * userinfo)
{
	char idstr[SHORT_STR_LEN] = {0};
	if(!userinfo)
	{
		syslog(LOG_ERR, "output userinfo bad input parameter!");
		return -1;
	}
	
	sprintf(idstr, "%d", userinfo->_id);

	printf(" %s  "STR_FORMAT"  "STR_FORMAT"  %s  %s  "STR_FORMAT"  "STR_FORMAT"  "
		STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"\n",
		(userinfo->_id>=0) ? idstr : "NULL",STR_VALUE(userinfo->name), STR_VALUE(userinfo->passwd), 
		(userinfo->administrator>0)?"true":(userinfo->administrator<0)?"NULL":"false", 
		(userinfo->email_alert>0)?"true":(userinfo->email_alert<0)?"NULL":"false", 
		STR_VALUE(userinfo->email_address), STR_VALUE(userinfo->language), STR_VALUE(userinfo->note), 
		STR_VALUE(userinfo->last_login_time), STR_VALUE(userinfo->last_login_ip));
	return 0;
}

int db_output_query_user(query_user * qu_userinfo)
{
	int i = 0;
	if(!qu_userinfo)
	{
		syslog(LOG_ERR, "output query user bad input parameter!");
		return -1;
	}
	if((0 == qu_userinfo->row_num) || (NULL == qu_userinfo->userinfo))
	{
		syslog(LOG_ERR, "output query_user row_num %d qu_userinfo->userinfo %p ", 
			qu_userinfo->row_num, qu_userinfo->userinfo);
		return -1;
	}
	
	for(i = 0; i < qu_userinfo->row_num; i++)
	{
		db_output_userinfo(&(qu_userinfo->userinfo[i]));
	}
	return 0;
}
int db_log_userinfo(admin_user * userinfo)
{
	char idstr[SHORT_STR_LEN] = {0};
	if(!userinfo)
	{
		syslog(LOG_ERR, "output userinfo bad input parameter!");
		return -1;
	}

	sprintf(idstr, "%d", userinfo->_id);

	syslog(LOG_DEBUG, " %s  "STR_FORMAT"  "STR_FORMAT"  %s  %s  "STR_FORMAT"  "STR_FORMAT"  "
		STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"\n",
		(userinfo->_id>=0) ? idstr : "NULL",STR_VALUE(userinfo->name), STR_VALUE(userinfo->passwd), 
		(userinfo->administrator>0)?"true":(userinfo->administrator<0)?"NULL":"false", 
		(userinfo->email_alert>0)?"true":(userinfo->email_alert<0)?"NULL":"false", 
		STR_VALUE(userinfo->email_address), STR_VALUE(userinfo->language), STR_VALUE(userinfo->note), 
		STR_VALUE(userinfo->last_login_time), STR_VALUE(userinfo->last_login_ip));
	return 0;
}

int db_log_query_user(query_user * qu_userinfo)
{
	int i = 0;
	if(!qu_userinfo)
	{
		syslog(LOG_ERR, "output query user bad input parameter!");
		return -1;
	}
	if((0 == qu_userinfo->row_num) || (NULL == qu_userinfo->userinfo))
	{
		syslog(LOG_ERR, "output query_user row_num %d qu_userinfo->userinfo %p ", 
			qu_userinfo->row_num, qu_userinfo->userinfo);
		return -1;
	}
	
	for(i = 0; i < qu_userinfo->row_num; i++)
	{
		db_log_userinfo(&(qu_userinfo->userinfo[i]));
	}
	return 0;
}
void db_destroy_admin_user(admin_user * userinfo)
{
	if(userinfo)
	{
		free(userinfo);
	}
	return;
}

void db_destroy_query_user(query_user * queryuser)
{
	if(queryuser)
	{
		if(queryuser->userinfo)
		{
			db_destroy_admin_user(queryuser->userinfo);
			queryuser->userinfo = NULL;
		}
		free(queryuser);
	}
	return;
}

int db_update_userinfo_by_id(admin_user *userinfo)
{
	MYSQL mysql;
	char * prepareStmt = "update %s set name='%s',email_alert=%s,email_address='%s',language='%s',note='%s'%s";
	char preStmt2[512] = {0};
	int prelen = 0;
	char * queryStmt = (char*)malloc(2048);
	int ret = 0;
	
	if(!userinfo || !queryStmt)
	{
		if(queryStmt)
		{
			free(queryStmt);
		}
		return -1;
	}
	
	if(strcmp(userinfo->passwd,""))
	{
		prelen+=sprintf((char *)preStmt2+prelen, ",passwd='%s'", userinfo->passwd);
	}
	if(userinfo->administrator != -1)
	{
		prelen+=sprintf((char *)preStmt2+prelen, ",administrator=%s", userinfo->administrator?"true":"false");
	}
	if(strcmp(userinfo->last_login_time,""))
	{
		prelen+=sprintf((char *)preStmt2+prelen, ",last_login_time='%s'", userinfo->last_login_time);
	}
	if(strcmp(userinfo->last_login_ip,""))
	{
		prelen+sprintf((char *)preStmt2+prelen, ",last_login_ip='%s'", userinfo->last_login_ip);
	}
	prelen+sprintf((char *)preStmt2+prelen, " where _id=%d", userinfo->_id);
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, userinfo->name, userinfo->email_alert?"true":"false", 
		userinfo->email_address, userinfo->language, userinfo->note, preStmt2);
	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(queryStmt)
	{
		free(queryStmt);
	}
	
	if(ret >= 0)
	{/* 0 maybe means no field changed */
		return 0;
	}
	return -1;
}

int db_update_user_login_info_by_id(admin_user *userinfo)
{
	MYSQL mysql;
	int prelen = 0;
	char queryStmt[256] = {0};
	int ret = 0;
	
	if(!userinfo)
	{
		syslog(LOG_ERR, "update user login info ERROR: bad input parameter: userinfo null pointer");
		return -1;
	}
	
	prelen+=sprintf((char *)queryStmt+prelen, "update %s set ", SOFTAC_ADMIN_USER_TABLE);
	
	if(strcmp(userinfo->last_login_time,""))
	{
		prelen+=sprintf((char *)queryStmt+prelen, "last_login_time='%s'", userinfo->last_login_time);
	}
	else
	{
		prelen+=sprintf((char *)queryStmt+prelen, "last_login_time=NOW()");
	}
	if(strcmp(userinfo->last_login_ip,""))
	{
		prelen+=sprintf((char *)queryStmt+prelen, ",last_login_ip='%s'", userinfo->last_login_ip);
	}
	prelen+=sprintf((char *)queryStmt+prelen, " where _id=%d", userinfo->_id);
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_delete_userinfo_by_id(int _id)
{
	MYSQL mysql;
	char * prepareStmt = "delete from %s where _id=%d";
	char queryStmt[256] = {0};
	int ret = 0;

	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, _id);	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}
int db_delete_userinfo_by_id_id(int startid, int endid)
{
	MYSQL mysql;
	char * prepareStmt = "delete from %s where _id>=%d and _id<=%d";
	char queryStmt[256] = {0};
	int ret = 0;

	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, startid, endid);	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}
int db_add_userinfo_into_admin_user(admin_user *userinfo)
{
	MYSQL mysql;
	char * prepareStmt = "insert into %s values(NULL,'%s','%s',%s,"
		"%s,'%s','%s','%s',NULL,'')";
	
	char * queryStmt = (char*)malloc(2048);
	int ret = 0;
	
	if(!userinfo || !queryStmt)
	{
		if(queryStmt)
		{
			free(queryStmt);
		}
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_ADMIN_USER_TABLE, userinfo->name, userinfo->passwd,
		userinfo->administrator?"true":"false", userinfo->email_alert?"true":"false", userinfo->email_address, userinfo->language, 
		userinfo->note);
	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);

	userinfo->_id = mysql_insert_id(&mysql);
	
	db_put_database_connection(&mysql);
	
	if(queryStmt)
	{
		free(queryStmt);
	}
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}
#endif
#endif

#ifdef OBJECT_ACTIVE_SUPPORT
#if OBJECT_ACTIVE_SUPPORT
int set_activeinfo_from_mysql_row(object_active* activeinfo, MYSQL_ROW row)
{
	int i = 0;
	if(!activeinfo || !row)
	{
		return -1;
	}
	for(i = 0; i < ACTIVEINFO_COLUMN_NUM; i++)
	{
		switch(i)
		{
			case ACTIVE_ID:
				if(row[i])
				{
					activeinfo->_id = (int)strtoul((char*)row[i], NULL, 10);
				}
				else
				{
					activeinfo->_id = -1;
				}
				break;
			case ACTIVE_EVENT_TIME:
				if(row[i])
				{
					strncpy(activeinfo->event_time, (char *)row[i], (ACTIVE_EVENT_TIME_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->event_time, "NULL");
					activeinfo->event_time[0] = -1;
				}
				break;
			case ACTIVE_EVENT:
				if(row[i])
				{
					strncpy(activeinfo->event, (char *)row[i], (ACTIVE_EVENT_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->event, "NULL");
					activeinfo->event[0] = -1;
				}
				break;
			case ACTIVE_MSG:
				if(row[i])
				{
					strncpy(activeinfo->msg, (char *)row[i], (ACTIVE_MSG_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->msg, "NULL");
					activeinfo->msg[0] = -1;
				}
				break;
			case ACTIVE_ADMIN:
				if(row[i])
				{
					strncpy(activeinfo->admin, (char *)row[i], (ACTIVE_ADMIN_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->admin, "NULL");
					activeinfo->admin[0] = -1;
				}
				break;
			case ACTIVE_AP:
				if(row[i])
				{
					strncpy(activeinfo->ap, (char *)row[i], (ACTIVE_AP_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->ap, "NULL");
					activeinfo->ap[0] = -1;
				}
				break;
			case ACTIVE_USER:
				if(row[i])
				{
					strncpy(activeinfo->user, (char *)row[i], (ACTIVE_USER_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->user, "NULL");
					activeinfo->user[0] = -1;
				}
				break;
			case ACTIVE_GUEST:
				if(row[i])
				{
					strncpy(activeinfo->guest, (char *)row[i], (ACTIVE_GUEST_LEN-1));
				}
				else
				{
					//strcpy(activeinfo->guest, "NULL");
					activeinfo->guest[0] = -1;
				}
				break;
			default:
				break;
		}
	}
	return 0;
}

int call_back_get_all_activeinfo(void * arg, MYSQL_ROW row, int row_num, int column_num)
{
	query_active * result = (query_active*)arg;
	int i = 0;
	
	if(!result)
	{
		syslog(LOG_ERR, "%s line %d Bad input parameter!\n", __func__, __LINE__);
		return -1;
	}
#if DATABASE_DEBUG_ENABLE
	syslog(LOG_DEBUG, "result %p userinfo %p row_num %d column_num %d ", result, result->activeinfo, row_num, column_num);
#endif
	if(result && (result->activeinfo == NULL) && row_num > 0 && column_num > 0)
	{
		
		result->activeinfo = (object_active *)malloc(sizeof(object_active) * row_num);
		if(NULL == result->activeinfo)
		{
			syslog(LOG_ERR, "%s line %d memory malloc failed!\n", __func__, __LINE__);
			return -1;
		}
		memset(result->activeinfo, 0, sizeof(object_active) * row_num);
		result->row_num = row_num;
		result->column_num = column_num;
		result->crr_row = 0;
		
	}
	if((result->crr_row < result->row_num) && ((result->activeinfo)+(result->crr_row)) && column_num >= ACTIVEINFO_COLUMN_NUM)
	{
		set_activeinfo_from_mysql_row(((result->activeinfo)+(result->crr_row)), row);
		result->crr_row++;
		return 0;
	}
	return -1;
}


int db_get_all_activeinfo_after_id(int _id, query_active*result)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s where _id>%d";
	/*select * from table where DATE_SUB(CURDATE(), INTERVAL INTERVAL 1 MONTH) <= date(column_time); 查询一个月之内的记录*/
	char qeryStmt[256] = {0};
	int ret = 0;
	
	if(!result)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(qeryStmt, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, _id);

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_activeinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret >= 0)
	{
		return 0;
	}
	return -1;
}

int db_get_all_activeinfo_after_id_by_object(int _id, query_active*result, char * admin, char *ap, char *user, char *guest)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s where _id>%d";
	/*select * from table where DATE_SUB(CURDATE(), INTERVAL INTERVAL 1 MONTH) <= date(column_time); 查询一个月之内的记录*/
	char qeryStmt[256] = {0};
	int len = 0;
	int ret = 0;
	
	if(!result)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	len += sprintf(qeryStmt+len, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, _id);
	if(admin && strcmp(admin, ""))
	{
		if(!strcmp(admin, "all"))
		{
			len += sprintf(qeryStmt+len, " and admin is NOT NULL and admin != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and admin='%s'", admin);
		}
	}
	if(ap && strcmp(ap, ""))
	{
		if(!strcmp(ap, "all"))
		{
			len += sprintf(qeryStmt+len, " and ap is NOT NULL and ap != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and ap='%s'", ap);
		}
	}
	if(user && strcmp(user, ""))
	{
		if(!strcmp(user, "all"))
		{
			len += sprintf(qeryStmt+len, " and user is NOT NULL and user != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and user='%s'", user);
		}
	}

	if(guest && strcmp(guest, ""))
	{
		if(!strcmp(guest, "all"))
		{
			len += sprintf(qeryStmt+len, " and guest is NOT NULL and guest != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and guest='%s'", guest);
		}
	}

	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_activeinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret >= 0)
	{
		return 0;
	}
	return -1;
}

int db_get_all_activeinfo_after_id_by_object_within_hours(int _id, query_active*result, char * admin, char *ap, char *user, char *guest, int hours)
{
	MYSQL mysql;
	char * prepareStmt = "select * from %s where _id>%d";
	/*select * from table where DATE_SUB(CURDATE(), INTERVAL INTERVAL 1 MONTH) <= date(column_time); 查询一个月之内的记录*/
	char qeryStmt[256] = {0};
	char timeStr[64] = {0};
	int len = 0;
	int ret = 0;
	
	if(!result)
	{
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	len += sprintf(qeryStmt+len, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, _id);
	if(hours)
	{
		time_t now = 0;
		now = time(NULL);
		now -= (hours*3600);
		len += sprintf(qeryStmt+len, " and event_time > \"%s\"", get_db_format_time_from_time_t(now, timeStr));
	}
	if(admin && strcmp(admin, ""))
	{
		if(!strcmp(admin, "all"))
		{
			len += sprintf(qeryStmt+len, " and admin is NOT NULL and admin != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and admin='%s'", admin);
		}
	}
	if(ap && strcmp(ap, ""))
	{
		if(!strcmp(ap, "all"))
		{
			len += sprintf(qeryStmt+len, " and ap is NOT NULL and ap != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and ap='%s'", ap);
		}
	}
	if(user && strcmp(user, ""))
	{
		if(!strcmp(user, "all"))
		{
			len += sprintf(qeryStmt+len, " and user is NOT NULL and user != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and user='%s'", user);
		}
	}

	if(guest && strcmp(guest, ""))
	{
		if(!strcmp(guest, "all"))
		{
			len += sprintf(qeryStmt+len, " and guest is NOT NULL and guest != ''");
		}
		else
		{
			len += sprintf(qeryStmt+len, " and guest='%s'", guest);
		}
	}
//	syslog(LOG_DEBUG, "get object queryStmt %s", qeryStmt);
	ret = db_do_query(&mysql, qeryStmt, call_back_get_all_activeinfo, result);
	
	db_put_database_connection(&mysql);
	
	if(ret >= 0)
	{
		return 0;
	}
	return -1;
}

int db_get_all_activeinfo_by_ids(alert_info_db_s *alert_ids, int archived, query_active*result)
{/* alert_ids[i].active_id end by 0 , archived is -1: all, 0: false, 1: true */
	MYSQL mysql;
	char * prepareStmt = "select * from %s where _id=%d";
	char qeryStmt[1024] = {0};
	int len = 0;
	int ret = 0;
	int i = 0;
	
	if(!result || !alert_ids)
	{
		return -1;
	}
	while(i < 256 && alert_ids[i].active_id && (-1 != archived && alert_ids[i].archived != archived))
	{
		i++;
	}
	if(i < 256 && (alert_ids[i].active_id && (-1 == archived || alert_ids[i].archived == archived)))
	{
		len += sprintf(qeryStmt+len, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, alert_ids[i].active_id);
		i++;
		while(i < 256)
		{
			if(alert_ids[i].active_id && (-1 == archived || alert_ids[i].archived == archived))
			{
				len += sprintf(qeryStmt+len, " or _id=%d", alert_ids[i].active_id);
			}
			i++;
		}
		db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
			SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

		ret = db_do_query(&mysql, qeryStmt, call_back_get_all_activeinfo, result);
		
		db_put_database_connection(&mysql);
		
		if(ret >= 0)
		{
			return 0;
		}
	}
	
	return -1;
}

void db_destroy_object_active(object_active * activeinfo)
{
	if(activeinfo)
	{
		free(activeinfo);
	}
	return;
}

void db_destroy_query_active(query_active * queryactive)
{
	if(queryactive)
	{
		if(queryactive->activeinfo)
		{
			db_destroy_object_active(queryactive->activeinfo);
			queryactive->activeinfo = NULL;
		}
		free(queryactive);
	}
	return;
}
int db_add_activeinfo_into_object_active(object_active *activeinfo)
{
	MYSQL mysql;
	char * prepareStmt = "insert into %s values(NULL,NULL,'%s','%s','%s','%s','%s','%s')";
	
	char * queryStmt = (char*)malloc(2048);
	int ret = 0;
	
	if(!activeinfo || !queryStmt)
	{
		if(queryStmt)
		{
			free(queryStmt);
		}
		return -1;
	}
	
	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, activeinfo->event,activeinfo->msg, 
		activeinfo->admin, activeinfo->ap,activeinfo->user, activeinfo->guest);
	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	if(ret > 0)
	{
		ret = mysql_insert_id(&mysql);
	}
	db_put_database_connection(&mysql);
	
	if(queryStmt)
	{
		free(queryStmt);
	}
	if(ret >= 0)
	{
		return ret;
	}
	return -1;
}

int db_delete_activeinfo_before_time(char * time)
{
	MYSQL mysql;
	char * prepareStmt = "delete from %s where event_time < \"%s\"";
	char queryStmt[256] = {0};
	int ret = 0;

	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, time);	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_delete_activeinfo_after_time(char * time)
{
	MYSQL mysql;
	char * prepareStmt = "delete from %s where event_time > \"%s\"";
	char queryStmt[256] = {0};
	int ret = 0;

	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, time);	

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_delete_activeinfo_by_id_id(int startid, int endid)
{
	MYSQL mysql;
	char * prepareStmt = "delete from %s where _id>=%d and _id<=%d";
	char queryStmt[256] = {0};
	int ret = 0;

	db_get_database_connection(&mysql,SOFTAC_DATABASE_HOSTNAME/*host*/, SOFTAC_DATABASE_USER/*user*/, 
		SOFTAC_DATABASE_PASSWD/**/, SOFTAC_DATABASE_NAME/*database*/);

	sprintf(queryStmt, prepareStmt, SOFTAC_OBJECT_ACTIVE_TABLE, startid, endid);

	ret = db_do_query(&mysql, queryStmt, NULL, NULL);
	
	db_put_database_connection(&mysql);
	
	if(ret > 0)
	{
		return 0;
	}
	return -1;
}

int db_output_activeinfo(object_active * activeinfo)
{
	char idstr[32] = {0};
	if(!activeinfo)
	{
		syslog(LOG_ERR, "output activeinfo bad input parameter!");
		return -1;
	}

	sprintf(idstr, "%d", activeinfo->_id);
	printf(" %s  "STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"  "
		STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"\n",
		(activeinfo->_id>=0) ? idstr : "NULL",STR_VALUE(activeinfo->event_time), 
		STR_VALUE(activeinfo->event), STR_VALUE(activeinfo->msg), 
		STR_VALUE(activeinfo->admin), STR_VALUE(activeinfo->ap), 
		STR_VALUE(activeinfo->user), STR_VALUE(activeinfo->guest));
	return 0;
}

int db_output_query_active(query_active * qu_activeinfo)
{
	int i = 0;
	if(!qu_activeinfo)
	{
		syslog(LOG_ERR, "output query active bad input parameter!");
		return -1;
	}
	if((0 == qu_activeinfo->row_num) || (NULL == qu_activeinfo->activeinfo))
	{
		syslog(LOG_ERR, "output query_active row_num %d qu_activeinfo->activeinfo %p ", 
			qu_activeinfo->row_num, qu_activeinfo->activeinfo);
		return -1;
	}
	
	for(i = 0; i < qu_activeinfo->row_num; i++)
	{
		db_output_activeinfo(&(qu_activeinfo->activeinfo[i]));
	}
	return 0;
}

int db_log_activeinfo(object_active * activeinfo)
{
	char idstr[32] = {0};
	if(!activeinfo)
	{
		syslog(LOG_ERR, "output activeinfo bad input parameter!");
		return -1;
	}

	sprintf(idstr, "%d", activeinfo->_id);
	syslog(LOG_DEBUG, " %s  "STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT"  "
		STR_FORMAT"  "STR_FORMAT"  "STR_FORMAT" \n",
		(activeinfo->_id>=0) ? idstr : "NULL",STR_VALUE(activeinfo->event_time), 
		STR_VALUE(activeinfo->event), STR_VALUE(activeinfo->msg), 
		STR_VALUE(activeinfo->admin), STR_VALUE(activeinfo->ap), 
		STR_VALUE(activeinfo->user), STR_VALUE(activeinfo->guest));
	return 0;
}

int db_log_query_active(query_active * qu_activeinfo)
{
	int i = 0;
	if(!qu_activeinfo)
	{
		syslog(LOG_ERR, "output query active bad input parameter!");
		return -1;
	}
	if((0 == qu_activeinfo->row_num) || (NULL == qu_activeinfo->activeinfo))
	{
		syslog(LOG_ERR, "output query_active row_num %d qu_activeinfo->activeinfo %p ", 
			qu_activeinfo->row_num, qu_activeinfo->activeinfo);
		return -1;
	}
	
	for(i = 0; i < qu_activeinfo->row_num; i++)
	{
		db_log_activeinfo(&(qu_activeinfo->activeinfo[i]));
	}
	return 0;
}

int db_add_object_active(object_active_args *args)
{
	object_active activeinfo;
	int i = 0;
	int ret = 0;
	char * msg = NULL;
	memset(&activeinfo, 0, sizeof(activeinfo));
	if(!args || args->active_index >= MAX_OBJECT_ACTIVE_INDEX)
	{
		if(args)
		{
			syslog(LOG_ERR, "bad input argument active index %d ", args->active_index);
		}
		else
		{
			syslog(LOG_ERR, "bad input argument args null ");
		}
		return -1;
	}
	if((args->argc < 0) || (args->argc > 0 && NULL == args->argv) || args->argc > 10)
	{
		syslog(LOG_ERR, "bad input argument argc %d but argv %p ", args->argc, args->argv);
		return -1;
	}
	strncpy(activeinfo.event, known_object_active[args->active_index].key, ACTIVE_EVENT_LEN-1);
	/*snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, known_object_active[args->active_index].msg known_object_active[args->active_index].nullstr, 
		args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5], args->argv[6], args->argv[7], 
		args->argv[8], args->argv[9]);*/
	if(args->user_defined_msg)
	{
		msg = args->user_defined_msg;
	}
	else
	{
		msg = known_object_active[args->active_index].msg;
	}
	
	switch(args->argc)
	{
		case 0:			
			snprintf((char *)activeinfo.msg, ACTIVE_MSG_LEN-1, "%s", msg);
			break;
		case 1:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0]);
			break;
		case 2:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1]);
			break;
		case 3:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2]);
			break;
		case 4:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3]);
			break;
		case 5:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4]);
			break;
		case 6:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5]);
			break;
		case 7:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5],
				args->argv[6]);
			break;
		case 8:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5],
				args->argv[6], args->argv[7]);
			break;
		case 9:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5],
				args->argv[6], args->argv[7], args->argv[8]);
			break;
		case 10:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5],
				args->argv[6], args->argv[7], args->argv[8], args->argv[9]);
			break;
		default:
			snprintf(activeinfo.msg, ACTIVE_MSG_LEN-1, msg, 
				args->argv[0], args->argv[1], args->argv[2], args->argv[3], args->argv[4], args->argv[5],
				args->argv[6], args->argv[7], args->argv[8], args->argv[9]);
			break;
	}
	if(known_object_active[args->active_index].adminIndex > 0)
	{
		strncpy(activeinfo.admin, (known_object_active[args->active_index].adminIndex <= args->argc)?
			args->argv[known_object_active[args->active_index].adminIndex-1]:"", ACTIVE_ADMIN_LEN);
	}
	
	if(known_object_active[args->active_index].apIndex > 0)
	{
		strncpy(activeinfo.ap, (known_object_active[args->active_index].apIndex <= args->argc)?
			args->argv[known_object_active[args->active_index].apIndex-1]:"", ACTIVE_AP_LEN);
	}
	
	if(known_object_active[args->active_index].userIndex > 0)
	{
		strncpy(activeinfo.user, (known_object_active[args->active_index].userIndex <= args->argc)?
			args->argv[known_object_active[args->active_index].userIndex-1]:"", ACTIVE_USER_LEN);
	}
	
	if(known_object_active[args->active_index].guestIndex > 0)
	{
		strncpy(activeinfo.guest, (known_object_active[args->active_index].guestIndex <= args->argc)?
			args->argv[known_object_active[args->active_index].guestIndex-1]:"", ACTIVE_GUEST_LEN);
	}
	ret = db_add_activeinfo_into_object_active(&activeinfo);
	if(0 > ret)
	{
		syslog(LOG_DEBUG, "add activeinfo into object active failed, ret %d", ret);
		db_log_activeinfo(&activeinfo);
	}
	return ret;
}

int active_ap_disconnect(unsigned char * mac)
{/* mac[6] trans to  mac string like "00:1f:64:00:00:01" */
	char macStr[32] = {0};
	if(mac)
	{
		snprintf(macStr, 31, "%02x:%02x:%02x:%02x:%02x:%02x", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	
	return active_ap_actived_or_changed(macStr, AP_WAS_DISCONNECTED);
}

int active_ap_offlined(unsigned char * mac)
{/* mac[6] trans to  mac string like "00:1f:64:00:00:01" */
	char macStr[32] = {0};
	if(mac)
	{
		snprintf(macStr, 31, "%02x:%02x:%02x:%02x:%02x:%02x", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	
	return active_ap_actived_or_changed(macStr, AP_WAS_OFFLINED);
}
int active_ap_auto_readmited(unsigned char * mac)
{/* mac[6] trans to  mac string like "00:1f:64:00:00:01" */
	char macStr[32] = {0};
	if(mac)
	{
		snprintf(macStr, 31, "%02x:%02x:%02x:%02x:%02x:%02x", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	
	return active_ap_actived_or_changed(macStr, AP_WAS_AUTO_READMITED);
}

int active_ap_discovery_pending(unsigned char * mac)
{/* mac[6] trans to  mac string like "00:1f:64:00:00:01" */
	char macStr[32] = {0};
	if(mac)
	{
		snprintf(macStr, 31, "%02x:%02x:%02x:%02x:%02x:%02x", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	
	return active_ap_actived_or_changed(macStr, AP_WAS_DISCOVERY_PENDING);
}

int active_ap_operated_by_admin(char * ap, char * admin, int operation)
{/* this function is used for ext module the mac format is like "00:1f:64:00:00:01" */
	object_active_args args;
	char * argv[] = {ap, admin};
	int ret = 0;

	if(AP_UPGRADE_BY_ADMIN_START <= operation || AP_ADMITED_BY_ADMIN > operation)
	{
		return -1;
	}
	memset(&args, 0, sizeof(object_active_args));
	args.active_index = operation;
	args.argc = 2;
	args.argv = argv;
	ret = db_add_object_active(&args);
	return ret;
}

int active_ap_actived_or_changed(char * ap, int active)
{/* this function is used for ext module the mac format is like "00:1f:64:00:00:01" */
	object_active_args args;
	char * argv[] = {ap};
	int ret = 0;

	if(AP_ADMITED_BY_ADMIN <= active || AP_DOWNLOAD_PUBKEY_START > active)
	{
		return -1;
	}
	memset(&args, 0, sizeof(object_active_args));
	args.active_index = active;
	args.argc = 1;
	args.argv = argv;
	ret = db_add_object_active(&args);
	return ret;
}

int active_ap_upgrade_to_version(char * ap, char * operater, char * version, char * info, int step)
{/* this function is used for ext module the mac format is like "00:1f:64:00:00:01" */
	object_active_args args;
	char * tmpoper = operater?operater:"";
	char * argv[] = {ap, tmpoper, version, info};
	int ret = 0;

	if(USER_KICKED_BY_ADMIN <= step || AP_UPGRADE_BY_ADMIN_START > step)
	{
		return -1;
	}
	memset(&args, 0, sizeof(object_active_args));
	args.active_index = step;
	args.argc = 3;
	if(AP_UPGRADE_BY_ADMIN_FAILED == step || AP_AUTO_UPGRADE_FAILED == step ||
		AP_UPGRADE_BY_ADMIN_STEP == step || AP_AUTO_UPGRADE_STEP == step)
	{
		args.argc = 4;
		if(!info)
		{
			args.argv[3] = "";
		}
	}
	args.argv = argv;
	ret = db_add_object_active(&args);
	return ret;
}

int active_user_operated_by_admin(char * user, char * admin, int operation)
{/* this function is used for ext module the mac format is like "00:1f:64:00:00:01" */
	object_active_args args;
	char * argv[] = {user, admin};
	int ret = 0;

	if(MAX_OBJECT_ACTIVE_INDEX <= operation || USER_KICKED_BY_ADMIN > operation)
	{
		return -1;
	}
	memset(&args, 0, sizeof(object_active_args));
	args.active_index = operation;
	args.argc = 2;
	args.argv = argv;
	ret = db_add_object_active(&args);
	return ret;
}

#endif
#endif

#ifdef TEST_MAIN
#if TEST_MAIN
int main()
{
#ifdef OBJECT_ACTIVE_SUPPORT
	object_active activeinfo;
	query_active queryactive;
	object_active_args args;
	char * argv[] = {"00:1f:64:00:08:09"};
	memset(&args, 0, sizeof(object_active_args));
	memset(&activeinfo, 0, sizeof(activeinfo));
	strcpy(activeinfo.event, "test event");
	strcpy(activeinfo.msg, "test msg");
	//db_add_activeinfo_into_object_active(&activeinfo);
	//db_delete_activeinfo_after_time("2014-08-13 05:40:49");
	memset(&queryactive, 0, sizeof(queryactive));
	args.active_index = AP_WAS_DISCONNECTED;
	args.argc = 1;
	args.argv = argv;
	db_add_object_active(&args);
	db_get_all_activeinfo_after_id(0, &queryactive);
	db_output_query_active(&queryactive);

	db_destroy_object_active(queryactive.activeinfo);
#endif
#ifdef ADMIN_USER_SUPPORT
	query_user queryuser;
	memset(&queryuser, 0, sizeof(queryuser));
	//db_delete_userinfo_by_id_id(2, 3);
	db_get_userinfo_list(&queryuser);
	db_output_query_user(&queryuser);
	db_destroy_admin_user(queryuser.userinfo);
#endif
	return 0;
}
#endif
#endif


