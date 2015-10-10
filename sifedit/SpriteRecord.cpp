
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "SpriteRecord.h"
#include "SpriteRecord.fdh"

SpriteRecord::SpriteRecord()
{
	SIFSprite::Init();		// initialize the fully-POD base class
}

SpriteRecord::SpriteRecord(SIFSprite *insprite)
{
	SIFSprite::CopyFrom(insprite);
}

SpriteRecord::~SpriteRecord()
{
	SIFSprite::FreeData();
}

SpriteRecord *SpriteRecord::Duplicate()
{
	SpriteRecord *rec = new SpriteRecord;
	
	rec->CopyFrom(this);
	return rec;
}

void SpriteRecord::CopyFrom(SpriteRecord *other)
{
	SIFSprite::CopyFrom(other);
	name.SetTo(other->name);
	comment.SetTo(other->comment);
	group = other->group;
}

/*
void c------------------------------() {}
*/

SpriteList::SpriteList()
{
}

SpriteList::~SpriteList()
{
	MakeEmpty();
}

/*
void c------------------------------() {}
*/

SpriteRecord *SpriteList::SpriteAt(int index)
{
	return (SpriteRecord *)fList.ItemAt(index);
}

int SpriteList::CountItems()
{
	return fList.CountItems();
}

void SpriteList::MakeEmpty()
{
	for(int i=0;;i++)
	{
		SpriteRecord *rec = SpriteAt(i);
		if (!rec) break;
		
		delete rec;
	}
	
	fList.MakeEmpty();
}

/*
void c------------------------------() {}
*/

SpriteRecord *SpriteList::AddSprite(SpriteRecord *record)
{
	if (record)
		fList.AddItem(record);
	
	return record;
}

SpriteRecord *SpriteList::AddSprite(SpriteRecord *record, int index)
{
	if (record)
		fList.AddItem(record, index);
	
	return record;
}

void SpriteList::DeleteSprite(int index)
{
	delete (SpriteRecord *)fList.RemoveItem(index);
}

SpriteRecord *SpriteList::RemoveSprite(int index)
{
	return (SpriteRecord *)fList.RemoveItem(index);
}

void SpriteList::SwapSprites(int index1, int index2)
{
	fList.SwapItems(index1, index2);
}

/*
void c------------------------------() {}
*/

int SpriteList::IndexOf(SpriteRecord *sprite)
{
	return fList.IndexOf(sprite);
}

SpriteRecord *SpriteList::RemoveSprite(SpriteRecord *sprite)
{
	return RemoveSprite(IndexOf(sprite));
}

void SpriteList::DeleteSprite(SpriteRecord *sprite)
{
	DeleteSprite(IndexOf(sprite));
}

/*
void c------------------------------() {}
*/






