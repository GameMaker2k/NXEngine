
#include <wx/wx.h>
#include "sifedit.h"
#include "MainWindow.h"
#include "EditArea.fdh"

#define SWAP(A, B)	{ A ^= B; B ^= A; A ^= B; }
static int zoomstops[] = { 2, 4, 8, 16, 32, -1 };

BEGIN_EVENT_TABLE(EditArea, wxPanel)

	EVT_PAINT(EditArea::OnPaint)
	EVT_ERASE_BACKGROUND(EditArea::OnErase)
    EVT_TIMER(100, EditArea::OnTimer)

	EVT_LEFT_DOWN(EditArea::OnMouseDown)
	EVT_LEFT_UP(EditArea::OnMouseUp)
	EVT_RIGHT_DOWN(EditArea::OnMouseDown)
	EVT_RIGHT_UP(EditArea::OnMouseUp)
	EVT_MOTION(EditArea::OnMouseMove)
	EVT_MOUSEWHEEL(EditArea::OnMouseWheel)
	
END_EVENT_TABLE()

/*
void c------------------------------() {}
*/

EditArea::EditArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
	: wxPanel(parent, id, pos, size)
{
	fTimer = new wxTimer(this, 100);
	fTimer->Start(125);
	
	fBackground = NULL;
	fZoomedSprite = NULL;
	
	fZoom = 16;
	fOffset = 1;
	fBlinkState = true;
	
	fDrag.dragging = false;
	fShift.mode = SM_NO_SHIFT;
}

EditArea::~EditArea()
{
	delete fBackground;
	delete fZoomedSprite;
}

/*
void c------------------------------() {}
*/

bool EditArea::SetBackground(const char *filename)
{
	delete fBackground;
	fBackground = new wxImage;
	
	if (!fBackground->LoadFile(wxString(PrefixPath(filename), wxConvUTF8)))
	{
		stat("SetBackground: failed to load file '%s'", filename);
		return 1;
	}
	
	return 0;
}

void EditArea::SetZoom(int factor)
{
	if (factor < 1) factor = 1;
	if (factor > 32) factor = 32	;
	
	if (fZoom != factor)
	{
		fZoom = factor;
		CreateZoomedSprite();
	}
}

void EditArea::NextZoomStop(int dir)
{
	int curstop;
	for(curstop=0;zoomstops[curstop] != -1;curstop++)
		if (zoomstops[curstop] == fZoom) break;
	
	curstop += (dir > 0) ? 1 : -1;
	
	if (zoomstops[curstop] == -1) curstop--;
	if (curstop < 0) curstop = 0;
	
	fZoom = zoomstops[curstop];
}

void EditArea::NextOffsStop()
{
	fOffset *= 2;
	if (fOffset > 16)
		fOffset = 1;
	
	RefreshNow();
}

/*
void c------------------------------() {}
*/

void EditArea::OnTimer(wxTimerEvent &event)
{
	if (gd.curmode != EM_VIEW)
	{
		fBlinkState ^= 1;
		RefreshNow();
	}
	else fBlinkState = 1;
}

void EditArea::OnPaint(wxPaintEvent &event)
{
wxPaintDC dc(this);
	Draw(&dc);
}

