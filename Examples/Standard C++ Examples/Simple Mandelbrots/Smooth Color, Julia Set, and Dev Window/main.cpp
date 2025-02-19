
// ****************************************************
// Smooth Color, Julia Set, and Dev Window (Mandelbrot)
// ****************************************************
//
// This set of Mandelbrot programs shows creating and displaying a Mandelbrot and/or Julia Set 
// in various stages, from very basic, to faster, all the way to an interactive program.
//
// This is #3 Smooth Color, Julia Set, and Dev Window
//
// ---------------------------
// Mandelbrot Example Projects
// ---------------------------
//
//  1. Basic Mandelbrot
//
//      This is a simple mandelbrot with no color gradation, so the colors change abruptly vs. the Smooth Color version that
//      has a nice color gradient throughout. 
// 
//      Either type can create some interesting mandelbrot displays and colors.
// 
//  2. Smooth Color, Julia Set, and Dev Window
//
//      This is a small change from the previous version, adding smooth coloring and the Julia
//      set to the output.   
//
//      The Dev Window is also used to show the ease-of-use of the Dev Controls without affecting your main code or window.
//
//      This version also adds some more power by assing z.sq() instead of z*z for a little faster
//      Mandelbrot calculation.  FastSetPixel() is also used in CSageBitmap to set the pixel much faster.
//
//  3. MandelBrot Interactive (zoom in/out, resize window, set Julia Set and more) 
//
//      This shows how to add Development-based controls to change values of the Mandelbrot
//      with just a few lines of code.
//
//      It shows using GetEvent() in the main eventloop to get and react to events. 
//  
// 
// -------------------------------------------------------------
// About This Version ( Smooth Color, Julia Set, and Dev Window)
// -------------------------------------------------------------
//
//  This verison adds smooth color to the mandelbrot and also adds the ability to display the Julia Set
//  (The Mandelbrot is special case of the Julia Set).
//
//  This version also adds a couple items to the DevWindow, showing how easy it is to use the Dev Controls
//  to add controls to your program without creating them or adding them to an existing window.
// 
//  You can set the Julia set and also the Julia Set paramaters from the Dev Window, and 
//  you can also use the Mouse Wheel on the edit boxes.
//
//  Run the program and look at the source code fore more information and comments.
//
//  I expect to do a video series on this set of programs and will explain more in-depth about the use of SageBox in
//  the videos.
//
// -------------------------------------------------------
// Using elements of this program for your own programming
// -------------------------------------------------------
//
// As with all SageBox demo code, it is written to show how SageBox works, but also general C/C++ programming practices.
// 
// ** Please feel free to use any of this code for your own personal purposes **
//
// SageBox is specifically written to be portable and used by example and the "intellisense" comments that come up for each function.
// Most SageBox code snippets are also portable and can be taken from one program and used in another.
//
// SageBox is free for personal use. 
//

#include "SageBox.h"

using namespace Sage::kw;      // Sagebox keyword options

// Color Table based on Wikipedia Mandelbrot Colors

static double rgbTable[17][3] = 
{
    { 0, 0, 0       }, { 25, 7, 26     }, { 9, 1, 47      }, { 4, 4, 73      }, 
    { 0, 7, 100     }, { 12, 44, 138   }, { 24, 82, 177   }, { 57, 125, 209  },
    { 134, 181, 229 }, { 211, 236, 248 }, { 241, 233, 191 }, { 248, 201, 95  },
    { 255, 170, 0   }, { 204, 128, 0   }, { 153, 87, 0    }, { 106, 52, 3    },
    { 106, 52, 3    }, // Added one more to make CreatColorTable()'s job easy
}; 

// -----------
// New Globals
// -----------
//
// These were originally in the main() function and now are *temporary* globals.
//
// This version is used to show the Dev Window and main event loop, so creating a class and changing 
// the structure of the code would have made it harder to see the additions, which is why these were moved
// to globals rather than being put into a class. 
//
// The next version will move these, main() and DrawMandelbrot() into a CMandelbrot class.
// The values below will then be proper class members.

static constexpr int colorTableSize = 16384;       // Color Table Size (must be divisible by 16)
RGBColor_t rgbColorTable[colorTableSize];          // Color Table derieved from rgbColors

