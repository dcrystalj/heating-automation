/*
 * TouchPanel.c
 *
 *  Created on: Mar 15, 2013
 *      Author: patricio
 */

/* Includes ------------------------------------------------------------------*/
#include "TouchPanel.h"
#include "TouchPanel_config.h"
#include "defs.h"
#include "timetemp.h"



/* Private variables ---------------------------------------------------------*/

#if(defCALIBRATE_TOUCH_PANEL == 1)
  MATRIX matrix ;
#elif(defCALIBRATE_TOUCH_PANEL == 0)
  MATRIX matrix = {756, -300720, 114179700, -395472, 2128, 93256592, -4213603};
//  MATRIX matrix = {3115, -45080, 13180715, -51835, -2065, 9878080, -637593};
  //MATRIX matrix = {-8652, -260400, 109371996, -411488, -38528, 339133152, -3784656};
#endif


POINT  display ;


POINT ScreenSample[3];

POINT DisplaySample[3] = {
//		{ 15, 15 },
//		{ 85, 50 },
//		{ 50, 85 }
                                { 36,  48 },
                                { 204, 160},
                                { 120, 272}
};

time_structure_t time_temp;

/* The queue used to send message to LCD gate-keeper task  */
extern xQueueHandle xLCDQueue;
extern FlagStatus xDrawSetTimeScreen;
extern FlagStatus faSensorAlive[MAX_TEMP_SESNORS];
extern time_structure_t time;
extern xTaskHandle xTimerFunctionHandle;
extern xTaskHandle xRefreshHandle;
extern void vRefreshTask( void *pvParameters );
extern int8_t selectedRelay;
extern int8_t pStatusRelays[MAX_RELAYS];
extern short int iAllSet( FlagStatus *fsArray, int length);


/*******************************************************************************
* Function Name  : ADS7843_SPI_Init
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
static void ADS7843_SPI_Init(void)
{
  SPI_InitTypeDef SPI_InitStruct;

  RCC_APB1PeriphClockCmd(Open_RCC_SPI,ENABLE);

  SPI_I2S_DeInit(Open_SPI);
  /* Open_SPI Config -------------------------------------------------------------*/
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;

  SPI_Init(Open_SPI, &SPI_InitStruct);

  SPI_Cmd(Open_SPI, ENABLE);
}

/*******************************************************************************
* Function Name  : TP_Init
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void TP_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(Open_SPI_SCK_GPIO_CLK | Open_SPI_MISO_GPIO_CLK | Open_SPI_MOSI_GPIO_CLK,ENABLE);

  RCC_AHB1PeriphClockCmd(Open_TP_CS_CLK | Open_TP_IRQ_CLK,ENABLE);
  Open_SPI_CLK_INIT(Open_RCC_SPI,ENABLE);

  GPIO_PinAFConfig(Open_SPI_SCK_GPIO_PORT,  Open_SPI_SCK_SOURCE,  Open_GPIO_AF_SPI);
  GPIO_PinAFConfig(Open_SPI_MISO_GPIO_PORT, Open_SPI_MISO_SOURCE, Open_GPIO_AF_SPI);
  GPIO_PinAFConfig(Open_SPI_MOSI_GPIO_PORT, Open_SPI_MOSI_SOURCE, Open_GPIO_AF_SPI);

  GPIO_InitStructure.GPIO_Pin = Open_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(Open_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = Open_SPI_MISO_PIN;
  GPIO_Init(Open_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = Open_SPI_MOSI_PIN;
  GPIO_Init(Open_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);


  /* TP_CS  */
  GPIO_InitStructure.GPIO_Pin = Open_TP_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(Open_TP_CS_PORT, &GPIO_InitStructure);


  /*TP_IRQ */
  GPIO_InitStructure.GPIO_Pin = Open_TP_IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(Open_TP_IRQ_PORT, &GPIO_InitStructure);


  TP_CS(1);
  ADS7843_SPI_Init();
}



