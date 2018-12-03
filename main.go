// main
package main

import (
	. "fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//加载账户名和密码
/*
	u:123456	p:123456
	u:zhaomy	p:391728
	u:888888	p:666666
*/
var m = make(map[string]string)
var login = make(map[string]int)

func handleConnetion(conn *net.TCPConn, logger *log.Logger) {
	username := ""
	buf := make([]byte, 128)
	n, err := conn.Read(buf)
	if err != nil {
		return
	}
	str := string(buf[:n-1])
	s := strings.Split(str, "@")
	logger.Println("user login : ", s[0])
	if s[1] == m[s[0]] {
		conn.Write([]byte("yes"))
		if _, ok := login[s[0]]; !ok {
			login[s[0]] = 1
		} else {
			login[s[0]] = login[s[0]] + 1
		}

	} else {
		conn.Write([]byte("no"))
		return
	}
	for {
		n, err := conn.Read(buf)
		if err != nil {
			logger.Println(s[0], ":user Disconnect")
			login[s[0]] = login[s[0]] - 1
			if login[s[0]] < 1 {
				delete(login, s[0])
			}
			return
		}
		logger.Println(username, ":", string(buf[:n]))
	}
}
func query() {
	Println("************************")
	for k, v := range login {
		Println("用户：", k, "   ", "数量：", v)
	}
	Println("************************")
}

//终端控制线程
func oper() {

	opt := 0
	for {
		Println("——————————————————————————————")
		Println("1、Query current user:")
		Println("2、没想好:")
		Println("3、Quit process:")
		Println("——————————————————————————————")
		Println("Please chooice:")
		Scan(&opt)
		switch opt {
		case 1:
			//查询
			query()
		case 2:
			//待更新
		case 3:
			//退出
			os.Exit(0)
		default:
			Printf("请输入1~3数字")
		}
		Println("继续...")
	}
}

func main() {
	func() {
		m["123456"] = "123456"
		m["zhaomy"] = "391728"
		m["888888"] = "666666"
	}()
	tcpAddr, err := net.ResolveTCPAddr("tcp", "127.0.0.1:8080")
	if err != nil {
		log.Panic(err)
	}
	tcpLister, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		log.Panic(err)
	}
	logfile, err := os.OpenFile("C:/minBox/serverlog/test.log", os.O_RDWR|os.O_CREATE, 0666)
	logger := log.New(logfile, "\r\n", log.Ldate|log.Ltime|log.Llongfile)
	logger.Println("server start running...")
	Println("server start running...")
	if err != nil {
		logger.Panic(err)
	}
	go func() {
		for {
			time.Sleep(30 * time.Second)
			logger.Println("Current user connection:", login)
		}
	}()
	go oper()
	for {
		Conn, err := tcpLister.AcceptTCP()
		if err != nil {
			logger.Panic(err)
		}
		logger.Println("new connection", Conn.RemoteAddr().String())
		go handleConnetion(Conn, logger)
	}

}

