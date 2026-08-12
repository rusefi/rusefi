/**
 * @file netdev.h
 *
 * @date Apr 17, 2026
 * @author Andrey Gusakov, 2026
 */

#ifndef __NETDEV_H__
#define __NETDEV_H__

void         netdev_init(void);
void         netdev_init_mac(void);
void         netdev_get_mac(unsigned char * mac_addr);
unsigned int netdev_read(void);
void         netdev_send(void);

#endif /* __NETDEV_H__ */
