/*
 * command.h
 *
 *  Created on: Aug 29, 2012
 *      Author: Admin
 */

#ifndef __GUT_COMMAND_H_
#define __GUT_COMMAND_H_
#include "buffer.h"

typedef int (*GUT_CMD_HANDLER)(int from, char *command, int argc, char **argv, int size, int total);
typedef int (*parse_handler)(int field, char *data);
typedef int (*parse_handler2)(int id, int field, char *data); //Sub token

typedef struct _GUT_HANDLER
{
    char *cmd;
    GUT_CMD_HANDLER handler;
    unsigned short dis_mask; //Disable handle from source mask
    unsigned short dis_pass; //This command can execute without pass
    unsigned short keep;     //This command must keep data
} GUT_HANDLER;

//int on_newday_comming();
//int server_on_command(int from, char * data, unsigned int size);
int server_on_command(int from, GUT_BUFFER *buffer);
int cmd_response(char *buff, int size, char *cmd, const char *fmt, ...);
int cmd_notify(char *buff, int size, const char *fmt, ...);
int cmd_string(char *buff, int size, char *cmd, const char *fmt, ...);
int cmd_response2(char *buff, int size, char *cmd, const char *fmt, ...);
int cmd_string2(char *buff, int size, char *cmd, const char *fmt, ...);
char *strtox_r(char *string, const char *seps, char **context);

#define GUT_DOWNLOAD_SOURCE_TCP0 0x01
#define GUT_DOWNLOAD_SOURCE_UDP 0x02
#define GUT_DOWNLOAD_SOURCE_TCP1 0x04
#define GUT_DOWNLOAD_SOURCE_DEBUGGER 0x08
#define GUT_DOWNLOAD_SOURCE_TERMINAL 0x40

#define GUT_END_CHAR '$'
#define GUT_END_STR "$"
#define GUT_PRIVATE_NO 0x2C424140 //@AB,
#define GUT_PUBLIC_NO 0x2C3E3E40  //@>>,

#endif /* __GUT_COMMAND_H_ */
