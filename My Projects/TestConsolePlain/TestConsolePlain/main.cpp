// ------------------------
// TestConsolePlain Project
// -------------------------
//
// This is an example of adding Sagebox to a Console Mode (main()) or Windows (WinMain) 
// project ad-hoc style, without formalizing it as a project. 
//
// This file started as a raw Win32 Console Mode project from the default Visual Studio Template.  Sagebox is
// added in below, showing how it can be adde to any existing project (Console or Windows)
//
// The "CSagebox.h.h" file is included, and then the library needed is specified with the $pragma. 
// 
// That is all that is needed to bring int the environment.
//
// *** Note: The current .lib path is for Win32/x32.  For x64, simply replace "x32" with "x64" below. 
//           This project should come up as an x32 project by default, but supports both x32 and x64.
//           If you get a "conflict" message, then either change the project to x32 or change "x32" to "x64"
//           below.


// Change paths to where Sagebox is located (the paths used are the ones on my system)

#include "c:\SageBox\git\include\Sagebox.h"
#include <iostream>

#pragma comment(lib,"c:\\Sagebox\\git\\lib\\debug\\x32\\Sagebox.lib")

int main()
{
   auto& cWin = Sagebox::NewWindow();

    cWin.FillCircle(400,400,150,PanColor::Red);

    int iValue = cWin.GetInteger("Enter a Number...",kw::Range(50,500));
    
    std::cout << "Number is " << iValue << "\n";

    return cWin.ExitButton();   // ExitButton() returns 0

}