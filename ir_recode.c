#include <stdio.h>
#include "ir_generic.h"
#include "ir_recode.h"

//Number of defined codes in code set #1
#define IR_CODE_CNT 34
//Code set #1
#define IR_CODE1_NO_CODE  0xffffffff
#define IR_CODE1_POWER    0x40bfea15 /* STAND_BY */ 
#define IR_CODE1_MUTE     0x40bffa05
#define IR_CODE1_CHPLUS   0x40bf708f
#define IR_CODE1_CHMINUS  0x40bf28d7
#define IR_CODE1_VOLPLUS  0x40bf7887
#define IR_CODE1_VOLMINUS 0x40bf6897
#define IR_CODE1_ARR_UP   IR_CODE1_CHPLUS /*code missing..*/
#define IR_CODE1_ARR_DN   0x40bf28d7 /* Arrow DN, verified. */
#define IR_CODE1_ARR_LF   0x40bf6897 /* VOL- */
#define IR_CODE1_ARR_R    0xb4b45aa5 /* VOL+ */
#define IR_CODE1_ARR_OK   0x40bf48b7
#define IR_CODE1_MENU     0x40bf58a7
#define IR_CODE1_EXIT     0x40bfe817
#define IR_CODE1_NUM1     0x40bfda25
#define IR_CODE1_NUM2     0x40bff20d
#define IR_CODE1_NUM3     0x40bfca35
#define IR_CODE1_NUM4     0x40bf5aa5
#define IR_CODE1_NUM5     0x40bff00f
#define IR_CODE1_NUM6     0x40bf7a85
#define IR_CODE1_NUM7     0x40bf6a95
#define IR_CODE1_NUM8     0x40bf728d
#define IR_CODE1_NUM9     0x40bf4ab5
#define IR_CODE1_NUM0     0x40bfaa55
#define IR_CODE1_RET      0x40bfba45
#define IR_CODE1_RUUTU    0x807f2ad5
#define IR_CODE1_TEXT     0x0
#define IR_CODE1_TEXT2    0x0
#define IR_CODE1_RESIZE   0x40bf32cd /*NTSC-PAL*/
#define IR_CODE1_RED      0x40bf18e7 /*LANGUAGE*/
#define IR_CODE1_GREEN    0x40bf08f7 /* AUDIO */
#define IR_CODE1_YELLOW   0x40bf38c7 /* PAUSE */
#define IR_CODE1_BLUE     0x40bf8877 /*TELETEXT*/
#define IR_CODE1_PLAY     0xb4b48c73
#define IR_CODE1_REC      0x40bf9867
#define IR_CODE1_STOP     0xb4b49c63
#define IR_CODE1_PAUSE    0xb4b41ce3
#define IR_CODE1_FAV      0x40bf9a65
#define IR_CODE1_TOGGLE   0x40bfba45
#define IR_CODE1_DIGEST   0x40bf1ae5
#define IR_CODE1_TVMODE   0x0
#define IR_CODE1_INFO     0x0
#define IR_CODE1_TV_RADIO 0x40bf9867
#define IR_CODE1_SUBTITLE 0x40bf3ac5


//Code set #2
#define IR_CODE2_ZERO_CODE 0x00000000
#define IR_CODE2_POWER    0x807ff20d
#define IR_CODE2_MUTE     0x807f40bf
#define IR_CODE2_PAGEUP   0x807f926d /* PAGE UP */
#define IR_CODE2_PAGEDN  0x807f00ff /* PAGE DN */
#define IR_CODE2_ARR_UP   0x807f50af
#define IR_CODE2_ARR_DN   0x807fd02f
#define IR_CODE2_ARR_LF   0x807ff00f
#define IR_CODE2_ARR_R    0x807f708f
#define IR_CODE2_ARR_OK   0x807f18e7
#define IR_CODE2_MENU     0x807fd22d
#define IR_CODE2_EXIT     0x807f807f
#define IR_CODE2_NUM1     0x807f32cd
#define IR_CODE2_NUM2     0x807fb24d
#define IR_CODE2_NUM3     0x807f728d
#define IR_CODE2_NUM4     0x807f02fd
#define IR_CODE2_NUM5     0x807f827d
#define IR_CODE2_NUM6     0x807f42bd
#define IR_CODE2_NUM7     0x807f22dd
#define IR_CODE2_NUM8     0x807fa25d
#define IR_CODE2_NUM9     0x807f38c7
#define IR_CODE2_NUM0     0x807f52ad
#define IR_CODE2_RET      
#define IR_CODE2_RECALL   0x807fca35
#define IR_CODE2_EPG      0x807fe817
#define IR_CODE2_BAR      0x807ff807 /* Between EPG and FAV */
#define IR_CODE2_TEXT     
#define IR_CODE2_TEXT2    
#define IR_CODE2_RESIZE   0x807fb04f /* 4:3/16:9 */
#define IR_CODE2_RED      0x807f9a65 /*LANGUAGE*/
#define IR_CODE2_GREEN    0x807fba45 /*  */
#define IR_CODE2_YELLOW   0x807faa55 /* SUBTITLE */
#define IR_CODE2_BLUE     0x807f7887 /* AUDIO L/R */
#define IR_CODE2_PLAY     
#define IR_CODE2_REC      
#define IR_CODE2_STOP     
#define IR_CODE2_PAUSE    
#define IR_CODE2_FAV      0x807fe01f 
#define IR_CODE2_TOGGLE   
#define IR_CODE2_DIGEST   
#define IR_CODE2_TV_AV    0x807f2ad5
#define IR_CODE2_TV_RADIO 0x807fe21d
#define IR_CODE2_BUTTON1  0x807f6897 /* Between Text TV and TV/Radio */
#define IR_CODE2_TXT_TV   0x807f30cf
#define IR_CODE2_SUBTITLE 

