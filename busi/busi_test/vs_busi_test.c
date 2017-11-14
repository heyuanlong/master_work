#include "vs_busi_test.h"


#include "stdio.h"
#include "vs_core.h"
#include "vs_net.h"



int vs_busi_test_1(vs_busi_t *busi)
{
	printf("--------------------------------vs_busi_test_1\n");
	return VS_OK;
}
int vs_busi_test_2(vs_busi_t *busi)
{
	printf("--------------------------------vs_busi_test_2\n");
	return VS_OK;
}
int vs_busi_test_3(vs_busi_t *busi)
{
	printf("--------------------------------vs_busi_test_3\n");
	ko_busi_close_conn(busi);
	return VS_OK;
}
int vs_busi_test_4(vs_busi_t *busi)
{
	printf("--------------------------------vs_busi_test_4\n");
	return VS_OK;
}

