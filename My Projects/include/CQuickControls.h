// This file copyright(c) 2021 Rob Nelson, All Rights Reserved.    E-mail rob@projectsagebox.com for more information.
//

// --------------------------------------------------------------------------------------------------------------
// PRELIMINARY VERSION.  This file has been brought into the Sagebox project from the original sources and has 
// not yet been commented for Sagebox, or properly formatted (I used tabs and am converting to spaces).
// --------------------------------------------------------------------------------------------------------------
//
// ****** UNDER CONSTRUCTION ******
//
// This file is still under construction and may not yet include specifics, awaiting proper testing and integration into Sagebox.
//

#pragma once
#include "CSageBox.h"

//****************************************************************************************************************
// CDevControls -- Fast, Windows Based controls for console mode and windows program prototyping and development
//****************************************************************************************************************
//
// CDevControls is a set of controls such as sliders, buttons, and so-forth that can be brought up very quickly without 
// the need to place the controls or created a window to store the controls. 
//
// The main purpose of CDevControls is to allow for protypying and development in places where the controls are useful, but you don't
// want to put them in your Windows interface or populate your console-mode program or library function with UI functions. 
//
// For example, if you want an abort button, or a simple slider to control program flow, both of these can be brought up with one line and controlled
// in different ways -- these can also be used without using any windows controls by specifying where to place the event and data from the control as it is 
// used. 
//
// This allows test elements to be put as an aside to the program that can later be removed or integrated into the program if they prove useful.
//
// ----------------------
// One-line instantiation
// ----------------------
//
// CDevControls works by keeping track of the controls and automatically placing them.  When first instantiated, nothing happens until a control is defined, after which a 
// window comes up with the control.
//
// As controls are added, the window grows to fit the controls.
//
// If there are too many controls to fit in the window, you can easily open another CDevControls window for more controls.
//
// For example, in console mode you can specify:
//
//   cSageBox.DevButton("Press to Stop");  to bring up a button.  or
//   cSageBox.DevEditbox("This is an Edit Box"); to bring up an edit box.
//
// ----------------------
// Accessing the Controls
// ----------------------
//
// Each control returns a pointer to the control, but this does not need to be used to access the control.
//
// For example,
//
// auto& myButton = cSageBox.DevButton("Stop"); 
//
// After which myButton.Pressed() can be used to determine when the button is pressed.
//
// However, sometimes you don't want to use the EventLoop() or access the button as a UI element in your code.
// In this case, you can set a Signal, such as:
//
// bool bSignal;
// myButton.SetSignal(&bSignal);
//
// Which will fill bSignal when the button is pressed (note that bSignal should be set to false to free it for the next press).
// --> You can also use GetSignal(bSignal), which is a macro that will return the Signal status and reset it at the same time.
//     ButtonSignal (which is a simple structure containing the bSignal and another boolean used in Checkboxes)
//     can also be used, such as ButtonSignal stSignal;, where stSignal.GetSignal() can then be used.
//
// As a oneliner, the following can be used without saving the button object:
//
// cSageBox.DevButton("Stop").SetSignal(&bSignal); 
//
// note:  While using signals isn't the safest way to go about checking the status, as the scope must be maintained for the signal (which usually is not a problem),
//        using the signal is a powerful way to use the button press, as the boolean value can be passed to any function that does not need to know or care that it is 
//        filled be a GUI control.
//
// See the notes on functions below for more information.
//
// -------------------------------------------------------------------------------------------
// cSageBox.DevButton() vs. cDevControls.AddButton(), etc. : Console Mode vs. using Windows
// -------------------------------------------------------------------------------------------
//
// When using a Window that has been created, or when creating another CDevControls object, the main functions are AddButton(), AddEditBox(), etc. 
// CSageBox has a pre-created CDevControls object (CDevControls) that can be access through various functions without the need to create a window or a 
// CDevControls object.
//
// This is also true when using a Window -- you can call cSageBox.QuickButton(), for example, in console or Window mode, with our without an active window.
// With an active Window, you can also call CWindow::DevControlsWindow() to create a CDevControls Window. 
//
// For very quick action, CSageBox features the ability to simply state things like:
//
// cSageBox.DevButton("Stop");
// cSageBox.DevEditBox();
// cSageBox.DevSlider("Some Slider")  
//
// with no preamble such as setting up the CQuickControls object.
//
// The equivalent, when setting one up yourself, is as follows:
//
// CDevControls cDevControls = MyWin.DevControlWindow() or (or cSageBox.DevControlWindow())
//
// cQuickControls->DevButton("Stop");
// cQuickControls->DevEditBox();
// cQuickControls->DevSlider("Some Slider")  
//
// --------------------------------
// Deleting a CDevControls Object
// --------------------------------
//
// The automatically created CDevControls object in CSagebox is automatically deleted, however:
//
// *** Important Note *** --> When using CDevControls() to return a Quick Control object THIS IS RETURNED AS A POINTER THAT MUST BE DELETED.
//                            In some examples, you may see Obj<CDevControls> cQuick = MyWin.DevControlsWindow() to turn the pointer into an object scoped
//                            to the current function to ensure it is deleted when the function is finished -- or, as a Obj<CDevControls> in a class to 
//                            ensure automatic deletion when the class object is destroyed.
//
// ------------
// SetTopmost()
// ------------
//
// This function is worth noting, as the CDevControls window can be overlapped by other windows.  Use SetTopmost() to set the window
// so that other windows will no overlap, or use opt::SetTopmost() as a parameter when creating the CDevControls window.
//
// For the automatically-created CDevControls window in CSagebox, use DevControlsTopmost() as a quick way to set it.
//
#if !defined(_CQuickControls_H_)
#define _CQuickControls_H_
namespace Sage
{
class CDevControls
{
	// Note: This is still in-progress, and items will be added as needed.

private:
	// Various control types used in CQuickControls
	//
	enum class ControlType
	{
		Button,
		Checkbox,
		Slider,
		EditBox,
		TextWidget,
		ComboBox,
		Window,
		Header,
		Divider,
	};

