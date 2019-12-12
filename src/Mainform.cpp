//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Mainform.h"
#include "Fun.h"
#include <jpeg.hpp>
#include <complex>
#include <complex.h>
#include "math.h"
#include <Math.hpp>
#include <complex>
#pragma link "Excel_2K_SRVR"
#include "Excel_2K_SRVR.h"
#include <OleServer.hpp>

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void Binary(Graphics::TBitmap *temp2, int add)  //10近，50远
{
    int h = temp2->Height;
    int w = temp2->Width ;
    int ** Gray=new int * [h];
    for(int i=0;i<h;i++)
    {
            Gray[i]=new int [w];
    }

    BmpToMatrixGray(temp2,Gray);
    int * dih = new int [256];
    MatrixToDih(Gray, temp2->Width, temp2->Height, dih,256);
    //int trd=OTSUSeg(dih,256);
    //int trd=IterativeSeg(dih,256);
    int trd=IterativeSeg(dih,230);
    Segbmp(temp2, trd+add);
    for (int sy = 0 ; sy < temp2->Height ; sy ++)
              {
                  delete [] Gray[sy] ;
              }
    delete [] Gray ;
    delete [] dih ;

}

void __fastcall TForm1::NoiseFill (Byte **fill, int w , int h , int zft[256])
{


          int sum = 0 ;
          int addsum[256] ;
          for (int y = 0 ; y < 256 ; y ++)
            {
                sum = sum + zft[y] ;
                addsum[y] = 0 ;
            }
          addsum[0] = zft[0] ;
          for (int y = 1 ; y < 256 ; y ++)
            {
                addsum[y] =  addsum[y-1] + zft[y];
            }
         srand((unsigned)time(NULL));
          for (int y = 0 ; y < h ; y ++)
            {
               for (int x = 0 ; x < w ; x ++)
                 {


                          int p =rand() % sum;

                       if ( p <= zft[0] )
                          fill[y][x] = 0 ;
                       else
                         {
                             for (int s = 0 ; s < 255 ; s++)
                               {

                                    if ( p>addsum[s] && p <= addsum[s+1] )
                                      {
                                            fill[y][x] = s+1 ;
                                      }
                               }
                         }
                 }
            }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComputeZFT (Byte **fill, int w , int h , int zft[256],Byte **back)
{
    for (int i = 0 ; i < 256 ; i ++)
      zft[i] = 0 ;
    for (int y = 0 ; y < h ; y ++)
      {
         for (int x = 0 ; x < w ; x ++)
           {
               if (back[y][x]==0)
                {
                  int ssb = fill[y][x];
                  zft[ssb] += 1 ;
                }
           }
      }
}
//---------------------------------------------------------------------------

int __fastcall TForm1::GetAllInput(Byte **fill, int w , int h , int **cin, int total, int horilen , int length,Byte **black)
{
     int halflen = (length-1)/2 ;
     int count = 0 ;
     bool isblack = false ;
     for (int y = halflen; y < h ; y ++)
       {
          for (int x = halflen ; x < w - halflen ; x ++)
            {
                  isblack = true ;
                  for (int j = y-halflen ; j <=y ; j ++)
                     {
                        for (int i = x-halflen ; i <= x + halflen ; i ++)
                          {
                              if (j < y )
                                {
                                    cin[count][(j-y+ halflen)*length + (i-x+ halflen)]= int (fill[j][i]);
                                   if ( int (black[j][i]) != 0 )
                                     {
                                         isblack = false ;
                                     }

                                }
                              else
                                {
                                    if (i < x)
                                      {
                                          cin[count][length*halflen + (i-x+ halflen)] =   int (fill[j][i]);
                                         if ( int (black[j][i]) != 0 )
                                           {
                                               isblack = false ;
                                           }
                                      }
                                }
                          }
                     }

                   cin[count][horilen-2] = x ;
                   cin[count][horilen-1] = y ;
                   if (isblack)
                      count ++ ;
            }
       }
    return count ;
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::GetLShapeOutput(Byte **fill, int w , int h , int x, int y , int *recieve , int length)
{
     int halflen = (length-1)/2 ;
     int m ;
     int n ;
     for (int j = y-halflen ; j <=y ; j ++)
       {
          for (int i = x-halflen ; i <= x + halflen ; i ++)
            {
                if (j < y )
                  {
                      n = j ;
                      m = i ;
                      if (j < 0)
                        {
                            n = h + j;
                        }
                      /*if ( j >= h )
                        {
                           n = j - h ;
                        }    */
                      if (i < 0)
                        {
                           m = w + i ;
                        }
                      /*if (i >= w)
                        {
                           m = i - w ;
                        }  */

                      if ( j <  h  && i < w )
                          recieve[(j-y+ halflen)*length + (i-x+ halflen)]= int (fill[n][m]);
                      else
                          recieve[(j-y+ halflen)*length + (i-x+ halflen)]= 300;
                  }
                else
                  {
                      if (i < x)
                        {
                             n = j ;
                             m = i ;
                              if (j < 0)
                                {
                                    n = h + j;
                                }
                              /*if ( j >= h )
                                {
                                   n = j - h ;
                                }  */
                              if (i < 0)
                                {
                                   m = w + i ;
                                }
                              /*if (i >= w)
                                {
                                   m = i - w ;
                                } */

                              if ( j <  h  && i < w )
                                  recieve[length*halflen + (i-x+ halflen)]= int (fill[n][m]);
                              else
                                  recieve[length*halflen + (i-x+ halflen)]= 300;

                        }
                  }
            }
       }
     return true ;

}
//---------------------------------------------------------------------------

int __fastcall TForm1::Compare(int *input, int *output , int length )
{
          int sum = 0 ;
          for(int i = 0 ; i < length ; i ++)
            {
                if (output[i] != 300)
                  {
                      int sst = (output[i]-input[i])   ;
                      sum = sum +  sst*sst;
                  }
            }
          return sum ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ByteToBmp(Graphics::TBitmap *pic , Byte **temp)
{
     pic->PixelFormat = pf24bit ;
     int w = pic->Width ;
     int h = pic->Height ;
     Byte *ptr ;
     for (int y = 0 ; y < h ; y ++)
       {
           ptr =(Byte*) pic->ScanLine[y];
           for (int x = 0 ; x < w ; x ++)
             {
                  ptr[x*3] = ptr[x*3+1]=ptr[x*3+2] = temp[y][x];
             }
       }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BmpToByte(Graphics::TBitmap* bmp, Byte **src )
{
        bmp->PixelFormat = pf24bit;
        Byte* ptr;
        for(int y = 0; y < bmp->Height; y++)
        {
                ptr = (Byte*)bmp->ScanLine[y];
                 for (int x = 0 ; x < bmp->Width ; x ++)
                  src[y][x]= ptr[x*3+1];
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{//合成N幅纹理图像
         int N=StrToInt(this->Edit3->Text);
               for(int q=0;q<N;q++)
           {
               TJPEGImage *jpg = new TJPEGImage ;
               Graphics::TBitmap *bmp = new Graphics::TBitmap ;
               jpg->LoadFromFile(GetCurrentDir()+"\\"+IntToStr(0)+"_"+IntToStr(0)+".jpg");
               bmp->Assign(jpg);

               AnsiString mmid = ExtractFileName(GetCurrentDir()+"\\"+IntToStr(0)+"_"+IntToStr(0)+".jpg").SubString(1,5) ;
               Graphics::TBitmap *pic = new Graphics::TBitmap ;
               pic->Assign(bmp);
               pic->PixelFormat = pf8bit ;
               Binary( pic , 0 );

              Dilation(pic);
              Dilation(pic);
              Dilation(pic);
              Dilation(pic);


              Erosion(pic);
              Erosion(pic);
              Erosion(pic);
              Erosion(pic);
              Erosion(pic);

              int w = 256 ;
              int h = 256 ;
              int w1 = bmp->Width ;
              int h1 = bmp->Height ;


              Byte **temp = new Byte * [h] ;
                     for (int y = 0 ; y < h ; y ++)
                       {
                          temp[y] = new Byte[w];
                       }
              Byte **temp1 = new Byte * [h1] ;
              Byte **tempb = new Byte * [h1] ;
                     for (int y = 0 ; y < h1 ; y ++)
                       {
                          temp1[y] = new Byte[w1];
                          tempb[y] = new Byte[w1];
                       }
              BmpToByte(bmp  , temp1);
              BmpToByte(pic  , tempb);
              for (int y = 0 ; y < h ; y ++)
              {
                 for (int x = 0 ; x < w ; x ++)
                   {
                       temp[y][x] = 255 ;
                   }
              }

             int zft[256] ;

             ComputeZFT(temp1, w1, h1, zft,tempb);
              int len = 9; //L形邻域大小
              int ma = ((len * len)  - 1)/2 ;
              int halflent = (len -1 )/ 2 ;
              int total = (w1-len+1)*(h1-halflent) ;
              int *recout = new int[ma];
              int **recin= new int*[total];
              for (int ssb = 0 ; ssb < total ; ssb ++)
                {
                    recin[ssb] = new int[ma+2];
                }
              int bcount = GetAllInput(temp1, w1 , h1 , recin, total, ma+2 , len , tempb);

          for (int js = 0 ; js < 1 ; js ++)
            {
               NoiseFill(temp, w , h , zft);
               for (int y = 0 ; y < h ; y ++)
                {
                   for (int x = 0 ; x <w ; x ++)
                     {
                         int xmax = 0 ;
                         int ymax = 0 ;
                         int cc = 0 ;
                         int max = 10000000000 ;
                         int xmax2 = 0 ;
                         int ymax2 = 0 ;
                         int max2 = 10000000000 ;
                         GetLShapeOutput(temp,w,h,x,y,recout,len);

                         for (int j = 0 ; j < bcount ; j ++)
                           {

                                cc = Compare(recin[j],recout,ma);

                                if ( cc<max2)
                                  {
                                     max2 = cc ;
                                     xmax2 = recin[j][ma] ;
                                     ymax2 = recin[j][ma+1] ;
                                  }


                           }
                           temp[y][x] =  temp1[ymax2][xmax2];
                     }
                }

            }

                Graphics::TBitmap *repic = new Graphics::TBitmap ;
                repic->Width = w ;
                repic->Height = h ;

                ByteToBmp(repic  , temp);

                 Graphics::TBitmap *pic1 = new Graphics::TBitmap ;
                 pic1->Width = 256 ; pic1->Height = 256 ;
                 pic1->Assign(repic);
                 AnsiString dir=GetCurrentDir()+"\\texture"+"\\"+IntToStr(0)+"-"+IntToStr(q)+".bmp";
                 pic1->SaveToFile(dir);


                 delete pic1;
                 delete bmp;
                 delete jpg;
                 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
   this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
  float fftfeamean[240]={0};
   int N=StrToInt(this->Edit3->Text);
   float **fftfea;
   fftfea=new float *[N];
   for(int j=0;j<N;j++)
   {
      fftfea[j]=new float [240];
   }

   Graphics::TBitmap *bmp = new Graphics::TBitmap;
   this->ListBox1->Clear();
     for(int q=0;q<N;q++)
     {
      AnsiString dir="e://texture/"+IntToStr(q)+".bmp";
      bmp->LoadFromFile(dir);
      FFTBmpfeaN(bmp,fftfea[q]);
     }

   for(int i=0;i<240;i++)
   {
      float s=0;
      for(int q=0;q<N;q++)
      {
        s+=fftfea[q][i];
      }
        fftfeamean[i]=s/N;
        this->ListBox1->Items->Add(FloatToStr(fftfeamean[i]));
   }
   delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
   float glcmfeamean[160]={0};
   int N=StrToInt(this->Edit3->Text);
   float **glcmfea;
   glcmfea=new float *[N];
   for(int j=0;j<N;j++)
   {
      glcmfea[j]=new float [160];
   }

   Graphics::TBitmap *bmp = new Graphics::TBitmap;
   this->ListBox2->Clear();
   //char tmp[1];
    for(int c=0;c<N;c++)
     {
      //tmp[0]=c;
      //AnsiString result(tmp,1);
      AnsiString dir=GetCurrentDir()+"\\texture"+"\\"+IntToStr(0)+'-'+IntToStr(c)+".bmp";
      bmp->LoadFromFile(dir);
      GlcmBmpTexture4(bmp,glcmfea[c]);
     }
     for(int c=0;c<N;c++)
       {for(int i=0;i<160;i++)
         {
           this->ListBox2->Items->Add(FloatToStr(glcmfea[c][i]));
         }
       }
   delete bmp;
   //将检材GLCM特征平均值导出到Excel中
   AnsiString StrFileName=GetCurrentDir()+"\\Export.xls";
   if(!FileExists(StrFileName))
   {
      MessageBox(Handle,"Excel文件Export.xls不存在！","信息提示",MB_OK);
      return;
   }
   Variant ExcelApp;   //对象
   Variant WorkBook1;   //工作簿
   Variant WorkSheet1;  //工作表
   Variant Range;       //范围
   Variant Borders;      //边框
   try
   {
      ExcelApp=Variant::CreateObject("Excel.Application");
   }
   catch(...)
   {
     MessageBox(Handle,"无法启动Excel！","信息提示",MB_ICONSTOP|MB_OK);
     return;
   }
   ExcelApp.OlePropertySet("Visible",false);//设置Excel为不可见
   //打开指定的Excel文件，文件中最好只设定一个Sheet
   ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Open",StrFileName.c_str());
   WorkBook1=ExcelApp.OlePropertyGet("ActiveWorkBook");
   WorkSheet1=WorkBook1.OlePropertyGet("ActiveSheet");   //获得当前默认的Sheet

   int nRow=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
   int nCol=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");
   //给Excel表赋值
   for (int q=1;q<=N;q++)
    {for(int i=1;i<161;i++)
      {
         AnsiString StrValue=this->ListBox2->Items->Strings[160*(q-1)+i-1];
         WorkSheet1.OlePropertyGet("Cells",i+4,q).OlePropertySet("Value",StrValue.c_str());
      }
     }
   AnsiString StrRange="A"+IntToStr(1)+":I"+IntToStr(nRow);//获取操作范围
   Range=WorkSheet1.OlePropertyGet("Range",StrRange.c_str());
   Borders=Range.OlePropertyGet("Borders");//获取边框对象
   // Borders.OlePropertySet("linestyle",xlNone); //去掉边框线
   //画表格边框
   Borders.OlePropertySet("linestyle",xlContinuous);
   Borders.OlePropertySet("weight",xlThin);
   Borders.OlePropertySet("colorindex",xlAutomatic);
   WorkBook1.OleProcedure("Save"); //保存表格
   WorkBook1.OleProcedure("Close"); //关闭表格
   ExcelApp.OleFunction("Quit");    //退出Excel
   char StrCurDir[MAX_PATH+1];
   GetCurrentDirectory(MAX_PATH,StrCurDir);
   char exeStr[]="Export.xls";
   SHELLEXECUTEINFO Info;
   memset(&Info,0,sizeof(Info));
   Info.cbSize=sizeof(Info);
   Info.lpVerb="open";
   Info.lpFile=exeStr;
   Info.lpParameters=NULL;
   Info.fMask=SEE_MASK_NOCLOSEPROCESS;
   Info.nShow=SW_SHOWDEFAULT;
   if(!ShellExecuteEx (&Info))
       MessageBox(Handle,"打开Excel文件Export.xls失败！","信息提示",MB_OK);
   SetCurrentDirectory(StrCurDir);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn7Click(TObject *Sender)
{//合成N幅纹理图像
           int N=StrToInt(this->Edit3->Text);
               for(int q=0;q<N;q++)
           {
               TJPEGImage *jpg = new TJPEGImage ;
               Graphics::TBitmap *bmp = new Graphics::TBitmap ;
               jpg->LoadFromFile("d://我的文档/桌面/纹理子图采集/样本/"+IntToStr(q)+".jpg");
               bmp->Assign(jpg);

               AnsiString mmid = ExtractFileName("d://我的文档/桌面/纹理子图采集/样本/"+IntToStr(q)+".jpg").SubString(1,5) ;
               Graphics::TBitmap *pic = new Graphics::TBitmap ;
               pic->Assign(bmp);
               pic->PixelFormat = pf8bit ;
               Binary( pic , 0 );

              Dilation(pic);
              Dilation(pic);
              Dilation(pic);
              Dilation(pic);


              Erosion(pic);
              Erosion(pic);
              Erosion(pic);
              Erosion(pic);
              Erosion(pic);

              int w = 256 ;
              int h = 256 ;
              int w1 = bmp->Width ;
              int h1 = bmp->Height ;


              Byte **temp = new Byte * [h] ;
                     for (int y = 0 ; y < h ; y ++)
                       {
                          temp[y] = new Byte[w];
                       }
              Byte **temp1 = new Byte * [h1] ;
              Byte **tempb = new Byte * [h1] ;
                     for (int y = 0 ; y < h1 ; y ++)
                       {
                          temp1[y] = new Byte[w1];
                          tempb[y] = new Byte[w1];
                       }
              BmpToByte(bmp  , temp1);
              BmpToByte(pic  , tempb);
              for (int y = 0 ; y < h ; y ++)
              {
                 for (int x = 0 ; x < w ; x ++)
                   {
                       temp[y][x] = 255 ;
                   }
              }

             int zft[256] ;

             ComputeZFT(temp1, w1, h1, zft,tempb);
              int len = 9; //L形邻域大小
              int ma = ((len * len)  - 1)/2 ;
              int halflent = (len -1 )/ 2 ;
              int total = (w1-len+1)*(h1-halflent) ;
              int *recout = new int[ma];
              int **recin= new int*[total];
              for (int ssb = 0 ; ssb < total ; ssb ++)
                {
                    recin[ssb] = new int[ma+2];
                }
              int bcount = GetAllInput(temp1, w1 , h1 , recin, total, ma+2 , len , tempb);

          for (int js = 0 ; js < 1 ; js ++)
            {
               NoiseFill(temp, w , h , zft);
               for (int y = 0 ; y < h ; y ++)
                {
                   for (int x = 0 ; x <w ; x ++)
                     {
                         int xmax = 0 ;
                         int ymax = 0 ;
                         int cc = 0 ;
                         int max = 10000000000 ;
                         int xmax2 = 0 ;
                         int ymax2 = 0 ;
                         int max2 = 10000000000 ;
                         GetLShapeOutput(temp,w,h,x,y,recout,len);

                         for (int j = 0 ; j < bcount ; j ++)
                           {

                                cc = Compare(recin[j],recout,ma);

                                if ( cc<max2)
                                  {
                                     max2 = cc ;
                                     xmax2 = recin[j][ma] ;
                                     ymax2 = recin[j][ma+1] ;
                                  }


                           }
                           temp[y][x] =  temp1[ymax2][xmax2];
                     }
                }

            }

                Graphics::TBitmap *repic = new Graphics::TBitmap ;
                repic->Width = w ;
                repic->Height = h ;

                ByteToBmp(repic  , temp);

                 Graphics::TBitmap *pic1 = new Graphics::TBitmap ;
                 pic1->Width = 256 ; pic1->Height = 256 ;
                 pic1->Assign(repic);
                 AnsiString dir="e://sample/"+IntToStr(q)+".bmp";
                 pic1->SaveToFile(dir);


                 delete pic1;
                 delete bmp;
                 delete jpg;
                 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn8Click(TObject *Sender)
{
   float fftfeamean[240]={0};
   int N=StrToInt(this->Edit3->Text);
   float **fftfea;
   fftfea=new float *[N];
   for(int j=0;j<N;j++)
   {
      fftfea[j]=new float [240];
   }

   Graphics::TBitmap *bmp = new Graphics::TBitmap;
   this->ListBox3->Clear();
     for(int q=0;q<N;q++)
     {
      AnsiString dir="e://sample/"+IntToStr(q)+".bmp";
      bmp->LoadFromFile(dir);
      FFTBmpfeaN(bmp,fftfea[q]);
     }

   for(int i=0;i<240;i++)
   {
      float s=0;
      for(int q=0;q<N;q++)
      {
        s+=fftfea[q][i];
      }
        fftfeamean[i]=s/N;
        this->ListBox3->Items->Add(FloatToStr(fftfeamean[i]));
   }
   delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn9Click(TObject *Sender)
{
  float glcmfeamean[160]={0};
   int N=StrToInt(this->Edit3->Text);
   float **glcmfea;
   glcmfea=new float *[N];
   for(int j=0;j<N;j++)
   {
      glcmfea[j]=new float [160];
   }

   Graphics::TBitmap *bmp = new Graphics::TBitmap;
   this->ListBox4->Clear();
    for(int q=0;q<N;q++)
     {
      AnsiString dir="e://sample/"+IntToStr(q)+".bmp";
      bmp->LoadFromFile(dir);
      GlcmBmpTexture4(bmp,glcmfea[q]);
     }

   for(int i=0;i<160;i++)
   {
      float s=0;
      for(int q=0;q<N;q++)
      {
        s+=glcmfea[q][i];
      }
        glcmfeamean[i]=s/N;
        this->ListBox4->Items->Add(FloatToStr(glcmfeamean[i]));
   }
   delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn10Click(TObject *Sender)
{//计算检材与样本FT特征向量欧氏距离
    float d=0;
    for(int i=0;i<240;i++)
    {
       d+=(StrToFloat(this->ListBox1->Items->Strings[i])-StrToFloat(this->ListBox3->Items->Strings[i]))*(StrToFloat(this->ListBox1->Items->Strings[i])-StrToFloat(this->ListBox3->Items->Strings[i]));
    }
    d=sqrt(d);
    this->Edit1->Text=FloatToStr(d);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn11Click(TObject *Sender)
{//计算检材与样本GLCM特征向量欧氏距离
    float d=0;
    for(int i=0;i<160;i++)
    {
       d+=(StrToFloat(this->ListBox2->Items->Strings[i])-StrToFloat(this->ListBox4->Items->Strings[i]))*(StrToFloat(this->ListBox2->Items->Strings[i])-StrToFloat(this->ListBox4->Items->Strings[i]));
    }
    d=sqrt(d);
    this->Edit2->Text=FloatToStr(d);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn12Click(TObject *Sender)
{//将检材FT特征平均值导出到Excel中
   AnsiString StrFileName=GetCurrentDir()+"\\Export.xls";
   if(!FileExists(StrFileName))
   {
      MessageBox(Handle,"Excel文件Export.xls不存在！","信息提示",MB_OK);
      return;
   }
   Variant ExcelApp;   //对象
   Variant WorkBook1;   //工作簿
   Variant WorkSheet1;  //工作表
   Variant Range;       //范围
   Variant Borders;      //边框
   try
   {
      ExcelApp=Variant::CreateObject("Excel.Application");
   }
   catch(...)
   {
     MessageBox(Handle,"无法启动Excel！","信息提示",MB_ICONSTOP|MB_OK);
     return;
   }
   ExcelApp.OlePropertySet("Visible",false);//设置Excel为不可见
   //打开指定的Excel文件，文件中最好只设定一个Sheet
   ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Open",StrFileName.c_str());
   WorkBook1=ExcelApp.OlePropertyGet("ActiveWorkBook");
   WorkSheet1=WorkBook1.OlePropertyGet("ActiveSheet");   //获得当前默认的Sheet
   //清空Excel表的数据
   int nRow=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
   int nCol=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");
   for(int i=5;i<=nRow+1;i++)
       WorkSheet1.OlePropertyGet("Rows",i).OlePropertySet("Value","");
   //给Excel表赋值
    for(int i=1;i<241;i++)
      {
         AnsiString StrValue=this->ListBox1->Items->Strings[i-1];
         WorkSheet1.OlePropertyGet("Cells",i+4,1).OlePropertySet("Value",StrValue.c_str());
      }

   AnsiString StrRange="A"+IntToStr(1)+":I"+IntToStr(nRow);//获取操作范围
   Range=WorkSheet1.OlePropertyGet("Range",StrRange.c_str());
   Borders=Range.OlePropertyGet("Borders");//获取边框对象
   // Borders.OlePropertySet("linestyle",xlNone); //去掉边框线
   //画表格边框
   Borders.OlePropertySet("linestyle",xlContinuous);
   Borders.OlePropertySet("weight",xlThin);
   Borders.OlePropertySet("colorindex",xlAutomatic);
   WorkBook1.OleProcedure("Save"); //保存表格
   WorkBook1.OleProcedure("Close"); //关闭表格
   ExcelApp.OleFunction("Quit");    //退出Excel
   char StrCurDir[MAX_PATH+1];
   GetCurrentDirectory(MAX_PATH,StrCurDir);
   char exeStr[]="Export.xls";
   SHELLEXECUTEINFO Info;
   memset(&Info,0,sizeof(Info));
   Info.cbSize=sizeof(Info);
   Info.lpVerb="open";
   Info.lpFile=exeStr;
   Info.lpParameters=NULL;
   Info.fMask=SEE_MASK_NOCLOSEPROCESS;
   Info.nShow=SW_SHOWDEFAULT;
   if(!ShellExecuteEx (&Info))
       MessageBox(Handle,"打开Excel文件Export.xls失败！","信息提示",MB_OK);
   SetCurrentDirectory(StrCurDir);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn13Click(TObject *Sender)
{//将检材GLCM特征平均值导出到Excel中
   AnsiString StrFileName=GetCurrentDir()+"\\Export.xls";
   if(!FileExists(StrFileName))
   {
      MessageBox(Handle,"Excel文件Export.xls不存在！","信息提示",MB_OK);
      return;
   }
   Variant ExcelApp;   //对象
   Variant WorkBook1;   //工作簿
   Variant WorkSheet1;  //工作表
   Variant Range;       //范围
   Variant Borders;      //边框
   try
   {
      ExcelApp=Variant::CreateObject("Excel.Application");
   }
   catch(...)
   {
     MessageBox(Handle,"无法启动Excel！","信息提示",MB_ICONSTOP|MB_OK);
     return;
   }
   ExcelApp.OlePropertySet("Visible",false);//设置Excel为不可见
   //打开指定的Excel文件，文件中最好只设定一个Sheet
   ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Open",StrFileName.c_str());
   WorkBook1=ExcelApp.OlePropertyGet("ActiveWorkBook");
   WorkSheet1=WorkBook1.OlePropertyGet("ActiveSheet");   //获得当前默认的Sheet

   int nRow=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
   int nCol=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");
   //给Excel表赋值
    for(int i=1;i<161;i++)
      {
         AnsiString StrValue=this->ListBox2->Items->Strings[i-1];
         WorkSheet1.OlePropertyGet("Cells",i+4,3).OlePropertySet("Value",StrValue.c_str());
      }

   AnsiString StrRange="A"+IntToStr(1)+":I"+IntToStr(nRow);//获取操作范围
   Range=WorkSheet1.OlePropertyGet("Range",StrRange.c_str());
   Borders=Range.OlePropertyGet("Borders");//获取边框对象
   // Borders.OlePropertySet("linestyle",xlNone); //去掉边框线
   //画表格边框
   Borders.OlePropertySet("linestyle",xlContinuous);
   Borders.OlePropertySet("weight",xlThin);
   Borders.OlePropertySet("colorindex",xlAutomatic);
   WorkBook1.OleProcedure("Save"); //保存表格
   WorkBook1.OleProcedure("Close"); //关闭表格
   ExcelApp.OleFunction("Quit");    //退出Excel
   char StrCurDir[MAX_PATH+1];
   GetCurrentDirectory(MAX_PATH,StrCurDir);
   char exeStr[]="Export.xls";
   SHELLEXECUTEINFO Info;
   memset(&Info,0,sizeof(Info));
   Info.cbSize=sizeof(Info);
   Info.lpVerb="open";
   Info.lpFile=exeStr;
   Info.lpParameters=NULL;
   Info.fMask=SEE_MASK_NOCLOSEPROCESS;
   Info.nShow=SW_SHOWDEFAULT;
   if(!ShellExecuteEx (&Info))
       MessageBox(Handle,"打开Excel文件Export.xls失败！","信息提示",MB_OK);
   SetCurrentDirectory(StrCurDir);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn14Click(TObject *Sender)
{//将样本FT特征平均值导出到Excel中
   AnsiString StrFileName=GetCurrentDir()+"\\Export.xls";
   if(!FileExists(StrFileName))
   {
      MessageBox(Handle,"Excel文件Export.xls不存在！","信息提示",MB_OK);
      return;
   }
   Variant ExcelApp;   //对象
   Variant WorkBook1;   //工作簿
   Variant WorkSheet1;  //工作表
   Variant Range;       //范围
   Variant Borders;      //边框
   try
   {
      ExcelApp=Variant::CreateObject("Excel.Application");
   }
   catch(...)
   {
     MessageBox(Handle,"无法启动Excel！","信息提示",MB_ICONSTOP|MB_OK);
     return;
   }
   ExcelApp.OlePropertySet("Visible",false);//设置Excel为不可见
   //打开指定的Excel文件，文件中最好只设定一个Sheet
   ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Open",StrFileName.c_str());
   WorkBook1=ExcelApp.OlePropertyGet("ActiveWorkBook");
   WorkSheet1=WorkBook1.OlePropertyGet("ActiveSheet");   //获得当前默认的Sheet

   int nRow=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
   int nCol=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");
   //给Excel表赋值
    for(int i=1;i<241;i++)
      {
         AnsiString StrValue=this->ListBox3->Items->Strings[i-1];
         WorkSheet1.OlePropertyGet("Cells",i+4,2).OlePropertySet("Value",StrValue.c_str());
      }

   AnsiString StrRange="A"+IntToStr(1)+":I"+IntToStr(nRow);//获取操作范围
   Range=WorkSheet1.OlePropertyGet("Range",StrRange.c_str());
   Borders=Range.OlePropertyGet("Borders");//获取边框对象
   // Borders.OlePropertySet("linestyle",xlNone); //去掉边框线
   //画表格边框
   Borders.OlePropertySet("linestyle",xlContinuous);
   Borders.OlePropertySet("weight",xlThin);
   Borders.OlePropertySet("colorindex",xlAutomatic);
   WorkBook1.OleProcedure("Save"); //保存表格
   WorkBook1.OleProcedure("Close"); //关闭表格
   ExcelApp.OleFunction("Quit");    //退出Excel
   char StrCurDir[MAX_PATH+1];
   GetCurrentDirectory(MAX_PATH,StrCurDir);
   char exeStr[]="Export.xls";
   SHELLEXECUTEINFO Info;
   memset(&Info,0,sizeof(Info));
   Info.cbSize=sizeof(Info);
   Info.lpVerb="open";
   Info.lpFile=exeStr;
   Info.lpParameters=NULL;
   Info.fMask=SEE_MASK_NOCLOSEPROCESS;
   Info.nShow=SW_SHOWDEFAULT;
   if(!ShellExecuteEx (&Info))
       MessageBox(Handle,"打开Excel文件Export.xls失败！","信息提示",MB_OK);
   SetCurrentDirectory(StrCurDir);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn15Click(TObject *Sender)
{//将样本GLCM特征平均值导出到Excel中
   AnsiString StrFileName=GetCurrentDir()+"\\Export.xls";
   if(!FileExists(StrFileName))
   {
      MessageBox(Handle,"Excel文件Export.xls不存在！","信息提示",MB_OK);
      return;
   }
   Variant ExcelApp;   //对象
   Variant WorkBook1;   //工作簿
   Variant WorkSheet1;  //工作表
   Variant Range;       //范围
   Variant Borders;      //边框
   try
   {
      ExcelApp=Variant::CreateObject("Excel.Application");
   }
   catch(...)
   {
     MessageBox(Handle,"无法启动Excel！","信息提示",MB_ICONSTOP|MB_OK);
     return;
   }
   ExcelApp.OlePropertySet("Visible",false);//设置Excel为不可见
   //打开指定的Excel文件，文件中最好只设定一个Sheet
   ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Open",StrFileName.c_str());
   WorkBook1=ExcelApp.OlePropertyGet("ActiveWorkBook");
   WorkSheet1=WorkBook1.OlePropertyGet("ActiveSheet");   //获得当前默认的Sheet

   int nRow=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
   int nCol=WorkSheet1.OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");
   //给Excel表赋值
    for(int i=1;i<161;i++)
      {
         AnsiString StrValue=this->ListBox4->Items->Strings[i-1];
         WorkSheet1.OlePropertyGet("Cells",i+4,4).OlePropertySet("Value",StrValue.c_str());
      }

   AnsiString StrRange="A"+IntToStr(1)+":I"+IntToStr(nRow);//获取操作范围
   Range=WorkSheet1.OlePropertyGet("Range",StrRange.c_str());
   Borders=Range.OlePropertyGet("Borders");//获取边框对象
   // Borders.OlePropertySet("linestyle",xlNone); //去掉边框线
   //画表格边框
   Borders.OlePropertySet("linestyle",xlContinuous);
   Borders.OlePropertySet("weight",xlThin);
   Borders.OlePropertySet("colorindex",xlAutomatic);
   WorkBook1.OleProcedure("Save"); //保存表格
   WorkBook1.OleProcedure("Close"); //关闭表格
   ExcelApp.OleFunction("Quit");    //退出Excel
   char StrCurDir[MAX_PATH+1];
   GetCurrentDirectory(MAX_PATH,StrCurDir);
   char exeStr[]="Export.xls";
   SHELLEXECUTEINFO Info;
   memset(&Info,0,sizeof(Info));
   Info.cbSize=sizeof(Info);
   Info.lpVerb="open";
   Info.lpFile=exeStr;
   Info.lpParameters=NULL;
   Info.fMask=SEE_MASK_NOCLOSEPROCESS;
   Info.nShow=SW_SHOWDEFAULT;
   if(!ShellExecuteEx (&Info))
       MessageBox(Handle,"打开Excel文件Export.xls失败！","信息提示",MB_OK);
   SetCurrentDirectory(StrCurDir);
}
//---------------------------------------------------------------------------




