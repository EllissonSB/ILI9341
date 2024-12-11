/*
 * ILI9341.c
 *
 *  Created on: Nov 27, 2024
 *      Author: ellis
 */

#include "ILI9341.h"
volatile uint16_t LCD_HEIGHT = ILI9341_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ILI9341_SCREEN_WIDTH;

/**
 * @brief  Initializes the ILI9341 display by setting the chip select pin to low.
 * 
 * This function configures the SPI interface for the ILI9341 display by 
 * pulling the chip select (CS) pin low, indicating the start of communication.
 * 
 * @note  Ensure that the GPIO port and pin for the chip select are correctly 
 *        defined and initialized before calling this function.
 */
void ILI9341_SPI_init(void){
		HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);

}
/**
 * @brief  Sends a single byte of data via SPI to the ILI9341 display.
 * @param  SPI_Data: The byte of data to be sent.
 * @retval None
 */
void ILI9341_SPI_SEND(unsigned char SPI_Data){
	HAL_SPI_Transmit(&hspi2, &SPI_Data, 1, 1);
}

/* Send command (char) to LCD */
/**
 * @brief  Sends a command to the ILI9341 display.
 * @param  Command: The command byte to be sent to the display.
 * @retval None
 *
 * This function sets the necessary GPIO pins to indicate that a command is being sent,
 * sends the command byte via SPI, and then resets the GPIO pins to their default state.
 */
void ILI9341_Write_Command(uint8_t Command)
{
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
	ILI9341_SPI_SEND(Command);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);
}

/* Send Data (char) to LCD */
/**
 * @brief  Writes a single byte of data to the ILI9341 display.
 * @param  Data: The byte of data to be sent to the display.
 * @retval None
 * 
 * This function sets the Data/Command (DC) pin to indicate that data is being sent,
 * selects the display by setting the Chip Select (CS) pin low, sends the data byte
 * via SPI, and then deselects the display by setting the CS pin high.
 */
void ILI9341_Write_Data(uint8_t Data)
{
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	ILI9341_SPI_SEND(Data);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);
}

/**
 * @brief Set the address window for drawing on the ILI9341 display.
 *
 * This function sets the rectangular area where subsequent drawing operations
 * will take place on the ILI9341 display. The area is defined by the coordinates
 * (X1, Y1) for the top-left corner and (X2, Y2) for the bottom-right corner.
 *
 * @param X1 The X coordinate of the top-left corner.
 * @param Y1 The Y coordinate of the top-left corner.
 * @param X2 The X coordinate of the bottom-right corner.
 * @param Y2 The Y coordinate of the bottom-right corner.
 */
/* Set Address - Location block - to draw into */
void ILI9341_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
	{
	ILI9341_Write_Command(0x2A);
	ILI9341_Write_Data(X1>>8);
	ILI9341_Write_Data(X1);
	ILI9341_Write_Data(X2>>8);
	ILI9341_Write_Data(X2);

	ILI9341_Write_Command(0x2B);
	ILI9341_Write_Data(Y1>>8);
	ILI9341_Write_Data(Y1);
	ILI9341_Write_Data(Y2>>8);
	ILI9341_Write_Data(Y2);

	ILI9341_Write_Command(0x2C);
}
/*HARDWARE RESET*/
/**
 * @brief  Resets the ILI9341 display.
 * 
 * This function performs a hardware reset on the ILI9341 display by toggling
 * the RESET and CHIP_SELECT pins. It first pulls the RESET pin low, waits for
 * 200 milliseconds, then pulls the CHIP_SELECT pin low, waits for another 200
 * milliseconds, and finally pulls the RESET pin high to complete the reset process.
 * 
 * @note This function uses HAL (Hardware Abstraction Layer) functions to control
 *       the GPIO pins and introduce delays.
 */
void ILI9341_Reset(void)
{
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
}

/*Ser rotation of the screen - changes x0 and y0*/
/**
 * @brief  Sets the rotation of the ILI9341 display.
 * @param  Rotation: The desired screen rotation. This parameter can be one of the following values:
 *         @arg SCREEN_VERTICAL_1: Vertical orientation with the top of the screen at the connector side.
 *         @arg SCREEN_HORIZONTAL_1: Horizontal orientation with the top of the screen at the connector side.
 *         @arg SCREEN_VERTICAL_2: Vertical orientation with the bottom of the screen at the connector side.
 *         @arg SCREEN_HORIZONTAL_2: Horizontal orientation with the bottom of the screen at the connector side.
 * @retval None
 */
