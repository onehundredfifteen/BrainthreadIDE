#pragma once

#include "unmanaged/btheaders/BTProcessMemoryInfo.h"

namespace BrainthreadIDE
{
    using namespace System;	
	using namespace System::Data;
	using namespace System::Windows::Forms;

	public ref class ThreadResourceState
	{
	public:	
		ThreadResourceState(TreeNode ^ node, array<int> ^ mem_image, int code_pos, int mem_pos, int lnz_pos) : trace(true)
		{
			this->memoryImage = mem_image;
			this->memoryInfoNode = node;

			this->CodePosition = code_pos;
			this->MemoryPosition = mem_pos;
			this->MemoryLNZPos = lnz_pos;
		}

		ThreadResourceState() : trace(true)
		{
			this->memoryImage = gcnew array<int>(1);
			this->memoryInfoNode = gcnew TreeNode("New thread spawning");

			this->CodePosition = 0;
			this->MemoryPosition = 0;
			this->MemoryLNZPos = 0;
		}

	private:	
		bool trace;
		array<int> ^ memoryImage;
		TreeNode ^ memoryInfoNode;

	public:

		int CodePosition;
		int MemoryPosition;
		int MemoryLNZPos;

		property bool Trace
		{
			bool get() { 
				return trace; 
			}
			void set(bool val) { 
				trace = val; 

				if(trace == false)
					this->setGrayNodeCaptions();
			}
		}

		property array<int> ^ MemoryImage
		{
			array<int> ^ get() { 
				return memoryImage; 
			}
			/*void set(array<int> ^ val) { 
				memoryImage = val; 
			}*/
		}

		property TreeNode ^ MemoryInfoNode
		{
			TreeNode ^ get() { 
				return memoryInfoNode; 
			}
			void set(TreeNode ^ val) { 
				memoryInfoNode = val; 
			}
		}

	private:
		void setGrayNodeCaptions()
		{
			for each(TreeNode ^ childNode in memoryInfoNode->Nodes)
			{
				childNode->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
				for each(TreeNode ^ _childNode in childNode->Nodes)
					_childNode->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
			}
		}

	};
}