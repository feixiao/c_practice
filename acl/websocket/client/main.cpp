#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include<chrono>
#include<thread>
#include<stdio.h>
#include<signal.h>

bool gStop = false;
void handler(int sig)
{
 	printf("get a sig,num is %d\n",sig);
 	gStop = true;
}

static void myThread(acl::websocket* ws);

static bool handshake(acl::socket_stream& conn)
{
	acl::http_request req(&conn);
	acl::http_header& hdr = req.request_header();
	hdr.set_ws_key("123456789")
		.set_ws_version(13)
		.set_upgrade("websocket")
		.set_url("/echo",false)
		.set_keep_alive(true);

	if (req.request(NULL, 0) == false)
	{
		printf("request error\r\n");
		return false;
	}

	int status = req.http_status();
	if (status != 101)
	{
		printf("invalid http status: %d\r\n", status);
		return false;
	}

	return true;
}

static bool send(acl::websocket& ws)
{
	acl::ifstream in;
	acl::string buf;

	auto tt = std::chrono::system_clock::to_time_t
	(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&tt);
	char date[60] = {0};
	sprintf(date, "%d-%02d-%02d      %02d:%02d:%02d",
		(int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
		(int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec);

	buf.copy(date, strlen(date));
	ws.reset()
		.set_frame_fin(true)
		.set_frame_opcode(acl::FRAME_BINARY)
		.set_frame_payload_len(buf.size())
		.set_frame_masking_key(12345678);

	if (ws.send_frame_data(buf, buf.size()) == false)
	{
		printf("send filenam error %s\r\n", acl::last_serror());
		return false;
	}
	return true;
}

static bool read_reply(acl::websocket& ws)
{
	if (ws.read_frame_head() == false)
	{
		printf("read_frame_head error %s\r\n", acl::last_serror());
		return false;
	}

	char cbuf[1024];
	unsigned char opcode = ws.get_frame_opcode();
	switch (opcode)
	{
	case acl::FRAME_TEXT:
	case acl::FRAME_BINARY:
		break;
	default:
		printf("invalid opcode: 0x%x\r\n", opcode);
		return false;
	}

	int ret = ws.read_frame_data(cbuf, sizeof(cbuf) - 1);
	if (ret <= 0)
	{
		printf("read_frame_data error\r\n");
		return false;
	}
	
	cbuf[ret] = 0;
	printf("reply from server: %s, len: %d\r\n", cbuf, ret);

	return true;
}

static void usage(const char* proc)
{
	printf("usage: %s -h [help] -s server_addr -m mode\r\n", proc);
}

int main(int argc, char* argv[])
{
	acl::acl_cpp_init();

	
	acl::log::stdout_open(true);  
	acl::log::open("./test.log");
	int ch;

	// std::string test = "hello";
	// printf("hello : %lu bytes", test.size());

	acl::string addr, mode;

	while ((ch = getopt(argc, argv, "hf:s:m")) > 0)
	{
		switch (ch)
		{
		case 'h':
			usage(argv[0]);
			return 0;
		case 's':
			addr = optarg;
			break;
		case 'm':
			mode = optarg;	
		default:
			break;
		}
	}

	if (addr.empty())
	{
		usage(argv[0]);
		return 1;
	}

  signal(SIGINT, handler); 

	acl::socket_stream conn;
	if (conn.open(addr, 30, 30) == false)
	{
		logger("connect %s error %s\r\n", addr.c_str(), acl::last_serror());
		return false;
	}

	if (handshake(conn) == false)
		return false;

	acl::websocket ws(conn);

	std::thread t1(myThread,&ws);

	// read and write
	if(strcmp(mode,"s")) {
		logger("one threads....\n");
	}else {
		logger("two threads....\n");
	}
	
	while (!gStop) {
		if(strcmp(mode,"s")) {
			//send(ws);
			read_reply(ws);
		}else {
			read_reply(ws);
		}

		t1.join();
	
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
	conn.close();
	return 0;
}


void myThread(acl::websocket* ws)
{
		logger("myThread run ...");
    while(!gStop) {
			send(*ws);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
}
