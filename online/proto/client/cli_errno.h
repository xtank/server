
#ifndef CLIENT_ERRNO_H
#define CLIENT_ERRNO_H

enum cli_errno_t 
{
	cli_err_sys_err = 1, /* 哎呀~~，出现了一点小状况~~ */
	cli_err_proto_format_err = 2, /* 一点小状况~~ */
	cli_err_cmd_not_find = 3, /* 一点小状况~~ */
};

#endif
