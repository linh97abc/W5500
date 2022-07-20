/**
 * @file  wiznet_port.c
 * @brief Wizchip porting layer
 *        Contains wizchip binds to spi api and to pbuf lwip api
 * @note Platform dependent methods from wizchip_conf.c were redeclared here.
 *       Original methods were declared as weak.
 * @date  8 feb 2020 г.
 * @author  Peter Borisenko
 */

#include <stdint.h>
#include <stdlib.h>
#include "wizchip_conf.h"

#include <altera_avalon_pio_regs.h>
#include <altera_avalon_spi.h>
#include <system.h>

#include <ucos_ii.h>

typedef struct tag_SPI_DATA_SETUP
{
  uint8_t *tx_data;
  uint8_t *rx_data;
  uint16_t size;
} SPI_DATA_SETUP;

typedef struct tag_SPI_TypeDef
{
  uint32_t base;
  uint32_t slave;
} SPI_TypeDef;

enum BitState
{
  Bit_RESET = 0,
  Bit_SET = 1
};

static void spi_read_write(SPI_TypeDef *spi_dev, SPI_DATA_SETUP *data)
{
  uint32_t write_len, read_len;

  write_len = (data->tx_data == NULL) ? 0 : data->size;
  read_len = (data->rx_data == NULL) ? 0 : data->size;
  alt_avalon_spi_command(spi_dev->base, spi_dev->slave, write_len, data->tx_data, read_len, data->rx_data, 0);
}

static void GPIO_WriteBit(uint32_t gpio_base, uint32_t pin, enum BitState val)
{
  IOWR_ALTERA_AVALON_PIO_DATA(gpio_base, val << pin);
}

static SPI_TypeDef wiz_spi_inst = {SPI_0_BASE, 0};

SPI_TypeDef *wiz_spi = &wiz_spi_inst;

#define WIZ_GPIO_PORT 0x00000000
#define WIZ_CS_PIN 0
#define WIZ_RST_PIN 1

void wiz_lowlevel_setup(void)
{
  /**
   * @note This is example low level init
   * @todo replace with your own init
   */

  // // CS Pin
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_CS_PIN, Bit_SET);

  // // RST Pin
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_RST_PIN, Bit_SET);
}

#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
OS_CPU_SR cpu_sr = 0;
#endif

void wizchip_cris_enter(void)
{
  OS_ENTER_CRITICAL();
}

void wizchip_cris_exit(void)
{
  OS_EXIT_CRITICAL();
}

void wizchip_cs_select(void)
{
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_CS_PIN, Bit_RESET);
}

void wizchip_cs_deselect(void)
{
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_CS_PIN, Bit_SET);
}

void wizchip_rst_assert(void)
{
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_RST_PIN, Bit_RESET);
}

void wizchip_rst_deassert(void)
{
  GPIO_WriteBit(WIZ_GPIO_PORT, WIZ_RST_PIN, Bit_SET);
}

void wiz_hwReset(void)
{
  uint32_t t = 168000;
  wizchip_rst_assert();
  while (--t)
    ;
  wizchip_rst_deassert();
  t = 168000;
  while (--t)
    ;
}

uint8_t wizchip_spi_readbyte(void)
{
  SPI_DATA_SETUP data;
  uint8_t rx;

  data.tx_data = NULL;
  data.rx_data = &rx;
  data.size = 1;
  spi_read_write(wiz_spi, &data);
  return rx;
}

void wizchip_spi_writebyte(uint8_t wb)
{
  SPI_DATA_SETUP data;

  data.tx_data = &wb;
  data.rx_data = NULL;
  data.size = 1;
  spi_read_write(wiz_spi, &data);
}

void wizchip_spi_readburst(uint8_t *pBuf, uint16_t len)
{
  SPI_DATA_SETUP data;

  data.tx_data = NULL;
  data.rx_data = pBuf;
  data.size = len;
  spi_read_write(wiz_spi, &data);
}

void wizchip_spi_writeburst(uint8_t *pBuf, uint16_t len)
{
  SPI_DATA_SETUP data;

  data.tx_data = pBuf;
  data.rx_data = NULL;
  data.size = len;
  spi_read_write(wiz_spi, &data);
}
