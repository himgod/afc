#!/bin/bash
#
# This file intends for make wireless software package
#

export MAKE=$(shell echo "make")

export COMPILER_PREFIX=

export AR=${COMPILER_PREFIX}ar
export RANLIB=${COMPILER_PREFIX}ranlib
export OBJDUMP=${COMPILER_PREFIX}objdump
export STRIP=${COMPILER_PREFIX}strip
export SIZE=${COMPILER_PREFIX}size
export CC=${COMPILER_PREFIX}gcc
export CXX=${COMPILER_PREFIX}g++
export LD=${COMPILER_PREFIX}ld
export NM=${COMPILER_PREFIX}nm

#export ADDITION_LIB_PATH=/opt/x86libs

#export DBUS_INCLUDE=-I${ADDITION_LIB_PATH}/usr/include/dbus-1.0 -I${ADDITION_LIB_PATH}/usr/lib/dbus-1.0/include
#export DBUS_LIBS=-L${ADDITION_LIB_PATH}/usr/lib -ldbus-1
#export XML2_INCLUDE=-I${ADDITION_LIB_PATH}/usr/include/libxml2
#export XML2_LIBS=-L${ADDITION_LIB_PATH}/usr/lib -lxml2
export SYS_LIB_PATH=/usr/lib/i386-linux-gnu
export SYS_INCLUDE_PATH=/usr/include
export DBUS_INCLUDE=-I${SYS_INCLUDE_PATH}/dbus-1.0 -I${SYS_LIB_PATH}/dbus-1.0/include
export DBUS_LIBS=-L${SYS_LIB_PATH} -ldbus-1
export XML2_INCLUDE=-I${SYS_INCLUDE_PATH}/libxml2
export XML2_LIBS=-L${SYS_LIB_PATH} -lxml2

export SHLIB_DIR=/opt/lib/
export BIN_DIR=/opt/bin
export TOPSRC_DIR=${shell pwd}/src
export ACCAPI_DIR=${TOPSRC_DIR}/accapi
export HANDLIB_MOD=${TOPSRC_DIR}/handlib
export WCPSS_MOD=${TOPSRC_DIR}/wcpss
export WAILIB_MOD=${TOPSRC_DIR}/wcpss/wai
export EXTLIB_MOD=${TOPSRC_DIR}/php/ext
export DBLIB_MOD=${TOPSRC_DIR}/dblib
export WMDLIB_MOD=${TOPSRC_DIR}/wmd


export YBUILDER_DESCRIBE="ybuilder"
export YBUILDER_DIR=${TOPSRC_DIR}/makeself/ybuilder
export YBUILDER_LIB_DIR=${YBUILDER_DIR}/opt/lib
export YBUILDER_BIN_DIR=${YBUILDER_DIR}/opt/bin
export YBUILDER_OPT_DIR=${YBUILDER_DIR}/opt
export YBUILDER_OPT_RUN_DIR=${YBUILDER_DIR}/opt/run
export YBUILDER_WEB_DIR=${YBUILDER_DIR}/var/www/afc
export YBUILDER_USR_LIB_DIR=${YBUILDER_DIR}/usr/lib
export YBUILDER_USR_BIN_DIR=${YBUILDER_DIR}/usr/bin
export YBUILDER_ETC_DIR=${YBUILDER_DIR}/etc
export YBUILDER_VER_DIR=${YBUILDER_ETC_DIR}/version
export YBUILDER_VAR_DIR=${YBUILDER_DIR}/var
export BUILDNO="`cat buildno_v1.0`"
export VERSION="`cat ${YBUILDER_VER_DIR}/version`"
export YBUILDER_NAME="ybuilder${VERSION}.${BUILDNO}.bin"

PREREQ_WCPSS=$(shell if [ -d ${WCPSS_MOD} ] ; then echo "wcpss" ; fi)
CLEAN_WCPSS=$(subst wcpss,cleanwcpss,${PREREQ_WCPSS})

.PHONY: all clean

all: wid handlib dblib ext

release: wid handlib dblib extrelease

wid: wcpsspub_ac dblib
	@echo "Building wid module ..."
	$(MAKE) -C ${WCPSS_MOD}/wid

wailib:
	@echo "Building wai library ..."
	$(MAKE) -C ${WAILIB_MOD}

wcpsspub_ac: wailib
	@echo "Building wcpss public lib ..."
	$(MAKE) -C ${WCPSS_MOD}/pub

handlib:
	@echo "Building handle public library ..."
	$(MAKE) -C ${HANDLIB_MOD}

dblib:
	@echo "Building up db library ..."
	$(MAKE) -C ${DBLIB_MOD}
wmdlib:
	@echo "Building up wmd library ..."
	$(MAKE) -C ${WMDLIB_MOD}

