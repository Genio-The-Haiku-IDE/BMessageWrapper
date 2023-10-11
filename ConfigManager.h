#pragma once
#include "GMessage.h"

class BView;
class BControl;

class ConfigManager {

public:
		void	Init();

		template<typename T>
		void AddConfig(const char* key, const char* description, T default_value, BMessage* extra = nullptr) {
			GMessage configKey;
			configKey["key"]			= key;
			configKey["description"]    = description;
			configKey["default_value"]  = default_value;
			configKey["type_code"] = MessageValue<T>::Type();
			if (extra)
				configKey["extra"] = *extra;

			configuration.AddMessage("config", &configKey);

		}


		void ResetToDefault() {
			GMessage msg;
			int i=0;
			while(configuration.FindMessage("config", i++, &msg) == B_OK) {
				storage[msg["key"]] = msg["default_value"];
			}
		}

		void Print() {
			storage.PrintToStream();
			configuration.PrintToStream();
		}

		auto operator[](const char* key) {
			type_code type;
			if (storage.GetInfo(key, &type) != B_OK) {
				printf("No info for key [%s]\n", key);
				throw new std::exception();
			}
			return storage[key];
		}

		bool Has(GMessage& msg, const char* key) {
			type_code type;
			return (msg.GetInfo(key, &type) == B_OK);
		}

		BView* MakeView();
		BView* MakeViewFor(const char* groupName, GMessage& config);
		BView* MakeSelfHostingViewFor(GMessage& config);
		BView* MakeViewFor(GMessage& config);


protected:
		GMessage storage;
		GMessage configuration;
};
