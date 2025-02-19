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
class CColorSelector;
class CDevControls
{
	// Note: This is still in-progress, and items will be added as needed.
    friend CWindow;
private:
    // keep Delete, copy constructor, and move constructor private. The CQuickDialog object is a managed object, and is deleted by the main window class. 
    CDevControls(const CDevControls &p2) 
    {
        FailBox("CDevControls::CopyConstructor","In Copy Constructor")   // In debug mode, lets us know if it is ever used
    };
    void operator delete(void * ptr)
    { 
        ::operator delete(ptr); 
    };

    class CDevWinHandler : public CWindowHandler
    {
    public:
        MsgStatus OnNCLButtonDown(int iMouseX,int iMouseY) override;
        MsgStatus OnSageEvent() override;
        MsgStatus OnGlobalEvent(int iEvent) override;
    };
    static constexpr char m_cDefaultOpen = '|';
    static constexpr char m_cDefaultClose = '|'; 
    // Various structure for individual control items
    struct stCheckboxData_t
    {
        CString       cText; 
        CButton     * cCheckbox;
        bool          bDefault;
    };
    struct stCheckboxDefaults_t
    {
        const char * sDefault;
        bool bDefault;
    };
    // How to specify checkbox defaults in text, i.e. "Checkbox 1|ch|" = checked; default is not checked.
    //
    static constexpr stCheckboxDefaults_t m_stCheckboxDefaults[] = 
    {
        { "0", false },
        { "1", true },
        { "u", false },
        { "c", true },
        { "uc", false },
        { "ch", true },
        { "f", false },
        { "t", true },
        { "unchecked", false },
        { "checked", true },
        { nullptr, true },
    };

	// Various control types used in CQuickControls
	//
	enum class ControlType
	{
		Button,
		Checkbox,
        RadioButtonGroup,
		Slider,
		EditBox,
		TextWidget,
        Bitmap,
		ComboBox,
		Window,
		Header,
		Divider,
		ColorSelector,
		Unknown,
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

        // *** Makeshift data settings -- the union above should be structures for each type, rather than just pointers to controls. 
        //    $$ This needs to be fixed at some point 

        bool bSlider_SmallStyle;         // For now, we'll just put these here
	};

	std::vector<stControl_t> m_vControls;		// Controls storage

	int m_iVecEntries = 0;			// i.e. entries in vControls
	int m_iNumControls = 0;			// i.e. actual controls

    static constexpr int kMinDevWindowWidth = 100;
    static constexpr int kMaxDevWindowWidth = 1000;
	static constexpr SIZE kMaxWinSize	= { 1920,1080 };	// Max Window Size
	static constexpr SIZE kMaxSize		= { 800, 700 };		// Max outlay size (in Window)
	static constexpr int kSectionWidth	= 400;				// TBD when multi-column support is added.
    static constexpr int kDefaultDevWindowWidth = kSectionWidth + 10;       // $$ +10 is some leftover, legacy code that should be corrected (this has to do with m_iCurrentX and how it is used)
	static constexpr int kEmbedSectionWidth	= 250;				// TBD when multi-column support is added.
    static constexpr SIZE szIndent      = { 10,10 };        // Indent for X and Y (left and top)
    static constexpr SIZE szEmbedIndent = { 10,10 };        // Indent for X and Y (left and top)
    static constexpr int kMaxComboboxWidth = 225;
    static constexpr int kDefaultColorSelectFontSize = 16; 

    RgbColor kDefaultWinBgColor = SageColor::Gray92;        // Default Color Light value (multiplied against bg color) 
    bool m_bWindowClosed    = false;                        // True when close controls are active (either 'X' button and optional Close Button)
    bool m_bCloseX          = false;                        // true when there is an 'x' to close on the window.
    bool m_bAutoHide        = false;                        // Auto-hide the window when the 'x' or close button is pressed.
    bool m_bAutoCloseHide   = false;                        // ** This may be innefective (or maybe it works)... 
                                                            //    Replaced (possibly supplemented) with directly signalling that an 'x' button has been pressed.
    bool m_bDisableClose    = false;                        // Use to disable adding (automatic and otherwise) 'X' and close buttons
    SizeRect m_srClose{};                                   // bounds for close rect, when active.


