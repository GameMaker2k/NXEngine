
#ifndef _SHEETMGR_H
#define _SHEETMGR_H

#include "../siflib/sif.h"


class SheetManager
{
public:
	SheetManager();
	virtual ~SheetManager();
	
	wxImage *GetSprite(int sprite, int frame, int dir);
	int SpriteSheetWidthFor(int sheetno);
	int SpriteSheetHeightFor(int sheetno);
	
private:
	bool LoadSpritesheet(int sheetno);
	wxImage *GetSubImage(int x, int y, int w, int h);
	wxImage *GetSubImageDirect(int x, int y, int w, int h);
	
	wxImage *fSpriteSheet;
	int fSheetNo;
};

#endif

