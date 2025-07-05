//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include <FMX.Platform.Win.hpp>
#include <ShellAPI.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include <iostream>


#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

#if _WIN64
#define Parm GWLP_WNDPROC
#elif _WIN32
#define Parm GWL_WNDPROC
#elif __APPLE__
  // macOS
#else
  #error Not a supported platform
#endif

std::vector <AnsiString>filenames;

TForm1 *Form1;

HWND m_Hwnd;
WNDPROC m_WndOldProc;
long m_OldWndProc;



LRESULT CALLBACK WndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	int count;
	int i;
	char fname[256];
	TRect f;


	if(Msg == WM_DROPFILES){

		const unsigned int count = DragQueryFile((HDROP)wParam,-1,NULL,0);
		for( unsigned int i = 0 ; i < count ; ++i ){
				const unsigned int length = DragQueryFile((HDROP)wParam, i, NULL, 0);
				std::unique_ptr<wchar_t[]> filename(new wchar_t[length + 1]);

				DragQueryFile((HDROP)wParam, i, filename.get(), length + 1);
				UnicodeString temp;
				temp = filename.get();
				Form1->Memo1->Lines->Add(temp);
		}

	}


	return CallWindowProc(m_WndOldProc,hWnd,Msg,wParam,lParam);

}

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{


	m_Hwnd = Fmx::Platform::Win::FormToHWND(Form1);
	DragAcceptFiles(m_Hwnd,true);

	m_OldWndProc = GetWindowLongPtr(m_Hwnd, Parm);
	m_WndOldProc = (WNDPROC)SetWindowLongPtr(m_Hwnd, Parm,(LONG)WndProc);

}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	SetWindowLongPtr(m_Hwnd,Parm,m_OldWndProc);
}