CWindow     * win          = nullptr;              // Our main window.  
CfPoint       winSize      = { 1400, 900 };         // Initial Window Size
 
// Initial Mandelbrot Values (Center, Range and Iterations)

CComplex    pJulia         = { -.4, .6 };          // Initial Julia values (when bJuliaSet == true)
CfPoint     centerJulia   = { 0, 0 };             // Initial Julia Set Center
CfPoint     centerMandel  = { -.6, 0 };           // Initial Mandelbrot Center
double      range          = 3.7;                  // Initial Range (i.e. "zoom" factor)
int         iMaxIterJulia   = 200;                  // Max Julia Set Iterations
int         iMaxIterMandel  = 50;                   // Max Mandelbrot Iterations
bool        bJuliaSet       = false;

// CreateColorTable() -- Creates a smooth n-sized color table based on the 16-color rgbTable. 
// 
// The output table *kTableSize) must be divisible by 16. 
//
// Note the addition of a 17th element in the rgbTable that is a repeat of the
// previous color -- this is so we can index the next color. This allows the writing of a 'dumb'
// routine.  It also means the last segment is a solid color, but with the Mandelbrot, the edge color
// usually is only seen as an outline, so it shouldn't make too much of a difference in the output.
//
// It might be nice to put another color in its place, but I wouldn't know what color to add, so
// I just repeated the previous color.
//
void CreateColorTable()
{
    for (int i=0;i<16;i++)
    { 
        for (int j=0;j<colorTableSize/16;j++)
        {
            double fPercent = 16*(double) j/colorTableSize;

            // Use a lambda function so we can just repeat it for each color (Red, Green, Blue) 

            auto newColor = [&](int c) { return (int) ((rgbTable[i][c])*(1.0-fPercent) + (rgbTable[i][c+3])*fPercent); };
            rgbColorTable[colorTableSize*i/16 + j] = { newColor(0), newColor(1), newColor(2) }; 
        }
    }
}

// DrawMandelbrot() -- Draw the mandelbrot (or Julia Set)
//
// This was in main() but is now a separate function.  This allows it to be called from anywhere now. 
// In this version, it is called once initially and then in reaction to the "Julia Set" checkbox. 
//
// This version features smooth coloring using the color table created.  In the previous version, the 
// color was established by performing "color % 16".  With smooth color, we can't really do that any longer,
// and calculate the color differently.
//
// A log() function is used to calculate a gradient between the iteration values, without which we would have the 
// same blocky colors as before.  See the code below
//
// The code for this is easily found on the internet.  The code on the internet is usually in the form of
// something like log(x)/log2(), which I changed to a singular log2(x), which is the same value.
//
void DrawMandelbrot()
{
    // Set the center and iterations dependent on Julia Set or Mandelbrot, since they are different.
    // In the next version, these will disappear because the iterations and center will be user-controlled
    // via an Iterations edit box and clicking on the screen to center.
    
    CfPoint center    = bJuliaSet ? centerJulia : centerMandel;
    int maxIter        = bJuliaSet ? iMaxIterJulia : iMaxIterMandel; 

    CfPoint pRange     = { range, range*winSize.y/winSize.x };   // Range based on X-axis

    CfPoint cfD         = pRange/winSize;                           // Unit Increment for each pixel
    CfPoint cfStart     = center - cfD*winSize/2;                   // Upper-left X,Y position to start 

    // print out some information about the mandelbrot.  This will be more useful in the next version

    SageDebug::printf("---\nIterations{x=70}= {y}%d{}\n",maxIter); 
    SageDebug::printf("Julia Set{x=70}= {y}%s\n",BoolStringU(bJuliaSet));   // Sage::BoolStringU() prints "Yes" or "No"
    SageDebug::printf("Center{x=70}= {c}(%g,%g)\n",center.x,center.y);      
    SageDebug::printf("Range{x=70}= {c}%.14lg\n",range);                    // Range can get very small (see next version of code)

    SageTimer timer; // Measure the time we're taking, just to get the data

    for (int i=0;i<winSize.y;i++)
    {
        double fy = (double) i*cfD.y + cfStart.y;
        for (int j=0;j<winSize.x;j++)
        {
            double fx = (double) j*cfD.x + cfStart.x;

            int iter = 0;

            // For Julia Set, set z = { fx, fy } and c to a static value, such as (.285, 0) or (-4.,.6) (and set center to (0,0);

            CComplex dz = { 1,0 };
            CComplex c  = { fx, fy };    
            CComplex z  = c;

            if (bJuliaSet) c = pJulia; 

            while (z.absSq() < 65536 && iter++ < maxIter-1) z = z.sq() + c; 

            RgbColor rgbOut{};    // Get RGB(0,0,0) for color when we gone past maximum iterations. Can also set to Rgb("Black"), Rgb(PanColor::Black), etc.
  
            if (iter < maxIter)
            {
                // Generate Smooth color by giving a gradient from one iteration to the next

                double log    = log2(log2(z.absSq())/2);          // still not sure of I should perform abs(z) or abs(x)^2
                double fIter = (((double) iter)+1.0 - log);   // Add the value to the iteration
                
                fIter /= ((double) maxIter-1);                 // Normalize it so we can perform a sqrt() or pow() function 

                // Note the sqrt() -- this simply brightens the result.  Results for Mandelbrots tend to be very small, with 
                // higher iterations as it nears the edge.   Performing sqrt() brightens up the lower areas allowing 
                // more color to be seen.
                //
                // In the next version, this will change the a pow(fIter,<some value>) for more control.

                auto index = (int) ((double)(colorTableSize-1)*sqrt(fIter));   // Get the index into the rgbTable

                rgbOut = rgbColorTable[index];
            }
            win->SetPixel(j,i,rgbOut);
        }
    }

    auto elapsedMs = (int) timer.ElapsedMs();    // get the end ms
    SageDebug::printf("Time{x=70}= {g}%d ms.\n",elapsedMs); 
}

