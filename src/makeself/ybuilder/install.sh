#!/bin/sh

echo "Install /etc ..."
cp -ra etc/* /etc/

echo "Install /lib ..."
cp -ra lib/* /lib/

echo "Install /opt ..."
pkill wid
rm -rf /opt/img/v*
cp -ra opt/* /opt/
tar -zxf /opt/img/v*.tar.gz -C /opt/img 2>/dev/null
rm -f /opt/img/v*.tar.gz
echo "AFi image files info:"
ls /opt/img/
extension_dir=`php ./get_extension_dir.php 2>/dev/null`;
if [ -e $extension_dir/ext.so ] ; then
if [ ! -L $extension_dir/ext.so ] ; then
rm -f $extension_dir/ext.so
ln -s /opt/lib/ext.so $extension_dir/ext.so
fi
else
ln -s /opt/lib/ext.so $extension_dir/ext.so
fi

echo "Install /usr ..."
cp -ra usr/* /usr/

echo "Install /var ..."
rm -rf /var/www/afc/library/*
cp -ra var/* /var/
mkdir -p /opt/run/wcpss
chmod 777 /opt/run/wcpss
cp /var/www/afc/load_wireless.php /opt/run/wcpss/
if [ ! -e /opt/run/afcconf ] ; then
echo "Create default config file ..."
cp -rf /var/www/afc/afcconf /opt/run/
else
	echo "Config file exists "
	if [ ! -e /opt/run/afcconf/map ] ; then
		echo "Create map directory ..."
		cp -rf /var/www/afc/afcconf/map /opt/run/afcconf/
	fi
fi
chmod 777 /opt/run/afcconf -R
chmod 777 /opt/run/afcconf/xml_php.xml
mkdir -p /var/www/afc/session
chown -R www-data:www-data /var/www/afc/session

echo "start ldconfig ..."
ldconfig

echo "start depmod ..."
depmod

#echo "Init database ..."
#mysql -uroot -pautelan < ./softacDB.sql

echo "Start wireless daemon..."
/opt/bin/startwid.sh
#/opt/bin/wid 0 1 &
echo "...done"
#rm -f /tmp/new_version_*
echo "Restart apache service..."
/etc/init.d/apache2 restart
echo "...done"
echo 1 > /opt/run/upgraded
chmod 666 /opt/run/upgraded
rm -rf /tmp/*
echo "All done."
