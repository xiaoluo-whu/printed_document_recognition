#ifndef funH
#define funH
#include <complex.h>
#include <Graphics.hpp>
using namespace std;
//---------------------------------------------------------------------------
void TextureCal3333(int **Glcm, int h, float * FEA);
void FFT(complex<float> *Array,complex<float> *FArray,int r);
void MatrixToDih(int **Matrix,int w,int h,int * dih,int length);
void MatrixToBmp(Graphics::TBitmap * temp,int *Matrix);

void BmpToMatrix(Graphics::TBitmap * temp,int **Matrix);
void BmpToMatrix(Graphics::TBitmap * temp,int *Matrix);
void MatrixToBmp(Graphics::TBitmap * temp,int **Matrix);

int IterativeSeg(int * Dih,int length);                          //直方图迭代分割基于迭代的阈值
//能区分出图像的前景和背景的主要区域所在，但在图像的细微处（如图1中的浅色线条）还没有很好的区分度。

void Segbmp(Graphics::TBitmap * temp,int trd);

void _fastcall Erosion(Graphics::TBitmap * temp1);
void _fastcall Dilation(Graphics::TBitmap * temp1);

void _fastcall Erosion(Graphics::TBitmap * temp1);
void _fastcall Dilation(Graphics::TBitmap * temp1);

void BmpToMatrixGray(Graphics::TBitmap * temp,int **Matrix);


void FFTBmpfeaN(Graphics::TBitmap* temp,  float * FEA);
void GlcmMatrixTexture(int **Gray1, int h,int dist, double * FEA);
void GlcmBmpTexture(Graphics::TBitmap* temp,  float * FEA);
void GlcmMatrixTexture4(int **Gray1, int h,int dist, float * FEA);
void GlcmBmpTexture4(Graphics::TBitmap* temp,  float * FEA);
void MatrixGlcm(int ** Gray,int **Glcm,int h,int w,int dist, int dir, int GrayLevel);
void TextureCal1(int **Glcm, int h, double * FEA);
#endif

 