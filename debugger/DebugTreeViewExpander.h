#pragma once

#include "TreeViewExpander.h"

namespace BrainthreadIDE 
{
	/*
	DebugTreeViewSelector

	This class can remember exanded nodes on a TreeView component
	When number of nodes was changed, last node stands as constant node
	-> First N nodes are thread nodes, last one is always a shared stack node
	*/

	public ref class DebugTreeViewExpander : TreeViewExpander
	{
	public: DebugTreeViewExpander(TreeView ^ tv) : TreeViewExpander(tv)
			{
			}

	public: void CollapseAllThreads() 
			{
				for each(TreeNode ^ child_node in this->treeView->Nodes)
				{
					if(child_node->Text->Contains("Thread"))
						this->CollapseAll();
				}
			}

	protected: virtual void ApplyChanges() override 
			{
				if(this->treeView->Nodes->Count > cli::safe_cast< array<bool> ^ >(expandStates[0])->Length)
				{ //copy expand state from 1st node collection
					for (int i = this->treeView->Nodes->Count - cli::safe_cast< array<bool> ^ >(expandStates[0])->Length; i > 0; --i)
					{
						expandStates->Insert(this->treeView->Nodes->Count - 1, nullptr);
					}
				}
			}

	
	};
}