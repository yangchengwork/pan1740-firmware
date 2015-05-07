/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals setup and initialization. 
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include "periph_setup.h"            // periphera configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization
#ifdef CFG_HCI_SPI
#include "spi_hci.h"
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


#if DEVELOPMENT_DEBUG

/**
 ****************************************************************************************
 * @brief GPIO_reservations. Globally reserved GPIOs
 *
 * @return void 
 ****************************************************************************************
*/
void GPIO_reservations(void)
{
#ifdef CFG_HCI_UART	
	RESERVE_GPIO( UART1_TX,  UART1_TX_GPIO_PORT,   UART1_TX_GPIO_PIN,   PID_UART1_TX);
	RESERVE_GPIO( UART1_RX,  UART1_RX_GPIO_PORT,   UART1_RX_GPIO_PIN,   PID_UART1_RX);
	RESERVE_GPIO( UART1_RTS, UART1_RTSN_GPIO_PORT, UART1_RTSN_GPIO_PIN, PID_UART1_RTSN);
	RESERVE_GPIO( UART1_CTS, UART1_CTSN_GPIO_PORT, UART1_CTSN_GPIO_PIN, PID_UART1_CTSN);
#endif	
#ifdef CFG_HCI_SPI		// HCI over SPI pins
	RESERVE_GPIO( SPI_EN,   SPI_CS_PORT,        SPI_CS_PIN,     PID_SPI_EN);
	RESERVE_GPIO( SPI_CLK,  SPI_CLK_PORT,       SPI_CLK_PIN,    PID_SPI_CLK);
	RESERVE_GPIO( SPI_DO,   SPI_DO_PORT,        SPI_DO_PIN,     PID_SPI_DO);
	RESERVE_GPIO( SPI_DI,   SPI_DI_PORT,        SPI_DI_PIN,     PID_SPI_DI);
	RESERVE_GPIO( DREADY,   SPI_DREADY_PORT,    SPI_DREADY_PIN, PID_GPIO);
#endif  // CFG_HCI_SPI

#ifdef CFG_HCI_BOTH_EIF 
	RESERVE_GPIO( HCI_EIF_SELECT, HCI_EIF_SELECT_PORT, HCI_EIF_SELECT_PIN, PID_GPIO);
#endif
	
#ifdef CFG_WKUP_EXT_PROCESSOR	
	// external MCU wakeup GPIO
	RESERVE_GPIO( EXT_WAKEUP_GPIO, EXT_WAKEUP_PORT, EXT_WAKEUP_PIN, PID_GPIO );    
#endif
}

#endif

/**
 ****************************************************************************************
 * @brief Map port pins
 *
 * The Uart and SPI port pins and GPIO ports(for debugging) are mapped
 ****************************************************************************************
 */
void set_pad_functions(void)        // set gpio port function mode
{
#ifdef CFG_HCI_UART
	GPIO_ConfigurePin( UART1_TX_GPIO_PORT,   UART1_TX_GPIO_PIN,   OUTPUT,         PID_UART1_TX, false );
	GPIO_ConfigurePin( UART1_RX_GPIO_PORT,   UART1_RX_GPIO_PIN,   INPUT,          PID_UART1_RX, false );
	GPIO_ConfigurePin( UART1_RTSN_GPIO_PORT, UART1_RTSN_GPIO_PIN, OUTPUT,         PID_UART1_RTSN, false );
	GPIO_ConfigurePin( UART1_CTSN_GPIO_PORT, UART1_CTSN_GPIO_PIN, INPUT_PULLDOWN, PID_UART1_CTSN, false );
#endif	
#ifdef CFG_HCI_SPI		// HCI over SPI pins
	GPIO_ConfigurePin( SPI_CS_PORT,  SPI_CS_PIN,  INPUT_PULLUP, PID_SPI_EN, true );
	GPIO_ConfigurePin( SPI_CLK_PORT, SPI_CLK_PIN, INPUT_PULLUP, PID_SPI_CLK, false );
	GPIO_ConfigurePin( SPI_DO_PORT,  SPI_DO_PIN,  OUTPUT,       PID_SPI_DO, false );	
	GPIO_ConfigurePin( SPI_DI_PORT,  SPI_DI_PIN,  INPUT_PULLUP, PID_SPI_DI, false );
	GPIO_ConfigurePin( SPI_DREADY_PORT,  SPI_DREADY_PIN,  OUTPUT,       PID_GPIO, false );
#endif  // CFG_HCI_SPI

#ifdef CFG_HCI_BOTH_EIF 
	GPIO_ConfigurePin( HCI_EIF_SELECT_PORT, HCI_EIF_SELECT_PIN, INPUT, PID_GPIO, true );
#endif
	
#ifdef CFG_WKUP_EXT_PROCESSOR		
	// external MCU wakeup GPIO
	GPIO_ConfigurePin( EXT_WAKEUP_PORT, EXT_WAKEUP_PIN, OUTPUT, PID_GPIO, false ); // initialized to low
#endif
	
#if 0	
	SetBits32(BLE_DIAGCNTL_REG, DIAG0, 0x08);
	SetBits32(BLE_DIAGCNTL_REG, DIAG1, 0x08);
	SetBits32(BLE_DIAGCNTL_REG, DIAG2, 0x08);
	SetBits32(BLE_DIAGCNTL_REG, DIAG3, 0x08);

	SetBits32(BLE_DIAGCNTL_REG, DIAG0_EN, 1);
	SetBits32(BLE_DIAGCNTL_REG, DIAG1_EN, 1);  
	SetBits32(BLE_DIAGCNTL_REG, DIAG2_EN, 1);  
	SetBits32(BLE_DIAGCNTL_REG, DIAG3_EN, 1);
#endif
}


/**
 ****************************************************************************************
 * @brief Enable pad's and peripheral clocks assuming that peripherals' power domain is down.
 *
 * The Uart and SPi clocks are set. 
 ****************************************************************************************
 */
void periph_init(void)  // set i2c, spi, uart, uart2 serial clks
{
	// Power up peripherals' power domain
	SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
	while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP)) ; 
    
#if ES4_CODE
	SetBits16(CLK_16M_REG,XTAL16_BIAS_SH_DISABLE, 1);
#endif    
	
	// TODO: Application specific - Modify accordingly!
	// Example: Activate UART and SPI.
	
	// Initialize UART component
#ifdef CFG_HCI_UART
	SetBits16(CLK_PER_REG, UART1_ENABLE, 1);    // enable clock - always @16MHz
	
	// baudr=9-> 115k2
	// mode=3-> no parity, 1 stop bit 8 data length
#ifdef UART_MEGABIT
	uart_init(UART_BAUDRATE_1M, 3);
#else
	uart_init(UART_BAUDRATE_115K2, 3);
#endif // UART_MEGABIT
#endif	

	// HCI over SPI driver
#ifdef CFG_HCI_SPI
	spi_hci_slave_init();
#endif
	//rom patch
	patch_func();

	//Init pads
	set_pad_functions();

	// Enable the pads
	SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}
