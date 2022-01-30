################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include Dave/Generated/UART/subdir.mk
-include Dave/Generated/TIMER/subdir.mk
-include Dave/Generated/SYSTIMER/subdir.mk
-include Dave/Generated/SPI_MASTER/subdir.mk
-include Dave/Generated/PIN_INTERRUPT/subdir.mk
-include Dave/Generated/INTERRUPT/subdir.mk
-include Dave/Generated/GLOBAL_DMA/subdir.mk
-include Dave/Generated/GLOBAL_CCU4/subdir.mk
-include Dave/Generated/E_EEPROM_XMC4/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/port/netif/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/port/include/arch/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/port/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/netif/ppp/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/netif/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/core/snmp/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/core/ipv4/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/core/subdir.mk
-include Dave/Generated/ETH_LWIP/lwip/api/subdir.mk
-include Dave/Generated/ETH_LWIP/subdir.mk
-include Dave/Generated/DIGITAL_IO/subdir.mk
-include Dave/Generated/CPU_CTRL_XMC4/subdir.mk
-include Dave/Generated/CLOCK_XMC4/subdir.mk
-include Dave/Generated/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
jme_DAQ_Integrated_XMC4700-F144x2048_0.hex \

SECONDARY_LIST += \
jme_DAQ_Integrated_XMC4700-F144x2048_0.lst \

SECONDARY_SIZE += \
jme_DAQ_Integrated_XMC4700-F144x2048_0.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "jme_DAQ_Integrated_XMC4700-F144x2048_0.map"

TOOLCHAIN_ROOT=C:/Infineon/Tools/DAVE IDE/4.5.0.202105191637/eclipse/ARM-GCC-49
PROJECT_LOC=C:/Users/Will/Documents/GitHub/Launch-DAQ

# All Target
all: jme_DAQ_Integrated_XMC4700-F144x2048_0.elf secondary-outputs

# Tool invocations
jme_DAQ_Integrated_XMC4700-F144x2048_0.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,$(MAP_FILE) -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb -g -gdwarf-2 -o "jme_DAQ_Integrated_XMC4700-F144x2048_0.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
jme_DAQ_Integrated_XMC4700-F144x2048_0.hex: jme_DAQ_Integrated_XMC4700-F144x2048_0.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "jme_DAQ_Integrated_XMC4700-F144x2048_0.elf" "jme_DAQ_Integrated_XMC4700-F144x2048_0.hex"
	@echo 'Finished building: $@'
	@echo.
jme_DAQ_Integrated_XMC4700-F144x2048_0.lst: jme_DAQ_Integrated_XMC4700-F144x2048_0.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "jme_DAQ_Integrated_XMC4700-F144x2048_0.elf" > "jme_DAQ_Integrated_XMC4700-F144x2048_0.lst"
	@echo 'Finished building: $@'
	@echo.