	// Control structure saved for each entry
	//
	struct stControl_t
	{
		POINT pLoc;
		SIZE szSize;
		ControlType controlType;
		CString cSectionName;	// unneeded memory for most items, but we don't care about memory usage here. 
		union Un
		{
			CButton * cButton;
			CSlider * cSlider;
			CEditBox * cEditBox;
		};
		Un un;
	};

	std::vector<stControl_t> m_vControls;		// Controls storage

	int m_iVecEntries = 0;			// i.e. entries in vControls
	int m_iNumControls = 0;			// i.e. actual controls

	static constexpr SIZE kMaxWinSize	= { 1920,1080 };	// Max Window Size
	static constexpr SIZE kMaxSize		= { 800, 700 };		// Max outlay size (in Window)
	static constexpr int kSectionWidth	= 400;				// TBD when multi-column support is added.
	
	CWindow * m_cParentWin	= nullptr;
	CWindow * m_cWin		= nullptr;
	bool m_bInitialHide		= true;		// Hide the window initially, until we see our first control.
	bool m_bShowing			= true;		
	bool m_bTopmost			= false;	// When true, window is forced as top window so other windows won't overlap. 

	int m_iCurrentX	= 0;
	int m_iCurrentY	= 0;

	int m_iUnNamedComoboxes		= 0;
	int m_iUnNamedSliders		= 0;
	int m_iUnNamedButtons		= 0;
	int m_iUnnamedSections		= 0;
	int m_iUnnamedCheckboxes	= 0;
	int m_iUnnamedEditBoxes		= 0;
	int m_iUnnamedWindows		= 0;

	void InitialShow();
	void SetWindow();
	bool AddControl(void * cControl,ControlType type,SIZE szSize);
	void SetTopWindow();
    bool isLastCheckbox(stControl_t * & stCheckbox);
public:
	// ----------------
	// Public Interface
	// ----------------

	// QuickControls() -- Start a Quick Controls window.  This window will remain invisible until a control as added.  It sizes itself 
	// dymamically to the number of controls added.
	//
	// Don't use this to create the Quick Control Window! -- Instead,
	// Use CSageBox::QuickControlWindow() or CWindow::QuickControlWindow() to create a QuickControl Window -- this 
	// allows you to set a title, background color, and position for the window.
	//
    // ** Important note ** the object pointer returned from CSageBox and Cwindows QuickControlWindow() functions
	// MUST BE DELETED, as it is not a managed object. 
	//
    // Example code tends to use Obj<CQuickControls> cQuickControl = QuickControlWindow() to treat it as a stack object
    // that is automatically deleted when the current function (or class) goes out of scope.
    // (However, since this is usually only used for develpment, leaving it allocated prior to program end causes no problems)
    //
	CDevControls(CWindow * cWin,const char * sTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	// AddButton() -- Add a button to the Quick Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
	CButton & AddButton(const char * sButtonText = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	// AddCheckbox() -- Add a checkbox to the Quick Control Window. This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
	CButton & AddCheckbox(const char * sButtonText = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	// AddEditBox() -- Add an EditBox to the quick control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
	CEditBox & AddEditBox(const char * sEditBoxTitle = nullptr,const cwfOpt & cwOpt = cwfOpt()); 

	CTextWidget & AddText(const char * sText,const cwfOpt & cwOpt = cwfOpt());
	CTextWidget & AddText(const char * sText,int iHeight,const cwfOpt & cwOpt = cwfOpt());

	// AddSlider() -- Add a slider to the Quick Controls Window.  The default width is 200 with a 0-100 range.  The Range can be 
	// changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
	CSlider & AddSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());

	CComboBox & AddComboBox(const char * sComboBoxName,const cwfOpt & cwOpt = cwfOpt());
	CWindow & AddWindow(const char * sTitle,int iNumlines,const cwfOpt & cwOpt = cwfOpt());
	// AddSection() -- Adds a text section to the window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
	bool AddSection(const char * sSectionName = nullptr,const cwfOpt & cwOpt = cwfOpt());

	// SetLocation() -- Change the location of the QuickControls Window
	//
	bool SetLocation(int iX,int iY);
	bool SetLocation(POINT pLoc);

	POINT GetLocation();
	SIZE GetWindowSize();

	// SetTopMost() -- Set the window as a "topmost" window, which will keep the window on top of all other windows, 
	// disallowing other windows from overlapping the window.  This can be useful when using the controls, as other windows, when they
	// gain focus, can overlap the Quick Controls Window
	//
    bool SetTopMost(bool bTopmost = true);

	// Show() -- Show or hide the Quick Controls Window
	//
	bool Show(bool bShow = true);

	// Hide() -- Show or hide the Quick Controls Window
	//
	bool Hide(bool bHide = true);
};
}; // namespace Sage
#endif // _CQuickControls_H_y
