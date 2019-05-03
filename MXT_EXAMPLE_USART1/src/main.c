/**
 * \file
 *
 * \brief Example of usage of the maXTouch component with USART
 *
 * This example shows how to receive touch data from a maXTouch device
 * using the maXTouch component, and display them in a terminal window by using
 * the USART driver.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage
 *
 * \section intro Introduction
 * This simple example reads data from the maXTouch device and sends it over
 * USART as ASCII formatted text.
 *
 * \section files Main files:
 * - example_usart.c: maXTouch component USART example file
 * - conf_mxt.h: configuration of the maXTouch component
 * - conf_board.h: configuration of board
 * - conf_clock.h: configuration of system clock
 * - conf_example.h: configuration of example
 * - conf_sleepmgr.h: configuration of sleep manager
 * - conf_twim.h: configuration of TWI driver
 * - conf_usart_serial.h: configuration of USART driver
 *
 * \section apiinfo maXTouch low level component API
 * The maXTouch component API can be found \ref mxt_group "here".
 *
 * \section deviceinfo Device Info
 * All UC3 and Xmega devices with a TWI module can be used with this component
 *
 * \section exampledescription Description of the example
 * This example will read data from the connected maXTouch explained board
 * over TWI. This data is then processed and sent over a USART data line
 * to the board controller. The board controller will create a USB CDC class
 * object on the host computer and repeat the incoming USART data from the
 * main controller to the host. On the host this object should appear as a
 * serial port object (COMx on windows, /dev/ttyxxx on your chosen Linux flavour).
 *
 * Connect a terminal application to the serial port object with the settings
 * Baud: 57600
 * Data bits: 8-bit
 * Stop bits: 1 bit
 * Parity: None
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "main.h"

volatile int n_botoes_na_tela = 0;
volatile uint8_t print_time_value;
volatile int time_left = 0;

/*
* 0 - diaria
* 1 - pesada
* 2 - rapida
*/
volatile int laundry_event = 0;

struct botao botoes[10];

volatile bool unlocked_flag = true;
volatile bool door_open;
volatile bool lock_unlock;


void door_callback(){
	door_open = !door_open;
}

void home_callback(void){
	if(unlocked_flag){
		draw_screen();
		switch(laundry_event){
			case 0:
				draw_diary_page();
				botoes[0] = botaoLavagemPesada;
				break;
			
			case 1:
				draw_heavy_page();
				botoes[0] = botaoLavagemRapida;
				break;
				
			case 2:
				draw_fast_page();
				botoes[0] = botaoLavagemDiaria;
				break;
		}
		botoes[1] = botaoDireita;
		botoes[2] = botaoEsquerda;
		botoes[3] = botaoUnlock;
		botoes[4] = botaoLock;
		n_botoes_na_tela = 5;
	}
}

void play_pause_callback(void){
	if(unlocked_flag){
		draw_screen();
	
		if(!door_open){
			
			switch(laundry_event){
				case 0:
					time_left = calculate_total_time(c_diario);
					break;
				
				case 1:
					time_left = calculate_total_time(c_pesado);
					break;
				
				case 2:
					time_left = calculate_total_time(c_rapido);
					break;
			}
			n_botoes_na_tela = 0;
			
			draw_working(time_left);
	
			rtc_enable_interrupt(RTC,  RTC_IER_SECEN);
		}
		else{
			draw_door_open();
		}
	}
}

void lavagem_callback(void){
	if(unlocked_flag){
		draw_screen();
		draw_laundry_menu();
		
		botoes[0] = botaoHome;
		botoes[1] = botaoPlayPause;
		botoes[2] = botaoUnlock;
		botoes[3] = botaoLock;
		
		n_botoes_na_tela = 4;
	}
}

void slice_right_callback(void){
	
	if(unlocked_flag){
		draw_screen();
		switch(laundry_event){
			case 0:
				laundry_event = 1;
				draw_heavy_page();
				botoes[0] = botaoLavagemPesada;
				break;
			
			case 1:
				laundry_event = 2;
				draw_fast_page();
				botoes[0] = botaoLavagemRapida;
				break;
			
			case 2:
				laundry_event = 0;
				draw_diary_page();
				botoes[0] = botaoLavagemDiaria;
				break;
		}
		botoes[1] = botaoDireita;
		botoes[2] = botaoEsquerda;
		botoes[3] = botaoUnlock;
		botoes[4] = botaoLock;
		n_botoes_na_tela = 5;
	}
}

