#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
/* Scheduler includes. */
#include "DataDefine.h"
#include "command.h"
#include "Parameters.h"

#define GUT_PUBLIC_AP_START 3
#define GUT_PRIVATE_AP_START 4

extern time_t g_sys_time;
static int response = 0;

static char buffTmp[255] = {0};
#define GUT_MAX_CMD_LEN 20
static char command[GUT_MAX_CMD_LEN] = {0};
static char item1[GUT_MAX_CMD_LEN] = {0};
static char item2[GUT_MAX_CMD_LEN] = {0};
static char item3[GUT_MAX_CMD_LEN] = {0};

extern System_t xSystem;
Parameter_t *g_config = &xSystem.Parameters;
static char Response[300];

void PrintHex(void *data, int len);

int next_char(char *data, char sep, int max)
{
    char *ptr = strchr(data, sep);
    if (ptr)
    {
        return (ptr - data) >= max ? max : (ptr - data);
    }
    else
        return 0;
}

//int cmd_string(char* buff, int size, char *cmd, const char *fmt, ...)
//{
//  int n = 0;
//    va_list ap;
//    memset(buff, 0, size);
//    n = snprintf(buff, size, "*SS,%s,%s,", g_config.deviceID, cmd);
//    va_start(ap, fmt);
//    char *bufftmp = buff + (n);
//    n = uvsnprintf(bufftmp, size-(n+1), (char *) fmt, ap );
//    va_end(ap);
//    bufftmp[n] = GUT_END_CHAR;
//    return n+1;
//}

/*
    NOTIFY:  @>>,DevID,MSG,sysTime,msgType,....
    USER REQUEST:   @>>,DevID,cmd,SecCode,get/set
    USER RESPONSE:  @>>,DevID,RSP,cmd,sysTime,msgType,....
    ADMIN REQUEST:  @AB,1234567890,888888,cmd,get/set
    ADMIN RESPONSE: @AB,1234567890,RSP,cmd,sysTime,msgType,....
    OLD:            *SS,1234567890,S96,112233,0#
*/
int cmd_response(char *buff, int size, char *cmd, const char *fmt, ...)
{
    //@AB,A00000001,RSP,NET,sysTime,Param ....
    int n = 0;
    va_list ap;
    unsigned long tmp[2] = {GUT_PRIVATE_NO, 0};
    char *p = (char *)tmp;
    n = snprintf(buff, size, "%s%s,RSP,%s,%u,", p, g_config->DeviceID, cmd, g_sys_time);
    va_start(ap, fmt);
    char *bufftmp = buff + (n);
    n = vsnprintf(bufftmp, size - (n + 1), (char *)fmt, ap);
    va_end(ap);
    bufftmp[n] = GUT_END_CHAR;
    return n + 1;
}

#if 0
int cmd_notify(char *buff, int size, const char *fmt, ...)
{
    int n = 0;
    va_list ap;
    memset(buff, 0, size);
#if GUT_CONF_PUBLIC
    unsigned long tmp[2] = {GUT_PUBLIC_NO, 0};
    char *p = (char *)tmp;
    //@>>,DevID,MSG,sysTime,....
    n = snprintf(buff, size, "%s%s,MSG,%u,", p, g_config.deviceID, g_sys_time);
#else
    unsigned long tmp[2] = {GUT_PRIVATE_NO, 0};
    char *p = (char *)tmp;
    n = snprintf(buff, size, "%s%s,V0,", p, g_config->DeviceID);
#endif
    va_start(ap, fmt);
    char *bufftmp = buff + (n);
    n = vsnprintf(bufftmp, size - (n + 1), (char *) fmt, ap);
    va_end(ap);
    bufftmp[n] = GUT_END_CHAR;
    return n + 1;
}
#endif

