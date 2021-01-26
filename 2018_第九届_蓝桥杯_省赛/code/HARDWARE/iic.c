/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "sys.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    


#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//�������Ŷ���
sbit SDA = P2^1;  /* ������ */
sbit SCL = P2^0;  /* ʱ���� */


//������������
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//Ӧ��λ����
void IIC_Ack(bit ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

/*******************************************************************************
* ������	��Read_AIN
* ����ֵ	��unsigned char chn
* ����ֵ	��unsigend char dat
* ����		��СĬhaa
* ʱ��		��2019��2��25��
* ������������ȡPCF8591AIN�ɼ�����
* ��ע		��chnΪPCF8591��ͨ��
*******************************************************************************/
unsigned char Read_AIN(unsigned char chn)
{
	unsigned char dat, val, ad_pwm;
	EA = 0;
	IIC_Start();						//IIC������ʼ�ź�							
	IIC_SendByte(0x90);			//PCF8591��д�豸��ַ		
	IIC_WaitAck();  		    //�ȴ��ӻ�Ӧ��		
	IIC_SendByte(chn); 			//д��PCF8591�Ŀ����ֽ�		
	IIC_WaitAck();  				//�ȴ��ӻ�Ӧ��						
	IIC_Stop(); 						//IIC����ֹͣ�ź�					
	
	IIC_Start();						//IIC������ʼ�ź�									
	IIC_SendByte(0x91); 	  //PCF8591�Ķ��豸��ַ		
	IIC_WaitAck(); 			    //�ȴ��ӻ�Ӧ��		
	dat = IIC_RecByte();	  //��ȡPCF8591ͨ��3������ 			
	IIC_Ack(0); 						//������Ӧ���ź�				
	IIC_Stop(); 						//IIC����ֹͣ�ź�		
	val = (dat * 50) / 255;
	
	if((val >= 38) && (val <= 50))
		ad_pwm = 4;
	else if((val >= 26) && (val < 38))
		ad_pwm = 3;
	else if((val >= 14) && (val < 26))
		ad_pwm = 2;
	else if((val >= 0) && (val < 14))
		ad_pwm = 1;
	
	EA = 1;
	
	return ad_pwm;	
}


void Write_E2PROM(unsigned char add, unsigned char dat)
{
	EA = 0;
  IIC_Start();
  IIC_SendByte(0xa0);	//����������ַ
  IIC_WaitAck();
  IIC_SendByte(add);	//���Ͳ�����ַ
  IIC_WaitAck();
  IIC_SendByte(dat);	//дһ�ֽ�
  IIC_WaitAck();
  IIC_Stop();
  somenop;
	EA = 1;
}

unsigned char Read_E2PROM(unsigned char add)
{
  unsigned char d;
	
	IIC_Start();
	IIC_SendByte(0xa0); 	//����������ַ
	IIC_WaitAck();
	IIC_SendByte(add);		//����Ҫ�����ĵ�ַ
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0xa1);		//���Ͷ�����
	IIC_WaitAck();
	d = IIC_RecByte();	//��һ�ֽ�
	IIC_Ack(0);
	IIC_Stop();
	return d;
}
