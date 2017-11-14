#include "vs_config.h"
#include "vs_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MS_CONF_FILE_PATH "../conf/vs.conf"
#define MS_CONF_LOAD_FAIL						-10001		//



#define LINE_LEN_BUF 100

int len;
char array[LINE_LEN_BUF*2][64];


void strTrim(char *str);
int find_first_not_of(char *str,int str_len,const char *sub,int sub_len);
int find_last_not_of(char *str,int str_len,const char *sub,int sub_len);


int config_get_key_value(const char *key, char *value)
{
	int i = 0;
	for (; i < len; )
	{	
		if (strcmp(key,array[i]) == 0){
			strcpy(value,array[i + 1]);
			return VS_OK;
		}
		i += 2; 
	}
	return VS_ERROR;
}


int config_init()
{
	const char  *f = MS_CONF_FILE_PATH;
	const char  *CharsToTrim = "#;";
	char  *p_equal;
	int sub_len = strlen(CharsToTrim);
	int endIndex;
	FILE *fp;
	#define LINE_LEN_BUF 100
	char line[LINE_LEN_BUF];
	char k[LINE_LEN_BUF];
	char v[LINE_LEN_BUF];

	if ((fp = fopen(f,"r")) == NULL){
		printf("File cannot be opened/n");  
    	return MS_CONF_LOAD_FAIL;
	}

	
	while ( fgets(line,LINE_LEN_BUF,fp) != NULL)
	{
		memset(k,0,LINE_LEN_BUF);
		memset(v,0,LINE_LEN_BUF);
		strTrim(line);
		if (strlen(line) == 0 || line[0] == '#' || line[0] == ';'){
			continue;
		}
		endIndex = find_last_not_of(line,strlen(line) ,CharsToTrim,sub_len);
		line[endIndex + 1 ] = '\0';

		if ( (p_equal=strchr(line,'=')) != NULL) {
			*p_equal = '\0';
			memcpy(k,line,LINE_LEN_BUF);
			memcpy(v,p_equal + 1 ,LINE_LEN_BUF);
			strTrim(k);
			strTrim(v);
			if (strlen(k) != 0){
				//printf("%s=%s\n",k,v);
				strcpy(array[len++],k);
				strcpy(array[len++],v);
			}
		}
	}
	fclose(fp);
	return VS_OK;
}


void strTrim(char *str)
{
	int str_len = strlen(str);
	const char *CharsToTrim = " \t\n\r";
	int sub_len = strlen(CharsToTrim);
	int startIndex;
	int endIndex;
	startIndex = find_first_not_of(str,str_len,CharsToTrim,sub_len);
	if (startIndex == str_len )
	{
		str[0] = '\0';
		return;
	}
	memmove(str,str + startIndex,str_len - startIndex + 1);//加1是为了'\0'
	endIndex = find_last_not_of(str,str_len - startIndex,CharsToTrim,sub_len);
	str[endIndex + 1 ] = '\0';
}

int find_first_not_of(char *str,int str_len,const char *sub,int sub_len)
{
	int i=0;
	int j=0;
	for ( i ; i < str_len; ++i)
	{
		for ( j ; j < sub_len; ++j)
		{
			if(str[i] == sub[j])
				break;
		}
		if(j == sub_len)
			break;
	}
	return i;
}
int find_last_not_of(char *str,int str_len,const char *sub,int sub_len)
{
	int i=str_len - 1;
	int j = 0;
	for ( i ; i >= 0; --i)
	{
		for ( j ; j < sub_len; ++j)
		{
			if(str[i] == sub[j])
				break;
		}
		if(j == sub_len)
			break;
	}
	return i;
}