#!/bin/bash
# @thelinuxchoice
checkroot() {
if [[ "$(id -u)" -ne 0 ]]; then
    printf "Run this program as root!\n"
    exit 1
fi
}
start() {

checkroot
declare -a dependencies=("/sbin/ipset");
for package in "${dependencies[@]}"; do
   if ! hash "$package" 2> /dev/null; then
     printf "'$package' isn't installed. apt-get install -y '$package'\n";
     exit 1
   fi
done
YOUR_IP=$(curl -s ifconfig.me)
printf "Configuring ipset..."
/sbin/ipset -q -N tor iphash
wget -q https://check.torproject.org/cgi-bin/TorBulkExitList.py?ip=$YOUR_IP -O - | /bin/sed '/^#/d' | while read IP
do
/sbin/ipset -q -A tor $IP
done
if [ ! -d "/etc/iptables" ]; then
/bin/mkdir "/etc/iptables"
/sbin/ipset -q save -f /etc/iptables/ipset.rules
printf "Done (saved: /etc/iptables/ipset.rules\n"
fi
/sbin/ipset -q save -f /etc/iptables/ipset.rules
printf "Done (saved: /etc/iptables/ipset.rules\n"
printf "Configuring iptables..."
checkiptables=$(/sbin/iptables --list | /bin/grep -o "tor src")
if [[ $checkiptables == "" ]]; then
/sbin/iptables -A INPUT -m set --match-set tor src -j DROP;
fi
if [ ! -e "/etc/iptables/rules.v4" ]; then
/usr/bin/touch "/etc/iptables/rules.v4"
/sbin/iptables-save > /etc/iptables/rules.v4
printf "Done (saved: /etc/iptables/rules.v4)\n"
else
/sbin/iptables-save > /etc/iptables/rules.v4
printf "Done (saved: /etc/iptables/rules.v4)\n"
fi
}
stop() {
checkroot
/sbin/iptables -D INPUT -m set --match-set tor src -j DROP
/sbin/ipset destroy tor
printf "Blocktor stopped,rules removed\n"
}

case "$1" in --start) start ;; --stop) stop ;; *)
printf "Usage: sudo ./blocktor.sh --start / --stop\n"
exit 1
esac
