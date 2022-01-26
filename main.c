/****************************************************************
* HEADER FILES
***************************************************************/
#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

// data types

typedef struct {
	uint32_t ms;
	uint32_t us;
	uint32_t seq_num;
} header_t;

typedef struct {
	uint8_t status[3];
	uint8_t ch1[3];
	uint8_t ch2[3];
	uint8_t ch3[3];
	uint8_t ch4[3];
}__attribute__((packed)) ADC_data_t;

typedef struct {
	header_t header;
	ADC_data_t data;
	uint8_t zeros[3]; // 3 zeros at the end of a read
}__attribute__((packed)) ADC_packet_t;

#define NUM_TC 4

typedef struct {
	header_t header;
	uint32_t data[NUM_TC];
} thermocouple_packet_t;

// counters
uint32_t sequence_number = 0;
uint32_t millisec = 0;		// Value to capture the amount of milliseconds that have passed since program start

// data buffers
ADC_packet_t ADC0_buff;
ADC_packet_t ADC1_buff;
thermocouple_packet_t TC_buff;

// read flags
uint8_t read_tc = 0;		// Flag for Thermocouple read
uint8_t read_adc0 = 0;	// Flag for ADC0 Read -- Interrupt
uint8_t read_adc1 = 0;	// Flag for ADC1 Read -- Interrupt

// config array
uint8_t configArray[56] = {0x00}; // Note: overwritten by each ADC, so put a breakpoint after each ADC config if you want to read the data for an individal ADC

// Networking
#define MAX_IP_CHARS 17 // 4 3-character numbers + 4 dots + 1 null
#define DST_IP_EEPROM_OFFSET 0
static char DST_IP[MAX_IP_CHARS] = "10.10.10.75"; // NOTE: this has to be an array, not a pointer! this is the default address
#define SRC_PORT 8080
#define ADC0_PORT 8080
#define ADC1_PORT 8081
#define TC_PORT 8082

struct udp_pcb* pcb;
struct pbuf* p;
ip_addr_t dst_ip;
struct netif* netif;


int set_dst_ip(const char* addr) {
	if(addr == NULL) {
		if(!E_EEPROM_XMC4_IsFlashEmpty()) {
			// if there's data in flash, copy it to DST_IP
			E_EEPROM_XMC4_ReadArray(0, (unsigned char*)DST_IP, MAX_IP_CHARS);
		}

		// set our address based on what's in DST_IP
		if(!ipaddr_aton(DST_IP, &dst_ip)) {
			// invalid addr
			return -1;
		}
	}

	// otherwise we need to write the address to flash AND update our local copy

	size_t len = strlen(addr) + 1;

	if(len > MAX_IP_CHARS) {
		// invalid addr
		return -1;
	}

	if(!ipaddr_aton(addr, &dst_ip)) {
		// invalid addr
		return -1;
	}

	// save the new address locally
	memcpy(DST_IP, addr, len);

	// write the new address out to flash
	if(E_EEPROM_XMC4_WriteArray(DST_IP_EEPROM_OFFSET, (unsigned char*)DST_IP, MAX_IP_CHARS)) {
		if(E_EEPROM_XMC4_STATUS_OK == E_EEPROM_XMC4_UpdateFlashContents()) {
			return 1;
		} else {
			return -1;
		}
	} else {
		return -1;
	}

	return 1;
}

void local_udp_init() {
	pcb = udp_new();
	// our IP is set in the ETH DAVE app, we just want to bind to a port to send from
	udp_bind(pcb, IP_ADDR_ANY, SRC_PORT);

	// NOTE: not checking for errors currently
	set_dst_ip(NULL);

	// allocate buffer at least the size of the largest packet we'll send
	if(sizeof(ADC_data_t) + sizeof(header_t) > sizeof(thermocouple_packet_t)) {
		p = pbuf_alloc(PBUF_TRANSPORT, sizeof(ADC_data_t) + sizeof(header_t), PBUF_RAM);
	} else {
		p = pbuf_alloc(PBUF_TRANSPORT, sizeof(thermocouple_packet_t), PBUF_RAM);
	}

	// set the interface to send on
	netif = ETH_LWIP_0.xnetif;
}

void send_data(void* data, uint16_t size, uint16_t port) {
	memcpy(p->payload, data, size);
	p->len = p->tot_len = size;

	udp_sendto_if(pcb, p, &dst_ip, port, netif);
}

void adc_register_config();
void xmc_ADC_setup();

