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
			{
				this->treeView = tv;
				this->expandStates = gcnew ArrayList;

				//get first layer
				expandStates->Add( this->GetExpandedState(this->treeView->Nodes) );

				//get second layers
				for each(TreeNode ^ child_node in this->treeView->Nodes)
				{
					expandStates->Add( this->GetExpandedState(child_node->Nodes) );
				}
			}

	public: virtual void Expand() 
			{
				int i = 1;
		
				if(expandStates->Count && expandStates[0])
				{
					//evaluate new items expand int 1st layer
					if(this->treeView->Nodes->Count != cli::safe_cast< array<bool> ^ >(expandStates[0])->Length)
						this->ApplyChanges();
					
					//set first layer
					this->SetExpandedState(this->treeView->Nodes, cli::safe_cast< array<bool> ^ >(expandStates[0]));

					//get second layers
					for each(TreeNode ^ child_node in this->treeView->Nodes)
					{
						this->SetExpandedState(child_node->Nodes, cli::safe_cast< array<bool> ^ >(expandStates[i++]));
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

	protected: virtual void ApplyChanges() 
			{
				if(this->treeView->Nodes->Count > cli::safe_cast< array<bool> ^ >(expandStates[0])->Length)
				{ //copy expand state from 1st node collection
					for (int i = this->treeView->Nodes->Count - cli::safe_cast< array<bool> ^ >(expandStates[0])->Length; i > 0; --i)
					{
						expandStates->Add(nullptr);
					}
				}
			}

	private: array<bool> ^ GetExpandedState(TreeNodeCollection ^ nodes) 
			{
				int i = 0;
				array<bool> ^ res;
				
				if(nodes->Count == 0)
					return nullptr;

				res = gcnew array<bool>(nodes->Count);

				for each(TreeNode ^ child_node in nodes)
				{
					res[i++] = child_node->IsExpanded;
				}

				return res;
			}

	private: void SetExpandedState(TreeNodeCollection ^ nodes, array<bool> ^ exp_states) 
			{
				int i = 0;
				
				if(exp_states == nullptr)
					return;

				for each(TreeNode ^ child_node in nodes)
				{
					if(i < exp_states->Length && exp_states[i++])
						child_node->Expand();
				}
			}
	
	protected: 
		TreeView ^ treeView;
		ArrayList ^ expandStates;
	};
}