int main()
{
    win = &Sagebox::NewWindow("SageBox: Mandelbrot Smooth Color and 3-D Depth",kw::SetSize(winSize));



    CreateColorTable();     // Initialize Color Table used for smooth coloring
    DrawMandelbrot();       // Draw initial Mandelbrot before entering event loop

    // Add a "Quick Dev Control" Checkbox.  This will grow in the next version
    // Dev Controls are used for quick controls that can be used for prototyping,
    // so that the controls don't need to go in the main window. 
    
    // These controls are automatically placed in a window created by the 
    // Devcontrols. 

    auto& checkboxJulia    = win->DevCheckbox("Julia Set"); 

    // Set up Edit Boxes for Julia Real and Imaginary components. 
    // The Default is set here, but inputbox::SetText() can be used to 
    // set the displayed value or string at any time.
    //
    // See the next version of this program for an example.

    auto& inputboxReal      = win->DevInputBox("Julia Real",Default(pJulia.real)); 
    auto& inputboxImag      = win->DevInputBox("Julia Imag",Default(pJulia.imag)); 

    // Enter the main "event loop", which is just a loop to get events
    // (i.e. mouse movements, button presses, etc.). 
    //
    // This loop is inactive (i.e. the thread is dead) until an event is received. 
    // Then important events can be checked. 
    //
    // This loop continues until the window is set to close.  Just press the 'X' on the window to exit the loop
    // and close the program.  We don't need the ExitButton() call from the previous version since this waits
    // for the window closure. 
    //
    // This will grow in the next version to include many more events, some with the Dev Window,
    // and some with the main Mandelbrot window.

    while(Sagebox::GetEvent())
    {
      bool redraw = false;

      // Check for events in which we're interested. 

       redraw |= checkboxJulia.Pressed(bJuliaSet);
       redraw |= inputboxReal.ReturnPressed() | inputboxImag.ReturnPressed();  // i.e. if <CR> key pressed on either box.
            
        if (redraw) 
        {
            // Get the values for Real and Imag edit boxes here.  This allows both boxes to be 
            // filled even though we're only seeing the Return Key event on the one  
            // actually pressed. 

            pJulia = { inputboxReal.GetFloat(), inputboxImag.GetFloat() }; // Convert display values to double values
            DrawMandelbrot();
        }
    }
    return 0;
}