	CWindow * m_cParentWin	= nullptr;
	CWindow * m_cWin		= nullptr;
    bool m_bPrimaryDev      = false;        // True when it is a top-level Dev Window (no parent)
    SIZE m_szTitleBitmap{};
	bool m_bInitialHide		    = true;		// Hide the window initially, until we see our first control.
	bool m_bShowing			    = true;		
	bool m_bTopmost			    = false;	// When true, window is forced as top window so other windows won't overlap. 
    bool m_bWindowCloseEvent    = false;   // Set when the 'X' or close button is pressed.  Resets when window is shown.
    bool m_bEmbedded            = false;    // Dev Window is embedded as child window (i.e. not a popup) when true
    bool m_bTransparent         = false;    // Only useful for embedded Dev Windows
    CButton * m_cButtonClose    = nullptr;  // Added close button for automatic close controls
	int m_iCurrentX	= 0;
	int m_iCurrentY	= 0;
    int m_iStaticWindowWidth = 0;   // Stop-gap item
    int m_iPadX = 0;
    int m_iPadY = 0;
    int m_iRightEdgePadX = 10;           // Default value, TBD
	int m_iUnNamedComoboxes		= 0;
	int m_iUnNamedSliders		= 0;
	int m_iUnNamedButtons		= 0;
	int m_iUnnamedSections		= 0;
	int m_iUnnamedCheckboxes	= 0;
	int m_iUnnamedEditBoxes		= 0;
	int m_iUnnamedWindows		= 0;
	int m_iUnnamedRadioGroups   = 0;
    CString m_csTitle;
    SIZE m_szTitleText          = {};

    int m_iGroupCount           = 0;
    static constexpr const char * kDefaultLabelColor = "Cyan";

    RgbColor m_rgbLabelColor        = {}; // Do not set here with text.  Set in constructor. Rgb(kDefaultLabelColor); 
    RgbColor m_rbgBgPlain           = { 48,48, 48 };
    RgbColor m_rbgBgColor1          = { 72,72,72 }; //{ 48, 48, 48};
    RgbColor m_rbgBgColor2          = { 32,32,32 }; //{ 32, 32, 32};
    bool     m_BgGradientStatic     = false;     // Does background gradient adjust as window adjusts (false = growing gradient vs. same gradient)
    bool     m_bDrawTopBar          = true;
    CBitmap  m_cBgBitmap;

    CDevWinHandler  m_cWindowHandler;
	
    static constexpr const char * m_sGroupPrefix = "__Sage_DevControls__";
    void InitialShow();
	void SetWindow();
    void ClearBg(int iLineStart = 0,bool bOverrideEmbed = false);
	bool AddControl(void * cControl,ControlType type,SIZE szSize,std::optional<int> iAddY = std::nullopt);
	void SetTopWindow();
    bool isLastCheckbox(stControl_t * & stCheckbox);
    bool isLastInputBox(stControl_t * & stCheckbox);
    bool isLastTextWidget(stControl_t * & stCheckbox);
    bool isLastButton(stControl_t * & stButton);
    stControl_t * GetLastControl();
    bool DrawCloseX(bool bUpdate); 
    bool HaveCloseX();
    void CheckCheckboxDefault(stCheckboxData_t & stData);
    CString CleanLine(const char * sLine);
    CString GetNewGroupName();
    static CDevControls m_cEmptyObject; 
    void BuildBackdrop(bool bOverrideEmebed = false);
    int GenerateAddWindowWidth(SIZE & szWin);
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
	CDevControls(CWindow * cWin,const char * sTitle = nullptr,const kwOpt & keywords = kw::none); 

    // GetWindow() -- Returns the Window object pointer (i.e. CWindow object) of the Dev Controls Window.
    // This can be used for various operations with the window.
    //
    // note: The Dev Controls Window is self-managed.  Use the Window functions with care.
    //
    CWindow * GetWindow(); 

