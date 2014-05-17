#ifndef ___DUIVISION_H__
#define ___DUIVISION_H__

#ifdef DLL_DUI
# ifdef DUIVISION_EXPORTS
# pragma message("dll export")
#  define DUI_EXP __declspec(dllexport)
# else
#  define DUI_EXP __declspec(dllimport)
# endif // DUIVISION_EXPORTS
#else
#define DUI_EXP 
#endif

#include <gdiplus.h> 
using namespace Gdiplus;

#include <shlwapi.h>

#include "duiid.h"

#include "../tinyxml/TinyXml.h"

//////////////////////////////////////////////////////////
// Task
#include "../common/BaseTask.h"
#include "../common/TaskMgr.h"

//////////////////////////////////////////////////////////
//Base
#include "DuiObject.h"
#include "ControlBase.h"
#include "GlobalFunction.h"
#include "DuiHandler.h"
#include "Panel.h"
#include "DuiSystem.h"

//////////////////////////////////////////////////////////
//Timer
#include "Timer.h"

//////////////////////////////////////////////////////////
//Button
#include "ButtonEx.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "HideButton.h"
#include "LinkButton.h"
#include "TextButton.h"

//////////////////////////////////////////////////////////
//List
#include "PopupList.h"
#include "DuiListCtrl.h"
#include "DuiGridCtrl.h"

//////////////////////////////////////////////////////////
//Other
#include "Area.h"
#include "Frame.h"
#include "ImageString.h"
#include "Rectangle.h"
#include "Runing.h"
#include "Scroll.h"
#include "SelectBox.h"

//////////////////////////////////////////////////////////
//Picture
#include "Picture.h"

//////////////////////////////////////////////////////////
//Progress
#include "Progress.h"

//////////////////////////////////////////////////////////
//Tab
#include "Tab.h"

//////////////////////////////////////////////////////////
//Text
#include "StaticText.h"

//////////////////////////////////////////////////////////
//Dialog
#include "DlgBase.h"
#include "DlgPopup.h"
#include "DlgSkin.h"

//////////////////////////////////////////////////////////
//Menu
#include "MenuItem.h"
#include "MenuEx.h"

//////////////////////////////////////////////////////////
//Edit
#include "EditEx.h"
#include "ComboBox.h"

#endif