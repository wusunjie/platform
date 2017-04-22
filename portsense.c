#include "portsense.h"

#include "config.h"

struct psense_ctrl {
	struct psense_param param;
	volatile int status;
	unsigned int count;
};

static port_read_func port_read_func_ptr = 0;
static struct psense_ctrl sense_ctrl[PSENSE_SIZE_MAX];
static int sense_size = 0;

int psense_init(struct psense_param *param, int size, port_read_func func)
{
	if (size > PSENSE_SIZE_MAX) {
		return -1;
	}
	for (int i = 0; i < size; i++) {
		sense_ctrl[i].param = param[i];
		sense_ctrl[i].status = 0;
		sense_ctrl[i].count = 0;
	}
	port_read_func_ptr = func;
	sense_size = size;
	return 0;
}

void psense_dowork(void)
{
	for (int i = 0; i < sense_size; i++) {
		switch (sense_ctrl[i].status) {
			case 0:
			{
				if (port_read_func_ptr) {
					if (port_read_func_ptr(sense_ctrl[i].param.tag)) {
						sense_ctrl[i].count++;
						if (sense_ctrl[i].count > sense_ctrl[i].param.prev_filter) {
							sense_ctrl[i].status = 1;
							sense_ctrl[i].count = 0;
						}
					}
					else {
						sense_ctrl[i].count = 0;
					}
				}
			}
			break;
			case 1:
			{
				if (port_read_func_ptr) {
					if (!port_read_func_ptr(sense_ctrl[i].param.tag)) {
						sense_ctrl[i].count++;
						if (sense_ctrl[i].count > sense_ctrl[i].param.post_filter) {
							sense_ctrl[i].status = 0;
							sense_ctrl[i].count = 0;
						}
					}
					else {
						if (sense_ctrl[i].param.long_press) {
							sense_ctrl[i].count++;
							if (sense_ctrl[i].count > sense_ctrl[i].param.long_press) {
								sense_ctrl[i].status = 2;
								sense_ctrl[i].count = 0;
							}
						}
						else {
							sense_ctrl[i].count = 0;
						}
					}
				}
			}
			break;
			case 2:
			{
				if (port_read_func_ptr) {
					if (!port_read_func_ptr(sense_ctrl[i].param.tag)) {
						sense_ctrl[i].count++;
						if (sense_ctrl[i].count > sense_ctrl[i].param.post_filter) {
							sense_ctrl[i].status = 0;
							sense_ctrl[i].count = 0;
						}
					}
					else {
						sense_ctrl[i].count = 0;
					}
				}
			}
			break;
			default:
			break;
		}
	}
}

int psense_read(unsigned int tag)
{
	for (int i = 0; i < sense_size; i++) {
		if (tag == sense_ctrl[i].param.tag) {
			return (int)sense_ctrl[i].status;
		}
	}
	return -1;
}