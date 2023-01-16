/* Michael Adams' PRIMARY.H programming "library." */

/* Designed for use with Borland C++ 3.1, but maybe compatable with other compilers.*/

/* This "library" uses C & C++ routines found or converted from... */
/*  1. "The Game Programmer's Encyclopedia" */
/*  2. "THE IBM PC PROGRAMMER'S GUIDE TO C"; 3rd Edition; Matthew Probert */
/*  3. "256-color VGA Programming in C" website; David Brackeen */
/*  4. The VBE 3.0 standard; Video Electronic Standards Association */
/*  5. Abandoned "Outbreak" project; Pietro Mokrosh */
/*  6. Personally coded material appropriately marked as "coded by me." */

/* Note: All references to VESA/SVGA material is labeled as SVGA. */

#if !defined(__PRIMARY_H)
#define __PRIMARY_H

/* Special declares: these declare the screen resolution */

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/* Includes */

#include <ALLOC.H>                      /* Used for wav-playing */
#include <conio.h>			/* Standard use */
#include <stdio.h>			/* Standard use */
#include <dos.h>			/* Standard use */
#include <stdlib.h>			/* Standard use */
#include <mem.h>			/* Used for memory pointers? */
#include <string.h>			/* Used for string conversions */
#include <time.h>			/* Used for timestamp routine */
#include <bios.h>			/* Used for bios keyboard-press loop in "Notes" */
#include <ctype.h>			/* Used for bios keyboard-press loop in "Notes" */
#include <math.h>			/* Used for math functions*/
#include <fstream.h>    /* C++ only; applies to IOSTREAM & FSTREAM functions */
#include <process.h>
#include <dir.h>
#include <io.h>


/* Defines */
#define DIRECT_BANKING		/* Used for SVGA */
#undef peek
#undef peekb

  /* SVGA */
#ifdef DIRECT_BANKING
/* only needed to setup registers BX,DX prior to the direct call.. */
extern far setbxdx(int,int);
#endif

/* Assembler-compatable & other variable defines */

typedef unsigned char byte;	 /* Defines "byte" variable type */
typedef unsigned short word;     /* Defines "word" variable type */
typedef unsigned long  dword;    /* Defines "dword" variable type */
typedef unsigned char uchar;     /* Defines "uchar" variable type */
typedef unsigned long ulong;
typedef void far *lpvoid;

enum BOOL {FALSE,TRUE};

/* Critical pointers */
byte *VGA=(byte*)0xA0000000L;     /* VGA memory pointer */
word *my_clock=(word*)0x0000046C; /* this points to the 18.2hz system clock */

/* macro to skip bytes in a file */
#define fskip(fp,n)   \
{                     \
   int i;             \
		      \
   for (i=0;i<n;i++)  \
      fgetc(fp);      \
}

/* Structures */

  /* mouse cursors (name = shape) */
const dword arrow[16][8]=
{
 { 15,  0,  0,  0,  0,  0,  0,  0},
 { 15, 15,  0,  0,  0,  0,  0,  0},
 { 15, 15, 15,  0,  0,  0,  0,  0},
 { 15, 15, 15, 15,  0,  0,  0,  0},
 { 15, 15, 15, 15, 15,  0,  0,  0},
 { 15, 15, 15, 15, 15, 15,  0,  0},
 { 15, 15, 15, 15, 15, 15, 15,  0},
 {  0,  0, 15, 15,  0,  0,  0,  0},
 {  0,  0, 15, 15,  0,  0,  0,  0},
 {  0,  0,  0, 15, 15,  0,  0,  0},
 {  0,  0,  0, 15, 15,  0,  0,  0},
 {  0,  0,  0,  0,  0,  0,  0,  0},
 {  0,  0,  0,  0,  0,  0,  0,  0},
 {  0,  0,  0,  0,  0,  0,  0,  0},
 {  0,  0,  0,  0,  0,  0,  0,  0},
 {  0,  0,  0,  0,  0,  0,  0,  0}
};

