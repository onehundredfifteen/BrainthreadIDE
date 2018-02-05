#pragma once

#include "../options/Language.h"
#include "Plugin.h"

namespace BrainthreadIDE 
{
	using namespace System;
	using namespace System::Reflection;
	using namespace System::Collections::Generic;

	public ref class PluginManager 
	{
	public: PluginManager(void)
			{
				array<String^>^ files;
				Assembly^ pluginAssembly;
	
				this->plugins = gcnew List<Plugin ^>;

				try {
					files = Directory::GetFiles( FileContext::BaseDirectory() + "\\bf" );  
				}
				catch(...) {
					return; //no plugin availible
				}
  
				for (int i = 0; i < files->Length; i++) {
					if(String::Compare(Path::GetExtension(files[i]), cPluginExt) == 0) {
						pluginAssembly = Assembly::LoadFrom(files[i]);
						// Obtain a reference to a method known to exist in assembly.
						for each (Type^ type in pluginAssembly->GetTypes()) {
							Plugin ^ new_plugin = gcnew Plugin(type, pluginAssembly->FullName);

							if(new_plugin->Valid()) {
								plugins->Add(new_plugin);
							}
							else 
								delete new_plugin;
						}
					}
				}
			}
	
	public: Plugin ^ GetPlugin(String ^ pluginName)
			{
				for each(Plugin ^ plugin in plugins)
				{
					if(plugin->Name() == pluginName && plugin->Installed) {
						return plugin;
					}
				}

				return nullptr;
			}

	public: List<String ^> ^ PluginNames()
			{
				List<String ^> ^ plugin_names = gcnew List<String ^>;
				
				for each(IBtPlugin ^ plugin in plugins) {
					plugin_names->Add( plugin->Name() ); 
				}

				return plugin_names;
			}
	public: List<String ^> ^ InvocablePluginNames()
			{
				List<String ^> ^ plugin_names = gcnew List<String ^>;
				
				for each(IBtPlugin ^ plugin in plugins) {
					if(plugin->OnInvoke("", Language::lBrainthread)) {
						plugin_names->Add( plugin->Name() ); 
					}
				}

				return plugin_names;
			}
	public: List<String ^> ^ PragmaPluginNames()
			{
				List<String ^> ^ plugin_names = gcnew List<String ^>;
				
				for each(IBtPlugin ^ plugin in plugins) {
					if(plugin->OnPragma("", Language::lBrainthread)) {
						plugin_names->Add( plugin->Name() ); 
					}
				}

				return plugin_names;
			}
	public: String ^ PluginDescription(String ^ name)
			{
				for each(Plugin ^ plugin in plugins) {
					if(plugin->Name() == name) {
						return String::Format("{0}\r\n\r\n{1}\r\n{3}\r\n\r\n{2}", name->ToUpper(), 
																						plugin->Info->Substring(0)->Insert(plugin->Info->IndexOf(','), ".dll"),  
																						plugin->Description(),
																						gcnew String('=', 44)); 
					} 
				}

				return "";
			}

	public: void GrantPrivileges(List<String ^> ^ granted_names)
			{
				for each(Plugin ^ plugin in plugins) {
					plugin->Installed = (granted_names->Contains(plugin->Name()));
				}
			}

	private:
		List<Plugin ^> ^ plugins;

	private: 
		literal String ^ cPluginExt = ".dll";
	
	};
}