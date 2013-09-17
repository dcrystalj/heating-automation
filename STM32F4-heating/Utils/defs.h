/*
 * defs.h
 *
 *  Created on: Mar 18, 2013
 *      Author: patricio
 */

#ifndef DEFS_H_
#define DEFS_H_

#define defsPRINT_INTRO_MSGS          1
#define defsCALIBRATE_TOUCH_PANEL     0
#define defsTEMPERATURE_INIT 		  1
#define defsDS1621_PRESENT            1


#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_EDGE_OFFSET 10
#define BUTTON_H 60

#define LCD_ICONS_START_X   0
#define LCD_ICONS_START_Y   259

#define LCD_RECT_START_X   0
#define LCD_RECT_START_Y   0


/* LCD buttons */
#define BUTTON_H  60

#define LCD_HOURP_X   (LCD_WIDTH/2 - BUTTON_H)/2
#define LCD_HOURM_X   (LCD_WIDTH/2 - BUTTON_H)/2
#define LCD_HOURP_Y   90
#define LCD_HOURM_Y   LCD_HOURP_Y + BUTTON_H + 20
#define LCD_MINP_X    LCD_HOURP_X + 2*BUTTON_H
#define LCD_MINM_X    LCD_HOURP_X + 2*BUTTON_H
#define LCD_MINP_Y    LCD_HOURP_Y
#define LCD_MINM_Y    LCD_HOURM_Y



/* Temperature constants */
#define MAX_TEMP_SESNORS  5
#define MAX_RELAYS		  8
#define MAX_THERMOASTATS  4
#define MAX_RETRIES       5
#define MAX_CONDITIONS    8

#define MINUTE (60000 / portTICK_RATE_MS)
#define SECOND (1000 / portTICK_RATE_MS)

/* Relays */
#define	FLOOR_R     (uint16_t) 0x80
#define	GROUND_R    (uint16_t) 0x40
#define	BATHROOM_R  (uint16_t) 0x20
#define	ATTIC_R     (uint16_t) 0x10
#define	BOILER_R    (uint16_t) 0x8
#define	FURNACE_R   (uint16_t) 0x4
#define	HEATER_R    (uint16_t) 0x2
#define	RESET_R     (uint16_t) 0x1

union Relays {
  FlagStatus *fsRelay;
  int16_t iRelay;
  int8_t status;

};
typedef enum
{
  AUTO     = 0,
  ON     	 = 1,
  OFF      = 2
} STATUS ;

typedef enum
 {
   TFURNACE   = 0,
   TSYSTEM    = 1,
   TBOILER    = 2,
   TOUTSIDE   = 3,
   TEXCHANGER = 4
 } TEMP_sensor;

 typedef enum
 {
   TSTAT1   = 0x2,
   TSTAT2   = 0x4,
   TSTAT3   = 0x8,
   TSTAT4   = 0x40
 } TSTAT;

 typedef enum
 {
   EXTRA_HEAT			= 0,
   SYSTEM_GT_FURNACE 	= 1,
   LOW_TBOILER       	= 2,
   THERMOSTAT1     		= 3,
   THERMOSTAT2    		= 4,
   THERMOSTAT3     		= 5,
   THERMOSTAT4     		= 6,
   SYSTEM_COND 			= 7
 } CONDITIONS;



#endif /* DEFS_H_ */
