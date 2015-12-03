/**
 * @file
 */
#pragma once

/* Includes ------------------------------------------------------------------*/
#include "mbed-drivers/mbed.h"
#include "data.h"

class SSD1306
{
  protected:
	   SSD1306(PinName sda, PinName scl);

    /**
	   * @brief  Initialize the singleton
	   * @retval true if initialization successful,
	   * @retval false otherwise
	   */
	  bool Init(void) {
      send_cmd(0xAE);   // display off
      send_cmd(0xD5);   // display divide ratio/osc. freq. ratio
      send_cmd(0x80);   // the suggested ratio 0x80
      send_cmd(0xA8);   // multiplex ation mode: 63
      send_cmd(0x3F);
      send_cmd(0xD3);   // set display offset
      send_cmd(0x00);   // no offset
      send_cmd(0x40);   // set display start line
      send_cmd(0x8D);   // set display offset
      send_cmd(0x14);
      send_cmd(0xA1);   // segment remap
      send_cmd(0xC8);   // set COM output scan direction
      send_cmd(0xDA);   // common pads hardware: alternative
      send_cmd(0x12);   // COMSCANDEC
      send_cmd(0x81);   // contrast control
      send_cmd(0xCF);
      send_cmd(0xD9);   // set pre-charge period
      send_cmd(0xF1);
      send_cmd(0xDB);   // VCOM deselect level mode
      send_cmd(0x40);   // set vcomh = 0.83 * VCC
      send_cmd(0xA4);   // set entire display on/off
      send_cmd(0xA6);   // set normal display
      send_cmd(0xAF);   // set display on

      //drawBMP(53, 0, 127, 8, splash);
      printBigNumber('1', 0, 0);

      return true;
	  };

  public:
	  static SSD1306* Instance(PinName sda, PinName scl);

    void locate(int x, int y);
    void cls(void);
    void drawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
    void printBigNumber(unsigned char s, int x, int y);

  private:
	  static SSD1306 *_instance;

    I2C *i2c;

    void send_cmd(uint8_t cmd);
    void send_data(uint8_t data);
};
