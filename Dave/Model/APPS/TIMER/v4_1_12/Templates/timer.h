/**
 * @file timer.h
 * @date 2021-01-08
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * TIMER v4.1.12 - Configures the properties of CCU4 or CCU8 peripheral as a timer.
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial version<br>
 *
 * 2015-05-08:
 *     - "shadow_mask" is added in APP config structure. This can be used directly to enable the shadow transfer.<br>
 *     - TIMER_GetTime() and TIMER_Clear() are added.
 *     - Enum items of "TIMER_MODULE" are changed to follow coding guidelines<br>
 *
 * 2015-05-22:
 *     -  API name changed
 *          a. TIMER_AcknowledgeInterrupt() --> TIMER_ClearEvent()<br>
 *
 * 2021-01-08:
 *     - Added check for minimum XMCLib version
 *
 * @endcond
 *
 */

#ifndef TIMER_H
#define TIMER_H
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "timer_conf.h"
#ifdef  TIMER_CCU4_USED
#include "GLOBAL_CCU4/global_ccu4.h"
#endif
#ifdef  TIMER_CCU8_USED
#include "GLOBAL_CCU8/global_ccu8.h"
#endif
#include "DAVE_Common.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define TIMER_XMC_LIB_MAJOR_VERSION 2
#define TIMER_XMC_LIB_MINOR_VERSION 0
#define TIMER_XMC_LIB_PATCH_VERSION 0

#if !((XMC_LIB_MAJOR_VERSION > TIMER_XMC_LIB_MAJOR_VERSION) ||\
      ((XMC_LIB_MAJOR_VERSION == TIMER_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION > TIMER_XMC_LIB_MINOR_VERSION)) ||\
      ((XMC_LIB_MAJOR_VERSION == TIMER_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION == TIMER_XMC_LIB_MINOR_VERSION) && (XMC_LIB_PATCH_VERSION >= TIMER_XMC_LIB_PATCH_VERSION)))
#error "TIMER requires XMC Peripheral Library v2.0.0 or higher"
#endif


/***********************************************************************************************************************
 * ENUMS
 **********************************************************************************************************************/
/**
 * @ingroup TIMER_enumerations
 * @{
 */

/**
 * @brief The type identifies the CCU4 or CCU8 timer selected.
 */
typedef enum TIMER_MODULE
{
  TIMER_MODULE_CCU4 = 0U, /**< CCU4 is selected */
  TIMER_MODULE_CCU8       /**< CCU8 is selected */
} TIMER_MODULE_t;

/**
 * @brief status of the TIMER APP
 */
typedef enum TIMER_STATUS{
  TIMER_STATUS_SUCCESS = 0U, /**< Status success */
  TIMER_STATUS_FAILURE       /**< Status failure */
} TIMER_STATUS_t;

/**
 * @}
 */
/***********************************************************************************************************************
* DATA STRUCTURES
***********************************************************************************************************************/
/**
 * @ingroup TIMER_datastructures
 * @{
 */

/**
 * @brief Initialization parameters of the TIMER APP
 */
