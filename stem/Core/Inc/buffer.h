#ifndef __GUT_BUFFER__
#define __GUT_BUFFER__

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

typedef struct _GUT_FRAME
{
    unsigned char channel;
    unsigned char control;
    int data_length;
    unsigned char *data;
} GUT_FRAME;
//#define GUT_BUFF_MSG  1
#if GUT_BUFF_MSG
typedef struct _GUT_BUFFER
{
    unsigned long tick;      //tickcount since start application
    unsigned short max_size; //Max size of Buffer;
    unsigned short size;     //Size of available data
    unsigned char *data;     //Point to data lParam
    unsigned short readp;    //High Word:Read offset - Low Word: Write Offset xParam
    unsigned short writep;
    unsigned short endpx;
    unsigned short flag_found;
} GUT_BUFFER;
#else
typedef struct _GUT_BUFFER
{
    unsigned long tick;      //tickcount since start application
    unsigned short max_size; //Max size of Buffer;
    unsigned short size;     //Size of available data
    unsigned char *data;     //Point to data lParam
    unsigned char *readp;    //High Word:Read offset - Low Word: Write Offset
    unsigned char *writep;
    unsigned char *endp;
    unsigned char flag_found;
} GUT_BUFFER;
#endif

#if GUT_BUFF_MSG
// increases buffer pointer by one and wraps around if necessary
#define INC_BUF_POINTER(buf, p)         \
    p++;                                \
    if (p >= buf->data + buf->max_size) \
        p = buf->data;
#else
// increases buffer pointer by one and wraps around if necessary
#define INC_BUF_POINTER(buf, p) \
    p++;                        \
    if (p == buf->endp)         \
        p = buf->data;
#endif

/* TIM IT enable */
#if 0
//SKS Interrupt Buffer
typedef struct _GUT_INT_BUFFER
{
    char bAvailable; //Data in ptrOutput is available;
    GUT_BUFFER *ptrInput;
    GUT_BUFFER *MasterBuff;
    int mode;
    unsigned long ulUpdateTime; //Update time, base on TickCount;
    unsigned long ulProcessTime; //Process Time ulTick100
}
GUT_INT_BUFFER;
#endif

void buff_alloc(GUT_BUFFER *buff, size_t size);
void buff_free(GUT_BUFFER *buff);

#if !GUT_BUFF_MSG
GUT_BUFFER *buff_new(size_t size);
void buff_del(GUT_BUFFER *buff);
#endif

void buff_init_static(GUT_BUFFER *buff, void *data, size_t max);
void buff_clean_static(GUT_BUFFER *buff);

#if GUT_BUFF_MSG
#define buffer_length(buf) ((buf->readp > buf->writep) ? (buf->max_size - (buf->readp - buf->writep)) : (buf->writep - buf->readp))
#define buffer_free(buf) ((buf->readp > buf->writep) ? (buf->readp - buf->writep) : (buf->max_size - (buf->writep - buf->readp)))
#else
#define buffer_length(buf) ((buf->readp > buf->writep) ? (buf->max_size - (buf->readp - buf->writep)) : (buf->writep - buf->readp))
#define buffer_free(buf) ((buf->readp > buf->writep) ? (buf->readp - buf->writep) : (buf->max_size - (buf->writep - buf->readp)))
#endif
int buff_add(GUT_BUFFER *buff, char c);
int buff_append(GUT_BUFFER *dest, GUT_BUFFER *src);
int buff_roll(GUT_BUFFER *buff, char c); //Roll to next occurs of c
size_t buff_remove(GUT_BUFFER *buff, size_t size);
void buff_clear(GUT_BUFFER *buff);
GUT_BUFFER *buff_copy(GUT_BUFFER *buff);

int buff_write(GUT_BUFFER *buf, const char *input, int count);
GUT_FRAME *buffer_get_frame(GUT_BUFFER *buf);
void destroy_frame(GUT_FRAME *frame);
#endif /* __GUT_BUFFER__ */
