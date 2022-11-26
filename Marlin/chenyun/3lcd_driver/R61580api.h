#ifndef R61580API_H
#define R61580API_H
#ifdef TFT_USE_R61580
/*******************************************************************************
* ���ļ����Ӳ�������ļ�ʹ��
* �������RTFT_WCMD,RTFT_WDAT,RTFT_RDAT����ĺ���
* ��Ϊ������������궨����Ӳ�������й�
* ���ߣ�YSZ
* ���ڣ�2017-05-16
*******************************************************************************/
#define						TFT_LMAX									320
#define						TFT_WMAX									240

extern unsigned short R61580_dirVar;
void R61580_Init(unsigned char dir);
void R61580_SetDir(unsigned char dir);
u16 R61580_ReadId();
void R61580_Workplace(const int& xStart,const int& yStart,const int& xEnd,const int& yEnd);
void R61580_Setpos(const int& x,const int& y);//������Ļ�����λ��
inline void R61580_ReadPixel(const int& x,const int& y,COLORREF& R,COLORREF& G,COLORREF& B)
{
	RTFT_WCMD(0x4F);
	RTFT_WDAT(x);
	RTFT_WCMD(0x4E);
	RTFT_WDAT(y);
	RTFT_WCMD(0x22);//��ȡ��������
	B=RTFT_RREG();//��ȡ����
	G=(B>>5)&0x3F;
	R=B>>11;
	B=(B>>3)&0x1F;
}

inline void R61580_Pixel(const int& x,const int& y,const COLORREF& color)
{
	R61580_Workplace(x,y,x,y);
	R61580_Setpos(x,y);
	RTFT_WCOLOR(color);
}

//���ײ㺯��������Ϊ���нӿں���,����ֱ��д���нӿں���,��Ϊһ�����̿��ܻ��кܶ�TFT���ײ�ӿ�,������ù��нӿ�,������,���ҵ��û����
#define RTFT_INIT						R61580_Init
#define	RTFT_SETDIR					R61580_SetDir
#define	RTFT_SETPOS					R61580_Setpos
#define	RTFT_WORKPLACE			R61580_Workplace
#define	RTFT_PIXEL					R61580_Pixel
#define	RTFT_READPIXEL			R61580_ReadPixel
#define	RTFT_DIRVAR					R61580_dirVar
#define	RTFT_READID					R61580_ReadId
#endif //#ifdef TFT_USE_R61580
#endif
