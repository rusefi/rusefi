#define THD_WORKING_AREA(name, size) uint8_t name[size];
#define NORMALPRIO 1
#define CH_CFG_ST_FREQUENCY 1000000

typedef void (*tfunc_t)(void *p);
typedef int tprio_t;
typedef uint32_t systime_t;
typedef uint32_t sysinterval_t;

class thread_t {
public:
	const char *name;
	tfunc_t           funcp;
	void              *arg;

	unsigned char threadId;
};

bool chThdShouldTerminateX(void);
systime_t chVTGetSystemTime(void);
systime_t chThdSleepUntilWindowed(systime_t prev, systime_t next);

thread_t *chThdCreateStatic(void *wsp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg);
static inline systime_t chTimeAddX(systime_t systime,
                                   sysinterval_t interval) {
  return systime + (systime_t)interval;
}

#define PAL_MODE_OUTPUT_PUSHPULL 0

namespace chibios_rt {
	struct ThreadReference {
		void requestTerminate() { }
		void wait() { }
	};
}

namespace chibios_rt {
	template <int N>
	struct BaseStaticThread {
		void start(tprio_t) { }
		virtual void main() = 0;
		void setName(const char*) { }
	};
}
