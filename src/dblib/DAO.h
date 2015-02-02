#ifndef _DAO_H
#define _DAO_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "/usr/include/mysql/mysql.h"
#include <stdlib.h>

//#define SOFTAC_DATABASE_HOST "localhost"
//#define SOFTAC_DATABASE_USER "AFC"
//#define SOFTAC_DATABASE_PASSWD "CFA"
//#define SOFTAC_DATABASE_NAME "softacDB"
#define SOFTAC_SYS_HEALTH_SCORE_TABLE "sys_health_score"
#define SOFTAC_AP_INDEX_INFO_TABLE "ap_index_info"
#define SOFTAC_AFI_DETAILINFO_DB_TABLE "afi_detailinfo_db"
#define SOFTAC_THROUGHOUT_INDEX_INFO_TABLE "throughout_index_info"
#define SOFTAC_USER_DETAILINFO_DB_TABLE "user_detailinfo_db"
#define SOFTAC_USER_INDEX_INFO_TABLE "user_index_info"
#define SOFTAC_WIFI_INDEX_INFO_TABLE "wifi_index_info"
#define SOFTAC_WIRELESS_INDEX_INFO_TABLE "wireless_index_info"

//#define SOFTAC_DATABASE_TEST_TABLE "test"

#define MAC_ADDRESS_LENGTH 40
#define MAC_ADDRESS_ALIAS_LENGTH 200
#define AFI_HOSTNAME_LENGTH 200
#define ACCESS_POINT_LENGTH 200
#define ESSID_LENGTH 200
#define AFI_N_LENGTH 200
#define AFI_NAME_LENGTH 200
#define AFI_TYPE_LENGTH 40
#define AFI_VERSION_LENGTH 40
#define IP_ADDRESS_LENGTH 16
#define DEVICE_TYPE_LENGTH 32
#define TIMESTAMP_LENGTH 40

#define SUCCESS 0
#define FAILURE -1 
//#define SIMPLE_STR_LEN 64 
#define FORMAT_TIME_STR "%Y-%m-%d %H:%M:%S"

#define TEST_MAIN 1
#define DEBUG_TABLE_AP_INDEX_ENABLE 0
#define DEBUG_TABLE_SYS_HEALTH_SCORE_ENABLE 0
#define DEBUG_TABLE_AFI_DETAIL_INFO_ENABLE 0
#define DEBUG_TABLE_THROUGHOUT_INDEX_ENABLE 0
#define DEBUG_TABLE_USER_DETAIL_INFO_ENABLE 0
#define DEBUG_TABLE_USER_INDEX_ENABLE 0
#define DEBUG_TABLE_WIFI_INDEX_ENABLE 0
#define DEBUG_TABLE_WIRELESS_INDEX_ENABLE 0

//时间辅助数组
int month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
/**
 *this is for struct body
 *每一张表格对应表示字段的构成的结构体和
 *相应存储查询集的结构体，以query_开头
 *其query_开头的结构体每一个属性分别表示：
 *num_columns为字段个数
 *num_rows表示查询结果集中的行数
 *cur_row表示当前操作的行
 *最后一个表示对应表结构的结构体类型的指针，存储结果集。
 */
 
//afi_detailinfo
//AFi详细信息
typedef struct afi_detailinfo{
    char mac_address[MAC_ADDRESS_LENGTH];
    char afi_name[AFI_NAME_LENGTH];
    char afi_type[AFI_TYPE_LENGTH];
    char afi_version[AFI_VERSION_LENGTH];
    char afi_ip_address[IP_ADDRESS_LENGTH];
    double running_time;
    long uplink_packet_num;
    long downlink_packet_num;
    long uplink_byte_num;
    long downlink_byte_num;
    char collect_timestamp[TIMESTAMP_LENGTH];
}afi_info;

typedef struct query_afi_info{
    int num_columns;
    int num_rows;
    int cur_row;
    afi_info* afiInfo;
}query_afi_info;

//ap_index_info
//AP指标信息
typedef struct ap_index_info{
    int signal_intensity;
    int down_up_BW;
    int time_delay;
    char collect_timestamp[TIMESTAMP_LENGTH];
}ap_info;

