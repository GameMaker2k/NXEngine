
#include <wx/wx.h>
#include <wx/encconv.h>
#include "sifedit.h"
#include "MainWindow.h"
#include "sifedit.fdh"

#ifndef WIN32
#include <sys/ioctl.h>
#include <linux/kd.h>
#endif

IMPLEMENT_APP(MyApp)

SpriteList spritelist;
GlobalData gd;
SheetManager sheetmgr;
const char *pathPrefix = "../data/";
MainWindow *mainwin = NULL;
int initial_zoom = 16;

static const int SESSION_VERSION = 1;


bool MyApp::OnInit()
{
	gd.ctrl = false;
	gd.shift = false;
	
	strcpy(gd.openfilename, "../sprites.sif");
	
	if (load_sif(gd.openfilename))
	{
		wxMessageBox( wxT("Failed to load sif; please see command line for details.") );
		return false;
	}
	
	new MainWindow();
	mainwin->Show(true);
	SetTopWindow(mainwin);
	
	return true;
}

int MyApp::FilterEvent(wxEvent &event)
{
	int type = event.GetEventType();
	
	if (type == wxEVT_KEY_DOWN)
	{
		if (mainwin->OnKeyDown((wxKeyEvent &)event))
			return 1;
	}
	else if (type == wxEVT_KEY_UP)
	{
		if (mainwin->OnKeyUp((wxKeyEvent &)event))
			return 1;
	}
	
	return -1;
}

/*
void c------------------------------() {}
*/

bool load_sif(const char *fname)
{
SIFLoader sif;
uint8_t *data;
int length;

	if (sif.LoadHeader(fname))
		return 1;
	
	// load filenames of spritesheets and textual names for sprite groups
	LoadStringSection(&sif, SIF_SECTION_SHEETS, &gd.sheetfiles);
	
	// load sprites array
	data = sif.FindSection(SIF_SECTION_SPRITES, &length);
	if (!data)
	{
		staterr("load_sif: missing section SPRITES");
		return 1;
	}
	
	// get length of array we need
	int nsprites = SIFSpritesSect::GetSpriteCount(data, length);
	SIFSprite *spritearray = (SIFSprite *)malloc(nsprites * sizeof(SIFSprite));
	
	if (SIFSpritesSect::Decode(data, length, spritearray, NULL, MAX_SPRITES))
		return 1;
	
	// names & comments for sprites are stored seperately in the .sif
	StringList names, comments;
	LoadStringSection(&sif, SIF_SECTION_NAMES, &names);
	LoadStringSection(&sif, SIF_SECTION_COMMENTS, &comments);
	
	// assimilate all the loaded data together and copy it into our
	// list which holds a derived class of SIFSprite that also has additional
	// meta information not cared about by the game.
	for(int s=0;s<nsprites;s++)
	{
		SpriteRecord *spr = new SpriteRecord(&spritearray[s]);
		
		spr->name.SetTo(names.StringAt(s) ? names.StringAt(s) : "NameUnknown");
		spr->comment.SetTo(comments.StringAt(s) ? comments.StringAt(s) : "");
		
		spritelist.AddSprite(spr);
	}
	
	LoadGroupsSection(&sif);
	if (gd.groupnames.CountItems() == 0)
	{
		stat("load_sif: SIF directory section is empty or missing, adding initial '/' grouping");
		gd.groupnames.AddString("/");
	}
	
	LoadSessionSection(&sif);
	
	sif.CloseFile();
	return 0;
}


static bool LoadStringSection(SIFLoader *sif, int type, StringList *strings)
{
uint8_t *data;
int length;

	strings->MakeEmpty();
	
	data = sif->FindSection(type, &length);
	if (!data) return 1;
	
	if (SIFStringArraySect::Decode(data, length, strings))
	{
		staterr("LoadStringSection: string array decoding failed for section %d", type);
		return 1;
	}
	
	return 0;
}


