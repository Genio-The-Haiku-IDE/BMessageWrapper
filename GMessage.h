/*
 * Copyright 2023, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 // Version 0.1
 // Requires C++17
 // 
 // Example:
 //   GMessage msg; 
 //   msg["name"] = "Andrea";
 //   msg["points"] = 1412;
 //   msg["active"] = true;
 //
 //   printf("Points: %d\n", (int32)msg["points"]);
 //
 // brace-enclosed list:
 //  GMessage msg2 = {{ {"name", "Andrea"}, {"points", 1412}, {"active", true} }};
 //
 // submessages:
 //	 GMessage msg3 = msg2;
 //	 msg["details"] = {{ {"id", 442}, {"cost", 78} }};
 
#define AUTO_WHAT 

#pragma once
#include <Message.h>
//
#include <variant>
#include <vector>
#include <string>
#include <memory>

using generic_type = std::variant<bool, int32, const char*>;
struct key_value {
	const char* 	key;
	generic_type	value;

};
using variant_list = std::vector<key_value>;

class GMessageReturn;


class GMessage : public BMessage {

public:
	explicit GMessage():BMessage() {};
	explicit GMessage(uint32 what):BMessage(what){};
	GMessage(variant_list n):BMessage() { _HandleVariantList(n); };
	
	// ~GMessage() {
			 // printf("Deleting..(%x)", this); PrintToStream();
	 // }

	auto operator[](const char* key) -> GMessageReturn;

private:
	void _HandleVariantList(variant_list& list);
};

template<typename T>
class MessageValue {
public:
	static T	Get(GMessage* msg, const char* key, T value){
					T write_specific_converter;
					return Unsupported_GetValue_for_GMessage(write_specific_converter);
	}
	static void	Set(GMessage* msg, const char* key, T value){
					T write_specific_converter;
					Unsupported_SetValue_for_GMessage(write_specific_converter);
	}
	static type_code	Type();
};

class GMessageReturn {
public:
		GMessageReturn(GMessage* msg, const char* key, GMessageReturn* parent = nullptr){ 
			fMsg=msg; fKey=key; fSyncParent = parent;
		}
		
		 ~GMessageReturn() {
			if (fSyncParent) {
				(*fSyncParent) = (*fMsg);
				delete fMsg;
			}
		 }

		template< typename Return >
        operator Return() { return MessageValue<Return>::Get(fMsg, fKey); };

		template< typename T >
		void operator =(T n) { MessageValue<T>::Set(fMsg, fKey, n); }
		
		void operator =(variant_list n);
		
		auto operator[](const char* key) -> GMessageReturn;

		void operator =(GMessageReturn n) {
			type_code typeFound;
			if (n.fKey == fKey && fMsg == n.fMsg)
				return;

			if (n.fMsg->GetInfo(n.fKey, &typeFound) == B_OK) {

				const void* data = nullptr;
				ssize_t numBytes = 0;
				if (n.fMsg->FindData(n.fKey, typeFound, &data, &numBytes) == B_OK) {
					fMsg->RemoveName(fKey); //remove the key
					fMsg->SetData(fKey, typeFound, data, numBytes);
				}
			}
		}
		
		void Print() {
			fMsg->PrintToStream();
		}

private:
		GMessage* fMsg;
		const char* fKey;
		GMessageReturn* fSyncParent;
};

auto GMessage::operator[](const char* key) -> GMessageReturn { return GMessageReturn(this, key); }

void GMessage::_HandleVariantList(variant_list& list) {
	MakeEmpty();
	auto i = list.begin();
	while(i != list.end()) {
		std::visit([&] (const auto& k) {
			(*this)[(*i).key] = k;
		}, (*i).value);
		i++;
	}
}

#define MESSAGE_VALUE(NAME, TYPE, typeCODE, DEFAULT) \
template<> \
class MessageValue<TYPE> { \
public: \
	static TYPE		Get(GMessage* msg, const char* key) { return msg->Get ## NAME (key, DEFAULT); } \
	static void		Set(GMessage* msg, const char* key, TYPE value) { msg->Set ## NAME (key, value); } \
	static type_code	Type() { return typeCODE; } \
}; \


#define MESSAGE_VALUE_REF(NAME, TYPE, typeCODE, DEFAULT) \
template<> \
class MessageValue<TYPE> { \
public: \
	static TYPE		Get(GMessage* msg, const char* key) { \
						TYPE value; \
						msg->Find ## NAME (key, &value); \
						return value; } \
	static void		Set(GMessage* msg, const char* key, TYPE value) { \
						msg->RemoveName(key); \
						msg->Add ## NAME (key, &value); } \
	static type_code	Type() { return typeCODE; } \
}; 

MESSAGE_VALUE(Bool, bool, B_BOOL_TYPE, false);
MESSAGE_VALUE(String, const char*, B_STRING_TYPE, "");
MESSAGE_VALUE(Int32, int32, B_INT32_TYPE, -1);
MESSAGE_VALUE(UInt32, uint32, B_UINT32_TYPE, 0);
MESSAGE_VALUE_REF(Message, GMessage, B_MESSAGE_TYPE, GMessage());
MESSAGE_VALUE_REF(Message, BMessage, B_MESSAGE_TYPE, BMessage());

//variant_list//
/*template<>
class MessageValue<variant_list> {
public:
	static variant_list	Get(GMessage* msg, const char* key){
							GMessage* newMsg = new GMessage();
							msg->FindMessage(key, newMsg);
							return newMsg; //BUG
	}
	static void	Set(GMessage* msg, const char* key, GMessage* value){
							msg->RemoveName(key);
							msg->AddMessage(key, value);
	}
	static type_code	Type() { return B_MESSAGE_TYPE; }
};*/
/*
template<>
class MessageValue<GMessage*> {
public:
	static GMessage*	Get(GMessage* msg, const char* key){
							GMessage* newMsg = new GMessage();
							msg->FindMessage(key, newMsg);
							return newMsg; //BUG
	}
	static void	Set(GMessage* msg, const char* key, GMessage* value){
							msg->RemoveName(key);
							msg->AddMessage(key, value);
	}
	static type_code	Type() { return B_MESSAGE_TYPE; }
};*/
 

void GMessageReturn::operator =(variant_list n) {
	GMessage xmsg(n);
	MessageValue<GMessage>::Set(fMsg, fKey, xmsg);
}	


auto GMessageReturn::operator[](const char* key) -> GMessageReturn {
	GMessage* newMsg = new GMessage();
	fMsg->FindMessage(fKey, newMsg);
	return GMessageReturn(newMsg, key, this);
};