#define IR_CODE2_NO_CODE  0xffffffff

uint32_t IR_CODETAB[IR_CODE_CNT][2] = {{IR_CODE1_NO_CODE, IR_CODE2_ZERO_CODE},
				       {IR_CODE1_POWER,   IR_CODE2_POWER},
				       {IR_CODE1_MUTE,    IR_CODE2_MUTE},
				       {IR_CODE1_NUM1,    IR_CODE2_NUM1},
                                       {IR_CODE1_NUM2,    IR_CODE2_NUM2},
                                       {IR_CODE1_NUM3,    IR_CODE2_NUM3},
                                       {IR_CODE1_NUM4,    IR_CODE2_NUM4},
                                       {IR_CODE1_NUM5,    IR_CODE2_NUM5},
                                       {IR_CODE1_NUM6,    IR_CODE2_NUM6},
                                       {IR_CODE1_NUM7,    IR_CODE2_NUM7},
                                       {IR_CODE1_NUM8,    IR_CODE2_NUM8},
                                       {IR_CODE1_NUM9,    IR_CODE2_NUM9},
				       {IR_CODE1_FAV,     IR_CODE2_FAV},
                                       {IR_CODE1_NUM0,    IR_CODE2_NUM0},
				       {IR_CODE1_TOGGLE,  IR_CODE2_BUTTON1},
				       {IR_CODE1_DIGEST,  IR_CODE2_NO_CODE}/**/,
				       {IR_CODE1_TVMODE,  IR_CODE2_NO_CODE}/**/,
				       {IR_CODE1_INFO,    IR_CODE2_NO_CODE}/**/,
				       {IR_CODE1_MENU,    IR_CODE2_MENU},
				       {IR_CODE1_RUUTU,   IR_CODE2_EPG}/**/,
				       {IR_CODE1_EXIT,    IR_CODE2_EXIT},
				       {IR_CODE1_CHPLUS  ,IR_CODE2_PAGEUP},
				       {IR_CODE1_CHMINUS ,IR_CODE2_PAGEDN},
				       {IR_CODE1_ARR_UP,  IR_CODE2_ARR_UP},
				       {IR_CODE1_ARR_LF  ,IR_CODE2_ARR_LF},
				       {IR_CODE1_ARR_OK,  IR_CODE2_ARR_OK},
				       {IR_CODE1_VOLPLUS, IR_CODE2_ARR_R},
				       {IR_CODE1_ARR_DN,  IR_CODE2_ARR_DN},
				       {IR_CODE1_RED,     IR_CODE2_RED},
				       {IR_CODE1_GREEN,   IR_CODE2_GREEN},
				       {IR_CODE1_YELLOW,  IR_CODE2_YELLOW},
				       {IR_CODE1_TV_RADIO, IR_CODE2_TV_RADIO},
				       {IR_CODE1_BLUE,     IR_CODE2_BLUE},
				       {IR_CODE1_SUBTITLE, IR_CODE2_YELLOW}};