typedef struct TIMER
{
  uint32_t time_interval_value_us; /**< Timer interval value for which event is being generated */
  const uint32_t timer_max_value_us;	/**< Maximum timer value in micro seconds for the available clock */
  const uint32_t timer_min_value_us;  /**< Minimum timer value in micro seconds for the available clock */
  const uint32_t shadow_mask;  /**< shadow transfer mask for the selected timer */
#ifdef  TIMER_CCU4_USED
  GLOBAL_CCU4_t* const global_ccu4_handler; /**< Reference to CCU4GLOBAL APP handler */
  XMC_CCU4_SLICE_t* const ccu4_slice_ptr;  /**< Reference to CCU4-CC4 slice identifier data handler */
  const uint8_t ccu4_slice_number;  /* Timer being used */
  XMC_CCU4_SLICE_COMPARE_CONFIG_t* const ccu4_slice_config_ptr; /**< Reference to initialization data structure of
                                                                           the core timer functionality */
  XMC_CCU4_SLICE_SR_ID_t  const ccu4_period_match_node; /**< Service Request Id for period match event */
#endif
#ifdef  TIMER_CCU8_USED
  GLOBAL_CCU8_t* const global_ccu8_handler; /**< Reference to CCU8GLOBAL APP handler */
  XMC_CCU8_SLICE_t* const ccu8_slice_ptr; /**< Reference to CCU8-CC8 slice identifier data handler */
  const uint8_t ccu8_slice_number;  /* Timer being used */
  XMC_CCU8_SLICE_COMPARE_CONFIG_t* const ccu8_slice_config_ptr; /**< Reference to initialization data structure of
                                                                           the core timer functionality */
  XMC_CCU8_SLICE_SR_ID_t const ccu8_period_match_node; /**< Service Request Id for period match event */
#endif
  TIMER_MODULE_t const timer_module; /**< Indicate which timer module is being used from CCU4 and CCU8 */
  uint16_t period_value; /**< Period value to be loaded into timer for the corresponding time tick */
  bool const start_control; /**< Indicate whether to start the APP during initialization itself */
  bool const period_match_enable; /**< Indicate the generation of period match event */
  bool initialized;  /* flag to indicate the initialization state of the APP instance */
} TIMER_t;

/**
 * @}
 */
/***********************************************************************************************************************
* API Prototypes
***********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup TIMER_apidoc
 * @{
 */
/**
 * @brief Get TIMER APP version
 * @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
 * <BR>
 * \par<b>Description:</b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * 
 * int main(void)
 * {
 *   DAVE_STATUS_t status;
 *   DAVE_APP_VERSION_t app_version;
 *
 *   status = DAVE_Init();	// TIMER_Init() is called from DAVE_Init()
 *
 *   app_version = TIMER_GetAppVersion();
 *
 *   if (app_version.major != 4U)
 *   {
 *     // Probably, not the right version.
 *   }
 *
 *   while(1U)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */
DAVE_APP_VERSION_t TIMER_GetAppVersion(void);

/**
 * @brief Initializes a TIMER with generated configuration.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return TIMER_STATUS_t\n  TIMER_STATUS_SUCCESS : if initialization is successful\n
 *                           TIMER_STATUS_FAILURE : if initialization is failed\n
 * <BR>
 * \par<b>Description:</b><br>
 * <ul>
 * <li>Enable the clock for the slice and invoke the LLD API with generated configuration handle.</li>
 * <li>Load the Period, Compare and Prescaler shadow registers with the generated values and enable the shadow transfer
 * request. This loads the values into the actual registers and start the TIMER based on the configuration.</li>
 * <li>If "Start after initialization" is not enabled, TIMER_Start() can be invoked to start the timer.</li>
 * </ul>
 * <BR>
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * 
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   while(1)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */
TIMER_STATUS_t TIMER_Init(TIMER_t  *const handle_ptr);

/**
 * @brief       Starts the timer if the initialization of the APP is successful.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return TIMER_STATUS_t\n TIMER_STATUS_SUCCESS : if timer start is successful\n
 *                          TIMER_STATUS_FAILURE : if timer start is failed\n
 * <BR>
 *
 * \par<b>Description:</b><br>
 * If "Start after initialization" is not enabled, TIMER_Start() can be invoked to start the timer. TIMER_Stop() can be
 * used to stop the Timer. No need to reconfigure the timer to start again.
 *
 * <BR>
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * 
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t timer_status;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     timer_status = TIMER_Start(&TIMER_0);
 *   }
 *   
 *   while(1)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */
TIMER_STATUS_t TIMER_Start(TIMER_t  *const handle_ptr);

/**
 * @brief Stops the TIMER, if it is running.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return TIMER_STATUS_t\n TIMER_STATUS_SUCCESS : if timer is running and stop is successful\n
 *                          TIMER_STATUS_FAILURE : if timer is in idle state, and stop is called\n
 *<BR>
 *
 * \par<b>Description:</b><br>
 * Clears the Timer run bit to stop. No further event is generated.
 *
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t timer_status;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     timer_status = TIMER_Start(&TIMER_0);
 *   }
 *
 *   if (timer_status == TIMER_STATUS_SUCCESS)
 *   {
 *     while(TIMER_GetInterruptStatus(&TIMER_0));
 *
 *     timer_status = TIMER_Stop(&TIMER_0);
 *   }
 *   
 *   while(1)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */

