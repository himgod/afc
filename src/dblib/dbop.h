#ifndef _DB_OPERATION_LIB_H_
#define _DB_OPERATION_LIB_H_

#define SOFTAC_DATABASE_HOSTNAME "localhost"
#define SOFTAC_DATABASE_USER "AFC"
#define SOFTAC_DATABASE_PASSWD "CFA"
#define SOFTAC_DATABASE_NAME "softacDB"
#define SOFTAC_ADMIN_USER_TABLE "admin_user"
#define SOFTAC_OBJECT_ACTIVE_TABLE "object_active"

#define STR_FORMAT "%s%s%s"
#define ARG_NULL(arg) ((NULL == arg) || (-1 == arg[0]))
#define STR_VALUE(arg) ARG_NULL(arg) ? "":"'",(ARG_NULL(arg) ? "NULL" : arg),ARG_NULL(arg) ? "":"'"
#define PTR_TO_STR(arg) ARG_NULL(arg)?"":(arg)

#define ADMIN_USER_SUPPORT 1

#define OBJECT_ACTIVE_SUPPORT 1

//#define TEST_MAIN 1

#define DATABASE_DEBUG_ENABLE 0
#define MAX_RESULT_STRING_LEN 4092
#define MAX_ROW_NUM 1024
#define MAX_COLUMN_NUM 32
#define MAX_VALUE_LEN 1024
#define SHORT_STR_LEN 32
#define SIMPLE_STR_LEN 64
#define LONG_STR_LEN 255
#define TRUE 1
#define FALSE 0

typedef struct alert_info_db{
	int active_id;
	int archived;
} alert_info_db_s;

typedef struct query_result_str{
	int len;
	char result[MAX_RESULT_STRING_LEN];
} result_string;
struct one_row_data
{
	int column_num;
	char ** row;// (MYSQL_ROW )malloc(sizeof(char *)*column_num)
};
#ifdef ADMIN_USER_SUPPORT

typedef enum 
{
	USER_ID = 0,
	USER_NAME,
	USER_PASSWD,
	USER_ADMINISTRATOR,
	USER_EMAIL_ALERT,
	USER_EMAIL_ADDRESS,
	USER_LANGUAGE,
	USER_NOTE,
	USER_LAST_LOGIN_TIME,
	USER_LAST_LOGIN_IP,
	USERINFO_COLUMN_NUM,
} USERINFO_COLUM_INDEX;

#define USER_NAME_LEN SIMPLE_STR_LEN
#define USER_PASSWD_LEN SIMPLE_STR_LEN
#define USER_EMAIL_LEN SIMPLE_STR_LEN
#define USER_LANGUAGE_LEN SIMPLE_STR_LEN
#define USER_NOTE_LEN LONG_STR_LEN
#define USER_LAST_LOGIN_TIME_LEN SIMPLE_STR_LEN
#define USER_LAST_LOGIN_IP_LEN SIMPLE_STR_LEN

typedef struct admin_userinfo
{
	int _id;
	char name[USER_NAME_LEN];
	char passwd[USER_PASSWD_LEN];
	char administrator;
	char email_alert;
	char email_address[USER_EMAIL_LEN];
	char language[USER_LANGUAGE_LEN];
	char note[USER_NOTE_LEN];
	char last_login_time[USER_LAST_LOGIN_TIME_LEN];
	char last_login_ip[USER_LAST_LOGIN_IP_LEN];
} admin_user;

typedef struct query_userinfo{
	int row_num;
	int column_num;
	int crr_row;
	admin_user *userinfo;
} query_user;

#endif

#ifdef OBJECT_ACTIVE_SUPPORT

typedef enum 
{
	ACTIVE_ID = 0,
	ACTIVE_EVENT_TIME,
	ACTIVE_EVENT,
	ACTIVE_MSG,
	ACTIVE_ADMIN,
	ACTIVE_AP,
	ACTIVE_USER,
	ACTIVE_GUEST,
	ACTIVEINFO_COLUMN_NUM,
} ACTIVEINFO_COLUM_INDEX;
#define ACTIVE_EVENT_TIME_LEN SIMPLE_STR_LEN
#define ACTIVE_EVENT_LEN SIMPLE_STR_LEN
#define ACTIVE_MSG_LEN LONG_STR_LEN
#define ACTIVE_ADMIN_LEN SIMPLE_STR_LEN
#define ACTIVE_AP_LEN SHORT_STR_LEN
#define ACTIVE_USER_LEN SHORT_STR_LEN
#define ACTIVE_GUEST_LEN SHORT_STR_LEN
#define WAI_OBJECT_AP " AP[%s] "
#define WAI_OBJECT_ADMIN " Admin[%s] "
typedef struct object_activeinfo
{
	int _id;
	char event_time[ACTIVE_EVENT_TIME_LEN];
	char event[ACTIVE_EVENT_LEN];
	char msg[ACTIVE_MSG_LEN];
	char admin[ACTIVE_ADMIN_LEN];
	char ap[ACTIVE_AP_LEN];
	char user[ACTIVE_USER_LEN];
	char guest[ACTIVE_GUEST_LEN];
} object_active;