uint32_t ir_recode(uint32_t ir_recv_code)
{
  uint32_t ret = IR_CODE2_NO_CODE;
  uint8_t i;
  dbg_printf(":::IR_CODETAB\n");
  for(i=0;i<IR_CODE_CNT;i++)
    {
      dbg_printf(":::0x%lx 0x%lx\n", IR_CODETAB[i][0], IR_CODETAB[i][1]);
      if (IR_CODETAB[i][1] == ir_recv_code) {
	ret = IR_CODETAB[i][0];
	break;
      }
    }
  return ret;
}

void ir_make_time_buf(uint32_t ir_code, uint16_t *ir_time_buf)
{
  uint8_t i = 0, bit_cnt;
  
  ir_time_buf[i++] = IR_START;
  
  for(bit_cnt=0;bit_cnt<32;bit_cnt++) {
    uint8_t bit;
    bit = (ir_code & 0x80000000) >> 31;
    ir_code = ir_code << 1;
    ir_time_buf[i++] = bit;
  }
  
  ir_time_buf[i++] = IR_STOP;

  for(;i<IRCODE_BUF_SZ;i++)
    {
      ir_time_buf[i] = IR_NOP;
    }
}

uint8_t ir_time_diff_decode(uint16_t timediff)
{
  uint8_t out;
  
  if ((timediff > 12) && (timediff < 28))
    out = 0;
  else if ((timediff >= 28) && (timediff < 55))
    out = 1;
  else if ((timediff >= 55) && (timediff < 90))
    out = IR_CONT;
  else if ((timediff >= 90) && (timediff < 110))
    out = IR_REP;
  else if ((timediff >= 150) && (timediff < 170))
    out = IR_START;
  else if ((timediff >= 450) && (timediff < 600))
    out = IR_STOP;
  else if (timediff == IR_MAX_WAIT_TIME)
    out = IR_END;  
  else
    out = IR_START; //probably start..

  return out;
}

uint32_t ir_time_buf_decode(uint16_t *ir_time_buf, uint16_t *time_buf_len)
{
  uint32_t codeval=0;
  uint8_t bit_cnt=0;
  uint8_t i;

  dbg_printf("LEN=%d\n",*time_buf_len);
  
  for(i=0;i<*time_buf_len-1;i++)
    dbg_printf("%d,",ir_time_buf[i+1] - ir_time_buf[i]);
  dbg_printf("[DIFF]\n");

  for(i=0;i<*time_buf_len;i++)
    {
      uint16_t t1,t2;
      uint8_t out = 0;
      t1 = ir_time_buf[i];
      t2 = ir_time_buf[i+1];
      
      out = ir_time_diff_decode(t2 - t1);
      
      dbg_printf("%d ", t1);
      ir_time_buf[i] = out;
    }

  dbg_printf("[t1]\n");	
  for(i=0;i<*time_buf_len-1;i++)
    dbg_printf("%d,",ir_time_buf[i]);
  dbg_printf("[out]\n");
  
  *time_buf_len = *time_buf_len - 1; //counted timestamps, now counts bits

  //calculate uint32 value
  for(i=0;i<*time_buf_len;i++)
    {
      if(ir_time_buf[i] < 2) {
	codeval = (codeval << 1) | ir_time_buf[i];
	bit_cnt++;
      }
    }

  if(bit_cnt == 32)
    return codeval;
  else
    return IR_CODE1_NO_CODE;
}

uint8_t ir_digit_check_and_get(uint8_t bit_cnt, uint8_t *bit_buf)
{
  uint8_t digit1, digit2;
  uint8_t pattern1, pattern2;
  
  digit1 = (~bit_buf[2] >> 2) & 0x3f;
  pattern1 = ((bit_buf[2] << 4) & 0x30) + ((bit_buf[1] >> 4) & 0x0f);
  digit2 =   ((bit_buf[1] << 2) & 0x3c) + ((bit_buf[0] >> 6) & 0x03);
  pattern2 = bit_buf[0] & 0x3f;

  dbg_printf("ir_digit_check_and_get: %x %x %x %x", bit_buf[2], bit_buf[1], bit_buf[0], 0);
  dbg_printf(": %x %x %x %x ", digit1, pattern1, digit2, pattern2   );

  if(pattern1 & 0x10)
    {
      dbg_printf("Is Button. %x ", pattern1);
      pattern1 &= ~0x10;
      dbg_printf("Is Button. %x ", pattern1);      
    }
  
  if(pattern1 == IR_BIT_PATTERN1 &&
     pattern2 == IR_BIT_PATTERN2 &&
     digit1   == digit2)
    {
      dbg_printf("Digit %d accepted.\n", digit1);
      return digit1;
    }
  return -1;
}
