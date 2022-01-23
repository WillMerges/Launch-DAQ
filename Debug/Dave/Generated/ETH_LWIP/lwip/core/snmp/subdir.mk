################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_dec.c \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_enc.c \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/mib2.c \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/mib_structs.c \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_in.c \
../Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_out.c 

OBJS += \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_dec.o \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_enc.o \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/mib2.o \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/mib_structs.o \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_in.o \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_out.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_dec.d \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_enc.d \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/mib2.d \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/mib_structs.d \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_in.d \
./Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_out.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/lwip/core/snmp/%.o: ../Dave/Generated/ETH_LWIP/lwip/core/snmp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