ext: handlib dblib wailib
	@echo "Building  php ext ..."
	@if [ -e ${TOPSRC_DIR}/php/include/php ] ;  \
	then \
		if [ -L ${TOPSRC_DIR}/php/include/php ] ; \
		then \
			rm -rf ${TOPSRC_DIR}/php/include/php ; \
		fi ;\
	fi; \
	if [ -d /usr/include/php5 ] ; \
	then \
		echo ${TOPSRC_DIR}/php/include ; \
		cd ${TOPSRC_DIR}/php/include ; \
		ln -s  /usr/include/php5  php; \
		cd -; \
	fi; 
	$(MAKE) -C ${EXTLIB_MOD} clean
	$(MAKE) -C ${EXTLIB_MOD}

extrelease: handlib dblib wailib
	@echo "Building  php ext release ..."
	@if [ -e ${TOPSRC_DIR}/php/include/php ] ;  \
	then \
		if [ -L ${TOPSRC_DIR}/php/include/php ] ; \
		then \
			rm -rf ${TOPSRC_DIR}/php/include/php ; \
		fi ;\
	fi ; \
	if [ -d ${TOPSRC_DIR}/php/include/php20121212 ] ; \
	then \
		echo ${TOPSRC_DIR}/php/include ; \
		cd ${TOPSRC_DIR}/php/include ; \
		ln -s  php20121212  php; \
		cd -; \
	fi; 
	$(MAKE) -C ${EXTLIB_MOD} clean
	$(MAKE) -C ${EXTLIB_MOD}

clean: cleanhandlib cleanwid cleanext cleandblib cleanwmdlib

cleanwid: cleanwcpsspub_ac cleanwailib
	@echo "Cleaning wid module ..."
	$(MAKE) -C ${WCPSS_MOD}/wid clean

cleanwcpsspub_ac:
	@echo "Cleaning wcpss public lib ..."
	$(MAKE) -C ${WCPSS_MOD}/pub clean

cleanwailib:
	@echo "Cleaning wai library ..."
	$(MAKE) -C ${WAILIB_MOD} clean

cleanhandlib:
	@echo "Cleaning hand  public library ..."
	$(MAKE) -C ${HANDLIB_MOD} clean

cleandblib:
	@echo "Cleaning up db library ..."
	$(MAKE) -C ${DBLIB_MOD} clean

cleanext: cleanwailib
	@echo "Cleaning php ext  ..."
	$(MAKE) -C ${EXTLIB_MOD} clean
cleanwmdlib:
	@echo "Cleaning wmd library ..."
	$(MAKE) -C ${WMDLIB_MOD} clean

genkey:
	@echo "making genkey..."
	@if [ -e ~/.ssh/id_dsa.pub ] ; \
		then \
			rm -rf ~/.ssh/id_dsa.pub ; \
			rm -rf ~/.ssh/id_dsa ; \
			echo "delete old id_dsa.pub" ; \
		fi ; \
		./genkey.sh 1>./genkey.out
	@echo "making genkey...done"

pubkey: cleankey
	@echo "make pubkey..."
	@if ! [ -d /opt/run/res ] ; \
		then \
			mkdir -p /opt/run/res ; \
		fi ;
		#sudo chmod a+rwx /opt/run/res ; 
		if ! [ -e ~/.ssh/id_dsa.pub ] ; \
		then \
		make genkey;	\
		fi ;\
		cat ~/.ssh/id_dsa.pub > /opt/run/res/id_dsa.pub 
	@echo "make pubkey...done"

cleankey:
	@echo "clean pub key..."
	@if [ -f ~/.ssh/id_dsa ] ; \
		then \
			rm -rf ~/.ssh/id_dsa ; \
		fi ;\
		if [ -f ~/.ssh/id_dsa.pub ] ; \
		then \
			rm -rf ~/.ssh/id_dsa.pub ; \
		fi
	@echo "clean pub key...done"