void ILI9341_Set_Rotation(uint8_t Rotation)
{

	uint8_t screen_rotation = Rotation;
	ILI9341_Write_Command(0x36);
	HAL_Delay(1);

	switch(screen_rotation)
		{
			case SCREEN_VERTICAL_1:
				ILI9341_Write_Data(0x40|0x08);
				LCD_WIDTH = 240;
				LCD_HEIGHT = 320;
				break;
			case SCREEN_HORIZONTAL_1:
				ILI9341_Write_Data(0x20|0x08);
				LCD_WIDTH  = 320;
				LCD_HEIGHT = 240;
				break;
			case SCREEN_VERTICAL_2:
				ILI9341_Write_Data(0x80|0x08);
				LCD_WIDTH  = 240;
				LCD_HEIGHT = 320;
				break;
			case SCREEN_HORIZONTAL_2:
				ILI9341_Write_Data(0x40|0x80|0x20|0x08);
				LCD_WIDTH  = 320;
				LCD_HEIGHT = 240;
				break;
			default:
				//EXIT IF SCREEN ROTATION NOT VALID!
				break;
		}
}
/*Enable LCD display*/
/**
 * @brief  Enables the ILI9341 display by setting the RESET pin high.
 * 
 * This function sets the RESET pin of the ILI9341 display to a high state,
 * effectively enabling the display. It uses the HAL_GPIO_WritePin function
 * to set the pin state.
 * 
 * @param  None
 * @retval None
 */
void ILI9341_Enable(void)
{
HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
}


/*Initialize LCD display*/
/**
 * @brief  Initializes the ILI9341 LCD display.
 * 
 * This function performs the necessary initialization sequence for the ILI9341
 * LCD display, including enabling the display, initializing the SPI interface,
 * resetting the display, and configuring various control registers.
 * 
 * The initialization sequence includes:
 * - Software reset
 * - Power control settings
 * - Driver timing control settings
 * - Power on sequence control
 * - Pump ratio control
 * - VCM control settings
 * - Memory access control
 * - Pixel format setting
 * - Frame rate control
 * - Display function control
 * - Gamma correction settings
 * - Exiting sleep mode
 * - Turning on the display
 * - Setting the initial rotation
 * 
 * @note This function assumes that the necessary hardware setup (e.g., GPIO, SPI)
 *       has already been performed.
 */
void ILI9341_Init(void)
{
	ILI9341_Enable();
	ILI9341_SPI_init();

	ILI9341_Reset();

	//SOFTWARE RESET
	ILI9341_Write_Command(0x01);
	HAL_Delay(1000);

	//POWER CONTROL A
	ILI9341_Write_Command(0xCB);
	ILI9341_Write_Data(0x39);
	ILI9341_Write_Data(0x2C);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x34);
	ILI9341_Write_Data(0x02);

	//POWER CONTROL B
	ILI9341_Write_Command(0xCF);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0xC1);
	ILI9341_Write_Data(0x30);

	//DRIVER TIMING CONTROL A
	ILI9341_Write_Command(0xE8);
	ILI9341_Write_Data(0x85);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x78);

	//DRIVER TIMING CONTROL B
	ILI9341_Write_Command(0xEA);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);

	//POWER ON SEQUENCE CONTROL
	ILI9341_Write_Command(0xED);
	ILI9341_Write_Data(0x64);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x12);
	ILI9341_Write_Data(0x81);

	//PUMP RATIO CONTROL
	ILI9341_Write_Command(0xF7);
	ILI9341_Write_Data(0x20);

	//POWER CONTROL,VRH[5:0]
	ILI9341_Write_Command(0xC0);
	ILI9341_Write_Data(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	ILI9341_Write_Command(0xC1);
	ILI9341_Write_Data(0x10);

	//VCM CONTROL
	ILI9341_Write_Command(0xC5);
	ILI9341_Write_Data(0x3E);
	ILI9341_Write_Data(0x28);

	//VCM CONTROL 2
	ILI9341_Write_Command(0xC7);
	ILI9341_Write_Data(0x86);

	//MEMORY ACCESS CONTROL
	ILI9341_Write_Command(0x36);
	ILI9341_Write_Data(0x48);

	//PIXEL FORMAT
	ILI9341_Write_Command(0x3A);
	ILI9341_Write_Data(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	ILI9341_Write_Command(0xB1);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x18);

	//DISPLAY FUNCTION CONTROL
	ILI9341_Write_Command(0xB6);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x82);
	ILI9341_Write_Data(0x27);

	//3GAMMA FUNCTION DISABLE
	ILI9341_Write_Command(0xF2);
	ILI9341_Write_Data(0x00);

	//GAMMA CURVE SELECTED
	ILI9341_Write_Command(0x26);
	ILI9341_Write_Data(0x01);

	//POSITIVE GAMMA CORRECTION
	ILI9341_Write_Command(0xE0);
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0x2B);
	ILI9341_Write_Data(0x0C);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x4E);
	ILI9341_Write_Data(0xF1);
	ILI9341_Write_Data(0x37);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x10);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x09);
	ILI9341_Write_Data(0x00);

	//NEGATIVE GAMMA CORRECTION
	ILI9341_Write_Command(0xE1);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x14);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x11);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0xC1);
	ILI9341_Write_Data(0x48);
	ILI9341_Write_Data(0x08);
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x0C);
	ILI9341_Write_Data(0x31);
	ILI9341_Write_Data(0x36);
	ILI9341_Write_Data(0x0F);

	//EXIT SLEEP
	ILI9341_Write_Command(0x11);
	HAL_Delay(120);

	//TURN ON DISPLAY
	ILI9341_Write_Command(0x29);

	//STARTING ROTATION
	ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
}

