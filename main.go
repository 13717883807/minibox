// main
package main

import (
	. "fmt"
	"log"
	"net"
	"strings"
)

func handleConnetion(conn *net.TCPConn) {
	//加载账户名和密码
	/*
		u:123456	p:123456
		u:zhaomy	p:391728
		u:888888	p:666666
	*/
	m := make(map[string]string)
	m["123456"] = "123456"
	m["zhaomy"] = "391728"
	m["888888"] = "666666"
	username := ""
	buf := make([]byte, 128)
	for i := 0; i < 3; i++ {
		n, err := conn.Read(buf)
		if err != nil {
			return
		}
		str := string(buf[:n-1])
		s := strings.Split(str, "@")
		Println(s[0], s[1])
		if s[1] == m[s[0]] {
			conn.Write([]byte("yes"))
			username = s[0]
			break
		} else {
			conn.Write([]byte("no"))
		}
	}
	for {
		n, err := conn.Read(buf)
		if err != nil {
			return
		}
		Println(username, ":", string(buf[:n]))
	}
}

func main() {
	tcpAddr, _ := net.ResolveTCPAddr("tcp", "127.0.0.1:8080")
	tcpLister, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		log.Panic(err)
	}
	defer tcpLister.Close()
	for {
		Conn, err := tcpLister.AcceptTCP()
		if err != nil {
			log.Panic(err)
		}
		Println(Conn.RemoteAddr().String())
		go handleConnetion(Conn)
	}
}
