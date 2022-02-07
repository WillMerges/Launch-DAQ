#include <DAVE.h>

// set to 1 if flash memory that stores network config should be reset to defaults
// set to 0 to use the last saved network config (or use defaults if flash is empty)
#define REFLASH 0

// packet data types

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
uint8_t read_tc = 0;	// Flag for Thermocouple read
uint8_t read_adc0 = 0;	// Flag for ADC0 Read -- Interrupt
uint8_t read_adc1 = 0;	// Flag for ADC1 Read -- Interrupt

// config array for ADCs
uint8_t configArray[56] = {0x00}; // Note: overwritten by each ADC, so put a breakpoint after each ADC config if you want to read the data for an individal ADC


// ADC rate
typedef enum {
	FAST_RATE,
	SLOW_RATE
} ADC_rate_t;

// holds persistent configuration information stored in flash
// NOTE: size must be smaller than emulated EEPROM size in DAVE app
typedef struct {
	ip_addr_t src_ip;
	ip_addr_t dst_ip;
	ip_addr_t default_gw;
	ip_addr_t subnet;
	uint16_t src_port;
	uint16_t adc0_port;
	uint16_t adc1_port;
	uint16_t tc_port;
	ADC_rate_t adc0_rate;
	ADC_rate_t adc1_rate;
} config_t;

config_t flash;

// default values
// NOTE: the IP addresses specified in the ETH_LWIP_0 DAVE app will
//		 be overwritten by either these defaults or data set in persistent flash storage
#define DEF_SRC_IP 		"10.10.10.75"
#define DEF_DST_IP 		"10.10.10.25"
#define DEF_DEF_GW 		"10.10.10.25"
#define DEF_SUBNET		"255.255.255.0"
#define DEF_SRC_PORT 	8080
#define DEF_ADC0_PORT	8080
#define DEF_ADC1_PORT 	8081
#define DEF_TC_PORT 	8082
#define DEFAULT_ADC_RATE SLOW_RATE

// UDP out PCB/buffer
struct udp_pcb* pcb;
struct pbuf* p;

// default (only) network interface
struct netif* netif;

// TFTP server PCB/buffer
struct udp_pcb* tftp_pcb;
struct pbuf* tftp_p;

// TFTP constants
#define TFTP_PORT 69
#define TFTP_RRQ 1
#define TFTP_WRQ 2
#define TFTP_DATA 3
#define TFTP_ACK 4
#define TFTP_ERR 5
#define TFTP_MAX_ERR_MSG_LEN 511 // one NULL-terminated data block

#define TFTP_BUFFER_SIZE 4096

// TFTP data buffer
size_t tftp_buff_index = 0;
uint8_t tftp_buff[TFTP_BUFFER_SIZE];

// TFTP headers/packets

typedef struct {
	uint16_t opcode;
	uint16_t block_num;
} tftp_ack_t;

typedef struct {
	uint16_t opcode;
	uint16_t error_code;
	// followed by a NULL terminated string
} tftp_err_t;

typedef struct {
	uint16_t opcode;
	uint16_t block_num;
	// followed by up to 512 bytes of data
} tftp_data_t;

// TFTP data
uint16_t curr_block = 0;
uint16_t ack_num = 0;
uint8_t tftp_send = 0;
ip_addr_t last_addr;
uint16_t last_port;

// message to output on any TFTP read requests
const char* help_msg = "write requests are files made up of zero or more commands\n" \
					   "each command must be newline terminated\n" \
					   "the only supported TFTP mode is octet\n"
					   "any write request will be interpreted as commands, and any read request will send this menu\n" \
					   "written files can be up to 4096 bytes (8 TFTP data transfers)"
					   "possible commands are:\n"
						"    ip.src=[source IP address, e.g. 10.10.10.25]\n" \
						"    ip.dst=[destination IP address]\n" \
						"    ip.gw=[default gateway IP address]\n" \
						"    ip.subnet=[subnet mask IP address]\n" \
						"    udp.src=[source UDP port]\n" \
						"    udp.adc0=[destination port for ADC0 packets]\n" \
						"    udp.adc1=[destination port for ADC1 packets]\n" \
						"    udp.tc=[destination port for thermocouple packets]\n" \
						"    rate.adc0=['slow' (8kHz sample rate) or 'fast' (43kHz sample rate)]\n" \
						"	 rate.ac1=['slow' (8kHz sample rate) or 'fast' (43kHz sample rate)]\n" \
						"    reset --- resets the DAQ after all commands transferred are processed\n";

