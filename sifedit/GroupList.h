
#ifndef _GROUPLIST_H
#define _GROUPLIST_H

#include "../common/BList.h"

struct GroupRecord
{
	DString name;			// name of group
	wxTreeItemId treeid;	// current node ID in left-hand sprite tree
};

class GroupList : private BList
{
public:
	GroupRecord *GroupAt(int index)
	{
		return (GroupRecord *)BList::ItemAt(index);
	}
	
	const char *GroupNameAt(int index)
	{
		GroupRecord *rec = GroupAt(index);
		if (rec) return rec->name.String();
		return NULL;
	}
	
	GroupRecord *AddGroup(GroupRecord *group)
	{
		BList::AddItem(group);
	}

	GroupRecord *AddGroup(const char *name)
	{
		GroupRecord *rec = new GroupRecord;
		rec->name.SetTo(name);
		BList::AddItem(rec);
	}
	
	GroupRecord *DeleteGroup(int index)
	{
		GroupRecord *rec = (GroupRecord *)BList::RemoveItem(index);
		delete rec;
	}
	
	int CountItems()
	{
		return BList::CountItems();
	}
	
	void MakeEmpty()
	{
		while(CountItems() > 0)
			DeleteGroup(CountItems() - 1);
	}
};



#endif
