
#ifndef _PREVIEWAREA_H
#define _PREVIEWAREA_H


class PreviewArea : public wxPanel
{
public:
	PreviewArea(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
	virtual ~PreviewArea();
	
private:
	void OnPaint(wxPaintEvent &event);
	bool LoadSpritesheet(int sheetno);

	void OnClick(wxMouseEvent &event);
	DECLARE_EVENT_TABLE()
	
	wxBitmap *fSpritesheetBitmap;
	int fSheetNo;
};


#endif