/*******************************************************************************
* Function Name  : DelayUS
* Description    :
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/

void DelayUS(__IO uint32_t cnt)
{
  uint16_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12;
     while (us--)
     {
       ;
     }
  }
}


/*******************************************************************************
* Function Name  : WR_CMD
* Description    :
* Input          : - cmd:
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
static void WR_CMD (uint8_t cmd)
{
  /* Wait for SPI3 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI3 data */
  SPI_I2S_SendData(Open_SPI,cmd);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read Open_SPI received data */
  SPI_I2S_ReceiveData(Open_SPI);
}



/*******************************************************************************
* Function Name  : RD_AD
* Description    :
* Input          : None
* Output         : None
* Return         :
* Attention    : None
*******************************************************************************/
static int RD_AD(void)
{
  unsigned short buf,temp;
  /* Wait for Open_SPI Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send Open_SPI data */
  SPI_I2S_SendData(Open_SPI,0x0000);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read Open_SPI received data */
  temp=SPI_I2S_ReceiveData(Open_SPI);
  buf=temp<<8;
  DelayUS(1);
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send Open_SPI data */
  SPI_I2S_SendData(Open_SPI,0x0000);
  /* Wait for Open_SPI data reception */
  while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read Open_SPI received data */
  temp=SPI_I2S_ReceiveData(Open_SPI);
  buf |= temp;
  buf>>=3;
  buf&=0xfff;
  return buf;
}


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X
* Input          : None
* Output         : None
* Return         :
* Attention    : None
*******************************************************************************/
int Read_X(void)
{
  int i;
  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHX);
  DelayUS(1);
  i=RD_AD();
  TP_CS(1);
  return i;
}

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         :
* Attention    : None
*******************************************************************************/
int Read_Y(void)
{
  int i;
  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHY);
  DelayUS(1);
  i=RD_AD();
  TP_CS(1);
  return i;
}


/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         :
* Attention    : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)
{
  int adx,ady;
  adx=Read_X();
  DelayUS(1);
  ady=Read_Y();
  *x=adx;
  *y=ady;
}

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    :
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos, uint16_t Ypos)
{
  Pixel(Xpos, Ypos, BLUE);     /* Center point */
  Pixel(Xpos+1, Ypos, BLUE);
  Pixel(Xpos, Ypos+1, BLUE);
  Pixel(Xpos+1, Ypos+1, BLUE);
}

/*******************************************************************************
* Function Name  : DrawCross
* Description    :
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetTextColor(WHITE);
  LCD_DrawUniLine(Xpos-15,Ypos,Xpos-2,Ypos);
  LCD_DrawUniLine(Xpos+2,Ypos,Xpos+15,Ypos);
  LCD_DrawUniLine(Xpos,Ypos-15,Xpos,Ypos-2);
  LCD_DrawUniLine(Xpos,Ypos+2,Xpos,Ypos+15);


  LCD_SetTextColor(RGB565CONVERT(184,158,131));
  LCD_DrawUniLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15);
  LCD_DrawUniLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15);

  LCD_DrawUniLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15);
  LCD_DrawUniLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15);

  LCD_DrawUniLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15);
  LCD_DrawUniLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15);

  LCD_DrawUniLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15);
  LCD_DrawUniLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7);

  LCD_SetTextColor(WHITE);
}

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : POINT *
* Attention    : None
*******************************************************************************/
POINT *Read_Ads7846(void)
{
  static POINT  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};

  do
  {
    TP_GetAdXY(TP_X,TP_Y);
    buffer[0][count]=TP_X[0];
    buffer[1][count]=TP_Y[0];
    count++;
  }
  while( !TP_INT_IN&& count<9 );  /* TP_INT_IN  */

  if( count==9 )   /* Average X Y  */
  {
  /* Average X  */
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
    temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
    temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

    m0=temp[0]-temp[1];
    m1=temp[1]-temp[2];
    m2=temp[2]-temp[0];

    m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
    m2=m2>0?m2:(-m2);

    if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

    if(m0<m1)
    {
      if(m2<m0)
        screen.x=(temp[0]+temp[2])/2;
      else
        screen.x=(temp[0]+temp[1])/2;
    }
    else if(m2<m1)
      screen.x=(temp[0]+temp[2])/2;
    else
      screen.x=(temp[1]+temp[2])/2;

  /* Average Y  */
  temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
  temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
  temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
  m0=temp[0]-temp[1];
  m1=temp[1]-temp[2];
  m2=temp[2]-temp[0];
  m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
  m2=m2>0?m2:(-m2);
  if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

  if(m0<m1)
  {
    if(m2<m0)
      screen.y=(temp[0]+temp[2])/2;
    else
      screen.y=(temp[0]+temp[1])/2;
    }
  else if(m2<m1)
     screen.y=(temp[0]+temp[2])/2;
  else
     screen.y=(temp[1]+temp[2])/2;

  return &screen;
  }

  return 0;
}