// load the metadata section which holds sprite group names and which sprites
// are in which groups. It's an array of pascal strings holding the group names
// followed by a flat uint8 array of group assignments, each index corresponding
// to a sprite in the main sprite array.
static bool LoadGroupsSection(SIFLoader *sif)
{
const uint8_t *data, *data_end;
int length;

	gd.groupnames.MakeEmpty();
	
	data = sif->FindSection(SIF_SECTION_GROUPS, &length);
	if (!data) return 1;
	
	data_end = data + (length - 1);
	
	int ngroups = read_U8(&data, data_end);
	for(int i=0;i<ngroups;i++)
	{
		DString name;
		SIFStringArraySect::ReadPascalString(&data, data_end, &name);
		gd.groupnames.AddString(name.String());
	}
	
	for(int i=0;SpriteRecord *sprite=spritelist.SpriteAt(i);i++)
	{
		sprite->group = read_U8(&data, data_end);
	}
	
	return 0;
}


// load the SIFEdit last-session information from the sif
// return 0 if information was loaded.
static bool LoadSessionSection(SIFLoader *sif)
{
	// first set defaults for all values
	gd.cursprite = 0;
	gd.curframe = 0;
	gd.curdir = RIGHT;
	gd.curmode = EM_BBOX;
	initial_zoom = 16;
	
	// load section into memory
	int datalen;
	const uint8_t *data = sif->FindSection(SIF_SECTION_SESSION, &datalen);
	if (!data)
	{
		stat("No session record.");
		return 1;
	}
	
	const uint8_t *data_end = data + (datalen - 1);
	if (read_U8(&data, data_end) != SESSION_VERSION)
	{
		staterr("Session record exists, but is incorrect version; ignoring");
		return 1;
	}
	
	// decode raw data
	gd.cursprite = read_U16(&data, data_end);
	gd.curframe = read_U8(&data, data_end);
	gd.curdir = read_U8(&data, data_end);
	gd.curmode = read_U8(&data, data_end);
	initial_zoom = read_U8(&data, data_end);
	
	// sanity checking
	if (!IsValidSprite(gd.cursprite)) gd.cursprite = 0;
	if (!IsValidFrame(gd.cursprite, gd.curframe)) gd.curframe = 0;
	if (!IsValidDir(gd.cursprite, gd.curdir)) gd.curdir = 0;
	
	if (gd.curmode < 0 || gd.curmode >= EM_NUM_MODES)
		gd.curmode = EM_BBOX;
	
	return 0;
}

/*
void c------------------------------() {}
*/

bool save_sif(const char *fname)
{
SIFLoader sif;
StringList names, comments;
uint8_t *data;
int datalength;
int i;

	stat("save_sif('%s')", fname);
	
	// first rebuild our autogenerated source files if needed
	if (generate_sprites_h("../autogen/sprites.h"))
		return 1;
	
	if (generate_assignsprites_cpp("../autogen/AssignSprites.cpp"))
		return 1;
	
	
	// now begin saving the sif
	if (sif.BeginSave())
		return 1;
	
	// write names of spritesheets
	if (SaveStringSection(&sif, SIF_SECTION_SHEETS, &gd.sheetfiles)) return 1;
	
	// form the raw sprites array and send it to the encoder
	int nsprites = spritelist.CountItems();
	SIFSprite *spritearray = (SIFSprite *)malloc(sizeof(SIFSprite) * nsprites);
	
	for(i=0;i<nsprites;i++)
	{
		SpriteRecord *sprite = spritelist.SpriteAt(i);
		
		spritearray[i].Init();
		spritearray[i].CopyFrom(sprite);
		
		names.AddString(sprite->Name() ? sprite->Name() : "");
		comments.AddString(sprite->Comment() ? sprite->Comment() : "");
	}
	
	data = SIFSpritesSect::Encode(spritearray, nsprites, &datalength);
	sif.AddSection(SIF_SECTION_SPRITES, data, datalength);
	free(spritearray);
	
	// write the names and comments we just split out above
	if (SaveStringSection(&sif, SIF_SECTION_NAMES, &names)) return 1;
	if (SaveStringSection(&sif, SIF_SECTION_COMMENTS, &comments)) return 1;
	
	// save groups section, which needs a special function because it contains
	// both textual names and assignment to individual sprites
	if (SaveGroupsSection(&sif)) return 1;
	
	// finally, add our session information such as last sprite open
	if (SaveSessionSection(&sif)) return 1;
	
	return sif.EndSave(fname);
}


bool SaveStringSection(SIFLoader *sif, int type, StringList *strings)
{
uint8_t *data;
int datalength;

	data = SIFStringArraySect::Encode(strings, &datalength);
	if (!data) return 1;
	
	sif->AddSection(type, data, datalength);
	return 0;
}


