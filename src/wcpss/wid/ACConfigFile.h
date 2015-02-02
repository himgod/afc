#ifndef ACCONFIGFILE_H
#define ACCONFIGFILE_H

/* xml_type: 1:operator, 2:enterprise*/
typedef enum
{
	xml_type_error = 0, /* error      */
	xml_type_op    = 1, /* operator   */
	xml_type_en    = 2, /* enterprise */
}xml_type;

#define WID_SYSTEM_AUTELAN_SNMP_OID        "31656"

#define WID_SYSTEM_ENTERPRISE_SNMP_OID_PATH "/devinfo/enterprise_snmp_oid"
#define WID_SYSTEM_OEM_FLAG_PATH            "/devinfo/oem_flag"
#define WID_ENTERPRISE_SNMP_OID_LEN			(10)
#define WID_OEM_FLAG_LEN					(2)


CWBool CWParseApimgXML(CWConfigVersionInfo_new ** VersionInfo, char *model_name);


#endif

