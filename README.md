# stm32f103_bootloader
📌 Overview

This project implements a custom UART-based bootloader for the STM32F103 microcontroller, along with a Windows-based host application to communicate with it.
The bootloader follows the AN3155 (USART protocol) specification and allows firmware updates and device interaction without requiring a hardware programmer.

Bootloader (STM32F103): listens on UART, executes commands.

Host Application (Windows): A console-based tool to send commands and manage firmware over UART.

🚀 Features

Currently, the following 6 bootloader commands are implemented:

1) Get Command (GET)
- Retrieves the bootloader version and list of supported commands.

2) Get Version
- Returns the bootloader version.

3) Get Chip ID (GET_ID)
- Reads the device’s unique identifier (STM32F103 series ID).

4) Go to address command(BL_GO_TO_ADDR)
- Jumps to the address entered in the command if it is valid address.

5) Write Memory (WRITE_MEM)
- Writes data to Flash, RAM, or option bytes depending on address.

6) Flash Erase (FLASH_ERASE)
- Sector wise or Chip Erase posible based on input.

✅ ACK/NACK protocol implemented.
✅ Checksum verification for command/data integrity.
✅ Flash write/erase supported.

/----- Steps to program the flash from the Bootloader -----/
1) Download the bootloader to microcntroller using st-link utility.
2) To Enter the Bootloader mode, pull the PORTB_0 pin to ground while you press the reset button.
3) In application code change the flash base address in linker file.
4) In the Bootloader code, at app.h file assign base address of the application flash to USER_APP_FLASH_BASE_ADDR macro.
5) Assign user application RAM base address to USER_APP_RAM_BASE_ADDR macro.
6) In the Host application, assign path of your application bin file to USER_APP macro in fileops.c file.
7) Now run the host application executive. And select the command 15 to flash the program. Host application will automatically download the .bin file to flash.

References
1) https://www.udemy.com/course/stm32f4-arm-cortex-mx-custom-bootloader-development/?couponCode=MT180825B
2) https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf
