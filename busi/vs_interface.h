#ifndef __VS_INTERFACE_H_
#define __VS_INTERFACE_H_


typedef   unsigned char	   vs_uint8_t;

#define TEST1  	1001
#define TEST2  	1002
#define TEST3  	1003
#define TEST4  	1004



typedef struct {
	int						user_id;
	int						role_id;
}vs_user_t;

//---------------------------------------------------------------

typedef struct {
	int msg_size;
	int msg_cmd;
}vs_header_t;

typedef struct{
	vs_header_t header;
	int 		age;
	char 		name[30];
}vs_test1_t;

typedef struct{
	vs_header_t header;
    int         extend_num;
    int         extend[0];
}vs_test2_t;

typedef struct{
	vs_header_t 			header;
	vs_uint8_t 				package_name_size;
	vs_uint8_t 				package_name[0];
}vs_test3_t;


typedef struct{
	vs_header_t 					header;
	int								user_number;
	vs_user_t						users[0];
}vs_test4_t;

//---------------------------------------------------------------




#endif