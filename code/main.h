/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file main.h
* @brief Header of main.c
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Vasil Milev
* @version           1.0.0
* @date              2018.07.04
*
* @copyright         (C) Copyright Endurosat
*
*                    Contents and presentations are protected world-wide.
*                    Any kind of using, copying etc. is prohibited without prior permission.
*                    All rights - incl. industrial property rights - are reserved.
*
* @history
* @revision{         1.0.0  , 2018.07.04, author Vasil Milev, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef MAIN_H
#define MAIN_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "stm32f4xx_hal.h"
#include <stdio.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#if !defined(__FILE_defined)
//typedef __FILE FILE;
# define __FILE_defined
#endif

/* USARTs related definitions */
#define UART_BUFFER_SIZE    256
#define LINE_BUFFER_SIZE UART_BUFFER_SIZE
/* End of USARTs related definitions */

/* LED related definitions */
#define GREEN_LED_ON() GPIOC->ODR |= 0x0010;
#define GREEN_LED_OFF() GPIOC->ODR &= ~0x0010;
#define GREEN_LED_XOR() GPIOC->ODR ^= 0x0010;
#define AMBER_LED_ON() GPIOC->ODR |= 0x0020;
#define AMBER_LED_OFF() GPIOC->ODR &= ~0x0020;
#define AMBER_LED_XOR() GPIOC->ODR ^= 0x0020;
/* End of LED related definitions */

/* ESTTC related definitions */
#define EPS_I2C_ADDRESS         0x18
#define EPS_OUT_PAR_NUM         19
#define EPS_MAX_PAR_NUM         (48+1)
#define ANT_I2C_ADDRESS         0x33
#define UHF_I2C_ADDRESS         0x22
#define UHF_MAX_PAR_NUM         8
#define OBC_I2C_ADDRESS         0x11
#define S_X_BAND_ADDRESS        0x40
/* End of ESTTC related definitions */

/* Bootloader related definitions */
#define APPL_ADDRESS            0x08040000
#define BOOT_ADDRESS            0x08000000
#define RTC_INIT_ADDRESS        0x40002850
#define MAILBOX_ADDRESS         0x40002854
#define MAILBOX_VAL_BOOT        0xB007B007
#define MAILBOX_VAL_APPL        0xA552A552
#define MAILBOX_VAL_HARD        0x0BAD0BAD

#define FLASH_BLANK             0xFF
#define FLASH_SECTOR_SIZE       0x20000
#define FLASH_MIN_SECTOR        6
#define FLASH_MAX_SECTOR        23
#define FLASH_MIN_ADDR          APPL_ADDRESS
#define FLASH_MAX_ADDR          0x08200000          // Last physical flash address
#define FLASH_BLOCK_SIZE        (128*1024)          // Size of one physical block from the flash
#define FLASH_SIZE              (FLASH_MAX_ADDR - FLASH_MIN_ADDR - FLASH_BLOCK_SIZE - sizeof(FlashInfoStruct))
#define FLASH_INFO_ADDRESS      (FLASH_MIN_ADDR + FLASH_SIZE)
#define RAM_MIN_ADDRESS         (0x20000000)
#define RAM_SIZE                (196603)
#define RAM_MAX_ADDRESS         (RAM_MIN_ADDRESS + RAM_SIZE)

/* End of Bootloader related definitions */

/* EPS related definitions */
#define BATT_LOW_3_5V                   1489
#define BATT_LOW_3_6V                   1531
#define BATT_LOW_3_7V                   1574
#define BATT_LOW_3_75V                  ((BATT_LOW_3_7V+BATT_LOW_3_8V)/2)
#define BATT_LOW_3_8V                   1617
#define BATT_LOW_3_85V                  ((BATT_LOW_3_8V+BATT_LOW_3_9V)/2)
#define BATT_LOW_3_9V                   1659
#define BATT_LOW_4_0V                   1702
#define BATT_LOW_4_1V                   1744

#define BATT_CURRENT_250mA              83
#define BATT_CURRENT_500mA              167
#define BATT_CURRENT_1000mA             333
/* End of EPS related definitions */

/* Gyroscope related definitions */
#define POWER_ON_GYRO_1  { HAL_GPIO_WritePin(GPIOB, PAN1_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_2  { HAL_GPIO_WritePin(GPIOB, PAN2_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_3  { HAL_GPIO_WritePin(GPIOB, PAN3_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_4  {  HAL_GPIO_WritePin(PAN4_VGY_GPIO_Port, PAN4_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_5  {  HAL_GPIO_WritePin(GPIOB, PAN5_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_6  {  HAL_GPIO_WritePin(GPIOB, PAN6_VGY_Pin, GPIO_PIN_RESET);  }

#define POWER_OFF_GYRO_1  { HAL_GPIO_WritePin(GPIOB, PAN1_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_2  { HAL_GPIO_WritePin(GPIOB, PAN2_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_3  { HAL_GPIO_WritePin(GPIOB, PAN3_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_4  {  HAL_GPIO_WritePin(PAN4_VGY_GPIO_Port, PAN4_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_5  {  HAL_GPIO_WritePin(GPIOB, PAN5_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_6  {  HAL_GPIO_WritePin(GPIOB, PAN6_VGY_Pin, GPIO_PIN_SET);  }
/* End of Gyroscope related definitions */

/* Accelerometer related definitions */
#define POWER_ON_ACC_1  { HAL_GPIO_WritePin(GPIOH, ACC1_EN_Pin, GPIO_PIN_RESET);  }
#define POWER_OFF_ACC_1  { HAL_GPIO_WritePin(GPIOH, ACC1_EN_Pin, GPIO_PIN_SET);  }
#define POWER_ON_ACC_2  { HAL_GPIO_WritePin(GPIOH, ACC2_EN_Pin, GPIO_PIN_RESET);  }
#define POWER_OFF_ACC_2  { HAL_GPIO_WritePin(GPIOH, ACC2_EN_Pin, GPIO_PIN_SET);  }
/* End of Accelerometer related definitions */

/* UHF related definitions */
  // Turn ON UHF
#define POWER_ON_UHF_1  { HAL_GPIO_WritePin(OBC_OUT5_GPIO_Port, OBC_OUT5_Pin, GPIO_PIN_SET); }
#define POWER_ON_UHF_2  { HAL_GPIO_WritePin(OBC_OUT6_GPIO_Port, OBC_OUT6_Pin, GPIO_PIN_RESET); }

  // Turn OFF UHF
#define POWER_OFF_UHF_1  { HAL_GPIO_WritePin(OBC_OUT5_GPIO_Port, OBC_OUT5_Pin, GPIO_PIN_RESET); }
#define POWER_OFF_UHF_2  { HAL_GPIO_WritePin(OBC_OUT6_GPIO_Port, OBC_OUT6_Pin, GPIO_PIN_SET); }
/* End of UHF related definitions */

#define	__disable_interrupt()	__disable_irq()
#define	__enable_interrupt()	__enable_irq()

#define SERVICE_TASK_PERIOD    (1000)
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No external types declared */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern FILE *COMM;
extern FILE *SYSCON;
extern FILE *PAYLOAD;
extern FILE *COM_SBAND;

extern uint32_t calendar_format;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void Error_Handler(void);

#endif    /* MAIN_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
