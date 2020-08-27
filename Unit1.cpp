//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <vcl.h>
#pragma hdrstop
#include <Inifiles.hpp>

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{

  StringList = new TStringList();
  int ii,jj;
  int l,j1,j2,j3;

  /**************************************/
  DWORD nBufferLength=256;
  char  szStr1[256];

  AnsiString template_dir="C:\\temp\\";
  AnsiString template_name="mp_angle_head.tmp";
  AnsiString tempfileini,tmp,tmp0,tmp1="",tmp2="",tmp3="",tmp4,tmp5;

  tempfile = template_dir + template_name ;
  ZeroMemory( szStr1, nBufferLength);
  if (0!=GetTempPath(nBufferLength,szStr1)) {
    tempfile = AnsiString(szStr1) + template_name ;
  }
  //ShowMessage(tempfile);

  /**************************************/

  //Caption = Caption + " (" + DateTimeToStr(Now()) + ")";  // display the time on the form's caption
  //StatusBar1->SimpleText =DateTimeToStr(Now());

  EditL->Text="0.0";
  EditP->Text="0.0";
  CheckBox1->Checked=False;
  CheckBox1Click(NULL);

  ComboBox1->Clear();

  AnsiString inifile;
  inifile = ChangeFileExt(Application->ExeName,".ini");

   //ListBox1->Items
  if (FileExists(inifile)) {
   TIniFile* pif = new TIniFile(inifile);
   tempfileini="";
   if (pif->SectionExists("FILES")) {
     tempfileini=pif->ReadString("FILES", "file",tempfile);
   }
   if (tempfileini=="") { tempfileini=tempfile ; }
   tmp1=ExtractFilePath(tempfileini) ;
   //if (!DirectoryExists(tmp1)) ShowMessage("Папка " + tmp1 +" не существует");
   if (tmp1=="") { tempfileini=ExtractFilePath(Application->ExeName) + tempfileini ; }
   if (0==AnsiCompareText(tempfileini,tmp1)) { tempfileini=tempfileini+template_name; }
   tempfile = tempfileini ;

   if (pif->SectionExists("HEADS")) {
     pif->ReadSection("HEADS", ComboBox1->Items);
     StringList->AddStrings(ComboBox1->Items);
     ComboBox1->Clear();
     for (ii=1;ii<=StringList->Count;ii++)
     {
       tmp2=pif->ReadString("HEADS", StringList->Strings[ii-1],"");
       tmp2=tmp2.Trim();
       StringList->Strings[ii-1]=tmp2;
       jj=tmp2.Pos(",");
       if (jj!=0) {
         tmp2=tmp2.SubString(1,jj-1);
         tmp2=tmp2.Trim();
       }
       ComboBox1->Items->Add( tmp2 ) ;
       //ShowMessage(tmp2);
     }
   }

   delete pif;
  }

  if (0!=ParamCount()) {
    ; //ShowMessage(System::CmdLine); //LPTSTR GetCommandLine()
    tmp5 = ParamStr(1) ;
    if (tmp5=="") { tmp5=tempfile ; }
    tmp1=ExtractFilePath(tmp5) ;
    if (tmp1=="") { tmp5=ExtractFilePath(Application->ExeName) + tempfileini ; }
    if (0==AnsiCompareText(tmp5,tmp1)) { tmp5=tmp5+template_name; }
    tempfile = tmp5 ;
  }

  if (StringList->Count<=0) {
     StringList->Add("HSK-A63-AG90/NBS6-185 , 184.885 , 33");
     StringList->Add("HSK-A63-AG90/NBS6-215 , 214.906 , 33");
     StringList->Add("HSK-A63-AG90/NBS6-245 , 245.000 , 33");
     StringList->Add("HSK-A63-AG90/NBS6-275 , 274.959 , 33");
     ComboBox1->Clear();
     for (ii=1;ii<=StringList->Count;ii++)
     {
       tmp2=StringList->Strings[ii-1];
       tmp2=tmp2.Trim();
       StringList->Strings[ii-1]=tmp2;
       jj=tmp2.Pos(",");
       if (jj!=0) {
         tmp2=tmp2.SubString(1,jj-1);
         tmp2=tmp2.Trim();
       }
       ComboBox1->Items->Add( tmp2 ) ;
       //ShowMessage(tmp2);
     }
  }

  /********Чтение уже записанных данных*******/
  FILE *in;
  char str[80];
  if ((in = fopen(tempfile.c_str(), "r")) != NULL)
  {
     str[0]='\0';  fgets(str,80,in);  tmp=str;
     EditL->Text=tmp.Trim();
     str[0]='\0';  fgets(str,80,in);  tmp=str;
     EditP->Text=tmp.Trim();
     fclose(in);

     jj=-1; // индекс головки для первоначальн установки
     /*******поиск записанных=прочитанных значений --- для установки**************/
     for (ii=1;ii<=StringList->Count;ii++)
     {
       tmp0=StringList->Strings[ii-1];
       tmp0=tmp0.Trim();
       l=tmp0.Length();
       j3=tmp0.LastDelimiter(",");
       if (j3!=0) {
         tmp3=tmp0.SubString(j3+1,l-1);
         tmp3=tmp3.Trim();
         tmp0=tmp0.SubString(1,j3-1);
         tmp0=tmp0.Trim();
         l=tmp0.Length();
       }
       j2=tmp0.LastDelimiter(",");
       if (j2!=0) {
         tmp2=tmp0.SubString(j2+1,l-1);
         tmp2=tmp2.Trim();
         tmp0=tmp0.SubString(1,j2-1);
         tmp0=tmp0.Trim();
         l=tmp0.Length();
       }
       if (0==AnsiCompareText(tmp3,EditP->Text) &&
            0==AnsiCompareText(tmp2,EditL->Text) ) { jj=ii-1; break; }

       tmp1=tmp0;
     }

     if (jj<0) {
        //ComboBox1->ItemIndex = 0;
        //ComboBox1Change(NULL);
        CheckBox1->Checked=True;
        CheckBox1Click(NULL);
        return ;
     }

     ComboBox1->ItemIndex = jj;
     ComboBox1Change(NULL);
     return ;
  }
  /*********************/
  ComboBox1->ItemIndex = 0;
  ComboBox1Change(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
  AnsiString tmp;

  FILE *out;
  if ((out = fopen(tempfile.c_str(), "w")) == NULL)
   {
      tmp="Не могу открыть/создать файл = " + tempfile;
      Application->MessageBox(tmp.c_str(), "File Error", IDOK);
      return ;
   }
  tmp=EditL->Text;
  fprintf(out, "%s \n", tmp.c_str());
  tmp=EditP->Text;
  fprintf(out, "%s \n", tmp.c_str());
  fprintf(out, "0 \n");

  fflush(out);
  fclose(out);

/* 1.0
  int iFileHandle;
  int iLength;

  if (FileExists(tempfile)) {
    if (DeleteFile(tempfile)) {
      tmp="Не могу удалить(переписать) файл = " + tempfile;
      Application->MessageBox(tmp.c_str(), "File Error", IDOK);
      return ;
    }
  }

  iFileHandle = FileCreate(tempfile);
  if (iFileHandle<=0) {
     tmp="Не могу создать файл = " + tempfile;
     Application->MessageBox(tmp.c_str(), "File Error", IDOK);
     return ;
  }

  tmp=EditL->Text;
  iLength = tmp.Length();
  FileWrite(iFileHandle, (char*)&(tmp), iLength);

  FileWrite(iFileHandle, (char*)&("\n"), 2);

  tmp=EditP->Text;
  iLength = tmp.Length();
  FileWrite(iFileHandle, (char*)&(tmp), iLength);

  FileClose(iFileHandle);
*/

  Application->ProcessMessages();
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
   EditL->Enabled=(CheckBox1->Checked);
   EditP->Enabled=(CheckBox1->Checked);
   if (CheckBox1->Checked) {
     //ComboBox1->Items->Insert(0,"");
     ComboBox1->Color=clGray;
     ComboBox1->Font->Color=clGray;
     ComboBox1->ItemIndex=0;
     ComboBox1->Enabled=False;
    } else {
     //ComboBox1->Items->Delete(0);
     ComboBox1->Color=clWindow;
     ComboBox1->Font->Color=clWindowText;
     ComboBox1->ItemIndex=0;
     ComboBox1->Enabled=True;

     ComboBox1Change(NULL);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditLChange(TObject *Sender)
{
   //double tmp=0.0;
   //tmp= fabs( atof(EditL->Text.c_str()) );
   //EditL->Text=tmp;
   //EditL->Text=FloatToStr(Value);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditPChange(TObject *Sender)
{
///
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
   AnsiString tmp0,tmp1="",tmp2="",tmp3="";
   int l,j1,j2,j3;

   if (StringList->Count<=0) return ;
   if (CheckBox1->Checked) return ;

   ComboBox1->Hint=ComboBox1->Items->Text;
   tmp0=StringList->Strings[ComboBox1->ItemIndex];

   tmp0=tmp0.Trim();
   l=tmp0.Length();
   j3=tmp0.LastDelimiter(",");
   if (j3!=0) {
      tmp3=tmp0.SubString(j3+1,l-1);
      tmp3=tmp3.Trim();
      tmp0=tmp0.SubString(1,j3-1);
      tmp0=tmp0.Trim();
      l=tmp0.Length();
   }
   j2=tmp0.LastDelimiter(",");
   if (j2!=0) {
      tmp2=tmp0.SubString(j2+1,l-1);
      tmp2=tmp2.Trim();
      tmp0=tmp0.SubString(1,j2-1);
      tmp0=tmp0.Trim();
      l=tmp0.Length();
   }
   tmp1=tmp0;

   EditL->Text=tmp2;
   EditP->Text=tmp3;

   //ShowMessage(tmp0 + "\n" + tmp1  + "\n" + tmp2  + "\n" + tmp3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
try
{
   delete StringList;
}
__finally
{
  ;
}

}
//---------------------------------------------------------------------------

