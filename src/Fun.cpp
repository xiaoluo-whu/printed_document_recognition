//---------------------------------------------------------------------------


#pragma hdrstop

#include "Fun.h"
#include "math.h"
#include <Math.hpp>
#include "stdlib.h"
#include <complex>
#include <windows.h>
#include <complex.h>
#include <system.hpp>
#include <SysUtils.hpp>
#include <Graphics.hpp>
#include <vcl.h>
#include <complex>

using namespace std; ///?????

//---------------------------------------------------------------------------

#pragma package(smart_init)

void BmpToMatrixGray(Graphics::TBitmap * temp,int **Matrix)
{
   int w=temp->Width;
   int h=temp->Height;
   temp->PixelFormat=pf24bit;
   for(int i=0;i<h;i++)
   {
      Byte * ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<w*3;j+=3)
      {
         Matrix[i][j/3]=Byte(ptr[j]*0.11+ptr[j+1]*0.59+ptr[j+2]*0.3+0.5);
      }
   }
}

void MatrixToDih(int **Matrix,int w,int h,int * dih,int length)
{
        for(int i=0;i<length;i++)
        {
                dih[i]=0;
        }
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<w;j++)
                {
                        if (Matrix[i][j] != 255)
                                dih[Matrix[i][j]]++;
                }
        }
}

void Segbmp(Graphics::TBitmap * temp,int trd)
{
   int w=temp->Width;
   int h=temp->Height;
   temp->PixelFormat=pf24bit;
   for(int i=0;i<h;i++)
   {
      Byte * ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<w*3;j+=3)
      {
         if((ptr[j]*0.11+ptr[j+1]*0.59+ptr[j+2]*0.3)>trd)
         {
                ptr[j]=ptr[j+1]=ptr[j+2]=255;
         }
         else
         {
                ptr[j]=ptr[j+1]=ptr[j+2]=0;
         }
      }
   }
}

int IterativeSeg(int * Dih,int length)
{
        int t1=0;
        int t2=0;
        int min=0;
        int max=0;
        int countall=0;
        int histall=0;
        int count=0;
        for(int i=0;i<length;i++)
        {
                countall=countall+Dih[i];
                histall=histall+i*Dih[i];
        }
        for(int i=0;i<length;i++)
        {
                if(Dih[i]!=0)
                {
                        min=i;
                        break;
                }
        }
        for(int i=length-1;i>=0;i--)
        {
                if(Dih[i]!=0)
                {
                        max=i;
                        break;
                }
        }
        t1=(min+max)/2;
        while (t1!=t2)
        {
                t2=t1;
                count=0;
                min=0;
                max=0;
                for(int i=0;i<t1;i++)
                {
                        count=count+Dih[i];
                        min=min+Dih[i]*i;
                }
                max=(histall-min)/(countall-count);
                min=min/count;
                t1=(min+max)/2;
        }
        return t1;
}

void _fastcall Dilation(Graphics::TBitmap * temp1)
{
   Graphics::TBitmap * temp=new Graphics::TBitmap;
   temp->Assign(temp1);
   int a,b;
   a=temp->Width;
   b=temp->Height;
   int c=(a+2)*(b+2);
   int * gray = new int [c];
   int * gray1 = new int [c];
   temp->PixelFormat=pf24bit;
   Byte * ptr;
   for(int i=0;i<temp1->Height;i++)
   {
      ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<temp1->Width*3;j+=3)
      {
          gray[(i+1)*(a+2)+j/3+1]=ptr[j];
          gray1[(i+1)*(a+2)+j/3+1]=ptr[j];
      }
   }
   for(int i=1;i<b+1;i++)
   {
      for(int j=1;j<a+1;j++)
      {
          if(gray[i*(a+2)+j]==0)
          {
              gray1[(i-1)*(a+2)+j]=0;
              gray1[(i+1)*(a+2)+j]=0;
              gray1[i*(a+2)+j+1]=0;
              gray1[i*(a+2)+j-1]=0;
          }
      }
   }
   for(int i=0;i<temp1->Height;i++)
   {
      ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<temp1->Width*3;j+=3)
      {
          ptr[j]=ptr[j+1]=ptr[j+2]=gray1[(i+1)*(a+2)+j/3+1];
      }
   }
   temp1->Assign(temp);
   delete temp;
   temp=NULL;
   delete []gray;
   delete []gray1;
   ptr=NULL;
}

void _fastcall Erosion(Graphics::TBitmap * temp1)
{
   Graphics::TBitmap * temp=new Graphics::TBitmap;
   temp->Assign(temp1);
   int a,b;
   a=temp->Width;
   b=temp->Height;
   int c=(a+2)*(b+2);
   int * gray = new int [c];
   int * gray1 = new int [c];
   temp->PixelFormat=pf24bit;
   Byte * ptr;
   for(int i=0;i<temp1->Height;i++)
   {
      ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<temp1->Width*3;j+=3)
      {
          gray[(i+1)*(a+2)+j/3+1]=ptr[j];
          gray1[(i+1)*(a+2)+j/3+1]=ptr[j];
      }
   }
   for(int i=1;i<b+1;i++)
   {
      for(int j=1;j<a+1;j++)
      {
          if(gray[i*(a+2)+j]==0&&gray[(i-1)*(a+2)+j]+gray[(i+1)*(a+2)+j]+gray[i*(a+2)+j+1]+gray[i*(a+2)+j-1]>0)
          {
              gray1[i*(a+2)+j]=255;
          }
      }
   }
   for(int i=0;i<temp1->Height;i++)
   {
      ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<temp1->Width*3;j+=3)
      {
          ptr[j]=ptr[j+1]=ptr[j+2]=gray1[(i+1)*(a+2)+j/3+1];
      }
   }
   temp1->Assign(temp);
   delete temp;
   delete []gray;
   delete []gray1;
   ptr=NULL;
}
void FFT(complex<float> *Array,complex<float> *FArray,int r)
{
    long count;
    int i,j,k;
    int bfsize,p;
    float angle;

    complex<float> * W,* X1,* X2, *X;

    count = 1<<r;
    X1=new complex<float> [count];
    X2=new complex<float> [count];
    W=new complex<float> [count/2];


    for(i=0;i<count;i++)
    {
       X1[i]=Array[i];
    }

    for(i=0;i<count/2;i++)
    {
       angle=-i*M_PI*2/count;
       W[i]=complex<float>(cos(angle),sin(angle));
    }

    for(k=0;k<r;k++)
    {
       for(j=0;j<1<<k;j++)
       {
          bfsize=1<<(r-k);
          for(i=0;i<bfsize/2;i++)
          {
             p=j*bfsize;
             X2[i+p]=X1[i+p]+X1[i+p+bfsize/2];
             X2[i+p+bfsize/2]=(X1[i+p]-X1[i+p+bfsize/2])*W[i*(1<<k)];
          }
       }
       X=X1;
       X1=X2;
       X2=X;
    }

    for(j=0;j<count;j++)
    {
       p=0;
       for(i=0;i<r;i++)
       {
          if(j&(1<<i))
          {
             p+=1<<(r-i-1);
          }
       }
       FArray[j]=X1[p];
    }
    delete []W;
    delete []X1;
    delete []X2;
    X=NULL;
}

void BmpToMatrix(Graphics::TBitmap * temp,int **Matrix)
{
   int w=temp->Width;
   int h=temp->Height;
   temp->PixelFormat=pf24bit;
   for(int i=0;i<h;i++)
   {
      Byte * ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<w*3;j+=3)
      {
         Matrix[i][j/3]=ptr[j];
      }
   }
}
void BmpToMatrix(Graphics::TBitmap * temp,int *Matrix)
{
   int w=temp->Width;
   int h=temp->Height;
   temp->PixelFormat=pf24bit;
   for(int i=0;i<h;i++)
   {
      Byte * ptr=(Byte*)temp->ScanLine[i];
      for(int j=0;j<w*3;j+=3)
      {
         Matrix[j/3+i*w]=ptr[j];
      }
   }
}

