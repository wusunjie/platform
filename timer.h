#ifndef _TIMER_H
#define _TIMER_H

#ifdef __cplusplus
extern "C" {
#endif


enum timer_status {
	TIMER_STATUS_NOT_RUNNING,
	TIMER_STATUS_DELAY,
	TIMER_STATUS_RUNNING,
	TIMER_STATUS_FIRED,
};

struct timer_ctrl;

extern void timer_init(void);

extern void timer_dowork(void);

extern struct timer_ctrl *timer_create(void);

extern int timer_start(struct timer_ctrl *ctrl, unsigned int delay, unsigned int period, unsigned char repeat);

extern enum timer_status timer_check(struct timer_ctrl *ctrl);

extern void timer_stop(struct timer_ctrl *ctrl);

extern void timer_destory(struct timer_ctrl *ctrl);

#ifdef __cplusplus
}
#endif

#endif