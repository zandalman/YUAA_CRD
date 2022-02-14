/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file AppTasks.c
* @brief Implementation of common task primitives
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
* @revision{         1.0.1  , 2020.01.16, author Georgi Georgiev, Moved everything, except StartDefaultTask() to DefTasks.c }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "AppTasks.h"
#include "fatfs.h"
#include "ESTTC.h"
#include "TaskMonitor.h"
#include "User_types.h"
#include "LIS3MDL_MAG_driver.h"
#include "DAT_Inputs.h"
#include "Panels.h"
#include "version.h"
#include "S_Band_Trnsm.h"
#include "X_Band_Trnsm.h"
#include  <Svc_RTC.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define APP_LED_ON_TIME             (50)                                /* given time in ms */
#define APP_TASK_CALL_PERIOD        (1000)                              /* given time in ms */
#define CR_BUF_SIZE                 1024
#define CR_BUF_MASK                 0x3FF
#define ADCS_INTERVAL               300000                              /* given time in ms */
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL TYPES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/**
* @brief Cosmic Ray Detector Buffer
*/
uint32_t cr_bfr[CR_BUF_SIZE];
int cr_ntw = 0;
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @brief That is a task used as example to configure all sensors and actuators and blinks the green LED.
* That can be changed freely as needed depending on the project.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @param[input]      argument - not used
* @param[output]     none
* @return            none
* @note              none
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void StartDefaultTask(void const * argument)
{

  //-------------------- sensors init start and print to both communication channels ------------------
  // That is just an example how to initialise all sensors and to print to two USART channels if needed
      fprintf(PAYLOAD,"-----\r");
      fprintf(COMM   ,"-----\r");

      if (Magnitometers_LIS3MDL_Init(LIS3MDL_MAG_I2C_ADDRESS_LOW) == E_OK)
      {
          fprintf(PAYLOAD,"MAG1_INIT_OK\r");
          fprintf(COMM  ,"MAG1_INIT_OK\r");
      }
      else{
          fprintf(PAYLOAD, "  MAG1 fail\r");
          fprintf(COMM  , "  MAG1 fail\r");
      }
      if (Magnitometers_LIS3MDL_Init(LIS3MDL_MAG_I2C_ADDRESS_HIGH) == E_OK)
      {
          fprintf(PAYLOAD,"MAG2_INIT_OK\r");
          fprintf(COMM  ,"MAG2_INIT_OK\r");
      }
      else{
          fprintf(PAYLOAD, "  MAG2 fail\r");
          fprintf(COMM  , "  MAG2 fail\r");
      }

      //Inizialize ACC Sensor 1
      if (AIS328DQ_Init(AIS328DQ_1_MEMS_I2C_ADDRESS) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"ACC1_INIT_OK\r");
        fprintf(COMM  ,"ACC1_INIT_OK\r");
      }
      else{
        I2C_Reset(&hi2c2);
      }

      //Inizialize ACC Sensor 2
      if (AIS328DQ_Init(AIS328DQ_2_MEMS_I2C_ADDRESS) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"ACC2_INIT_OK\r");
        fprintf(COMM  ,"ACC2_INIT_OK\r");
      }
      else{
        I2C_Reset(&hi2c2);
      }

      Panels_Init();

      /* Set PWM of the magnetorquer to 0% (i.e. OFF) */
      if (SetMagnetorque(PAN_X_M, 0, 1) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"TRQ%u_INIT_OK 0%%\r", TRQ_1);
        fprintf(COMM  ,"TRQ%u_INIT_OK 0%%\r", TRQ_1);
      }

      /* Set PWM of the magnetorquer to 0% (i.e. OFF) */
      if (SetMagnetorque(PAN_Y_M, 0, 1) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"TRQ%u_INIT_OK 0%%\r", TRQ_2);
        fprintf(COMM  ,"TRQ%u_INIT_OK 0%%\r", TRQ_2);
      }

      /* Set PWM of the magnetorquer to 0% (i.e. OFF) */
      if (SetMagnetorque(PAN_Z_M, 0, 1) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"TRQ%u_INIT_OK 0%%\r", TRQ_3);
        fprintf(COMM  ,"TRQ%u_INIT_OK 0%%\r", TRQ_3);
      }

      //Inizialize GYR Sensor X
      if (ADIS16265_Init(PAN_X_M) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"GYR%u_INIT_OK\r", GYR_1);
        fprintf(COMM  ,"GYR%u_INIT_OK\r", GYR_1);
      }
      else{
        fprintf(PAYLOAD,"GYR%u_INIT_FAIL\r", GYR_1);
        fprintf(COMM  ,"GYR%u_INIT_FAIL\r", GYR_1);
      }

      //Inizialize GYR Sensor Y
      if (ADIS16265_Init(PAN_Y_M) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"GYR%u_INIT_OK\r", GYR_2);
        fprintf(COMM  ,"GYR%u_INIT_OK\r", GYR_2);
      }
      else{
        fprintf(PAYLOAD,"GYR%u_INIT_FAIL\r", GYR_2);
        fprintf(COMM  ,"GYR%u_INIT_FAIL\r", GYR_2);
      }

      //Inizialize GYR Sensor Z
      if (ADIS16265_Init(PAN_Z_M) == SEN_SUCCESS)
      {
        fprintf(PAYLOAD,"GYR%u_INIT_OK\r", GYR_3);
        fprintf(COMM  ,"GYR%u_INIT_OK\r", GYR_3);
      }
      else{
        fprintf(PAYLOAD,"GYR%u_INIT_FAIL\r", GYR_3);
        fprintf(COMM  ,"GYR%u_INIT_FAIL\r", GYR_3);
      }

      //----------------------------sensors init end
      TaskMonitor_TaskInitialized(TASK_MONITOR_DEFAULT);   /* The task is initialized and is ready */

      for( ; ; )
      {
          TaskMonitor_IamAlive(TASK_MONITOR_DEFAULT); /* Prevent from WatchDog reset */

          /* blink the Green LED for 50ms to indicate the OBC is running */
          GREEN_LED_ON();
          osDelay(APP_LED_ON_TIME);
          GREEN_LED_OFF();

          osDelay(APP_TASK_CALL_PERIOD);    /* Give processing time for the other tasks */
      }

}

void ProcessMags(void const * argument)
{
    Compass_Axis_t MAG_raw;
    osThreadId ID;

    TaskMonitor_TaskInitialized(TASK_MONITOR_DEFAULT);   /* The task is initialized and is ready */

    for( ; ; )
    {
        TaskMonitor_IamAlive(TASK_MONITOR_DEFAULT); /* Prevent from WatchDog reset */

        ID = osThreadGetId();
        if(ID == NULL)
        {
            fprintf(COMM,"NO RUNNING THREAD\r");
        }
        else
        {
            fprintf(COMM,"Running thread ID = %d\r", ID);
        }
        osDelay(3000);

        /*Magnetometer test code */
        if (Magnitometers_LIS3MDL_Read_Data(&MAG_raw, LIS3MDL_MAG_I2C_ADDRESS_LOW) != E_OK) {
          I2C_Reset(&hi2c2);
          fprintf(COMM,"ERR - executing\r");
        }
        else {
          fprintf(COMM,"OK+++%1.0f/%01.0f/%1.0f\r", MAG_raw.AXIS_X, MAG_raw.AXIS_Y, MAG_raw.AXIS_Z);
          fprintf(COMM,"Magnetometer 1 - Magnetic field in specific range X=%1.0f Y=%1.0f Z=%1.0f\r", MAG_raw.AXIS_X, MAG_raw.AXIS_Y, MAG_raw.AXIS_Z);
        }

        if (Magnitometers_LIS3MDL_Read_Data(&MAG_raw, LIS3MDL_MAG_I2C_ADDRESS_HIGH) != E_OK) {
          I2C_Reset(&hi2c2);
          fprintf(COMM,"ERR - executing\r");
        }
        else {
          fprintf(COMM,"OK+++%1.0f/%01.0f/%1.0f\r", MAG_raw.AXIS_X, MAG_raw.AXIS_Y, MAG_raw.AXIS_Z);
          fprintf(COMM,"Magnetometer 2 - Magnetic field in specific range X=%1.0f Y=%1.0f Z=%1.0f\r", MAG_raw.AXIS_X, MAG_raw.AXIS_Y, MAG_raw.AXIS_Z);
        }

        osDelay(1000);    /* Give processing time for the other tasks */
    }
}