void FFTBmpfeaN(Graphics::TBitmap* temp,  float * FEA )
{
   int w=temp->Width;
   int h=temp->Height;
   temp->PixelFormat=pf24bit;
   int * tfea= new int [240];
   for(int i=0;i<240;i++)
   {
        tfea[i]=0;
        FEA[i]=0;
   }
   int *gray3=new  int [w*h];
   complex<float> * gray2=new complex<float>[w*h];
   complex<float> * gray1=new complex<float>[w*h];
   BmpToMatrix(temp,gray3);
   int p;
   int m,n;
   int w1,h1,wp,hp;
   w1=1;
   h1=1;
   wp=0;
   hp=0;
   while(w1*2<=w)
   {
        w1*=2;
        wp++;
   }
   while(h1*2<=h)
   {
        h1*=2;
        hp++;
   }

   for(int i=0;i<h;i++)
   {
      for(int j=0;j<w;j++)
      {
         gray1[i*w+j]=complex<float>(gray3[i*w+j],0);
      }
   }

   for(int i=0;i<h;i++)
   {
      FFT(&gray1[i*w],&gray2[i*w],wp);
   }

   for(int i=0;i<h;i++)
   {
      for(int j=0;j<w;j++)
      {
         gray1[i+h*j]=gray2[j+w*i];
      }
   }

   for(int i=0;i<w;i++)
   {
      FFT(&gray1[i*h],&gray2[i*h],hp);
   }

   for(int i=0;i<h;i++)
   {
      for(int j=0;j<w;j++)
      {
         p=sqrt(gray2[j*h+i].real()*gray2[j*h+i].real()+gray2[j*h+i].imag()*gray2[j*h+i].imag())/100;   ////分母含义

         m=h-1-(i<h/2?i+h/2:i-h/2);
         n=w-1-(j<w/2?j+w/2:j-w/2);
         gray3[m*w+n]=p;
      }
   }
   float r1,arg1;
   int r2,arg2;

   for(int i=0;i<h/2;i++)
   {
        for(int j=0;j<w;j++)
        {
                r1=Power((i-128)*(i-128)+(j-128)*(j-128),0.5);
                if(j==128)
                {
                        arg1=M_PI/2;
                }
                else
                {
                        arg1=ArcTan2(128-i,j-128);
                }
                if(r1 <128&&r1>0)
                {
                        r2=r1/16;
                        arg2=arg1*15/M_PI;
                        FEA[r2*30+arg2*2]+=gray3[i*w+j];
                        tfea[r2*30+arg2*2]+=1;
                }
        }
   }

   for(int i=0;i<120;i++)
   {
        if (tfea[i*2] ==0)
          {
             tfea[i*2] = 1 ;
          }
        FEA[i*2]=FEA[i*2]/tfea[i*2];
   }
   for(int i=0;i<h/2;i++)
   {
        for(int j=0;j<w;j++)
        {
                r1=Power((i-128)*(i-128)+(j-128)*(j-128),0.5);
                if(j==128)
                {
                        arg1=M_PI/2;
                }
                else
                {
                        arg1=ArcTan2(128-i,j-128);
                }
                if(r1 <128&&r1>0)
                {
                        r2=r1/16;
                        arg2=arg1*15/M_PI;
                        FEA[r2*30+arg2*2+1]+=(gray3[i*w+j]-FEA[r2*30+arg2*2])*(gray3[i*w+j]-FEA[r2*30+arg2*2]);
                }
        }
   }
   for(int i=0;i<120;i++)
   {
        if (tfea[i*2] ==0)
          {
             tfea[i*2] = 1 ;
          }
        FEA[i*2+1]=FEA[i*2+1]/tfea[i*2];
   }
   delete []tfea;
   delete []gray1;
   delete []gray2;
   delete []gray3;

}

void MatrixGlcm(int ** Gray,int **Glcm,int h,int w,int dist, int dir, int GrayLevel)
{
        //int GrayLevel = 8;  //灰度级
        //dist: 步长  =1,2,3,4,5,6,7?
        int **Gray1 = new int *[h];
        for(int i=0;i<h;i++)
        {
                Gray1[i]= new int [w];
        }
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<w;j++)
                {
                        Gray1[i][j]=Gray[i][j]/(256/GrayLevel);

                }
        }
        for(int i=0;i<GrayLevel;i++)
        {
                for(int j=0;j<GrayLevel;j++)
                {
                        Glcm[i][j]=0;

                }
        }
        
        int count = 0 ;
      if(dir==0)  //0度
        {
                for(int i=0;i<h;i++)
                {
                        for(int j=0;j<w-dist;j++)
                        {
                                Glcm[Gray1[i][j]][Gray1[i][j+dist]]+=1;
                                Glcm[Gray1[i][j+dist]][Gray1[i][j]]+=1;
                                count ++ ;
                        }
                }
        }
        else  if(dir==1)   //90度
        {
                for(int i=0;i<h-dist;i++)
                {
                        for(int j=0;j<w;j++)
                        {
                                Glcm[Gray1[i][j]][Gray1[i+dist][j]]+=1;
                                Glcm[Gray1[i+dist][j]][Gray1[i][j]]+=1;
                                count ++ ;
                        }
                }
        }
        for(int i=0;i<h;i++)
        {
                delete []Gray1[i];
        }
        delete []Gray1;
}