//INTERNAL FUNCTION OF LIBRARY, USAGE NOT RECOMENDED, USE Draw_Pixel INSTEAD
/*Sends single pixel colour information to LCD*/
/**
 * @brief  Draws a single color on the ILI9341 display.
 * @param  Colour: 16-bit color value to be sent to the display.
 * @retval None
 * 
 * This function sends a 16-bit color value to the ILI9341 display. It first
 * splits the color into two 8-bit values and stores them in a temporary buffer.
 * Then, it sets the Data/Command pin to indicate data transmission and selects
 * the display by pulling the chip select pin low. The color data is transmitted
 * via SPI, and finally, the chip select pin is set high to complete the operation.
 */
void ILI9341_Draw_Colour(uint16_t Colour)
{
//SENDS COLOUR
	unsigned char TempBuffer[2] = {Colour>>8, Colour};
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, TempBuffer, 2, 1);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);
}


//INTERNAL FUNCTION OF LIBRARY
/*Sends block colour information to LCD*/
/**
 * @brief Draws a burst of color on the ILI9341 display.
 *
 * This function sends a burst of color data to the ILI9341 display using SPI.
 * It divides the data into manageable chunks and transmits them in blocks.
 *
 * @param Colour The 16-bit color value to be sent.
 * @param Size The number of pixels to be colored.
 *
 * The function first determines the buffer size based on the given size and
 * the maximum burst size. It then prepares a buffer with the color data and
 * transmits it in blocks using the HAL SPI transmit function. Any remaining
 * data that does not fit into a full block is transmitted separately.
 *
 * The function uses GPIO pins to control the data/command mode and chip select
 * signals for the SPI communication.
 */
void ILI9341_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
	{
	//SENDS COLOUR
	uint32_t Buffer_Size = 0;
	if((Size*2) < BURST_MAX_SIZE)
	{
		Buffer_Size = Size;
	}
	else
	{
		Buffer_Size = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);

	unsigned char chifted = 	Colour>>8;;
	unsigned char burst_buffer[Buffer_Size];
	for(uint32_t j = 0; j < Buffer_Size; j+=2)
		{
			burst_buffer[j] = 	chifted;
			burst_buffer[j+1] = Colour;
		}

	uint32_t Sending_Size = Size*2;
	uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

	if(Sending_in_Block != 0)
	{
		for(uint32_t j = 0; j < (Sending_in_Block); j++)
			{
			HAL_SPI_Transmit(&hspi2, (unsigned char *)burst_buffer, Buffer_Size, 10);
			}
	}

	//REMAINDER!
	HAL_SPI_Transmit(&hspi2, (unsigned char *)burst_buffer, Remainder_from_block, 10);

	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);
}


//FILL THE ENTIRE SCREEN WITH SELECTED COLOUR (either #define-d ones or custom 16bit)
/**
 * @brief Fills the entire screen with a specified color.
 *
 * This function sets the address to cover the entire screen and sends the specified color information
 * to the LCD to fill the screen.
 *
 * @param Colour The color to fill the screen with. This is a 16-bit value representing the color.
 */
/*Sets address (entire screen) and Sends Height*Width ammount of colour information to LCD*/
void ILI9341_Fill_Screen(uint16_t Colour)
{
	ILI9341_Set_Address(0,0,LCD_WIDTH,LCD_HEIGHT);
	ILI9341_Draw_Colour_Burst(Colour, LCD_WIDTH*LCD_HEIGHT);
}


//DRAW PIXEL AT XY POSITION WITH SELECTED COLOUR
//
//Location is dependant on screen orientation. x0 and y0 locations change with orientations.
//Using pixels to draw big simple structures is not recommended as it is really slow
//Try using either rectangles or lines if possible
//
/**
 * @brief  Draws a pixel on the ILI9341 display at the specified coordinates with the given color.
 * @param  X: The X coordinate of the pixel (0 to LCD_WIDTH-1).
 * @param  Y: The Y coordinate of the pixel (0 to LCD_HEIGHT-1).
 * @param  Colour: The color of the pixel in RGB565 format.
 * @retval None
 * 
 * This function sets the address window of the ILI9341 display to the specified
 * coordinates and sends the color data to draw a single pixel. If the coordinates
 * are out of bounds, the function returns immediately without drawing anything.
 * 
 * The function uses the HAL (Hardware Abstraction Layer) library to control the
 * GPIO pins and SPI communication. It sends the necessary commands and data to
 * the ILI9341 display controller to set the address window and draw the pixel.
 * 
 * The sequence of operations is as follows:
 * 1. Check if the coordinates are within bounds.
 * 2. Set the column address (X coordinate).
 * 3. Set the page address (Y coordinate).
 * 4. Write the pixel color data.
 */
