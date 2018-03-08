#pragma once

#include "PluginInterface.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Reflection;

	public ref class Plugin : IBtPlugin
	{
	public: Plugin(Type^ type, String ^ ainfo)
			{
				isValidPluginStruct = false;

				this->MethodName = type->GetMethod( cMethod_IBtPlugin_Name );
				this->MethodDescription = type->GetMethod( cMethod_IBtPlugin_Description );
				this->MethodInvoke = type->GetMethod( cMethod_IBtPlugin_Invoke);
				this->MethodPragma = type->GetMethod( cMethod_IBtPlugin_Pragma );

				this->assemblyInfo = ainfo;

				ConstructorInfo^ typeConstructor = type->GetConstructor(Type::EmptyTypes);

				if(typeConstructor && this->MethodName && this->MethodDescription && this->MethodInvoke && this->MethodPragma)
				{
					this->pluginObject = typeConstructor->Invoke(gcnew array<Object^>(0));

					isValidPluginStruct = (this->pluginObject != nullptr);
				}
			}

	public: bool Valid()
			{
				return isValidPluginStruct && (Invocable || Pragmable);
			}

	public: virtual String ^ Name()
			{
				return cli::safe_cast<String^>( this->MethodName->Invoke(this->pluginObject, gcnew array<Object^>(0)));
			}
	public: virtual String ^ Description()
			{
				return cli::safe_cast<String^>( this->MethodDescription->Invoke(this->pluginObject, gcnew array<Object^>(0)));
			}
	public: virtual String ^ OnInvoke(String ^ code, Language language)
			{
				try
				{
					return cli::safe_cast<String ^>( this->MethodInvoke->Invoke(this->pluginObject, gcnew array<Object^>(2){code, language} ));
				}
				catch(Exception ^ ex)
				{
					MessageBox::Show( String::Format("Plugin {0} raised an error: {1}", this->Name(), ex->Message), "Invoke failed", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return nullptr;
				}
			}
	public: virtual String ^ OnPragma(String ^ code, Language language)
			{
				try
				{
					return cli::safe_cast<String ^>( this->MethodPragma->Invoke(this->pluginObject, gcnew array<Object^>(2){code, language} ));
				}
				catch(Exception ^ ex)
				{
					MessageBox::Show( String::Format("Plugin {0} raised an error: {1}", this->Name(), ex->Message), "Pragma failed", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return nullptr;
				}
			}

	public: property bool Installed {		
				bool get() {
					return isInstalled;
				}
				void set(bool val) {
					isInstalled = val;
				}
			}

			property String ^ Info {		
				String ^ get() {
					return assemblyInfo;
				}
			}

			property bool Invocable {		
				bool get() {
					return nullptr != this->OnInvoke("", Language::lBrainthread);
				}
			}

			property bool Pragmable {		
				bool get() {
					return nullptr != this->OnPragma("", Language::lBrainthread);
				}
			}
		
	protected:
		Object^ pluginObject;

		MethodInfo^ MethodName;
		MethodInfo^ MethodDescription;
		MethodInfo^ MethodInvoke;
		MethodInfo^ MethodPragma;
		
		String ^ assemblyInfo;
		bool isInstalled;
		bool isValidPluginStruct;

	private:
		literal String ^ cMethod_IBtPlugin_Name = "Name";
		literal String ^ cMethod_IBtPlugin_Description = "Description";
		literal String ^ cMethod_IBtPlugin_Invoke = "OnInvoke";
		literal String ^ cMethod_IBtPlugin_Pragma = "OnPragma";
	
	};
}