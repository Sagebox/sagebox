

// --------------------------------------
// Sagebox  Basic Input Examples
// --------------------------------------
//
// This example shows using the Sagebox::Conio class to get input from the console-mode window with validation.
//
// In this case, the GetInteger() functions will prompt with the message given.  If the range is not between
// RadiusMin and RadiusMax, then a message will be printed letting the user know of the range.   This will continue
// until the user presses Return on a blank line or enters a value within the range.
//
// If the user presses Return with a blank line, three things can happen
//
// 1. if there is no default and no prohibition on entering blank lines (see #2 and #3), either 0 is returned or the
//    the minimum value (when kw::Range() or kw::Min() is applied).
// 2. If kw::NoBlanks() is specified, then the function will tell the user a valid number must be entered.
// 3. If kw::Default() (i.e. Default(200)) is specified, the default will be returned, even if it is not within the Range
//    specified with kw::Range() (or Min()/Max()). 
//
//    This allows unique numbers to be given as a signal to quit, such as:
//
//    int iMyValue = Sagebox::conio.GetInteger("Enter a number from 100 to 200 (or blank line to quit): ",Range(100,200) : Default(0)); 
//
//    In this case, if iMyValue returns 0, this means the user pressed return on a blank line.
//
//    Optionally, NoBlanks() can be used instead of Default(), which causes the functon to continue to print error messages until the user
//    enters a value within the 100-200 range (or presses ^C) to quit the program.


#include "Sagebox.h"

int main()
{   
                           
    constexpr int RadiusMin    = 1;
    constexpr int RadiusMax    = 400;

    RgbColor rgbColor = Rgb("Red");         // We can also use PanColor::Red, SageColor;:Red, {255,0,0} or Windows COLORREF RGB(255,0,0)                                            // We can also use a standard Windows COLORREF RGB(255,0,0); 
    
    // Use the window's console i/o versions for input/output.  If the user closes the window the entry falls through with 
    // a return of 0 -- if the user presses control-C, the program exits as in a console program (this can be turned off).

    // Here, we are using win.console.GetInteger(), which is what "win >> Radius2" below eventually calls. 
    // In this case, we specify "kw::Range" (we do not need to the kw:: part, but typing kw:: will cause the editor to show all options)
    // to tell the GetInteger() routine the acceptable range, and to bring up a message box.
   
    int Radius1 = Sagebox::conio.GetInteger("Enter Radius 1: ",kw::Range(RadiusMin,RadiusMax));
    int Radius2 = Sagebox::conio.GetInteger("Enter Radius 2: ",kw::Range(RadiusMin,RadiusMax));

    // If we get here, then we know we have some good values. 

    auto &win = SageBox::NewWindow();    // It's a simple app so we can just create Sagebox and the Window together.
    win.FillEllipse(400,400,Radius1,Radius2,rgbColor); 

    return win.ExitButton();    // Now we really need an ExitButton() or WaitforClose() (or some method to pause exit),
                                // because the program is done, and everything will close on exit. 
}
 