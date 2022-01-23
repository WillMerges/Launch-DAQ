################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/lwip/api/api_lib.c \
../Dave/Generated/ETH_LWIP/lwip/api/api_msg.c \
../Dave/Generated/ETH_LWIP/lwip/api/err.c \
../Dave/Generated/ETH_LWIP/lwip/api/netbuf.c \
../Dave/Generated/ETH_LWIP/lwip/api/netdb.c \
../Dave/Generated/ETH_LWIP/lwip/api/netifapi.c \
../Dave/Generated/ETH_LWIP/lwip/api/sockets.c \
../Dave/Generated/ETH_LWIP/lwip/api/tcpip.c 

OBJS += \
./Dave/Generated/ETH_LWIP/lwip/api/api_lib.o \
./Dave/Generated/ETH_LWIP/lwip/api/api_msg.o \
./Dave/Generated/ETH_LWIP/lwip/api/err.o \
./Dave/Generated/ETH_LWIP/lwip/api/netbuf.o \
./Dave/Generated/ETH_LWIP/lwip/api/netdb.o \
./Dave/Generated/ETH_LWIP/lwip/api/netifapi.o \
./Dave/Generated/ETH_LWIP/lwip/api/sockets.o \
./Dave/Generated/ETH_LWIP/lwip/api/tcpip.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/lwip/api/api_lib.d \
./Dave/Generated/ETH_LWIP/lwip/api/api_msg.d \
./Dave/Generated/ETH_LWIP/lwip/api/err.d \
./Dave/Generated/ETH_LWIP/lwip/api/netbuf.d \
./Dave/Generated/ETH_LWIP/lwip/api/netdb.d \
./Dave/Generated/ETH_LWIP/lwip/api/netifapi.d \
./Dave/Generated/ETH_LWIP/lwip/api/sockets.d \
./Dave/Generated/ETH_LWIP/lwip/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/lwip/api/%.o: ../Dave/Generated/ETH_LWIP/lwip/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

