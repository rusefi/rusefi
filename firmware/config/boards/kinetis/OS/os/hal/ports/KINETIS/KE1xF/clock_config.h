#ifndef KINETIS_CLOCK_CONFIG_H_
#define KINETIS_CLOCK_CONFIG_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @brief Kinetis Clock Type. */
typedef enum {
	kinetis_clock_default_firc = 0,
    kinetis_clock_int_osc_run,	// internal oscillator, normal-speed "run" mode
    kinetis_clock_int_osc_hs,	// internal oscillator, high-speed mode
    kinetis_clock_ext_osc_hs,	// external oscillator
} kinetis_clock_type_e;

/*!
 * @brief This function executes the configuration of clocks.
 *
 */
void ke1xf_clock_init(kinetis_clock_type_e clockType);

kinetis_clock_type_e ke1xf_clock_get_current_type(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* KINETIS_CLOCK_CONFIG_H_ */
