///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 11 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MainGUI__
#define __MainGUI__

#include <wx/intl.h>

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_SHEETS_LIST 1000
#define ID_NEW_SPRITE 1001
#define ID_DELETE_SPRITE 1002
#define ID_NEW_FRAME 1003
#define ID_DELETE_FRAME 1004
#define ID_PREV_FRAME 1005
#define ID_NEXT_FRAME 1006
#define ID_SPRITE_NAME_TEXT 1007
#define ID_SET_SPRITE_NAME 1008
#define ID_SPRITES_TREE 1009
#define ID_COMMENTS_TEXT 1010
#define ID_INSERT_FRAME 1011
#define ID_MOVE_PREVIEW 1012
#define ID_COPY 1013
#define ID_PASTE_FRAME 1014
#define ID_PASTE_SPRITE 1015
#define ID_SWAP_FRAME 1016
#define ID_SAVE 1017
#define ID_CHANGE_DIR 1018
#define ID_SWAP_LR 1019
#define ID_WALK_ANIM 1020
#define ID_LINK_BBOX 1021
#define ID_COPY_LR 1022
#define ID_COPY_ALL_LR 1023
#define ID_AUTOGEN_FLIP 1024
#define ID_EDIT_MODE 1025

///////////////////////////////////////////////////////////////////////////////
/// Class MainWindowGUI
///////////////////////////////////////////////////////////////////////////////
class MainWindowGUI : public wxFrame 
{
	private:
	
	protected:
		
		
		wxPanel* fPreviewAreaContainer;
		
		wxChoice* fSheetsList;
		
		wxButton* fNewSpriteButton;
		wxButton* fDeleteSpriteButton;
		wxButton* fNewFrameButton;
		wxButton* fDeleteFrameButton;
		wxButton* fPrevFrameButton;
		wxButton* fNextFrameButton;
		
		wxTextCtrl* fSpriteNameText;
		wxButton* fSetSpriteNameButton;
		wxTextCtrl* fCommentsText;
		
		
		wxButton* fInsertFrame;
		wxButton* fMovePreviewButton;
		wxButton* fCopyButton;
		wxButton* fPasteFrameButton;
		wxButton* fPasteSpriteButton;
		wxButton* fSwapFrameButton;
		wxButton* fSaveButton;
		wxButton* fChangeDirButton;
		wxButton* fSwapDirButton;
		wxButton* fWalkAnimButton;
		wxButton* fCopyLRButton;
		wxButton* fCopyAllDirButton;
		wxButton* fAutogenFlipButton;
		
		wxNotebook* fEditModeTabs;
		wxPanel* m_panel552;
		wxPanel* m_panel55;
		wxPanel* m_panel5;
		wxPanel* m_panel6;
		wxPanel* m_panel553;
		wxPanel* m_panel51;
		wxPanel* m_panel52;
		wxPanel* m_panel53;
		wxPanel* m_panel54;
		wxPanel* m_panel541;
		wxPanel* m_panel551;
		wxPanel* m_panel531;
		wxPanel* fFramesAreaContainer;
		wxPanel* fEditAreaContainer;
	
	public:
		wxTreeCtrl* fSpritesTree;
		wxCheckBox* fLinkBBoxCheckbox;
		MainWindowGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("SIFEdit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1015,772 ), long style = wxDEFAULT_FRAME_STYLE|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~MainWindowGUI();
	
};

#endif //__MainGUI__