/*
* @brief main() - Application entry point
*
* <b>Details of function</b><br>
* This routine is the application entry point. It is invoked by the device startup code. It is responsible for
* invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
* code.
*/
int main(void) {
	DAVE_STATUS_t status;
	uint32_t timer_systimer_lwip; 				// Timer for Ethernet Checkouts???
	uint8_t null[18] = {0x00}; 					// Null packet to "send" during ADC Transfers

	// DAVE STARTUP
	status = DAVE_Init(); /* Initialization of DAVE APPs  */
	if(status != DAVE_STATUS_SUCCESS) {
		/* Placeholder for error handler code.
		* The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");
		while(1) {};
	}

	// Initialize UDP interface
	local_udp_init();

	//Initialize ADCs
	//Unlock / Config
	// ADC0
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_0); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	adc_register_config();

	// ADC 1
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_1); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	adc_register_config();

	// Turn on ADCs
	// ADC0
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_0); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	xmc_ADC_setup();

	// ADC 1
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_1); // Change slave
	for (int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	xmc_ADC_setup();

	// RETURN TO ADC0
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_0); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)


	// Initialize and start lwip system timer
	// don't need for UDP (no packets timeout, don't need to ask lwip to check)
	timer_systimer_lwip = SYSTIMER_CreateTimer(10000, SYSTIMER_MODE_PERIODIC, (SYSTIMER_CALLBACK_t)sys_check_timeouts, 0); // WAS  //1000000
	SYSTIMER_StartTimer(timer_systimer_lwip);

	// Enable interrupts once configuration complete
	PIN_INTERRUPT_Enable(&PIN_INTERRUPT_ADC0); 	// ADC0 DRDY Interrupt
	PIN_INTERRUPT_Enable(&PIN_INTERRUPT_ADC1); 	// ADC1 DRDY Interrupt
	INTERRUPT_Enable(&INTERRUPT_TC);			// Thermocouple Timer Interrupt
	INTERRUPT_Enable(&INTERRUPT_TIMESTAMP);		// Millisecond Timestamping Interrupt Enabled

	enum XMC_SPI_CH_SLAVE_SELECT slave_select[NUM_TC] = {SPI_MASTER_SS_SIGNAL_0, SPI_MASTER_SS_SIGNAL_1, SPI_MASTER_SS_SIGNAL_2, SPI_MASTER_SS_SIGNAL_3};
	uint8_t tc_index = 0;

	while(1) {
		// Thermocouple SPI Transfers
		if(read_tc == 1){ // Does timer say we should transfer?
			if (!SPI_MASTER_IsRxBusy(&SPI_MASTER_TC)) { // Check if SPI is not busy
				SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_TC, slave_select[tc_index]);
				SPI_MASTER_Receive(&SPI_MASTER_TC, (uint8_t*)&(TC_buff.data[tc_index]), sizeof(uint32_t));

				tc_index++;

				if(tc_index == NUM_TC) {
					tc_index = 0;
					read_tc = 0;

					// Send out TC packet
					TC_buff.header.seq_num = sequence_number;
					sequence_number++;
					send_data((void*)&TC_buff, sizeof(thermocouple_packet_t), TC_PORT);
				}
			}
		} // End TC Read

		// ADC SPI Transfers
		// NEED TO SET SOME SORT OF PRIORITY HERE, WHERE WE NEED TO HAVE ADC1 HAPPEN, EVEN IF ADC1 IS READY -- NOT SURE IF THIS IS A REAL PROBLEM ONCE WE ACTUALLY HAVE INTERRUPTS INSTEAD OF READ0 and READ1 AUTO-SET TO 1 AT BEGINNING OF LOOP
		// NOTE: I HAD TO MAKE THE FIFO IN THE DAVE APP 32, NOT 16, BECAUSE 16 WOULD NOT HOLD ENOUGH DATA AND THE SPI TRANSFER WOULD SPLIT

		// ADC0 SPI Transfers
		if (read_adc0) { // Flag set
			if (!SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)) { // SPI not already in transaction
				SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC, SPI_MASTER_SS_SIGNAL_0); // Change to ADC0
				SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, (uint8_t*)&ADC0_buff.data, sizeof(ADC_data_t) + 3);
				read_adc0 = 0; // Reset Flag

				// send out ADC0 packet
				ADC0_buff.header.seq_num = sequence_number;
				sequence_number++;
				// be careful not to send zeros
				send_data((void*)&ADC0_buff, sizeof(ADC_packet_t) + sizeof(header_t), ADC0_PORT);
			}
		}

		// ADC1 SPI Transfers
		if (read_adc1) {
			if (!SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)) { // SPI not already in transaction
				SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC, SPI_MASTER_SS_SIGNAL_1); // Change to ADC1
				SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, (uint8_t*)&ADC0_buff.data, sizeof(ADC_data_t) + 3);
				read_adc1 = 0; // Reset Flag

				// send out ADC0 packet
				ADC1_buff.header.seq_num = sequence_number;
				sequence_number++;
				// be careful not to send zeros
				send_data((void*)&ADC1_buff, sizeof(ADC_packet_t) + sizeof(header_t), ADC1_PORT);
			}
		}
	} // End While Loop
} // End main


// INTERRUPTS /////////////////////////////////////////////////////////////////////////////////////

	// Timer configured with 1000us period = 1ms
		void TimeStampIRQ(void) {
			TIMER_ClearEvent(&TIMER_TIMESTAMP); // Clear Event Flag
			millisec++; 						// New device uptime
			return;
		}

	// Thermocouple trigger -- Timer configured with 100000us period = 100ms = 10Hz
		void TCIRQ(void) {
			TIMER_ClearEvent(&TIMER_TC);		// Clear Event Flag
			TC_buff.header.ms = millisec;
			TC_buff.header.us = TIMER_GetTime(&TIMER_TIMESTAMP) / 100;
			read_tc = 1;		// Set flag to read Thermocouples
		}


	// Data Ready Interrupt for ADC0
		void ADC0_DRDY_INT(){
			ADC0_buff.header.ms = millisec;
			ADC0_buff.header.us = TIMER_GetTime(&TIMER_TIMESTAMP) / 100;
			read_adc0 = 1; // Set flag to read ADC0
		}

	// Data Ready Interrupt for ADC1
		void ADC1_DRDY_INT(){
			ADC1_buff.header.ms = millisec;
			ADC1_buff.header.us = TIMER_GetTime(&TIMER_TIMESTAMP) / 100;
			read_adc1 = 1; // Set flag to read ADC1
		}


// FUNCIONS ///////////////////////////////////////////////////////////////////////////////////////

void adc_register_config() {
	// Register Configurations
		uint8_t unlock[3] = {0x06, 0x55, 0x0}; 				// Unlocks ADC
		uint8_t null[18] = {0x00};							// Sends null for reads
		uint8_t write_A_SYS_CFG[3] = {0x4B, 0x68, 0x00};	// b(01101000) -- Neg Charge Pump Powered Down | High-Res | 2.442 Internal Reference | Internal Voltage Enabled | 5/95% Comparator Threshold
		uint8_t write_D_SYS_CFG[3] = {0x4C, 0x3C, 0x00};	// b(00111100) -- Watchdog Disabled | No CRC | 12ns delay for DONE (not used) | 12ns delay for Hi-Z on DOUT | Fixed Frame Size (6 frames) | CRC disabled
		uint8_t write_CLK1[3] = {0x4D, 0x02, 0x00};			// b(00000010) -- XTAL CLK Source | CLKIN /2
		uint8_t write_CLK2_43kHz[3] = {0x4E, 0x4E, 0x00};	// b(01001110) -- ICLK / 4 | OSR = fMOD / 48
//		uint8_t write_CLK2_8kHz[3] = {0x4E, 0x48, 0x00};	// b(01001000) -- ICLK / 4 | OSR = fMOD / 256
		// NOTE -- write_CLK2_43kHz gives a final sample rate of 42.667kHz
		// NOTE -- write_CLK2_8kHz gives a final sample rate of 8kHz


	// Clear configArray for debug
		for (uint8_t i = 0; i<  56; i++){
			configArray[i] = 0x00;
		}

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	//  Unlock ADC for configuration
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, unlock, configArray, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+3, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Write to A_SYS_CFG (See Above)
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_A_SYS_CFG, configArray+6, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion
		// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+9, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Write to D_SYS_CFG (See Above)
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_D_SYS_CFG, configArray+12, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+15, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Write to CLK1 (See Above)
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_CLK1, configArray+18, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+21, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Write to CLK2 (See Above)
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_CLK2_43kHz, configArray+24, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+27, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

}

void xmc_ADC_setup(){
	uint8_t null[18] = {0x00};						// Sends null for reads
	uint8_t write_ADC_ENA[3] = {0x4F, 0x0F, 0x00};	// b(00001111) -- Enables all ADC channels (note: no option to enable certain channels, all or nothing)
	uint8_t wakeup[3] = {0x00, 0x33, 0x00};			// b(00110011) -- Bring ADC out of standby (start collection)

	// Write to ADC_ENA (See Above)
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_ADC_ENA, configArray, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+30, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Wakeup ADC and start conversions
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, wakeup, configArray, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+33, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Set to "infinite" frame length
		XMC_SPI_CH_SetFrameLength(XMC_SPI2_CH0, 64); // When set to 64, frame does not end based on DAVE App Configuration -- this allows us to grab all 144 bits of data out of the ADC during data collection

}

/* NOTES:
ADC0 = IEPE
	IEPE0 = ADC0-CH1
	IEPE1 = ADC0-CH2
	IEPE2 = ADC0-CH3
	IEPE3 = ADC0-CH4

ADC1 = FB/CL
	FB0 = ADC1-CH1
	FB1 = ADC1-CH2
	CL0 = ADC1-CH3
	CL1 = ADC1-CH4
*/