const dword hourglass[16][8]=
{
 {  6,  6,  6,  6,  6,  6,  6,  6},
 {  6, 15, 15, 15, 15, 15, 15,  6},
 {  6,  6, 15, 15, 15, 15,  6,  6},
 {  6,  0,  6, 15, 15,  6,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  0,  6,  6,  0,  0,  6},
 {  6,  0,  6, 15, 15,  6,  0,  6},
 {  6,  6, 15, 15, 15, 15,  6,  6},
 {  6, 15, 15, 15, 15, 15, 15,  6},
 {  6,  6,  6,  6,  6,  6,  6,  6}
};

  /* Use this array to dump screen area into; employ getvesapixel to do this */
dword precursor[16][8];

  /* SuperVGA information block */

struct
{
   char VESASignature[4];			/* 'VESA' 4 byte signature */
   short VESAVersion;				/* VBE version number */
   char far *OEMStringPtr;			/* Pointer to OEM string */
   long Capabilites;					/* Capabilites of video card */
   unsigned far *VideoModePtr;	/* Pointer to supported modes */
   short TotalMemory;				/* Number of 64kb memory blocks */
   char reserved[236];				/* Pad to 256 byte block size */
} VbeInfoBlock;

  /* SuperVGA mode information block */

struct
{
   unsigned short ModeAttributes;	/* Mode attributes */
   unsigned char WinAAttributes;		/* Window A attributes */
   unsigned char WinBAttributes;		/* Window B attributes */
   unsigned short WinGranularity;	/* Window granularity in k */
   unsigned short WinSize;				/* Window size in k */
   unsigned short WinASegment;		/* Window A segment */
   unsigned short WinBSegment;		/* Window B segment */
   void (far *WinFuncPtr)(void);		/* Pointer to window function */
   unsigned short BytesPerScanLine;	/* Bytes per scanline */
   unsigned short XResolution;		/* Horizontal resolution */
   unsigned short YResolution;		/* Vertical resolution */
   unsigned char XCharSize;			/* Character cell width */
   unsigned char YCharSize;			/* Character cell height */
   unsigned char NumberOfPlanes;		/* Number of memory planes */
   unsigned char BitsPerPixel;		/* Bits per pixel */
   unsigned char NumberOfBanks;		/* Number of CGA style banks */
   unsigned char MemoryModel;			/* Memory model type */
   unsigned char BankSize;				/* Size of CGA style banks */
   unsigned char NumberOfImagePages; /* Number of images pages */
   unsigned char resl;					/* Reserved */
   unsigned char RedMaskSize;			/* Size of direct color red mask */
   unsigned char RedFieldPosition;	/* Bit posn of lsb of red mask */
   unsigned char GreenMaskSize;		/* Size of direct color green mask */
   unsigned char GreenFieldPosition;/* Bit posn of lsb of green mask */
   unsigned char BlueMaskSize;		/* Size of direct color blue mask */
   unsigned char BlueFieldPosition; /* Bit posn of lsb of blue mask */
   unsigned char RsvdMaskSize;      /* Size of direct color res mask */
   unsigned char RsvdFieldPosition; /* Bit posn of lsb of res mask */
   unsigned char DirectColorModeInfo; /* Direct color mode attributes */
   unsigned char res2[216];			/* Pad to 256 byte block size */
} ModeInfoBlock;

/* SVGA Memory Models */

typedef enum
{
   memPL  = 3, /* Planar memory model */
   memPK  = 4, /* Packed pixel memory model */
   memRGB = 6, /* Direct color RGB memory model */
   memYUV = 7, /* Direct color YUV memory model */
} memModels;

/* Global Variables */

int horz, oldhorz, temphorz;     /* Mouse horizontal locations */
int clickhorz, clickvert;        /* Mouse button click coordinates */
int vert, oldvert, tempvert;     /* Mouse vertical locations */
dword tempcolor;                 /* Mouse/Screen pixel data */
unsigned char vermaj, vermin;    /* Used for Mouse version info */
BOOL button1, button2;           /* Used for Mouse button detection */
int hmpr, vmpr;			 /* Used with Mouse function 11 & mousealign */
char mystr[256];                 /* Used by SVGA routines */
char *get_str();                 /* Used by SVGA routines */
int xres,yres;                   /* Resolution of video mode used  (SVGA)*/
int bytesperline;                /* Logical CRT scanline length (SVGA)*/
int curBank;                     /* Current read/write bank (SVGA)*/
unsigned int bankShift;          /* Bank granularity adjust factor (SVGA)*/
int oldMode;                     /* Old video mode number (SVGA)*/
char far *screenPtr;             /* Pointer to start of video memory (SVGA)*/
void (far *bankSwitch)(void);    /* Direct bank switching function (SVGA)*/

