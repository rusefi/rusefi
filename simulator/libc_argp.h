#include <stdlib.h>
#include <argp.h>

/* Program documentation. */
static char doc[] =
    "FOME Simulator -- https://wiki.fome.tech/";

/* A description of the arguments we accept. */
static char args_doc[] = "";

/* The options we understand. */
static struct argp_option options[] = {
    {"quiet", 'q', 0,         0, "Don't produce verbose output", 0 },
    {"socketcan-device",
                'd', "DEVICE",  0, "SocketCAN DEVICE (default: can0) to use", 0 },
    {"timeout", 't', "SECONDS", 0, "Run for SECONDS and then exit (negative values ignored)", 0 },
    { 0, 0, 0, 0, 0, 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    int quiet;
    char * socketcanDevice;
    int timeout;
};

/* Parse a single option. */
static error_t
parse_opt(int key, char * arg, struct argp_state * state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments * arguments = (struct arguments *)state->input;

    switch (key) {
        case 'd':
            arguments->socketcanDevice = arg;
            break;
        case 'q':
            arguments->quiet = 1;
            break;
        case 't':
            arguments->timeout = atoi(arg);
            break;

        case ARGP_KEY_ARG:
            /* if (state->arg_num >= 0) */ {
                /* Too many arguments. */
                argp_usage(state);
            }
            break;

        case ARGP_KEY_END:
        case ARGP_KEY_NO_ARGS:
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };
