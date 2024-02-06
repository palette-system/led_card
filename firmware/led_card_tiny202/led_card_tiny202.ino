
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define NUM_LED 34
#define mbPORTA 0x01 << 7
uint8_t PixData[NUM_LED];
uint8_t rgb_m[3];
uint8_t light_pw;
uint8_t btn_flag;
uint8_t ncolor;

int hold_index;
uint8_t select_index;
uint8_t color_index;
uint8_t play_mode;
short anindex;

void SendPixData() {
	cli();
	for(uint8_t iLED=0; iLED<NUM_LED; iLED++) {
    if (PixData[iLED] == 0) {
      rgb_m[0] = 0; rgb_m[1] = 0; rgb_m[2] = 0;
    } else if (PixData[iLED] == 1) { // 緑
      rgb_m[0] = light_pw; rgb_m[1] = 0; rgb_m[2] = 0;
    } else if (PixData[iLED] == 2) { // 赤
      rgb_m[0] = 0; rgb_m[1] = light_pw; rgb_m[2] = 0;
    } else if (PixData[iLED] == 3) { // 青
      rgb_m[0] = 0; rgb_m[1] = 0; rgb_m[2] = light_pw;
    } else if (PixData[iLED] == 4) { // 黄色
      rgb_m[0] = light_pw; rgb_m[1] = light_pw; rgb_m[2] = 0;
    } else if (PixData[iLED] == 5) { // 水色
      rgb_m[0] = light_pw; rgb_m[1] = 0; rgb_m[2] = light_pw;
    } else if (PixData[iLED] == 6) { // 赤
      rgb_m[0] = 0; rgb_m[1] = light_pw; rgb_m[2] = 0;
    } else if (PixData[iLED] == 7) {// 白
      rgb_m[0] = light_pw; rgb_m[1] = light_pw; rgb_m[2] = light_pw;
    } else if (PixData[iLED] == 10) {
      rgb_m[0] = 0; rgb_m[1] = light_pw / 10; rgb_m[2] = 0;
    }
		for(uint8_t iRGB=0; iRGB<3; iRGB++) {
      uint8_t txByte = rgb_m[iRGB];
			for(uint8_t iBIT=0; iBIT<8; iBIT++) {
				uint8_t mbData = txByte & 0x80;
				if(!mbData) {	// Send 1
					VPORTA.OUT |= mbPORTA;
					asm volatile (
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					::
					);
					VPORTA.OUT &= ~mbPORTA;
					asm volatile (
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					::
					);
				} else {		// Send 0:
					VPORTA.OUT |= mbPORTA;
					asm volatile (
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					::
					);
					VPORTA.OUT &= ~mbPORTA;
					asm volatile (
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					"nop\n\t"
					::
					);
				}
				txByte = txByte << 1;
			}
		}
	}
	sei();
}



void ClrALL(void) {
	for(uint8_t i=0; i<NUM_LED; i++) {
		PixData[i] = 0;
	}
}

void SetPAT8(void) {
  short i;
	for(i=0; i<9; i++) {
    if (i == select_index) {
      PixData[i] = color_index;
    } else if (select_index == 8 && i == 7) {
      PixData[i] = color_index;
    } else {
      PixData[i] = ncolor;
    }
	}
  for(i=9; i<34; i++) { PixData[i] = 0; }
}

// 小さいくるくる
void SetPAT25_A(int s) {
  short i;
  for(i=0; i<34; i++) { PixData[i] = 0; }
	for(i=9; i<34; i++) {
    if (anindex == 0 && (i == 25 || i == 17)) PixData[i] = color_index;
    if (anindex == 1 && (i == 26 || i == 16)) PixData[i] = color_index;
    if (anindex == 2 && (i == 27 || i == 15)) PixData[i] = color_index;
    if (anindex == 3 && (i == 20 || i == 22)) PixData[i] = color_index;
	}
  anindex++;
  if (anindex > 3) { anindex = 0; select_index++; }
  SendPixData();
  for (i=0; i<s; i++) { _delay_ms(50); }
}

// 29, 30, 31, 32, 33
// 24, 25, 26, 27, 28
// 19, 20, 21, 22, 23
// 14, 15, 16, 17, 18
//  9, 10, 11, 12, 13
// 大きいくるくる
void SetPAT25_B(int s) {
  short i;
  for(i=0; i<34; i++) { PixData[i] = 0; }
	for(i=9; i<34; i++) {
    if (anindex == 0 && (i == 30 || i == 12)) PixData[i] = color_index;
    if (anindex == 1 && (i == 31 || i == 11)) PixData[i] = color_index;
    if (anindex == 2 && (i == 32 || i == 10)) PixData[i] = color_index;
    if (anindex == 3 && (i == 33 || i == 9)) PixData[i] = color_index;
    if (anindex == 4 && (i == 28 || i == 14)) PixData[i] = color_index;
    if (anindex == 5 && (i == 19 || i == 23)) PixData[i] = color_index;
    if (anindex == 6 && (i == 24 || i == 18)) PixData[i] = color_index;
    if (anindex == 7 && (i == 29 || i == 13)) PixData[i] = color_index;
	}
  anindex++;
  if (anindex > 7) { anindex = 0; select_index++; }
  SendPixData();
  for (i=0; i<s; i++) { _delay_ms(50); }
}