install:
	@echo -n "Install wai.so.\c"
	@cp ${WAILIB_MOD}/libwai.so ${SHLIB_DIR}
	@echo -n "...done\n"
	@echo -n "Install ext.so.\c"
	@cp ${EXTLIB_MOD}/modules/ext.so ${SHLIB_DIR}
	@echo -n "...done\n"
	@echo -n "Install libdbop.so.\c"
	@cp ${DBLIB_MOD}/libdbop.so ${SHLIB_DIR}
	@echo -n "...done\n"
	@echo -n "Install libhandw.so.\c"
	@cp ${HANDLIB_MOD}/libhandw.so ${SHLIB_DIR}
	@echo -n "...done\n"
	@echo -n "Install wireless daemon.\c"
	@if [ -f ${BIN_DIR}/wid ] ; \
		then \
			rm -rf ${BIN_DIR}/wid ; \
		fi
	@cp ${WCPSS_MOD}/wid/wid ${BIN_DIR}/
	@cp ${WCPSS_MOD}/res/startwid.sh ${BIN_DIR}/
	@cp ${TOPSRC_DIR}/web/load_wireless.php /opt/run/wcpss/
	@#sudo chmod a+x ${BIN_DIR}/startwid.sh ${BIN_DIR}/wid
	@${BIN_DIR}/startwid.sh	
	@echo -n "Install wireless daemon...done\n"
	@echo -n "Install php scripts.\n"
	@#rm -rf /var/www/afc/*
	@if ! [ -L ${TOPSRC_DIR}/web/img ] ; \
		then \
			rm -rf ${TOPSRC_DIR}/web/img ; \
			ln -sf /opt/img ${TOPSRC_DIR}/web/img; \
		fi
	@if ! [ -L ${TOPSRC_DIR}/web/res ] ; \
		then  \
			rm -rf ${TOPSRC_DIR}/web/res ; \
			ln -sf /opt/run/res ${TOPSRC_DIR}/web/res; \
		fi
	@sudo cp -r ${TOPSRC_DIR}/web/* /var/www/afc/
	@#sudo cp /var/www/afc/afcconf /opt/run/ -r
	@if [ -e /var/www/afc/map -o -L /var/www/afc/map ] ; \
		then \
			rm -rf /var/www/afc/map ; \
		fi
	@ln -s /opt/run/afcconf/map /var/www/afc/
	@#sudo chown -R www-data:www-data /var/www/afc/session
	@echo -n "...done\n"
	@echo -n "Reload web server.\c"
	@sudo /etc/init.d/apache2 restart
	@echo -n "...done\n"
ybuilder: release
	mkdir -p ${YBUILDER_LIB_DIR}
	mkdir -p ${YBUILDER_BIN_DIR}
	mkdir -p ${YBUILDER_WEB_DIR}
	cp ${WAILIB_MOD}/libwai.so ${YBUILDER_LIB_DIR}
	cp ${EXTLIB_MOD}/modules/ext.so ${YBUILDER_LIB_DIR}
	cp ${DBLIB_MOD}/libdbop.so ${YBUILDER_LIB_DIR}
	cp /opt/lib/libiconv.so ${YBUILDER_LIB_DIR}/
	cp ${HANDLIB_MOD}/libhandw.so ${YBUILDER_LIB_DIR}
	cp ${WCPSS_MOD}/wid/wid ${YBUILDER_BIN_DIR}/
	cp ${WCPSS_MOD}/res/startwid.sh ${YBUILDER_BIN_DIR}/
	cp ${TOPSRC_DIR}/dblib/softacDB.sql ${YBUILDER_OPT_DIR}/run/db/
	-cp -r /opt/run/res/* ${YBUILDER_OPT_DIR}/run/res/
	-rm -rf ${YBUILDER_OPT_DIR}/img/*
	-cp -r /opt/img/upgrade.info ${YBUILDER_OPT_DIR}/img/
	-cp -r /opt/img/`head -n 1 /opt/img/upgrade.info`.tar.gz ${YBUILDER_OPT_DIR}/img/
	chmod a+x ${YBUILDER_BIN_DIR}/startwid.sh ${YBUILDER_BIN_DIR}/wid
	@if ! [ -L ${TOPSRC_DIR}/web/img ] ; \
		then \
			rm -rf ${TOPSRC_DIR}/web/img ; \
			ln -sf /opt/img ${TOPSRC_DIR}/web/img; \
		fi
	@if ! [ -L ${TOPSRC_DIR}/web/res ] ; \
		then  \
			rm -rf ${TOPSRC_DIR}/web/res ; \
			ln -sf /opt/run/res ${TOPSRC_DIR}/web/res; \
		fi
	rm -rf ${YBUILDER_WEB_DIR}/*
	cp -rf ${TOPSRC_DIR}/web/* ${YBUILDER_WEB_DIR}/
	@rm -rf ${YBUILDER_OPT_DIR}/run/afcconf/ 
	@if [ -e ${YBUILDER_WEB_DIR}/map -o -L ${YBUILDER_WEB_DIR}/map ] ; \
		then \
			rm -rf ${YBUILDER_WEB_DIR}/map ; \
		fi
	@ln -s /opt/run/afcconf/map ${YBUILDER_WEB_DIR}/
	echo ${BUILDNO} > ${YBUILDER_VER_DIR}/buildno
	echo "`whoami`@`hostname`:`tty`" > ${YBUILDER_VER_DIR}/builder
	echo "`date`" > ${YBUILDER_VER_DIR}/buildtime
	echo "${YBUILDER_NAME}" > ${YBUILDER_VER_DIR}/versionstring
	${TOPSRC_DIR}/makeself/makeself.sh ${YBUILDER_DIR} $(YBUILDER_NAME) ${YBUILDER_DESCRIBE} ./install.sh
	
synweb:
	@sudo cp -r ${TOPSRC_DIR}/web/* /var/www/afc/
	@#sudo cp /var/www/afc/afcconf /opt/run/ -r
	@if [ -e /var/www/afc/map -o -L /var/www/afc/map ] ; \
		then \
			rm -rf /var/www/afc/map ; \
		fi
	@ln -s /opt/run/afcconf/map /var/www/afc/
	
.PHONY: genkey pubkey cleankey synweb