bool SaveGroupsSection(SIFLoader *sif)
{
DBuffer data;
int i;

	int ngroups = gd.groupnames.CountItems();
	
	// first comes the array of group names
	data.Append8(ngroups);
	for(i=0;i<ngroups;i++)
	{
		SIFStringArraySect::WritePascalString(gd.groupnames.StringAt(i), &data);
	}
	
	// and then the array to assigns sprites into groups
	int nsprites = spritelist.CountItems();
	for(i=0;i<nsprites;i++)
	{
		data.Append8(spritelist.SpriteAt(i)->group);
	}
	
	int datalen = data.Length();
	return sif->AddSection(SIF_SECTION_GROUPS, data.TakeData(), datalen);
}


bool SaveSessionSection(SIFLoader *sif)
{
DBuffer data;

	data.Append8(SESSION_VERSION);
	
	data.Append16(gd.cursprite);
	data.Append8(gd.curframe);
	data.Append8(gd.curdir);
	data.Append8(gd.curmode);
	data.Append8(mainwin ? mainwin->fEditArea->ZoomFactor() : 16);
	
	int datalen = data.Length();
	return sif->AddSection(SIF_SECTION_SESSION, data.TakeData(), datalen);
}

/*
void c------------------------------() {}
*/

const char *PrefixPath(const char *path)
{
	DString str(pathPrefix);
	str.AppendString(path);
	return stprintf("%s", str.String());
}

const char *GetSpriteHeaderName(const char *sprname)
{
char *out = GetStaticStr();
int i, j;

	strcpy(out, "SPR_");
	j = 4;
	
	for(i=0;sprname[i];i++)
	{
		char ch = toupper(sprname[i]);
		if (ch == ' ') ch = '_';
		
		bool ok = false;
		if (ch >= 'A' && ch <= 'Z') ok = true;
		else if (ch == '_')ok = true;
		else if ((ch >= '0' && ch <= '9') && j != 0) ok = true;
		
		if (ok) out[j++] = ch;
	}
	
	out[j] = 0;
	return out;
}


char *wxStringToChar(wxString input)
{
#if (wxUSE_UNICODE)
	char *buffer = GetStaticStr();
	
	while(input.size() >= 255)
	{
		input = input.Left(input.Len() - 1);
	}
	
	wxEncodingConverter wxec;
	wxec.Init(wxFONTENCODING_ISO8859_1, wxFONTENCODING_ISO8859_1, wxCONVERT_SUBSTITUTE);
	wxec.Convert(input.mb_str(), buffer);
	return buffer;
#else
	return (char *)(input.c_str());
#endif
}

void sound(int freq, int time_ms)
{
#ifndef WIN32
const int kClockTickRate = 1193180;
	
	int console_fd = open("/dev/console", O_WRONLY);
	if (console_fd == -1)
	{
		staterr("-- FAILED TO OPEN /dev/console");
		return;
	}
	
	ioctl(console_fd, KIOCSOUND, kClockTickRate / freq);
	usleep(time_ms * 1000);
	ioctl(console_fd, KIOCSOUND, 0);
	
	close(console_fd);
#endif
}

void blip()
{
	sound(500, 50);
}

/*
void c------------------------------() {}
*/

int FindFirstSpriteInGroup(int groupno)
{
	for(int i=0;;i++)
	{
		SpriteRecord *rec = spritelist.SpriteAt(i);
		if (!rec) return -1;
		
		if (rec->group == groupno)
			return i;
	}
}

// reorder the sprite groups so that the groups whose indexes are in movelist
// appear just after the group "insertafter".
void ReorderGroups(BList *movelist, int insertafter)
{
	stat("ReorderGroups(insertafter=%d)", insertafter);
	
	// the algorithm will fail if insertafter is one of the listed groups
	// to be moved--that doesn't make any sense.
	if (movelist->HasItem((void *)insertafter))
	{
		staterr("ReorderGroups: insertafter is listed as one of the groups to be moved");
		return;
	}
	
	// add all groups to the new list, but skip those indexes which appear
	// in movelist. after adding i==insertafter, dump everything in movelist
	// in at that position as well.
	StringList newgroups;
	int old_to_new[gd.groupnames.CountItems()];
	int gi = 0;
	
	for(int i=0;i<gd.groupnames.CountItems();i++)
	{
		if (!movelist->HasItem((void *)i))
		{
			newgroups.AddString(gd.groupnames.StringAt(i));
			old_to_new[i] = gi++;
		}
		
		if (i == insertafter)
		{
			for(int j=0;j<movelist->CountItems();j++)
			{
				int index = (size_t)movelist->ItemAt(j);
				newgroups.AddString(gd.groupnames.StringAt(index));
				old_to_new[index] = gi++;
			}
		}
	}
	
	// change em out
	gd.groupnames = newgroups;
	
	// now we have to go through all the sprites and fix their group fields
	// to use the new indexes.
	for(int i=0;;i++)
	{
		SpriteRecord *rec = spritelist.SpriteAt(i);
		if (!rec) break;
		
		if (rec->group >= 0 && rec->group <= gd.groupnames.CountItems())
			rec->group = old_to_new[rec->group];
	}
}

