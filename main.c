#include <stdio.h>
#include <stdint.h>

#include "wizchip_conf.h"
#include <socket.h>
#include "ioLibrary_Driver/port/wizchip_port.h"

#include <ucos_ii.h>
#include <config_prio.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>

/* Buffer */
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)

/* Socket */
#define SOCKET_LOOPBACK 0

/* Port */
#define PORT_LOOPBACK 5000

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */
/* Network */
static wiz_NetInfo g_net_info =
    {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
        .ip = {169, 254, 151, 140},
        .sn = {255, 255, 255, 0}, // Subnet Mask
        .gw = {192, 168, 22, 1},  // Gateway
        .dns = {8, 8, 8, 8},      // DNS server
        .dhcp = NETINFO_STATIC    // DHCP enable/disable
};

// static uint8_t sFTP_destip[4] = {192, 168, 88, 203}; // For FTP client examples; destination network info
static uint8_t sFTP_destip[4] = {169, 254, 151, 138}; // For FTP client examples; destination network info
static uint16_t sFTP_destport = 65432;                // For FTP client examples; destination network info

#define VMI_SOCKET 0

// void net_link_up_cb()
//{
//
// }
//
// void net_link_down_cb()
//{
//
// }

int32_t send_missile_id_to_server(uint8_t missile_id1, uint8_t missile_id2)
{
    int32_t err;

    err = socket(VMI_SOCKET, Sn_MR_TCP, sFTP_destport, 0);

    if (err != VMI_SOCKET)
    {
        printf("Can not open socket, err: %ld\n", err);
        return err;
    }

    err = connect(VMI_SOCKET, sFTP_destip, sFTP_destport);

    if (err != SOCK_OK)
    {
        printf("connect error, err: %ld\n", err);

        err = close(VMI_SOCKET);

        if (err != SOCK_OK)
        {
            printf("Can not close socket, err: %ld\n", err);
        }

        return err;
    }

    uint8_t missile_id[2] = {missile_id1, missile_id2};

    err = send(VMI_SOCKET, (uint8_t *)missile_id, 2);

    if (err != 2)
    {
        printf("Can not send packet, err: %ld\n", err);
    }

    disconnect(VMI_SOCKET);
    if (err != SOCK_OK)
    {
        printf("disconnect error, err: %ld\n", err);
    }

    close(VMI_SOCKET);
    if (err != SOCK_OK)
    {
        printf("Can not close socket, err: %ld\n", err);
    }

    return err;
}

void mainTask(void *arg)
{
    (void)arg;

    wizchip_port_init();
    network_initialize(g_net_info);

    /* Get network information */
    print_network_information(g_net_info);

    uint16_t id = 0;
    uint16_t id_pre = id;

    /* Infinite loop */
    while (id == id_pre)
    {
        id = IORD_ALTERA_AVALON_PIO_DATA(PIO_MISSILE_ID_BASE);
        id = ~id;

        OSTimeDlyHMSM(0, 0, 0, 500);
    }

    uint8_t id1, id2;

    id1 = id & 0xFF;
    id2 = id >> 8;
    while (1)
    {
        send_missile_id_to_server(id1, id2);
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

#define MAIN_TASK_STK_SIZE 1024
static OS_STK mainTaskStk[MAIN_TASK_STK_SIZE];

int main()
{
    OSTaskCreateExt(mainTask,
                    NULL,
                    &mainTaskStk[MAIN_TASK_STK_SIZE - 1],
                    MAIN_TASK_PRIO,
                    MAIN_TASK_PRIO,
                    mainTaskStk,
                    MAIN_TASK_STK_SIZE,
                    NULL,
                    0);

    OSStart();

    return 0;
}
