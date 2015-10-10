
#include <wx/wx.h>
#include "sifedit.h"
#include "SheetManager.h"
#include "SheetManager.fdh"

SheetManager::SheetManager()
{
	fSpriteSheet = NULL;
	fSheetNo = -1;
}

SheetManager::~SheetManager()
{
	delete fSpriteSheet;
	fSpriteSheet = NULL;
}

/*
void c------------------------------() {}
*/

bool SheetManager::LoadSpritesheet(int sheetno)
{
	if (fSheetNo == sheetno && fSpriteSheet) return 0;
	fSheetNo = -1;
	
	delete fSpriteSheet;
	fSpriteSheet = NULL;
	
	const char *filename = gd.sheetfiles.StringAt(sheetno);
	if (!filename)
	{
		staterr("LoadSpritesheet: filename unknown for sheet # %d", sheetno);
		return 1;
	}
	
	fSpriteSheet = new wxImage;
	
	if (load_spritesheet(fSpriteSheet, filename))
	{
		staterr("LoadSpritesheet: failed to load file '%s'", filename);
		delete fSpriteSheet;
		fSpriteSheet = NULL;
		return 1;
	}
	
	fSheetNo = sheetno;
	return 0;
}


wxImage *SheetManager::GetSubImage(int x, int y, int w, int h)
{
	// create a bitmap and memorydc and fill it with background
	wxBitmap membmp(w, h);
	wxMemoryDC memdc(membmp);
	
	memdc.SetPen(*wxTRANSPARENT_PEN);
	
	memdc.SetBrush(wxBrush(wxColour(255, 0, 255)));
	memdc.DrawRectangle(0, 0, w, h);
	
	wxBrush hash = *wxWHITE_BRUSH;
	hash.SetStyle(wxCROSSDIAG_HATCH);
	
	memdc.SetBrush(hash);
	memdc.DrawRectangle(0, 0, w, h);
	
	// calculate clipping in case the requested image is partially off the spritesheet
	int src_x1 = x;
	int src_y1 = y;
	int src_x2 = src_x1 + (w - 1);
	int src_y2 = src_y1 + (h - 1);
	int dst_x = 0;
	int dst_y = 0;
	bool nodraw = false;
	
	if (src_x1 < 0)
	{
		dst_x = -src_x1;
		src_x1 = 0;
		if (dst_x >= w) nodraw = true;
	}
	
	if (src_y1 < 0)
	{
		dst_y = -src_y1;
		src_y1 = 0;
		if (dst_y >= h) nodraw = true;
	}
	
	if (src_x2 >= fSpriteSheet->GetWidth()) src_x2 = (fSpriteSheet->GetWidth() - 1);
	if (src_y2 >= fSpriteSheet->GetHeight()) src_y2 = (fSpriteSheet->GetHeight() - 1);
	if (src_x1 >= fSpriteSheet->GetWidth()) nodraw = true;
	if (src_y1 >= fSpriteSheet->GetHeight()) nodraw = true;
	if (src_x2 < 0) nodraw = true;
	if (src_y2 < 0) nodraw = true;
	
	int src_w = (src_x2 - src_x1) + 1;
	int src_h = (src_y2 - src_y1) + 1;
	
	// obtain the portion of valid data and blit it in
	if (!nodraw)
	{
		wxImage *sub = GetSubImageDirect(src_x1, src_y1, src_w, src_h);
		if (sub)
		{
			memdc.DrawBitmap(wxBitmap(*sub), dst_x, dst_y);
		}
	}
	
	// convert our bitmap to an image and return
	wxImage *result = new wxImage(membmp.ConvertToImage());
	return result;
}


wxImage *SheetManager::GetSubImageDirect(int x, int y, int w, int h)
{
	wxImage *sub = new wxImage;
	
	*sub = fSpriteSheet->GetSubImage(wxRect(x, y, w, h));
	if (!sub->GetData())
	{
		staterr("SheetManager: GetSubImage() failed");
		delete sub;
		return NULL;
	}
	
	return sub;
}

/*
void c------------------------------() {}
*/

wxImage *SheetManager::GetSprite(int s, int f, int d)
{
	SpriteRecord *sprite = spritelist.SpriteAt(s);
	if (!sprite) return NULL;
	
	if (LoadSpritesheet(sprite->spritesheet))
		return NULL;
	
	return GetSubImage(sprite->frame[f].dir[d].sheet_offset.x, \
						sprite->frame[f].dir[d].sheet_offset.y, \
						sprite->w, sprite->h);
}

int SheetManager::SpriteSheetWidthFor(int sheetno)
{
	if (LoadSpritesheet(sheetno))
		return -1;
	
	return fSpriteSheet->GetWidth();
}

int SheetManager::SpriteSheetHeightFor(int sheetno)
{
	if (LoadSpritesheet(sheetno))
		return -1;
	
	return fSpriteSheet->GetHeight();
}