    // group() -- returns the group (i.e. WinGroup) substructure for the Dev Controls Window.  This can be used to perform group functions.
    // For example, when using multiple DevSlider() calls with a declared common group (i.e. DevSlider("MySlider",opt::Group(100,1)), group.SliderMoved(100)
    // can be used to determine if any slider in the group was moved, the slider ID and the slider position.
    //
    // Groups created in DevControls must be used through that Dev Controls Window's group function, which is why group() is provided.
    //
    // For example, using cMyDevWindow->group()->SliderMoved(MyGroup,..) will check any sliders within the given group within the DevWindow.    
    //
    CWindow::WinGroup * group(); 

    /// <summary>
    /// Adds 'X' on top-right and optional "Close" button.
    /// <para></para>&#160;&#160;&#160;
    /// Dev Windows are not typically closed and do not have close controls by default.  AllowClose() adds the ability to close the window,
    /// which can be used as a way to terminate a console-mode or other program that does not have a visible window. 
    /// <para></para>
    /// Use WindowClosed() to determine if the window has been closed (or the added Close Button has been pressed.
    /// <para></para>
    /// When the 'X' button is pressed, a Sage Event is sent to the window where it can be checked. The Close button also sends a Sage Event.
    /// <para></para>&#160;&#160;&#160;
    /// Note: When the user presses the 'X' button or "Close" button, the window is not closed or hidden.  These controls are only used
    /// to provide an indication (through WindowClosed()) that the user has pressed a control as a message to terminate the application.
    /// <para></para>&#160;&#160;&#160;
    /// ---> This function is not used to close the Dev Window but as an easy way for the user to signal to close the application.
    /// Note: To disable the addition of the 'x' and close button, use AllowAutoClose()
    /// </summary>
    /// <param name="bAddCloseButton">when TRUE adds a "Close" button.  Otherwise only the 'X' is placed on the right-top of the window for closure.</param>
    /// <returns></returns>
    bool AllowClose(bool bAllowClose = true,bool bAddCloseButton = false); 

    /// <summary>
    /// When bAllow = false,Disables the Dev Window from closing on its own -- the program must close it purposely. <para></para>
    /// This will disable the 'x' from appearing when the Dev Window is the only window open, so that the  <para></para>
    /// user cannot close it with the 'x' button. <para></para>
    /// . <para></para>
    /// This function must be called before AllowClose() is called, otherwise results may be unpredictable. <para></para>
    /// Note: to manually add the 'x' (and Close Button), use AllowClose().
    /// </summary>
    /// <returns></returns>
    bool AllowAutoClose(bool bAllow = true); 

    /// <summary>
    /// Sets the Y position of the next control added.
    /// </summary>
    bool SetNextY(int iY);

    // Returns true if a close button or 'x' has been placed.  Used internally to decide when to attach a close button
    // or just stay with a previous configuration.
    //
    bool Closeable(); 

    /// <summary>
    /// Auto-hides the DevWindow when the user presses the 'x' button or close button.  This does not destroy the devwindow, and only hides it.
    /// <para></para>
    /// Once Hidden, the DevWindow can be shown again with a call to Show()
    /// </summary>
    /// <param name="bAutoHide"> - True to auto-hide window; false to turn auti-hide off</param>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    bool AutoHide(bool bAutoHide = true,bool bAddCloseButton = false);

    /// <summary>
    /// Returns TRUE if the "Close" Button or 'X' has been pressed (both are added by AllowClose()).
    /// <para></para>
    /// This can be used as a quick way to close an application that only has a Dev Window and no other window except the Console Window.
    /// </summary>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns>true if the user has attempted to close the window.</returns>
    bool WindowClosed(bool bAddCloseButton = false); 

    /// <summary>
    /// Returns true if the DevWindow Close Button was pressed, but only once -- it returns false after this point until the DevWindow
    /// is again made visible by a call to Show()
    /// <para></para>
    /// --> This difference from WindowsClose() which will continuously give a Windows-Close status of TRUE if the window is closed
    /// <para></para>
    /// WindowCloseEvent(), however, is an event status and only returns TRUE once after the close (or X) button is pressed.
    /// </summary>
    /// <param name="bAddCloseButton"> -- True to create a close button.  Otherwise only the 'x' appears in the upper-right corner.</param>
    /// <returns></returns>
    bool WindowCloseEvent(bool bAddCloseButton = false);

