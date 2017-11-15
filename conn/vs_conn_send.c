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
			//LOG_ERROR("socket_send_tcp error:%d", errno);
			return VS_CONN_SEND_ERROR;
		}
		send_len += flag;
	}
	return send_len;
}
