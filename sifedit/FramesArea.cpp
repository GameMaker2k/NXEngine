
#include <wx/wx.h>
#include "sifedit.h"
#include "MainWindow.h"
#include "FramesArea.h"
#include "FramesArea.fdh"

BEGIN_EVENT_TABLE(FramesArea, wxPanel)
	EVT_PAINT(FramesArea::OnPaint)
	EVT_LEFT_DOWN(FramesArea::OnLeftDown)
END_EVENT_TABLE()

/*
void c------------------------------() {}
*/

FramesArea::FramesArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
	: wxPanel(parent, id, pos, size)
{
	fNumFramesPerLine = 0;
	fScaleFactor = 1;
}

FramesArea::~FramesArea()
{
}

/*
void c------------------------------() {}
*/

void FramesArea::OnPaint(wxPaintEvent &event)
{
	// create a backbuffer
	wxBitmap bbbmp(GetSize().GetWidth(), GetSize().GetHeight());
	wxMemoryDC bbdc(bbbmp);
	
	// clear everything
	bbdc.SetBrush(*wxMEDIUM_GREY_BRUSH);
	bbdc.SetPen(*wxTRANSPARENT_PEN);
	bbdc.DrawRectangle(0, 0, bbbmp.GetWidth(), bbbmp.GetHeight());
	
	int nframes = CurFrameCount();
	int mywidth = GetSize().GetWidth();
	int x = 0;
	int y = 0;
	
	fNumFramesPerLine = 0;
	fScaleFactor = (mainwin->fEditArea->ZoomFactor() >= 16) ? 2 : 1;
	
	int scaledwidth = CurSprite()->w * fScaleFactor;
	int scaledheight = CurSprite()->h * fScaleFactor;
	
	for(int f=0;f<nframes;f++)
	{
		wxImage *img = sheetmgr.GetSprite(gd.cursprite, f, gd.curdir);
		
		if (img)
		{
			img->Rescale(scaledwidth, scaledheight);
			bbdc.DrawBitmap(wxBitmap(*img), x, y);
		}
		
		if (f == gd.curframe)
		{
			bbdc.SetBrush(*wxTRANSPARENT_BRUSH);
			bbdc.SetPen(*wxRED_PEN);
			
			bbdc.DrawRectangle(x, y, scaledwidth, scaledheight);
			bbdc.DrawRectangle(x+1, y+1, scaledwidth - 2, scaledheight - 2);
		}
		
		x += (scaledwidth + 2);
		if (x + scaledwidth > mywidth - 2)
		{
			x = 0;
			y += (scaledheight + 2);
			if (!fNumFramesPerLine) fNumFramesPerLine = f+1;
		}
	}
	
	// blit contents of the backbuffer to the screen
	wxPaintDC dc(this);
	dc.Blit(0, 0, bbdc.GetSize().GetWidth(), bbdc.GetSize().GetHeight(), &bbdc, 0, 0);
}


void FramesArea::OnLeftDown(wxMouseEvent &event)
{
int x, y, f;

	x = event.m_x / ((CurSprite()->w * fScaleFactor) + 2);
	y = event.m_y / ((CurSprite()->h * fScaleFactor) + 2);
	
	f = (y * fNumFramesPerLine) + x;
	if (f >= 0 && f < CurSprite()->nframes)
	{
		gd.curframe = f;
		mainwin->UpdateDisplay();
	}
}





