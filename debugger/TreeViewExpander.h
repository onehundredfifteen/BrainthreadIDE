#pragma once

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Collections;
	using namespace System::Windows::Forms;

	/*
	This class can remember exanded nodes on a TreeView component
	but only a two 'layers' depth (for this purpose is sufficient)
	*/

	public ref class TreeViewExpander 
	{
	public: TreeViewExpander(TreeView ^ tv) 
				: treeView(tv)
			{
				this->expandStates = gcnew ArrayList(this->treeView->Nodes->Count);

				//get first layer
				for each(TreeNode ^ node in this->treeView->Nodes) {
					this->expandStates->Add( gcnew ExpanderItem(node));
				}
			}

	public: ref class ExpanderItem 
			{
			public: ExpanderItem(TreeNode ^ node) 
						: isExpanded(node->IsExpanded) {
						this->items = gcnew ArrayList;

						for each(TreeNode ^ _node in node->Nodes)
								this->items->Add(gcnew ExpanderItem(_node));

					}

			public: void Expand(TreeNode^ node) {
						System::Collections::IEnumerator ^ etor = (safe_cast<System::Collections::IEnumerable^>(node->Nodes))->GetEnumerator();

						if(this->isExpanded)
							node->Expand();

						for each(ExpanderItem ^ item in this->items)
							if(etor->MoveNext()) {
								item->Expand( safe_cast<TreeNode^>(etor->Current) );
							}	
					}
			private:
				bool isExpanded;
				ArrayList ^ items;
			};

	public: virtual void Expand() 
			{
				this->CollapseAll();
				if(expandStates->Count > 0) {
					System::Collections::IEnumerator ^ etor = (safe_cast<System::Collections::IEnumerable^>(this->treeView->Nodes))->GetEnumerator();
					for each(ExpanderItem ^ item in this->expandStates)
						if(etor->MoveNext()) {
								item->Expand( safe_cast<TreeNode^>(etor->Current) );
							}	
				}
			}

	public: virtual void ExpandAll() 
			{
				this->treeView->ExpandAll();
			}
	public: virtual void CollapseAll() 
			{
				this->treeView->CollapseAll();
			}
	
	protected: 
		TreeView ^ treeView;
		ArrayList ^ expandStates; //System::Collections::Generic::List<ExpanderItem ^> ^ expandStates;
	};
}