int cmd_response2(char *buff, int size, char *cmd, const char *fmt, ...)
{
    //@>>,12121212,RSP,NET,sysTime,Param ....
    //@>>,12121212,POS,sysTime,Param ....
    int n = 0;
    va_list ap;
    unsigned long tmp[2] = {GUT_PUBLIC_NO, 0};
    char *p = (char *)tmp;
    n = snprintf(buff, size, "%s%s,RSP,%s,%u,", p, g_config->DeviceID, cmd, g_sys_time);
    va_start(ap, fmt);
    char *bufftmp = buff + (n);
    n = vsnprintf(bufftmp, size - (n + 1), (char *)fmt, ap);
    va_end(ap);
    bufftmp[n] = GUT_END_CHAR;
    return n + 1;
}

char *strtox_r(char *string, const char *seps, char **context)
{
    char *head; /* start of word */
    char *tail; /* end of word */

    /* If we're starting up, initialize context */
    if (string)
    {
        *context = string;
    }

    /* Get potential start of this next word */
    head = *context;
    if (head == NULL)
    {
        return NULL;
    }

    /* Skip any leading separators */
    //while (*head && strchr(seps, *head)) {
    //    head++;
    //}

    /* Did we hit the end? */
    if (*head == 0)
    {
        /* Nothing left */
        *context = NULL;
        return NULL;
    }

    /* skip over word */
    tail = head;
    while (*tail && !strchr(seps, *tail))
    {
        tail++;
    }

    /* Save head for next time in context */
    if (*tail == 0)
    {
        *context = NULL;
    }
    else
    {
        *tail = 0;
        tail++;
        *context = tail;
    }

    /* Return current word */
    return head;
}

int parse_string(char *data, char *delim, parse_handler handle)
{
    char *str = data;
    char *saveptr = NULL;
    char *token = strtox_r(str, delim, &saveptr);
    int field = 0;
    while (token != NULL)
    {
        //Update field
        handle(field, token);
        field++;
        token = strtox_r(NULL, delim, &saveptr);
    }
    return 0;
}

int parse_string2(char *data, char *delim, char *subdelim, parse_handler2 handle)
{
    int idx = 0;
    int field = 0;
    char *str = data;
    char *saveptr = NULL;
    char *token = strtox_r(str, delim, &saveptr);
    while (token != NULL)
    {
        if (strlen(token) == 0)
        {
            token = strtox_r(NULL, delim, &saveptr);
            continue;
        }
        field = 0;
        char *saveptr2 = NULL;
        char *subtoken = strtox_r(token, subdelim, &saveptr2);
        while (subtoken != NULL)
        {
            //Update field
            handle(idx, field, subtoken);
            field++;
            subtoken = strtox_r(NULL, subdelim, &saveptr2);
        }
        idx++;
        token = strtox_r(NULL, delim, &saveptr);
    }
    return 0;
}

#if 0
static int cmd_ack(int from, char *command, int argc, char **argv, int size, int total)
{

    int type = atoi(argv[4]);
    int critiId = atoi(argv[5]);
    switch (type)
    {
    case 1:
        finish_critical(critiId);
        break;
    default:
        break;
    }
    response = 0;
    return size;
}
#endif

static int cmd_S90P(int from, char *command, int argc, char **argv, int size, int total)
{
    // Cau hinh ID
    // @>>,SERIAL,12345678,0,A00000002$
    // @>>,SERIAL,12345678,1,A00000002$

    int len = 0;
    char set = 0;

    if (argv[3] == NULL)
        return size;
    set = atoi(argv[3]);
    if (set != 0) // Read
    {
        set = 0;
        if (argv[4] == NULL)
            return size;
        char *ptr = argv[4];
        //if (isalpha(*ptr)) {
        if (*ptr == 'A')
        {
            ptr++;
            unsigned long id = strtoul(ptr, NULL, 10);
            while (isdigit(*ptr))
                ptr++;
            len = ptr - argv[4];
            //if (len == 9)
            {
                memset(g_config->DeviceID, 0, 12);
                //memcpy(g_config.deviceID, argv[4], len<=12?len:12);
                snprintf((char *)g_config->DeviceID, sizeof(g_config->DeviceID), "A%08u", (uint32_t)id);
                set = 1;
                SaveConfigToFlash();
            }
        }
    }
    cmd_response2(Response, sizeof(Response), command, "%d,%s", set, g_config->DeviceID);
    response = 1;
    return size;
}

