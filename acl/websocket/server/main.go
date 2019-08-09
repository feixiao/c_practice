package main

import (
	"github.com/gorilla/websocket"
	"github.com/gin-gonic/gin"
	"github.com/satori/go.uuid"
	"os"
	"os/signal"
	"net/http"
	"syscall"
	"log"
	"time"
)

// ClientManager is a websocket manager
type ClientManager struct {
	Clients    map[*Client]bool
	Broadcast  chan []byte
	Register   chan *Client
	Unregister chan *Client
}


// Client is a websocket client
type Client struct {
	ID     string
	Socket *websocket.Conn
	Send   chan []byte
}

// Manager define a ws server manager
var Manager = ClientManager{
	Broadcast:  make(chan []byte),
	Register:   make(chan *Client),
	Unregister: make(chan *Client),
	Clients:    make(map[*Client]bool),
}

// Start is to start a ws server
func (manager *ClientManager) Start() {
	for {
			select {
			case conn := <-manager.Register:
					log.Println("client enter")
					manager.Clients[conn] = true
				
			case conn := <-manager.Unregister:
					log.Println("client out")
					if _, ok := manager.Clients[conn]; ok {
							close(conn.Send)
							delete(manager.Clients, conn)
					}
			}
	}
}


func (c *Client) Read() {
	defer func() {
			Manager.Unregister <- c
			c.Socket.Close()
	}()

	for {
			msgType, message, err := c.Socket.ReadMessage()
			if err != nil {
					log.Println(err)
					Manager.Unregister <- c
					c.Socket.Close()
					break
			}
			log.Println(string(message))
			c.Socket.WriteMessage(msgType, message)
	}
}


func main() {


	go Manager.Start()
	g := gin.Default()

	g.NoRoute(func(c *gin.Context) {
		c.String(http.StatusNotFound, "The incorrect API route.")
	})

	// The user handlers, requiring authentication
	g.GET("/echo", handleEcho)
	g.GET("/", handleRedirect)

	go func() {
		addr := ":8300"
		log.Printf("RoomServer service on %s ", addr)
		http.ListenAndServe(addr, g)
	}()


	c := make(chan os.Signal, 1)
	signal.Notify(c, syscall.SIGHUP, syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGINT)
	for {
		s := <-c
		log.Printf("tcpserver get a signal %s", s.String())
		switch s {
		case syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGINT:
			time.Sleep(time.Second)
			log.Printf("tcpserver exit")
			return
		case syscall.SIGHUP:
		default:
			return
		}
	}
}

func handleEcho(c *gin.Context) {
	conn, err := (&websocket.Upgrader{CheckOrigin: func(r *http.Request) bool { return true }}).Upgrade(c.Writer, c.Request, nil)
	if err != nil {
		http.NotFound(c.Writer, c.Request)
		return
	}
	// websocket connect
	client := &Client{ID: uuid.NewV4().String(), Socket: conn, Send: make(chan []byte)}
	log.Println("header : ", c.Request.Header)
	Manager.Register <- client

	go client.Read()
}


func handleRedirect(c *gin.Context) { 
		http.Redirect(c.Writer, c.Request, "http://localhost:8300/echo", http.StatusMovedPermanently)
}