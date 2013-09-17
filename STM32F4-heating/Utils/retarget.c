
#include "retarget.h"

/*-----------------------------------------------------------*/
/*
 * Retargets the C library printf function to the LCD.
 * Pa3cio, 25.2.2013
 */
int fputc( int ch, FILE *f )
{
  /* Character counter - counts chars in the line: */
  static uint16_t usColumn = 0;
  /* Physical column - we allow the first char in the column to start in arbitrary column, 
     not only in the column that is divisible by COLUMN_INCREMENT */
  static uint16_t usRefColumn = COLUMN_START;
  /* Line counter */
  static uint16_t usLine = 0;

  if( ( usColumn == 0 ) && ( usLine == 0 ) )
  {
    LCD_Clear(BLACK);
  }

  if( ch != '\n' )
  {
    /* Display one character on LCD */
    LCD_PutChar( usRefColumn, usLine, (u8) ch );
		
    /* Increment the column position by mainCOLUMN_INCREMENT */
    usRefColumn += COLUMN_INCREMENT;
		
    /* Increment the character counter */
    usColumn += COLUMN_INCREMENT;
    if ( (MAX_COLUMN - usColumn) < COLUMN_INCREMENT ) /* Do we have enough place for the character?*/
    {
      usLine += ROW_INCREMENT;
      usRefColumn = COLUMN_START;
      usColumn = 0;
    }
  }
  else
  {
    /* Move back to the first column of the next line. */
    usLine += ROW_INCREMENT;
    usRefColumn = COLUMN_START;
    usColumn = 0;	
  }

  /* Wrap back to the top of the display. */
  if( usLine >= MAX_LINE )
  {
    usLine = 0;
  }
	
  return ch;
}
/*-----------------------------------------------------------*/



