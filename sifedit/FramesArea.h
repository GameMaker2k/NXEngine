
#ifndef _FRAMESAREA_H
#define _FRAMESAREA_H

#include "../siflib/sif.h"


class FramesArea : public wxPanel
{
public:
	FramesArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
	virtual ~FramesArea();
	
private:
	void OnPaint(wxPaintEvent &event);
	void OnLeftDown(wxMouseEvent &event);
	DECLARE_EVENT_TABLE()

	int fNumFramesPerLine;
	int fScaleFactor;
};

#endif

