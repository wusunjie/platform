#include "portsense.h"
#include "ringbuffer.h"
#include "timer.h"

enum {
	ACC_SENSE,
};

struct blink_status {
	unsigned char sent;
	unsigned char read_count;
	unsigned char buf[2];
	struct timer_ctrl *timer;
};

static struct psense_param params = {
	ACC_SENSE,
	10,
	5,
	50
};

static unsigned char read_buffer[100];
static unsigned char write_buffer[100];
static struct ringbuffer read_buf;
static struct ringbuffer write_buf;

static int port_read_func_cb(unsigned int tag);

int port_read_func_cb(unsigned int tag)
{
	switch (tag) {
		case ACC_SENSE:
		{
			/* read the port */
			return 1;
		}
		break;
		default:
		break;
	}
	return 0;
}

void sw_init(void)
{
	timer_init();
	psense_init(&params, 1, port_read_func_cb);
	ringbuffer_init(&read_buf, read_buffer, 100);
	ringbuffer_init(&write_buf, write_buffer, 100);
}

void hw_init(void)
{
	/* setup_timer(); */
}

void blink_init(struct blink_status *status)
{
	status->sent = status->read_count = status->timer = 0;
	status->buf[0] = status->buf[1] = 0;
}

void blink_fsm(struct blink_status *status)
{
	if (PSENSE_STATUS_LONG == psense_read(ACC_SENSE)) {
		ringbuffer_write(&write_buf, (unsigned char *)"Hello World", 11);
		status->sent = 1;
	}
	if (status->timer) {
		if (TIMER_STATUS_FIRED == timer_check(status->timer)) {
			/* blink finished */
			timer_destory(status->timer);
		}
	}
	else {
		if (status->sent) {
			status->read_count += ringbuffer_read(&read_buf, status->buf + status->read_count, 2);
			if (2 == status->read_count) {
				status->read_count = 0;
				if ((status->buf[0] == 'O') && (status->buf[1] == 'K')) {
					status->timer = timer_create();
					timer_start(status->timer, 0, 30, 0);
				}
			}
		}
	}
}

int main(void)
{
	struct blink_status status;
	
	sw_init();
	hw_init();

	blink_init(&status);
	
	while (1) {
		blink_fsm(&status);
	}
	return 0;
}

void timer_isr(void)
{
	psense_dowork();
	timer_dowork();
}

void serial_isr(void)
{
	unsigned char data = 0; 
	if (0) { /* this is read event */
		data = 0; /* read from register */
		ringbuffer_write(&read_buf, &data, 1);
	}
	else {
		ringbuffer_read(&write_buf, &data, 1);
		/* send data here */
	}
}








