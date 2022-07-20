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
// #include <machine/endian.h>
// #include "stm32f4xx.h"
// #include "stm32f4xx_gpio.h"
// #include "stm32f4xx_exti.h"
#include "wizchip_conf.h"
// #include "lwip/pbuf.h"

// #include "spi.h" //!< @todo replace with your SPI header

#include <ucos_ii.h>

typedef struct tag_SPI_DATA_SETUP {
  uint8_t *tx_data;
  uint8_t *rx_data;
  uint16_t size;
} SPI_DATA_SETUP;

typedef struct tag_SPI_TypeDef {

} SPI_TypeDef;


typedef struct tag_GPIO_TypeDef {

} GPIO_TypeDef;

enum BitState
{
  Bit_RESET = 0,
  Bit_SET = 1
};

static void spi_read_write(SPI_TypeDef *spi_dev, SPI_DATA_SETUP *data)
{}

static void GPIO_WriteBit(GPIO_TypeDef *wiz_gpio_port, uint32_t wiz_cs_pin, enum BitState val) {}

SPI_TypeDef* wiz_spi;
GPIO_TypeDef* wiz_gpio_port;
uint32_t wiz_cs_pin;
GPIO_TypeDef* wiz_rst_gpio_port;
uint32_t wiz_rst_pin;

void wiz_lowlevel_setup(void)
{
  /**
   * @note This is example low level init
   * @todo replace with your own init
   */

  // GPIO_InitTypeDef gpioInit;
  // EXTI_InitTypeDef extiInit;
  // NVIC_InitTypeDef NVIC_InitStructure;

  // EXTI_StructInit(&extiInit);
  // GPIO_StructInit(&gpioInit);

  // wiz_spi= SPI2;

  // wiz_cs_pin= GPIO_Pin_13;
  // wiz_gpio_port= GPIOE;

  // wiz_rst_pin= GPIO_Pin_13;
  // wiz_rst_gpio_port= GPIOD;

  // SPI_BspInit(wiz_spi); // Located in spi.h. Replace with your own

  // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, SPL_ENABLE);
  // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, SPL_ENABLE);
  // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, SPL_ENABLE);

  // // CS Pin
  // gpioInit.GPIO_Pin = wiz_cs_pin;
  // gpioInit.GPIO_Mode = GPIO_Mode_OUT;
  // gpioInit.GPIO_Speed = GPIO_Speed_25MHz;
  // gpioInit.GPIO_OType = GPIO_OType_PP;
  // gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  // GPIO_Init(wiz_cs_gpio_port, &gpioInit);

  // GPIO_WriteBit(wiz_cs_gpio_port, wiz_cs_pin, Bit_SET);

  // // RST Pin
  // gpioInit.GPIO_Pin = wiz_rst_pin;
  // gpioInit.GPIO_Mode = GPIO_Mode_OUT;
  // gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  // gpioInit.GPIO_OType = GPIO_OType_PP;
  // gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
  // GPIO_Init(wiz_rst_gpio_port, &gpioInit);

  // GPIO_WriteBit(wiz_rst_gpio_port, wiz_rst_pin, Bit_SET);

  // // INT Pin
  // gpioInit.GPIO_Pin = GPIO_Pin_6;
  // gpioInit.GPIO_Mode = GPIO_Mode_IN;
  // gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  // gpioInit.GPIO_OType = GPIO_OType_PP;
  // gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
  // GPIO_Init(GPIOE, &gpioInit);

  // SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, GPIO_PinSource6);

  // extiInit.EXTI_Line = EXTI_Line6;
  // extiInit.EXTI_Mode = EXTI_Mode_Interrupt;
  // extiInit.EXTI_Trigger = EXTI_Trigger_Falling;
  // extiInit.EXTI_LineCmd = SPL_ENABLE;
  // EXTI_Init(&extiInit);

  // EXTI_ClearITPendingBit(EXTI_Line6);
  // NVIC_ClearPendingIRQ(EXTI9_5_IRQn);

  // /* Enable Ethernet global Interrupt */
  // NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelCmd = SPL_ENABLE;
  // NVIC_Init(&NVIC_InitStructure);
}

#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
OS_CPU_SR  cpu_sr = 0;
#endif

void    wizchip_cris_enter(void)           {
  OS_ENTER_CRITICAL();
}

void    wizchip_cris_exit(void)          {
  OS_EXIT_CRITICAL();
}

void  wizchip_cs_select(void)            {
  GPIO_WriteBit(wiz_gpio_port, wiz_cs_pin, Bit_RESET);
}

void  wizchip_cs_deselect(void)          {
  GPIO_WriteBit(wiz_gpio_port, wiz_cs_pin, Bit_SET);
}

void  wizchip_rst_assert(void)            {
    GPIO_WriteBit(wiz_rst_gpio_port, wiz_rst_pin, Bit_RESET);
}

void  wizchip_rst_deassert(void)          {
    GPIO_WriteBit(wiz_rst_gpio_port, wiz_rst_pin, Bit_SET);
}


void wiz_hwReset(void) {
    uint32_t t= 168000;
    wizchip_rst_assert();
    while(--t);
    wizchip_rst_deassert();
    t= 168000;
    while(--t);
}

uint8_t wizchip_spi_readbyte(void)        {
  SPI_DATA_SETUP  data;
  uint8_t rx;

  data.tx_data  = NULL;
  data.rx_data  = &rx;
  data.size     = 1;
  spi_read_write(wiz_spi, &data);
  return rx;
}

void  wizchip_spi_writebyte(uint8_t wb) {
  SPI_DATA_SETUP  data;

  data.tx_data  = &wb;
  data.rx_data  = NULL;
  data.size     = 1;
  spi_read_write(wiz_spi, &data);
}

void  wizchip_spi_readburst(uint8_t* pBuf, uint16_t len)  {
  SPI_DATA_SETUP  data;

  data.tx_data  = NULL;
  data.rx_data  = pBuf;
  data.size     = len;
  spi_read_write(wiz_spi, &data);
}

void  wizchip_spi_writeburst(uint8_t* pBuf, uint16_t len) {
  SPI_DATA_SETUP  data;

  data.tx_data  = pBuf;
  data.rx_data  = NULL;
  data.size     = len;
  spi_read_write(wiz_spi, &data);
}
