#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

//#include "cc.h"

#define ETHERNET_MTU		1500

err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);


#endif