int write_flash_config();
void local_udp_reset();
void tftp_err(const char* msg, ip_addr_t* addr, uint16_t port);
char my_tolower(char c);
void adc_soft_reset();

// parse the commands in the TFTP buffer
// returns 1 on success, 0 on failure
uint8_t parse_config_commands(ip_addr_t* addr, uint16_t port) {
	uint8_t ret = 1;
	uint8_t reset = 0;

	size_t index = 0;
	char* str = NULL;

	while(index < tftp_buff_index) {
		str = (char*)&tftp_buff[index];

		uint8_t newline = 0;
		for(; index < tftp_buff_index; index++) {
			if((char)tftp_buff[index] == '\n') {
				tftp_buff[index] = '\0';
				newline = 1;
				index++;
				break;
			}
		}

		if(!newline) {
			// reached the end of the buffer with no newline
			tftp_err("command not newline terminated", addr, port);
			ret = 0;
			break;
		} else {
			char* val = NULL;
			for(size_t i = 0; i < strlen(str); i++) {
				str[i] = my_tolower(str[i]);
				if(str[i] == '=') {
					val = &str[i + 1];
					str[i] = '\0';
//					break; // keep going to convert everything to lower case
				}
			}

			if(!val) {
				tftp_err("invalid command", addr, port);
				ret = 0;
				break;
			}

			if(strcmp(str, "ip.src") == 0) {
				ip_addr_t ip;
				if(!ipaddr_aton(val, &ip)) {
					tftp_err("unable to parse ip.src", addr, port);
					ret = 0;
					break;
				}

				flash.src_ip = ip;
			} else if(strcmp(str, "ip.dst") == 0) {
				ip_addr_t ip;
				if(!ipaddr_aton(val, &ip)) {
					tftp_err("unable to parse ip.dst", addr, port);
					ret = 0;
					break;
				}

				flash.dst_ip = ip;
			} else if(strcmp(str, "ip.gw") == 0) {
				ip_addr_t ip;
				if(!ipaddr_aton(val, &ip)) {
					tftp_err("unable to parse ip.gw", addr, port);
					ret = 0;
					break;
				}

				flash.default_gw = ip;
			} else if(strcmp(str, "ip.subnet") == 0) {
				ip_addr_t ip;
				if(!ipaddr_aton(val, &ip)) {
					tftp_err("unable to parse ip.subnet", addr, port);
					ret = 0;
					break;
				}

				flash.subnet = ip;
			} else if(strcmp(str, "udp.src") == 0) {
				flash.src_port = (uint16_t)atoi(val);
			} else if(strcmp(str, "udp.adc0") == 0) {
				flash.adc0_port = (uint16_t)atoi(val);
			} else if(strcmp(str, "udp.adc1") == 0) {
				flash.adc1_port = (uint16_t)atoi(val);
			} else if(strcmp(str, "udp.tc") == 0) {
				flash.tc_port = (uint16_t)atoi(val);
			} else if(strcmp(str, "rate.adc0") == 0) {
				if(strcmp(val, "fast") == 0) {
					flash.adc0_rate = FAST_RATE;
				} else if(strcmp(val, "slow") == 0) {
					flash.adc0_rate = SLOW_RATE;
				} else {
					tftp_err("invalid ADC0 rate", addr, port);
					ret = 0;
					break;
				}
			} else if(strcmp(str, "rate.adc1") == 0) {
				if(strcmp(val, "fast") == 0) {
					flash.adc1_rate = FAST_RATE;
				} else if(strcmp(val, "slow") == 0) {
					flash.adc1_rate = SLOW_RATE;
				} else {
					tftp_err("invalid ADC1 rate", addr, port);
					ret = 0;
					break;
				}
			} else if(strcmp(str, "reset")) {
				reset = 1;
			} else {
				tftp_err("unknown config parameter", addr, port);
				ret = 0;
				break;
			}
		}
	}

	if(ret) {
		// update network interface w/ new addresses
		local_udp_reset();

		if(write_flash_config() < 0) {
			// failed to save config, still set though

			tftp_err("failed to save configuration to flash", addr, port);
			ret = 0;
		}
	}

	if(reset) {
		// reset the whole system

		// reset the ADC
		adc_soft_reset();

		// reset the XMC
		NVIC_SystemReset();
	}

	return ret;
}

