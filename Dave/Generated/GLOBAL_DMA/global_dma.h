
/**
 * @file global_dma.h
 * @date 2021-01-08
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * GLOBAL_DMA v4.0.10 - Initializes the DMA module, sets channel priorities and reserves DMA interrupt node
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
 * 2015-02-20:
 *     - Initial version <br>
 *
 * 2015-04-22:
 *     - GLOBAL_DMA_IsInitialized() is now a static function <br>
 *
 * 2015-06-19:
 *     - Removed the individual LLD version check <br>
 *
 * 2021-01-08:
 *     - Modified check for minimum XMCLib version
 *
 * @endcond
 */

#ifndef GLOBAL_DMA_H
#define GLOBAL_DMA_H

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

#include "xmc_dma.h"
#include "DAVE_Common.h"
#include "global_dma_conf.h"


/**********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define GLOBAL_DMA_XMC_LIB_MAJOR_VERSION 2
#define GLOBAL_DMA_XMC_LIB_MINOR_VERSION 0
#define GLOBAL_DMA_XMC_LIB_PATCH_VERSION 0

#if !((XMC_LIB_MAJOR_VERSION > GLOBAL_DMA_XMC_LIB_MAJOR_VERSION) ||\
      ((XMC_LIB_MAJOR_VERSION == GLOBAL_DMA_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION > GLOBAL_DMA_XMC_LIB_MINOR_VERSION)) ||\
      ((XMC_LIB_MAJOR_VERSION == GLOBAL_DMA_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION == GLOBAL_DMA_XMC_LIB_MINOR_VERSION) && (XMC_LIB_PATCH_VERSION >= GLOBAL_DMA_XMC_LIB_PATCH_VERSION)))
#error "GLOBAL_DMA requires XMC Peripheral Library v2.0.0 or higher"
#endif

/**********************************************************************************************************************
 * ENUMERATIONS
 **********************************************************************************************************************/

/**
 * @ingroup GLOBAL_DMA_enumerations
 * @{
 */

/**
 * @brief GLOBAL_DMA APP function status returns
 */
typedef enum GLOBAL_DMA_STATUS
{
  GLOBAL_DMA_STATUS_SUCCESS = 0U, /**< GLOBAL_DMA operation successful */
  GLOBAL_DMA_STATUS_FAILURE = 1U  /**< GLOBAL_DMA operation failure */
} GLOBAL_DMA_STATUS_t;

/**
 * @}
 */

/**********************************************************************************************************************
 * DATA STRUCTURES
 **********************************************************************************************************************/

/**
 * @ingroup GLOBAL_DMA_datastructures
 * @{
 */

/**
 * @brief GLOBAL_DMA interrupt configuration
 */
typedef struct GLOBAL_DMA_INTERRUPT_CONFIG
{
  uint8_t priority; /** Node interrupt priority */
  uint8_t sub_priority; /** Node interrupt sub-priority */
} GLOBAL_DMA_INTERRUPT_CONFIG_t;

/**
 * @brief GLOBAL_DMA configuration structure
 */
typedef struct GLOBAL_DMA
{
  XMC_DMA_t *dma; /** Pointer to DMA unit */
  const GLOBAL_DMA_INTERRUPT_CONFIG_t *const config; /** Pointer to configuration data */
  bool initialized; /** Is DMA_GLOBAL initialized? */
  IRQn_Type irq_node; /** Mapped NVIC Node */
} GLOBAL_DMA_t;

/**
 * @}
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * API PROTOTYPES
 **********************************************************************************************************************/

/**
 * @ingroup GLOBAL_DMA_apidoc
 * @{
 */

/**
 * @brief Get GLOBAL_DMA APP version
 * @return DAVE_APP_VERSION_t APP version details (major, minor and patch number)
 *
 * \par<b>Description: </b><br>
 * The function can be used to check application software compatibility with a
 * specific version of the APP.
 *
 * Example Usage:
 *
 * @code
 * #include "DAVE.h"
 *
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   DAVE_APP_VERSION_t version;
 *
 *   // Initialize DMA_CH APP. The GLOBAL_DMA APP
 *   // is initialized in the DMA_CH initialization
 *   // routine
 *   init_status = DAVE_Init();
 *
 *   version = GLOBAL_DMA_GetAppVersion();
 *   if (version.major != 4U)
 *   {
 *     // Probably, not the right version.
 *   }
 *
 *   // More code here
 *   while(1)
 *   {
 *   }
 *   return (0);
 * }
 * @endcode<BR> </p>
 */
DAVE_APP_VERSION_t GLOBAL_DMA_GetAppVersion(void);

/**
 * @brief Check if GLOBAL_DMA is already initialized
 * @return bool Return "true" if initialized, "false" otherwise
 *
 * \par<b>Description: </b><br>
 * The function checks if the GLOBAL_DMA APP is initialized. The function
 * is meant for (and used within) the top-level DMA_CH APP. The DMA_CH
 * initialization routine uses the function to check initialization before
 * invoking the GLOBAL_DMA_Init() function.
 *
 * Example Usage:
 *
 * @code
 * #include "DAVE.h"
 *
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *   bool init_global_dma;
 *
 *   // Initialize DMA_CH APP. The GLOBAL_DMA APP
 *   // is initialized in the DMA_CH initialization
 *   // routine
 *   //
 *   // GLOBAL_DMA_IsInitialized() is invoked in within
 *   // the initialization routine of the DMA_CH APP. 
 *   init_status = DAVE_Init();
 *
 *   // Check if GLOBAL_DMA is initialized
 *   init_global_dma = GLOBAL_DMA_IsInitialized(&GLOBAL_DMA_0);
 *
 *   if (!init_global_dma)
 *   {
 *     // GLOBAL_DMA wasn't initialized. The DMA
 *     // module isn't enabled.
 *   }
 *
 *   // More code here
 *   while(1)
 *   {
 *   }
 *   return (0);
 * }
 * @endcode<BR> </p>
 */
__STATIC_INLINE bool GLOBAL_DMA_IsInitialized(GLOBAL_DMA_t *const obj)
{
  XMC_ASSERT("DMA_GLOBAL_IsInitialized: NULL DMA_GLOBAL_t object", (obj != NULL));

  return (bool)(obj->initialized);
}

/**
 * @brief GLOBAL_DMA initialization function
 * @return GLOBAL_DMA_STATUS_t Return ::GLOBAL_DMA_STATUS_SUCCESS if initialized,
 *                             GLOBAL_DMA_STATUS_FAILURE otherwise
 *
 * \par<b>Description: </b><br>
 * The function initializes the GLOBAL_DMA APP. It is meant for (and used within)
 * the top-level DMA_CH APP. The DMA_CH initialization routine invokes this
 * function. In essence, it enables the GPDMA peripheral and event handling.
 *
 * Example Usage:
 *
 * @code
 * #include "DAVE.h"
 *
 * int main(void)
 * {
 *   DAVE_STATUS_t init_status;
 *
 *   // Initialize DMA_CH APP. The GLOBAL_DMA APP
 *   // is initialized in the DMA_CH initialization
 *   // routine
 *   init_status = DAVE_Init();
 *
 *   // More code here
 *   while(1)
 *   {
 *   }
 *   return (0);
 * }
 * @endcode<BR> </p>
 */
GLOBAL_DMA_STATUS_t GLOBAL_DMA_Init(GLOBAL_DMA_t *const obj);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#include "global_dma_extern.h"

#endif /* GLOBAL_DMA_H */
