#include "cmd_msg.h"

cmd_msg_t cmd_msg_arr[] = 
{
/**** SECTION 客户端逻辑模块与逻辑服务器通讯协议 ****/
	{"onlineproto.cs_enter_srv", "onlineproto.sc_enter_srv", 101}, /* 进入在线服务器 */
	{"onlineproto.cs_keep_live", "onlineproto.sc_keep_live", 102}, /* 进入在线服务器 */


};
uint32_t cmd_msg_arr_size = sizeof(cmd_msg_arr) / sizeof(cmd_msg_t);
