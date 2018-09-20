#!/bin/sh

# Michael Adams @ https://github.com/unquietwiki ; Sep 20, 2018
# Attempt to get Let's Encrypt certs loaded onto an OpenWRT router. Should run this every 30-60 days.
# Copy to /etc/uhttpd-acme-lecert.sh & chmod a+x the resulting file. Edit the ACME & uhttpd configs, as needed.

# Reference 1: https://openwrt.org/zh-cn/doc/uci/uhttpd?s[]=uhttpd
# Reference 2: https://stokito.wordpress.com/2017/10/16/how-to-letsencrypt-https-on-openwrt-with-uhttpd/

# opkg install libuhttpd-openssl acme luci-app-acme

DOMAIN=example.com

echo Adjusting firewall rules to make sure HTTP and HTTPS are allowed externally
iptables -A INPUT -p tcp --dport 80 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp --sport 80 -m conntrack --ctstate ESTABLISHED -j ACCEPT
iptables -A INPUT -p tcp --dport 443 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp --sport 443 -m conntrack --ctstate ESTABLISHED -j ACCEPT

echo Purging old ACME cert from uhttpd...
killall -9 uhttpd
rm /etc/ssl/private/uhttpd.key
rm /etc/ssl/certs/uhttpd.crt
/etc/init.d/acme restart

echo Waiting on new ACME cert...
sleep 60

echo Restoring original firewall...
/etc/init.d/firewall restart

echo Loading new ACME cert...
openssl rsa -in /etc/acme/$DOMAIN/$DOMAIN.key -outform DER -out /etc/ssl/private/uhttpd.key
openssl x509 -in /etc/acme/$DOMAIN/fullchain.cer -outform DER -out /etc/ssl/certs/uhttpd.crt
/etc/init.d/uhttpd restart

echo Done!
