//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TStaticText *StaticText1;
        TComboBox *ComboBox1;
        TBitBtn *BitBtn1;
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *EditL;
        TEdit *EditP;
        TStatusBar *StatusBar1;
        TLabel *Label3;
        TLabel *Label4;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall EditLChange(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall EditPChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        AnsiString tempfile;
        TStrings *StringList;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
