/*
 * @file kill_for_coverity.c
 *
 * See http://rusefi.com/forum/viewtopic.php?f=5&t=655&p=10491#p10491
 *
 * @date Feb 25, 2015
 */

void special_abort(const char msg) {
   __coverity_panic__();
}