void ILI9341_Draw_Pixel(uint16_t X,uint16_t Y,uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;	//OUT OF BOUNDS!

	//ADDRESS
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	ILI9341_SPI_SEND(0x2A);
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

	//XDATA
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer[4] = {X>>8,X, (X+1)>>8, (X+1)};
	HAL_SPI_Transmit(&hspi2, Temp_Buffer, 4, 1);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

	//ADDRESS
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	ILI9341_SPI_SEND(0x2B);
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

	//YDATA
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer1[4] = {Y>>8,Y, (Y+1)>>8, (Y+1)};
	HAL_SPI_Transmit(&hspi2, Temp_Buffer1, 4, 1);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

	//ADDRESS
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	ILI9341_SPI_SEND(0x2C);
	HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

	//COLOUR
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer2[2] = {Colour>>8, Colour};
	HAL_SPI_Transmit(&hspi2, Temp_Buffer2, 2, 1);
	HAL_GPIO_WritePin(CHIP_SELECT_GPIO_Port, CHIP_SELECT_Pin, GPIO_PIN_SET);

}


//DRAW RECTANGLE OF SET SIZE AND HEIGTH AT X and Y POSITION WITH CUSTOM COLOUR
//
//Rectangle is hollow. X and Y positions mark the upper left corner of rectangle
//As with all other draw calls x0 and y0 locations dependant on screen orientation
//

/**
 * @brief Draw a filled rectangle on the ILI9341 display.
 * 
 * This function draws a filled rectangle with the specified dimensions and color
 * on the ILI9341 display. The rectangle is clipped to the display boundaries if
 * it exceeds the display dimensions.
 * 
 * @param X The X coordinate of the top-left corner of the rectangle.
 * @param Y The Y coordinate of the top-left corner of the rectangle.
 * @param Width The width of the rectangle.
 * @param Height The height of the rectangle.
 * @param Colour The color of the rectangle in RGB565 format.
 * 
 * @note If the specified rectangle exceeds the display boundaries, it will be
 *       clipped to fit within the display.
 */
void ILI9341_Draw_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if((X+Width-1)>=LCD_WIDTH)
		{
			Width=LCD_WIDTH-X;
		}
	if((Y+Height-1)>=LCD_HEIGHT)
		{
			Height=LCD_HEIGHT-Y;
		}
	ILI9341_Set_Address(X, Y, X+Width-1, Y+Height-1);
	ILI9341_Draw_Colour_Burst(Colour, Height*Width);
}

/**
 * @brief Draws a horizontal line on the ILI9341 display.
 * 
 * This function draws a horizontal line starting from the specified (X, Y) 
 * coordinates and extending for the specified width. The line is drawn using 
 * the specified color.
 * 
 * @param X The starting X coordinate of the line.
 * @param Y The starting Y coordinate of the line.
 * @param Width The width of the line.
 * @param Colour The color of the line.
 * 
 * @note If the specified coordinates are outside the display boundaries, 
 *       the function will return without drawing anything.
 * @note If the specified width extends beyond the display boundaries, 
 *       the width will be adjusted to fit within the display.
 */
//DRAW LINE FROM X,Y LOCATION to X+Width,Y LOCATION
void ILI9341_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if((X+Width-1)>=LCD_WIDTH)
		{
			Width=LCD_WIDTH-X;
		}
	ILI9341_Set_Address(X, Y, X+Width-1, Y);
	ILI9341_Draw_Colour_Burst(Colour, Width);
}

//DRAW LINE FROM X,Y LOCATION to X,Y+Height LOCATION
/**
 * @brief  Draws a vertical line on the ILI9341 display.
 * @param  X: The X coordinate of the starting point of the vertical line.
 * @param  Y: The Y coordinate of the starting point of the vertical line.
 * @param  Height: The height of the vertical line.
 * @param  Colour: The color of the vertical line.
 * @retval None
 * @note   If the specified height exceeds the display boundaries, it will be adjusted to fit within the display.
 */
void ILI9341_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t Height, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if((Y+Height-1)>=LCD_HEIGHT)
		{
			Height=LCD_HEIGHT-Y;
		}
	ILI9341_Set_Address(X, Y, X, Y+Height-1);
	ILI9341_Draw_Colour_Burst(Colour, Height);
}