void EditArea::Draw(wxDC *dc)
{
	if (!fZoomedSprite || !fZoomedSpriteSettings.equ(GetZoomedSpriteSettings()))
	{
		if (CreateZoomedSprite())
			return;
	}
	
	// create a backbuffer
	wxBitmap bbbmp(GetSize().GetWidth(), GetSize().GetHeight());
	wxMemoryDC bbdc(bbbmp);
	
	// clear everything
	bbdc.SetBrush(*wxLIGHT_GREY_BRUSH);
	bbdc.SetPen(*wxTRANSPARENT_PEN);
	bbdc.DrawRectangle(0, 0, bbbmp.GetWidth(), bbbmp.GetHeight());
	
	// draw sprite onto backbuffer
	bbdc.DrawBitmap(wxBitmap(*fZoomedSprite), fOffset*fZoom, fOffset*fZoom);
	
	// draw overlays
	bbdc.SetPen(fBlinkState ? *wxRED_PEN : *wxBLACK_PEN);
	bbdc.SetBrush(*wxTRANSPARENT_BRUSH);
	
	if (fShift.mode != SM_NO_SHIFT)
	{
	}
	else if (fDrag.dragging)
	{
		DrawRect(&bbdc, &fDrag.draggedrect);
	}
	else if (gd.curmode == EM_VIEW)
	{
		SIFSprite *sprite = CurSprite();
		SIFDir *dir = CurDir();
		
		bbdc.SetPen(*wxCYAN_PEN);
		DrawRect(&bbdc, &sprite->solidbox);
		
		bbdc.SetPen(*wxRED_PEN);
		DrawRect(&bbdc, &sprite->bbox);
		DrawPoints(&bbdc, &sprite->block_l.point[0], sprite->block_l.count);
		DrawPoints(&bbdc, &sprite->block_r.point[0], sprite->block_r.count);
		DrawPoints(&bbdc, &sprite->block_u.point[0], sprite->block_u.count);
		DrawPoints(&bbdc, &sprite->block_d.point[0], sprite->block_d.count);
		
		bbdc.SetPen(*wxGREEN_PEN);
		if (!dir->actionpoint.equ(0, 0)) DrawPoints(&bbdc, &dir->actionpoint, 1);
		if (!dir->actionpoint2.equ(0, 0)) DrawPoints(&bbdc, &dir->actionpoint2, 1);
		
		bbdc.SetPen(*wxLIGHT_GREY_PEN);
		if (!sprite->spawn_point.equ(0, 0)) DrawPoints(&bbdc, &sprite->spawn_point, 1);
		
		bbdc.SetPen(*wxCYAN_PEN);
		if (!dir->drawpoint.equ(0, 0)) DrawPoints(&bbdc, &dir->drawpoint, 1);
	}
	else
	{
		SIFRect *bbox = GetCurrentBBox();
		if (bbox) DrawRect(&bbdc, bbox);
		
		int npoints;
		SIFPoint *points = GetCurrentPointList(&npoints);
		if (points)
		{
			if (gd.curmode == EM_ACTIONPT || gd.curmode == EM_ACTIONPT2)
			{
				bbdc.SetPen(fBlinkState ? *wxGREEN_PEN : *wxBLACK_PEN);
			}
			
			DrawPoints(&bbdc, points, npoints);
		}
	}
	
	// blit contents of the backbuffer to the screen
	dc->Blit(0, 0, bbdc.GetSize().GetWidth(), bbdc.GetSize().GetHeight(), &bbdc, 0, 0);
	
	// draw coordinates when dragging
	if (fShift.mode)
	{
		switch(fShift.mode)
		{
			case SM_SIZE:
				DrawGuideText(dc, stprintf("%d x %d", \
					CurSprite()->w, CurSprite()->h));
			break;
			
			case SM_SHEET_OFFSET:
				DrawGuideText(dc, stprintf("[%d, %d]", \
					CurDir()->sheet_offset.x, CurDir()->sheet_offset.y));
			break;
			
			case SM_OFFSET_ALL:
				DrawGuideText(dc, stprintf("+[%d, %d]", \
					fShift.totaldx, fShift.totaldy));
			break;
		}
	}
}

// prevent horrific flicker under Windows--keep it from
// trying to erase the background every time we paint.
void EditArea::OnErase(wxEraseEvent &event)
{
}

void EditArea::DrawRect(wxDC *dc, SIFRect *rect)
{
	int x = (fOffset * fZoom) + (rect->x1 * fZoom);
	int y = (fOffset * fZoom) + (rect->y1 * fZoom);
	int w = ((rect->x2 - rect->x1) + 1) * fZoom;
	int h = ((rect->y2 - rect->y1) + 1) * fZoom;
	
	dc->DrawRectangle(x, y, w, h);
}

void EditArea::DrawPoints(wxDC *dc, SIFPoint *point, int count)
{
	for(int i=0;i<count;i++)
	{
		int x = (fOffset * fZoom) + (point[i].x * fZoom);
		int y = (fOffset * fZoom) + (point[i].y * fZoom);
		
		dc->DrawRectangle(x, y, fZoom, fZoom);
		dc->DrawLine(x, y, x+(fZoom-1), y+(fZoom-1));
	}
}

