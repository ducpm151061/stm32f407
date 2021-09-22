/*
 * buffer.c
 *
 *  Created on: Mar 30, 2013
 *      Author: Dinh Duong Ha
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
const unsigned char r_crctable[256] = //reversed, 8-bit, poly=0x07
    {
        0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75,
        0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
        0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69,
        0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
        0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D,
        0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
        0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51,
        0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
        0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05,
        0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
        0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19,
        0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
        0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D,
        0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
        0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21,
        0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
        0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95,
        0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
        0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89,
        0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
        0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD,
        0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
        0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1,
        0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
        0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5,
        0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
        0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9,
        0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
        0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD,
        0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
        0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1,
        0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF};

unsigned char make_fcs(const unsigned char *input, int count)
{
    unsigned char fcs = 0xFF;
    int i;
    for (i = 0; i < count; i++)
    {
        fcs = r_crctable[fcs ^ input[i]];
    }
    return (0xFF - fcs);
}
void buff_alloc(GUT_BUFFER *buff, size_t max_size)
{
    buff->size = 0;
    buff->tick = 0;
    if (max_size)
    {
        buff->max_size = max_size;
        buff->data = malloc(max_size);
        if (buff->data == NULL)
            buff->max_size = 0;
    }
    else
    {
        buff->max_size = 0;
        buff->data = NULL;
    }
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = 0;
    //buff->endp = buff->max_size;
#else
    buff->readp = buff->data;
    buff->writep = buff->data;
    buff->endp = buff->data + buff->max_size;
#endif
    buff->flag_found = 0;
}

void buff_free(GUT_BUFFER *buff)
{
    if (buff->data)
    {
        free(buff->data);
    }
    buff->data = NULL;
    buff->size = 0;
    buff->max_size = 0;
    buff->tick = 0;
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = 0;
    //buff->endp = buff->max_size;
    buff->flag_found = 0;
#else
    buff->readp = buff->data;
    buff->writep = buff->data;
    buff->endp = buff->data + buff->max_size;
    buff->flag_found = 0;
#endif
}

void buff_init_static(GUT_BUFFER *buff, void *data, size_t max_size)
{
    buff->size = 0;
    buff->max_size = max_size;
    buff->data = (unsigned char *)data;
    buff->tick = 0;
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = 0;
    //buff->endp = buff->max_size;
#else
    buff->readp = buff->data;
    buff->writep = buff->data;
    buff->endp = buff->data + buff->max_size;
#endif
    buff->flag_found = 0;
}

void buff_clean_static(GUT_BUFFER *buff)
{
    buff->size = 0;
    buff->max_size = 0;
    buff->data = NULL;
    buff->tick = 0;
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = 0;
    //buff->endp = buff->max_size;
#else
    buff->readp = buff->data;
    buff->writep = buff->data;
    buff->endp = buff->data + buff->max_size;
#endif
}

#if !GUT_BUFF_MSG
GUT_BUFFER *buff_new(size_t size)
{
    GUT_BUFFER *buff = malloc(sizeof(GUT_BUFFER));
    if (buff)
    {
        buff->data = malloc(size);
        if (buff->data)
        {
            buff->max_size = size;
            buff->size = 0;
            buff->tick = 0;
            memset(buff->data, 0, buff->size);
#if GUT_BUFF_MSG
            buff->readp = 0;
            buff->writep = 0;
            //buff->endp = buff->max_size;
#else
            buff->readp = buff->data;
            buff->writep = buff->data;
            buff->endp = buff->data + buff->max_size;
#endif
        }
        else
        {
            free(buff);
            buff = NULL;
        }
    }
    else
    {
        //GUT_log(LOG_DETAIL, "ALLOC GUT_BUFFER FAILURE");
    }
    return buff;
}

void buff_del(GUT_BUFFER *buff)
{
    if (buff)
    {
        if (buff->data)
            free(buff->data);
        buff->size = 0;
        buff->max_size = 0;
        buff->data = NULL;
#if GUT_BUFF_MSG
        buff->readp = 0;
        buff->writep = 0;
        //buff->endp = buff->max_size;
#else
        buff->readp = buff->data;
        buff->writep = buff->data;
        buff->endp = buff->data + buff->max_size;
#endif
        free(buff);
    }
}

#endif

size_t buff_size(GUT_BUFFER *buff)
{
    return buff->size;
}

int buff_add(GUT_BUFFER *buff, char c)
{
    if (buff->max_size == 0)
        return -1;
    if (buff->size >= buff->max_size)
        buff->size = 0;
    buff->data[buff->size++] = c;
    buff->data[buff->size] = 0;
    return buff->size;
}

int buff_append(GUT_BUFFER *dest, GUT_BUFFER *src)
{
    size_t total = dest->size + src->size;
    if (total >= dest->max_size)
        return 0;
    memcpy(dest->data + dest->size, (const char *)src->data, src->size);
    //for (int i = 0; i < src->size; i++)
    //  dest->data[dest->size+i] = src->data[i];
    dest->size += src->size;
    dest->data[dest->size] = 0;
    src->size = 0;
    return dest->size;
}

int buff_write(GUT_BUFFER *dest, const char *input, int size)
{
#if 0
    int c = buf->endp - buf->writep;

    count = min(count, buffer_free(buf));
    if (count > c)
    {
        memcpy(buf->writep, input, c);
        memcpy(buf->data, input + c, count - c);
        buf->writep = buf->data + (count - c);
    }
    else
    {
        memcpy(buf->writep, input, count);
        buf->writep += count;
        if (buf->writep == buf->endp)
            buf->writep = buf->data;
    }
    return count;
#endif
    size_t total = dest->size + size;
    if (total >= dest->max_size)
        return 0;
    memcpy(dest->data + dest->size, (const char *)input, size);
    //for (int i = 0; i < src->size; i++)
    //  dest->data[dest->size+i] = src->data[i];
    dest->size += size;
    dest->data[dest->size] = 0;
    return dest->size;
}

int buff_roll(GUT_BUFFER *buff, char c)
{
    for (int i = 1; i < buff->size; i++)
    {
        if (buff->data[i] == c)
        {
            memmove(buff->data, buff->data + i, (buff->size - i));
            buff->size -= i;
            buff->data[buff->size] = 0;
            return buff->size;
        }
    }
    buff->size = 0;
    buff->data[0] = 0;
    return buff->size;
}

int buff_roll_str(GUT_BUFFER *buff, const char *str)
{
    for (int i = 1; i < buff->size; i++)
    {
        for (int j = 0; j < strlen(str); j++)
        {
            if (buff->data[i] == str[j])
            {
                memmove(buff->data, buff->data + i, (buff->size - i));
                buff->size -= i;
                buff->data[buff->size] = 0;
                return buff->size;
            }
        }
    }
    buff->size = 0;
    buff->data[0] = 0;
    return buff->size;
}

GUT_BUFFER *buff_copy(GUT_BUFFER *buff)
{
    register int size = buff->size;
    GUT_BUFFER *dest = buff_new(size);
    if (dest)
    {
        memcpy(dest->data, buff->data, size);
        dest->size = size;
        //dest->update = xTaskGetTickCount();
    }
    else
    {
        //      usart_x0_send('C');
        //      usart_x0_send('C');
        //      usart_x0_send('\r');
        //      usart_x0_send('\n');
    }
    return dest;
}

size_t buff_remove(GUT_BUFFER *buff, size_t size)
{
    if (buff->size > size)
    {
        memmove(buff->data, buff->data + size, buff->size - size);
        buff->size -= size;
    }
    else
    {
        buff->size = 0;
    }
    buff->data[buff->size] = 0;
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = buff->size;
#else
    buff->readp = buff->data;
    buff->writep = buff->data + buff->size;
#endif
    return buff->size;
}

void buff_clear(GUT_BUFFER *buff)
{
    buff->size = 0;
    buff->data[0] = 0;
#if GUT_BUFF_MSG
    buff->readp = 0;
    buff->writep = 0;
    //buff->endp = buff->max_size;
#else
    buff->readp = buff->data;
    buff->writep = buff->data + buff->size;
    buff->endp = buff->data + buff->max_size;
#endif
}

void destroy_frame(GUT_FRAME *frame)
{
    if (frame)
    {
        if (frame->data_length > 0)
            free(frame->data);
        free(frame);
    }
}
#if 0
//Get Frame
GUT_FRAME *buffer_get_frame(GUT_BUFFER *buf)
{
    int end;
    int length_needed = 5; // channel, type, length, fcs, flag
    unsigned char *data;
    unsigned char fcs = 0xFF;

    GUT_FRAME *frame = NULL;
//    usart_x0_string("\r\nZZZZZZZZZZZZZ\r\n");
//    for(int i = 0; i < buf->size; i++) {
//        usart_x0_send(buf->data[i]);
//    }
//    usart_x0_string("\r\nZZZZZZZZZZZZZ\r\n");
    // Find start flag
    while (!buf->flag_found && buffer_length(buf) > 0)
    {
#if GUT_BUFF_MSG
        if (*(buf->data + buf->readp) == F_FLAG)
            buf->flag_found = 1;
        buf->readp++;
        if (buf->readp >= buf->max_size) buf->readp = 0;
#else
        if (*buf->readp == F_FLAG)
            buf->flag_found = 1;
        INC_BUF_POINTER(buf, buf->readp);
#endif
    }
    if (!buf->flag_found) // no frame started
        return NULL;

    // skip empty frames (this causes troubles if we're using DLC 62)
#if GUT_BUFF_MSG
    while (buffer_length(buf) > 0 &&
            (*(buf->data + buf->readp) == F_FLAG))
    {
        //INC_BUF_POINTER(buf, buf->readp);
        buf->readp++;
        if (buf->readp >= buf->max_size) buf->readp = 0;
    }
#else
    while (buffer_length(buf) > 0 &&
            (*buf->readp == F_FLAG))
    {
        INC_BUF_POINTER(buf, buf->readp);
    }
#endif
    if (buffer_length(buf) >= length_needed)
    {
#if GUT_BUFF_MSG
        data = buf->data + buf->readp;
#else
        data = buf->readp;
#endif
        frame = malloc(sizeof(GUT_FRAME));

        frame->channel = ((*data & 252) >> 2);
        fcs = r_crctable[fcs^*data];
#if GUT_BUFF_MSG
        INC_BUF_POINTER(buf, data);
#else
        INC_BUF_POINTER(buf, data);
#endif
        frame->control = *data;
        fcs = r_crctable[fcs^*data];
        INC_BUF_POINTER(buf, data);

        frame->data_length = (*data & 254) >> 1;
        fcs = r_crctable[fcs^*data];
        if ((*data & 1) == 0)
        {
            /* Current spec (version 7.1.0) states these kind of frames to be invalid
             * Long lost of sync might be caused if we would expect a long
             * frame because of an error in length field.
            INC_BUF_POINTER(buf,data);
            frame->data_length += (*data*128);
            fcs = r_crctable[fcs^*data];
            length_needed++;
            */
            free(frame);