TIMER_STATUS_t TIMER_Stop(TIMER_t  *const handle_ptr);

/**
 * @brief Returns the current time in micro seconds by scaling with 100.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return uint32_t\n time in microseconds
 *<BR>
 *
 * \par<b>Description:</b><br>
 * By using prescaler and frequency and timer register value, this API calculates the current time in micro seconds.
 * Then the value is scaled with 100, before returning.
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t timer_status;
 *   uint32_t elapsed_time;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     timer_status = TIMER_Start(&TIMER_0);
 *   }
 *
 *   timer_status = TIMER_Stop(&TIMER_0);
 *
 *   elapsed_time = TIMER_GetTime(&TIMER_0);
 *
 *   while(1)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */
uint32_t TIMER_GetTime(TIMER_t *const handle_ptr);

/**
 * @brief Clears the timer register.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return TIMER_STATUS_t\n TIMER_STATUS_SUCCESS : if clear is successful\n
 *                          TIMER_STATUS_FAILURE : if timer is not initialized and clear is requested\n
 *<BR>
 *
 * \par<b>Description:</b><br>
 * TIMER_Clear() clears the timer register so that next cycle starts from reset value.
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t timer_status;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     timer_status = TIMER_Start(&TIMER_0);
 *   }
 *
 *   if (TIMER_GetTimerStatus(&TIMER_0))
 *   {
 *     timer_status = TIMER_Stop(&TIMER_0);
 *   }
 *
 *   timer_status = TIMER_Clear(&TIMER_0);
 *
 *   while(1)
 *   {
 *   }
 *   return 1;
 * }
 * @endcode<BR>
 */
TIMER_STATUS_t TIMER_Clear(TIMER_t *const handle_ptr);

/**
 * @brief Returns the running state of the timer.
 *
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return bool\n true  : if the timer is running\n
 *                false : if the timer is not running\n
 *<BR>
 *
 * \par<b>Description:</b><br>
 * TIMER_GetTimerStatus() reads the run bit of the timer to indicate the actual state of the TIMER.
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * 
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t timer_status;
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     timer_status = TIMER_Start(&TIMER_0);
 *   }
 *
 *   if (TIMER_GetTimerStatus(&TIMER_0))
 *   {
 *     while(TIMER_GetTimerStatus(&TIMER_0));
 *
 *     timer_status = TIMER_Stop(&TIMER_0);
 *   }
 * 
 *   while(1)
 *   {
 *   }
 *   
 *   return 1;
 * }
 * @endcode<BR>
 */

bool TIMER_GetTimerStatus(TIMER_t  *const handle_ptr);

/**
 * @brief Set the new time interval for the event generation, by checking with the supported range.
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @param time_interval new time interval value in micro seconds.
 *
 * @return TIMER_STATUS_t\n TIMER_STATUS_SUCCESS : Setting new time interval value is successful\n
 *                          TIMER_STATUS_FAILURE : New time value is not in range of supported time value\n
 *                                                 Timer is in running condition
 *              <BR>
 *
 * \par<b>Description:</b><br>
 * Based on the timer interval, prescaler value is calculated for the CCU timer. By using this prescaler and
 * time interval values  Period value is calculated. The period value is updated into the shadow register and shadow
 * transfer request is enabled. Timer has to be stopped before updating the time interval.<br>\n
 *
 * \par<b>Note:</b><br>
 * Input time interval value has to be scaled by 100 to the actual required value.\n
 * e.g. : required timer interval value = 30.45 micro seconds\n
 *        Input value to the API = 30.45 * 100 = 3045
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * #include "xmc_gpio.h"                   // GPIO LLD header, this contains the interface for Port functionality
 * #define TIMER_GPIO_PORT XMC_GPIO_PORT0  // PORT0 Address
 * #define TIMER_GPIO_PIN  0U              // Pin number
 * #define TIMER_500MS 500000*100U
 *
 * volatile uint32_t count = 0U;          // count variable to change the time tick interval
 * uint32_t shadow_transfer_msk;          // This is to generate the slice specific shadow transfer mask
 *
 * const XMC_GPIO_CONFIG_t GPIO_0_config  =
 * {
 *   .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
 *   .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
 * };
 *
 * int main(void)
 * {
 *   DAVE_STATUS_t status;
 *
 *   XMC_GPIO_Init(TIMER_GPIO_PORT, TIMER_GPIO_PIN, &GPIO_0_config);
 *
 *   status = DAVE_Init();		// Initialization of DAVE APPs
 *
 *   while(1U)
 *   {
 *   }
 *   return 1;
 * }
 *
 * void Timetick_Handler(void)
 * {
 *   count++;
 *
 *   TIMER_ClearEvent(&TIMER_0);
 *
 *   XMC_GPIO_ToggleOutput(TIMER_GPIO_PORT, TIMER_GPIO_PIN);
 *
 *   if(count > 10)
 *   {
 *     count = 0U;
 *     TIMER_Stop(&TIMER_0);
 *     status = TIMER_SetTimeInterval(&TIMER_0, TIMER_500MS);
 *     if (status == TIMER_STATUS_SUCCESS)
 *     {
 *       TIMER_Start(&TIMER_0);
 *     }
 *   }
 * }
 * @endcode<BR>
 */