/* Standard Display Functions (Non SVGA) */

	/* Set Video Mode */
   /* Ex: set_mode(0x13); sets video mode to mode 13h */
void set_mode(byte mode)
{
 asm{
 mov ah, 0x00
 mov al, mode
 int 0x10
 }
}

	/* Plot a pixel */
   /* Ex: plot_pixel(5,5,10); plots a pixel at (5,5) in color # 10 */
void plot_pixel(int x,int y, byte color)
{
 asm{
 mov ah,0x0C
 mov al,color
 mov cx,x
 mov dx,y
 int 0x10
 }
}

	/* Set background color */
   /* Ex: background(10); sets background color to #10 */
void background(unsigned char color)
{
 asm{
 mov ah,11
 mov bh,0
 mov bl,color
 int 0x10
 }
}


/* SVGA display routines (these can be found in the VBE 3.0 standard) */

	/* Get SVGA info., returning true if VBE found*/
int getVbeInfo(void)
{
 union REGS regs;
 struct SREGS segs;
 char far *VbeInfo = (char far *)&VbeInfoBlock;
 regs.x.ax = 0x4F00;
 regs.x.di = FP_OFF(VbeInfo);
 segs.es = FP_SEG(VbeInfo);
 int86x(0x10, &regs, &regs, &segs);
 return (regs.x.ax == 0x4F);
}

	/* Get video mode information, given a VBE mode number (0x100, etc.)*/
   /* Return 0 if the mode is not available, or if it is not a 256 color */
   /* packed-pixel mode. */
int getModeInfo(int mode)
{
 union REGS regs;
 struct SREGS segs;
 char far *modeInfo = (char far *)&ModeInfoBlock;
 if (mode < 0x100) return 0; /* Ignore non-VBE modes */
 regs.x.ax = 0x4F01;
 regs.x.cx = mode;
 regs.x.di = FP_OFF(modeInfo);
 segs.es = FP_SEG(modeInfo);
 int86x(0x10, &regs, &regs, &segs);
 if (regs.x.ax != 0x4F) return 0;
 if ((ModeInfoBlock.ModeAttributes & 0x1)
		&& ModeInfoBlock.MemoryModel == memPK
	&& ModeInfoBlock.BitsPerPixel == 8
	&& ModeInfoBlock.NumberOfPlanes == 1)
   return 1;
 return 0;
}

	/* Set SVGA graphics mode */
void setVBEMode(int mode)
{
 asm{
 mov ax,0x4F02
 mov bx,mode
 int 0x10
 }
}

	/* Get current SVGA graphics mode */
int getVBEMode(void)
{
 word tempword;
 asm{
 mov ax,0x4F03
 int 0x10
 mov tempword,bx
 }
 return tempword;
}

	/* Set new read/write bank. We must set both Window A and Window B, as */
   /* many VBE's have these set as separately available read and write */
   /* windows. We also use a simple (but very effective) optimization of */
   /* checking if the requested bank is currently active. */
void setBank(int bank)
{
 if(bank == curBank) return;	/* Bank already active */
 curBank = bank;                /* Save current bank number */
 bank <<= bankShift;            /* Adjust to window granularity */
  #if def DIRECT_BANKING
   set bxdx(0,bank);
   bankSwitch();
   setbxdx(1,bank);
   bankSwitch();
  #else
  asm{
  mov ax,0x4F05
  mov bx,0
  mov dx,bank
  int 0x10
  mov ax,0x4F05
  mov bx,1
  mov dx,bank
  int 0x10
  }
  #endif
}

	/* Plot a pixel in SVGA mode */
void vesapixel(int x, int y, dword color)
{
 dword addr = (dword)y * bytesperline + x;
 setBank((int)(addr >> 16));
 *(screenPtr + (addr & 0xFFFF)) = color;
}
	/* Get a pixel in SVGA mode */
