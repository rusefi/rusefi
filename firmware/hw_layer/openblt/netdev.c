/**
 * @file netdev.c
 *
 * @date Apr 17, 2026
 * @author Andrey Gusakov, 2026
 */

/*---------------------------------------------------------------------------*/
#include "uip.h"
#include "uip_arp.h"
#include "boot.h"

#include "hal.h"

/*---------------------------------------------------------------------------*/
#define NETDEV_DEFAULT_MACADDR0           (0x08)
#define NETDEV_DEFAULT_MACADDR1           (0x00)
#define NETDEV_DEFAULT_MACADDR2           (0x27)
#define NETDEV_DEFAULT_MACADDR3           (0x69)
#define NETDEV_DEFAULT_MACADDR4           (0x5B)
#define NETDEV_DEFAULT_MACADDR5           (0x45)

/* Timeout time for transmitting a packet. */
#define NETDEV_TX_PACKET_TIMEOUT_MS       (250u)
#define NETDEV_LINK_POLL_INTERVAL_MS      (5u)

/*---------------------------------------------------------------------------*/

static bool current_link_status = false;
static systime_t link_poll_time;
static struct uip_eth_addr macAddress;

/*---------------------------------------------------------------------------*/
void netdev_init(void)
{
  /* Store the default MAC address. */
  macAddress.addr[0] = NETDEV_DEFAULT_MACADDR0;
  macAddress.addr[1] = NETDEV_DEFAULT_MACADDR1;
  macAddress.addr[2] = NETDEV_DEFAULT_MACADDR2;
  macAddress.addr[3] = NETDEV_DEFAULT_MACADDR3;
  macAddress.addr[4] = NETDEV_DEFAULT_MACADDR4;
  macAddress.addr[5] = NETDEV_DEFAULT_MACADDR5;

  static const MACConfig mac_config = { macAddress.addr };

  macStart(&ETHD1, &mac_config);

  current_link_status = macPollLinkStatus(&ETHD1);
  link_poll_time = chVTGetSystemTime();
}


/*---------------------------------------------------------------------------*/
void netdev_init_mac(void)
{
  /* Configure the MAC address */
  uip_setethaddr(macAddress);
}


/*---------------------------------------------------------------------------*/
void netdev_get_mac(unsigned char * mac_addr)
{
  mac_addr[0] = macAddress.addr[0];
  mac_addr[1] = macAddress.addr[1];
  mac_addr[2] = macAddress.addr[2];
  mac_addr[3] = macAddress.addr[3];
  mac_addr[4] = macAddress.addr[4];
  mac_addr[5] = macAddress.addr[5];
}


/*---------------------------------------------------------------------------*/
static void netdev_poll_link(void){
  /* poll link change */
  if (chVTTimeElapsedSinceX(link_poll_time) > TIME_MS2I(NETDEV_LINK_POLL_INTERVAL_MS)) {
    current_link_status = macPollLinkStatus(&ETHD1);
    link_poll_time = chVTGetSystemTime();
  }
}


/*---------------------------------------------------------------------------*/
unsigned int netdev_read(void)
{
  MACReceiveDescriptor rd;
  unsigned int result = 0;

  netdev_poll_link();

  if (macWaitReceiveDescriptor(&ETHD1, &rd, TIME_IMMEDIATE) != MSG_OK) {
    return 0;
  }

  result = macReadReceiveDescriptor(&rd, uip_buf, UIP_BUFSIZE);

  macReleaseReceiveDescriptorX(&rd);

  /* Give the result back to the caller. */
  return result;
}


/*---------------------------------------------------------------------------*/
void netdev_send(void)
{
  MACTransmitDescriptor td;

  netdev_poll_link();

  if (!current_link_status) {
    return;
  }

  if (macWaitTransmitDescriptor(&ETHD1, &td, TIME_MS2I(NETDEV_TX_PACKET_TIMEOUT_MS)) != MSG_OK) {
    return;
  }

  macWriteTransmitDescriptor(&td, uip_buf, uip_len);
  macReleaseTransmitDescriptorX(&td);
}
