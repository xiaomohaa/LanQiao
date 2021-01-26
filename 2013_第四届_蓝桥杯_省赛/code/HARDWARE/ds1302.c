#include "sys.h"

sbit DS1302_IO = P2^3;
sbit DS1302_CK = P1^7;
sbit DS1302_CE = P1^3;

void DS1302ByteWrite(unsigned char dat) 
{
	unsigned char mask;
	DS1302_IO = 1;					//����IO����
	for(mask = 0x01; mask != 0; mask <<= 1)	//��λ��ǰ����λ�Ƴ�
	{
		if((dat&mask) != 0)		//���������λ����
			DS1302_IO = 1;
		else
			DS1302_IO = 0;
		DS1302_CK = 1;				//����ʱ����
		DS1302_CK = 0;				//����ʱ���ߣ����һ��λ�Ĳ���
	}
	DS1302_IO = 1;  				//д��֮��ȷ���ͷ�IO����
}  

unsigned char DS1302ByteRead(void)
{
	unsigned char mask, dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<= 1)	//��λ��ǰ����λ��ȡ
	{
		if(DS1302_IO)			//���ȶ�ȡ��ʱ��IO���ţ�������dat�еĶ�Ӧλ
			dat |= mask;

		DS1302_CK = 1;		//����ʱ��
		DS1302_CK = 0;		//������ʱ�ӣ����һ��λ�Ĳ���
	}
	return dat;					//���ض������ֽ�����
} 

void DS1302SingleWrite(unsigned char reg, unsigned char dat)     
{
	DS1302_CE = 1;					//ʹ��Ƭѡ�ź�
	DS1302ByteWrite((reg << 1) | 0x80);	//����д�Ĵ���ָ��
	DS1302ByteWrite(dat);		//д���ֽ�����
	DS1302_CE = 0;					//����Ƭѡ�ź�
}

unsigned char DS1302SingleRead(unsigned char reg)
{
	unsigned char dat;
	
	DS1302_CE = 1;					//ʹ��Ƭѡ�ź�
	DS1302ByteWrite((reg << 1) | 0x81);	//���Ͷ��Ĵ���ָ��
	dat = DS1302ByteRead();	//��ȡ�ֽ�����
	DS1302_CE = 0;					//����Ƭѡ�ź�
	
	DS1302_IO = 0;					//���ֽڶ�����ӵģ�
	
	return dat;         
}

void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);				//����ͻ��д�Ĵ���ָ��
	for(i = 0; i < 7; i ++)				//����д��8�ֽ�����
		DS1302ByteWrite(*dat++);        
	DS1302_CE = 0;      
}

void DS1302BurstRead (unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBF);				//����ͻ�����Ĵ���ָ��
	for(i = 0; i < 7; i++)				//������ȡ8���ֽ�
		dat[i] = DS1302ByteRead();      
	DS1302_CE = 0;  
	
	DS1302_IO = 0;								//ͻ���������
}	

void GetRealTime(unsigned char *time)
{
	unsigned char buf[8];
	
	DS1302BurstRead(buf);
	time[0] = buf[6];
	time[1] = buf[4];
	time[2] = buf[3];
	time[3] = buf[2];
	time[4] = buf[1];
	time[5] = buf[0];
	time[6] = buf[5];
}

void SetRealTime(unsigned char *time)
{
	unsigned char buf[8];
	
	buf[7] = 0;
	buf[6] = time[0];
	buf[4] = time[1];
	buf[3] = time[2];
	buf[2] = time[3];
	buf[1] = time[4];
	buf[0] = time[5];
	buf[5] = time[6];
	DS1302BurstWrite(buf);
}

void InitDS1302(void)
{
	unsigned char dat;
	signed char i = 7;
	unsigned char timeinit[8] = {0x19, 0x03, 0x11, 0x08, 0x30, 0x00, 0x02};
	
	DS1302_CE = 0;								//��ʼ��DS1302ͨ������
	DS1302_CK = 0;
	dat = DS1302SingleRead(0);			//��ȡ��Ĵ���
	DS1302SingleWrite(7, 0x00);  	//����д����������д������
	SetRealTime(&timeinit);					//����DS1302ʱ��
	DS1302SingleWrite(7, 0x80);  	//д�����Խ�ֹд������
}

