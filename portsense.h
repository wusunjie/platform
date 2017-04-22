#ifndef _PORTSENSE_H
#define _PORTSENSE_H

#ifdef __cplusplus
extern "C" {
#endif

enum psense_status {
	PSENSE_STATUS_UP,
	PSENSE_STATUS_DOWN,
	PSENSE_STATUS_LONG,
};

struct psense_param {
	unsigned int tag;
	unsigned int prev_filter;
	unsigned int post_filter;
	unsigned int long_press;
};

typedef int (*port_read_func)(unsigned int tag);

extern int psense_init(struct psense_param *param, int size, port_read_func func);

extern void psense_dowork(void);

extern int psense_read(unsigned int tag);

#ifdef __cplusplus
}
#endif

#endif