dword getvesapixel(int x, int y)
{
 dword oldcolor;
 dword addr = (dword)y * bytesperline + x;
 setBank((int)(addr >> 16));
 oldcolor = *(screenPtr + (addr & 0xFFFF));
return oldcolor;
}

	/* Return NEAR pointer to FAR string pointer */
char *get_str(char far *p)
{
 int i;
 char *q=mystr;
  for(i=0;i<255;i++)
  {
   if(*p) *q++ = *p++;
   else break;
  }
 *q = '\0';
 return(mystr);
}

   /* List available SVGA modes (Not useable if program is in protected mode)*/
void availableModes(void)
{
 unsigned far *p;
 if (!getVbeInfo())
 {
  printf("No VESA VBE detected\n");
  exit(1);
 }
 printf("VESA VBE Version %d.%d detected (%s)\n\n",
  VbeInfoBlock.VESAVersion >> 8, VbeInfoBlock.VESAVersion & 0xF,
  get_str(VbeInfoBlock.OEMStringPtr));
 printf("Available 256 color video modes:\n");
  for (p = VbeInfoBlock.VideoModePtr; *p !=(unsigned)-1;p++)
  {
   if(getModeInfo(*p))
   {
    printf("	%4d x %4d %d bits per pixel\n",
	ModeInfoBlock.XResolution, ModeInfoBlock.YResolution,
      ModeInfoBlock.BitsPerPixel);
   }
  }
  printf("nUsage: hellovbe <xres><yres>\n");
  exit(1);
}

   /* Employ a "shift-factor," instead of multiplying, to activate SVGA mode */
void initGraphics(unsigned int x, unsigned int y)
{
 unsigned far *p;
  if(!getVbeInfo())
  {
   printf("No VESA VBE detected\n");
   exit(1);
  }
 for (p = VbeInfoBlock.VideoModePtr; *p != (unsigned)-1; p++)
  {
   if (getModeInfo(*p) && ModeInfoBlock.XResolution == x
		&& ModeInfoBlock.YResolution == y)
   {
    xres = x; yres = y;
    bytesperline = ModeInfoBlock.BytesPerScanLine;
    bankShift = 0;
    while ((unsigned)(64 >> bankShift) != ModeInfoBlock.WinGranularity)
     bankShift++;
   bankSwitch = ModeInfoBlock.WinFuncPtr;
    curBank = -1;
    screenPtr = (char far *)(( (dword)0xA000)<<(16 | 0));
    oldMode = getVBEMode();
    setVBEMode(*p);
    return;
   }
  }
 printf("Valid video mode not found\n");
 exit(1);
}

/* "Vesatext" string plotting functions */
 /* Use these to display strings & text in VESA & other graphics modes. */
 /* Warning: may only work with C++ compilers! */

	/* 8x8 font */
void vesatext8(int x, int y, char *inchar, dword c)
{
 word a,b,d;
 int h,i,j,length,ch;
 length = strlen(inchar);
 for(h = 0;h < (length+1);h++)
 {
 ch = inchar[h];
  for(i = 0;i < 8;i++)
  {
   for(j = 7;j > -1;j--)
   {
   b = 0x000E + (ch * 8) + i;
   a = peek(0xFFA6,b);
   d = pow(2,j);
   if((d & a) != 0) vesapixel((x-j)+8,y+i,c);
   }
  }
 x += 8;
 }
}


/* String Functions */

     void truncstr(char *p,int num)
     {
	  /* Truncate string by losing last num characters */
	  if (num < strlen(p))
	       p[strlen(p) - num] = 0;
     }

     void trim(char *text)
     {
	  /* remove trailing spaces */
	  char *p;

	  p = &text[strlen(text) - 1];
          while(*p == 32 && p >= text)
	       *p-- = 0;
     }

     void strlench(char *p,int num)
     {
	  /* Change length of string by adding or deleting characters */

	  if (num > 0)
	       memmove(p + num,p,strlen(p) + 1);
	  else
	  {
	       num = 0 - num;
	       memmove(p,p + num,strlen(p) + 1);
	  }
     }