static int cmd_date(int from, char *command, int argc, char **argv, int size, int total)
{
    // Cau hinh ngay xuat xuong
    // @>>,DATE,12345678,get/set,pos,date$
    // @>>,DATE,12345678,0,0,A00000002$
    // @>>,DATE,12345678,1,A00000002$

    int len = 0;
    char set = 0;
    if (argc < 4)
        return size;
    if (argv[3] == NULL)
        return size;
    if (argv[4] == NULL)
        return size;
    memset(buffTmp, 0, sizeof(buffTmp));
    int pos = (int)strtoul(argv[4], NULL, 10);
    set = atoi(argv[3]);
    if (set != 0)
    {
        set = 0;
        if (argc < 6)
            return size;
        char *ptr = argv[5];

        memcpy(buffTmp, ptr, size - (ptr - argv[0]));
        switch (pos)
        {
        case 0:
        {
            set = 1;
            snprintf((char *)g_config->NgayXuatXuong, sizeof(g_config->NgayXuatXuong), "%s", buffTmp);
        }
        break;
        case 1:
        {
            set = 1;
            snprintf((char *)g_config->NgayHieuChinh, sizeof(g_config->NgayHieuChinh), "%s", buffTmp);
        }
        break;
        case 2:
        {
            set = 1;
            snprintf((char *)g_config->NgayDuPhong, sizeof(g_config->NgayDuPhong), "%s", buffTmp);
        }
        break;
        default:
            break;
        }

        if (set)
        {
            SaveConfigToFlash();
        }
    }
    else
    {
        switch (pos)
        {
        case 0:
        {
            snprintf(buffTmp, sizeof(g_config->NgayXuatXuong), "%s", (char *)g_config->NgayXuatXuong);
        }
        break;
        case 1:
        {
            snprintf(buffTmp, sizeof(g_config->NgayHieuChinh), "%s", (char *)g_config->NgayHieuChinh);
        }
        break;
        case 2:
        {
            snprintf(buffTmp, sizeof(g_config->NgayDuPhong), "%s", (char *)g_config->NgayDuPhong);
        }
        break;
        default:
            break;
        }
    }
    cmd_response2(Response, sizeof(Response), command, "%d,%d,%s", set, pos, buffTmp);
    response = 1;
    return size;
}

static int cmd_calib(int from, char *command, int argc, char **argv, int size, int total)
{
    // @>>,CALIB,12345678,get/set,position,adc,mgl$
    // @>>,CALIB,12345678,0,1$
    // @>>,CALIB,12345678,1,1,150,1255$

    int len = 0;
    char set = 0;
    int pos = 0;
    uint16_t adc = 0;
    uint16_t mgl = 0;
    if (argc < 5)
        return size;
    pos = (int)strtoul(argv[4], NULL, 10);
    if (pos < 12)
    {
        adc = g_config->AlMCalibData.s.Data[pos].ValueADC;
        mgl = g_config->AlMCalibData.s.Data[pos].ValueMgL;
    }
    set = atoi(argv[3]);
    if (set != 0) // Read
    {
        set = 0;
        if (argc < 7)
            return size;
        adc = (uint16_t)strtoul(argv[5], NULL, 10);
        mgl = (uint16_t)strtoul(argv[5], NULL, 10);
        if (pos < 12)
        {
            set = 1;
            g_config->AlMCalibData.s.Data[pos].ValueADC = adc;
            g_config->AlMCalibData.s.Data[pos].ValueMgL = mgl;
            SaveConfigToFlash();
        }
    }
    cmd_response2(Response, sizeof(Response), command, "%d,%s,%d,%d,%d", set, pos, adc, mgl);
    response = 1;
    return size;
}