/**********************************************************************
 *
 *     Function: setCalibrationMatrix()
 *
 *  Description: Calling this function with valid input data
 *                in the display and screen input arguments
 *                causes the calibration factors between the
 *                screen and display points to be calculated,
 *                and the output argument - matrixPtr - to be
 *                populated.
 *
 *               This function needs to be called only when new
 *                calibration factors are desired.
 *
 *   The following set of equations represent a valid display
 *    point given a corresponding set of touch screen points:
 *
 *
 *                                              /-     -\
 *              /-    -\     /-            -\   |       |
 *              |      |     |              |   |   Xs  |
 *              |  Xd  |     | A    B    C  |   |       |
 *              |      |  =  |              | * |   Ys  |
 *              |  Yd  |     | D    E    F  |   |       |
 *              |      |     |              |   |   1   |
 *              \-    -/     \-            -/   |       |
 *                                              \-     -/
 *
 *
 *    where:
 *
 *           (Xd,Yd) represents the desired display point
 *                    coordinates,
 *
 *           (Xs,Ys) represents the available touch screen
 *                    coordinates, and the matrix
 *
 *           /-   -\
 *           |A,B,C|
 *           |D,E,F| represents the 2D transformation matrix
 *           \-   -/  composed of rotation, scaling and translation

 *
 *
 *  Argument(s): displayPtr (input) - Pointer to an array of three
 *                                     sample, reference points.
 *               screenPtr (input) - Pointer to the array of touch
 *                                    screen points corresponding
 *                                    to the reference display points.
 *               matrixPtr (output) - Pointer to the calibration
 *                                     matrix computed for the set
 *                                     of points being provided.
 *
 *
 *  From the article text, recall that the matrix coefficients are
 *   resolved to be the following:
 *
 *
 *      Divider =  (Xs0 - Xs2)*(Ys1 - Ys2) - (Xs1 - Xs2)*(Ys0 - Ys2)
 *
 *
 *
 *                 (Xd0 - Xd2)*(Ys1 - Ys2) - (Xd1 - Xd2)*(Ys0 - Ys2)
 *            A = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Xs0 - Xs2)*(Xd1 - Xd2) - (Xd0 - Xd2)*(Xs1 - Xs2)
 *            B = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 Ys0*(Xs2*Xd1 - Xs1*Xd2) +
 *                             Ys1*(Xs0*Xd2 - Xs2*Xd0) +
 *                                           Ys2*(Xs1*Xd0 - Xs0*Xd1)
 *            C = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Yd0 - Yd2)*(Ys1 - Ys2) - (Yd1 - Yd2)*(Ys0 - Ys2)
 *            D = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Xs0 - Xs2)*(Yd1 - Yd2) - (Yd0 - Yd2)*(Xs1 - Xs2)
 *            E = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 Ys0*(Xs2*Yd1 - Xs1*Yd2) +
 *                             Ys1*(Xs0*Yd2 - Xs2*Yd0) +
 *                                           Ys2*(Xs1*Yd0 - Xs0*Yd1)
 *            F = ---------------------------------------------------
 *                                   Divider
 *
 *
 *       Return: SUCCESS - the calibration matrix was correctly
 *                     calculated and its value is in the
 *                     output argument.
 *               ERROR - an error was detected and the
 *                     function failed to return a valid
 *                     set of matrix values.
 *                     The only time this sample code returns
 *                     0 is when Divider == 0
 *
 *
 *
 *                 NOTE!    NOTE!    NOTE!
 *
 *  setCalibrationMatrix() and getDisplayPoint() will do fine
 *  for you as they are, provided that your digitizer
 *  resolution does not exceed 10 bits (1024 values).  Higher
 *  resolutions may cause the integer operations to overflow
 *  and return incorrect values.  If you wish to use these
 *  functions with digitizer resolutions of 12 bits (4096
 *  values) you will either have to a) use 64-bit signed
 *  integer variables and math, or b) judiciously modify the
 *  operations to scale results by a factor of 2 or even 4.
 *
 *
 */
