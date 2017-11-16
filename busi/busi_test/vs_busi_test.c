#include "vs_busi_test.h"


#include "stdio.h"
#include "vs_core.h"
#include "vs_net.h"



int vs_busi_test_1(vs_busi_t *busi)
{
	vs_log_info("--------------------------------vs_busi_test_1");
	return VS_OK;
}
int vs_busi_test_2(vs_busi_t *busi)
{
	vs_log_info("--------------------------------vs_busi_test_2");
	return VS_OK;
}
int vs_busi_test_3(vs_busi_t *busi)
{
	vs_log_info("--------------------------------vs_busi_test_3");
	ko_busi_close_conn(busi);
	return VS_OK;
}
int vs_busi_test_4(vs_busi_t *busi)
{
	vs_log_info("--------------------------------vs_busi_test_4");
	return VS_OK;
}