void slice_left_callback(void){
	
	if(unlocked_flag){
		draw_screen();
		switch(laundry_event){
			case 0:
				laundry_event = 2;
				draw_fast_page();
				botoes[0] = botaoLavagemRapida;
				break;
			
			case 1:
				laundry_event = 0;
				draw_diary_page();
				botoes[0] = botaoLavagemDiaria;
				break;
			
			case 2:
				laundry_event = 1;
				draw_heavy_page();
				botoes[0] = botaoLavagemPesada;
				break;
		}

		botoes[1] = botaoDireita;
		botoes[2] = botaoEsquerda;
		botoes[3] = botaoUnlock;
		botoes[4] = botaoLock;
		n_botoes_na_tela = 5;
	}
	
}

void unlock_callback(void){
	if(unlocked_flag == true){
		//deixa lugar do botao branco
		ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
		ili9488_draw_filled_rectangle(botaoLock.x, botaoLock.y, botaoLock.x + botaoLock.image->width, botaoLock.y + botaoLock.image->height);
		ili9488_draw_filled_rectangle(botaoUnlock.x, botaoUnlock.y, botaoUnlock.x + botaoUnlock.image->width, botaoUnlock.y + botaoUnlock.image->height);
		
		ili9488_draw_pixmap(botaoLock.x,
							botaoLock.y,
							botaoLock.image->width,
							botaoLock.image->height,
							botaoLock.image->data);
		unlocked_flag = false;
		pio_enable_interrupt(UNLOCK_PIO, UNLOCK_PIO_IDX_MASK);
		return;
	}
}

void lock_callback(void){
	if (time_left <= 0){
		lock_unlock = true;
	}
}

int processa_touch(struct botao *b, struct botao *rtn, uint N ,uint x, uint y ){
	for (int i=0; i<N; i++){
		if (((x >= b->x) && (x <= b->x + b->size_x)) && ((y >= b->y) && (y <= b->y + b->size_y))){
			*rtn = *b;
			return 1;
		}
		b++;
	}
	return 0;
}

	
static void configure_lcd(void){
	/* Initialize display parameter */
	g_ili9488_display_opt.ul_width = ILI9488_LCD_WIDTH;
	g_ili9488_display_opt.ul_height = ILI9488_LCD_HEIGHT;
	g_ili9488_display_opt.foreground_color = COLOR_CONVERT(COLOR_WHITE);
	g_ili9488_display_opt.background_color = COLOR_CONVERT(COLOR_WHITE);
	
	/* Initialize LCD */
	ili9488_init(&g_ili9488_display_opt);
}

/**
 * \brief Set maXTouch configuration
 *
 * This function writes a set of predefined, optimal maXTouch configuration data
 * to the maXTouch Xplained Pro.
 *
 * \param device Pointer to mxt_device struct
 */
