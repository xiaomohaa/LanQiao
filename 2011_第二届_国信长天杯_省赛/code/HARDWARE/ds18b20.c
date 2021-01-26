 #include "sys.h"

sbit DS18B20_IO = P1^4;

void Delayus(unsigned int us)
{
	do{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(--us);
}

bit Get18B20Ack(void)
{
	bit ack;
	
	DS18B20_IO = 0;			//����500us�ĸ�λ����
	Delayus(500);
//	EA = 0;							//��ֹ���ж�
	ET0 = 0;
	DS18B20_IO = 1;			//��ʱ60us
	Delayus(60);
//	EA = 1;							//����ʹ�����ж�
	ET0 = 1;
	ack = DS18B20_IO;		//��ȡ��������
	while(!DS18B20_IO);	//�ȴ������������
	
	return ack; 	
}

void DS18B20Write(unsigned char dat)
{
 	unsigned char mask;
						
	for(mask = 0x01; mask != 0; mask <<= 1)	//��λ���ȣ������Ƴ�8��bit
	{
//		EA = 0;
		ET0 = 0;
		DS18B20_IO = 0;				//����2us�͵�ƽ����
		Delayus(2);
	 	if(dat & mask)				//�����bitֵ
		 	DS18B20_IO = 1;
		else
		 	DS18B20_IO = 0;
//		EA = 1;
		ET0 = 1;
		Delayus(60);					//��ʱ60us
		DS18B20_IO = 1;				//����ͨ������
	}
}

unsigned char DS18B20Read(void)
{
 	unsigned char mask, dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<= 1)	//��λ���ȣ����βɼ�8��bit
	{
//		EA = 0;
		ET0 = 0;
		DS18B20_IO = 0;				//����2us�͵�ƽ����
		Delayus(2);
		DS18B20_IO = 1;				//�����͵�ƽ���壬�ȴ�18B20�������
		Delayus(2);						//��ʱ2us
	 	if(DS18B20_IO)				//��ȡͨ�������ϵ�ֵ
		 	dat |= mask;
//		EA = 1;
		ET0 = 1;
		Delayus(60);					//����ʱ60us
	}
	
	return dat;	
}

bit Start18B20()
{
 	bit ack;
	static bit flag = 1;

	ack = Get18B20Ack();		//ִ�����߸�λ������ȡ18B20Ӧ��
	if(ack == 0)						//��18B20��ȷӦ��������һ��ת��
	{
		DS18B20Write(0xCC);		//����ROM����
		
		if(flag)
		{
			flag = 0;
			DS18B20Write(0x4e);			//д�ݴ���ָ��4E
			DS18B20Write(0x63);			//д���ٻ�����TH������ֵ99��
			DS18B20Write(0x00);			//д���ٻ�����TL������ֵ0��
			DS18B20Write(0x1f);			//д���üĴ���4
															//0x1f �� 0.5000��C  ת��ʱ��93.75ms
															//0x3f �� 0.2000��C  ת��ʱ��187.5ms
															//0x5f �� 0.1250��C  ת��ʱ��375ms
															//0x7f �� 0.0625��C  ת��ʱ��750ms
		}
		
		ack = Get18B20Ack();		//ִ�����߸�λ������ȡ18B20Ӧ��
		if(ack == 0)						//��18B20��ȷӦ��������һ��ת��
		{
			DS18B20Write(0xCC);		//����ROM����
			DS18B20Write(0x44);		//����һ���¶�ת��
		}
	}

	return ~ack;						//ack == 0 ��ʾ�����ɹ������Է���ֵ����ȡ��
}

bit Get18B20Temp(int *temp)
{
 	bit ack;
	unsigned char LSB, MSB;			//16bit�¶�ֵ�ĵ��ֽں͸��ֽ�

	ack = Get18B20Ack();				//ִ�����߸�λ������ȡ18B20Ӧ��
	if(ack == 0)								//��18B20��ȷӦ�����ȡ�¶�ֵ
	{
	 	DS18B20Write(0xCC);				//����ROM����
		DS18B20Write(0xBE);				//���Ͷ�����
		LSB = DS18B20Read();			//���¶�ֵ�ĵ��ֽ�
		MSB = DS18B20Read();			//���¶�ֵ�ĸ��ֽ�
		*temp = ( MSB << 8) + LSB;	//�ϳ�16bit������
	}

	return ~ack;								//ack == 0 ��ʾ����Ӧ�����Է���ֵΪ1��ȡ��ֵ
} 