ErrorStatus setCalibrationMatrix( POINT *displayPtr,
                          POINT *screenPtr,
                          MATRIX *matrixPtr)
{

   ErrorStatus  retValue = SUCCESS ;



    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

    if( matrixPtr->Divider == 0 )
    {
        retValue = ERROR ;
    }
    else
    {
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;

        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
    }

    return( retValue ) ;

} /* end of setCalibrationMatrix() */




/**********************************************************************
 *
 *     Function: getDisplayPoint()
 *
 *  Description: Given a valid set of calibration factors and a point
 *                value reported by the touch screen, this function
 *                calculates and returns the true (or closest to true)
 *                display point below the spot where the touch screen
 *                was touched.
 *
 *
 *
 *  Argument(s): displayPtr (output) - Pointer to the calculated
 *                                      (true) display point.
 *               screenPtr (input) - Pointer to the reported touch
 *                                    screen point.
 *               matrixPtr (input) - Pointer to calibration factors
 *                                    matrix previously calculated
 *                                    from a call to
 *                                    setCalibrationMatrix()
 *
 *
 *  The function simply solves for Xd and Yd by implementing the
 *   computations required by the translation matrix.
 *
 *                                              /-     -\
 *              /-    -\     /-            -\   |       |
 *              |      |     |              |   |   Xs  |
 *              |  Xd  |     | A    B    C  |   |       |
 *              |      |  =  |              | * |   Ys  |
 *              |  Yd  |     | D    E    F  |   |       |
 *              |      |     |              |   |   1   |
 *              \-    -/     \-            -/   |       |
 *                                              \-     -/
 *
 *  It must be kept brief to avoid consuming CPU cycles.
 *
 *
 *       Return: SUCCESS - the display point was correctly calculated
 *                     and its value is in the output argument.
 *               ERROR - an error was detected and the function
 *                         failed to return a valid point.
 *
 *
 *
 *                 NOTE!    NOTE!    NOTE!
 *
 *  setCalibrationMatrix() and getDisplayPoint() will do fine
 *  for you as they are, provided that your digitizer
 *  resolution does not exceed 10 bits (1024 values).  Higher
 *  resolutions may cause the integer operations to overflow
 *  and return incorrect values.  If you wish to use these
 *  functions with digitizer resolutions of 12 bits (4096
 *  values) you will either have to a) use 64-bit signed
 *  integer variables and math, or b) judiciously modify the
 *  operations to scale results by a factor of 2 or even 4.
 *
 *
 */
ErrorStatus getDisplayPoint( POINT *displayPtr,
                     POINT *screenPtr,
                     MATRIX *matrixPtr )
{
    ErrorStatus  retValue = SUCCESS ;


    if( matrixPtr->Divider != 0 )
    {

            /* Operation order is important since we are doing integer */
            /*  math. Make sure you add all terms together before      */
            /*  dividing, so that the remainder is not rounded off     */
            /*  prematurely.                                           */

        /* XD = AX+BY+C */
        displayPtr->x = ( (matrixPtr->An * screenPtr->x) +
                          (matrixPtr->Bn * screenPtr->y) +
                           matrixPtr->Cn
                        ) / matrixPtr->Divider ;

        /* YD = DX+EY+F */
        displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) +
                          (matrixPtr->En * screenPtr->y) +
                           matrixPtr->Fn
                        ) / matrixPtr->Divider ;
    }
    else
    {
        retValue = ERROR ;
    }

    return( retValue ) ;

} /* end of getDisplayPoint() */





