/**
 * @file    startup_ke1xf.S
 * @brief   A startup file for Kinetis KE1XF series.
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#if !defined(__DOXYGEN__)

                .syntax unified
                .cpu    cortex-m3
                .fpu    softvfp

                .thumb
                .text

                .align  2
                .thumb_func

        		.global       Reset_Handler
Reset_Handler:
				/* Errata #e10536 workaround for Kinetis MKE1XF:
				 * "WDOG: After getting RCS assertion by polling, 4 LPO clock-time delay is the minimum requirement before the next block"
				 * https://www.nxp.com/docs/en/errata/Kinetis_E_0N79P.pdf
				 * https://community.nxp.com/thread/460264
				 */
				mov.w	r3, #100
wait_for_wdt:
				nop
				subs	r3, #1
				bne.n	wait_for_wdt

				/* Now we can startup normally... */
         		b          _crt0_entry
                         
#endif /* !defined(__DOXYGEN__) */
