
#include <wx/wx.h>
#include "sifedit.h"
#include "MainWindow.h"
#include "EditArea.h"
#include "FramesArea.h"
#include "PreviewArea.h"

#include "MainWindow.fdh"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)

	EVT_MENU(ID_Quit, MainWindow::OnQuit)
	EVT_COMMAND(ID_Quit, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnQuit)
	EVT_SIZE(MainWindow::OnSize)
    EVT_TIMER(101, MainWindow::OnFixTimer)

	// menu
	EVT_MENU(ID_ADD_GROUP, MainWindow::OnAddGroup)
	EVT_MENU(ID_ADD_SPRITESHEET, MainWindow::OnAddSpritesheet)
	EVT_MENU(ID_REMOVE_SPRITESHEET, MainWindow::OnRemoveSpritesheet)
	
	EVT_MENU(ID_COPY_DP_FRAMES, MainWindow::OnTopButtonClicked)
	EVT_MENU(ID_COPY_DP_DIRS, MainWindow::OnTopButtonClicked)
	EVT_MENU(ID_COPY_DP_FRAMES_DIRS, MainWindow::OnTopButtonClicked)
	EVT_MENU(ID_SWAP_APS, MainWindow::OnTopButtonClicked)
	
	// left column
	EVT_TREE_SEL_CHANGED(ID_SPRITES_TREE, MainWindow::OnTreeSelChanged)
	EVT_TREE_END_LABEL_EDIT(ID_SPRITES_TREE, MainWindow::OnTreeEndLabelEdit)
	EVT_TREE_BEGIN_DRAG(ID_SPRITES_TREE, MainWindow::OnTreeBeginDrag)
	EVT_TREE_END_DRAG(ID_SPRITES_TREE, MainWindow::OnTreeEndDrag)
	EVT_TREE_ITEM_MENU(ID_SPRITES_TREE, MainWindow::OnTreeMenu)
	
	EVT_TEXT(ID_COMMENTS_TEXT, MainWindow::OnCommentChanged)
	EVT_TEXT_ENTER(ID_SPRITE_NAME_TEXT, MainWindow::OnSetSpriteNameClicked)
	
	EVT_MENU(ID_MENU_DELETE, MainWindow::OnTreeMenuResponse)
	EVT_MENU(ID_MENU_SET_DIRS_1, MainWindow::OnTreeMenuResponse)
	EVT_MENU(ID_MENU_SET_DIRS_2, MainWindow::OnTreeMenuResponse)
	EVT_MENU(ID_MENU_SET_DIRS_3, MainWindow::OnTreeMenuResponse)
	EVT_MENU(ID_MENU_SET_DIRS_4, MainWindow::OnTreeMenuResponse)
	
	EVT_CHOICE(ID_SHEETS_LIST, MainWindow::OnSheetsListChoice)
	
	EVT_COMMAND(ID_NEW_SPRITE, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnNewSprite)
	EVT_COMMAND(ID_DELETE_SPRITE, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnDeleteSprite)
	EVT_COMMAND(ID_NEXT_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnPrevNextFrame)
	EVT_COMMAND(ID_PREV_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnPrevNextFrame)
	
	EVT_COMMAND(ID_NEW_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnNewFrame)
	EVT_COMMAND(ID_DELETE_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnDeleteFrame)
	
	// upper area
	EVT_NOTEBOOK_PAGE_CHANGED(ID_EDIT_MODE, MainWindow::OnEditModeChanged)
	EVT_COMMAND(ID_CHANGE_DIR, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnChangeDirClicked)
	
	EVT_COMMAND(ID_MOVE_PREVIEW, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnMovePreviewClicked)
	
	EVT_COMMAND(ID_COPY, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnClipboardButtonClicked)
	EVT_COMMAND(ID_PASTE_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnClipboardButtonClicked)
	EVT_COMMAND(ID_PASTE_SPRITE, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnClipboardButtonClicked)
	EVT_COMMAND(ID_SWAP_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnClipboardButtonClicked)
	
	EVT_COMMAND(ID_COPY_LR, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	EVT_COMMAND(ID_COPY_ALL_LR, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	EVT_COMMAND(ID_SWAP_LR, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	EVT_COMMAND(ID_WALK_ANIM, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	EVT_COMMAND(ID_AUTOGEN_FLIP, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	EVT_COMMAND(ID_INSERT_FRAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnTopButtonClicked)
	
	EVT_COMMAND(ID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnSaveButtonClicked)
	EVT_COMMAND(ID_SET_SPRITE_NAME, wxEVT_COMMAND_BUTTON_CLICKED, MainWindow::OnSetSpriteNameClicked)
	
END_EVENT_TABLE()

/*
void c------------------------------() {}
*/


MainWindow::MainWindow()
	: MainWindowGUI((wxWindow *)NULL), fFixTimer(this, 101)
{
extern int initial_zoom;

	mainwin = this;
	fDragging = false;
	fTreeMenu.upforindex = -1;
	
	gd.clipboard.sprite = -1;
	gd.clipboard.frame = -1;
	gd.clipboard.dir = -1;
	gd.nojerk = false;
	fSpritesTextJustGotFocus = false;
	
	// Windows apparently needs to be explicitly told this, or it'll set the
	// background color to a very non-native and awful-looking dark grey.
	#ifdef WIN32
		SetBackgroundColour(*wxLIGHT_GREY);
	#endif
	
	// create status bar
	CreateStatusBar();
	
	// create edit area
	fEditArea = new EditArea(fEditAreaContainer, ID_EditArea, wxPoint(0, 0), fEditAreaContainer->GetSize());
	fEditArea->SetBackground("bkBlue.pbm");
	fEditArea->UpdateCtrlShiftStates();
	fEditArea->SetZoom(initial_zoom);
	
	// create frames area
	fFramesArea = new FramesArea(fFramesAreaContainer, ID_FramesArea, wxPoint(0, 0), fFramesAreaContainer->GetSize());
	
	// and preview area
	fPreviewArea = new PreviewArea(fPreviewAreaContainer, ID_PreviewArea, wxPoint(0, 0), fPreviewAreaContainer->GetSize());
	
	fSpriteNameText->Connect(ID_SPRITE_NAME_TEXT, wxEVT_SET_FOCUS, wxFocusEventHandler(MainWindow::OnSpriteNameTextGotFocus));
	fSpriteNameText->Connect(ID_SPRITE_NAME_TEXT, wxEVT_LEFT_UP, wxMouseEventHandler(MainWindow::OnSpriteNameTextLeftUp));
	
	// must go after control creation or on Windows it crashes because a
	// Size event is generated.
	InitMenuBar();

	// populate controls
	UpdateSpritesTree();
	UpdateSheetsList();
	UpdateDisplay();
	
	fEditArea->SetFocus();
	fFixTimer.Start(325, true);
}

void MainWindow::InitMenuBar()
{
    wxMenuBar *bar = new wxMenuBar;

	wxMenu *file = new wxMenu;
    file->Append(ID_ADD_GROUP, _("New Group..."));
    file->Append(ID_ADD_SPRITESHEET, _("Add Spritesheet..."));
    file->Append(ID_REMOVE_SPRITESHEET, _("Remove Spritesheet..."));
	file->AppendSeparator();
    file->Append(wxID_EXIT, _("E&xit"));

	wxMenu *misc = new wxMenu;
	misc->Append(ID_COPY_DP_FRAMES_DIRS, _("Drawpoint to all frames/dirs"));
	misc->Append(ID_COPY_DP_FRAMES, _("Drawpoint to all frames"));
	misc->Append(ID_COPY_DP_DIRS, _("Drawpoint to all dirs"));
	misc->AppendSeparator();
	misc->Append(ID_SWAP_APS, _("&Swap AP<->AP2"));

    bar->Append(file, _("&File"));
    bar->Append(misc, _("&Misc"));
    SetMenuBar(bar);
}

void MainWindow::OnFixTimer(wxTimerEvent &evt)
{
	wxTreeItemId sel = fSpritesTree->GetSelection();
	if (sel.IsOk())
		fSpritesTree->EnsureVisible(sel);
}

/*
void c------------------------------() {}
*/


void MainWindow::UpdateSpritesTree()
{
wxTreeItemId id, root, parent;
int g, s;
StringList expanded;

	// record which groups are expanded
	wxTreeItemIdValue cookie;
	id = fSpritesTree->GetFirstChild(fSpritesTree->GetRootItem(), cookie);
	while(id)
	{
		if (fSpritesTree->IsExpanded(id))
		{
			// done by strings as group's order or even existance may have
			// been changed (hence would be why were are doing an update).
			const char *groupname = wxStringToChar(fSpritesTree->GetItemText(id));
			expanded.AddString(groupname);
		}
		
		id = fSpritesTree->GetNextChild(fSpritesTree->GetRootItem(), cookie);
	}
	
	
	fSpritesTree->DeleteAllItems();
	fSpritesTree->SetIndent(4);
	
	root = fSpritesTree->AddRoot(_("root"));
	
	// add all groups into tree
	fGroupTreeIDs.MakeEmpty();
	
	for(g=0;;g++)
	{
		const char *groupname = gd.groupnames.StringAt(g);
		if (!groupname) break;
		
		id = fSpritesTree->AppendItem(root, wxString(groupname, wxConvUTF8));
		fSpritesTree->SetItemBold(id, true);
		
		SpriteTreeData *data = new SpriteTreeData;
		data->groupno = g;
		data->isgroup = true;
		
		fSpritesTree->SetItemData(id, data);
		fGroupTreeIDs.AddItem(id);
	}
	
	// add sprites into their groups
	fSpriteTreeIDs.MakeEmpty();
	
	for(s=0;;s++)
	{
		SpriteRecord *sprite = spritelist.SpriteAt(s);
		if (!sprite) break;
		
		// get treeid of the group node this sprite is in
		parent = fGroupTreeIDs.ItemAt(sprite->group);
		
		if (!parent.IsOk())
		{
			staterr("UpdateSpritesTree: sprite %s(%d) references invalid group %d",
					sprite->Name(), s, sprite->group);
			
			wxTreeItemId bogus;	// can't leave a gap
			fSpriteTreeIDs.AddItem(bogus);
		}
		else
		{
			id = fSpritesTree->AppendItem(parent, wxString(sprite->Name(), wxConvUTF8));
			
			SpriteTreeData *data = new SpriteTreeData;
			data->spriteno = s;
			data->isgroup = false;
			
			fSpritesTree->SetItemData(id, data);
			fSpriteTreeIDs.AddItem(id);
		}
	}
	
	// re-expand any groups that were expanded before
	id = fSpritesTree->GetFirstChild(fSpritesTree->GetRootItem(), cookie);
	while(id)
	{
		const char *itemname = wxStringToChar(fSpritesTree->GetItemText(id));
		
		if (expanded.ContainsString(itemname))
			fSpritesTree->Expand(id);
		
		id = fSpritesTree->GetNextChild(fSpritesTree->GetRootItem(), cookie);
	}

}


void MainWindow::UpdateSheetsList()
{
	fSheetsList->Clear();
	for(int i=0;char *str=gd.sheetfiles.StringAt(i);i++)
	{
		fSheetsList->Append(wxString(str, wxConvUTF8));
	}
}


void MainWindow::UpdateDisplay()
{
	fEditArea->Refresh();
	fFramesArea->Refresh();
	fPreviewArea->Refresh();
	
	// ensure gui controls correspond with internal state
	if (GetSpriteSelectedInTree() != gd.cursprite)
	{
		// this helps not to jerk the selection back to the current sprite
		// if we are trying to eg rearrange the groups
		if (!gd.nojerk)
		{
			SetSpriteSelectedInTree(gd.cursprite);
		}
		else gd.nojerk = false;
	}
	
	fSheetsList->SetSelection(CurSprite()->spritesheet);
	
	fSpriteNameText->ChangeValue(wxString(CurSprite()->Name(), wxConvUTF8));
	fCommentsText->ChangeValue(wxString(CurSprite()->Comment(), wxConvUTF8));
	
	fEditModeTabs->SetSelection(gd.curmode);
	SetCurrentDirText();
	
	UpdateTitle();
}


void MainWindow::UpdateTitle()
{
char str[1024];
SpriteRecord *cursprite = CurSprite();
SIFDir *curdir = CurDir();

	DString title("SIFEdit - ");
	
	sprintf(str, "Sprite %d of %d - Frame %d of %d - Dirs %d - Size: [%d, %d]",
		gd.cursprite, spritelist.CountItems(), gd.curframe, cursprite->nframes,
		cursprite->ndirs, cursprite->w, cursprite->h);
	title.AppendString(str);
	
	sprintf(str, " - Sheet Offset %s", curdir->sheet_offset.ToString());
	title.AppendString(str);
	
	/*if (cursprite->ndirs > 1)
	{
		int otherd = (gd.curdir ^ 1);
		if (otherd >= cursprite->ndirs) otherd = 0;
		
		sprintf(str, " / %s", CurFrame()->dir[otherd].sheet_offset.ToString());
		title.AppendString(str);
	}*/
	
	if (!cursprite->spawn_point.equ(0, 0))
	{
		sprintf(str, " - Spawn @ %s", cursprite->spawn_point.ToString());
		title.AppendString(str);
	}
	
	if (!curdir->drawpoint.equ(0, 0))
	{
		sprintf(str, " - Draw %s", curdir->drawpoint.ToString());
		title.AppendString(str);
	}
	
	if (!curdir->actionpoint.equ(0, 0))
	{
		sprintf(str, " - Action %s", curdir->actionpoint.ToString());
		title.AppendString(str);
	}
	
	sprintf(str, " - Zoom x%d", fEditArea->ZoomFactor());
	title.AppendString(str);
	
	SetTitle(wxString(title.String(), wxConvUTF8));
}

/*
void c------------------------------() {}
*/

void MainWindow::OnTreeSelChanged(wxTreeEvent &evt)
{
	// don't mess up cursprite while drag n' dropping items over open folders
	if (fDragging) return;
	
	wxTreeItemId id = evt.GetItem();
	SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(id);
	if (!data) return;
	
	if (!data->isgroup)
	{
		SetCurrentSprite(data->spriteno);
	}
}

void MainWindow::OnTreeEndLabelEdit(wxTreeEvent &evt)
{
	const char *newname = wxStringToChar(evt.GetLabel());
	if (!newname[0])
	{	// empty name entered or user started edit but then canceled
		evt.Veto();
		return;
	}
	
	wxTreeItemId id = evt.GetItem();
	SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(id);
	if (!data) { evt.Veto(); return; }
	
	if (!data->isgroup)
	{	// changing name of a sprite
		int spriteno = data->spriteno;
		
		SpriteRecord *sprite = spritelist.SpriteAt(spriteno);
		if (sprite) sprite->name.SetTo(newname);
	}
	else
	{	// changing name of a group
		int groupno = -1;
		for(int i=0;;i++)
		{
			if (fGroupTreeIDs.ItemAt(i) == id)
			{
				groupno = i;
				break;
			}
		}
		
		if (groupno != -1)
		{
			gd.groupnames.SetString(groupno, newname);
		}
		else
		{
			staterr("Couldn't find groupno for tree label edit");
			evt.Veto();
		}
		
		gd.nojerk = true;
	}
	
	UpdateDisplay();
}


void MainWindow::OnTreeBeginDrag(wxTreeEvent &evt)
{
	wxTreeItemId id = evt.GetItem();
	SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(id);
	
	if (data)
	{
		stat("OnTreeBeginDrag of sprite %d", data->spriteno);
		fSpritesTree->GetSelections(fDragSelections);
		fDragging = true;
		
		evt.Allow();
	}
}


void MainWindow::OnTreeEndDrag(wxTreeEvent &evt)
{
enum { BEFORE, AFTER };
SpriteTreeData *targetdata;
SpriteRecord *target;
int targetindex, insertgroup, insertmode;
int i;

	stat("OnTreeEndDrag()");
	fDragging = false;
	
	// save a pointer to it now as the indexes are gonna get all screwed with
	SpriteRecord *cursprite = CurSprite();
	
	wxTreeItemId targetid = evt.GetItem();
	targetdata = (SpriteTreeData *)fSpritesTree->GetItemData(targetid);
	if (!targetdata) return;
	
	
	if (!targetdata->isgroup)
	{
		// drop target is a sprite. insert just underneath the sprite.
		targetindex = targetdata->spriteno;
		insertgroup = spritelist.SpriteAt(targetindex)->group;
		insertmode = AFTER;
	}
	else
	{
		// drop target is a group header. if the group is expanded and there
		// are already items in it, we'll insert before the first sprite that
		// is using this group #. Otherwise, we'll insert after the last sprite
		// in the list.
		
		// first, assume we'll be inserting at the bottom.
		targetindex = spritelist.CountItems() - 1;
		insertgroup = targetdata->groupno;
		insertmode = AFTER;
		
		// if it's expanded try to find a sprite already in the group that we
		// can insert before.
		if (fSpritesTree->IsExpanded(targetid))
		{
			int firstSpriteInGroup = FindFirstSpriteInGroup(insertgroup);
			if (firstSpriteInGroup != -1)
			{
				targetindex = firstSpriteInGroup;
				insertmode = BEFORE;
				
				// if one of the entries to be moved was the first entry in the group
				// and they are trying to move all the other entries up to the top then
				// this won't work, so to cover that case we will, if possible, instead
				// try to move the items to AFTER the sprite just before the one we want
				// to insert before.
				if (targetindex > 0)
				{
					targetindex--;
					insertmode = AFTER;
				}
			}
		}
	}
	
	// save pointer to target now so we can find it later, as indexes will likely get
	// messed up in a second when we remove items from the list.
	target = spritelist.SpriteAt(targetindex);
	if (!target)
	{
		staterr("OnTreeEndDrag: Couldn't find target pointer for index %d!", targetindex);
		return;
	}
	
	stat("targetindex=%d  insertgroup=%d  insertmode=%s",
		targetindex, insertgroup, insertmode==BEFORE ? "BEFORE":"AFTER");
	
	
	// get the list of what we're going to move
	stat("nitems to move: %d", fDragSelections.Count());
	BList movelist, movegrouplist;
	for(i=0;i<fDragSelections.Count();i++)
	{
		SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(fDragSelections[i]);
		if (!data) continue;
		
		if (!data->isgroup)
		{
			// they selected multiple sprites, and then dropped them onto themselves.
			if (data->spriteno == targetindex)
			{
				staterr("OnTreeEndDrag: One or more sprites dropped onto themselves.");
				return;
			}
			
			stat("Adding sprite %d to movelist", data->spriteno);
			movelist.AddItem(spritelist.SpriteAt(data->spriteno));
		}
		else
		{
			stat("Adding group %d ('%s') to movegrouplist", data->groupno, gd.groupnames.StringAt(data->groupno));
			movegrouplist.AddItem((void *)data->groupno);
		}
	}
	
	// check if they were actually reordering groups instead of moving sprites,
	// and if so, stop the normal processing and take a detour.
	if (movelist.CountItems() == 0)
	{
		if (movegrouplist.CountItems() != 0)
		{
			int insertaftergroup;
			if (targetdata->isgroup)
			{
				insertaftergroup = targetdata->groupno;
				if (fSpritesTree->IsExpanded(targetid) && insertaftergroup > 0)
					insertaftergroup--;
			}
			else
			{
				insertaftergroup = spritelist.SpriteAt(targetdata->spriteno)->group;
			}
			
			stat("moving %d groups to be inserted after group %d ('%s')",
				movegrouplist.CountItems(), insertaftergroup,
				gd.groupnames.StringAt(insertaftergroup));
			
			ReorderGroups(&movegrouplist, insertaftergroup);
			gd.nojerk = true;
			UpdateSpritesTree();
			UpdateDisplay();
			return;
		}
		
		stat("Nothing to do!");
		return;
	}
	
	
	// remove all sprites to be moved from the list
	for(int i=0;i<movelist.CountItems();i++)
	{
		spritelist.RemoveSprite((SpriteRecord *)movelist.ItemAt(i));
	}
	
	// recalculate target sprite index, since it may have been changed
	// when we removed the sprites from the list.
	targetindex = spritelist.IndexOf(target);
	if (targetindex == -1)
	{
		staterr("OnTreeEndDrag: Lost the target!");
		return;
	}
	
	// reinsert all sprites to be moved below the target spriteno
	if (insertmode == AFTER) targetindex++;
	for(int i=0;i<movelist.CountItems();i++)
	{
		SpriteRecord *sprite = (SpriteRecord *)movelist.ItemAt(i);
		sprite->group = insertgroup;
		spritelist.AddSprite(sprite, targetindex++);
	}
	
	stat("old cursprite was %d; name '%s'", gd.cursprite, cursprite->Name());
	
	// recalc index of current sprite in case it got messed with
	gd.cursprite = spritelist.IndexOf(cursprite);
	if (gd.cursprite == -1)
	{
		staterr("OnTreeEndDrag: ERROR: drag operation seems to have killed cursprite");
		gd.cursprite = 0;
	}
	
	stat("new cursprite is %d; '%s'", gd.cursprite,
		CurSprite() ? CurSprite()->Name() : "ERROR");
	
	UpdateSpritesTree();
	UpdateDisplay();
}


void MainWindow::OnTreeMenu(wxTreeEvent &evt)
{
	wxTreeItemId id = evt.GetItem();
	SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(id);
	if (!data) return;
	
	if (data->isgroup)
	{
		fTreeMenu.isgroup = true;
		fTreeMenu.upforindex = data->groupno;
	}
	else
	{
		fTreeMenu.isgroup = false;
		fTreeMenu.upforindex = data->spriteno;
	}
	
	wxMenu *menu = new wxMenu;
	
	if (!data->isgroup)
	{
		// "Send To"
		wxMenu *sendto = new wxMenu;
		for(int i=0;char *name = gd.groupnames.StringAt(i);i++)
		{
			sendto->Append(ID_MENU_SEND_TO+i, wxString(name, wxConvUTF8));
			
			Connect(ID_MENU_SEND_TO+i, wxEVT_COMMAND_MENU_SELECTED,\
				  wxCommandEventHandler(MainWindow::OnTreeMenuResponse));
		}
		menu->Append(wxID_ANY, _("Move To"), sendto);
		
		// "Set Dirs"
		wxMenu *dirsmenu = new wxMenu;
		dirsmenu->AppendRadioItem(ID_MENU_SET_DIRS_1, _T("1"));
		dirsmenu->AppendRadioItem(ID_MENU_SET_DIRS_2, _T("2"));
		dirsmenu->AppendRadioItem(ID_MENU_SET_DIRS_3, _T("3"));
		dirsmenu->AppendRadioItem(ID_MENU_SET_DIRS_4, _T("4"));
		dirsmenu->Check(ID_MENU_SET_DIRS_1 + (CurDirCount() - 1), true);
		menu->Append(wxID_ANY, _("Set Dirs"), dirsmenu);
	}
	
	menu->Append(ID_MENU_DELETE, _T("Delete"));
	
	// and then display
	wxPoint point = evt.GetPoint();
	fSpritesTree->PopupMenu(menu, point.x, point.y);
}

void MainWindow::OnTreeMenuResponse(wxCommandEvent &evt)
{
	if (fTreeMenu.upforindex == -1)
	{
		staterr("MainWindow::OnTreeMenuResponse: what? no upforindex!");
		return;
	}
	
	int id = evt.GetId();
	switch(id)
	{
		case ID_MENU_DELETE:
		{
			if (fTreeMenu.isgroup)
			{
				PromptForGroupDelete(fTreeMenu.upforindex);
			}
			else
			{
				PromptForSpriteDelete(fTreeMenu.upforindex);
			}
		}
		break;
		
		case ID_MENU_SET_DIRS_1:
		case ID_MENU_SET_DIRS_2:
		case ID_MENU_SET_DIRS_3:
		case ID_MENU_SET_DIRS_4:
		{
			if (id == ID_MENU_SET_DIRS_1 && gd.curdir != 0)
			{
				for(int f=0;f<CurSprite()->nframes;f++)
					CurSprite()->frame[f].dir[0] = CurSprite()->frame[f].dir[gd.curdir];
			}
			
			CurSprite()->ndirs = (id - ID_MENU_SET_DIRS_1) + 1;
			
			if (gd.curdir >= CurDirCount())
				gd.curdir = CurDirCount() - 1;
		}
		break;
	}
	
	// handle "Send To..." menu item
	if (id >= ID_MENU_SEND_TO && id < ID_MENU_SEND_TO + gd.groupnames.CountItems())
	{
		int movetogroup = (id - ID_MENU_SEND_TO);
		
		wxArrayTreeItemIds selections;
		fSpritesTree->GetSelections(selections);
		
		for(int i=0;i<selections.Count();i++)
		{
			SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(selections[i]);
			
			if (data && !data->isgroup)
			{
				SpriteRecord *sprite = spritelist.SpriteAt(data->spriteno);
				if (sprite) sprite->group = movetogroup;
			}
		}
		
		gd.nojerk = true;
		UpdateSpritesTree();
	}
	
	fTreeMenu.upforindex = -1;
	UpdateDisplay();
}

/*
void c------------------------------() {}
*/

void MainWindow::OnSheetsListChoice(wxCommandEvent &evt)
{
	int newvalue = evt.GetSelection();
	if (newvalue >= 0 && newvalue < gd.sheetfiles.CountItems())
	{
		CurSprite()->spritesheet = newvalue;
		UpdateDisplay();
	}
}

void MainWindow::OnNewSprite(wxCommandEvent &evt)
{
SpriteRecord *newsprite;
int newindex;

	newindex = spritelist.CountItems();
	newsprite = CurSprite()->Duplicate();
	spritelist.AddSprite(newsprite);
	
	newsprite->name.SetTo(stprintf("Sprite %d", newindex));
	newsprite->comment.SetTo("");
	
	newsprite->SetNumFrames(1);
	newsprite->block_l.count = 0;
	newsprite->block_r.count = 0;
	newsprite->block_u.count = 0;
	newsprite->block_d.count = 0;
	newsprite->spawn_point.set(0, 0);
	newsprite->bbox.set(0, 0, newsprite->w-1, newsprite->h-1);
	newsprite->solidbox.set(0, 0, newsprite->w-1, newsprite->h-1);
	for(int d=0;d<newsprite->ndirs;d++)
	{
		newsprite->frame[0].dir[d].actionpoint.set(0, 0);
		newsprite->frame[0].dir[d].actionpoint2.set(0, 0);
		newsprite->frame[0].dir[d].drawpoint.set(0, 0);
		newsprite->frame[0].dir[d].pf_bbox.set(0, 0, newsprite->w-1, newsprite->h-1);
	}
	
	UpdateSpritesTree();
	SetCurrentSprite(newindex, LEFT);
	fSpriteNameText->SetFocus();
}

void MainWindow::OnDeleteSprite(wxCommandEvent &evt)
{
	PromptForSpriteDelete(gd.cursprite);
}


void MainWindow::OnPrevNextFrame(wxCommandEvent &evt)
{
	if (evt.GetId() == ID_NEXT_FRAME)
	{
		if (++gd.curframe >= CurFrameCount())
			gd.curframe = 0;
	}
	else
	{
		if (--gd.curframe < 0)
			gd.curframe = CurFrameCount() - 1;
	}
	
	UpdateDisplay();
}

void MainWindow::OnNewFrame(wxCommandEvent &evt)
{
	SIFFrame newframe = *CurFrame();
	MoveFrameToNextFrame(&newframe, CurSprite());
	CurSprite()->AddFrame(&newframe);
	
	gd.curframe = (CurFrameCount() - 1);
	UpdateDisplay();
}

void MainWindow::OnDeleteFrame(wxCommandEvent &evt)
{
	if (CurFrameCount() <= 1) { blip(); return; }
	
	CurSprite()->DeleteFrame(gd.curframe);
	if (gd.curframe >= CurFrameCount())
		gd.curframe = CurFrameCount() - 1;
	
	UpdateDisplay();
}

/*
void c------------------------------() {}
*/

void MainWindow::OnEditModeChanged(wxNotebookEvent &evt)
{
	gd.curmode = fEditModeTabs->GetSelection();
	UpdateDisplay();
}

void MainWindow::OnChangeDirClicked(wxCommandEvent &evt)
{
	gd.curdir++;
	if (gd.curdir >= CurDirCount() || gd.curdir >= SIF_MAX_DIRS)
		gd.curdir = 0;
	
	UpdateDisplay();
}

void MainWindow::OnMovePreviewClicked(wxCommandEvent &evt)
{
	fEditArea->NextOffsStop();
}

void MainWindow::OnClipboardButtonClicked(wxCommandEvent &evt)
{
	
	switch(evt.GetId())
	{
		case ID_COPY:
			gd.clipboard.sprite = gd.cursprite;
			gd.clipboard.frame = gd.curframe;
			gd.clipboard.dir = gd.curdir;
		break;
		
		case ID_PASTE_FRAME:
		{
			if (IsValidFrame(gd.clipboard.sprite, gd.clipboard.frame))
			{
				*CurFrame() = spritelist.SpriteAt(gd.clipboard.sprite)->frame[gd.clipboard.frame];
			}
			else blip();
		}
		break;
		
		case ID_PASTE_SPRITE:
		{
			if (IsValidSprite(gd.clipboard.sprite))
			{
				const char *str = stprintf("Sure you wanna Paste Sprite, replacing \"%s\" "
										   "with a duplicate of \"%s\"?",
										CurSprite()->Name(),
									spritelist.SpriteAt(gd.clipboard.sprite)->Name());
				
				if (wxMessageBox(wxString(str, wxConvUTF8), _T("Confirmation"), \
								 wxYES_NO | wxICON_INFORMATION, this) == wxYES)
				{
					if (gd.clipboard.sprite != gd.cursprite)
					{
						// call SIFSprite's copyfrom so we don't copy over the name
						// or change the group, etc.
						((SIFSprite *)CurSprite())->CopyFrom( \
							spritelist.SpriteAt(gd.clipboard.sprite));
					}
				}
			}
			else blip();
		}
		break;
		
		case ID_SWAP_FRAME:
		{
			if (IsValidFrame(gd.clipboard.sprite, gd.clipboard.frame))
			{
				SIFFrame temp;
				SIFFrame *other;
				
				other = &spritelist.SpriteAt(gd.clipboard.sprite)->frame[gd.clipboard.frame];
				
				temp = *other;
				*other = *CurFrame();
				*CurFrame() = temp;
			}
		}
		break;
		
		default:
			staterr("Unhandled clipboard action %d", evt.GetId());
		break;
	}
	
	if (evt.GetId() != ID_COPY)
		UpdateDisplay();
}

void MainWindow::OnTopButtonClicked(wxCommandEvent &evt)
{
int srcdir, destdir;

	SpriteRecord *cursprite = CurSprite();
	srcdir = gd.curdir;
	destdir = -1;
	
	if (cursprite->ndirs >= 2)
	{
		if (srcdir == LEFT) destdir = RIGHT;
		else if (srcdir == RIGHT) destdir = LEFT;
	}
	
	if (destdir == -1 && cursprite->ndirs >= 4)
	{
		if (srcdir == UP) destdir = DOWN;
		else if (srcdir == DOWN) destdir = UP;
	}
	
	int evtid = evt.GetId();
	switch(evtid)
	{
		case ID_COPY_LR:
		{
			if (destdir == -1) { blip(); return; }
			CurFrame()->dir[destdir] = CurFrame()->dir[srcdir];
		}
		break;
		
		case ID_COPY_ALL_LR:
		{
			if (destdir == -1) { blip(); return; }
			for(int f=0;f<cursprite->nframes;f++)
			{
				cursprite->frame[f].dir[destdir] = cursprite->frame[f].dir[srcdir];
			}
		}
		break;
		
		case ID_SWAP_LR:
		{
			if (destdir == -1) { blip(); return; }
			for(int f=0;f<cursprite->nframes;f++)
			{
				SIFDir temp = cursprite->frame[f].dir[destdir];
				cursprite->frame[f].dir[destdir] = cursprite->frame[f].dir[srcdir];
				cursprite->frame[f].dir[srcdir] = temp;
			}
		}
		break;
		
		case ID_AUTOGEN_FLIP:
		{
			if (destdir == -1) { blip(); return; }
			
			int delta = (gd.curdir & 1) ? cursprite->h : -cursprite->h;
			for(int f=0;f<cursprite->nframes;f++)
			{
				cursprite->frame[f].dir[destdir] = cursprite->frame[f].dir[srcdir];
				cursprite->frame[f].dir[destdir].sheet_offset.y += delta;
			}
			
			gd.curdir = destdir;
		}
		break;
		
		// a tool for the standing frame in-between the 1st and 2nd frames of a walk anim.
		// the current frame is set to the first frame of the walk anim.
		// our job is thus to insert a copy of frame 0 at curframe+1 and
		// (what will then be) curframe+3.
		case ID_WALK_ANIM:
		{
			cursprite->InsertFrame(&cursprite->frame[0], gd.curframe + 1);
			cursprite->InsertFrame(&cursprite->frame[0], gd.curframe + 3);
		}
		break;
		
		case ID_INSERT_FRAME:
		{
			SIFFrame newframe = *CurFrame();
			MoveFrameToNextFrame(&newframe, CurSprite());
			
			cursprite->InsertFrame(&newframe, gd.curframe);
		}
		break;
		
		case ID_COPY_DP_FRAMES:
		case ID_COPY_DP_DIRS:
		case ID_COPY_DP_FRAMES_DIRS:
		{
			SIFPoint dp = CurDir()->drawpoint;
			
			for(int f=0;f<cursprite->nframes;f++)
			{
				if (evtid == ID_COPY_DP_DIRS && f != gd.curframe) continue;
				
				for(int d=0;d<cursprite->ndirs;d++)
				{
					if (evtid == ID_COPY_DP_FRAMES && d != gd.curdir) continue;
					
					cursprite->frame[f].dir[d].drawpoint = dp;
				}
			}
		}
		break;
		
		case ID_SWAP_APS:
		{
			SIFPoint temp = CurDir()->actionpoint;
			CurDir()->actionpoint = CurDir()->actionpoint2;
			CurDir()->actionpoint2 = temp;
		}
		break;
		
		default:
			staterr("Unhandled LR-button action %d", evt.GetId());
		break;
	}
	
	UpdateDisplay();
}

/*
void c------------------------------() {}
*/

void MainWindow::OnSaveButtonClicked(wxCommandEvent &evt)
{
	if (save_sif(gd.openfilename))
	{
		wxMessageBox(_T("Save failed! Please check command line for details."), \
					 _T("Error"), wxOK | wxICON_ERROR, this);
	}
}

void MainWindow::OnCommentChanged(wxCommandEvent &evt)
{
	CurSprite()->comment.SetTo(wxStringToChar(fCommentsText->GetValue()));
}

void MainWindow::OnSetSpriteNameClicked(wxCommandEvent &evt)
{
stat("%08x %08x", fSpriteNameText, this);
	char *newname = wxStringToChar(fSpriteNameText->GetValue());
	if (!newname || !newname[0])
		newname = stprintf("Sprite %d", gd.cursprite);
	
	CurSprite()->name.SetTo(newname);
	
	UpdateSpritesTree();
	UpdateDisplay();
}

// this is a workaround for an apparent bug in GTK
void MainWindow::OnSpriteNameTextGotFocus(wxFocusEvent &evt)
{
	// a terrible hack. we can't trust "this" pointer as this function is being
	// called in context of fSpriteNameText, not really MainWindow.
	mainwin->fSpritesTextJustGotFocus = true;
}

void MainWindow::OnSpriteNameTextLeftUp(wxMouseEvent &evt)
{
	if (mainwin->fSpritesTextJustGotFocus)
	{
		mainwin->fSpritesTextJustGotFocus = false;
		mainwin->fSpriteNameText->SetSelection(-1, -1);
	}
	
	evt.Skip();//wxTextCtrl::
}

/*
void c------------------------------() {}
*/

void MainWindow::OnAddGroup(wxCommandEvent &evt)
{
	wxString wxname = wxGetTextFromUser(_("So what would you like to call the new group?"),
									_("Create group"), _(""), this);
	const char *name = wxStringToChar(wxname);
	if (!name || !name[0]) return;
	
	gd.groupnames.AddString(name);
	UpdateSpritesTree();
}

void MainWindow::OnAddSpritesheet(wxCommandEvent &evt)
{
	wxString wxname = wxGetTextFromUser(_("Input path to new spritesheet or else!"),
									_("Add spritesheet"), _(""), this);
	const char *name = wxStringToChar(wxname);
	if (!name || !name[0]) return;
	
	gd.sheetfiles.AddString(name);
	
	UpdateSheetsList();
	UpdateDisplay();
}

void MainWindow::OnRemoveSpritesheet(wxCommandEvent &evt)
{
int i, sheetno, usedby, count;

	// ask them which one they want deleted
	wxArrayString choices;
	for(i=0;char *name = gd.sheetfiles.StringAt(i);i++)
	{
		DString str(name);
		str.AppendString(stprintf(" (%d)", CountSpritesheetUsage(i)));
		
		choices.Add(wxString(str.String(), wxConvUTF8));
	}
	
ask_again: ;
	sheetno = wxGetSingleChoiceIndex(_("Which spritesheet do you want removed?"),
									 _("Remove spritesheet"),
									 choices, this);
	if (sheetno == -1) return;
	
	if ((count = CountSpritesheetUsage(sheetno, &usedby)) > 0)
	{
		char str[1024];
		sprintf(str, "Sheet still in use by %s%s", \
			GetSpriteHeaderName(spritelist.SpriteAt(usedby)->Name()),
			(count <= 1) ? "." : ", among others.");
		
		wxMessageBox(wxString(str, wxConvUTF8),
					 _T("Error"), wxOK | wxICON_ERROR, this);
		goto ask_again;
	}
	
	RemoveSpritesheet(sheetno);
	UpdateSheetsList();
	UpdateDisplay();
}


/*
void c------------------------------() {}
*/

void MainWindow::SetCurrentSprite(int spr, int dir)
{
	if (spr < 0 || spr >= spritelist.CountItems())
	{
		staterr("MainWindow::SetCurrentSprite: invalid sprite index %d", spr);
		return;
	}
	
	if (gd.cursprite != spr)
	{
		gd.cursprite = spr;
		gd.curframe = 0;
		
		gd.curdir = dir;
		if (gd.curdir >= CurDirCount())
			gd.curdir = CurDirCount() - 1;
	}
	
	UpdateDisplay();
	fSpritesTree->SetFocus();	// focus back to tree so keyboard navigation works
}

void MainWindow::PromptForSpriteDelete(int delno)
{
	if (delno < 0 || delno >= spritelist.CountItems())
	{
		staterr("PromptForSpriteDelete: invalid delno %d", delno);
		return;
	}
	
	if (spritelist.CountItems() <= 1)
	{
		wxMessageBox(_T("You can't delete this sprite, because it's the last sprite left in the file."), \
					 _T("Error"), wxOK | wxICON_ERROR, this);
		
		return;
	}
	
	char str[10000];
	sprintf(str, "Permanently delete %s?", GetSpriteHeaderName(spritelist.SpriteAt(delno)->Name()));
	
	int result = wxMessageBox(wxString(str, wxConvUTF8), \
				 			_T("Confirmation"), \
							wxYES_NO | wxICON_INFORMATION, this);
	if (result != wxYES) return;
	
	// delete the sprite
	spritelist.DeleteSprite(delno);
	
	if (gd.cursprite >= spritelist.CountItems())
		gd.cursprite--;
	
	UpdateSpritesTree();
	SetCurrentSprite(gd.cursprite);
	UpdateDisplay();
}

void MainWindow::PromptForGroupDelete(int delgroup)
{
char str[10000];

	if (delgroup < 0 || delgroup >= gd.groupnames.CountItems())
	{
		staterr("PromptForGroupDelete: invalid delno %d", delgroup);
		return;
	}
	
	if (gd.groupnames.CountItems() <= 1)
	{
		wxMessageBox(_T("You can't delete this group, because it's the only group in the file and there must be at least one group."), \
					 _T("Error"), wxOK | wxICON_ERROR, this);
		
		return;
	}
	
	int kickgroup = 0;
	if (delgroup == 0) kickgroup = 1;
	
	if (FindFirstSpriteInGroup(delgroup) != -1)
	{
		sprintf(str, "Delete the group \"%s\"?", gd.groupnames.StringAt(delgroup));
		
		strcat(str, " All it's members will be kicked out to \"");
		strcat(str, gd.groupnames.StringAt(kickgroup));
		strcat(str, "\".");
		
		int result = wxMessageBox(wxString(str, wxConvUTF8), \
					 			_T("Confirmation"), \
								wxYES_NO | wxICON_EXCLAMATION, this);
		
		if (result != wxYES)
			return;
	}
	
	DeleteGroup(delgroup, kickgroup);
	
	UpdateSpritesTree();
	UpdateDisplay();
}

void MainWindow::SetCurrentDirText()
{
	char buf[80];
	static const char *dirnames[] = { "RIGHT", "LEFT", "UP", "DOWN" };
	bool enable = true;
	
	if (CurDirCount() > 1)
	{
		sprintf(buf, "Showing %s-FACING frame", dirnames[gd.curdir]);
	}
	else
	{
		strcpy(buf, "Showing THE ONLY frame");
		enable = false;
	}
	
	fChangeDirButton->Enable(enable);
	fCopyLRButton->Enable(enable);
	fCopyAllDirButton->Enable(enable);
	fAutogenFlipButton->Enable(enable);
	
	wxString newlabel = wxString(buf, wxConvUTF8);
	if (fChangeDirButton->GetLabel() != newlabel)
		fChangeDirButton->SetLabel(newlabel);
}

/*
void c------------------------------() {}
*/

int MainWindow::GetSpriteSelectedInTree()
{
	wxTreeItemId id = fSpritesTree->GetSelection();
	SpriteTreeData *data = (SpriteTreeData *)fSpritesTree->GetItemData(id);
	if (data && !data->isgroup) return data->spriteno;
	
	return -1;	// selection on e.g. one of the group nodes
}

bool MainWindow::SetSpriteSelectedInTree(int sprite)
{
	wxTreeItemId id = fSpriteTreeIDs.ItemAt(sprite);
	if (!id.IsOk()) return 1;
	
	fSpritesTree->UnselectAll();
	fSpritesTree->SelectItem(id);
	return 0;
}

/*
void c------------------------------() {}
*/

void MainWindow::MoveFrameToNextFrame(SIFFrame *frame, SIFSprite *sprite)
{
	int max_x = (sheetmgr.SpriteSheetWidthFor(sprite->spritesheet) - sprite->w);
	
	for(int d=0;d<sprite->ndirs;d++)
		frame->dir[d].sheet_offset.x += sprite->w;
	
	if (frame->dir[gd.curdir].sheet_offset.x > max_x)
	{
		for(int d=0;d<sprite->ndirs;d++)
		{
			frame->dir[d].sheet_offset.x = 0;
			frame->dir[d].sheet_offset.y += sprite->h;
		}
	}
}

/*
void c------------------------------() {}
*/

bool MainWindow::OnKeyDown(wxKeyEvent &event)
{
	int kc = event.GetKeyCode();
	
	if (kc == WXK_CONTROL)
	{
		gd.ctrl = true;
		fEditArea->UpdateCtrlShiftStates();
	}
	else if (kc == WXK_SHIFT)
	{
		gd.shift = true;
		fEditArea->UpdateCtrlShiftStates();
	}
	else if (kc == WXK_ESCAPE)
	{
		Close(true);
		return 1;
	}
	
	return 0;
}

bool MainWindow::OnKeyUp(wxKeyEvent &event)
{
	int kc = event.GetKeyCode();
	if (kc == WXK_CONTROL)
	{
		gd.ctrl = false;
		fEditArea->UpdateCtrlShiftStates();
	}
	else if (kc == WXK_SHIFT)
	{
		gd.shift = false;
		fEditArea->UpdateCtrlShiftStates();
	}
	
}

void MainWindow::OnSize(wxSizeEvent &evt)
{
	MainWindowGUI::OnSize(evt);
	
	fEditArea->SetSize(fEditAreaContainer->GetSize());
	fFramesArea->SetSize(fFramesAreaContainer->GetSize());
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

/*void MainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	const char *str = "Howdy";
	
	wxMessageBox(wxString(str, wxConvUTF8), \
				 _T("About Hello World"), \
				 wxOK | wxICON_INFORMATION, this);
}*/




