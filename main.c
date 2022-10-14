#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>

#include "lib/usart/usart.h"
#include "lib/oled/oled.h"
#include "lib/adc/adc.h"
#include "lib/joystick/joystick.h"
#include "lib/menu/menu.h"
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"

#define FOSC 4951200  // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1

// void SRAM_test() {
//   volatile char *ext_ram = (char *)0x1800;  // Start address for the SRAM
//   uint16_t ext_ram_size = 0x800;
//   uint16_t write_errors = 0;
//   uint16_t retrieval_errors = 0;

//   printf("Starting SRAM test...\n");
//   // rand() stores some internal state, so calling this function in a loop will
//   // yield different seeds each time (unless srand() is called before this
//   // function)
//   uint16_t seed = rand();
//   // Write phase: Immediately check that the correct value was stored
//   srand(seed);
//   for (uint16_t i = 0; i < ext_ram_size; i++) {
//     uint8_t some_value = rand();
//     ext_ram[i] = some_value;
//     uint8_t retreived_value = ext_ram[i];
//     if (retreived_value != some_value) {
//       printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,
//              retreived_value, some_value);
//       write_errors++;
//     }
//   }

//   // Retrieval phase: Check that no values were changed during or after the
//   // write phase
//   srand(seed);
//   // reset the PRNG to the state it had before the write phase
//   for (uint16_t i = 0; i < ext_ram_size; i++) {
//     uint8_t some_value = rand();
//     uint8_t retreived_value = ext_ram[i];
//     if (retreived_value != some_value) {
//       printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,
//              retreived_value, some_value);
//       retrieval_errors++;
//     }
//   }
//   printf(
//       "SRAM test completed with \n%4d errors in write phase and \n%4d errors "
//       "in retrieval phase\n\n",
//       write_errors, retrieval_errors);
// }

// void option1_fn() {
//   printf("\n\n\nSELECTED OPTION 1!!! <<<<<<<<<<<<<<<<<<\n");

//   SCREEN_line(10, 10, 60, 60);
// }

// void option2_fn() {
//   printf("\n\n\nSELECTED OPTION 2!!! <<<<<<<<<<<<<<<<<<\n");

//   SCREEN_circle(50, 50, 10);
// }

// void option3_fn() {
//   printf("\n\n\nSELECTED OPTION 3!!! <<<<<<<<<<<<<<<<<<\n");

//   SCREEN_reset();
// }

uint8_t reset_cmd[] = {0xC0};
uint8_t reset_wr[] = {0x10};

void spi_write() {
  printf("\n\n\nSELECTED SPI WRITE!!! <<<<<<<<<<<<<<<<<<\n");

  // Reset
  MCP2515_reset();
  printf("Reset 1\n");

  MCP2515_reset();
  printf("Reset 2\n");

  // Clear masks
  MCP2515_write_reg(MCP_RXM0SIDH, 0x00);
  MCP2515_write_reg(MCP_RXM0SIDL, 0x00);

  // Set interrupts
  MCP2515_write_reg(MCP_CANINTE, 0x01);
  
  
  SPI_send_length("\x02\x0f\x40", 3);
  
  // Verify device entered Loopback mode
  uint8_t dummy = MCP2515_read_reg(MCP_CANSTAT);
  if (0x40 != (dummy && 0xE0)) {
    SPI_send_length("\x02\x0f\x40", 3);
  }
  printf("READ MCP_CANSTAT 0x%x\n", SPI_send_length("\x03\x0e\x00", 3));

  MCP2515_write((message_t){0x01, "\xAA\xBB\xCC\xDD\xEE\xFF\x00\x00", 8, DATA_FRAME});
  MCP2515_rts();

  while(MCP2515_read_reg(MCP_CANINTF) & MCP_RX0IF == 0);

  printf("READ BYTE: 0x%02x\n", MCP2515_read_byte());

  message_t res = MCP2515_read();
  
  printf("READ ID: %x\n READ LENGTH: %d\n READ REMOTE: %d\n READ DATA: 0x", res.id, res.length, res.remote);

  for (int i = 0; i < res.length; i++) {\
    printf("%02x", res.data[i]);
  }

  printf("\n\n");

  
  
  MCP2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0x00);
	  


  // Set loopback mode
  // MCP2515_bit_modify(MCP_CANCTRL, (1 << 6), MODE_LOOPBACK);
  // SPI_send("\x0f\x80", 2);
  

  // MCP2515_write(0x55);
  // MCP2515_rts();
  // printf("Waiting\n");
  // printf("READ DATA: %x \n\n\n", MCP2515_read());
  // /*printf("READ MCP_CANTRL 0x%x\n", */SPI_send("\x03\x0f\x00", 3);//);
  // /*printf("READ MCP_CANSTAT 0x%x\n", */SPI_send("\x03\x0e\x00", 3);//);
  // /*printf("READ STATUS 0x%x\n", */MCP2515_read_status();//);
}

void spi_read() {
  for (int i = 0; i < 100; i++) {
    SPI_send("\x03\x0e\x00");
  }
  
  SPI_send("\x03\x30\x00");
  SPI_send("\x03\x40\x00");
  SPI_send("\x03\x50\x00");
}

void spi_reset() {
  MCP2515_reset();
}


// void spi_status() {
//   MCP2515_read_status();
// }

// menu_option_t option1 = {"Line       ", option1_fn};
// menu_option_t option2 = {"Circle     ", option2_fn};
// menu_option_t option3 = {"Reset      ", option3_fn};
menu_option_t option4 = {"CANInit", spi_write};
// menu_option_t option5 = {"CANReset   ", spi_reset};
// menu_option_t option6 = {"ReadCANSTAT", spi_read};
// menu_option_t option7 = {"ReadStatus ", spi_status};

int main() {
  USART_Initialize(MYUBRR);
  JOYSTICK_init();

  MCUCR |= (1 << SRE);
  SFIOR |= (1 << XMM2);

  ADC_init();

  SPI_init(CPOL_LOW, CPHA_LEADING, DORD_MSB);

  // SRAM_test();

	//// OLED Init
  OLED_init();
  SCREEN_reset();
  // SCREEN_goto_line(4);
  // OLED_goto_line(1);
  
  // SCREEN_print("Ia Tsomaia", SCREEN_print_char8);
  // OLED_set_brightness(64);
  // SCREEN_goto_line(1);
  // SCREEN_print("Juanpi", SCREEN_print_char4);

  // SCREEN_line(10, 10, 60, 60);
// 

  MENU_init();
  // MENU_add_option(&option1);
  // MENU_add_option(&option2);
  // MENU_add_option(&option3);
  MENU_add_option(&option4);
  // MENU_add_option(&option5);
  // MENU_add_option(&option6);
  // MENU_add_option(&option7);
  
  
  while (1) {
    joy_t joystick = JOYSTICK_get_data(OFFSET);
    adc_t adc = ADC_get_data(NO_CORRECTION);
    if (joystick.direction == NEUTRAL) {
      // printf("SLIDER1: %ld \t SLIDER2: %ld \t BUTTON 1: %d \t  BUTTON 2: %d \n\r", adc.AIN0, adc.AIN1, (PINB & 0x01), ((PINB & (1 << 1)) >> 1));
    } else {
      // printf("DATA[X]: %ld \t DATA[Y]: %ld \t %s \r\n", joystick.x, joystick.y, JOY_DIRECTION_STRINGS[joystick.direction]);
    }
  }

  return 0;
}
