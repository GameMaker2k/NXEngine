
#ifndef _EDITAREA_H
#define _EDITAREA_H

#include "../siflib/sif.h"

enum OverlayTypes
{
	OVERLAY_POINT,
	OVERLAY_RECT
};

struct Overlay
{
	int type;
	int x1, y1, x2, y2;
};

enum EditModes
{
	EM_VIEW,
	EM_BBOX,
	EM_BLOCKL,
	EM_BLOCKR,
	EM_BLOCKU,
	EM_BLOCKD,
	EM_ACTIONPT,
	EM_ACTIONPT2,
	EM_SPAWNPT,
	EM_DRAWPT,
	EM_SOLIDBOX,
	EM_PFBOX,
	
	EM_NUM_MODES
};

// for fDrag.mode; holds whether we are dragging the sides of the existing bbox
// or creating an entirely new one. If we are dragging the sides then multiple values
// can be ORed to indicate that we are dragging one of the diagonal corners.
enum DragModeFlags
{
	DM_CREATE_NEW		= 0x00,
	
	DM_ADJUST_X1		= 0x01,
	DM_ADJUST_X2		= 0x02,
	DM_ADJUST_Y1		= 0x04,
	DM_ADJUST_Y2		= 0x08,
	
	DM_ADJUST_SHEET_OFFSET	=	0x10,
	DM_ADJUST_ALL_OFFSETS	=	0x20,
	DM_ADJUST_SIZE			=	0x40
};

enum ShiftModes
{
	SM_NO_SHIFT,
	SM_SHEET_OFFSET,
	SM_OFFSET_ALL,
	SM_SIZE
};


struct ZoomedSpriteSettings
{
	bool equ(ZoomedSpriteSettings other);
	
	int zoom;
	int sheetno;
	int sheet_x, sheet_y;
	int w, h;
};

class EditArea : public wxPanel
{
public:
	EditArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
	virtual ~EditArea();
	
	bool SetBackground(const char *filename);
	
	void SetZoom(int factor);
	int ZoomFactor() { return fZoom; }
	void NextZoomStop(int dir);
	void NextOffsStop();
	
	void SetOffset(int offs) { fOffset = offs; Refresh(); }
	int Offset() { return fOffset; }
	
	void UpdateCtrlShiftStates();
	
private:
	SIFRect *GetCurrentBBox();
	SIFPoint *GetCurrentPointList(int *count_out, SIFPointList **outerstruct=NULL);
	
	void OnPaint(wxPaintEvent &event);
	void OnErase(wxEraseEvent &event);
	void OnTimer(wxTimerEvent &event);
	void RefreshNow();
	void Draw(wxDC *dc);
	
	void DrawRect(wxDC *dc, SIFRect *rect);
	void DrawPoints(wxDC *dc, SIFPoint *point, int count);
	void DrawGuideText(wxDC *dc, const char *str);
	
	void FillBackground(wxDC *dc);
	bool CreateZoomedSprite();
	
	void OnMouseDown(wxMouseEvent &event);
	void OnMouseUp(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	
	void HandleBeginDrag(wxMouseEvent &event, int button);
	void HandlePointEdit(int x, int y, int button);
	
	void DragUpdateNewBBox(int x, int y);
	void DragUpdateAdjustedBBox(int x, int y);
	int SelectDragMode(int px, int py);
	void OfferAppropriateDragCursor(int px, int py);
	void SetCursorForDragMode(int mode);
	
	void PixelToSpriteCoords(int x, int y, int *x_out, int *y_out);
	void SpriteToPixelCoords(int x, int y, int *x_out, int *y_out);
	
	ZoomedSpriteSettings GetZoomedSpriteSettings();
	
	void HandleShiftMode(int px, int py);
	void OffsetAllFrames(int dx, int dy);
	void UpdateShiftStatusText();
	
	
	DECLARE_EVENT_TABLE()
	
	wxImage *fBackground;
	wxImage *fZoomedSprite;
	ZoomedSpriteSettings fZoomedSpriteSettings;
	
	int fZoom;
	int fOffset;
	
	wxTimer *fTimer;
	bool fBlinkState;
	
	struct
	{
		bool dragging;
		int mode;
		int lastx, lasty;
		int anchor_x, anchor_y;
		
		SIFRect origrect;
		SIFRect draggedrect;
		
		bool mouse_moved;
		int orig_px, orig_py;
	} fDrag;
	
	struct
	{
		int mode;
		int lastpx, lastpy;
		int totaldx, totaldy;
		bool havefirst;
		bool buttondown;
		
		bool expand_bbox;
		bool expand_solidbox;
	} fShift;
};

#endif

