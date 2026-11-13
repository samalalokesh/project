//kpm.c 

#include <LPC21xx.h> 
#include "types.h" 
#include "defines.h" 
#include "kpm_defines.h" 
#include "lcd.h" 
#include "lcd_definesh.h" 
 
/* 
u8 kpmLUT[4][4]= 
{ 
        {0,1,2,3}, 

        {4,5,6,7}, 

        {8,9,10,11}, 

        {12,13,14,15} 
}; 
*/ 
 
u8 kpmLUT[4][4]= 

{ 

        {'1','2','3','A'}, 

        {'4','5','6','B'}, 

        {'7','8','9','C'}, 

        {'*','0','=','D'} 

}; 
 
 
void InitKPM(void)
{
  //ground all row lines
  WRITENIBBLE(IODIR1,ROW0,15);
        //all col lines are input high
        //default any input & high
}        
 
 
u8 ColScan(void)
{
        u8 t;
        t=(READNIBBLE(IOPIN1,COL0)<15)?0:1;
        return t;
} 
 
u8 RowCheck(void)
{
        u8 r;
        for(r=0;r<4;r++)
        {
                //ground iteratively one row
                //at a time,keeping other rows high
                WRITENIBBLE(IOPIN1,ROW0,~(1<<r));
                //check if key was pressed in that row
                if(!ColScan())
                        break;
        }
  //re-initialize all rows as ground
  WRITENIBBLE(IOPIN1,ROW0,0);
  return r;
} 
 
u8 ColCheck(void)
{
        u8 c;
        for(c=0;c<4;c++)
        {
                if(READBIT(IOPIN1,COL0+c)==0)
                        break;
        }
        return c;
} 
 
u8 KeyScan(void)
{
         u8 r,c,keyV;
   //wait for any key press
   while(ColScan());
   //if any key pressed,
   //identify row in which key was pressed
   r=RowCheck();
   //identify col in which key was pressed
   c=ColCheck();
   //extract key value from LUT
   keyV=kpmLUT[r][c];
   return keyV;
} 

u32 ReadNum(void)
{ 

        u32 keyV,sum=0; 

        while(1) 

        { 

                keyV=KeyScan(); 

                if((keyV>='0') && (keyV<='9')) 

                { 

                        sum=(sum*10)+(keyV-'0'); 

                        CmdLCD(GOTO_LINE2_POS0); 

                        U32LCD(sum); 

                        while(ColScan()==0); 

                } 

                else 

                { 

                        if(keyV=='C') 

                        { 

                                CmdLCD(CLEAR_LCD); 

                        } 

                        while(ColScan()==0); 

                        break; 

                } 

        } 

        return sum; 

} 
 
 
void ReadNum2(u32 *num,u8 *lastKey)
{
        while(1)
        {
                *lastKey=KeyScan();
                if((*lastKey>='0') && (*lastKey<='9'))
                {
                        *num=(*num * 10)+(*lastKey-'0');
                        CmdLCD(GOTO_LINE2_POS0);
                 
                U32LCD(*num); 

                        while(ColScan()==0);
                }
                else
                {
                        if(*lastKey=='C')
                        {
                                CmdLCD(CLEAR_LCD);
                        }
                        while(ColScan()==0);
                        break;
                }
        }
} 