#if GUT_BUFF_MSG
            buf->readp = data - buf->data;
#else
            buf->readp = data;
#endif
            buf->flag_found = 0;
            return buffer_get_frame(buf);
        }
        length_needed += frame->data_length;
        if (!(buffer_length(buf) >= length_needed))
        {
            free(frame);
            return NULL;
        }
        INC_BUF_POINTER(buf, data);
        //extract data
        if (frame->data_length > 0)
        {
            if ((frame->data = malloc(sizeof(char) * frame->data_length)))
            {
#if GUT_BUFF_MSG
                end = (buf->data + buf->max_size) - data;
#else
                end = buf->endp - data;
#endif
                if (frame->data_length > end)
                {
                    memcpy(frame->data, data, end);
                    memcpy(frame->data + end, buf->data, frame->data_length - end);
                    data = buf->data + (frame->data_length - end);
                }
                else
                {
                    memcpy(frame->data, data, frame->data_length);
                    data += frame->data_length;
#if GUT_BUFF_MSG
                    if (data == buf->data + buf->max_size)
                        data = buf->data;
#else
                    if (data == buf->endp)
                        data = buf->data;
#endif
                }
                if (FRAME_IS(UI, frame))
                {
                    for (end = 0; end < frame->data_length; end++)
                        fcs = r_crctable[fcs ^ (frame->data[end])];
                }
            }
            else
            {
                frame->data_length = 0;
            }
        }
        // check FCS
        if (r_crctable[fcs ^ (*data)] != 0xCF)
        {
            destroy_frame(frame);
            buf->flag_found = 0;
            //buf->dropped_count++;
#if GUT_BUFF_MSG
            buf->readp = data - buf->data;
#else
            buf->readp = data;
#endif
            return buffer_get_frame(buf);
        }
        else
        {
            // check end flag
            INC_BUF_POINTER(buf, data);
            if (*data != F_FLAG)
            {
                gut_log(LOG_DETAIL, "Dropping frame: End flag not found. Instead: %d\n", *data);
                destroy_frame(frame);
                buf->flag_found = 0;
                //buf->dropped_count++;
#if GUT_BUFF_MSG
                buf->readp = data - buf->data;
#else
                buf->readp = data;
#endif
                return buffer_get_frame(buf);
            }
            else
            {
                //buf->received_count++;
            }
            INC_BUF_POINTER(buf, data);
        }
