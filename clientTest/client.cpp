#include "client.h"
#include <time.h>
#include <unistd.h>


#define MAXBUF 1500
char ip[32] = {0};
int port;
int nums = 0;


int main(int argc, char const *argv[])
{

	char buf[MAXBUF];
	int sockLink = -1;

	if (argc < 4){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);
	int pthreadCount = atoi(argv[3]);
	nums = atoi(argv[4]);

	srand(time(0));

	pthread_attr_t attr;
	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init\n");
		return 0;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate\n");
		return 0;
	}

	for ( int i= 0; i < pthreadCount ; ++i){
		pthread_t threadId;
		int preturn;
		preturn = pthread_create(&threadId,&attr,deal,NULL);
		if (preturn != 0){
			perror("pthread_create\n");
			continue;
		}
	}
	read(STDOUT_FILENO,buf,MAXBUF);

	printf("exit\n");
	return 0;
}



void * deal(void *param)
{
	char buf[MAXBUF];
	int bufLen=0;
	int type=0;
	int i;

	int sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	int res = 0;
	int tempSendLen = 0;
	int sendLen = 0;
	for(i = 0 ; i < nums; i++){
		res = getBuf(buf,&bufLen,&type);
		tempSendLen = 0;
		for(;;){
			sendLen = send(sockLink,buf + tempSendLen ,bufLen - tempSendLen,MSG_NOSIGNAL);
			if(sendLen > 0){
				tempSendLen += sendLen;
				if(tempSendLen == bufLen){
					break;
				}
			}else if (sendLen == 0){		
				perror("client is close\n");
				return 0;
			}else{
				if( errno == EAGAIN || errno == EWOULDBLOCK){
					continue;
				}else{
					close(sockLink);
					perror("client have error\n");
					return 0;
				}
			}
		}
	}
	printf("closeLink\n");
	close(sockLink);
}

int connectServer(char *ip,int port)
{
	int clientSoct = socket(AF_INET,SOCK_STREAM,0);
	if (clientSoct == -1){
		perror("init socket\n");
		return -1;
		/* code */
	}

	struct sockaddr_in clientSockAddr;
	bzero(&clientSockAddr,sizeof(clientSockAddr));
	clientSockAddr.sin_family=AF_INET;
	clientSockAddr.sin_port = htons(port);
	clientSockAddr.sin_addr.s_addr = inet_addr(ip);

	
	if (connect(clientSoct,(struct sockaddr *)&clientSockAddr,sizeof(struct sockaddr)) == -1){
		perror("connect socket\n");
		return -1;
	}

	fcntl(clientSoct, F_SETFL, fcntl(clientSoct, F_GETFL) | O_NONBLOCK);


	return clientSoct;
}

int getBuf(char *buf,int *bufLen,int *type)
{
	*type = (int)(random() % 4) + TEST1;
	printf("msg_type---:%d\n", *type);
	switch(*type){
		case TEST1:	getBufTest1(buf,bufLen);
			break;
		case TEST2:	getBufTest2(buf,bufLen);
			break;
		case TEST3:	getBufTest3(buf,bufLen);
			break;
		case TEST4:	getBufTest4(buf,bufLen);
			break;
		default:
			break;

	}
	return *bufLen;
}




int getBufTest1(char *buf,int *bufLen)
{
	vs_test1_t *req = (vs_test1_t*)malloc(sizeof(vs_test1_t));
	memset(req,0,sizeof(vs_test1_t));
	req->header.msg_size = sizeof(vs_test1_t);
	req->header.msg_cmd = TEST1;

	req->age = random() % 100;
	strcpy(req->name,"this is NAME");
	memcpy(buf,(char *)req,req->header.msg_size);
	*bufLen = req->header.msg_size;
	free(req);
}



int getBufTest2(char *buf,int *bufLen)
{
	int extend_num = 3;
	int packetLen = sizeof(vs_test2_t) + sizeof(int) * extend_num;

	vs_test2_t *req = (vs_test2_t*)malloc(packetLen);
	memset(req,0,packetLen);
	req->header.msg_size = packetLen;
	req->header.msg_cmd = TEST2;

	req->extend_num = extend_num;
	req->extend[0] = 101;
	req->extend[1] = 102;
	req->extend[2] = 103;

	memcpy(buf,(char *)req,req->header.msg_size);
	*bufLen = req->header.msg_size;
	free(req);
}


int getBufTest3(char *buf,int *bufLen)
{
	const char * package_name="this is package_name!!!";
	int msgLen = strlen(package_name);
	int packetLen = sizeof(vs_test3_t) + msgLen;

	vs_test3_t *req = (vs_test3_t*)malloc(packetLen);
	memset(req,0,packetLen);
	req->header.msg_size = packetLen;
	req->header.msg_cmd = TEST3;


	req->package_name_size = msgLen ;	
	memcpy(req->package_name,package_name,msgLen);

	memcpy(buf,(char *)req,req->header.msg_size);
	*bufLen = req->header.msg_size;
	free(req);
}


int getBufTest4(char *buf,int *bufLen)
{
	int user_number = 2;
	vs_user_t u1{12,22};
	vs_user_t u2{13,33};

	int packetLen = sizeof(vs_test4_t) + sizeof(vs_user_t) * user_number;

	vs_test4_t *req = (vs_test4_t*)malloc(packetLen);
	memset(req,0,packetLen);
	req->header.msg_size = packetLen;
	req->header.msg_cmd = TEST4;

	req->user_number = user_number;
	req->users[0] = u1;
	req->users[1] = u2;

	memcpy(buf,(char *)req,req->header.msg_size);
	*bufLen = req->header.msg_size;
	free(req);
}