static void mxt_init(struct mxt_device *device)
{
	enum status_code status;

	/* T8 configuration object data */
	uint8_t t8_object[] = {
		0x0d, 0x00, 0x05, 0x0a, 0x4b, 0x00, 0x00,
		0x00, 0x32, 0x19
	};

	/* T9 configuration object data */
	uint8_t t9_object[] = {
		0x8B, 0x00, 0x00, 0x0E, 0x08, 0x00, 0x80,
		0x32, 0x05, 0x02, 0x0A, 0x03, 0x03, 0x20,
		0x02, 0x0F, 0x0F, 0x0A, 0x00, 0x00, 0x00,
		0x00, 0x18, 0x18, 0x20, 0x20, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x02,
		0x02
	};

	/* T46 configuration object data */
	uint8_t t46_object[] = {
		0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x03,
		0x00, 0x00
	};
	
	/* T56 configuration object data */
	uint8_t t56_object[] = {
		0x02, 0x00, 0x01, 0x18, 0x1E, 0x1E, 0x1E,
		0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
		0x1E, 0x1E, 0x1E, 0x1E, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00
	};

	/* TWI configuration */
	twihs_master_options_t twi_opt = {
		.speed = MXT_TWI_SPEED,
		.chip  = MAXTOUCH_TWI_ADDRESS,
	};

	status = (enum status_code)twihs_master_setup(MAXTOUCH_TWI_INTERFACE, &twi_opt);
	Assert(status == STATUS_OK);

	/* Initialize the maXTouch device */
	status = mxt_init_device(device, MAXTOUCH_TWI_INTERFACE,
			MAXTOUCH_TWI_ADDRESS, MAXTOUCH_XPRO_CHG_PIO);
	Assert(status == STATUS_OK);

	/* Issue soft reset of maXTouch device by writing a non-zero value to
	 * the reset register */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_COMMANDPROCESSOR_T6, 0)
			+ MXT_GEN_COMMANDPROCESSOR_RESET, 0x01);

	/* Wait for the reset of the device to complete */
	delay_ms(MXT_RESET_TIME);

	/* Write data to configuration registers in T7 configuration object */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 0, 0x20);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 1, 0x10);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 2, 0x4b);
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_POWERCONFIG_T7, 0) + 3, 0x84);

	/* Write predefined configuration data to configuration objects */
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_GEN_ACQUISITIONCONFIG_T8, 0), &t8_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_TOUCH_MULTITOUCHSCREEN_T9, 0), &t9_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_SPT_CTE_CONFIGURATION_T46, 0), &t46_object);
	mxt_write_config_object(device, mxt_get_object_address(device,
			MXT_PROCI_SHIELDLESS_T56, 0), &t56_object);

	/* Issue recalibration command to maXTouch device by writing a non-zero
	 * value to the calibrate register */
	mxt_write_config_reg(device, mxt_get_object_address(device,
			MXT_GEN_COMMANDPROCESSOR_T6, 0)
			+ MXT_GEN_COMMANDPROCESSOR_CALIBRATE, 0x01);
}

void draw_screen(void) {
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	ili9488_draw_filled_rectangle(0, 0, ILI9488_LCD_WIDTH-1, ILI9488_LCD_HEIGHT-1);
}

uint32_t convert_axis_system_x(uint32_t touch_y) {
	// entrada: 4096 - 0 (sistema de coordenadas atual)
	return ILI9488_LCD_HEIGHT - ILI9488_LCD_HEIGHT*touch_y/4096;
}

uint32_t convert_axis_system_y(uint32_t touch_x) {
	// entrada: 0 - 4096 (sistema de coordenadas atual)
	
	return ILI9488_LCD_WIDTH - ILI9488_LCD_WIDTH*touch_x/4096;
}

void mxt_handler(struct mxt_device *device, struct botao *botoes, uint Nbotoes)
{
	/* USART tx buffer initialized to 0 */
	char tx_buf[STRING_LENGTH * MAX_ENTRIES] = {0};
	uint8_t i = 0; /* Iterator */

	/* Temporary touch event data struct */
	struct mxt_touch_event touch_event;

	/* Collect touch events and put the data in a string,
	 * maximum 2 events at the time */
	do {
		/* Temporary buffer for each new touch event line */
		char buf[STRING_LENGTH];
	
		/* Read next next touch event in the queue, discard if read fails */
		if (mxt_read_touch_event(device, &touch_event) != STATUS_OK) {
			continue;
		}
		
		 // eixos trocados (quando na vertical LCD)
		//uint32_t conv_x = convert_axis_system_x(touch_event.y);
		//uint32_t conv_y = convert_axis_system_y(touch_event.x);
		uint32_t conv_x = convert_axis_system_y(touch_event.x);
		uint32_t conv_y = convert_axis_system_x(touch_event.y);
		
		/* Format a new entry in the data string that will be sent over USART */
		sprintf(buf, "X:%3d Y:%3d \n", conv_x, conv_y);
		
		if (touch_event.status == 32){
			/* -----------------------------------------------------*/
			struct botao bAtual;
			if(processa_touch(botoes, &bAtual, Nbotoes, conv_x, conv_y)){
				bAtual.p_handler();
			}
			//update_screen(conv_x, conv_y);
			/* -----------------------------------------------------*/
		}
		/* Add the new string to the string buffer */
		strcat(tx_buf, buf);
		i++;

		/* Check if there is still messages in the queue and
		 * if we have reached the maximum numbers of events */
	} while ((mxt_is_message_pending(device)) & (i < MAX_ENTRIES));

	/* If there is any entries in the buffer, send them over USART */
	if (i > 0) {
		usart_serial_write_packet(USART_SERIAL_EXAMPLE, (uint8_t *)tx_buf, strlen(tx_buf));
	}
}