typedef struct query_ap_index_info{
    int num_columns;
    int num_rows;
    int cur_row;
    ap_info* apInfo;
}query_ap_info;

//sys_health_score
//系统健康评分信息
typedef struct sys_health_score{
    int health_score;
    int signal_interference;
    long downlink_byte_num;
    long uplink_byte_num;
    int cover_area;
    int signal_intensity;
    int user_access_rate;
    int online_user_num;
    char collect_timestamp[TIMESTAMP_LENGTH];
}sys_score;

typedef struct query_sys_score_info{
    int num_columns;
    int num_rows;
    int cur_row;
    sys_score* sysInfo;
}query_sys_info;
//throughout_index_info
//流量指标信息
typedef struct throughout_index_info{
    long downlink_packet_num;
    long uplink_packet_num;
    long downlink_byte_num;
    long uplink_byte_num;
    char collect_timestamp[TIMESTAMP_LENGTH];
}throughout_info;

typedef struct query_throughout_info{
    int num_columns;
    int num_rows;
    int cur_row;
    throughout_info* throughoutInfo;
}query_throughout_info;
//user_detailinfo_db
//用户详细信息
typedef struct user_detailinfo{
    char mac_address[MAC_ADDRESS_LENGTH];
    char mac_alias[MAC_ADDRESS_ALIAS_LENGTH];
    char hostname[AFI_HOSTNAME_LENGTH];
    char ip_address[IP_ADDRESS_LENGTH];
    char device_type[DEVICE_TYPE_LENGTH];
    char is_online[8];
    char access_point[ACCESS_POINT_LENGTH];
    double running_time;
    long downlink_packet_num;
    long uplink_packet_num;
    long downlink_byte_num;
    long uplink_byte_num;
    char collect_timestamp[TIMESTAMP_LENGTH];
}user_detailinfo;

typedef struct query_user_detailinfo{
    int num_columns;
    int num_rows;
    int cur_row;
    user_detailinfo* userInfo;
}query_user_detailinfo;
//user_index_info
//用户指标信息
typedef struct user_index_info{
    int signal_intensity;
    int consultation_rate;
    int access_rate;
    int WL_retransmission_rate;
    char collect_timestamp[TIMESTAMP_LENGTH];
}user_info;

typedef struct query_user_index_info{
    int num_columns;    
    int num_rows;
    int cur_row;
    user_info* userInfo;
}query_user_info;
//wifi_index_info
//无线指标信息
typedef struct wifi_index_info{
    int signal_noise_rate;
    int channel_utilization;
    int signal_interference;
    char collect_timestamp[TIMESTAMP_LENGTH];
}wifi_info;

typedef struct query_wifi_info{
    int num_columns;
    int num_rows;
    int cur_row;
    wifi_info* wifiInfo;
}query_wifi_info;
//wireless_index_info
//无线业务信息
typedef struct wireless_index_info{
    char essid[ESSID_LENGTH];
    char AFi_n[AFI_N_LENGTH];
    int online_user_count;
    long downlink_packet_num;
    long uplink_packet_num;
    long downlink_byte_num;
    long uplink_byte_num;
    char collect_timestamp[TIMESTAMP_LENGTH];
}wireless_info;

typedef struct query_wireless_info{
    int num_columns;
    int num_rows;
    int cur_row;
    wireless_info* wirelessInfo;
}query_wireless_info;


/*
 *格式化时间转换函数
 */
int get_db_format_time_str_from_tm(struct tm* timeinfo,char* format_time);
int get_db_tm_from_format_time(const char* format_time,struct tm* timeinfo);
//获取24小时之前的时间戳
char* get_time_within_24h(const char* timestamp,char* format_time);
//获取30天之前的时间戳
char* get_time_within_30d(const char* timestamp,char* format_time);
/*
 *数据库接口，包含了添加，删除，插入等操作
 */
int db_do_query_2(MYSQL* mysql,const char* query,int(*call_back)(void*,MYSQL_RES*,int,int),void* result);
/*
 *查询接口
 *
 */
