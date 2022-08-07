/**
 * @file  wiznet_port.h
 * @brief Header for wizchip porting layer
 * @date  8 feb 2020 г.
 * @author  Peter Borisenko
 */

#ifndef __WIZCHIP_PORT_H_
#define __WIZCHIP_PORT_H_

#include "wizchip_conf.h"
#include <config_prio.h>

#define WIZ_SPI_BASE SPI_W5X00_BASE

#define WIZ_GPIO_PORT PIO_W5X00_BASE
#define WIZ_CS_PIN 0
#define WIZ_RST_PIN 1

#define WIZCHIP_PHY_LINK_TASK_STK_SIZE 1024

#ifndef WIZCHIP_SPI_MUTEX_PRIO
#error "WIZCHIP_SPI_MUTEX_PRIO is not define"
#endif

void wizchip_port_init(void);
void network_initialize(wiz_NetInfo net_info);
void print_network_information(wiz_NetInfo net_info);

#ifdef CONFIG_WIZCHIP_PHY_LINK_CHECK
void reg_wizchip_linkupdown_cbfunc(void (*linkup_cb)(void *), void (*linkdown_cb)(void *), void *arg);

void task_wizchip_phy_link();
#endif

#endif /* __WIZCHIP_PORT_H_ */
