#include "sys.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    

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

unsigned char Read_AIN(unsigned char chn)
{
	unsigned char dat;
	unsigned int val;
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
	val = (dat * 500) / 255;
	EA = 1;

	return val;	
}