void EditArea::DrawGuideText(wxDC *dc, const char *str)
{
int x, y;
wxCoord tew, teh;

	dc->SetFont(wxFont(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	dc->SetTextForeground(wxColour(0, 0, 0));
	dc->SetTextBackground(wxColour(255, 255, 255));
	dc->SetBackgroundMode(wxSOLID);
	
	wxString wxstr(str, wxConvUTF8);
	dc->GetTextExtent(wxstr, &tew, &teh);
	
	x = (fOffset * fZoom) + ((CurSprite()->w * fZoom) / 2) - (tew / 2);
	y = (fOffset * fZoom) + (CurSprite()->h * fZoom) + 8;
	
	dc->DrawText(wxstr, x, y);
}

/*
void c------------------------------() {}
*/

// converts from e.g. mouse coords on the window to sprite coords on the (zoomed) sprite
void EditArea::PixelToSpriteCoords(int x, int y, int *x_inout, int *y_inout)
{
	x -= (fOffset * fZoom);
	y -= (fOffset * fZoom);
	if (x < 0) x -= fZoom;
	if (y < 0) y -= fZoom;
	x /= fZoom;
	y /= fZoom;
	
	if (x_inout) *x_inout = x;
	if (y_inout) *y_inout = y;
}

// converts from sprite coords to actual window coordinates relative to the editarea
// returns the upper-left corner of the enlarged pixel.
void EditArea::SpriteToPixelCoords(int x, int y, int *x_inout, int *y_inout)
{
	x *= fZoom;
	y *= fZoom;
	x += (fOffset * fZoom);
	y += (fOffset * fZoom);
	
	if (x_inout) *x_inout = x;
	if (y_inout) *y_inout = y;
}

/*
void c------------------------------() {}
*/

// returns a pointer the bbox which is currently being edited, based on the edit mode, or NULL.
SIFRect *EditArea::GetCurrentBBox()
{
	switch(gd.curmode)
	{
		case EM_BBOX:	  return &CurSprite()->bbox;
		case EM_SOLIDBOX: return &CurSprite()->solidbox;
		case EM_PFBOX:	  return &CurDir()->pf_bbox;
	}
	
	return NULL;
}

// returns a pointer to the first point in the pointlist (or single point in some modes)
// which is currently being edited, and the number of points, based on the edit mode, or NULL.
SIFPoint *EditArea::GetCurrentPointList(int *count_out, SIFPointList **outerstruct)
{
	int dummy1;
	SIFPointList *dummy2;
	if (!count_out) count_out = &dummy1;
	if (!outerstruct) outerstruct = &dummy2;
	
	*outerstruct = NULL;
	
	switch(gd.curmode)
	{
		case EM_BLOCKL:
			*outerstruct = &CurSprite()->block_l;
			break;
		
		case EM_BLOCKR:
			*outerstruct = &CurSprite()->block_r;
			break;
		
		case EM_BLOCKU:
			*outerstruct = &CurSprite()->block_u;
			break;
		
		case EM_BLOCKD:
			*outerstruct = &CurSprite()->block_d;
			break;
		
		case EM_SPAWNPT: *count_out = 1; return &CurSprite()->spawn_point;
		
		case EM_ACTIONPT: *count_out = 1; return &CurDir()->actionpoint;
		case EM_ACTIONPT2: *count_out = 1; return &CurDir()->actionpoint2;
		case EM_DRAWPT: *count_out = 1; return &CurDir()->drawpoint;
	}
	
	if (outerstruct && *outerstruct)
	{
		*count_out = (*outerstruct)->count;
		return &((*outerstruct)->point[0]);
	}
	
	return NULL;
}

/*
void c------------------------------() {}
*/

void EditArea::OnMouseDown(wxMouseEvent &event)
{
	fShift.buttondown = event.m_leftDown;
	if (fShift.mode) return;
	
	int x, y;
	PixelToSpriteCoords(event.m_x, event.m_y, &x, &y);
	
	int button = event.m_leftDown ? LEFT : RIGHT;
	
	// begin drag if allowed
	SIFRect *bbox = GetCurrentBBox();
	if (bbox)
	{
		HandleBeginDrag(event, button);
	}
	else	// else maybe they're trying to add/remove a point
	{
		HandlePointEdit(x, y, button);
	}
}


void EditArea::OnMouseMove(wxMouseEvent &event)
{
	if (fShift.mode)
	{
		HandleShiftMode(event.m_x, event.m_y);
		return;
	}
	
	if (!fDrag.dragging)
	{
		OfferAppropriateDragCursor(event.m_x, event.m_y);
		return;
	}
	
	SetCursorForDragMode(fDrag.mode);
	if (!fDrag.mouse_moved)
	{
		if (event.m_x != fDrag.orig_px || event.m_y != fDrag.orig_py)
		{
			fDrag.mouse_moved = true;
		}
	}
	
	int x, y;
	PixelToSpriteCoords(event.m_x, event.m_y, &x, &y);
	
	if (x == fDrag.lastx && y == fDrag.lasty)
		return;
	
	// update the displayed bbox as they drag
	if (fDrag.mode == DM_CREATE_NEW)
		DragUpdateNewBBox(x, y);
	else
		DragUpdateAdjustedBBox(x, y);
	
	// update screen
	RefreshNow();
	
	fDrag.lastx = x;
	fDrag.lasty = y;
}


void EditArea::OnMouseUp(wxMouseEvent &event)
{
	fShift.buttondown = event.m_leftDown;
	if (fShift.mode) return;
	
	if (fDrag.dragging)
	{
		// apply the new bbox
		if (fDrag.mouse_moved)
		{
			SIFRect *bbox = GetCurrentBBox();
			if (bbox)
			{
				*bbox = fDrag.draggedrect;
				
				if (mainwin->fLinkBBoxCheckbox->GetValue() && \
					gd.curmode == EM_BBOX)
				{
					CurSprite()->solidbox = fDrag.draggedrect;
				}
			}
		}
		
		fDrag.dragging = false;
		SetCursor(*wxSTANDARD_CURSOR);
	}
}

void EditArea::OnMouseWheel(wxMouseEvent &event)
{
	int amt = event.m_wheelRotation / event.m_wheelDelta;
	if (amt == 0) return;
	
	NextZoomStop(amt);
	mainwin->UpdateDisplay();
}

/*
void c------------------------------() {}
*/

void EditArea::DragUpdateNewBBox(int x, int y)
{
	SIFRect rect;
	rect.x1 = fDrag.anchor_x;
	rect.y1 = fDrag.anchor_y;
	rect.x2 = x;
	rect.y2 = y;
	
	if (rect.x1 > rect.x2) SWAP(rect.x1, rect.x2);
	if (rect.y1 > rect.y2) SWAP(rect.y1, rect.y2);
	
	fDrag.draggedrect = rect;
}

void EditArea::DragUpdateAdjustedBBox(int x, int y)
{
	// get distance from anchor
	int distx = (x - fDrag.anchor_x);
	int disty = (y - fDrag.anchor_y);
	
	SIFRect rect = fDrag.origrect;
	
	if (fDrag.mode & DM_ADJUST_X1) rect.x1 += distx;
	if (fDrag.mode & DM_ADJUST_X2) rect.x2 += distx;
	if (fDrag.mode & DM_ADJUST_Y1) rect.y1 += disty;
	if (fDrag.mode & DM_ADJUST_Y2) rect.y2 += disty;
	
	fDrag.draggedrect = rect;
}


// returns which drag mode would be used if you pressed the left button
// right now at (unzoomed) pixel position x, y.
int EditArea::SelectDragMode(int px, int py)
{
	SIFRect *bbox;
	int x1, y1, x2, y2;
	
	if (!(bbox = GetCurrentBBox()))
		return DM_CREATE_NEW;	// not in bbox-edit mode, so always use standard cursor
	
	x1 = bbox->x1;
	y1 = bbox->y1;
	x2 = bbox->x2;
	y2 = bbox->y2;
	
	// zoom the bbox's coords to get it's real, zoomed, coords on the screen
	SpriteToPixelCoords(x1, y1, &x1, &y1);
	SpriteToPixelCoords(x2, y2, &x2, &y2);
	x2 += (fZoom - 1);	// get lower-left corner
	y2 += (fZoom - 1);	// instead of upper-left
	
	// find out whether we are closest to the left, right, top or bottom edge of the bbox.
	int xclosest = 9000;
	int yclosest = 9000;
	int xwinner = 0;
	int ywinner = 0;
	int nc;
	
	if (py >= y1 && py <= y2)
	{
		if ((nc = abs(px - x1)) < xclosest) { xclosest = nc; xwinner = DM_ADJUST_X1; }
		if ((nc = abs(px - x2)) < xclosest) { xclosest = nc; xwinner = DM_ADJUST_X2; }
	}
	
	if (px >= x1 && px <= x2)
	{
		if ((nc = abs(py - y1)) < yclosest) { yclosest = nc; ywinner = DM_ADJUST_Y1; }
		if ((nc = abs(py - y2)) < yclosest) { yclosest = nc; ywinner = DM_ADJUST_Y2; }
	}
	
	// check if we're close enough to count
	#define CLOSE_DIST		5
	
	int result = 0;			// equivalent to DM_CREATE_NEW
	if (xclosest <= CLOSE_DIST)	result |= xwinner;
	if (yclosest <= CLOSE_DIST) result |= ywinner;
	
	// offset whole bbox
	if (result == 0 && px >= x1 && px <= x2 && py >= y1 && py <= y2)
		result = (DM_ADJUST_X1 | DM_ADJUST_X2 | DM_ADJUST_Y1 | DM_ADJUST_Y2);
	
	return result;
}

// called from OnMouseMove, if they are close to one of the sides of
// the bbox it sets the mouse cursor to a sizing cursor to offer to drag that side.
void EditArea::OfferAppropriateDragCursor(int px, int py)
{
	int dm = DM_CREATE_NEW;
	
	if (!fDrag.dragging)
		dm = SelectDragMode(px, py);
	
	SetCursorForDragMode(dm);
}

// sets the cursor to that used when doing or offering the given drag mode
void EditArea::SetCursorForDragMode(int mode)
{
	switch(mode)
	{
		case DM_ADJUST_X1:
		case DM_ADJUST_X2: SetCursor(wxCursor(wxCURSOR_SIZEWE)); return;
		
		case DM_ADJUST_Y1:
		case DM_ADJUST_Y2: SetCursor(wxCursor(wxCURSOR_SIZENS)); return;
		
		case DM_ADJUST_X1 | DM_ADJUST_Y1:
		case DM_ADJUST_X2 | DM_ADJUST_Y2:
			SetCursor(wxCursor(wxCURSOR_SIZENWSE));
		return;
		
		case DM_ADJUST_X2 | DM_ADJUST_Y1:
		case DM_ADJUST_X1 | DM_ADJUST_Y2:
			SetCursor(wxCursor(wxCURSOR_SIZENESW));
		return;
		
		case DM_ADJUST_X1 | DM_ADJUST_X2 | DM_ADJUST_Y1 | DM_ADJUST_Y2:
		{
			// don't OFFER it, only set it if we're doing it now via right mouse button
			if (fDrag.dragging)
			{
				SetCursor(wxCursor(wxCURSOR_SIZING));
				return;
			}
		}
		break;
	}
	
	SetCursor(*wxSTANDARD_CURSOR);
}

/*
void c------------------------------() {}
*/

void EditArea::HandleBeginDrag(wxMouseEvent &event, int button)
{
int x, y;

	SIFRect *bbox = GetCurrentBBox();
	if (!bbox) return;
	
	PixelToSpriteCoords(event.m_x, event.m_y, &x, &y);
	
	fDrag.mode = SelectDragMode(event.m_x, event.m_y);
	if (fDrag.mode == (DM_ADJUST_X1 | DM_ADJUST_Y1 | DM_ADJUST_X2 | DM_ADJUST_Y2))
	{
		if (button != RIGHT)
			fDrag.mode = DM_CREATE_NEW;
	}
	
	fDrag.origrect = *bbox;
	fDrag.anchor_x = x;
	fDrag.anchor_y = y;
	fDrag.dragging = true;
	
	fDrag.orig_px = event.m_x;
	fDrag.orig_py = event.m_y;
	fDrag.mouse_moved = false;
	
	fDrag.lastx = -1;
	fDrag.lasty = -1;
	
	OnMouseMove(event);
}

void EditArea::HandlePointEdit(int x, int y, int button)
{
SIFPointList *list;
SIFPoint *point;
int npoints;

	stat("HandlePointEdit mouse click at %d,%d  btn %d", x, y, button);
	
	point = GetCurrentPointList(&npoints, &list);
	if (!point) return;
	
	if (list)
	{	// dealing with a list, such as blockl/r/u/d
		
		// delete any point currently at this position
		// for RIGHT this is the action requested
		// for LEFT it prevents adding more than one block point in the same spot
		SIFPointList newlist;
		newlist.count = 0;
		for(int i=0;i<npoints;i++)
		{
			if (list->point[i].x != x || list->point[i].y != y)
			{
				newlist.point[newlist.count].x = list->point[i].x;
				newlist.point[newlist.count].y = list->point[i].y;
				newlist.count++;
			}
		}
		*list = newlist;
		
		if (button == LEFT)
		{
			if (list->count < SIF_MAX_BLOCK_POINTS)
			{
				list->point[list->count].x = x;
				list->point[list->count].y = y;
				list->count++;
			}
			else blip();
		}
	}
	else
	{	// dealing with a single point, such as spawn_point
		if (button == RIGHT) return;
		point->x = x;
		point->y = y;
	}
	
	RefreshNow();
	mainwin->UpdateTitle();
}

/*
void c------------------------------() {}
*/

void EditArea::UpdateCtrlShiftStates()
{
	int oldShiftMode = fShift.mode;
	fShift.mode = SM_NO_SHIFT;
	
	if (gd.ctrl)
	{
		fShift.mode = (!gd.shift) ? SM_SHEET_OFFSET : SM_OFFSET_ALL;
	}
	else if (gd.shift)
	{
		fShift.mode = SM_SIZE;
	}
	
	UpdateShiftStatusText();
	
	if (fShift.mode != oldShiftMode)
	{
		if (fShift.mode)
		{
			fDrag.dragging = false;
			fShift.havefirst = false;
			fShift.buttondown = false;
			fShift.totaldx = fShift.totaldy = 0;
			
			fShift.expand_bbox = CurSprite()->bbox.equ(0, 0, CurSprite()->w-1, CurSprite()->h-1);
			fShift.expand_solidbox = CurSprite()->solidbox.equ(0, 0, CurSprite()->w-1, CurSprite()->h-1);
			
			SetCursor(wxCursor(wxCURSOR_SIZING));
		}
		else
		{
			SetCursor(*wxSTANDARD_CURSOR);
		}
		
		RefreshNow();
	}
}

void EditArea::HandleShiftMode(int px, int py)
{
	SetCursor(wxCursor(wxCURSOR_SIZING));
	
	if (!fShift.buttondown)
	{
		fShift.havefirst = false;
		return;
	}
	
	if (!fShift.havefirst)
	{
		fShift.lastpx = px;
		fShift.lastpy = py;
		fShift.totaldx = fShift.totaldy = 0;
		fShift.havefirst = true;
		stat("HandleShiftMode(%d, %d): Took first.", px, py);
		return;
	}
	
	const int SENSITIVITY = 2;
	int deltax = (px - fShift.lastpx) / SENSITIVITY;
	int deltay = (py - fShift.lastpy) / SENSITIVITY;
	
	SpriteRecord *sprite = CurSprite();
	SIFDir *dir = CurDir();
	
	if (deltax)
	{
		switch(fShift.mode)
		{
			case SM_SIZE:
			{
				sprite->w += deltax;
				if (sprite->w < 1) sprite->w = 1;
				if (sprite->w > 255) sprite->w = 255;
			}
			break;
			
			case SM_SHEET_OFFSET:
			{
				dir->sheet_offset.x += deltax;
				if (dir->sheet_offset.x < 0) dir->sheet_offset.x = 0;
			}
			break;
			
			case SM_OFFSET_ALL:
			{
				OffsetAllFrames(deltax, 0);
			}
			break;
		}
		
		fShift.lastpx = px;
	}
	
	if (deltay)
	{
		switch(fShift.mode)
		{
			case SM_SIZE:
			{
				sprite->h += deltay;
				if (sprite->h < 1) sprite->h = 1;
				if (sprite->h > 255) sprite->h = 255;
			}
			break;
			
			case SM_SHEET_OFFSET:
			{
				dir->sheet_offset.y += deltay;
				if (dir->sheet_offset.y < 0) dir->sheet_offset.y = 0;
			}
			break;
			
			case SM_OFFSET_ALL:
			{
				OffsetAllFrames(0, deltay);
			}
			break;
		}
		
		fShift.lastpy = py;
	}
	
	if (deltax || deltay)
	{
		if (fShift.mode == SM_SIZE)
		{
			if (fShift.expand_bbox)
				CurSprite()->bbox.set(0, 0, CurSprite()->w-1, CurSprite()->h-1);
				
			if (fShift.expand_solidbox)
				CurSprite()->solidbox.set(0, 0, CurSprite()->w-1, CurSprite()->h-1);
		}
		
		fShift.totaldx += deltax;
		fShift.totaldy += deltay;
		
		mainwin->UpdateDisplay();
		UpdateShiftStatusText();
	}
}

void EditArea::UpdateShiftStatusText()
{
char *status = "Press CTRL=Sheet Offset, Shift=Size, CTRL+Shift=Offset All";

	switch(fShift.mode)
	{
		case SM_SHEET_OFFSET:
			status = stprintf("Adjusting sheet offset: [%d, %d]",
						CurDir()->sheet_offset.x, CurDir()->sheet_offset.y);
		break;
		
		case SM_OFFSET_ALL:
			status = "Adjusting sheet offset of ALL frames in current sprite...drag!";
		break;
		
		case SM_SIZE:
			status = stprintf("Sizing current sprite: %dx%d",
						CurSprite()->w, CurSprite()->h);
		break;
	}
	
	mainwin->SetStatusText(wxString(status, wxConvUTF8));
}


void EditArea::OffsetAllFrames(int dx, int dy)
{
	SpriteRecord *sprite = CurSprite();
	int nframes = CurFrameCount();
	
	for(int i=0;i<nframes;i++)
	{
		sprite->frame[i].dir[gd.curdir].sheet_offset.x += dx;
		sprite->frame[i].dir[gd.curdir].sheet_offset.y += dy;
	}
}

/*
void c------------------------------() {}
*/

bool EditArea::CreateZoomedSprite()
{
	delete fZoomedSprite;
	fZoomedSprite = NULL;
	
	// cut out a wximage from the spritesheet containing just the sprite we are interested in
	wxImage *sprite = sheetmgr.GetSprite(gd.cursprite, gd.curframe, gd.curdir);
	if (!sprite)
	{
		staterr("CreateZoomedSprite: failed to GetSprite()");
		return 1;
	}
	
	// add alpha to the sprite by setting all black pixels to transparent
	int alphasize = (sprite->GetWidth() * sprite->GetHeight());
	uint8_t *alpha = (uint8_t *)malloc(alphasize);
	uint8_t *data = sprite->GetData();
	int i, j;
	for(i=j=0;i<alphasize;i++)
	{
		if (data[j] == 0 && data[j+1] == 0 && data[j+2] == 0)
		{
			alpha[i] = 0;
		}
		else
		{
			alpha[i] = 255;
		}
		
		j += 3;
	}
	
	sprite->SetAlpha(alpha);
	
	// zoom the sprite to the desired size
	int zoomw = CurSprite()->w * fZoom;
	int zoomh = CurSprite()->h * fZoom;
	sprite->Rescale(zoomw, zoomh);
	
	// create a temporary offscreen memory DC of the appropriate size
	wxBitmap membmp(zoomw, zoomh);
	wxMemoryDC memdc(membmp);
	
	// blit the background and sprite into the memory DC
	FillBackground(&memdc);
	memdc.DrawBitmap(wxBitmap(*sprite), 0, 0);
	
	// obtain the image from the bitmap associated with the memory DC
	fZoomedSprite = new wxImage(membmp.ConvertToImage());
	
	fZoomedSpriteSettings = GetZoomedSpriteSettings();
	delete sprite;
	return 0;
}

ZoomedSpriteSettings EditArea::GetZoomedSpriteSettings()
{
ZoomedSpriteSettings set;
	
	set.zoom = fZoom;
	set.sheet_x = CurDir()->sheet_offset.x;
	set.sheet_y = CurDir()->sheet_offset.y;
	set.sheetno = CurSprite()->spritesheet;
	set.w = CurSprite()->w;
	set.h = CurSprite()->h;
	
	return set;
}

bool ZoomedSpriteSettings::equ(ZoomedSpriteSettings other)
{
	if (sheet_x != other.sheet_x) return false;
	if (sheet_y != other.sheet_y) return false;
	if (w != other.w) return false;
	if (h != other.h) return false;
	if (sheetno != other.sheetno) return false;
	if (zoom != other.zoom) return false;
	
	return true;
}

/*
void c------------------------------() {}
*/

void EditArea::FillBackground(wxDC *dc)
{
	if (!fBackground) return;
	wxBitmap bg(*fBackground);
	
	int w = dc->GetSize().GetWidth();
	int h = dc->GetSize().GetHeight();
	
	for(int y=0;y<h;y+=bg.GetWidth())
	for(int x=0;x<w;x+=bg.GetHeight())
	{
		dc->DrawBitmap(bg, x, y);
	}
}

void EditArea::RefreshNow()
{
	#ifdef WIN32
		wxClientDC dc(this);
		Draw(&dc);
	#else
		Refresh();
		//Update();
	#endif
}






