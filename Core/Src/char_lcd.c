//
// Created by yuno on 22. 3. 18..
//

#include "char_lcd.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define SLAVE_LCD_ADDRESS 0x27<<1


#define SBI(address,ab) ((address) |= (0x01<<(ab)))
#define CBI(address, ab) ((address) &= ~(0x01<<(ab)))
#define RS 0 //RS bit: 0 = Instruction, 1 = Data
#define RW 1 //RW bit: 0 = Write, 1 = Read
#define EN 2 //EN bit: expander output is connected to LCD's E pin. 폴링 엣지에서 샘플링
#define BL 3 //BACKLIGHT bit : 1 = Enable backlight




/* 참고: 하위 4bit 는 컨트롤 비트, 상위 4비트는 데이터 비트.
 *  하위 4bit : BACKLIGHT, EN, RW, RS
 *  3 BACKLIGHT bit : 1 = Enable backlight
 *  2 EN bit: expander output is connected to LCD's E pin. 폴링 엣지에서 샘플링
 *  1 RW bit: 0 = Write, 1 = Read
 *  0 RS bit: 0 = Instruction, 1 = Data
 */

/**
 * @brief LCD BF 상태 검사. 리턴값 & 0x80 하면 busy 검사 가능
 * @return LCD에서 읽은 데이터
 */

uint8_t LCD_BF(){
    uint8_t val=0;
    uint8_t TxData[2]={0b00000010 | 0x08, 0b00000110 | 0x08};

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, TxData, 2, 10);
    HAL_I2C_Master_Receive(&hi2c1,SLAVE_LCD_ADDRESS,&val,1,10);

    return val;
}


/**
 * @brief 데이터를 4비트씩 두번 LCD에 씀
 * @param rs 0 = instruction, 1 = data
 * @param data 8bit data
*/

void LCD_4bit_Write(uint8_t rs, uint8_t data){
    uint8_t TxData[3]={0x08, 0x08, 0x08};
    while(LCD_BF()&0x80){} // Wait LCD ready

    if(rs)
        SBI(TxData[0], RS);
    else
        CBI(TxData[0], RS);
    CBI(TxData[0], RW);

    TxData[0]= (data & 0b11110000) | (TxData[0] & 0b00001111); TxData[1]=TxData[0];
    CBI(TxData[0], EN);
    SBI(TxData[1], EN);
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, TxData, 2, 100);

    TxData[0]=(((data << 4) & 0b11110000) | (TxData[0] & 0b00001111)); TxData[1]= TxData[0];

    CBI(TxData[0], EN);
    SBI(TxData[1], EN);
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, TxData, 2, 100);

    TxData[0]=0x08;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, TxData, 1, 100);
}



/**
 * @brief init lcd
 */

void LCD_init(){

    uint8_t Txdata[2]={0x34, 0x30};

    HAL_Delay(20);
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, Txdata, 2, 10);
    HAL_Delay(5);
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, Txdata, 2, 10);
    HAL_Delay(1);
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, Txdata, 2, 10);
    HAL_Delay(1);
    Txdata[0]=0x20;
    Txdata[1]=0x24;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_LCD_ADDRESS, Txdata, 2 , 10);
    HAL_Delay(1);
    LCD_4bit_Write(0,0b00101000);    //function set //
    LCD_4bit_Write(0,0b00001000);    //display off
    LCD_4bit_Write(0,0b00000001);    //clear display
    LCD_4bit_Write(0,0b00000010);    //return home
    LCD_4bit_Write(0,0b00000110);    //entry mode set
    LCD_4bit_Write(0,0b00001100);    //display on    //cursor off*/
}


void LCD_Write_str(uint8_t *str, int length){
    for(int i=0;i<length;i++){
        if(str[i]=='\0')
            str[i]=' ';
        LCD_4bit_Write(1,str[i]);
    }
}

void LCD_set_cursor(uint8_t line, uint8_t pos){
    uint8_t Tx_data = 0x80;
    int line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    Tx_data += line_offsets[line] + pos;
    LCD_4bit_Write(0, Tx_data);
}

void LCD_clear(){
    uint8_t Tx_data = 0x01;
    LCD_4bit_Write(0, Tx_data);
}






