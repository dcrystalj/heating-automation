
#ifndef LCD_MESSAGE_H
#define LCD_MESSAGE_H



typedef enum
{
  TIME =      0,
  TEMP =      1,
  UNAME =     2,
  IMAGE =     3,
  LCDCLR =    4,
} LCD_Msg_Type;


typedef enum
{
  LCD_NORMAL =    0,
  LCD_SETTIME =   1,
  LCD_SETDATE =   2,
  LCD_SETTINGS =  3,
  LCD_INFO    =   4,
  LCD_RELAY    =  5,
  LCD_OFF =		  6
} LCD_ScreenState;

typedef struct
{
  uint8_t Xpos;
  uint16_t Ypos;
  uint16_t Height;
  uint8_t Width;
} BitmapPropertiesStruct;

/* The structure passed to the LCD when there is text to display. */
typedef struct
{
  long xColumn;
  LCD_Msg_Type xMsgType;
  signed char *pcMessage;
  uint16_t *pBitmap;
  BitmapPropertiesStruct stBitmapProperties;
} xLCDMessage;





#endif
