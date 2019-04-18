#include <asf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conf_board.h"
#include "conf_example.h"
#include "conf_uart_serial.h"

#define MAX_ENTRIES        3
#define STRING_LENGTH     70
#define USART_TX_MAX_LENGTH     0xff

struct ili9488_opt_t g_ili9488_display_opt;