void readCounterChip(void const * argument)
{

    HAL_StatusTypeDef szret;
    uint8_t szbuf[8];
    uint8_t i = 0;
    uint32_t value = 0;
    uint32_t left_byte = 0x00;
    uint32_t right_byte = 0x00;
    uint32_t mid_byte = 0x00;
    uint8_t flag = 0;

    #define SZINFC &hi2c2
    #define emptyREG 0x00
    #define WRADDR 0xa0
    #define SZADDR 0xa1

    osDelay(1000);

    TaskMonitor_TaskInitialized(TASK_MONITOR_DEFAULT);

    //Write mode to chip

    szbuf[0] = 0x00;
    szbuf[1] = 0x20;
    szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
    if ( szret != HAL_OK)
        fprintf( COMM, "got error - %d\n", szret);
    szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
    if ( szret != HAL_OK)
        fprintf( COMM, "got error - %d\n", szret);
    //fprintf( COMM, "MODE = %02x\n", szbuf[0]);

    szbuf[0] = 0x00;
    szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
    if ( szret != HAL_OK)
        fprintf( COMM, "got error - %d\n", szret);
    szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
    if ( szret != HAL_OK)
        fprintf( COMM, "got error - %d\n", szret);
    fprintf( COMM, "MODE = %02x\n", szbuf[0]);

    for( ; ; )
    {
        TaskMonitor_IamAlive(TASK_MONITOR_DEFAULT); /* Prevent from WatchDog reset */

        if(flag)
        {
            szbuf[0] = 0x00;
            szbuf[1] = 0xA0;
            szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
            if ( szret != HAL_OK)
                fprintf( COMM, "got error - %d\n", szret);
            szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
            if ( szret != HAL_OK)
                fprintf( COMM, "got error - %d\n", szret);
            //fprintf( COMM, "STOP COUNTER %02x\n", szbuf[0]);
        }

        szbuf[0] = 0x01;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        fprintf( COMM, "sz%d = %02x\n", i, szbuf[0]);
        right_byte = szbuf[0];

        szbuf[0] = 0x02;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        fprintf( COMM, "sz%d = %02x\n", i, szbuf[0]);
        mid_byte = szbuf[0];

        szbuf[0] = 0x03;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        fprintf( COMM, "sz%d = %02x\n", i, szbuf[0]);
        left_byte = szbuf[0];

        left_byte = left_byte << 16;
        mid_byte = mid_byte << 8;
        value = (left_byte ^ mid_byte) ^ right_byte;

        cr_ntw = (cr_ntw == CR_BUF_SIZE) ? 0 : cr_ntw;
        cr_bfr[cr_ntw] = value;
        cr_ntw++;

        fprintf( COMM, "CNT = %lu\n", (unsigned long)value);

        szbuf[0] = 0x01;
        szbuf[1] = 0x00;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);

        szbuf[0] = 0x02;
        szbuf[1] = 0x00;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);

        szbuf[0] = 0x03;
        szbuf[1] = 0x00;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);

        szbuf[0] = 0x00;
        szbuf[1] = 0x20;
        szret = HAL_I2C_Master_Transmit(SZINFC, SZADDR, szbuf, 2, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        szret = HAL_I2C_Master_Receive(SZINFC, SZADDR, szbuf, 1, HAL_MAX_DELAY);
        if ( szret != HAL_OK)
            fprintf( COMM, "got error - %d\n", szret);
        //fprintf( COMM, "START COUNTER %02x\n", szbuf[0]);

        flag = 1;
        i++;
        osDelay(ADCS_INTERVAL);

    }
}
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