/*From NY2008 BBS game source*/
/*copy end chars  beginning from beg to dest*/
/*similiar to strncpy*/
void strzcpy(char dest[],const char src[], int beg,int end)
{
  int cnt=0;
  do
    dest[cnt++]=src[beg++];
  while (cnt<=end && src[cnt]!=0);
  dest[cnt]='\0';
}

/* Prompt to press ENTER (Non SVGA) */
 /* Example: askme(10,10); displays the prompt at text coord. (10,10) */

void askme(int amx, int amy)
{
 textcolor(7);
 gotoxy(amx,amy);
 printf("Press ENTER to continue");
 getch();
}

/* Display a standard timestamp; uses C++ output format */
 /* Example: telltime(10,10); displays the timestamp at text coord. (10,10) */

void telltime(int x, int y)
{
  time_t now;
  char date[30];
  now = time(NULL);
  strcpy(date,ctime(&now));
  gotoxy(x,y); printf(date,"\n");
  return;
}


/* Mouse control (All were directly coded by me (Michael Adams) via the */
/* official Microsoft Mouse programming guide of 1994. Check Microsoft's */
/* website on different mouse functions. */

  /* Reset Mouse */
void mousereset(void)
{
 asm{
 mov ax,0x00
 int 0x33
 }
return;
}

  /* "Home" Mouse (Sets horz,vert to 0,0) */
void mousehome(void)
{
 asm{
 mov ax,0x04
 mov cx,0    /* horz cursor coordinate = 0 */
 mov dx,0    /* vert cursor coordinate = 0 */
 int 0x33
 }

 horz = 0; vert = 0; /* Reset mouse location variables */

 return;
}

  /* Set Horizontal Mouse limits */
void mousehorzrange(word minlimit, word maxlimit)
{
 asm{
 mov ax,0x07
 mov cx,minlimit
 mov dx,maxlimit
 int 0x33
 }
return;
}

  /* Set Vertical Mouse limits */
void mousevertrange(word minlimit, word maxlimit)
{
 asm{
 mov ax,0x08
 mov cx,minlimit
 mov dx,maxlimit
 int 0x33
 }
return;
}

  /* Turn Mouse cursor on */
void mouseon(void)
{
 asm{
 mov ax,0x01
 int 0x33
 }
return;
}

  /* Turn Mouse cursor off */
void mouseoff(void)
{
 asm{
 mov ax,0x02
 int 0x33
 }
return;
}

  /* Get Mouse Driver Version */
void mousedriverinfo(void)
{
 asm{
 mov ax,0x24
 int 0x33
 mov vermaj,bh
 mov vermin,bl
 }
 printf("Mouse driver version: ");
 printf("%u.%u\n", vermaj, vermin);
 return;
}

  /* Load Mouse info into useable global variables */
void mousestatus()
{
 asm{
 mov ax,0x03
 int 0x33
 mov horz,cx
 mov vert,dx
 }
}

void leftmousecheck()
{
 asm{
 mov ax,0x05
 mov bx,0
 int 0x33
 mov button1,ax
 mov clickhorz,cx
 mov clickvert,dx
 }
}

void rightmousecheck()
{
 asm{
 mov ax,0x05
 mov bx,1
 int 0x33
 mov button2,ax
 mov clickhorz,cx
 mov clickvert,dx
 }
}

	/* Align mouse to new mickey-to-screen pixel ratios */
	 /* Baseline: 8 mprh (Horizontal), 16 mprv (Vertical)*/
void mousealign(int mprh, int mprv)
{
 asm{
 mov ax, 15
 mov cx, mprh
 mov dx, mprv
 int 0x33
 }
 hmpr = mprh; /* mprh & mprv can be used as dividers when calling function*/
 vmpr = mprv; /* 11 to determine mouse movement in mickeys.*/
 return;
}

   /* Create a mouse cursor in SVGA mode (see "Additional Tips") */
