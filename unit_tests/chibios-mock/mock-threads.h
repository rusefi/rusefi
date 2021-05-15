#define THD_WORKING_AREA(name, size) uint8_t name[size];

typedef int tprio_t;

struct thread_t {
	char *name;
};
