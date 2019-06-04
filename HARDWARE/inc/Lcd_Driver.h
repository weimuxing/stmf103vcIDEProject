
#ifndef _LCD_DRIVER_H_
#define _LCD_DRIVER_H_
#include "main.h"
//-----------------------------SPI ��������--------------------------------------//
#define USE_HARDWARE_SPI     1  //1:Enable Hardware SPI;0:USE Soft SPI

//-------------------------��Ļ������������--------------------------------------//
#define LCD_X_SIZE	        176
#define LCD_Y_SIZE	        220

/////////////////////////////////////�û�������///////////////////////////////////	 
//֧�ֺ��������ٶ����л�
#define USE_HORIZONTAL  		1	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.

#ifdef USE_HORIZONTAL//��������˺��� 
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE
#endif
//////////////////////////////////////////////////////////////////////////////////
	 






#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111


#if 0
//�����Գ���ʹ�õ���ģ��SPI�ӿ�����
//�����ɸ��Ľӿ�IO���ã�ʹ����������4 IO������ɱ���Һ��������ʾ
/******************************************************************************
�ӿڶ�����Lcd_Driver.h�ڶ��壬����ݽ����޸Ĳ��޸���ӦIO��ʼ��LCD_GPIO_Init()
#define LCD_CTRL   	  	GPIOB		//����TFT���ݶ˿�
#define LCD_LED        	GPIO_Pin_9  //PB9 ������TFT -LED
#define LCD_RS         	GPIO_Pin_10	//PB10������TFT --RS
#define LCD_CS        	GPIO_Pin_11 //PB11 ������TFT --CS
#define LCD_RST     	GPIO_Pin_12	//PB12������TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13������TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15������TFT - SDI
*******************************************************************************/
#define LCD_CTRL   	  	GPIOB		//����TFT���ݶ˿�
#define LCD_CS        	GPIO_Pin_12 //MCU_PB11--->>TFT --CS/CE
#define LCD_SCL        	GPIO_Pin_13	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_15	//PB15 MOSI--->>TFT --SDA/DIN
#define Status_LED      GPIO_Pin_8
#if 0
#define LCD_LED        	GPIO_Pin_9  //MCU_PB9--->>TFT --BL
#define LCD_RS         	GPIO_Pin_10	//PB11--->>TFT --RS/DC
#define LCD_CS        	GPIO_Pin_11 //MCU_PB11--->>TFT --CS/CE
#define LCD_RST     	GPIO_Pin_12	//PB10--->>TFT --RST

#endif
#define LCD_CTRL1   	GPIOE		//����TFT���ݶ˿�

#define LCD_LED        	GPIO_Pin_15  //MCU_PB9--->>TFT --BL
#define LCD_RS         	GPIO_Pin_13	//PB11--->>TFT --RS/DC
#define LCD_RST     	GPIO_Pin_14	//PB10--->>TFT --RST


//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//Һ�����ƿ���1�������궨��
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL1->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET  	LCD_CTRL1->BSRR=LCD_RST    
#define	LCD_LED_SET  	LCD_CTRL1->BSRR=LCD_LED   
#define Status_LED_SET  LCD_CTRL->BSRR=Status_LED 
//Һ�����ƿ���0�������궨��
#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	LCD_CTRL1->BRR=LCD_RS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    
#define	LCD_RST_CLR  	LCD_CTRL1->BRR=LCD_RST    
#define	LCD_LED_CLR  	LCD_CTRL1->BRR=LCD_LED 
#define Status_LED_CLR  LCD_CTRL->BRR=Status_LED 

#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->IDR;   //��������

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} LCD_RS_GPIO_Port
#else

    #define	LCD_CS_SET  	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,  LCD_CS_Pin, GPIO_PIN_SET )
    #define	LCD_RS_SET  	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET )
    #define	LCD_RST_SET  	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port ,LCD_RESET_Pin, GPIO_PIN_SET) 
    #define	LCD_LED_SET  	HAL_GPIO_WritePin(LCD_BL_EN_GPIO_Port ,  LCD_BL_EN_Pin, GPIO_PIN_SET )  
    //#define Status_LED_SET  HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) 
    //Һ�����ƿ���0�������궨��
    #define	LCD_CS_CLR  	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,  LCD_CS_Pin,  GPIO_PIN_RESET)   
    #define	LCD_RS_CLR  	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin,  GPIO_PIN_RESET)     
    #define	LCD_RST_CLR  	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port , LCD_RESET_Pin, GPIO_PIN_RESET)   
    #define	LCD_LED_CLR  	HAL_GPIO_WritePin(LCD_BL_EN_GPIO_Port ,  LCD_BL_EN_Pin, GPIO_PIN_RESET )
    //#define Status_LED_CLR  HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)

    #define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
    #define LCD_DATAIN     LCD_DATA->IDR;   //��������

    
#endif


void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void Lcd_WriteReg(u8 Index,u8 Data);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Lcd_SetRegion(u8 x_start,u8 y_start,u8 x_end,u8 y_end);
void Lcd_WriteData_16Bit(u16 Data);

#endif