void vesamousestart(int cursor)
{
 mousestatus();

  /* Copy old screen */
  for(tempvert = 0; tempvert < 16; tempvert++)
  {
   for(temphorz = 0; temphorz < 8; temphorz++)
   {
   precursor[tempvert][temphorz] = getvesapixel(horz+temphorz,vert+tempvert);
   }
  }

  /* Plot mouse */

   /* Arrow */
  if(cursor==1){
  for(tempvert = 0; tempvert < 16; tempvert++)
  {
   for(temphorz = 0; temphorz < 8; temphorz++)
   {
   tempcolor = arrow[tempvert][temphorz];
   if(tempcolor > 0)vesapixel(horz+temphorz,vert+tempvert,tempcolor);
   }
  }}

   /* Hourglass */
  if(cursor==2){
  for(tempvert = 0; tempvert < 16; tempvert++)
  {
   for(temphorz = 0; temphorz < 8; temphorz++)
   {
   tempcolor = hourglass[tempvert][temphorz];
   if(tempcolor > 0)vesapixel(horz+temphorz,vert+tempvert,tempcolor);
   }
  }}

 /* Update old mouse location */
 oldhorz = horz; oldvert = vert;

 return;
}

   /* Used along with vesamousestart (see "Additional Tips") */
void vesamousecursor(int oldcursor)
{
 mousestatus();

 if((horz != oldhorz) || (vert != oldvert))
 {

  /* Plot old screen */
  for(tempvert = 0; tempvert < 16; tempvert++)
  {
   for(temphorz = 0; temphorz < 8; temphorz++)
   {
   tempcolor = precursor[tempvert][temphorz];
   vesapixel(oldhorz+temphorz,oldvert+tempvert,tempcolor);
   }
  }

 vesamousestart(oldcursor); /* Recreate mouse */
 }

 return;
}

/* Misc. Graphics Functions */

   /* SVGA clrscr */
void vesaclrscr()
{
 int tx,ty;
  for(tx=0;tx<SCREEN_WIDTH;tx++)
   for(ty=0;ty<SCREEN_HEIGHT;ty++)
   {
   vesapixel(tx,ty,0);
   }
 return;
}

   /* Line */
void bline(int x, int y, int x2, int y2, word c)
{
      int i, steep = 0, sx, sy, dx, dy, e;
      dx = abs(x2 - x);
      sx = ((x2 - x) > 0) ? 1 : -1;
      dy = abs(y2 - y);
      sy = ((y2 - y) > 0) ? 1 : -1;
      if(dy > dx)
      {
	    steep =  x;   x =  y;   y = steep;  /* swap  x and  y */
	    steep = dx;  dx = dy;  dy = steep;  /* swap dx and dy */
	    steep = sx;  sx = sy;  sy = steep;  /* swap sx and sy */
	    steep = 1;
      }
      e = 2 * dy - dx;
      for(i = 0; i < dx; i++)
      {
	    if(steep)
		  vesapixel(y, x, c);
	    else  vesapixel(x, y, c);
	    while(e >= 0)
	    {
		  y += sy;
		  e -= 2 * dx;
	    }
	    x += sx;
	    e += 2 * dy;
      }
      vesapixel(x2, y2, c);
 return;
}

void blineold(int x, int y, int x2, int y2, word c)
{
      int i, steep = 0, sx, sy, dx, dy, e;
      dx = abs(x2 - x);
      sx = ((x2 - x) > 0) ? 1 : -1;
      dy = abs(y2 - y);
      sy = ((y2 - y) > 0) ? 1 : -1;
      if(dy > dx)
      {
	    steep =  x;   x =  y;   y = steep;  /* swap  x and  y */
	    steep = dx;  dx = dy;  dy = steep;  /* swap dx and dy */
	    steep = sx;  sx = sy;  sy = steep;  /* swap sx and sy */
	    steep = 1;
      }
      e = 2 * dy - dx;
      for(i = 0; i < dx; i++)
      {
	    if(steep)
		  vesapixel(y, x, c);
	    else  vesapixel(x, y, c);
	    while(e >= 0)
	    {
		  y += sy;
		  e -= 2 * dx;
	    }
	    x += sx;
	    e += 2 * dy;
      }
      plot_pixel(x2, y2, c);
 return;
}


   /* Circle */