// send TFTP data blocks in response to a read request
// assumes 'str' is a NULL-terminated string
// NOTE: does not wait for ACKs, just sends all data immediately
void tftp_send_data(const char* str, ip_addr_t* addr, uint16_t port) {
	uint16_t block_num = 1;

	tftp_data_t header;
	header.opcode = htons(TFTP_DATA);

	ssize_t len = strlen(str);

	while(1) {
		header.block_num = htons(block_num);
		block_num++;
		memcpy(tftp_p->payload, (void*)&header, sizeof(tftp_data_t));

		if(len > 511) {
			// send 512 bytes in this packet then send another one
			memcpy(tftp_p->payload + sizeof(tftp_data_t), (void*)str, 512);

			tftp_p->len = tftp_p->tot_len = sizeof(tftp_data_t) + 512;

			udp_sendto_if(tftp_pcb, tftp_p, addr, port, netif);

			// wait for the ACK to come in or timeout
			uint32_t start = millisec;
			while(ack_num != block_num - 1) {
				if(millisec - start > 2000) {
					// 2s elapsed
					// don't resend, just wait for another request
					tftp_err("timed out waiting for acknowledgment", addr, port);
					return;
				}
			};

			ack_num = 0;
			len -= 512;
			str = &str[512];
		} else if(len == 0) {
			// special case where we send just the header
			tftp_p->len = tftp_p->tot_len = sizeof(tftp_data_t);

			udp_sendto_if(tftp_pcb, tftp_p, addr, port, netif);

			// we're done sending at this point
			break;
		} else {
			// we can send this all in one packet (511 bytes or less)
			memcpy(tftp_p->payload + sizeof(tftp_data_t), (void*)str, len);

			tftp_p->len = tftp_p->tot_len = sizeof(tftp_data_t) + len;

			udp_sendto_if(tftp_pcb, tftp_p, addr, port, netif);

			// we're done sending at this point
			break;
		}
	}
}

// send TFTP error message
void tftp_err(const char* msg, ip_addr_t* addr, uint16_t port) {
	tftp_err_t header;
	header.opcode = htons(TFTP_ERR); // ERROR opcode
	header.error_code = htons(4); // Illegal TFTP operation error code

	memcpy(tftp_p->payload, (void*)&header, sizeof(tftp_err_t));

	size_t len = strlen(msg);
	if(len > TFTP_MAX_ERR_MSG_LEN - 1) {
		len = TFTP_MAX_ERR_MSG_LEN - 1;
	}
	memcpy(tftp_p->payload + sizeof(tftp_err_t), (void*)msg, len);
	((char*)tftp_p->payload)[len + sizeof(tftp_err_t)] = '\0';

	tftp_p->len = tftp_p->tot_len = sizeof(tftp_err_t) + len + 1;

	udp_sendto_if(tftp_pcb, tftp_p, addr, port, netif);
}

// send TFTP ack for a block number
void tftp_ack(uint16_t block, ip_addr_t* addr, uint16_t port) {
	tftp_ack_t header;
	header.opcode = htons(TFTP_ACK); // ACK opcode
	header.block_num = htons(block);

	memcpy(tftp_p->payload, (void*)&header, sizeof(tftp_ack_t));

	tftp_p->len = tftp_p->tot_len = sizeof(tftp_ack_t);

	udp_sendto_if(tftp_pcb, tftp_p, addr, port, netif);
}

// convert a character to lower case
// NOTE: only converts basic alphabet
char my_tolower(char c) {
	if(c >= 'A' && c <= 'Z') {
		return c + ('a' - 'A');
	}

	return c;
}

