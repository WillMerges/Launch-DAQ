################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/auth.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/chap.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/chpms.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/fsm.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/ipcp.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/lcp.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/magic.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/md5.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/pap.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp_oe.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/randm.c \
../Dave/Generated/ETH_LWIP/lwip/netif/ppp/vj.c 

OBJS += \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/auth.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/chap.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/chpms.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/fsm.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ipcp.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/lcp.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/magic.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/md5.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/pap.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp_oe.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/randm.o \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/vj.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/auth.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/chap.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/chpms.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/fsm.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ipcp.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/lcp.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/magic.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/md5.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/pap.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp_oe.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/randm.d \
./Dave/Generated/ETH_LWIP/lwip/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/lwip/netif/ppp/%.o: ../Dave/Generated/ETH_LWIP/lwip/netif/ppp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

