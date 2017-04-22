struct psense_param {
	unsigned int prev_filter;
	unsigned int post_filter;
	unsigned char tag;
};

typedef int (*port_read_func)(unsigned char tag);

extern int psense_init(struct psense_param *param, int size, port_read_func func);

extern void psense_tick(void);

extern int psense_read(unsigned char tag);