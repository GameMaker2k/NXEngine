
#include <wx/treectrl.h>
#include "TreeItemList.h"
#include "TreeItemList.fdh"

struct Entry
{
	wxTreeItemId id;
};

/*
void c------------------------------() {}
*/

TreeItemList::~TreeItemList()
{
	MakeEmpty();
}

/*
void c------------------------------() {}
*/

void TreeItemList::AddItem(wxTreeItemId id)
{
	Entry *entry = new Entry;
	entry->id = id;
	
	fList.AddItem(entry);
}

wxTreeItemId TreeItemList::RemoveItem(int index)
{
	Entry *entry = (Entry *)fList.RemoveItem(index);
	wxTreeItemId old = entry->id;
	delete entry;
	
	return old;
}

bool TreeItemList::RemoveItem(wxTreeItemId id)
{
	for(int i=0;;i++)
	{
		Entry *entry = (Entry *)fList.ItemAt(i);
		if (!entry) break;
		
		if (entry->id == id)
		{
			fList.RemoveItem(i);
			delete entry;
			return 0;
		}
	}
	
	return 1;
}

void TreeItemList::MakeEmpty()
{
	for(int i=0;;i++)
	{
		Entry *entry = (Entry *)fList.ItemAt(i);
		if (!entry) break;
		
		delete entry;
	}
	
	fList.MakeEmpty();
}

int TreeItemList::CountItems()
{
	return fList.CountItems();
}

wxTreeItemId TreeItemList::ItemAt(int index)
{
	Entry *entry = (Entry *)fList.ItemAt(index);
	if (entry) return entry->id;
	
	wxTreeItemId NotIsOk;
	return NotIsOk;
}







