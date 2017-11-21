#include "vs_conn_send.h"





int vs_conn_send(int fd, void *buf, int size)
{
	int flag = 0;
	int send_len = 0;

	if (size <= 0) {
		return 0;
	}

	while (size > send_len) {
		flag = send(fd, buf + send_len, size - send_len, 0);
		if (flag < 0) {
			switch (errno) {
			case EINTR:
				continue;
			case EAGAIN:
				return send_len;
			}
			vs_log_sys_error("socket_send_tcp error:%d,%s", errno,strerror(errno));
			return VS_CONN_SEND_ERROR;
		}
		send_len += flag;
	}
	return send_len;
}

int vs_conn_udp_send_simple(int fd, void *buf, int size, const char *ip, const int port)
{
	struct sockaddr_in  addr;
	int					flag;
	int					nlen;
	int					nsend;


	nlen = size;
	nsend = 0;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	while (nlen > 0)
	{	
		nsend = sendto(fd, buf, nlen, 0, (struct sockaddr*)&addr, sizeof(addr));
		if (nsend <= 0)
		{
			if (errno == EINTR) {//���ж���
				nsend = 0;//������Ը�Ϊcontinue
			}
			else {
				vs_log_sys_error("vs_conn_udp_send_simple error:%d,%s", errno, strerror(errno));
				break;
			}
		}
		nlen -= nsend;
		buf += nsend;
	}
	return VS_OK;
}