// checks that a TFTP RRQ/WRQ packet has a mode of 'octet'
// ignores the filename
// assumes the packet at 'data' is a WRQ/RRQ packet at least 4 bytes long
// returns 0 on failure, 1 on success
uint8_t tftp_check_octet_mode(char* data, size_t len, ip_addr_t* addr, uint16_t port) {
	if(len < 4) {
		return 0;
	}

	// we don't care what the filename is, always change our config info
	char* mode = NULL;
	size_t i = 2; // start reading at filename
	for(; i < len; i++) {
		if(data[i] == '\0') {
			mode = &data[++i];
			break;
		}
	}

	if(mode == NULL) {
		tftp_err("no filename given", addr, port);
		return 0;
	}

	if(data[len - 1] != '\0') {
		tftp_err("mode string not null-terminated", addr, port);
		return 0;
	}

	// convert to lower case
	for(size_t j = 0; j < strlen(mode); j++) {
		mode[j] = my_tolower(mode[j]);
	}

	if(strcmp(mode, "octet") != 0) {
		tftp_err("only supported mode is octet", addr, port);
		return 0;
	}

	return 1;
}

// TFTP UDP interface receive callback
void tftp_recv(void* arg, struct udp_pcb* pcb, struct pbuf* p, ip_addr_t* addr, uint16_t port) {
	if(p->len != p->tot_len) {
		tftp_err("cannot fragment packets", addr, port);
		return;
	}

	if(p->len < 4) {
		// need at least an opcode, and two zeros / a block number
		tftp_err("packet too small", addr, port);
	}

	uint16_t opcode = ntohs(*((uint16_t*)p->payload));

	if(opcode == TFTP_WRQ) {
		if(curr_block == 0) {
			if(tftp_send) {
				tftp_err("server busy", addr, port);
				return;
			}

			// we don't care what the filename is, always change our config info
			// we just care that the data is in octet mode
			if(!tftp_check_octet_mode((char*)p->payload, p->len, addr, port)) {
				return;
			}

			tftp_ack(curr_block, addr, port);
			curr_block++;
		} else {
			tftp_err("write request already in progress, abandoning", addr, port);
			curr_block = 0; // reset and abandon the current WRQ

			return;
		}
	} else if(opcode == TFTP_DATA) {
		if(curr_block == 0) {
			tftp_err("no write request in progress", addr, port);
			return;
		}

		if(tftp_send) {
			tftp_err("server busy", addr, port);
			return;
		}

		if(p->len - 4 > 512) {
			tftp_err("too much data in one packet", addr, port);
			return;
		}

		uint16_t block_num = ntohs(*((uint16_t*)(p->payload + 2)));

		if(block_num != curr_block) {
			tftp_err("unexpected block number", addr, port);
			return;
		}

		// copy the data into the buffer
		if(tftp_buff_index + p->len - 4 >= TFTP_BUFFER_SIZE) {
			tftp_err("file too large", addr, port);
			tftp_buff_index = 0;
			curr_block = 0;

			return;
		}

		uint8_t* data = (uint8_t*)(p->payload + 4);
		memcpy(tftp_buff + tftp_buff_index, data, p->len - 4);

		tftp_buff_index += (p->len - 4);

		// send the ACK
		tftp_ack(curr_block, addr, port);

		if(p->len - 4 < 512) {
			// last data packet, end of transfer

			if(parse_config_commands(addr, port)) {
				tftp_ack(curr_block, addr, port);
			} // otherwise sent an error message

			// mark the end of the transfer regardless
			tftp_buff_index = 0;
			curr_block = 0;
		} else {
			// otherwise we have more data to receive and expect the next block
			curr_block++;
		}
	} else if(opcode == TFTP_RRQ) {
		if(tftp_send) {
			tftp_err("server busy", addr, port);
			return;
		}

		// we don't care what the filename is as long as the mode is octet
		if(!tftp_check_octet_mode((char*)p->payload, p->len, addr, port)) {
			return;
		}

		last_addr = *addr;
		last_port = port;

		// we can't send immediately because we need to listen for ACKs
		tftp_send = 1;
	} else if(opcode == TFTP_ACK) {
		ack_num = ntohs(*(uint16_t*)(p->payload + 2));
	} else {
		tftp_err("unsupported opcode", addr, port);
	}
}

