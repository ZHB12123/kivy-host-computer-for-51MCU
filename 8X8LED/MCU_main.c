/*********************************************
**					 ���пƼ�				**
**					 ����ʵ��				**
** ˵����P0�����У�����͵�ƽ��Ч��			**
**		 595��������У�����ߵ�ƽ��Ч��	**
*********************************************/

#include"reg51.h"
#include<intrins.h>

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;

/*******************************************************************************
* ������         : Hc595SendByte(unsigned char dat)
* ��������		   : ��74H595����һ���ֽڵ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Hc595SendByte(unsigned char dat)
{
	unsigned char a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();
	_nop_();
	RCLK=0;
}

unsigned char Data1[8]={0xe7,0xc3,0x81,0x24,0x66,0xe7,0xe7,0xe7};
//unsigned char Data2[8]={0xe7,0xe7,0xe7,0x66,0x24,0x81,0xc3,0xe7};
unsigned char Data[8];
unsigned char num;

void init();
void UsartInit();

void main(void)
{ 	
	unsigned char i=0,j=0,k=0,a=0,Bit=0x80;
	Hc595SendByte(0x00);
	init();
	UsartInit();
   	while(1)
   	{	
		/*if (num==20)
		{
			k=a;
			for (j=0;j<8;j++)
			{
				Data[j]=Data1[k%8];
				k++;
			}
			if (a==16)
				a=0;
			a++;
			
			num=0;	
		} */

		P0=Data1[i];
		Hc595SendByte(Bit);
		Bit=Bit>>1;
		i++;
		if (i==8)
		{
			Bit=0x80;
			i=0;
		}
		Hc595SendByte(0x00);
				
   	}
}
void UsartInit()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}

unsigned char str_pos=0;
void Usart() interrupt 4
{
	unsigned char receiveData;
	str_pos=str_pos%8;
	receiveData=SBUF;//��ȥ���յ�������
	Data1[str_pos]=receiveData;
	
	RI = 0;//��������жϱ�־λ
	SBUF=receiveData;//�����յ������ݷ��뵽���ͼĴ���
	
	while(!TI);			 //�ȴ������������
	TI=0;						 //���������ɱ�־λ
	str_pos++;
}

void init()
{
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
}

void T0_time() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	num++;
}