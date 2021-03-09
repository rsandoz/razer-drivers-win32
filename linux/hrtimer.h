#include <linux/kernel.h>

#define 	CLOCK_MONOTONIC   1

struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node* rb_right;
	struct rb_node* rb_left;
};
/* The alignment might seem pointless, but allegedly CRIS needs it */

union ktime {
	s64	tv64;
#if BITS_PER_LONG != 64 && !defined(CONFIG_KTIME_SCALAR)
	struct {
# ifdef __BIG_ENDIAN
		s32	sec, nsec;
# else
		s32	nsec, sec;
# endif
	} tv;
#endif
};

typedef union ktime ktime_t;		/* Kill this */

struct timerqueue_node {
	struct rb_node node;
	ktime_t expires;
};

struct hrtimer {
	struct timerqueue_node		node;
	ktime_t				_softexpires;
	enum hrtimer_restart(*function)(struct hrtimer*);
	struct hrtimer_clock_base* base;
	u8				state;
	u8				is_rel;
	u8				is_soft;
	u8				is_hard;
};

enum hrtimer_restart {
	HRTIMER_NORESTART,	/* Timer is not restarted */
	HRTIMER_RESTART,	/* Timer must be restarted */
};

enum hrtimer_mode {
	HRTIMER_MODE_ABS = 0x00,
	HRTIMER_MODE_REL = 0x01,
	HRTIMER_MODE_PINNED = 0x02,
	HRTIMER_MODE_SOFT = 0x04,
	HRTIMER_MODE_HARD = 0x08,

	HRTIMER_MODE_ABS_PINNED = HRTIMER_MODE_ABS | HRTIMER_MODE_PINNED,
	HRTIMER_MODE_REL_PINNED = HRTIMER_MODE_REL | HRTIMER_MODE_PINNED,

	HRTIMER_MODE_ABS_SOFT = HRTIMER_MODE_ABS | HRTIMER_MODE_SOFT,
	HRTIMER_MODE_REL_SOFT = HRTIMER_MODE_REL | HRTIMER_MODE_SOFT,

	HRTIMER_MODE_ABS_PINNED_SOFT = HRTIMER_MODE_ABS_PINNED | HRTIMER_MODE_SOFT,
	HRTIMER_MODE_REL_PINNED_SOFT = HRTIMER_MODE_REL_PINNED | HRTIMER_MODE_SOFT,

	HRTIMER_MODE_ABS_HARD = HRTIMER_MODE_ABS | HRTIMER_MODE_HARD,
	HRTIMER_MODE_REL_HARD = HRTIMER_MODE_REL | HRTIMER_MODE_HARD,

	HRTIMER_MODE_ABS_PINNED_HARD = HRTIMER_MODE_ABS_PINNED | HRTIMER_MODE_HARD,
	HRTIMER_MODE_REL_PINNED_HARD = HRTIMER_MODE_REL_PINNED | HRTIMER_MODE_HARD,
};

inline u64 hrtimer_forward_now(struct hrtimer* timer, ktime_t interval)
{
	return 0;
}

static inline ktime_t ms_to_ktime(u64 ms)
{
	static const ktime_t ktime_zero = { .tv64 = 0 };

	return ktime_zero;
}

inline void hrtimer_start_range_ns(struct hrtimer* timer,
	ktime_t tim,
	u64 delta_ns,
	const enum hrtimer_mode mode)
{

}

inline int hrtimer_cancel(struct hrtimer* timer)
{
	return 0;
}

void hrtimer_init(struct hrtimer* timer, int which_clock,
	enum hrtimer_mode mode)
{

}