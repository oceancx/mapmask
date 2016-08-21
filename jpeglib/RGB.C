/*
 *	Cloud Wu's JPEG decoder
 *
 *			2000/3/4 第 1 版
 *
 *		允许非赢利性质的自由使用, 但如果使用此代码的全部或部分
 *		请署上 Cloud Wu (云风)
 *
 *		商业使用请向作者直接联系
 *
 *		www.codingnow.com
 *		cloudwu@263.net
 *	
 *		YCbCr->RGB16 的转换, 如果需要处理真彩,需要另外写
 */

#include "jpegint.h"
#include <stdio.h>

extern short jpeg_ybuf[256];
extern short jpeg_cbbuf[64];
extern short jpeg_crbuf[64];

static WORD jpeg_Y[64];
static signed char Cr1_402[256],Cb0_34414[256],Cr0_71414[256],Cb1_772[256];
static __int64 x8080808080808080=0x8080808080808080;
static __int64 x7f7f7f7f7f7f7f7f=0x7f7f7f7f7f7f7f7f;
static __int64 x03e003e003e003e0=0x03e003e003e003e0;
void (*YCbCr411)(WORD *bmppixel,int pitch);
void YCbCr411_565(WORD *bmppixel,int pitch);
void YCbCr411_555(WORD *bmppixel,int pitch);
void YCbCr411_nommx(WORD *bmppixel,int pitch);

static WORD (*RGB16)(signed char Y,signed char r,signed char g,signed char b);
WORD RGB16_555(signed char Y,signed char r,signed char g,signed char b);
WORD RGB16_565(signed char Y,signed char r,signed char g,signed char b);
void (*jpeg_idct) (short *buf);

__int64 mmx_mask_red, mmx_mask_green;

void jpeg_setcolormode(int is555,int isMMX)
{
	int i;
	if (is555) {
		YCbCr411=YCbCr411_555;
		RGB16=RGB16_555;
		mmx_mask_red=0xfc00fc00fc00fc00;
		mmx_mask_green=0x03e003e003e003e0;
		for (i=0;i<32;i++)
			jpeg_Y[i*2]=jpeg_Y[i*2+1]=i<<10|i<<5|i;
	}
	else {
		YCbCr411=YCbCr411_565;
		RGB16=RGB16_565;
		mmx_mask_red=0xf800f800f800f800;
		mmx_mask_green=0x07e007e007e007e0;
		for (i=0;i<64;i++)
			jpeg_Y[i]=(i>>1)<<11|i<<5|(i>>1);
	}
	for (i=0;i<256;i++) {
		Cr1_402[i]=(int)(1.402*(signed char)i+1)>>1;
		Cb0_34414[i]=(int)(-0.34414*(signed char)i+1)>>1;
		Cr0_71414[i]=(int)(-0.71414*(signed char)i+1)>>1;
		Cb1_772[i]=(int)(1.772*(signed char)i+1)>>1;
	}
	if (!isMMX) {
		YCbCr411=YCbCr411_nommx;
		jpeg_idct=jpeg_idct2d;
	}
	else jpeg_idct=jpeg_idct8x8aan;
}

void Y2RGB(WORD *bmppixel,int pitch)
{
	__asm {
		xor eax,eax;
		mov ebx,pitch;
		lea esi,jpeg_ybuf;
		sub ebx,16;
		mov edx,8;
		mov edi,bmppixel;
		mov ecx,edx;
		ALIGN 4
_loop:
		lodsb;
		add al,0x80;
		movzx eax,al;
		shr eax,2;
		mov ax,[jpeg_Y+eax*2];
		stosw;
		dec ecx;
		jnz _loop;
		mov ecx,8;
		add edi,ebx;
		dec edx;
		jnz _loop;
	}
}

/*
R = Y                    + 1.402  *(Cr-128)
G = Y - 0.34414*(Cb-128) - 0.71414*(Cr-128)
B = Y + 1.772  *(Cb-128)
*/

void YCbCr411_565(WORD *bmppixel,int pitch)
{
	
	
}