// 29, 30, 31, 32, 33
// 24, 25, 26, 27, 28
// 19, 20, 21, 22, 23
// 14, 15, 16, 17, 18
//  9, 10, 11, 12, 13
// アルファベット
void SetPAT25_C(int s) {
  short i, j, k, p;
  uint8_t m[5] = {29, 24, 19, 14, 9};

  for(i=0; i<34; i++) { PixData[i] = 0; }
  if (anindex == 0) { // A
    PixData[9] = PixData[14] = PixData[19] = PixData[24] = PixData[30] = PixData[31] = PixData[32] = PixData[28] = PixData[23] = color_index;
    PixData[18] = PixData[13] = PixData[20] = PixData[21] = PixData[22] = color_index;
  }
  if (anindex == 1) { // B
    PixData[29] = PixData[30] = PixData[31] = PixData[32] = PixData[24] = PixData[28] = PixData[19] = PixData[20] = PixData[21] = color_index;
    PixData[22] = PixData[14] = PixData[18] = PixData[9] = PixData[10] = PixData[11] = PixData[12] = color_index;
  }
  if (anindex == 2) { // C
    PixData[30] = PixData[31] = PixData[32] = PixData[24] = PixData[19] = PixData[14] = PixData[10] = PixData[11] = PixData[12] = color_index;
    PixData[28] = PixData[18] = color_index;
  }
  if (anindex == 3) { // D
    PixData[29] = PixData[30] = PixData[31] = PixData[32] = PixData[24] = PixData[28] = PixData[19] = PixData[23] = PixData[14] = color_index;
    PixData[18] = PixData[9] = PixData[10] = PixData[11] = PixData[12] = color_index;
  }
  anindex++;
  if (anindex > 3) { anindex = 0; select_index++; }
  for (i=0; i<=140; i+=10) {
    light_pw = i;
    SendPixData();
    _delay_ms(50);
  }
  for (i=0; i<s; i++) { _delay_ms(50); }
  for (k=0; k<5; k++) {
    for (i=0; i<5; i++) {
      p = m[i];
      for (j=0; j<4; j++) {
        PixData[p+j] = PixData[p+j+1];
      }
      PixData[p+4] = 0;
    }
    SendPixData();
    _delay_ms(200);
  }
  _delay_ms(1000);

}
// 29, 30, 31, 32, 33
// 24, 25, 26, 27, 28
// 19, 20, 21, 22, 23
// 14, 15, 16, 17, 18
//  9, 10, 11, 12, 13


int main(void)
{
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);
	VPORTA.DIR |= mbPORTA;
  pinMode(PIN_PA6, INPUT_PULLUP);
  select_index = 4;
  color_index = 5;
  play_mode = 0;
  btn_flag = 0;
  light_pw = 0;
  ncolor = 10;
  anindex = 0;
  short i;

	
	while(1)
	{
    if (select_index < 9) {

      for (i=0; i<240; i+=20) {
        light_pw = i;
        SetPAT8();
        SendPixData();
        _delay_ms(50);
      }
      /*
      n = 0;
      while (n < 3) {
        for (i=240; i>=180; i-=10) {
          light_pw = i;
          SetPAT8();
          SendPixData();
          _delay_ms(50);
        }
        for (i=180; i<240; i+=10) {
          light_pw = i;
          SetPAT8();
          SendPixData();
          _delay_ms(50);
        }
        n++;
      }
      */
      _delay_ms(1000);
      for (i=250; i>=0; i-=10) {
        light_pw = i;
        SetPAT8();
        SendPixData();
        _delay_ms(50);
      }
      _delay_ms(200);
      select_index++;
      if (select_index == 7) select_index = 8;
    } else {
      if (select_index == 9) { anindex = 0; select_index = 10; light_pw = 240; }
      if (select_index >= 10 && select_index <= 12) SetPAT25_A(10);
      if (select_index == 13 || select_index == 14 || select_index == 17 || select_index == 18) SetPAT25_B(2);
      if (select_index == 15 || select_index == 16 || select_index == 19 || select_index == 20) SetPAT25_A(2);
      if (select_index == 21) SetPAT25_C(40);
      

    }

    if (select_index >= 22) {
      select_index = 0;
      color_index++;
      if (ncolor == 0) { ncolor = 10; }
      else if (ncolor == 10) { ncolor = 0; }
      if (color_index > 6) {
        color_index = 1;
      }
    }
  }

}