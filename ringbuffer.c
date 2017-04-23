#include "ringbuffer.h"

static int ringbuffer_full(struct ringbuffer *buffer);
static int ringbuffer_empty(struct ringbuffer *buffer);
static int ringbuffer_push(struct ringbuffer *buffer, unsigned char data);
static int ringbuffer_pop(struct ringbuffer *buffer, unsigned char *data);

int ringbuffer_full(struct ringbuffer *buffer)
{
	return ((buffer->tail + 1) % buffer->size) == buffer->head;
}

int ringbuffer_empty(struct ringbuffer *buffer)
{
	return buffer->tail == buffer->head;
}

int ringbuffer_push(struct ringbuffer *buffer, unsigned char data)
{
	if (!ringbuffer_full(buffer)) {
		buffer->buf[buffer->tail] = data;
		buffer->tail = (buffer->tail + 1) % buffer->size;
		return 0;
	}
	return -1;
}

int ringbuffer_pop(struct ringbuffer *buffer, unsigned char *data)
{
	if (!ringbuffer_empty(buffer)) {
		*data = buffer->buf[buffer->head];
		buffer->head = (buffer->head + 1) % buffer->size;
		return 0;
	}
	return -1;
}

void ringbuffer_init(struct ringbuffer *buffer, unsigned char *b, unsigned int size)
{
	buffer->buf = b;
	buffer->size = size;
	buffer->head = buffer->tail = 0;
}

unsigned int ringbuffer_write(struct ringbuffer *buffer, unsigned char *b, unsigned int len)
{
	unsigned int l = 0;
	for (int i = 0; i < len; i++) {
		if (!ringbuffer_push(buffer, b[i])) {
			l++;
		}
		else {
			break;
		}
	}
	return l;
}

unsigned int ringbuffer_read(struct ringbuffer *buffer, unsigned char *b, unsigned int len)
{
	unsigned int l = 0;
	for (int i = 0; i < len; i++) {
		if (!ringbuffer_pop(buffer, &b[i])) {
			l++;
		}
		else {
			break;
		}
	}
	return l;
}