void bcircle(int xc, int yc, int r, word c)
{
      int x = 0, d = 2 * (1 - r), w = 2 * SCREEN_WIDTH / SCREEN_HEIGHT;
      while(r >= 0)
      {
	    vesapixel(xc + x, yc + r, c);
	    vesapixel(xc + x, yc - r, c);
	    vesapixel(xc - x, yc + r, c);
	    vesapixel(xc - x, yc - r, c);
	    if (d + r > 0)
		  d -= (w * --r) - 1;
	    if (x > d)
		  d += (2 * ++x) + 1;
      }
 return;
}

void bcircleold(int xc, int yc, int r, word c)
{
      int x = 0, d = 2 * (1 - r), w = 2 * SCREEN_WIDTH / SCREEN_HEIGHT;
      while(r >= 0)
      {
	    plot_pixel(xc + x, yc + r, c);
	    plot_pixel(xc + x, yc - r, c);
	    plot_pixel(xc - x, yc + r, c);
	    plot_pixel(xc - x, yc - r, c);
	    if (d + r > 0)
		  d -= (w * --r) - 1;
	    if (x > d)
		  d += (2 * ++x) + 1;
      }
 return;
}

   /* Rectangle (coded by me) */
void rectangle(int rx,int ry,int rx2,int ry2,dword rc)
{
 bline(rx,ry,rx,ry2,rc);
 bline(rx,ry2,rx2,ry2,rc);
 bline(rx2,ry2,rx2,ry,rc);
 bline(rx2,ry,rx,ry,rc);
 return;
}

   /* Fill routine */
void bfill(int x, int y, dword col,int pattern)
{
	  /* Fill a boundered shape using a hatch pattern */

          int xa;
          int ya;
          int bn;
	  int byn;

	  int hatch[10][8] = {
		    {255,255,255,255,255,255,255,255},
		    {128,64,32,16,8,4,2,1},
		    {1,2,4,8,16,32,64,128},
		    {1,2,4,8,8,4,2,1},
		    {238,238,238,238,238,238,238,238},
		    {170,85,170,85,170,85,170,85},
		    {192,96,48,24,12,6,3,1},
		    {62,62,62,0,227,227,227,0},
		    {129,66,36,24,24,36,66,129},
		    {146,36,146,36,146,36,146,36}
		    };

	  /* Patterns for fill, each integer describes a row of dots */

          xa = x;
	  ya = y;  /* Save Origin */

	  if(getvesapixel(x,y))
               return;

          bn = 1;
          byn = 0;

	  do
	  {
               if (hatch[pattern][byn] != 0)
               {
                    /* If blank ignore */
		    do
		    {
                         if ((bn & hatch[pattern][byn]) == bn)
			 {
			 vesapixel(x,y,col);
			 }
			 x--;
                         bn <<= 1;
			 if (bn > 128)
                              bn = 1;
                    }
		    while(!getvesapixel(x,y) && (x > -1));
     
		    x = xa + 1;
                    bn = 128;

		    do
		    {
			 if ((bn & hatch[pattern][byn]) == bn)
			 {
			 vesapixel(x,y,col);
			 }
                         x++;
                         bn >>=1;
                         if (bn <1)
			      bn = 128;
		    }
		    while((!getvesapixel(x,y)) && (x <= SCREEN_WIDTH));
               }
	       x = xa;
	       y--;
               bn = 1;
	       byn++;
               if (byn > 7)
                    byn = 0;
     
          }
	  while(!getvesapixel(x,y) && ( y > -1));
     
	  /* Now travel downwards */
     
	  y = ya + 1;

          byn = 7;
	  bn = 1;
          do
          {
               /* Travel left */
               if (hatch[pattern][byn] !=0)
	       {
                    do
		    {
			 if ( (bn & hatch[pattern][byn]) == bn)
			 {
			 vesapixel(x,y,col);
			 }
     
                         x--;
			 bn <<= 1;
                         if (bn > 128)
			      bn = 1;
		    }
		    while(!getvesapixel(x,y) && (x > -1));
     
		    /* Back to x origin */
		    x = xa + 1 ;
		    bn = 128;

		    /* Travel right */
		    do
		    {
			 if ((bn & hatch[pattern][byn]) == bn)
			 {
			 vesapixel(x,y,col);
			 }
			 x++;
                         bn >>=1;
			 if (bn <1)
			      bn = 128;
                    }
		    while((!getvesapixel(x,y)) && (x <= SCREEN_WIDTH));
               }
               x = xa;
               bn = 1;
               y++;
	       byn--;
               if (byn < 0)
                    byn = 7;
          }
	  while((!getvesapixel(x,y)) && (y <= SCREEN_HEIGHT));

 return;
}

