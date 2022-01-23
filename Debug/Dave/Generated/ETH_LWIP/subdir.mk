################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/eth_lwip.c \
../Dave/Generated/ETH_LWIP/eth_lwip_conf.c \
../Dave/Generated/ETH_LWIP/xmc_eth_phy_dp83848.c \
../Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8031rnl.c \
../Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8081rnl.c 

OBJS += \
./Dave/Generated/ETH_LWIP/eth_lwip.o \
./Dave/Generated/ETH_LWIP/eth_lwip_conf.o \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_dp83848.o \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8031rnl.o \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8081rnl.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/eth_lwip.d \
./Dave/Generated/ETH_LWIP/eth_lwip_conf.d \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_dp83848.d \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8031rnl.d \
./Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8081rnl.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/%.o: ../Dave/Generated/ETH_LWIP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