// initialize TFTP server
// should only be called once
void tftp_init() {
	tftp_pcb = udp_new();
	udp_bind(tftp_pcb, IP_ADDR_ANY, TFTP_PORT);
	tftp_p = pbuf_alloc(PBUF_TRANSPORT, sizeof(tftp_data_t) + 512, PBUF_RAM);

	// set the TFTP callback for any packets we receive
	udp_recv(tftp_pcb, &tftp_recv, NULL);
}

// loads network configuration from persistent flash memory into 'flash' variable
// if there is no configuration in flash (e.g. after re-flashing), writes defaults to flash
// returns: 1 on success, -1 on error
int load_flash_config(uint8_t force_default) {
	if(E_EEPROM_XMC4_IsFlashEmpty() || force_default) {
		// nothing in flash, set the defaults and save them to flash
		if(!ipaddr_aton(DEF_SRC_IP, &flash.src_ip)) {
			// bad address
			return -1;
		}

		if(!ipaddr_aton(DEF_DST_IP, &flash.dst_ip)) {
			// bad address
			return -1;
		}

		if(!ipaddr_aton(DEF_DEF_GW, &flash.default_gw)) {
			// bad address
			return -1;
		}

		if(!ipaddr_aton(DEF_SUBNET, &flash.subnet)) {
			// bad address
			return -1;
		}

		flash.src_port = DEF_SRC_PORT;
		flash.adc0_port = DEF_ADC0_PORT;
		flash.adc1_port = DEF_ADC1_PORT;
		flash.tc_port = DEF_TC_PORT;
		flash.adc0_rate = DEFAULT_ADC_RATE;
		flash.adc1_rate = DEFAULT_ADC_RATE;

		// write out the default configuration to flash
		return write_flash_config();
	} else {
		// we have a previously set configuration to load
		E_EEPROM_XMC4_ReadArray(0, (unsigned char*)&flash, sizeof(config_t));
	}

	return 1;
}

int write_flash_config() {
	if(E_EEPROM_XMC4_WriteArray(0x0, (unsigned char*)&flash, sizeof(config_t))) {
		if(E_EEPROM_XMC4_STATUS_OK != E_EEPROM_XMC4_UpdateFlashContents()) {
			return -1;
		}
	} else {
		return -1;
	}

	return 1;
}

// reset UDP configuration
// assumes 'local_udp_init' has been called but one of the addresses/ports in 'flash' has changed
void local_udp_reset() {
	// bind to the source port
	udp_bind(pcb, IP_ADDR_ANY, flash.src_port);

	// only call this if want to change TFTP server address when config set
	// without this line, TFTP server address doesn't change until reboot in case of error
//	udp_bind(tftp_pcb, IP_ADDR_ANY, TFTP_PORT);

	// set IP addresses
	netif_set_addr(netif, &flash.src_ip, &flash.subnet, &flash.default_gw);
}

// initialize UDP interface
// should only be called once, if network configuration changes call 'local_udp_reset'
void local_udp_init() {
	pcb = udp_new();

	// allocate buffer at least the size of the largest packet we'll send
	if(sizeof(ADC_data_t) + sizeof(header_t) > sizeof(thermocouple_packet_t)) {
		p = pbuf_alloc(PBUF_TRANSPORT, sizeof(ADC_data_t) + sizeof(header_t), PBUF_RAM);
	} else {
		p = pbuf_alloc(PBUF_TRANSPORT, sizeof(thermocouple_packet_t), PBUF_RAM);
	}

	// set the interface to send on
	netif = ETH_LWIP_0.xnetif;

	// set addresses and things
	local_udp_reset();
}

// send data over the Ethernet
void send_data(void* data, uint16_t size, uint16_t port) {
	memcpy(p->payload, data, size);
	p->len = p->tot_len = size;

	udp_sendto_if(pcb, p, &flash.dst_ip, port, netif);
}