typedef enum object_index
{
	/*admin info changed */
	ADMIN_CHANGE_INFO,
	/*ap changed automatically */
	AP_DOWNLOAD_PUBKEY_START,
	AP_DOWNLOAD_PUBKEY_FAILED,
	AP_SAVE_PUBKEY_FAILED,
	AP_DOWNLOAD_SAVE_PUBKEY_SUCCESS,
	AP_WAS_DISCOVERY_PENDING,
	AP_WAS_DISCONNECTED,
	AP_WAS_OFFLINED,
	AP_WAS_AUTO_READMITED,
	/*ap was operated by admin */
	AP_ADMITED_BY_ADMIN,
	AP_FORGOTEN_BY_ADMIN,
	AP_RESTARTED_BY_ADMIN,
	AP_BLOCKED_BY_ADMIN,
	/*ap was upgraded by admin or auto to a special version */
	AP_UPGRADE_BY_ADMIN_START,
	AP_UPGRADE_BY_ADMIN_STEP,
	AP_UPGRADE_BY_ADMIN_FAILED,
	AP_UPGRADED_BY_ADMIN_SUCCESS,
	AP_AUTO_UPGRADE_START,
	AP_AUTO_UPGRADE_STEP,
	AP_AUTO_UPGRADE_FAILED,
	AP_AUTO_UPGRADED_SUCCESS,
	/*user operated by admin */
	USER_KICKED_BY_ADMIN,
	USER_BLOCKED_BY_AMDIN,
	USER_AUTHORIZED_BY_ADMIN,
	USER_UNAUTHORIZED_BY_ADMIN,
	
	/*please add befor MAX_OBJECT_ACTIVE_INDEX */
	MAX_OBJECT_ACTIVE_INDEX,
} object_index_e;

typedef struct object_active_pre
{
	char *key;
	char *msg;
	//char *nullstr;
	int  adminIndex;/* start from 1,   0 as no need this arg */
	int  apIndex;
	int  userIndex;
	int  guestIndex;
} object_active_pre;
typedef struct active_args
{
	object_index_e active_index;
	int argc;/*max arg count is 10 */
	char ** argv;
	char * user_defined_msg;/* NULL or user defined msg to replace known msg */
} object_active_args;

typedef struct query_activeinfo{
	int row_num;
	int column_num;
	int crr_row;
	object_active *activeinfo;
} query_active;

#endif

typedef struct query_result_arr{
	int row_num;
	int column_num;
	int crr_row;
	char ***result;
} result_array;

/* timeinfo is input time , year is since 1900, month is 0-11, timeStr is output arg*/
char * get_db_format_time_from_tm(struct tm * timeinfo, char * timeStr);
char * get_db_format_time_from_time_t(time_t rawtime, char * timeStr);
struct tm* get_tm_from_db_format_time(struct tm * timeinfo, char * timeStr);
time_t get_time_t_from_db_format_time(char * timeStr);


#ifdef ADMIN_USER_SUPPORT
//int db_check_user_passwd(char *username, char *passwd, int * userid);
int db_check_user_passwd(admin_user *userinfo, int login);
int db_get_user_passwd(char *username, char *passwd);
int db_get_all_userinfo_by_name(char *username, query_user *result);
int db_get_all_userinfo_by_id(int _id, query_user *result);
int db_output_userinfo(admin_user * userinfo);
int db_output_query_user(query_user * qu_userinfo);
int db_log_userinfo(admin_user * userinfo);
int db_log_query_user(query_user * qu_userinfo);
void db_destroy_admin_user(admin_user * userinfo);
void db_destroy_query_user(query_user * queryuser);
int db_update_userinfo_by_id(admin_user *userinfo);
int db_update_user_login_info_by_id(admin_user *userinfo);
int db_delete_userinfo_by_id(int _id);
int db_add_userinfo_into_admin_user(admin_user *userinfo);
int db_get_userinfo_list(query_user *result);

#endif
#ifdef OBJECT_ACTIVE_SUPPORT
int db_get_all_activeinfo_after_id(int _id, query_active*result);
int db_get_all_activeinfo_after_id_by_object(int _id, query_active*result, char * admin, char *ap, char *user, char *guest);
int db_get_all_activeinfo_after_id_by_object_within_hours(int _id, query_active*result, char * admin, char *ap, char *user, char *guest, int hours);
int db_get_all_activeinfo_by_ids(alert_info_db_s *alert_ids, int archived, query_active*result);
void db_destroy_object_active(object_active * activeinfo);
void db_destroy_query_active(query_active * queryactive);
int db_add_activeinfo_into_object_active(object_active *activeinfo);
int db_delete_activeinfo_before_time(char * time);
int db_output_activeinfo(object_active * activeinfo);
int db_output_query_active(query_active * qu_activeinfo);
int db_log_activeinfo(object_active * activeinfo);
int db_log_query_active(query_active * qu_activeinfo);
int active_ap_disconnect(unsigned char * mac);
int active_ap_offlined(unsigned char * mac);
int active_ap_auto_readmited(unsigned char * mac);
int active_ap_discovery_pending(unsigned char * mac);
int active_ap_operated_by_admin(char * ap, char * admin, int operate);
int active_ap_actived_or_changed(char * ap, int active);
int active_ap_upgrade_to_version(char * ap, char * operater, char * version, char * info, int step);
int active_user_operated_by_admin(char * user, char * admin, int operation);

#endif

#endif
