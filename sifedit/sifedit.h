
#ifndef _SIFEDIT_H
#define _SIFEDIT_H

#include <wx/wx.h>
#include <sys/param.h>
#include "../common/BList.h"
#include "../common/StringList.h"
#include "../common/DBuffer.h"
#include "../common/DString.h"

#include "EditArea.h"
#include "SheetManager.h"
#include "SpriteRecord.h"

#include "../siflib/sifloader.h"
#include "../siflib/sif.h"
#include "../siflib/sectSprites.h"
#include "../siflib/sectStringArray.h"

#define MAX_SPRITES			512

enum Directions
{
	RIGHT,
	LEFT,
	UP,
	DOWN
};

class MyApp : public wxApp
{
	virtual bool OnInit();
	int FilterEvent(wxEvent &event);
};

struct GlobalData
{
	int cursprite;
	int curframe;
	int curdir;
	int curmode;
	bool ctrl, shift;
	
	struct
	{
		int sprite, frame, dir;
	} clipboard;
	
	StringList sheetfiles;		// spritesheet names
	StringList groupnames;		// names of sprite groups
	
	char openfilename[MAXPATHLEN];
	bool nojerk;	// hack to prevent jerking spritetree selection back to current sprite when not desirable
};

extern GlobalData gd;
extern SpriteList spritelist;
extern const char *pathPrefix;
extern SheetManager sheetmgr;

const char *GetSpriteHeaderName(const char *sprname);

SpriteRecord *CurSprite();
SIFFrame *CurFrame();
SIFDir *CurDir();
int CurFrameCount();
int CurDirCount();
int CountSpritesheetUsage(int sheetno, int *firstuse_out = NULL);

#endif

