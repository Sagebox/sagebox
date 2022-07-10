
#include "Pendulum.h"

using namespace Sage::opt;  // Sagebox options/keywords

// Sagebox Interactive Pendulum with Dev Controls.  Programming with Sagebox Demonstration Program.
//
// Also See: Double Pendulum (Simple Version)           -- Only calculates and renders the double pendulum, with no event handling.
//           Interactive Pendulum                       -- Adds the ability to start and stop the pendulum, move the boobs around,
//                                                         Zoom in and out with the MouseWheel, and set the Peg position with the mouse.
//
// ----------------------------------------------
// Sagebox Interactive Pendulum with Dev Controls
// ----------------------------------------------
// 
// In this version, a QuickForm Window is created to merge a regular window and Dev Controls window into one nice package. 
// With the Dev Controls, many elements of the pendulum can be changed in real-time. 
// 
// Instruction Display
// 
//      An Instruction Text display is added which disappears when the bobs are moved with the mouse.
//      As with the previous interactive version, the Bobs are set at a low angular velocity so they can be moved and then dropped.
//      
//      On average, for each control, there are two lines of code added: 1 line to create the control, and another line of code to 
//      check if the control has
//      changed (via an event) and use its value. 
//      
//      The Dev controls window adds a number of options:
// 
//          1. Mass Input Boxes     - There are now two input boxes where the mass of each bob can be set independently.
//          2. Dampen Slider        - This sets the dampening (friction) on the slider and can be changed while the pendulum, is moving.
//                                    note: lower settings (close to 0) can lead to math overruns, which are detected by the algorithm and shut down.
//          3. Zoom Slider          - Moves the Zoom in and out.  The MouseWheel can also be used for this. 
//          4. Pendulum Size        - Sets the size of the pendulum bobs.   This can be nice for zooming in and out.  Does not affect mass or length 
//                                    of the bobs and only changes the display size.
//          5. Rod Thickness        - Sets the thickness of the rods as displayed.  Does not change the mass or length of the rods and is only used for 
//                                    display. These show a good example of using Radio Buttons in Sagebox
//          6. Display Values       - Checkbox to display or disable display of realtime values (angle, mass, etc.)
//          7. Maintain Rod Length  - Checkbox to set static rod length.  When checked, the rod length will not change when moving the pendulum bobs with 
//                                    the mouse
//          8. Show Trail           - Checkbox to show or hide the Trails displayed on the bottom pendulum
//          9. Single Pendulum      - Checkox to set a Single Pendulum.  When checked, only the top pendulum is displayed.  The second Bob and its rod is
//                                    hidden, and the mass of the second bob is set to 0 -- the top pendulum runs as a single pendulum until "Single Pendulum" 
//                                    is unchcked and the mass of second bob returns.
//          10. Show Timing         - Checkbox to show real time information on program execution.  When checked, this shows the milliseconds for each loop
//                                    to calculate and draw the pendulum
//          11. Start,Stop,Drop     - This is a button that changes text depending on the state.   See the SetTextButton() function calls.
//          12. Quit Button         - This can be pressed to stop the program.  The Window can also simply be closed.
//      
//      Functions Added:
// 
//      SetStartButtonText()    - Sets the text of the Start/Stop button to "Start" or "Stop", depending on the mode.
//      HandleEvents()          - This function is not added, but the code to handle the Dev Controls has been added.
// 
// -----------------------------------------------------------------
// Added with the Sagebox Interactive Pendulum (and in this version)
// -----------------------------------------------------------------
// 
//      This version adds to the Sagebox Double Pendulum (Simple Version) by adding the following interactive control:
// 
//          1. The pendulum can be stoped and then moved by clicking on the window.
//          2. The bobs can be moved anywhere and the lengths of the rods will changed as you move the bobs.
//          3. After moving the bobs click anywhere on the screen (not on a bob) to drop the pendulum.
//          4. Zoom Control.  You can use the MouseWheel to zoom in and out, enlarging or reducing the zoom of the pendulum 
//                            The size and mass of the pendulum stay the same, as only the zoom changes.
//          5. Moving the Pedulum Up and Down.  Clicking on the Window with the Right Mouse Button will set Y position of the Peg holding up the
//                                      Pendulum.  You can then move the mouse up and down to set the high or low point of the pendulum peg.
// 
//      Functions Added
// 
//          HandleEvents()          - This handles the events occuring such as mouse clicks, mouse moving, Mouse Wheel, etc.
//          HandleMouseClick()      - This handles the mouse click to stop the pendulum moving and determine if a pendulum bob is being moved.
//          HandleMouseDrag()       - Handles the case where the mouse button is down and moving and a bob is being dragged.
//                                    This sets the new position of the pendulum bobs and their rod lengths.
//          DisplayInstructions()   - Displays instructions on how to use the mouse to move the bobs, etc. Disappears when the Mouse clicks on the window
// 
// ----------------------------------
// Sagebox Double Pendulum In General
// ----------------------------------
// 
//      This double pendulum program shows a double pendulum moving in realtime using Sagebox functions. 
//
//      The window is created in the main() function and passed to the DPenulum class where it calculates and draws the pendulum.
//
//      In this version, the pendulum runs until it stops. 
//
//      The Init() function can be changed to change the bob masses, lengths, dampening and other aspects. 
//
//      The code itself is very short, using just a few GUI calls through Sagebox (at least if you don't count the DisplayValues() function
//      that prints real-time information to the window.
//
// ---------------
// Vertical Resync
// ---------------
// 
//      This program works by waiting for the vertical resync, then drawing the pendulum and updating the window. 
//      The opt::RealTime() setting enables the high resolution timer and sets other configgurations to allow better
//      real-time display
// 
// --------------
// Pendulum Trail
// --------------
//
//      The program shows a fading trail of where the second bob swings around.  This can be disabled by setting m_bShowTrail in 
//      DPendulum to false.
//
// --------------
// Timing Display 
// --------------
//
//      When bShowTiming is set to True, the time for each loop is displayed in the Sagebox Process Window, showing the milliseconds
//      taken to calculate and draw the pendulum.
//
// --------------
// Python Version
// --------------
//
// See the Python versions of the double pendulum.
//
int main()
{
    bool bKeepRodLength         = false;    // Keeps the rod lengths static when moving the pendulum bobs.
    bool bDisplayValues         = true;     // Display current values to the window on/off
    bool bDisplayInstructions   = true;     // Display instructions.  Set to off as soon as one of the bobs is moved by the user.
    bool bShowTiming            = false;    // Display real-time timing information in ms 
    bool bQuitProgram           = false;    // Quit button sets it to true and exits.  Window can also just be closed. 

    auto qForm  = Sagebox::QuickForm("label=' Sagebox C++ Double Pendulum Example'",opt::RealTime() | opt::SageIcon());
 
    // Get the window and dev control window.  We can use "auto [ cWin, cDev ] = QuickForm().GetWins(), but then it fails
    // to work in the lambdas below, so they are assigned separately here. 
    //
    // qForm.Win and qForm.Dev can be used, but getting separate values helps make the code shorter and more readable. 

    auto& cWin = qForm.cWin;        // Get Main Canvas Window. 
    auto& cDev = qForm.cDev;        // Get Dev Controls Window

    // Initialize the pendulum with the Sagebox Window, as well as the Rod Lengths, Mass1 & Mass2, dampening
    // and starting angles (in degrees)

    DoublePendulum pend(cWin,220, 185, 10.0, 10.0, -15.0, -15.0, .9985, .33);

    // Build Dev Controls

    cDev.AddText("Pendulum Controls",Font(17) | TextColor("Cyan"));     // Simple text

    // The defaults set are in inline with the defaults for the pendulum.  Some of them hard-coded here for easier coding and 
    // readability when they would otherwse (i.e. not a demo) certain function would call get/set functions in the Pendulum class to separate 
    // (i.e. decouple) context and data.  

    auto& cWeight1          = cDev.AddEditBox("Weight 1",Default(pend.m_fMass[0])); 
    auto& cWeight2          = cDev.AddEditBox("Weight 2",Default(pend.m_fMass[1]));  

    auto& cDampen           = cDev.AddSlider("Dampen Multiplier",Default(15));  // consistent with .9985 default value
    auto& cZoom             = cDev.AddSlider("Zoom",Range(25,150) | Default(100)); 
    auto& cPendSize         = cDev.AddSlider("Pendulum Size",Default(15)); 

    // Add 3 radio buttons.  Horz() puts them horizontally, otherwise they are put vertically. opt::Columns() can specifiy specific # of columns

    auto cThickLines        = cDev.AddRadioButtons("Normal\nThick\nThicker",opt::Horz() | opt::Title("Rod Thickness")); 

    auto& cDisplay          = cDev.AddCheckbox("Display Values",Default(bDisplayValues));           // "-" puts it on next line but closer to last checkbox
    auto& cStaticLength     = cDev.AddCheckbox("-Maintain Rod Length"); 
    auto& cShowTrail        = cDev.AddCheckbox("+Show Trail",Default(pend.m_bShowTrail));           // "+" to add checkbox on same line as last
    auto& cSinglePendulum   = cDev.AddCheckbox("-Single Pendulum",Default(pend.m_bSinglePend));   
    auto& cShowTiming       = cDev.AddCheckbox("-Show Timing",Default(bShowTiming));   
    auto& cButtonStart      = cDev.AddButton("   Stop   ");                                 // Use spaces to pad the display so it is wider. 
    auto& cQuitButton       = cDev.AddButton("+   Quit   ");                                // "+" to add button on same line as last


    int iDragging = 0;                          // > 0 when we're dragging a pendulum bob
    double fThickness[3] = { 1.0, 2.0, 3.0 };   // Thickness for the Rod Thickness radio box

    auto szWinSize = cWin.GetWindowSize();      // Used when we move the display offset up/down with right mouse button
    
    // Set the start button text based on whether or not we're paused. 
    //
    auto SetStartButtonText = [&](bool bPause) { bPause ? cButtonStart.SetText("   Start   ") : cButtonStart.SetText("   Stop   "); };
    

    // If the mouse was clicked, Pause or Unpause the display.  Also set a dragging indicator if the user pressed on 
    // one of the pendulum bobs. 
    //
    auto HandleMouseClick = [&]()
    {
        bDisplayInstructions = false;
        bool bForceHold = !pend.m_bPause;
  
        CfPoint pPos = cWin.GetMousePos(); 
        double fRadius1 = pend.fTopRadius*pend.m_fZoom*pend.m_fCircleMult;
        double fRadius2 = pend.fBotRadius*pend.m_fZoom*pend.m_fCircleMult;

        iDragging = 0;

        if (pPos.WithinRect(pend.m_RodVertex[1]-fRadius1,pend.m_RodVertex[1] + fRadius1)) iDragging = 1;
        if (pPos.WithinRect(pend.m_RodVertex[2]-fRadius2,pend.m_RodVertex[2] + fRadius2)) iDragging = 2;

        if (!iDragging) SetStartButtonText(pend.m_bPause = bForceHold);
        else
        {
            pend.m_bPause = true;
            cButtonStart.SetText("   Drop!   "); 
        }
    };

    // Handle when the mouse is down and moving and we're dragging a pendulum.
    //
    auto HandleMouseDrag = [&]()
    {
        auto pDelta =  (CfPoint) cWin.GetMousePos()-pend.m_RodVertex[iDragging-1];
        pend.m_fAngle[iDragging-1] = std::atan2(pDelta.x,pDelta.y);

        if (!bKeepRodLength) 
            pend.m_fLength[iDragging - 1] = std::sqrt(pDelta.x*pDelta.x + pDelta.y*pDelta.y)/pend.m_fZoom;
        pend.Reset(); 
    };

    // Handle events occuring in the Controls, such as checkboxes, setting the mass, line thickness, etc.

    auto HandleEvents = [&]()
    {
        if (cWin.MouseClicked()) HandleMouseClick();
        if (cWin.MouseDragEvent() && iDragging) HandleMouseDrag();

        cStaticLength.Pressed(bKeepRodLength);              // Set KeepRodLength if checkbox pressed
        cDisplay.Pressed(bDisplayValues);                   // Display real-time values (angle, mass, etc.)
        cSinglePendulum.Pressed(pend.m_bSinglePend);        // Single Pendulum on/off
        cShowTrail.Pressed(pend.m_bShowTrail);              // Show bottom pendulum trail

        if (cQuitButton.Pressed()) bQuitProgram = true;

        if (cShowTiming.Pressed()) Sagebox::ShowProcessWindow(bShowTiming = !bShowTiming,true);     // Turn process/debug window & reporting on/off

        // Check various sliders and set new values if the position has changed.

        if (auto fDamp = cDampen.Moved())     { pend.m_fDamp1       = pend.m_fDamp2 = 1-(double)*fDamp/10000; if (!*fDamp) pend.ResetOverflow(); }
        if (auto iSize = cPendSize.Moved())     pend.m_fCircleMult  = (double) *iSize/100*5 + .25;
        if (auto iZoom = cZoom.Moved())         pend.m_fZoom        = (double) *iZoom/100.0; 
            
        // If the radio buttons have changed, then set the new thickness value. 

        if (auto iSize = cThickLines.Pressed()) pend.m_fThickMul    = fThickness[*iSize];

        double fValue;
    
        if (cWeight1.ReturnPressed(fValue) && fValue > 0) pend.m_fMass[0] = fValue;
        if (cWeight2.ReturnPressed(fValue)) pend.m_fMass[1] = fValue;

        if (auto iDir = cWin.MouseWheelMoved()) iDir < 0 ? pend.m_fZoom *= .95 : pend.m_fZoom *= 1/.95; 

        if (cWin.MouseRButtonDown()) pend.m_RodVertex[0].y = cWin.GetMousePos().y;
        if (cButtonStart.Pressed()) SetStartButtonText(pend.m_bPause = !pend.m_bPause);

    };

    // Display instruction in the window
    //
    auto DisplayInstructions = [&]()
    {
        cWin.SetFgColor("Gray192");         // Set the text color

        // {y} = yellow, {g} = green. {30} = set font to size 30pt

        cWin.SetWriteIndent(300);   // Set the newline position for each line 
        cWin.SetWritePos(300,50); 
        cWin.Write("{30}{y}Double Pendulum\n"); 
        cWin.Write("\nClick on either or both pendulums to set position.\n"); 
        cWin.Write("Click on the screen (or press {g}\"Drop\"{/}) to drop the pendulum.\n\n");
        cWin.Write("Click on {g}\"Maintain Rod Length\"{/} to change pendulum angles without changing the rod length.\n\n");
        cWin.Write("Use the controls to the left to change states while pendulum is in motion or before dropping the pendulum.\n\n");
        cWin.Write("{p}While the pendulum is moving\n\n"); 
        cWin.Write("Right-click on the screen to move the display area up and down\n"); 
        cWin.Write("Use the Mouse Wheel to zoom in and out\n"); 
        cWin.Write("Click on the display area to stop the pendulums so you can move them.");            
        cWin.SetWriteIndent(0);     // Set newline indent back to 0

    };

    // Display relevant pendulum values out to the window. 
    //
    auto DisplayValues = [&]()
    {
        cWin.SetWriteIndent(10);    // Set leftmost column at 10 pixels out we can just use \n without setting it for each line.
        cWin.SetWritePadding(5);    // Add some space between lines when writing to the window (for nicer display)

        cWin.SetFgColor(SageColor::Gray172);         // Set the text color.  Also can be simpl "Gray172" in quotes.

        // {g}  green, {c} = cyan.  {x=130) sets the X write position to that value so things line up

        cWin.SetWritePos(10,10); 

        cWin.printf("Mass 1:{x=130}{g}%.2f\n",          (float) pend.m_fMass[0]); 
        cWin.printf("Mass 2:{x=130}{g}%.2f\n",          (float) pend.m_fMass[1]); 
        cWin.printf("Length 1:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[0])); 
        cWin.printf("Length 2:{x=130}{g}%.2f\n",        (float) (pend.m_fLength[1])); 
        cWin.printf("Dampening:{x=130}{g}%g\n\n",       (float) (1.0-(pend.m_fDamp1*pend.m_fOverflowMul))); 
        cWin.printf("Ang Accel 1:{x=130}{c}%f\n",       (float) pend.m_fAngAccel1); 
        cWin.printf("Ang Accel 2:{x=130}{c}%f\n",       (float) pend.m_fAngAccel2); 
        cWin.printf("Ang Velocity 1:{x=130}{c}%f\n",    (float) pend.m_fAngVel1); 
        cWin.printf("Ang Velocity 2:{x=130}{c}%f\n",    (float) pend.m_fAngVel2); 
        cWin.printf("Angle 1:{x=130}{c}%.2f\xb0\n",     (float) (pend.m_fAngle[0]*180.0/Sage::Math::PI)); 
        cWin.printf("Angle 2:{x=130}{c}%.2f\xb0\n\n",   (float) (pend.m_fAngle[1]*180.0/Sage::Math::PI)); 
        cWin.printf("Zoom:{x=130}{g}%.2f%%\n",          (float) (pend.m_fZoom*100.0)); 

        // If there has been an overflow, then display it in red.  This means the math/float-point had
        // some sort of resolution problem, usually when the bob starts wobbling severely.

        // Only display it if we've have more than two, since they can happen at end points here and there. 

        if (pend.m_iOverflowCount > 2)
        {
            cWin.printf("\n\n\n\n{r}Math Overflow (%d)\n",pend.m_iOverflowCount-2); 
            cWin.printf("{12}Try increasing dampening (also lowering weight values and weight ratios).");
        }
        cWin.SetWritePadding(0);    // Reset Padding (since we may be displaying instructions)

    };

    // -- Main Render Loop --
    //
    // Waits for vertical resync and then draws updates the pendulum and renders it. 
   
    while (cWin.VsyncWait() && !bQuitProgram)
    {
        cWin.Cls();                         // Clear the window
        CSageTimer csTimer;                 // Time the loop so we can see how long it takes 

        // Use EventPending() so we only look for events when we know there is one to look for. 
        // EventPending() is not required, but allows us to not spend time looking for events when nothing has happened.

        if (cWin.EventPending()) HandleEvents(); 

        pend.Update();                      // Update the pendulum position
        pend.Render();                      // Draw the pendulum
        if (bDisplayInstructions) DisplayInstructions();
        if (bDisplayValues) DisplayValues();

        auto usTime = csTimer.ElapsedUs();                                              // Get time we took to update and draw the pendulum in microseconds
        if (bShowTiming) SageDebug::printf("Time = {p}%f ms\n",(float) usTime/1000);    // Print the time out to the Sagebox process/debug window

        cWin.Update();                  // Update the bitmap in the window. 

    }
    return 0; 
}