// deletes the group with index 'delgroup', and "kicks" all sprites that were previously
// in the group out to the group with index 'kickgroup'.
void DeleteGroup(int delgroup, int kickgroup)
{
	// remove all members from delgroup first and move them out to kickgroup
	for(int i=0;;i++)
	{
		SpriteRecord *sprite = spritelist.SpriteAt(i);
		if (!sprite) break;
		
		if (sprite->group == delgroup)
			sprite->group = kickgroup;
		
		// go ahead and re-order indexes now since we're already iterating
		// and it's not that complicated a process
		if (sprite->group > delgroup)
			sprite->group--;
	}
	
	// remove the group itself
	gd.groupnames.RemoveString(delgroup);
}

/*
void c------------------------------() {}
*/

int CountSpritesheetUsage(int sheetno, int *firstuse_out)
{
	if (firstuse_out) *firstuse_out = -1;
	
	int count = 0;
	int nsprites = spritelist.CountItems();
	for(int i=0;i<nsprites;i++)
	{
		if (spritelist.SpriteAt(i)->spritesheet == sheetno)
		{
			if (count == 0 && firstuse_out) *firstuse_out = i;
			count++;
		}
	}
	
	return count;
}

void RemoveSpritesheet(int sheetno)
{
	gd.sheetfiles.RemoveString(sheetno);
	
	int nsprites = spritelist.CountItems();
	for(int i=0;i<nsprites;i++)
	{
		if (spritelist.SpriteAt(i)->spritesheet >= sheetno)
			spritelist.SpriteAt(i)->spritesheet--;
	}
}


/*
void c------------------------------() {}
*/

bool load_spritesheet(wxImage *image, const char *filename)
{
	filename = PrefixPath(filename);
	
	if (!file_exists(filename))
	{
		staterr("LoadSpritesheet: file not exist: '%s'", filename);
		return 1;
	}
	
	if (!image->LoadFile(wxString(filename, wxConvUTF8))) return 1;
	if (!image->IsOk()) return 1;
	return 0;
}

/*
void c------------------------------() {}
*/

SpriteRecord *CurSprite()
{
	return spritelist.SpriteAt(gd.cursprite);
}

SIFFrame *CurFrame()
{
	SpriteRecord *spr = CurSprite();
	
	if (spr && gd.curframe >= 0 && gd.curframe < spr->nframes)
		return &spr->frame[gd.curframe];
	
	return NULL;
}

SIFDir *CurDir()
{
	SpriteRecord *spr = CurSprite();
	if (!spr) return NULL;
	
	if (gd.curframe >= 0 && gd.curframe < spr->nframes && \
		gd.curdir >= 0 && gd.curdir < spr->ndirs && \
		gd.curdir < SIF_MAX_DIRS)
	{
		return &spr->frame[gd.curframe].dir[gd.curdir];
	}
	
	return NULL;
}

int CurFrameCount()
{
	SpriteRecord *spr = CurSprite();
	return spr ? spr->nframes : NULL;
}

int CurDirCount()
{
	SpriteRecord *spr = CurSprite();
	return spr ? spr->ndirs : NULL;
}

bool IsValidSprite(int s)
{
	return (s >= 0 && s < spritelist.CountItems());
}

bool IsValidFrame(int s, int f)
{
	SpriteRecord *spr = spritelist.SpriteAt(s);
	if (!spr) return false;
	
	return (f >= 0 && f < spr->nframes);
}

bool IsValidDir(int s, int d)
{
	SpriteRecord *spr = spritelist.SpriteAt(s);
	if (!spr) return false;
	
	return (d >= 0 && d < spr->ndirs);
}