// definitions needed by main
void adc_register_config(ADC_rate_t rate);
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

	// load config from flash
	load_flash_config(REFLASH);

	// Initialize UDP interface
	local_udp_init();

	// Init TFTP server
	tftp_init();

	//Initialize ADCs
	//Unlock / Config
	// ADC0
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_0); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	adc_register_config(flash.adc0_rate);

	// ADC 1
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC,  SPI_MASTER_SS_SIGNAL_1); // Change slave
	for(int i = 0; i < 9000; i++) {}; // Dumb Delay (Remove?)
	adc_register_config(flash.adc1_rate);

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
					send_data((void*)&TC_buff, sizeof(thermocouple_packet_t), flash.tc_port);
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
				SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, (uint8_t*)&(ADC0_buff.data), sizeof(ADC_data_t) + 3);
				read_adc0 = 0; // Reset Flag

				// send out ADC0 packet
				ADC0_buff.header.seq_num = sequence_number;
				sequence_number++;
				// be careful not to send zeros
				send_data((void*)&ADC0_buff, sizeof(ADC_data_t) + sizeof(header_t), flash.adc0_port);
			}
		}

		// ADC1 SPI Transfers
		if (read_adc1) {
			if (!SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)) { // SPI not already in transaction
				SPI_MASTER_EnableSlaveSelectSignal(&SPI_MASTER_ADC, SPI_MASTER_SS_SIGNAL_1); // Change to ADC1
				SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, (uint8_t*)&(ADC1_buff.data), sizeof(ADC_data_t) + 3);
				read_adc1 = 0; // Reset Flag

				// send out ADC0 packet
				ADC1_buff.header.seq_num = sequence_number;
				sequence_number++;
				// be careful not to send zeros
				send_data((void*)&ADC1_buff, sizeof(ADC_data_t) + sizeof(header_t), flash.adc1_port);
			}
		}

		if(tftp_send) {
			// always just send out the help message
			tftp_send_data(help_msg, &last_addr, last_port);
			tftp_send = 0;
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

void adc_register_config(ADC_rate_t rate) {
	// Register Configurations
		uint8_t unlock[3] = {0x06, 0x55, 0x0}; 				// Unlocks ADC
		uint8_t null[18] = {0x00};							// Sends null for reads
		uint8_t write_A_SYS_CFG[3] = {0x4B, 0x68, 0x00};	// b(01101000) -- Neg Charge Pump Powered Down | High-Res | 2.442 Internal Reference | Internal Voltage Enabled | 5/95% Comparator Threshold
		uint8_t write_D_SYS_CFG[3] = {0x4C, 0x3C, 0x00};	// b(00111100) -- Watchdog Disabled | No CRC | 12ns delay for DONE (not used) | 12ns delay for Hi-Z on DOUT | Fixed Frame Size (6 frames) | CRC disabled
		uint8_t write_CLK1[3] = {0x4D, 0x02, 0x00};			// b(00000010) -- XTAL CLK Source | CLKIN /2
		uint8_t write_CLK2_43kHz[3] = {0x4E, 0x4E, 0x00};	// b(01001110) -- ICLK / 4 | OSR = fMOD / 48
		uint8_t write_CLK2_8kHz[3] = {0x4E, 0x48, 0x00};	// b(01001000) -- ICLK / 4 | OSR = fMOD / 256


		uint8_t* write_CLK2 = NULL;
		if(rate == FAST_RATE) {
			write_CLK2 = write_CLK2_43kHz;
		} else {
			write_CLK2 = write_CLK2_8kHz;
		}

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
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, write_CLK2, configArray+24, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

	// Transfer Null
		SPI_MASTER_Transfer(&SPI_MASTER_ADC, null, configArray+27, 3U);
		while(SPI_MASTER_IsRxBusy(&SPI_MASTER_ADC)){} // Wait for completion

}

// perform a software reset on the ADC
// blocking operation
void adc_soft_reset() {
	uint8_t reset_cmd = {0x00, 0x11, 0x00};

	// could do SPI transfer and read first 24 bits (status word)
	// status should be READY (0xFFdd) where dd is the device id

	while(SPI_MASTER_IsTxBusy(&SPI_MASTER_ADC)) {};
	SPI_MASTER_Transmit(&SPI_MASTER_ADC, reset_cmd, 3);
	while(SPI_MASTER_IsTxBusy(&SPI_MASTER_ADC)) {};
}

void xmc_ADC_setup(){
	uint8_t null[18] = {0x00};						// Sends null for reads
	// I actually think the ADC_ENA register address is 0x04 (was 0x4F before)
	uint8_t write_ADC_ENA[3] = {0x0F, 0x0F, 0x00};	// b(00001111) -- Enables all ADC channels (note: no option to enable certain channels, all or nothing)
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
