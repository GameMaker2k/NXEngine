///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 11 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainGUI.h"

///////////////////////////////////////////////////////////////////////////

MainWindowGUI::MainWindowGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bGlobalHoz;
	bGlobalHoz = new wxBoxSizer( wxHORIZONTAL );
	
	
	bGlobalHoz->Add( 4, 0, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bLeftHandColumn;
	bLeftHandColumn = new wxBoxSizer( wxVERTICAL );
	
	bLeftHandColumn->SetMinSize( wxSize( 160,-1 ) ); 
	
	bLeftHandColumn->Add( 0, 3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bTopLeftCorner;
	bTopLeftCorner = new wxBoxSizer( wxHORIZONTAL );
	
	fPreviewAreaContainer = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
	fPreviewAreaContainer->SetMinSize( wxSize( -1,128 ) );
	
	bTopLeftCorner->Add( fPreviewAreaContainer, 1, wxEXPAND | wxALL, 5 );
	
	bLeftHandColumn->Add( bTopLeftCorner, 0, wxEXPAND, 5 );
	
	
	bLeftHandColumn->Add( 0, 12, 0, wxEXPAND, 5 );
	
	wxArrayString fSheetsListChoices;
	fSheetsList = new wxChoice( this, ID_SHEETS_LIST, wxDefaultPosition, wxDefaultSize, fSheetsListChoices, 0 );
	fSheetsList->SetSelection( 0 );
	bLeftHandColumn->Add( fSheetsList, 0, wxALL|wxEXPAND, 3 );
	
	
	bLeftHandColumn->Add( 0, 2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSpritePlusMinus;
	bSpritePlusMinus = new wxBoxSizer( wxHORIZONTAL );
	
	fNewSpriteButton = new wxButton( this, ID_NEW_SPRITE, _("New &Sprite"), wxDefaultPosition, wxDefaultSize, 0 );
	bSpritePlusMinus->Add( fNewSpriteButton, 1, wxALL, 3 );
	
	fDeleteSpriteButton = new wxButton( this, ID_DELETE_SPRITE, _("- Sprite"), wxDefaultPosition, wxDefaultSize, 0 );
	bSpritePlusMinus->Add( fDeleteSpriteButton, 1, wxALL, 3 );
	
	bLeftHandColumn->Add( bSpritePlusMinus, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bFrameControls;
	bFrameControls = new wxBoxSizer( wxHORIZONTAL );
	
	fNewFrameButton = new wxButton( this, ID_NEW_FRAME, _("+ Fra"), wxDefaultPosition, wxDefaultSize, 0 );
	fNewFrameButton->SetMinSize( wxSize( 0,-1 ) );
	
	bFrameControls->Add( fNewFrameButton, 1, wxALL, 3 );
	
	fDeleteFrameButton = new wxButton( this, ID_DELETE_FRAME, _("- Fra"), wxDefaultPosition, wxDefaultSize, 0 );
	fDeleteFrameButton->SetMinSize( wxSize( 0,-1 ) );
	
	bFrameControls->Add( fDeleteFrameButton, 1, wxALL, 3 );
	
	fPrevFrameButton = new wxButton( this, ID_PREV_FRAME, _("<"), wxDefaultPosition, wxSize( 24,-1 ), 0 );
	fPrevFrameButton->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bFrameControls->Add( fPrevFrameButton, 0, wxALL, 3 );
	
	fNextFrameButton = new wxButton( this, ID_NEXT_FRAME, _(">"), wxDefaultPosition, wxSize( 24,-1 ), 0 );
	fNextFrameButton->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bFrameControls->Add( fNextFrameButton, 0, wxBOTTOM|wxRIGHT|wxTOP, 3 );
	
	bLeftHandColumn->Add( bFrameControls, 0, wxEXPAND, 5 );
	
	
	bLeftHandColumn->Add( 0, 16, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSpriteNameSetter;
	bSpriteNameSetter = new wxBoxSizer( wxHORIZONTAL );
	
	fSpriteNameText = new wxTextCtrl( this, ID_SPRITE_NAME_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSpriteNameSetter->Add( fSpriteNameText, 1, wxALL, 3 );
	
	fSetSpriteNameButton = new wxButton( this, ID_SET_SPRITE_NAME, _("Set"), wxDefaultPosition, wxSize( -1,25 ), 0 );
	fSetSpriteNameButton->SetMinSize( wxSize( 32,25 ) );
	
	bSpriteNameSetter->Add( fSetSpriteNameButton, 0, wxALL, 3 );
	
	bLeftHandColumn->Add( bSpriteNameSetter, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bTreeSizer;
	bTreeSizer = new wxBoxSizer( wxVERTICAL );
	
	fSpritesTree = new wxTreeCtrl( this, ID_SPRITES_TREE, wxDefaultPosition, wxDefaultSize, wxTR_EDIT_LABELS|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_LINES_AT_ROOT|wxTR_MULTIPLE|wxSUNKEN_BORDER|wxVSCROLL );
	bTreeSizer->Add( fSpritesTree, 1, wxALL|wxEXPAND, 3 );
	
	bLeftHandColumn->Add( bTreeSizer, 1, wxEXPAND, 5 );
	
	fCommentsText = new wxTextCtrl( this, ID_COMMENTS_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	fCommentsText->SetMinSize( wxSize( -1,48 ) );
	
	bLeftHandColumn->Add( fCommentsText, 0, wxALL|wxEXPAND, 3 );
	
	bGlobalHoz->Add( bLeftHandColumn, 0, wxEXPAND, 5 );
	
	
	bGlobalHoz->Add( 4, 0, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bRightHandSide;
	bRightHandSide = new wxBoxSizer( wxVERTICAL );
	
	
	bRightHandSide->Add( 0, 1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bUpperArea;
	bUpperArea = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* bLeftGrid;
	bLeftGrid = new wxGridSizer( 2, 2, 0, 0 );
	
	fInsertFrame = new wxButton( this, ID_INSERT_FRAME, _("Ins. Frame"), wxDefaultPosition, wxDefaultSize, 0 );
	bLeftGrid->Add( fInsertFrame, 0, wxALL|wxEXPAND, 3 );
	
	fMovePreviewButton = new wxButton( this, ID_MOVE_PREVIEW, _("Move Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	bLeftGrid->Add( fMovePreviewButton, 0, wxALL|wxEXPAND, 3 );
	
	fCopyButton = new wxButton( this, ID_COPY, _("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	bLeftGrid->Add( fCopyButton, 0, wxALL|wxEXPAND, 3 );
	
	fPasteFrameButton = new wxButton( this, ID_PASTE_FRAME, _("Paste Frame"), wxDefaultPosition, wxDefaultSize, 0 );
	bLeftGrid->Add( fPasteFrameButton, 0, wxALL|wxEXPAND, 3 );
	
	bUpperArea->Add( bLeftGrid, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bMiddleArea;
	bMiddleArea = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bBelowBig;
	bBelowBig = new wxBoxSizer( wxHORIZONTAL );
	
	fPasteSpriteButton = new wxButton( this, ID_PASTE_SPRITE, _("Paste Sprite"), wxDefaultPosition, wxDefaultSize, 0 );
	bBelowBig->Add( fPasteSpriteButton, 0, wxALL, 3 );
	
	fSwapFrameButton = new wxButton( this, ID_SWAP_FRAME, _("XChg Frame"), wxDefaultPosition, wxDefaultSize, 0 );
	bBelowBig->Add( fSwapFrameButton, 0, wxALL, 3 );
	
	fSaveButton = new wxButton( this, ID_SAVE, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bBelowBig->Add( fSaveButton, 1, wxALL, 3 );
	
	bMiddleArea->Add( bBelowBig, 1, wxEXPAND, 5 );
	
	fChangeDirButton = new wxButton( this, ID_CHANGE_DIR, _("Showing RIGHT-FACING frame"), wxDefaultPosition, wxDefaultSize, 0 );
	bMiddleArea->Add( fChangeDirButton, 0, wxALL|wxEXPAND, 3 );
	
	bUpperArea->Add( bMiddleArea, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bRightButtons;
	bRightButtons = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bRightTopSizer;
	bRightTopSizer = new wxBoxSizer( wxHORIZONTAL );
	
	fSwapDirButton = new wxButton( this, ID_SWAP_LR, _("Swap LR"), wxDefaultPosition, wxDefaultSize, 0 );
	bRightTopSizer->Add( fSwapDirButton, 0, wxALL, 3 );
	
	fWalkAnimButton = new wxButton( this, ID_WALK_ANIM, _("Walk Anim"), wxDefaultPosition, wxDefaultSize, 0 );
	bRightTopSizer->Add( fWalkAnimButton, 0, wxALL, 3 );
	
	fLinkBBoxCheckbox = new wxCheckBox( this, ID_LINK_BBOX, _("BB/SB"), wxDefaultPosition, wxDefaultSize, 0 );
	fLinkBBoxCheckbox->SetValue(true);
	
	bRightTopSizer->Add( fLinkBBoxCheckbox, 0, wxALL, 5 );
	
	bRightButtons->Add( bRightTopSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bRightLowerSizer;
	bRightLowerSizer = new wxBoxSizer( wxHORIZONTAL );
	
	fCopyLRButton = new wxButton( this, ID_COPY_LR, _("Copy LR"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bRightLowerSizer->Add( fCopyLRButton, 0, wxALL, 3 );
	
	fCopyAllDirButton = new wxButton( this, ID_COPY_ALL_LR, _("Copy ALL LR"), wxDefaultPosition, wxDefaultSize, 0 );
	bRightLowerSizer->Add( fCopyAllDirButton, 0, wxALL, 3 );
	
	fAutogenFlipButton = new wxButton( this, ID_AUTOGEN_FLIP, _("Autogen Flip"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bRightLowerSizer->Add( fAutogenFlipButton, 0, wxALL, 3 );
	
	bRightButtons->Add( bRightLowerSizer, 0, wxEXPAND, 3 );
	
	bUpperArea->Add( bRightButtons, 0, wxEXPAND, 5 );
	
	bRightHandSide->Add( bUpperArea, 0, wxEXPAND, 5 );
	
	
	bRightHandSide->Add( 0, 8, 0, wxEXPAND, 5 );
	
	fEditModeTabs = new wxNotebook( this, ID_EDIT_MODE, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel552 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel552->SetMinSize( wxSize( 0,0 ) );
	m_panel552->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel552, _("View"), false );
	m_panel55 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel55->SetMinSize( wxSize( 0,0 ) );
	m_panel55->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel55, _("Bounding Box"), false );
	m_panel5 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel5->SetMinSize( wxSize( 0,0 ) );
	m_panel5->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel5, _("Block L"), false );
	m_panel6 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel6->SetMinSize( wxSize( 0,0 ) );
	m_panel6->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel6, _("Block R"), false );
	m_panel553 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel553->SetMinSize( wxSize( 0,0 ) );
	m_panel553->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel553, _("Block U"), false );
	m_panel51 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel51->SetMinSize( wxSize( 0,0 ) );
	m_panel51->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel51, _("Block D"), false );
	m_panel52 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel52->SetMinSize( wxSize( 0,0 ) );
	m_panel52->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel52, _("Action Pt"), false );
	m_panel53 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel53->SetMinSize( wxSize( 0,0 ) );
	m_panel53->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel53, _("Action2"), true );
	m_panel54 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel54->SetMinSize( wxSize( 0,0 ) );
	m_panel54->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel54, _("Spawn Point"), false );
	m_panel541 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel541->SetMinSize( wxSize( 0,0 ) );
	m_panel541->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel541, _("Draw Pt"), false );
	m_panel551 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel551->SetMinSize( wxSize( 0,0 ) );
	m_panel551->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel551, _("SolidBox"), false );
	m_panel531 = new wxPanel( fEditModeTabs, wxID_ANY, wxDefaultPosition, wxSize( 0,0 ), wxTAB_TRAVERSAL );
	m_panel531->SetMinSize( wxSize( 0,0 ) );
	m_panel531->SetMaxSize( wxSize( 0,0 ) );
	
	fEditModeTabs->AddPage( m_panel531, _("PFBox"), false );
	
	bRightHandSide->Add( fEditModeTabs, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 3 );
	
	wxBoxSizer* bBoxAndFrames;
	bBoxAndFrames = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	fFramesAreaContainer = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	fFramesAreaContainer->SetMinSize( wxSize( 107,-1 ) );
	
	bSizer18->Add( fFramesAreaContainer, 1, wxEXPAND | wxALL, 3 );
	
	bBoxAndFrames->Add( bSizer18, 0, wxEXPAND, 3 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	fEditAreaContainer = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
	bSizer17->Add( fEditAreaContainer, 1, wxALL|wxEXPAND, 3 );
	
	bBoxAndFrames->Add( bSizer17, 1, wxEXPAND, 5 );
	
	bRightHandSide->Add( bBoxAndFrames, 1, wxEXPAND, 5 );
	
	bGlobalHoz->Add( bRightHandSide, 1, wxEXPAND, 5 );
	
	this->SetSizer( bGlobalHoz );
	this->Layout();
}

MainWindowGUI::~MainWindowGUI()
{
}