void build_laundry_types(){
	//c_rapido.nome = "Rapido";
	c_rapido.enxagueTempo = 5;
	c_rapido.enxagueQnt = 3;
	c_rapido.centrifugacaoRPM = 900;
	c_rapido.centrifugacaoTempo = 5;
	c_rapido.heavy = 0;
	c_rapido.bubblesOn = 1;

	//c_diario.nome = "Diario";
	c_diario.enxagueTempo = 15;
	c_diario.enxagueQnt = 2;
	c_diario.centrifugacaoRPM = 1200;
	c_diario.centrifugacaoTempo = 8;
	c_diario.heavy = 0;
	c_diario.bubblesOn = 1;
	
	//c_pesado.nome = "Pesado";
	c_pesado.enxagueTempo = 10;
	c_pesado.enxagueQnt = 3;
	c_pesado.centrifugacaoRPM = 1200;
	c_pesado.centrifugacaoTempo = 10;
	c_pesado.heavy = 1;
	c_pesado.bubblesOn = 1;
}

void build_buttons(){
	botaoLavagemDiaria.x = 150;
	botaoLavagemDiaria.y = 50;
	botaoLavagemDiaria.size_x = 180;
	botaoLavagemDiaria.size_y = 180;
	botaoLavagemDiaria.p_handler = lavagem_callback;
	botaoLavagemDiaria.image = &diario;
	
	botaoLavagemPesada.x = 150;
	botaoLavagemPesada.y = 50;
	botaoLavagemPesada.size_x = 180;
	botaoLavagemPesada.size_y = 180;
	botaoLavagemPesada.p_handler = lavagem_callback;
	botaoLavagemPesada.image = &pesado;
	
	botaoLavagemRapida.x = 150;
	botaoLavagemRapida.y = 50;
	botaoLavagemRapida.size_x = 180;
	botaoLavagemRapida.size_y = 180;
	botaoLavagemRapida.p_handler = lavagem_callback;
	botaoLavagemRapida.image = &rapido;

	botaoDireita.x = 400;
	botaoDireita.y = 90;
	botaoDireita.size_x = 75;
	botaoDireita.size_y = 110;
	botaoDireita.p_handler = slice_right_callback;
	botaoDireita.image = &right_arrow;
	
	botaoEsquerda.x = 20;
	botaoEsquerda.y = 90;
	botaoEsquerda.size_x = 75;
	botaoEsquerda.size_y = 110;
	botaoEsquerda.p_handler = slice_left_callback;
	botaoEsquerda.image = &left_arrow;

	botaoUnlock.x = 400;
	botaoUnlock.y = 240;
	botaoUnlock.size_x = 70;
	botaoUnlock.size_y = 70;
	botaoUnlock.p_handler = unlock_callback;
	botaoUnlock.image = &unlock;
	
	botaoLock.x = 20;
	botaoLock.y = 240;
	botaoLock.size_x = 70;
	botaoLock.size_y = 70;
	botaoLock.image = &lock;
	
	botaoHome.x = 20;
	botaoHome.y = 90;
	botaoHome.size_x = 100;
	botaoHome.size_y = 100;
	botaoHome.p_handler = home_callback;
	botaoHome.image = &home;
	
	botaoPlayPause.x = 380;
	botaoPlayPause.y = 90;
	botaoPlayPause.size_x = 100;
	botaoPlayPause.size_y = 100;
	botaoPlayPause.p_handler = play_pause_callback;
	botaoPlayPause.image = &play_pause;
		
	botaoOk.x = 175;
	botaoOk.y = 105;
	botaoOk.size_x = 100;
	botaoOk.size_y = 100;
	botaoOk.p_handler = home_callback;
	botaoOk.image = &OK;
	
	imageNop.x = 115;
	imageNop.y = 35;
	imageNop.size_x = 251;
	imageNop.size_y = 251;
	imageNop.image = &nopImage;
}

