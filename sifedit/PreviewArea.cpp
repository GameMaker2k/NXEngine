
#include <wx/wx.h>
#include <stdint.h>
#include "sifedit.h"
#include "PreviewArea.h"
#include "MainWindow.h"
#include "PreviewArea.fdh"

BEGIN_EVENT_TABLE(PreviewArea, wxPanel)
	EVT_PAINT(PreviewArea::OnPaint)
	EVT_LEFT_DOWN(PreviewArea::OnClick)
END_EVENT_TABLE()

/*
void c------------------------------() {}
*/

PreviewArea::PreviewArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
	: wxPanel(parent, id, pos, size)
{
	fSpritesheetBitmap = NULL;
	fSheetNo = -1;
}

PreviewArea::~PreviewArea()
{
}

/*
void c------------------------------() {}
*/

void PreviewArea::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	
	SpriteRecord *cursprite = CurSprite();
	if (!cursprite) return;
	
	int required_sheetno = cursprite->spritesheet;
	if (!fSpritesheetBitmap || fSheetNo != required_sheetno)
	{
		if (LoadSpritesheet(required_sheetno))
			return;
	}
	
	int boxwidth = GetSize().GetWidth();
	int boxheight = GetSize().GetHeight();
	
	wxBitmap bbbmp(boxwidth, boxheight);
	wxMemoryDC bbdc(bbbmp);
	
	// clear everything
	bbdc.SetPen(*wxTRANSPARENT_PEN);
	
	bbdc.SetBrush(wxBrush(wxColour(0, 0, 41)));
	bbdc.DrawRectangle(0, 0, boxwidth, boxheight);
	
	wxBrush hash = wxBrush(wxColour(100, 100, 181));
	hash.SetStyle(wxCROSSDIAG_HATCH);
	
	bbdc.SetBrush(hash);
	bbdc.DrawRectangle(0, 0, boxwidth, boxheight);
	
	// blit in current spritesheet
	// center current sprite in it
	int x, y;
	
	x = CurDir()->sheet_offset.x;
	y = CurDir()->sheet_offset.y;
	
	x -= boxwidth / 2;
	y -= boxheight / 2;
	
	x += cursprite->w / 2;
	y += cursprite->h / 2;
	
	bbdc.DrawBitmap(*fSpritesheetBitmap, -x, -y);
	
	// draw guidelines for area used by sprite
	/*int x1, y1, x2, y2;
	
	x1 = boxwidth / 2;
	y1 = boxheight / 2;
	
	x1 -= cursprite->w / 2;
	x2 = x1 + cursprite->w;
	
	y1 -= cursprite->h / 2;
	y2 = y1 + cursprite->h;
	
	bbdc.SetPen(wxPen(wxColour(120, 120, 190)));
	bbdc.DrawLine(x1, 0, x1, boxheight);
	bbdc.DrawLine(x2, 0, x2, boxheight);
	bbdc.DrawLine(0, y1, boxwidth, y1);
	bbdc.DrawLine(0, y2, boxwidth, y2);*/
	
	dc.Blit(0, 0, bbdc.GetSize().GetWidth(), bbdc.GetSize().GetHeight(), &bbdc, 0, 0);
}


bool PreviewArea::LoadSpritesheet(int sheetno)
{
	fSheetNo = -1;
	delete fSpritesheetBitmap;
	fSpritesheetBitmap = NULL;
	
	const char *filename = gd.sheetfiles.StringAt(sheetno);
	if (!filename)
	{
		staterr("PreviewArea::LoadSpritesheet: filename unknown for sheet # %d", sheetno);
		return 1;
	}
	
	wxImage spritesheet;
	if (load_spritesheet(&spritesheet, filename))
	{
		staterr("PreviewArea::LoadSpritesheet: failed to load file '%s'", filename);
		return 1;
	}
	
	fSpritesheetBitmap = new wxBitmap(spritesheet);
	fSheetNo = sheetno;
	return 0;
}

/*
void c------------------------------() {}
*/

void PreviewArea::OnClick(wxMouseEvent &event)
{
	if (save_sif(gd.openfilename))
	{
		wxMessageBox(_T("Save failed! Please check command line for details."), \
					 _T("Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	
	mainwin->Close(true);
}













