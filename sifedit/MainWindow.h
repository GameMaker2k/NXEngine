
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "gui/MainGUI.h"
#include "TreeItemList.h"

class EditArea;
class FramesArea;
class PreviewArea;

enum
{
	ID_Quit = wxID_EXIT,
	ID_MENU_SEND_TO = 3000,
	ID_FIRST = 2000,
	
	ID_EditArea,
	ID_FramesArea,
	ID_PreviewArea,
	
	ID_MENU_DELETE,
	ID_MENU_SET_DIRS_1,
	ID_MENU_SET_DIRS_2,
	ID_MENU_SET_DIRS_3,
	ID_MENU_SET_DIRS_4,
	
	ID_ADD_GROUP,
	ID_ADD_SPRITESHEET,
	ID_REMOVE_SPRITESHEET,
	
	ID_COPY_DP_FRAMES,
	ID_COPY_DP_DIRS,
	ID_COPY_DP_FRAMES_DIRS,
	ID_SWAP_APS,
	
	ID_LAST
};

class SpriteTreeData : public wxTreeItemData
{
public:

	union
	{
		int spriteno;
		int groupno;
	};
	
	bool isgroup;
};

class MainWindow : public MainWindowGUI
{
public:
	MainWindow();
	void OnFixTimer(wxTimerEvent &evt);
	
	void OnQuit(wxCommandEvent &evt);
	void OnSize(wxSizeEvent &evt);
	bool OnKeyDown(wxKeyEvent &evt);
	bool OnKeyUp(wxKeyEvent &evt);
	
	// sprite tree handlers
	void OnTreeSelChanged(wxTreeEvent &evt);
	void OnTreeEndLabelEdit(wxTreeEvent &evt);
	void OnTreeBeginDrag(wxTreeEvent &evt);
	void OnTreeEndDrag(wxTreeEvent &evt);
	void OnTreeMenu(wxTreeEvent &evt);
	void OnTreeMenuResponse(wxCommandEvent &evt);
	
	// left column handlers
	void OnSheetsListChoice(wxCommandEvent &evt);
	
	void OnNewSprite(wxCommandEvent &evt);
	void OnDeleteSprite(wxCommandEvent &evt);
	void OnPrevNextFrame(wxCommandEvent &evt);
	
	void OnNewFrame(wxCommandEvent &evt);
	void OnDeleteFrame(wxCommandEvent &evt);
	
	// upper area handlers
	void OnEditModeChanged(wxNotebookEvent &evt);
	void OnChangeDirClicked(wxCommandEvent &evt);
	void OnMovePreviewClicked(wxCommandEvent &evt);
	
	void OnClipboardButtonClicked(wxCommandEvent &evt);
	void OnTopButtonClicked(wxCommandEvent &evt);
	
	void OnSaveButtonClicked(wxCommandEvent &evt);
	void OnCommentChanged(wxCommandEvent &evt);
	
	void OnSetSpriteNameClicked(wxCommandEvent &evt);
	void OnSpriteNameTextGotFocus(wxFocusEvent &evt);
	void OnSpriteNameTextLeftUp(wxMouseEvent &evt);
	
	
	void OnAddGroup(wxCommandEvent &evt);
	void OnAddSpritesheet(wxCommandEvent &evt);
	void OnRemoveSpritesheet(wxCommandEvent &evt);
	
	// updating functions
	void UpdateSpritesTree();
	void UpdateSheetsList();
	void UpdateDisplay();
	
	void UpdateTitle();
	
	// control and utility functions
	void SetCurrentSprite(int spr, int dir=RIGHT);
	void PromptForSpriteDelete(int delno);
	void PromptForGroupDelete(int delgroup);
	void SetCurrentDirText();
	
	void MoveFrameToNextFrame(SIFFrame *frame, SIFSprite *sprite);
	
	EditArea *fEditArea;
	FramesArea *fFramesArea;
	PreviewArea *fPreviewArea;
	
	TreeItemList fGroupTreeIDs;			// wxTreeIDs that correspond to each group
	TreeItemList fSpriteTreeIDs;

private:
	void InitMenuBar();
	
	int GetSpriteSelectedInTree();
	bool SetSpriteSelectedInTree(int sprite);
	
	wxTimer fFixTimer;
	bool fSpritesTextJustGotFocus;
	
	struct
	{
		int upforindex;
		bool isgroup;
	} fTreeMenu;
	
	wxArrayTreeItemIds fDragSelections;
	bool fDragging;
	DECLARE_EVENT_TABLE()
};

extern MainWindow *mainwin;

#endif