void draw_laundry_menu(){
	ili9488_draw_pixmap(botaoHome.x,
	botaoHome.y,
	botaoHome.image->width,
	botaoHome.image->height,
	botaoHome.image->data);
	
	ili9488_draw_pixmap(botaoPlayPause.x,
	botaoPlayPause.y,
	botaoPlayPause.image->width,
	botaoPlayPause.image->height,
	botaoPlayPause.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(botaoHome.x + 25,
						botaoHome.y + botaoHome.image->height + 10,
						"HOME" );
	
	ili9488_draw_string(botaoPlayPause.x + 5,
	botaoPlayPause.y + botaoPlayPause.image->height + 10,
	"INICIAR" );
	
	ili9488_draw_pixmap(botaoUnlock.x,
	botaoUnlock.y,
	botaoUnlock.image->width,
	botaoUnlock.image->height,
	botaoUnlock.image->data);
	
	switch(laundry_event){
		case 0:
			time_left = calculate_total_time(c_diario);
			break;
		
		case 1:
			time_left = calculate_total_time(c_pesado);
			break;
		
		case 2:
			time_left = calculate_total_time(c_rapido);
			break;
	}
	
	
	char total_left[32];
	
	sprintf(total_left,"%02d",time_left);
	font_draw_text(&calibri_36, total_left, 180, 150, 1);
	
	ili9488_draw_string(225, 160, "MINUTOS" );
	
	time_left = 0;
}

void draw_diary_page(){
	ili9488_draw_pixmap(botaoLavagemDiaria.x,
	botaoLavagemDiaria.y,
	botaoLavagemDiaria.image->width,
	botaoLavagemDiaria.image->height,
	botaoLavagemDiaria.image->data);
	
	ili9488_draw_pixmap(botaoDireita.x,
	botaoDireita.y,
	botaoDireita.image->width,
	botaoDireita.image->height,
	botaoDireita.image->data);
	
	ili9488_draw_pixmap(botaoEsquerda.x,
	botaoEsquerda.y,
	botaoEsquerda.image->width,
	botaoEsquerda.image->height,
	botaoEsquerda.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(botaoLavagemDiaria.x + 5,
						botaoLavagemDiaria.y + botaoLavagemDiaria.image->height + 10,
						"LAVAGEM DIARIA" );
						
	ili9488_draw_pixmap(botaoUnlock.x,
	botaoUnlock.y,
	botaoUnlock.image->width,
	botaoUnlock.image->height,
	botaoUnlock.image->data);
}

void draw_fast_page(){
	ili9488_draw_pixmap(botaoLavagemRapida.x,
	botaoLavagemRapida.y,
	botaoLavagemRapida.image->width,
	botaoLavagemRapida.image->height,
	botaoLavagemRapida.image->data);
	
	ili9488_draw_pixmap(botaoDireita.x,
	botaoDireita.y,
	botaoDireita.image->width,
	botaoDireita.image->height,
	botaoDireita.image->data);
	
	ili9488_draw_pixmap(botaoEsquerda.x,
	botaoEsquerda.y,
	botaoEsquerda.image->width,
	botaoEsquerda.image->height,
	botaoEsquerda.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(botaoLavagemRapida.x + 5,
						botaoLavagemRapida.y + botaoLavagemRapida.image->height + 10,
						"LAVAGEM RAPIDA" );
						
	ili9488_draw_pixmap(botaoUnlock.x,
	botaoUnlock.y,
	botaoUnlock.image->width,
	botaoUnlock.image->height,
	botaoUnlock.image->data);
}

void draw_heavy_page(){
	ili9488_draw_pixmap(botaoLavagemPesada.x,
	botaoLavagemPesada.y,
	botaoLavagemPesada.image->width,
	botaoLavagemPesada.image->height,
	botaoLavagemPesada.image->data);
	
	ili9488_draw_pixmap(botaoDireita.x,
	botaoDireita.y,
	botaoDireita.image->width,
	botaoDireita.image->height,
	botaoDireita.image->data);
	
	ili9488_draw_pixmap(botaoEsquerda.x,
	botaoEsquerda.y,
	botaoEsquerda.image->width,
	botaoEsquerda.image->height,
	botaoEsquerda.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(botaoLavagemPesada.x + 5,
						botaoLavagemPesada.y + botaoLavagemPesada.image->height + 10,
						"LAVAGEM PESADA" );
						
						
	ili9488_draw_pixmap(botaoUnlock.x,
	botaoUnlock.y,
	botaoUnlock.image->width,
	botaoUnlock.image->height,
	botaoUnlock.image->data);
}

void RTC_Handler(void)
{
	uint32_t ul_status = rtc_get_status(RTC);

	/*
	*  Verifica por qual motivo entrou
	*  na interrupcao, se foi por segundo
	*  ou Alarm
	*/
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		rtc_clear_status(RTC, RTC_SCCR_SECCLR);
		print_time_value = 1u;
	}
	
	/* Time or date alarm */
	if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
		rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
	}
	
	rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
	rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
	rtc_clear_status(RTC, RTC_SCCR_CALCLR);
	rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);	
}

void RTC_init(){
	/* Configura o PMC */
	pmc_enable_periph_clk(ID_RTC);

	/* Default RTC configuration, 24-hour mode */
	rtc_set_hour_mode(RTC, 0);

	/* Configura data e hora manualmente */
	rtc_set_date(RTC, YEAR, MOUNTH, DAY, WEEK);
	rtc_set_time(RTC, HOUR, MINUTE, SECONDS);

	/* Configure RTC interrupts */
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
}

int calculate_total_time(t_ciclo act_cicle){
	int total = act_cicle.centrifugacaoTempo + 
				(act_cicle.enxagueQnt * act_cicle.enxagueTempo);
	return total;
}

void font_draw_text(tFont *font, const char *text, int x, int y, int spacing) {
	char *p = text;
	while(*p != NULL) {
		char letter = *p;
		int letter_offset = letter - font->start_char;
		if(letter <= font->end_char) {
			tChar *current_char = font->chars + letter_offset;
			ili9488_draw_pixmap(x, y, current_char->image->width, current_char->image->height, current_char->image->data);
			x += current_char->image->width + spacing;
		}
		p++;
	}
}

void draw_done_laundry(){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	draw_screen();
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(135, 75, "LAVAGEM CONCLUIDA" );
	
	ili9488_draw_pixmap(botaoOk.x,
	botaoOk.y,
	botaoOk.image->width,
	botaoOk.image->height,
	botaoOk.image->data);
	
	botoes[0] = botaoOk;
	n_botoes_na_tela = 1;
}

void draw_working(int time_left){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	draw_screen();
	
	char total_left[32];
	
	sprintf(total_left,"%02d",time_left);
	font_draw_text(&arial_72, total_left, 120, 90, 1);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(210, 140, "MINUTOS RESTANTES" );
	
}

void draw_door_open(){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	draw_screen();
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(135, 75, "A PORTA ESTA ABERTA" );
	
	ili9488_draw_pixmap(botaoOk.x,
	botaoOk.y,
	botaoOk.image->width,
	botaoOk.image->height,
	botaoOk.image->data);
	
	botoes[0] = botaoOk;
	n_botoes_na_tela = 1;
	
}

void draw_locked_door(){
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
	draw_screen();
	
	ili9488_draw_pixmap(imageNop.x,
	imageNop.y,
	imageNop.image->width,
	imageNop.image->height,
	imageNop.image->data);
	
	ili9488_set_foreground_color(COLOR_CONVERT(COLOR_BLACK));
	ili9488_draw_string(imageNop.x + 20, 
						imageNop.y + imageNop.image->width + 10,
						"PORTA TRANCADA" );
	
	delay_ms(1000);
	time_left -= 1;
}

void print_time(void){	
	time_left--;
	
	if(time_left>0){	
		draw_working(time_left);
	}
	else{
		rtc_disable_interrupt(RTC,RTC_IER_SECEN);
		draw_done_laundry();
	}
}

void init_led(void){
	pmc_enable_periph_clk(LED1_PIO_ID);
	
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
}

void init_but(void){
	pmc_enable_periph_clk(DOOR_PIO_ID);
	pio_set_input(DOOR_PIO, DOOR_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	
	pio_enable_interrupt(DOOR_PIO, DOOR_PIO_IDX_MASK);
	pio_handler_set(DOOR_PIO, DOOR_PIO_ID, DOOR_PIO_IDX_MASK, PIO_IT_FALL_EDGE, door_callback);
	NVIC_EnableIRQ(DOOR_PIO_ID);
	NVIC_SetPriority(DOOR_PIO_ID, 3);
	
	pmc_enable_periph_clk(UNLOCK_PIO_ID);
	pio_set_input(UNLOCK_PIO, UNLOCK_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	
	pio_handler_set(UNLOCK_PIO, UNLOCK_PIO_ID, UNLOCK_PIO_IDX_MASK, PIO_IT_FALL_EDGE, lock_callback);
	NVIC_EnableIRQ(UNLOCK_PIO_ID);
	NVIC_SetPriority(UNLOCK_PIO_ID, 3);
}

void do_unlock(void){
	if(unlocked_flag == false){
		//deixa lugar do botao branco
		ili9488_set_foreground_color(COLOR_CONVERT(COLOR_WHITE));
		ili9488_draw_filled_rectangle(botaoUnlock.x, botaoUnlock.y, botaoUnlock.x + botaoUnlock.image->width, botaoUnlock.y + botaoUnlock.image->height);
		ili9488_draw_filled_rectangle(botaoLock.x, botaoLock.y, botaoLock.x + botaoLock.image->width, botaoLock.y + botaoLock.image->height);

		
		ili9488_draw_pixmap(botaoUnlock.x,
		botaoUnlock.y,
		botaoUnlock.image->width,
		botaoUnlock.image->height,
		botaoUnlock.image->data);
		unlocked_flag = true;
		pio_disable_interrupt(UNLOCK_PIO, UNLOCK_PIO_IDX_MASK);
		return;
	}
}

int main(void)
{
	struct mxt_device device; /* Device data container */

	/* Initialize the USART configuration struct */
	const usart_serial_options_t usart_serial_options = {
		.baudrate     = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength   = USART_SERIAL_CHAR_LENGTH,
		.paritytype   = USART_SERIAL_PARITY,
		.stopbits     = USART_SERIAL_STOP_BIT
	};

	sysclk_init(); /* Initialize system clocks */
	board_init();  /* Initialize board */
	configure_lcd();
	draw_screen();
	init_led();
	init_but();
	
	/* Initialize the mXT touch device */
	mxt_init(&device);
	
	/* Initialize stdio on USART */
	stdio_serial_init(USART_SERIAL_EXAMPLE, &usart_serial_options);

	printf("\n\rmaXTouch data USART transmitter\n\r");
	
	RTC_init();
	
	build_buttons();
	build_laundry_types();
	draw_diary_page();
				
	botoes[0] = botaoLavagemDiaria;
	botoes[1] = botaoDireita;
	botoes[2] = botaoEsquerda;
	botoes[3] = botaoUnlock;
	botoes[4] = botaoLock;
	
	n_botoes_na_tela = 5;
	print_time_value = 0u;
	door_open = false;
	lock_unlock = false;
		
	while (true) {
		/* Check for any pending messages and run message handler if any
		 * message is found in the queue */
		if (mxt_is_message_pending(&device)) {
			mxt_handler(&device, botoes, n_botoes_na_tela);
		}
		
		if (print_time_value){
			print_time();
			print_time_value = 0u;
		}
		
		if(door_open){
			if(time_left>0){
				draw_locked_door();
				door_open = false;
			}
			else{
				pio_set(LED1_PIO,LED1_PIO_IDX_MASK);
			}
		}
		else{
			pio_clear(LED1_PIO,LED1_PIO_IDX_MASK);
		}
		
		if (lock_unlock){
			do_unlock();
			lock_unlock = false;
		}
	}

	return 0;
}
