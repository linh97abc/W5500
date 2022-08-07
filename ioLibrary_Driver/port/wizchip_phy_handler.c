#include "wizchip_conf.h"

#ifdef CONFIG_WIZCHIP_PHY_LINK_CHECK

static void *wizchip_link_arg;
static void (*wizchip_linkup_cb)(void *);
static void (*wizchip_linkdown_cb)(void *);

void reg_wizchip_linkupdown_cbfunc(void (*linkup_cb)(void *), void (*linkdown_cb)(void *), void *arg)
{
    wizchip_linkup_cb = linkup_cb;
    wizchip_linkdown_cb = linkdown_cb;
    wizchip_link_arg = arg;
}

void task_wizchip_phy_link()
{
    uint8_t phyReg;
    static uint8_t phyRegPre = PHY_LINK_OFF;

    while (1)
    {
        phyReg = wizphy_getphylink();

        if ((phyReg != PHY_LINK_ON) && (phyRegPre == PHY_LINK_ON))
        {
            if (wizchip_linkdown_cb)
            {
                wizchip_linkdown_cb(wizchip_link_arg);
            }

            phyRegPre = PHY_LINK_OFF;
        }
        else if ((phyReg == PHY_LINK_ON) && (phyRegPre == PHY_LINK_OFF))
        {
            if (wizchip_linkup_cb)
            {
                wizchip_linkup_cb(wizchip_link_arg);
            }

            phyRegPre = PHY_LINK_ON;
        }
    }
}
#endif