/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void delay_ms(__IO uint32_t ms)
{
  uint32_t i,j;
  for( i = 0; i < ms; i++ )
  {
    for( j = 0; j < 0xffffff; j++ );
  }
}

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
  uint8_t i;
  POINT  *Ptr = (void*)0 ;


  /* Draw three targets on your display. Drawing one at time is  */
  /*  probably a simpler implementation. These targets should be */
  /*  widely separated but also avoid the areas too near the     */
  /*  edges where digitizer output tends to become non-linear.   */
  /*  The recommended set of points is (in display resolution    */
  /*   percentages):                                             */
  /*                                                             */
  /*                  ( 15, 15)                                  */
  /*                  ( 85, 50)                                  */
  /*                  ( 50, 85)                                  */
  /*                                                             */
  /* Each time save the display and screen set (returned by the  */
  /*  digitizer when the user touches each calibration target    */
  /*  into the corresponding array).                             */
  /* Since you normalized your calibration matrix above, you     */
  /*  should be able to use touch screen data as it would be     */
  /*  provided by the digitizer driver.  When the matrix equals  */
  /*  unity, getDisplayPoint() returns the same raw input data   */
  /*  as output.                                                 */


  LCD_Clear(BLACK);

  for( i = 0; i < 3; i++ )
  {
     LCD_StringLine(0,10,"Pritisni na krizec....");
     DrawCross(DisplaySample[i].x,DisplaySample[i].y);
     /* Read ADS7846 :*/
     while( Ptr == (void*)0 )
     {
       Ptr=Read_Ads7846();
     }
     ScreenSample[i].x= Ptr->x;
     ScreenSample[i].y= Ptr->y;
     Ptr = (void*)0;
     LCD_Clear(BLACK);
     TP_Delay(0xFFFFFF);
     TP_Delay(0xFFFFFF);
  }
  /* We have collected all the points. Now set calibration matrix: */
  ErrorStatus e = setCalibrationMatrix( &DisplaySample[0], &ScreenSample[0], &matrix );

  /*An,
  Bn,
  Cn,
  Dn,
  En,
  Fn,
  Divider */

#if(defCALIBRATE_TOUCH_PANEL == 0)
  char chStringa[20];
  itoa(matrix.An, chStringa);
  LCD_StringLine(0,10, chStringa);

  char chStringb[20];
  itoa(matrix.Bn, chStringb);
  LCD_StringLine(0,30, chStringb);

  char chStringc[20];
  itoa(matrix.Cn, chStringc);
  LCD_StringLine(0,40, chStringc);

  char chStringd[20];
  itoa(matrix.Dn, chStringd);
  LCD_StringLine(0,60, chStringd);

  char chStringe[20];
  itoa(matrix.En, chStringe);
  LCD_StringLine(0,80, chStringe);

  char chStringf[20];
  itoa(matrix.Fn, chStringf);
  LCD_StringLine(0,100, chStringf);

  char chStringg[20];
  itoa(matrix.Divider, chStringg);
  LCD_StringLine(0,120, chStringg);

  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
  TP_Delay(0xFFFFFF);
#endif

  TP_Delay(0xFFFFFF);
  //LCD_Clear(BLACK);
}
/* return if button was clicked */
int8_t iButton(int8_t i, POINT xDisplayPtr)
{
	if((xDisplayPtr.x > i*BUTTON_H) &&
	  (xDisplayPtr.x < (i+1)*BUTTON_H) &&
	  (xDisplayPtr.y > LCD_ICONS_START_Y) &&
	  (xDisplayPtr.y < LCD_ICONS_START_Y+BUTTON_H))
		return 1;

	return 0;
}
void backToHome(){
	xLCDMessage xMessage;
	/* Send the message to the LCD gatekeeper for display home screen */
	xLCDScreenState = LCD_NORMAL;
	xMessage.xMsgType = LCDCLR;
	xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
	xMessage.xMsgType = TEMP;
	xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
}

