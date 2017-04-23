#include "timer.h"

#include "config.h"

struct timer_ctrl {
	unsigned int delay;
	unsigned int period;
	unsigned char repeat;
	unsigned char used;
	unsigned int current;
};

static volatile unsigned int timer_jiffies = 0;
static struct timer_ctrl timerlist[TIMER_SIZE_MAX];

void timer_init(void)
{
	for (int i = 0; i < TIMER_SIZE_MAX; i++) {
		timerlist[i].delay = 0;
		timerlist[i].period = 0;
		timerlist[i].repeat = 0;
		timerlist[i].used = 0;
		timerlist[i].current = 0;
	}
	timer_jiffies = 0;
}

void timer_dowork(void)
{
	timer_jiffies++;
}

struct timer_ctrl *timer_create(void)
{
	for (int i = 0; i < TIMER_SIZE_MAX; i++) {
		if (!timerlist[i].used) {
			timerlist[i].delay = 0;
			timerlist[i].period = 0;
			timerlist[i].repeat = 0;
			timerlist[i].used = 1;
			timerlist[i].current = 0;
			return timerlist + i;
		}
	}
	return 0;
}

int timer_start(struct timer_ctrl *ctrl, unsigned int delay, unsigned int period, unsigned char repeat)
{
	if (!ctrl) {
		return -1;
	}
	if (!ctrl->used) {
		return -1;
	}
	if (ctrl->current) {
		return -1;
	}
	ctrl->delay = delay;
	ctrl->period = period;
	ctrl->repeat = repeat;
	ctrl->used = 0;
	ctrl->current = timer_jiffies;
	return 0;
}

enum timer_status timer_check(struct timer_ctrl *ctrl)
{
	if (!ctrl || !ctrl->used || !ctrl->current) {
		return TIMER_STATUS_NOT_RUNNING;
	}
	if (timer_jiffies < ctrl->current + ctrl->delay) {
		return TIMER_STATUS_DELAY;
	}
	if (timer_jiffies < ctrl->current + ctrl->delay + ctrl->period) {
		return TIMER_STATUS_RUNNING;
	}
	if (ctrl->repeat) {
		ctrl->delay = 0;
		ctrl->current = timer_jiffies;
	}
	else {
		ctrl->used = 0;
	}
	return TIMER_STATUS_FIRED;
}

void timer_stop(struct timer_ctrl *ctrl)
{
	if (ctrl) {
		ctrl->current = 0;
	}
}

void timer_destory(struct timer_ctrl *ctrl)
{
	if (ctrl) {
		ctrl->used = 0;
	}
}