uint16_t Cal_CRC16(const uint8_t *data, uint32_t size);

/// Private command with other password
GUT_HANDLER handlers[] =
    {
        {NULL, NULL}};

/// Public command without password
GUT_HANDLER handlers2[] =
    {
        /*
    {"GET",     sks_cmd_G2},
    {"DEAD",    cmd_dead},
    {"ACCEPT",  cmd_accept},
    {"SH0",     cmd_dlt_ping},
    {"SH1",     cmd_dlt},
    {"SH2",     cmd_dlt2},
    {"SH3",     cmd_reprint},
    */
        {NULL, NULL}};

/// Public command with password

GUT_HANDLER handlers3[] =
    {
        {"SERIAL", cmd_S90P}, //SET SERIAL
        {"DATE", cmd_date},   //SET DATE
        {"CALIB", cmd_calib}, //SET CALIB
        {NULL, NULL}};

#define GUT_MAX_ARGV 25
int server_on_command(int fromSource, GUT_BUFFER *buffer)
{
    unsigned int size = buffer->size;

    int from = (fromSource & 0xFFFFFF);
    int fromIdx = ((fromSource >> 24) & 0xFF);
    int result = 0;
    int field = 0;
    int ftmp;

    int type = 0;
    int countTmp = buffer->size;
cont_proccess:
    while ((buffer->size > 0) && (countTmp > 0) && (buffer->size <= size))
    {
        countTmp--;
        int endPos = 0;
        char item_buff[20] = {0};
        char *arg[GUT_MAX_ARGV] = {0};
        char *data = (char *)buffer->data;
        char *ptr = data;
        size = buffer->size;
        endPos = 0;
        ptr = data;
        data[size] = 0;
        //*XX,YYYYYYYYYY,CMD,VVVVVV,PARA1,PARA2,�#
        //***,CMD,YYYYYYYYYY,PARA1,PARA2,�#
        //*SB,CMD,CUSTOMER_ID(ADMINCODE/SECKEY?),PARA1,PARA2,�#
        //sks_log(LOG_MASK_MAIN_TASK, "Process COMMAND size=%d data=%s\r\n", buffer->size, buffer->data);
        //xprintf("Process COMMAND from %d size=%d data=%s\r\n", fromSource, buffer->size, buffer->data);
        //READxx,....#
        int readOut = 0;
        unsigned long cmdTime = 0;
        //unsigned long cmdTime2 = 0;
        unsigned long tmpNode = 0;
        if ((memcmp(data, "READ", 4) == 0) && (data[4] != ',')) //READxxddMMyy#
        {
            for (int i = 0; i < size; i++)
            {
                if ((tmpNode == 0) && (data[i] == ','))
                {
                    tmpNode = i;
                }
                if (data[i] == GUT_END_CHAR)
                {
                    endPos = i + 1;
                    break;
                }
            }
            if (endPos == 0)
                return 0; //Waiting for more data,
            if ((endPos == 13) && (tmpNode == 0))
            {
                cmdTime = atoi(data + 4);
                buff_remove(buffer, 13);
                continue;
            }
            else if (data[12] == ',')
            {
                // READxxddmmyy,hhmmss# -> In report
                //cmdTime2 = atoi(data+13);
                //raise_beep_short();
                //Beep(3);
                //MT_data(cmdTime, data+13);
                buff_roll(buffer, GUT_END_CHAR);
                //buff_remove(buffer, 1);
                continue;
            }
            else
            {
                buff_roll(buffer, GUT_END_CHAR);
                //buff_remove(buffer, 1);
                continue;
            }
        }
        else if (memcmp(data, "WRITE", 5) == 0)
        {
            //          //WRITExx#
            //          if (size < 8) return 0;  //Wait for more data
            //          cmdTime = atoi(data+5);
            //          if (data[7] == ',') {
            //              cmdTime2 = atoi(data+8);
            //          }
            //          //MT_data(cmdTime*1000000, cmdTime2);
            buff_roll(buffer, GUT_END_CHAR);
            //buff_remove(buffer, 5);
            continue;
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                if ((*ptr != '@') && (*ptr != '*') && (*ptr != 'R') && (*ptr != 'W'))
                    ptr++;
            }
            if (ptr > data)
            {
                buff_remove(buffer, ptr - data);
                if (buffer->size <= 1)
                    return 0;
                continue;
            }

            for (int i = 0; i < size; i++)
            {
                //if ((data[i] == GUT_END_CHAR)||(data[i] == '\n')||(data[i] == '\0')) {
                if ((data[i] == GUT_END_CHAR) || (data[i] == '\0'))
                {
                    endPos = i + 1;
                    break;
                }
            }
        } // Find End Pos
#if 0
        char *pos = strchr(data, GUT_END_CHAR);
        if (pos != NULL)
        {
            char array[255] = {0};
            memcpy(array, data, pos - data);
        }
#endif

        if (endPos == 0)
            return 0; //Waiting for end command sign

        unsigned long *pInt2 = (unsigned long *)ptr;
        int j = 0;
        for (int i = 0; i < endPos; i++)
        {
            //if ((data[i] == ',') || (data[i] == '\n')|| (data[i] == GUT_END_CHAR)|| (data[i] == 0x00)) //complete one item
            if ((data[i] == ',') || (data[i] == GUT_END_CHAR) || (data[i] == 0x00)) //complete one item
            {
                if (j > 0)
                {
                    if ((*(uint32_t *)(ptr) == GUT_PRIVATE_NO) || (*(uint32_t *)(ptr) == GUT_PUBLIC_NO) || (*(uint32_t *)(ptr) == 0x44414552))
                    {
                        field = 0;
                        type = 0;
                        memset(command, 0, GUT_MAX_CMD_LEN);
                        memset(item1, 0, GUT_MAX_CMD_LEN);
                        memset(item2, 0, GUT_MAX_CMD_LEN);
                        memset(item3, 0, GUT_MAX_CMD_LEN);
                    }
                    ftmp = field;
                    arg[field % GUT_MAX_ARGV] = ptr;
                    switch (ftmp)
                    {
                    case 0:
                    {
                        if ((*(uint32_t *)(ptr) != GUT_PRIVATE_NO) && (*(uint32_t *)(ptr) != GUT_PUBLIC_NO) && (*(uint32_t *)(ptr) != 0x44414552))
                        {
                            buff_remove(buffer, endPos);
                            goto cont_proccess;
                        }

                        if (*(uint32_t *)(ptr) == GUT_PRIVATE_NO)
                        {
                        }
                        else
                        {
                            type = 1;
                        }
                    }
                    break;
                    case 1:
                        //#if GUT_CONF_PUBLIC
                        item1[0] = 0;
                        strcat(item1, item_buff);
                        //#else
                        //                  if (memcmp(ptr, g_config.deviceID, 10)) invalide_code |= 0x1; //Invalid DevID
                        //#endif
                        break;
                    case 2:
                        //#if GUT_CONF_PUBLIC
                        item2[0] = 0;
                        strcat(item2, item_buff);
                        //#else
                        //                  memcpy(command, ptr, j<GUT_MAX_CMD_LEN?j:GUT_MAX_CMD_LEN);
                        //#endif
                        break;
                    case 3:
                        //#if GUT_CONF_PUBLIC
                        item3[0] = 0;
                        strcat(item3, item_buff);
                        //#else
                        //                  if (memcmp(ptr, g_config.adminCode, 6)) invalide_code |= 0x2; //Invalid AdminCode
                        //#endif
                        break;
                    default:
                        //return endPos;
                        break;
                    }
                }
                //else
                //  arg[field%GUT_MAX_ARGV] = NULL;
                field++;
                if (field >= GUT_MAX_ARGV)
                    break;
                j = 0; //reset index
                ptr = data + i + 1;
                arg[field % GUT_MAX_ARGV] = ptr;
            }
            else
            {
                item_buff[j % sizeof(item_buff)] = data[i];
                j++;
                j = j % (sizeof(item_buff));
                item_buff[j] = 0x00;
            }
        }
        //if (field > GUT_MAX_ARGV) return endPos;  //Too many param
        int cmdIdx = 0;
        result = -1;
        response = 0;
        //DebugPrint("START PROCESS COMMAND TYPE: %d, field: %d\r\n", type, field);
        // type ==1 => No security check
        if (type == 1) /*||(from == GUT_CMD_SOURCE_CUSSER)*/
        {
            //FKC7VT3
            //Check Security here
            if (field <= 2)
            {
                buff_remove(buffer, endPos);
                goto cont_proccess;
                //return endPos; //Too few param
            }
            result = endPos;
            command[0] = 0x0;
            strcat(command, item1);

            int found = 0; // Process free cmd, no need security code
            cmdIdx = 0;
            while (handlers2[cmdIdx].cmd)
            {
                if (strncmp(command, handlers2[cmdIdx].cmd, 7) != 0)
                {
                    cmdIdx++;
                    continue;
                }
                if (handlers2[cmdIdx].handler == NULL)
                {
                    cmdIdx++;
                    continue;
                }
                memset(Response, 0, sizeof(Response));
                result = handlers2[cmdIdx].handler(from, command, field, arg, endPos, size);
                found = 1;
                break;
            }
            if ((!found) /*&& (type != 1)*/)
            {
#if 0
                // CHECK SECURITY CODE
                if (strncmp(item2, g_config.secKey, 7) != 0)
                {
                    buff_remove(buffer, endPos);
                    goto cont_proccess;
                    //return endPos;
                }
#endif
                cmdIdx = 0;
                while (handlers3[cmdIdx].cmd)
                {
                    if (strcmp(command, handlers3[cmdIdx].cmd) != 0)
                    {
                        cmdIdx++;
                        continue;
                    }
                    if (handlers3[cmdIdx].handler == NULL)
                    {
                        cmdIdx++;
                        continue;
                    }
                    memset(Response, 0, sizeof(Response));
                    //DebugPrint("PROCESS COMMAND %s handlers3[%d].cmd=%s\r\n", command, cmdIdx, handlers3[cmdIdx].cmd);
                    result = handlers3[cmdIdx].handler(from, command, field, arg, endPos, size);
                    break;
                }
            }
        }
        else
        {
            if (field <= 4)
            {
                buff_remove(buffer, endPos);
                goto cont_proccess;
                //return endPos; //Too few param
            }
            command[0] = 0x0;
            strcat(command, item2);

            int cmdIdx = 0;
            while (handlers[cmdIdx].cmd)
            {
                if (strcmp(command, handlers[cmdIdx].cmd) == 0)
                {
                    //DebugPrint("PROCESS COMMAND %s handlers[%d].cmd=%s", command, cmdIdx, handlers[cmdIdx].cmd);
                    if (handlers[cmdIdx].handler == NULL)
                        break;
                    if (!(handlers[cmdIdx].dis_mask & from)) //Chi cho phep gui lenh tu nguon
                    {
                        memset(Response, 0, sizeof(Response));
                        result = handlers[cmdIdx].handler(from, command, field, arg, endPos, size);
                    }
                    break;
                }
                cmdIdx++;
            }
        }
        if (result > 0)
        {
            //if (!((result==0)&&(arg[4][0]==GUT_END_CHAR))) return 0; //WAITING FOR MORE DATA
            if (response)
            {
                // TODO: Disable beep for sleeping
                Beep(5);
            }
            buff_remove(buffer, result);
        }
        else if (result < 0)
        {
            buff_remove(buffer, endPos);
        }
        else
        {
            break;
        }
    } //END WHILE
    return result;
}
