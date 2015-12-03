/**
 * @file
 * @brief   Implementation file for the SSD1306 singleton class
 */

/* Includes ------------------------------------------------------------------*/
#include "SSD1306.h"

#define addr    (0x78)

/* Static variables ----------------------------------------------------------*/
SSD1306* SSD1306::_instance = NULL;

/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 */
SSD1306::SSD1306(PinName sda, PinName scl) : i2c(new I2C(sda, scl))
{

}

/**
 * @brief     Get singleton instance
 * @return    a pointer to the initialized singleton instance of class SSD1306.
 *            A return value of NULL indicates an out of memory situation.
 * @param[in] sda I2C data line pin
 * @param[in] scl I2C clock line pin.
 */
SSD1306* SSD1306::Instance(PinName sda, PinName scl) {
    if(_instance == NULL) {
		    _instance = new SSD1306(sda, scl);

		    if(_instance != NULL) {
			      bool ret = _instance->Init();
			      if(!ret) {
				        error("Failed to init SSD1306 display!\n");
			      }
		    }
	  }

	  return _instance;
}

void SSD1306::resetDisplay(void)
{
    displayOff();
    clear();
    display();
    displayOn();
}

void SSD1306::displayOn(void)
{
    sendCommand(0xAF);        //display on
}

void SSD1306::displayOff(void)
{
    sendCommand(0xAE);		//display off
}

void SSD1306::send_cmd(uint8_t cmd) {
    char c[2] = { 0x00, cmd };
    i2c->write(addr, c, 2, 1);
}

void SSD1306::send_data(uint8_t data) {
    char c[2] = { 0x40, data };
    i2c->write(addr, c, 2, 1);
}

void SSD1306::locate(int x, int y) {
    send_cmd( 0xb0 + y );
    send_cmd( ( ( x & 0xf0 ) >> 4 ) | 0x10 );
    send_cmd( ( x & 0x0f ) | 0x01 );
}

void SSD1306::cls(void) {
    int x, y;
    for(y = 0; y < 8; y++){
        locate(0, y);
        for(x = 0; x < 128; x++) send_data(0x00);
    }
}

void SSD1306::drawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                      unsigned char y1, unsigned char BMP[]) {
    unsigned int j=0;
    unsigned char x,y;

    if( y1 % 8 == 0 ) {
        y = y1/8;
    } else {
        y = y1/8 + 1;
    }

    for(y = y0; y < y1; y++) {
        locate(x0, y);
        for(x = x0; x < x1; x++) {
            send_data(BMP[j++]);
        }
    }
}

void SSD1306::drawBitmap(int x, int y, int width, int height, const char *bitmap) {
  for (int i = 0; i < width * height / 8; i++ ) {
    unsigned char charColumn = 255 - pgm_read_byte(bitmap + i);
    for (int j = 0; j < 8; j++) {
      int targetX = i % width + x;
      int targetY = (i / (width)) * 8 + j + y;
      if (bitRead(charColumn, j)) {
        setPixel(targetX, targetY);
      }
    }
  }
}

void SSD1306::printBigNumber(unsigned char s, int x, int y) {
    locate(x,y);
    int salto=0;

    for(int i=0; i<96; i++) {
        if(s == ' ') {
            send_data(0);
        } else {
            send_data(bigNumbers[s-0x30][i]);
        }

        if(salto == 23) {
            salto = 0;
            x++;
            locate(x,y);
        } else {
            salto++;
        }
    }
}