TIMER_STATUS_t TIMER_SetTimeInterval(TIMER_t  *const handle_ptr, uint32_t time_interval);

/**
 * @brief Indicates the occurrence of time interval event.
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return bool\n true  : if event set\n
 *                false : if event is not set
 * <BR>
 *
 * \par<b>Description:</b><br>
 * The status returned, can be utilized to generate the delay function.
 *
 * Example Usage:
 * @code
 * #include "DAVE.h"
 * #define TIMER_DELAY_MUL_FACTOR 100000U // Converts micro seconds to milli seconds with multiplication factor for
 *                                        // TIMER_GetInterruptStatus().
 * void TIMER_Delay(uint32_t);
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   TIMER_STATUS_t status;
 *   uint32_t delay_val; // delay value in terms milli seconds
 *
 *   init_status = DAVE_Init();	// TIMER_Init(&TIMER_0) will be called from DAVE_Init()
 *
 *   TIMER_ClearEvent(&TIMER_0);
 *
 *   if(init_status == DAVE_STATUS_SUCCESS)
 *   {
 *     delay_val = 1000; // 1000 milli seconds
 *
 *     TIMER_Delay(delay_val);
 *   }
 *
 *   while(1)
 *   {
 *   }
 *   
 *   return 1;
 * }
 *
 * void TIMER_Delay(uint32_t delay_val)
 * {
 *   uint32_t delay_cnt;
 *
 *   delay_cnt = delay_val * TIMER_DELAY_MUL_FACTOR;
 *
 *   TIMER_SetTimeInterval(&TIMER_0,delay_cnt);
 *
 *   TIMER_Start(&TIMER_0);
 *
 *   while(!TIMER_GetInterruptStatus(&TIMER_0));
 *
 *   TIMER_Stop(&TIMER_0);
 * }
 * @endcode<BR>
 */

bool TIMER_GetInterruptStatus(TIMER_t * const handle_ptr);

/**
 * @brief Clears the period match interrupt status of the given timer.
 * @param handle_ptr pointer to the TIMER APP configuration.
 * @return None
 * <BR>
 * \par<b>Description:</b><br>
 * For each occurrence of the time interval event, it has to be cleared through software only. So next event is
 * considered as new.
 *
 * <BR><P ALIGN="LEFT"><B>Example Usage:</B>
 * @code
 * #include "DAVE.h"
 * int main(void)
 * {
 *   DAVE_STATUS_t status;
 *
 *   status = DAVE_Init();		// Initialization of DAVE APPs
 *
 *   while(1U)
 *   {
 *   }
 *   return 1;
 * }
 *
 * void Timetick_Handler(void)
 * {
 *   TIMER_ClearEvent(&TIMER_0);
 * }
 * @endcode<BR>
 */
void TIMER_ClearEvent(TIMER_t *const handle_ptr);

/**
 * @}
 */
#include "timer_extern.h"   /* Included to access the APP Handles at Main.c */

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
