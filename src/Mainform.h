//---------------------------------------------------------------------------

#ifndef MainformH
#define MainformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <complex.h>

using namespace std;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TOpenDialog *OpenDialog1;
        TBitBtn *BitBtn4;
        TListBox *ListBox1;
        TBitBtn *BitBtn5;
        TListBox *ListBox2;
        TSaveDialog *SaveDialog1;
        TListBox *ListBox3;
        TListBox *ListBox4;
        TBitBtn *BitBtn10;
        TBitBtn *BitBtn11;
        TEdit *Edit1;
        TEdit *Edit2;
        TBitBtn *BitBtn7;
        TBitBtn *BitBtn8;
        TBitBtn *BitBtn9;
        TLabel *Label1;
        TEdit *Edit3;
        TBitBtn *BitBtn12;
        TBitBtn *BitBtn13;
        TBitBtn *BitBtn14;
        TBitBtn *BitBtn15;
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall BitBtn7Click(TObject *Sender);
        void __fastcall BitBtn8Click(TObject *Sender);
        void __fastcall BitBtn9Click(TObject *Sender);
        void __fastcall BitBtn10Click(TObject *Sender);
        void __fastcall BitBtn11Click(TObject *Sender);
        void __fastcall BitBtn12Click(TObject *Sender);
        void __fastcall BitBtn13Click(TObject *Sender);
        void __fastcall BitBtn14Click(TObject *Sender);
        void __fastcall BitBtn15Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void __fastcall NoiseFill (Byte **fill, int w , int h , int zft[256]);
        void __fastcall ComputeZFT (Byte **fill, int w , int h , int zft[256],Byte **back);
        int __fastcall GetAllInput(Byte **fill, int w , int h , int **cin, int total, int horilen , int length,Byte **black);
        bool __fastcall GetLShapeOutput(Byte **fill, int w , int h , int x, int y , int *recieve , int length);
        int __fastcall Compare(int *input, int *output , int length );
        void __fastcall ByteToBmp(Graphics::TBitmap *pic , Byte **temp);
        void __fastcall BmpToByte(Graphics::TBitmap* bmp, Byte **src );
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
