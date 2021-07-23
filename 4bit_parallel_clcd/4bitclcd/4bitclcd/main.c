#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT      PORTA      // 1602 LCD 포트
#define LCD_DDR      DDRA      // 1602 LCD 방향

#define RS_PIN         0      // RS 제어 핀의 비트 번호
#define RW_PIN         1      // R/W 제어 핀의 비트 번호
#define E_PIN         2      // E 제어 핀의 비트 번호
#define D4_PIN			4
#define D5_PIN			5
#define D6_PIN			6
#define D7_PIN			7

#define COMMAND_CLEAR_DISPLAY   0x01
#define COMMAND_8_BIT_MODE      0x38   // 8비트, 2라인, 5x8 폰트
#define COMMAND_4_BIT_MODE      0x28   // 4비트, 2라인, 5x8 폰트

#define COMMAND_DISPLAY_ON_OFF_BIT      2
#define COMMAND_CURSOR_ON_OFF_BIT      1
#define COMMAND_BLINK_ON_OFF_BIT      0

void LCD_pulse_enable(void)       // 하강 에지에서 동작
{
	LCD_PORT |= (1 << E_PIN);   // E를 HIGH로
	_delay_us(1);
	LCD_PORT &= ~(1 << E_PIN);   // E를 LOW로
	_delay_ms(1);
}

void LCD_write_data(uint8_t data)
{
	LCD_PORT |= (1 << RS_PIN);   // 문자 출력에서 RS는 1
	uint8_t tmp = LCD_PORT & 0x0F;
	LCD_PORT = (data & 0xF0) | tmp;
	LCD_pulse_enable();         // 문자 출력
	_delay_ms(2);
	LCD_PORT = ((data << 4) & 0xF0) | tmp;
	LCD_pulse_enable();         // 문자 출력
	_delay_ms(2);
}

void LCD_write_command(uint8_t command)
{
	LCD_PORT &= ~(1 << RS_PIN);   // 명령어 실행에서 RS는 0
	uint8_t tmp = LCD_PORT & 0x0F;
	LCD_PORT =(command & 0xF0) | tmp;      // 데이터 핀에 명령어 전달
	LCD_pulse_enable();         // 명령어 실행
	_delay_ms(2);
	LCD_PORT = ((command << 4) & 0xF0) | tmp;      // 데이터 핀에 명령어 전달
	LCD_pulse_enable();         // 명령어 실행
	_delay_ms(2);
}

void LCD_clear(void)
{
	LCD_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_init(void)
{
	_delay_ms(50);            // 초기 구동 시간
	DDRB = 0xFF;
	
	// 연결 핀을 출력으로 설정
	LCD_DDR |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN) | (1 << D4_PIN) | (1 << D5_PIN) | (1 << D6_PIN) | (1 << D7_PIN);
	LCD_PORT = 0x00;
	LCD_PORT |= (1 << D5_PIN);
	LCD_pulse_enable();

	// RW 핀으로 LOW를 출력하여 쓰기 전용으로 사용
	LCD_PORT &= ~(1 << RW_PIN);
	
	LCD_write_command(COMMAND_4_BIT_MODE);      // 4비트 모드
	
	// display on/off control
	// 화면 on, 커서 off, 커서 깜빡임 off
	LCD_write_command(0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT));

	LCD_clear();            // 화면 지움

	// Entry Mode Set
	// 출력 후 커서를 오른쪽으로 옮김, 즉, DDRAM의 주소가 증가하며 화면 이동은 없음
	LCD_write_command(0x06);
}

void LCD_write_string(char *string)
{
	uint8_t i;
	for(i = 0; string[i]; i++)         // 종료 문자를 만날 때까지
		LCD_write_data(string[i]);      // 문자 단위 출력
}

void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %= 16;      // [0 15]
	row %= 2;      // [0 1]

	// 첫째 라인 시작 주소는 0x00, 둘째 라인 시작 주소는 0x40	
	LCD_write_command(0x80 + (0x40 * row) + col);   // 커서 이동
}

int main(void)
{
	LCD_init();               // 텍스트 LCD 초기화
	LCD_clear();            // 화면 지움
	
	LCD_goto_XY(0, 0);         // 0행 0열로 이동
	LCD_write_string("aadfdncvn");         // 문자 단위 출력
	LCD_goto_XY(1, 4);         // 0행 0열로 이동
	LCD_write_string("dnzdgcc");         // 문자 단위 출력
		
	/*
	_delay_ms(1000);         // 1초 대기
	
	LCD_clear();            // 화면 지움
	
	// 화면에 보이는 영역은 기본값으로 0~1행, 0~15열로 설정되어 있다.
	LCD_goto_XY(0, 0);         // 0행 0열로 이동
	LCD_write_data('a');         // 문자 단위 출력
	LCD_goto_XY(0, 5);
	LCD_write_data('2');
	LCD_goto_XY(1, 0);
	LCD_write_data('c');
	LCD_goto_XY(1, 5);
	LCD_write_data('4');
	*/
	while(1) {
		
	}
	return 0;
}