void bfillold(int x, int y, dword col,int pattern)
{
	  /* Fill a boundered shape using a hatch pattern */

	  int xa;
          int ya;
          int bn;
	  int byn;

	  int hatch[10][8] = {
		    {255,255,255,255,255,255,255,255},
		    {128,64,32,16,8,4,2,1},
		    {1,2,4,8,16,32,64,128},
		    {1,2,4,8,8,4,2,1},
		    {238,238,238,238,238,238,238,238},
		    {170,85,170,85,170,85,170,85},
		    {192,96,48,24,12,6,3,1},
		    {62,62,62,0,227,227,227,0},
		    {129,66,36,24,24,36,66,129},
		    {146,36,146,36,146,36,146,36}
		    };

	  /* Patterns for fill, each integer describes a row of dots */

          xa = x;
	  ya = y;  /* Save Origin */

	  if(getvesapixel(x,y))
               return;

          bn = 1;
          byn = 0;

	  do
          {
               if (hatch[pattern][byn] != 0)
               {
                    /* If blank ignore */
		    do
		    {
                         if ((bn & hatch[pattern][byn]) == bn)
			 {
			 plot_pixel(x,y,col);
			 }
			 x--;
                         bn <<= 1;
			 if (bn > 128)
                              bn = 1;
                    }
		    while(!getvesapixel(x,y) && (x > -1));
     
		    x = xa + 1;
                    bn = 128;

		    do
		    {
			 if ((bn & hatch[pattern][byn]) == bn)
			 {
			 plot_pixel(x,y,col);
			 }
                         x++;
                         bn >>=1;
                         if (bn <1)
			      bn = 128;
		    }
		    while((!getvesapixel(x,y)) && (x <= SCREEN_WIDTH));
               }
	       x = xa;
	       y--;
               bn = 1;
	       byn++;
               if (byn > 7)
		    byn = 0;
     
          }
	  while(!getvesapixel(x,y) && ( y > -1));
     
	  /* Now travel downwards */
     
	  y = ya + 1;

          byn = 7;
	  bn = 1;
          do
          {
               /* Travel left */
               if (hatch[pattern][byn] !=0)
	       {
                    do
		    {
			 if ( (bn & hatch[pattern][byn]) == bn)
			 {
			 plot_pixel(x,y,col);
			 }
     
                         x--;
			 bn <<= 1;
                         if (bn > 128)
                              bn = 1;
		    }
		    while(!getvesapixel(x,y) && (x > -1));
     
                    /* Back to x origin */
		    x = xa + 1 ;
		    bn = 128;

		    /* Travel right */
		    do
		    {
			 if ((bn & hatch[pattern][byn]) == bn)
			 {
			 plot_pixel(x,y,col);
			 }
			 x++;
                         bn >>=1;
			 if (bn <1)
			      bn = 128;
                    }
		    while((!getvesapixel(x,y)) && (x <= SCREEN_WIDTH));
               }
               x = xa;
               bn = 1;
               y++;
	       byn--;
               if (byn < 0)
                    byn = 7;
          }
	  while((!getvesapixel(x,y)) && (y <= SCREEN_HEIGHT));

 return;
}

/* Additional Tips                                                           */
/* *. This include may generate a warning. If so, ignore it.                 */
/* 1. If you are looking for a continuous loop for time-updating & so forth, */
/*   use "_bios_keybrd(_KEYBRD_READY)==0" in the while part of a do-while    */
/*   loop. It requires the use of the ctype.h & bios.h include files, already*/
/*   active in this document.                                                */
/*                                                                           */
/* 2. The following code sequence is useful for creating a mouse cursor      */
/*   in SVGA mode.                                                           */
/*                                                                           */
/*        vesamousestart(1);                                                 */
/*        do{                                                                */
/*        vesamousecursor(1);                                                */
/*        }while(_bios_keybrd(_KEYBRD_READY)==0);                            */


#endif  /* __PRIMARY_H */