void YCbCr411_555(WORD *bmppixel,int pitch)
{
	
}
int inc=0;
WORD RGB16_555(signed char Y,signed char r,signed char g,signed char b)
{	
	
	unsigned short result=0;
		inc++;
		if(inc<10){
			printf("\n Y:0x%8x r:0x%8x g:0x%8x b:0x%8x result:0x%x\n",(signed char)Y,(signed char)r,(signed char)g,(signed char)b,result);
			printf("\n Y:%d r:%d g:%d b:%d result:%d\n",(signed char)Y,(signed char)r,(signed char)g,(signed char)b,result);
		}
		
	__asm {
		movsx dx,byte ptr Y;
		movsx ax,byte ptr r;
		add dx,128;
		movsx bx,byte ptr g;
		sar dx,1;
		movsx cx,byte ptr b;
		add ax,dx;
		jns _ax_n0;
		xor ax,ax;
_ax_n0:
		test ax,0xff80;
		jz _ax_7f;
		mov ax,0x7f;
_ax_7f:
		add bx,dx;
		jns _bx_n0;
		xor bx,bx;
_bx_n0:
		test bx,0xff80;
		jz _bx_7f;
		mov bx,0x7f;
_bx_7f:
		add cx,dx;
		jns _cx_n0;
		xor cx,cx;
_cx_n0:
		test cx,0xff80;
		jz _cx_7f;
		mov cx,0x7f;
_cx_7f:
		shl ax,8;
		shl bx,3;
		and ax,0x7c00;
		and bx,0x3e0;
		shr cx,2;
		or ax,bx;
		or ax,cx;
		mov result,ax;
		}
	if(inc<10){
			printf("Y:0x%8x r:0x%8x g:0x%8x b:0x%8x result:0x%x\n",(signed char)Y,(signed char)r,(signed char)g,(signed char)b,result);
			printf("Y:%d r:%d g:%d b:%d result:%d\n",(signed char)Y,(signed char)r,(signed char)g,(signed char)b,result);
	}
	return result;
}


WORD RGB16_565(signed char Y,signed char r,signed char g,signed char b)
{
	WORD result=0;
	
	return result;
}

// C 语言版
void YCbCr411_nommx(WORD *bmppixel,int pitch)
{
	int i,j,Cb,Cr,m;
	signed char r,g,b;
	signed char *CrBuf=(signed char *)jpeg_crbuf;
	signed char *CbBuf=(signed char *)jpeg_cbbuf;
	signed char *YBuf=(signed char *)jpeg_ybuf;
	for (m=0;m<4;++m,bmppixel-=(m&1)*pitch*4-8+16*(m==2))
	for (i=0;i<8;i+=2) {
		for (j=0;j<8;j+=2) {
			Cb=CbBuf[(i/2+4*(m/2))*8+j/2+4*(m&1)];
			Cr=CrBuf[(i/2+4*(m/2))*8+j/2+4*(m&1)];

			r=Cr1_402[(BYTE)Cr];
			g=Cb0_34414[(BYTE)Cb]+Cr0_71414[(BYTE)Cr];
			b=Cb1_772[(BYTE)Cb];

			*bmppixel=RGB16(YBuf[i*8+j+128*m],r,g,b),++bmppixel;
			*bmppixel=RGB16(YBuf[i*8+j+1+128*m],r,g,b),++bmppixel;
			*(bmppixel+pitch/2-2)=RGB16(YBuf[i*8+j+8+128*m],r,g,b);
			*(bmppixel+pitch/2-1)=RGB16(YBuf[i*8+j+9+128*m],r,g,b);
		}
		bmppixel=(unsigned short*)((unsigned)bmppixel+pitch*2-16);
	}

}


void YCbCr111(WORD *bmppixel,int pitch)
{
	int i,j,Cb,Cr;
	signed char *CrBuf=(signed char *)jpeg_crbuf;
	signed char *CbBuf=(signed char *)jpeg_cbbuf;
	signed char *YBuf=(signed char *)jpeg_ybuf;
	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			Cb=CbBuf[i*8+j];
			Cr=CrBuf[i*8+j];

			*bmppixel=RGB16(YBuf[i*8+j],
				Cr1_402[(BYTE)Cr],
				(signed char)(Cb0_34414[(BYTE)Cb]+Cr0_71414[(BYTE)Cr]),
				Cb1_772[(BYTE)Cb]),++bmppixel;
		}
		bmppixel=(unsigned short*)((unsigned)bmppixel+pitch-16);
	}
}