//查询全部
int db_get_ap_index_info(query_ap_info* query_info);
int db_get_afi_info(query_afi_info* query_info);
int db_get_sys_score_info(query_sys_info* query_info);
int db_get_throughout_index_info(query_throughout_info* query_info);
int db_get_user_detail_info(query_user_detailinfo* query_info);
int db_get_user_index_info(query_user_info* query_info);
int db_get_wifi_index_info(query_wifi_info* query_info);
int db_get_wireless_index_info(query_wireless_info* query_info);

//TODO
//根据时间段查询
//获取24小时内的数据，默认从调用函数那一刻算起，倒推24小时
//int test(int a,int b = 0);
int db_get_ap_index_info_within_24h(const char* timestamp,query_ap_info* query_info);
int db_get_afi_info_within_24h(const char* timestamp,query_afi_info* query_info);
int db_get_sys_score_info_within_24h(const char* timestamp,query_sys_info* query_info);
int db_get_throughout_index_info_within_24h(const char* timestamp,query_throughout_info* query_info);
int db_get_user_detail_info_within_24h(const char* timestamp,query_user_detailinfo* query_info);
int db_get_user_index_info_within_24h(const char* timestamp,query_user_info* query_info);
int db_get_wifi_index_info_within_24h(const char* timestamp,query_wifi_info* query_info);
int db_get_wireless_index_info_within_24h(const char* timestamp,query_wireless_info* query_info);
//获取30天内的数据，默认从调用函数那一天算起，倒推30天
int db_get_ap_index_info_within_30d(const char* timestamp,query_ap_info* query_info);
int db_get_afi_info_within_30d(const char* timestamp,query_afi_info* query_info);
int db_get_sys_score_info_within_30d(const char* timestamp,query_sys_info* query_info);
int db_get_throughout_index_info_within_30d(const char* timestamp,query_throughout_info* query_info);
int db_get_user_detail_info_within_30d(const char* timestamp,query_user_detailinfo* query_info);
int db_get_user_index_info_within_30d(const char* timestamp,query_user_info* query_info);
int db_get_wifi_index_info_within_30d(const char* timestamp,query_wifi_info* query_info);
int db_get_wireless_index_info_within_30d(const char* timestamp,query_wireless_info* query_info);
//根据不同字段
int db_get_afi_info_by_mac(const char* mac,query_afi_info* query_info);
int db_get_afi_info_by_AFi_name(const char* AFi_name,query_afi_info* query_info);
int db_get_user_detail_info_by_mac(const char* mac,query_user_detailinfo* query_info);
int db_get_user_detail_info_by_mac_alias(const char* mac_alias,query_user_detailinfo* query_info);
int db_get_user_detail_info_by_access_point(const char* access_point,query_user_detailinfo* query_info);
/*
 *添加数据库接口
 */
int db_add_ap_index_info(ap_info* ap);
int db_add_afi_index_info(afi_info* afi);
int db_add_sys_score_info(sys_score* sys);
int db_add_throughout_index_info(throughout_info* throughout);
int db_add_user_detail_info(user_detailinfo* userinfo);
int db_add_user_index_info(user_info* userinfo);
int db_add_wifi_index_info(wifi_info* wifi);
int db_add_wireless_index_info(wireless_info* wireless);

/*
 *删除数据库记录接口
 */
int db_delete_ap_index_info_before_time(const char* timestamp);
int db_delete_ap_index_info_after_time(const char* timestamp);
int db_delete_afi_info_before_time(const char* timestamp);
int db_delete_afi_info_after_time(const char* timestamp);
int db_delete_sys_score_info_before_time(const char* timestamp);
int db_delete_sys_score_info_after_time(const char* timestamp);
int db_delete_throughout_index_info_before_time(const char* timestamp);
int db_delete_throughout_index_info_after_time(const char* timestamp);
int db_delete_user_detail_info_before_time(const char* timestamp);
int db_delete_user_detail_info_after_time(const char* timestamp);
int db_delete_user_index_info_before_time(const char* timestamp);
int db_delete_user_index_info_after_time(const char* timestamp);
int db_delete_wifi_index_info_before_time(const char* timestamp);
int db_delete_wifi_index_info_after_time(const char* timestamp);
int db_delete_wireless_index_info_before_time(const char* timestamp);
int db_delete_wireless_index_info_after_time(const char* timestamp);
/*
 *数据库更新操作接口
 */
int db_update_afi_info();
int db_update_user_detail_info();
#endif