#if GUT_BUFF_MSG
        buf->readp = data - buf->data;
#else
        buf->readp = data;
#endif
    }
    return frame;
}
#endif

#if 0
GUT_FRAME *buffer_get_frame(GUT_BUFFER *buf)
{
    int end;
    int length_needed = 5; // channel, type, length, fcs, flag | begin, cam_id, command, content, cks2, parity1, end
    unsigned char *data;
    unsigned char fcs = 0xFF;

    GUT_FRAME *frame = NULL;
#if 0
    // Find start flag
    while (!buf->flag_found && buffer_length(buf) > 0)
    {
        if (*buf->readp == CAM_START_FLAG)
            buf->flag_found = 1;
        INC_BUF_POINTER(buf, buf->readp);
    }
    if (!buf->flag_found) // no frame started
        return NULL;

    // skip empty frames (this causes troubles if we're using DLC 62)
    while (buffer_length(buf) > 0 && (*buff->readp == CAM_START_FLAG))
    {
        INC_BUF_POINTER(buf, buf->readp);
    }

    if (buffer_length(buf) >= length_needed)
    {
        data = buf->readp;
        frame = pvPortMalloc(sizeof(GUT_FRAME));

        frame->channel = ((*data & 252) >> 2);
        //fcs = r_crctable[fcs^*data];
        INC_BUF_POINTER(buf, data);

        frame->control = *data;
        fcs = r_crctable[fcs^*data];
        INC_BUF_POINTER(buf, data);

        frame->data_length = (*data & 254) >> 1;
        fcs = r_crctable[fcs^*data];
        if ((*data & 1) == 0)
        {
            /* Current spec (version 7.1.0) states these kind of frames to be invalid
            * Long lost of sync might be caused if we would expect a long
            * frame because of an error in length field.
            INC_BUF_POINTER(buf,data);
            frame->data_length += (*data*128);
            fcs = r_crctable[fcs^*data];
            length_needed++;
            */
            free(frame);
            buf->readp = data;
            buf->flag_found = 0;
            return buffer_get_frame(buf);
        }

        length_needed += frame->data_length;
        if (!(gsm0710_buffer_length(buf) >= length_needed))
        {
            free(frame);
            return NULL;
        }
        INC_BUF_POINTER(buf, data);
        //extract data
        if (frame->data_length > 0)
        {
            if ((frame->data = malloc(sizeof(char) * frame->data_length)))
            {
                end = buf->endp - data;
                if (frame->data_length > end)
                {
                    memcpy(frame->data, data, end);
                    memcpy(frame->data + end, buf->data, frame->data_length - end);
                    data = buf->data + (frame->data_length - end);
                }
                else
                {
                    memcpy(frame->data, data, frame->data_length);
                    data += frame->data_length;
                    if (data == buf->endp)
                        data = buf->data;
                }
                if (FRAME_IS(UI, frame))
                {
                    for (end = 0; end < frame->data_length; end++)
                        fcs = r_crctable[fcs ^ (frame->data[end])];
                }
            }
            else
            {
                syslog(LOG_ALERT, "Out of memory, when allocating space for frame data.\n");
                frame->data_length = 0;
            }
        }
        // check FCS
        if (r_crctable[fcs ^ (*data)] != 0xCF)
        {
            syslog(LOG_INFO, "Dropping frame: FCS doesn't match\n");
            destroy_frame(frame);
            buf->flag_found = 0;
            buf->dropped_count++;
            buf->readp = data;
            return gsm0710_buffer_get_frame(buf);
        }
        else
        {
            // check end flag
            INC_BUF_POINTER(buf, data);
            if (*data != F_FLAG)
            {
                syslog(LOG_WARNING, "Dropping frame: End flag not found. Instead: %d\n", *data);
                destroy_frame(frame);
                buf->flag_found = 0;
                buf->dropped_count++;
                buf->readp = data;
                return gsm0710_buffer_get_frame(buf);
            }
            else
            {
                buf->received_count++;
            }
            INC_BUF_POINTER(buf, data);
        }
        buf->readp = data;
    }
#endif
    return frame;
}
#endif