jme_DAQ_Integrated_XMC4700-F144x2048_0.siz: jme_DAQ_Integrated_XMC4700-F144x2048_0.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "jme_DAQ_Integrated_XMC4700-F144x2048_0.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)
	-$(RM) Dave/Generated/CLOCK_XMC4/clock_xmc4.o Dave/Generated/CLOCK_XMC4/clock_xmc4_conf.o Dave/Generated/CPU_CTRL_XMC4/cpu_ctrl_xmc4.o Dave/Generated/CPU_CTRL_XMC4/cpu_ctrl_xmc4_conf.o Dave/Generated/DAVE.o Dave/Generated/DIGITAL_IO/digital_io.o Dave/Generated/DIGITAL_IO/digital_io_conf.o Dave/Generated/ETH_LWIP/eth_lwip.o Dave/Generated/ETH_LWIP/eth_lwip_conf.o Dave/Generated/ETH_LWIP/xmc_eth_phy_dp83848.o Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8031rnl.o Dave/Generated/ETH_LWIP/xmc_eth_phy_ksz8081rnl.o Dave/Generated/ETH_LWIP/lwip/api/api_lib.o Dave/Generated/ETH_LWIP/lwip/api/api_msg.o Dave/Generated/ETH_LWIP/lwip/api/err.o Dave/Generated/ETH_LWIP/lwip/api/netbuf.o Dave/Generated/ETH_LWIP/lwip/api/netdb.o Dave/Generated/ETH_LWIP/lwip/api/netifapi.o Dave/Generated/ETH_LWIP/lwip/api/sockets.o Dave/Generated/ETH_LWIP/lwip/api/tcpip.o Dave/Generated/ETH_LWIP/lwip/core/def.o Dave/Generated/ETH_LWIP/lwip/core/dhcp.o Dave/Generated/ETH_LWIP/lwip/core/dns.o Dave/Generated/ETH_LWIP/lwip/core/init.o Dave/Generated/ETH_LWIP/lwip/core/mem.o Dave/Generated/ETH_LWIP/lwip/core/memp.o Dave/Generated/ETH_LWIP/lwip/core/netif.o Dave/Generated/ETH_LWIP/lwip/core/pbuf.o Dave/Generated/ETH_LWIP/lwip/core/raw.o Dave/Generated/ETH_LWIP/lwip/core/stats.o Dave/Generated/ETH_LWIP/lwip/core/sys.o Dave/Generated/ETH_LWIP/lwip/core/tcp.o Dave/Generated/ETH_LWIP/lwip/core/tcp_in.o Dave/Generated/ETH_LWIP/lwip/core/tcp_out.o Dave/Generated/ETH_LWIP/lwip/core/timers.o Dave/Generated/ETH_LWIP/lwip/core/udp.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/autoip.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/icmp.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/igmp.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/inet_chksum.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_addr.o Dave/Generated/ETH_LWIP/lwip/core/ipv4/ip_frag.o Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_dec.o 
	-$(RM) Dave/Generated/ETH_LWIP/lwip/core/snmp/asn1_enc.o Dave/Generated/ETH_LWIP/lwip/core/snmp/mib2.o Dave/Generated/ETH_LWIP/lwip/core/snmp/mib_structs.o Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_in.o Dave/Generated/ETH_LWIP/lwip/core/snmp/msg_out.o Dave/Generated/ETH_LWIP/lwip/netif/etharp.o Dave/Generated/ETH_LWIP/lwip/netif/slipif.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/auth.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/chap.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/chpms.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/fsm.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/ipcp.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/lcp.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/magic.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/md5.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/pap.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/ppp_oe.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/randm.o Dave/Generated/ETH_LWIP/lwip/netif/ppp/vj.o Dave/Generated/ETH_LWIP/lwip/port/include/arch/memcpy.o Dave/Generated/ETH_LWIP/lwip/port/netif/ethernetif.o Dave/Generated/ETH_LWIP/lwip/port/sys_arch.o Dave/Generated/E_EEPROM_XMC4/e_eeprom_xmc4.o Dave/Generated/E_EEPROM_XMC4/e_eeprom_xmc4_conf.o Dave/Generated/GLOBAL_CCU4/global_ccu4.o Dave/Generated/GLOBAL_CCU4/global_ccu4_conf.o Dave/Generated/GLOBAL_DMA/global_dma.o Dave/Generated/GLOBAL_DMA/global_dma_conf.o Dave/Generated/INTERRUPT/interrupt.o Dave/Generated/INTERRUPT/interrupt_conf.o Dave/Generated/PIN_INTERRUPT/pin_interrupt.o Dave/Generated/PIN_INTERRUPT/pin_interrupt_conf.o Dave/Generated/SPI_MASTER/spi_master.o Dave/Generated/SPI_MASTER/spi_master_conf.o Dave/Generated/SYSTIMER/systimer.o Dave/Generated/SYSTIMER/systimer_conf.o Dave/Generated/TIMER/timer.o Dave/Generated/TIMER/timer_conf.o Dave/Generated/UART/uart.o Dave/Generated/UART/uart_conf.o Libraries/Newlib/syscalls.o Libraries/XMCLib/src/xmc4_eru.o Libraries/XMCLib/src/xmc4_flash.o Libraries/XMCLib/src/xmc4_gpio.o 
	-$(RM) Libraries/XMCLib/src/xmc4_rtc.o Libraries/XMCLib/src/xmc4_scu.o Libraries/XMCLib/src/xmc_can.o Libraries/XMCLib/src/xmc_ccu4.o Libraries/XMCLib/src/xmc_ccu8.o Libraries/XMCLib/src/xmc_common.o Libraries/XMCLib/src/xmc_dac.o Libraries/XMCLib/src/xmc_dma.o Libraries/XMCLib/src/xmc_dsd.o Libraries/XMCLib/src/xmc_ebu.o Libraries/XMCLib/src/xmc_ecat.o Libraries/XMCLib/src/xmc_eru.o Libraries/XMCLib/src/xmc_eth_mac.o Libraries/XMCLib/src/xmc_eth_phy_dp83848.o Libraries/XMCLib/src/xmc_eth_phy_ksz8031rnl.o Libraries/XMCLib/src/xmc_eth_phy_ksz8081rnb.o Libraries/XMCLib/src/xmc_fce.o Libraries/XMCLib/src/xmc_gpio.o Libraries/XMCLib/src/xmc_hrpwm.o Libraries/XMCLib/src/xmc_i2c.o Libraries/XMCLib/src/xmc_i2s.o Libraries/XMCLib/src/xmc_ledts.o Libraries/XMCLib/src/xmc_posif.o Libraries/XMCLib/src/xmc_rtc.o Libraries/XMCLib/src/xmc_sdmmc.o Libraries/XMCLib/src/xmc_spi.o Libraries/XMCLib/src/xmc_uart.o Libraries/XMCLib/src/xmc_usbd.o Libraries/XMCLib/src/xmc_usbh.o Libraries/XMCLib/src/xmc_usic.o Libraries/XMCLib/src/xmc_vadc.o Libraries/XMCLib/src/xmc_wdt.o Startup/startup_XMC4700.o Startup/system_XMC4700.o main.o  jme_DAQ_Integrated_XMC4700-F144x2048_0.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
