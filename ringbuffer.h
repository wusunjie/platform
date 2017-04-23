#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif


struct ringbuffer {
	unsigned char *buf;
	unsigned int size;
	unsigned int head;
	unsigned int tail;
};

extern void ringbuffer_init(struct ringbuffer *buffer, unsigned char *b, unsigned int size);

extern unsigned int ringbuffer_write(struct ringbuffer *buffer, unsigned char *b, unsigned int len);

extern unsigned int ringbuffer_read(struct ringbuffer *buffer, unsigned char *b, unsigned int len);


#ifdef __cplusplus
}
#endif

#endif