void TextureCal3333(int **Glcm, int h, float * FEA)
{
        float **Gray1 = new float *[h];
        for(int i=0;i<h;i++)
        {
                Gray1[i]= new float [h];
        }
        int total=0;
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<h;j++)
                {
                        total+=Glcm[i][j];
                }
        }
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<h;j++)
                {
                        Gray1[i][j]=Glcm[i][j]/(float)total;

                }
        }
      

        FEA[0]=0;
        FEA[1]=0;
        FEA[2]=0;
        FEA[3]=0;
        float u1(0),u2(0),o1(0),o2(0);
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<h;j++)
                {
                    u1 = u1 + (i+1)* Gray1[i][j];
                    u2 = u2 + (j+1)* Gray1[i][j];

                }
        }
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<h;j++)
                {
                    o1 = o1 + (i+1-u1)* (i+1-u1)*Gray1[i][j];
                    o2 = o2 + (j+1-u2)* (j+1-u2)*Gray1[i][j];
                }
        }
        for(int i=0;i<h;i++)
        {
                for(int j=0;j<h;j++)
                {

                        FEA[0] = FEA[0] + (i-j)*(i-j)* Gray1[i][j] ;  //对比度
                        if(Gray1[i][j]!=0)
                        {
                                FEA[1]-=Gray1[i][j]*Log2(Gray1[i][j]); //熵
                        }
                        FEA[2]= FEA[2] + (i+1)*(j+1)*Gray1[i][j];               //自相关
                        FEA[3]= FEA[3]+(1/(1+(i-j)*(i-j)))*Gray1[i][j];  //局部平稳
                }
        }
       FEA[2]= (FEA[2] - u1*u2) / (o1*o2);



        for(int i=0;i<h;i++)
        {
                delete []Gray1[i];
        }
        delete []Gray1;
}

void GlcmMatrixTexture4(int **Gray1, int h,int dist, float * FEA)
{
        // fnumb*4+2  维特征
        int Graylevel=256;   //8?
        int fnumb=4;
        int **Gray2 = new int *[Graylevel];
        for(int i=0;i<Graylevel;i++)
        {
                Gray2[i]= new int [Graylevel];
        }
        float * FEA0 =new float [4];
        float * FEA1 =new float [4];
        MatrixGlcm(Gray1,Gray2, h,h,dist,0, Graylevel);
        TextureCal3333(Gray2,  Graylevel,  FEA0);
        for(int i=0;i<fnumb;i++)
        {
                FEA[i]= FEA0[i];
        }
        MatrixGlcm(Gray1,Gray2, h,h,dist,1, Graylevel);
        TextureCal3333(Gray2,  Graylevel,  FEA1);
        for(int i=0;i<fnumb;i++)
        {
                FEA[i+fnumb]= FEA1[i];
        }
        /////////////////////
        //灰度均值方差
        
        /////////////////////
        delete []FEA0;
        delete []FEA1;
        for(int i=0;i<Graylevel;i++)
        {
                delete []Gray2[i];
        }
        delete []Gray2;
}

void GlcmBmpTexture4(Graphics::TBitmap* temp,  float * FEA)
{
        int w=temp->Width;
        int h=temp->Height;
        int ** Gray=new int * [h];
        for(int i=0;i<h;i++)
        {
                Gray[i]=new int [w];
        }
        BmpToMatrix(temp,Gray);
        int feanumb=8;
        float  fea[8]={0};
        for(int i=0;i<20;i++)
        {
                GlcmMatrixTexture4(Gray,  h,  i+1,  fea);
                for(int j=0;j<feanumb;j++)
                {
                        FEA[i*feanumb+j]=fea[j];
                }
        }
        for(int i=0;i<h;i++)
        {
                delete []Gray[i];
        }
        delete []Gray;
        delete []fea;
}
//---------------------------------------------------------------------------