void vTouchPanelTask( void *pvParameters ) {
  portTickType xLastTouchTime;
  xLCDMessage xMessage;

  static POINT *xpScrPtr;
  static POINT xDisplayPtr;

  int isRefresh = 0;

  xLastTouchTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelay( 300 / portTICK_RATE_MS );
    do
    {
      xpScrPtr=Read_Ads7846();

      /** put LCD to sleep after 10 seconds inactivity
       * prevent sleep mode if at least one temperature sensor doesn't work
      */
      if( xLCDScreenState != LCD_OFF && xTaskGetTickCount() > xLastTouchTime + 1 * MINUTE && iAllSet(faSensorAlive, MAX_TEMP_SESNORS) )
      {
		  xLCDScreenState = LCD_OFF;
		  xMessage.xMsgType =  LCDCLR;
		  xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
      }
      /* wake up if is in sleep and one of sensors get in ERROR state */
      else if( xLCDScreenState == LCD_OFF && xTaskGetTickCount() > xLastTouchTime + 1 * MINUTE && !iAllSet(faSensorAlive, MAX_TEMP_SESNORS) )
    	  break;

    } while( xpScrPtr == (void*)0 );

    //wake up LCD back
    if(xLCDScreenState == LCD_OFF)
    {
        LCD_DisplayOn();
    	LCD_BackLight(100);

    	//delete refresh task
    	if(xRefreshHandle != NULL)
      	  vTaskDelete(xRefreshHandle);

    	//return home and display temperature immediately
    	xLCDScreenState = LCD_NORMAL;
    	xMessage.xMsgType =  TEMP;
    	xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

    	//update last touch time to prevent falling to sleep again
        xLastTouchTime = xTaskGetTickCount();

        //deselect any of relays
        selectedRelay = -1;

    	continue;
    }
    xLastTouchTime = xTaskGetTickCount();

    getDisplayPoint(&xDisplayPtr, xpScrPtr, &matrix);

    switch( xLCDScreenState ) {
	  case LCD_INFO:
		  if (iButton(0,xDisplayPtr)) {
			  backToHome();
			  break;
		  }
      case LCD_SETTINGS:
		  if (iButton(0,xDisplayPtr)) {
			  backToHome();
			  break;
		  }
      case LCD_RELAY:

    	//back button
        if (iButton(0,xDisplayPtr)) {
        	backToHome();
        	selectedRelay = -1;

        	if(xRefreshHandle != NULL){
        	  vTaskDelete(xRefreshHandle);
        	}

        	break;
        }

        //select relay
        int i;
        for (i=0; i<MAX_RELAYS; i++){
            if ((xDisplayPtr.y > 5 + i*30) && (xDisplayPtr.y < 5 + (i+1)*30)) {
            	selectedRelay = i;

            	//refresh
            	xMessage.xMsgType = LCDCLR;
			    xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
			    break;
            }
        }


        //was something selected?
        if(selectedRelay >= 0){

			//turn selected relay to AUTO
        	if (iButton(1, xDisplayPtr))
				pStatusRelays[selectedRelay] = AUTO;

			//ON or
        	if (iButton(2, xDisplayPtr))
				pStatusRelays[selectedRelay] = ON;

			//OFF
        	if (iButton(3, xDisplayPtr))
				pStatusRelays[selectedRelay] = OFF;

        }

        //refresh screen
        xMessage.xMsgType = LCDCLR;
		xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

        break;
      case LCD_NORMAL:
        if (  iButton(0, xDisplayPtr) )
        {
          xDrawSetTimeScreen = SET;
          xLCDScreenState = LCD_SETTIME;
          vTaskSuspend( xTimerFunctionHandle ); /* temporarly suspend timer task */
          /* Send the message to the LCD gatekeeper for display. */
          // get current time
          time_temp.hours = time.hours;
          time_temp.minutes = time.minutes;
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType =  LCDCLR;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
          while(uxQueueMessagesWaiting(xLCDQueue)){}
          xDrawSetTimeScreen = SET;
          xLCDScreenState = LCD_SETTIME;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }
        if ( iButton(1, xDisplayPtr) )
        {
          xLCDScreenState = LCD_SETTINGS;
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType =  LCDCLR;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }
        if (  iButton(2, xDisplayPtr) )
        {
          xLCDScreenState = LCD_RELAY;

          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType =  LCDCLR;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

          if(isRefresh == 0){
        	  xTaskCreate( vRefreshTask, ( const signed char * ) "REFRESH", configMINIMAL_STACK_SIZE, (void *) xLCDScreenState, tskIDLE_PRIORITY + 1UL, xRefreshHandle );
        	  isRefresh = 1;
          }

        }
        if ( iButton(3, xDisplayPtr) ) {
          xLCDScreenState = LCD_INFO;
          /* Send the message to the LCD gate keeper for display. */
          xMessage.xMsgType =  LCDCLR;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }
        break;

      case LCD_SETTIME:
    	  //OK
        if ( iButton( 3, xDisplayPtr ) ) {
          // save new time:
          time.hours = time_temp.hours;
          time.minutes = time_temp.minutes;

          vTaskResume( xTimerFunctionHandle );
          backToHome();
        }

        //CANCEL
        if ( iButton( 0, xDisplayPtr ) ) {
          vTaskResume( xTimerFunctionHandle );
          backToHome();
        }

        /* check if hours increased : */
        if (  (xDisplayPtr.x > LCD_HOURP_X) &&
              (xDisplayPtr.x < LCD_HOURP_X+BUTTON_H) &&
              (xDisplayPtr.y > LCD_HOURP_Y) &&
              (xDisplayPtr.y < LCD_HOURP_Y+BUTTON_H)) {
          xLCDScreenState = LCD_SETTIME;
          time_temp.hours ++;
          if (time_temp.hours == 24) time_temp.hours = 0;
          FormatTimeString(&time_temp);
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType = TIME;
          xMessage.pcMessage = time_temp.timestring;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }

        /* check if hours decreased : */
        if (  (xDisplayPtr.x > LCD_HOURM_X) &&
              (xDisplayPtr.x < LCD_HOURM_X+BUTTON_H) &&
              (xDisplayPtr.y > LCD_HOURM_Y) &&
              (xDisplayPtr.y < LCD_HOURM_Y+BUTTON_H)) {
          xLCDScreenState = LCD_SETTIME;
          time_temp.hours --;
          if (time_temp.hours == 0xFF) time_temp.hours = 23;
          FormatTimeString(&time_temp);
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType = TIME;
          xMessage.pcMessage = time_temp.timestring;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }

        /* check if minutes increased : */
        if (  (xDisplayPtr.x > LCD_MINP_X) &&
              (xDisplayPtr.x < LCD_MINP_X+BUTTON_H) &&
              (xDisplayPtr.y > LCD_MINP_Y) &&
              (xDisplayPtr.y < LCD_MINP_Y+BUTTON_H)) {
          xLCDScreenState = LCD_SETTIME;
          time_temp.minutes ++;
          if (time_temp.minutes == 60) time_temp.minutes = 0;
          FormatTimeString(&time_temp);
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType = TIME;
          xMessage.pcMessage = time_temp.timestring;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }

        /* check if minutes decreased : */
        if (  (xDisplayPtr.x > LCD_MINM_X) &&
              (xDisplayPtr.x < LCD_MINM_X+BUTTON_H) &&
              (xDisplayPtr.y > LCD_MINM_Y) &&
              (xDisplayPtr.y < LCD_MINM_Y+BUTTON_H)) {
          xLCDScreenState = LCD_SETTIME;
          time_temp.minutes --;
          if (time_temp.minutes == 0xFF) time_temp.minutes = 59;
          FormatTimeString(&time_temp);
          /* Send the message to the LCD gatekeeper for display. */
          xMessage.xMsgType = TIME;
          xMessage.pcMessage = time_temp.timestring;
          xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }
        break;
      default: break;
    }



    xpScrPtr = (void*)0;
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void TP_Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}