	// AddButton() -- Add a button to the Quick Control Window.  This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
	CButton & AddButton(const char * sButtonText = nullptr,const kwOpt & keywords = kw::none); 

	// AddCheckbox() -- Add a checkbox to the Quick Control Window. This accepts all options as normal buttons, but 
	// the default will add a regular button. 
	//
	// The Name used as a title for the button, but is optional. 
	//
    CButton& AddCheckbox(const char* sButtonText = nullptr, const kwOpt& keywords = kw::none);

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtons(const char * sButtonNames,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtons(const char * * sButtonNames,const kwOpt & keywords = kw::none);

    /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtons(int iNumButtons,const char * * sButtonNames,const kwOpt & keywords = kw::none);

     /// <summary>
    /// Adds a RadioGroup to the DevWindow.  See different prototypes for different methods (const char *, const char **, vector&lt;char *&gt;
    /// <para></para>
    /// In the case of using a simple string, separate button names with '\n', such as: 
    /// <para></para>
    /// --> AddRadioButtonGroup("Button1\nButton2\nButton3"); 
    /// <para></para>
    /// Use Title() for the section title, and Default to set a default (if Default() is not supplied, the first button is the default)
    /// <para></para>
    /// --> Example: AddRadioButtonGroup("Button1\nButton2\nButton3",Title("Select Button") | Default(1))
    /// <para></para>
    /// This sets the title to "Select Button" and sets the default button to the second button.
    /// <para></para>
    /// You can also use char * *, or vector&lt;char *&gt; to give a list of pointers to each button. 
    /// <para></para>
    /// --> Important Note: In the case of using a "char * *" array, the last entry MUST be a nullptr.  You can precede the char ** array with the number of items to avoid using a nullptr.
    /// </summary>
    /// <param name="iNumButtons">- (optional) When using "const char * *" for a list of names, you can give it the number of items (the end does not need to be a nullptr in this case)</param>
    /// <param name="sButtonNames">String of button names, or char * * list, or std::vector&lt;const char *&gt; of names.</param>
    /// <param name="cwOpt">Options such as Default() and Title()</param>
    /// <returns></returns>
    ButtonGroup AddRadioButtons(std::vector<char *> & vButtonNames,const kwOpt & keywords = kw::none);


    // -- Checkbox groups
    
	
    ButtonGroup AddCheckboxGroup(const char * sButtonNames,const kwOpt & keywords = kw::none);
    ButtonGroup AddCheckboxGroup(const char * * sButtonNames,const kwOpt & keywords = kw::none);
    ButtonGroup AddCheckboxGroup(int iNumButtons,const char * * sButtonNames,const kwOpt & keywords = kw::none);
    ButtonGroup AddCheckboxGroup(std::vector<char *> & vButtonNames,const kwOpt & keywords = kw::none);
	
    // AddEditBox() -- Add an EditBox to the quick control Window.  The sEditBoxTitle, while optional, will provide a
	// label to the left of the edit box.  The default width is 150 pixels or so, but can be changed with normal EditBox options
	//
    // Note: InputBox and EditBox are the same.  EditBox is kept to remain consistent with Windows terminology
    //
	CEditBox & AddInputBox(const char * sEditBoxTitle = nullptr,const kwOpt & keywords = kw::none); 

	CTextWidget & AddTextWidget(const char * sText,const kwOpt & keywords = kw::none);
	CTextWidget & AddTextWidget(const char * sText,int iHeight,const kwOpt & keywords = kw::none);
	CTextWidget & AddTextWidget(int iHeight,const kwOpt & keywords = kw::none);
	CTextWidget & AddTextWidget(const kwOpt & keywords = kw::none);

    /// @AddColorSelect
    /// <summary>
    /// Adds a Color Selector Control to the DevWindow. 
    /// <para></para>
    /// A color selector control allows the user to visually select a color and/or set the RGB values of the color
    /// independently in a control panel embedded in the dev window.
    /// <para></para>
    /// --> A ColorSelector reference is returned to control the Color Selector in the Dev Window
    /// <para></para>
    /// --> See ColorSelector documentation for more information.
    /// <para></para>
    /// --> ** note -- A popup Color Selector can be created by instantiating a ColorSelector() object.  This will create an independent Color Selector window.
    /// </summary>
    /// <param name="sTitle"> - [optional] Title of the color selector panel in the Dev Window.  Can be omitted.</param>
    /// <param name="keywords"> - [optional] Various keywords, such as kw::TitleColor(), kw::Font(), etc. to control the title display.</param>
    /// <returns>ColorSelect reference to control the Color Selector Control.</returns>
    ColorSelector & AddColorSelect(const char * sTitle,const kwOpt & keywords = kw::none); 

    /// @AddColorSelect
    /// <summary>
    /// Adds a Color Selector Control to the DevWindow. 
    /// <para></para>
    /// A color selector control allows the user to visually select a color and/or set the RGB values of the color
    /// independently in a control panel embedded in the dev window.
    /// <para></para>
    /// --> A ColorSelector reference is returned to control the Color Selector in the Dev Window
    /// <para></para>
    /// --> See ColorSelector documentation for more information.
    /// <para></para>
    /// --> ** note -- A popup Color Selector can be created by instantiating a ColorSelector() object.  This will create an independent Color Selector window.
    /// </summary>
    /// <param name="sTitle"> - [optional] Title of the color selector panel in the Dev Window.  Can be omitted.</param>
    /// <param name="keywords"> - [optional] Various keywords, such as kw::TitleColor(), kw::Font(), etc. to control the title display.</param>
    /// <returns>ColorSelect reference to control the Color Selector Control.</returns>
    ColorSelector & AddColorSelect(const kwOpt & keywords = kw::none); 
    
	// AddSlider() -- Add a slider to the Quick Controls Window.  The default width is 200 with a 0-100 range.  The Range can be 
	// changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
//	CSlider & AddSlider(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CSlider & AddSlider(const char * sSliderName = nullptr,const kwOpt & keywords = kw::none);

	// AddSliderf() -- Add a floating-point slider to the Quick Controls Window.  The default width is 200 with a 0-100 range.  The Range can be 
	// changed with default Slider options, i.e. opt::Range(0,200), for example, to set a range of 0-200.
	// -->
	// The title is displayed beneath the slider, as well as the value. 
	//
//	CSlider & AddSliderf(const char * sSliderName = nullptr,const cwfOpt & cwOpt = cwfOpt());
	CSlider & AddSliderf(const char * sSliderName = nullptr,const kwOpt & keywords = kw::none);

	CComboBox & AddComboBox(const char * sComboBoxItems,const kwOpt & keywords = kw::none);
	
    CWindow & AddWindow(const char * sTitle = nullptr,int iNumlines = 0,const kwOpt & keywords = kw::none);
	CWindow & AddWindow(const char * sTitle,const kwOpt & keywords);
	CWindow & AddWindow(int iNumLines,const kwOpt & keywords = kw::none);
	CWindow & AddWindow(const kwOpt & keywords);

    CWindow & AddText(int iInHeight = 0,const char * sString = nullptr,const kwOpt& keywords = kw::none);
    CWindow & AddText(int iInHeight,const kwOpt& keywords);
    CWindow & AddText(const char * sString,const kwOpt& keywords = kw::none);
    CWindow & AddText(const kwOpt& keywords);

    /// # AddBitmap
    /// <summary>
    /// Adds a bitmap to the window. This bitmap is copied and stored by the Dev Window.  The original bitmap does not need to be kept allocated.<para></para>
    /// The bitmap can be a 32-bit BMP or .PNG file and will blend in to the background.<para></para>
    /// --> Bitmaps in the Dev Window are meant to be either icons or small bitmaps for headers.  <para></para>
    /// A String may be added to the the right of the bitmap.  If the bitmap is not found, the string will still be printed.<para></para>
    /// Sagebox options can be used to set the Font and color of the text.  The text will be centered vertically to the bitmap's center.<para></para>
    /// ---> Examples: AddBitmap(MyBitmap)<para></para>
    /// AddBitmap("c:\\bitmaps\\mybitmap.bmp",false) <para></para>
    /// AddBitmap("c:\\bitmaps\\mybitmap.png"," Project Controls",opt::PadY(10))        -- add 10 pixels to the Y position in the window after the bitmap.<para></para>
    /// AddBitmap("c:\\bitmaps\\mybitmap.png"," Project Controls",opt::Font(20 | opt::TextColor("green"))<para></para>
    /// </summary>   
    /// <param name="cBitmap">- The bitmap to display.  This can either be a Sage::CBitmap type (with transparencies) or a path to the bitmap.</param>
    /// <param name="sText">- [optional] Text to the right of the bitmap. Note the space in the example above to add some blank space.</param>
    /// <param name="bDrawTopBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <param name="cwOpt">- [optional] Options such as opt::Font(), opt::TextColor(), opt::PadY() to control the next and next control position.</param>
    /// <returns></returns>
    bool AddBitmap(CBitmap & cBitmap,bool bDrawTopBar = true);                                                               /// # AddBitmap
    bool AddBitmap(CBitmap & cBitmap,const char * sText,bool bDrawTopBar = true,const kwOpt & keywords = kw::none);    /// # AddBitmap
    bool AddBitmap(CBitmap & cBitmap,const char * sText,const kwOpt & keywords);                                               /// # AddBitmap

    bool AddBitmap(const char * sPath,bool bDrawTopBar = true);                                                               /// # AddBitmap
    bool AddBitmap(const char * sPath,const char * sText,bool bDrawTopBar = true,const kwOpt & keywords = kw::none);    /// # AddBitmap
    bool AddBitmap(const char * sPath,const char * sText,const kwOpt & keywords);                                               /// # AddBitmap

	// AddSection() -- Adds a text section to the window, to separate types of controls.
	// You can use opt::fgColor() to set the text color of the section name.
	//
	bool AddSection(const char * sSectionName = nullptr,const kwOpt & keywords = kw::none);

	// SetLocation() -- Change the location of the QuickControls Window
	//
	bool SetLocation(int iX,int iY);
    // SetLocation() -- Change the location of the QuickControls Window
    //
    bool SetLocation(POINT pLoc);


    /// <summary>
    /// AdjustWinPos() - Adjust the horizontal and/or vertical positon of the window
    /// </summary>
    /// <param name="iHorz"> - Amount to adjust horizontal positon (negative or positive number.  0 for no movement.</param>
    /// <param name="iVert"> - Amount to adjust vertical positon (negative or positive number.  0 for no movement.</param>
    /// <returns></returns>
    bool AdjustWinPos(int iHorz, int iVert);

    /// <summary>
    /// AdjustWinPos() - Adjust the horizontal and/or vertical positon of the window
    /// </summary>
    /// <param name="pAdjust"> - POINT structure with movement amounts in order of horizintal and vertical.  Use 0 for no movement. Negative numbers can be used.</param>
    /// <returns></returns>
    bool AdjustWinPos(POINT pAdjust);

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
    bool Cls();

    /// #SetBgColor
    /// <summary>
    /// Sets the background color (or colors) of the Dev Window.<para></para>
    /// --> Note: This function should be called before any Dev Controls are created (i.e. buttons, sliders, etc.) so that<para></para>
    /// they will blend properly into the background.<para></para>
    /// Two colors may be used to form a gradient, which will span the maximum vertical length of the Dev Window (i.e. it will show more as controls are added)
    /// .<para></para>
    /// Colors can be Sagebox Rgb Color (i.e. RgbColor(0,255,0), SageColor::Red, PanColor::Green,etc.) or string such as <para></para>
    /// "Green","red","PanColor:ForestGreen", etc.<para></para>
    /// .<para></para>
    /// Examples: SetBgColor(SageColor::Black)<para></para>
    ///          SetBgColor("black","blue")<para></para>
    /// </summary>
    /// <param name="rgbColor1">- Color to clear the dev window</param>
    /// <param name="rgbColor2">- [optional] Second color to clear the background with a gradient</param>
    /// <param name="bDrawBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <returns></returns>
    bool SetBgColor(RgbColor rgbColor1,RgbColor rgbColor2 = Sage::RgbVal::Undefined,bool bDrawBar = true);  // # SetBgColor
    bool SetBgColor(RgbColor rgbColor1,bool bDrawBar);                                                   // # SetBgColor
    bool SetBgColor(const char * sColor1,const char * sColor2 = nullptr,bool bDrawBar = true);           // # SetBgColor
    bool SetBgColor(const char * sColor1,bool bDrawBar);                                                 // # SetBgColor

    /// #SetBgBitmap
    /// <summary>
    /// Sets the background bitmap of the Dev Window.  The bitmap provided should span the <para></para>
    /// width and height of the full Dev Window, as Dev Windows grow vertically as controls are added.<para></para>
    /// --> Note: This function should be called before any Dev Controls are created (i.e. buttons, sliders, etc.) so that<para></para>
    /// .<para></para>
    /// Options can be used to set a new Y position.  Use opt::PadY() option to set the current Y position<para></para>
    /// This can be useful if the bitmap has a header, allowing the first control to start beneath it.<para></para>
    /// .<para></para>
    /// Examples:  SetBgBitmap(MyBitmap)<para></para>
    ///            SetBgBitmap("c:\\bitmaps\\MyBitmap.jpg")<para></para>
    ///            SetBgBitmap(MyBitmap,false,opt::PadY(50))<para></para>
    /// 
    /// </summary>
    /// <param name="cBitmap">- The bitmap to set as background bitmap.  This bitmap may be a Sage::CBitmap type or string containing the bitmap path.</param>
    /// <param name="bDrawBar">- [optional] When true (default), the top menu bar is drawn in the original color.  When false, the top menu bar is covered by the new clear screan colors.</param>
    /// <param name="cwOpt"> - [optional] Options.  The only useful option (right now) is the PadY() option to set the first control Y position in the DevWindow</param>
    /// <returns></returns>
    bool SetBgBitmap(CBitmap & cBitmap,bool bDrawBar = true,const kwOpt & keywords = kw::none);       // # SetBgBitmap
    bool SetBgBitmap(const char * sBitmap,bool bDrawBar = true,const kwOpt & keywords = kw::none);    // # SetBgBitmap
    bool SetBgBitmap(CBitmap & cBitmap,const kwOpt & keywords);                                       // # SetBgBitmap
    bool SetBgBitmap(const char * sBitmap,const kwOpt & keywords);                                    // # SetBgBitmap

    // Sets the window width of the dev window.
    // This should be used before using any operations with the Dev Window, otherwise some
    // window-drawing mismatch or other drawing issues may occur. 
    //
    // Note: Minimum size is 100 pixels. Maximum size is 1000 pixels. 
    //       This may also affect auto-placement if the window.  Use SetLocation() or AdjustWinPos() to compensize
    //
    bool SetWindowWidth(int iWindowWidth); 

    /// <summary>
    /// Adjusts the width of the window based on the usual default size. 
    /// <para></para>
    /// For example, DevAdjustWidth(-100) makes the window 100 pixels narrower than usual, where DevAdjustWith(200) will make the window 200 wider than default settings.
    /// <para></para>
    /// This function should be used before any controls on the Dev Window are created.  
    /// <para></para>
    /// DevAdjustWidth() is not additive.  It adjusts from the default window width, not the current width (since the window is usually not opened when called).
    /// <para></para>
    /// note:  The minimum window size is 100 pixels, with a maximum of 1000.  If the adjustment exceeds these values, the window will grow or shrink to these minimum and maximum values.
    /// <para></para>
    /// --> Changing the width of the window may affect auto-placment.  Use DevSetLocation() or DevAdjustWinPos() to set the window placement manually.
    /// </summary>
    bool AdjustWindowWidth(int iWindowWidthAdjust); 

    /// <summary>
    /// Sets the window to close automatically when there are no other windows open. <para></para>
    /// By default, the Dev Window is a 'primary' window and won't close when <para></para>
    /// functions such as WaitPending() or GetEvent() are used.<para></para>
    /// .<para></para>
    /// When set to false (default), the window won't close until it is closed by the user. 
    /// or the program exits.
    /// </summary>
    bool AutoClose(bool bAutoClose = true);

    static CDevControls & GetEmptyObject() { return m_cEmptyObject; };
    bool isValid() { return this != nullptr && m_cWin != nullptr; }
    static void _Deleter(void * pObj);
    ~CDevControls();
};
}; // namespace Sage
#endif // _CQuickControls_H_y
