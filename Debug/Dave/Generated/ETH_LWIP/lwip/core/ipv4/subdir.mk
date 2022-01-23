################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/autoip.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/icmp.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/igmp.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet_chksum.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_addr.c \
../Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_frag.c 

OBJS += \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/autoip.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/icmp.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/igmp.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet_chksum.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_addr.o \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_frag.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/autoip.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/icmp.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/igmp.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet_chksum.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_addr.d \
./Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_frag.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/lwip/core/ipv4/%.o: ../Dave/Generated/ETH_LWIP/lwip/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

