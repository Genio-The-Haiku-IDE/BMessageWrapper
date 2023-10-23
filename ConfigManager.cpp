/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ConfigManager.h"
#include <ScrollBar.h>
#include <TabView.h>
#include <ScrollView.h>
#include <LayoutItem.h>
#include <GroupView.h>
#include <Layout.h>
#include <SpaceLayoutItem.h>
#include <map>
#include <Control.h>
#include <MessageFilter.h>
#include <CheckBox.h>
#include <Spinner.h>
#include <TextControl.h>

class ConfigManager;

#define ON_NEW_VALUE	'ONVA'

template<class C, typename T>
class GControl : public C {
public:
		GControl(GMessage& msg, T value, ConfigManager* cfg):fConfigManager(cfg),
			C("", "", nullptr){

				C::SetName(msg["key"]);
				C::SetLabel(msg["label"]);
				LoadValue(value);

				GMessage* invoke = new GMessage(ON_NEW_VALUE);
				(*invoke)["key"] = msg["key"];

				C::SetMessage(invoke);
			}

		void AttachedToWindow() {
			C::AttachedToWindow();
			C::SetTarget(this);
		}
		void MessageReceived(BMessage* msg) {
			if (msg->what == ON_NEW_VALUE) {
				GMessage& gsm = *((GMessage*)msg);
				RetriveValue(gsm);
				fConfigManager->UpdateValue(gsm);
			}
			C::MessageReceived(msg);
		}

		void RetriveValue(GMessage& dest) {
			dest["value"] = (T)C::Value();
		}

		void LoadValue(T value) {
			C::SetValue(value);
		}

private:
		ConfigManager*	fConfigManager;
};

template<>
void GControl<BTextControl, const char*>::RetriveValue(GMessage& dest) {
	dest["value"] = BTextControl::Text();
}
template<>
void GControl<BTextControl, const char*>::LoadValue(const char* value) {
	BTextControl::SetText(value);
}

BView*
ConfigManager::MakeView()
{
	bool has = configuration.HasMessage("config");
	if (!has)
		return nullptr;

	BTabView *tabView = new BTabView("config");

	std::map<std::string, GMessage> divededByGroup;
	GMessage msg;
	int i=0;
	while(configuration.FindMessage("config", i++, &msg) == B_OK)  {
		//printf("Adding for %s -> %s\n", (const char*)msg["group"], (const char*)msg["label"]);
		divededByGroup[(const char*)msg["group"]].AddMessage("config", &msg);
	}
	std::map<std::string, GMessage>::iterator iter = divededByGroup.begin();
	while(iter != divededByGroup.end())  {
	//iter->second.PrintToStream();
		//printf("Working for %s\n", iter->first.c_str());
		BView *groupView = MakeViewFor(iter->first.c_str(), iter->second);
		if (groupView == NULL) {
			//printf("Skipping for %s\n", iter->first.c_str());
			continue;
		}

		BScrollView *scrollView = new BScrollView(groupView->Name(), groupView, 0,
			true, true, B_NO_BORDER);
		scrollView->SetExplicitMinSize(BSize(B_V_SCROLL_BAR_WIDTH,
			B_H_SCROLL_BAR_HEIGHT));

		tabView->AddTab(scrollView);
		iter++;

	}

	/*if (hintRect != NULL) {
		tabView->MoveTo(hintRect->LeftTop());
		tabView->ResizeTo(hintRect->Size());
	} else {*/
		// Apps not using layouted views may expect PreferredSize() to return
		// a sane value right away, and use this to set the maximum size of
		// things. Layouted views return their current size until the view has
		// been attached to the window, so in order to prevent breakage, we set
		// a default view size here.
		tabView->ResizeTo(600, 400);
	/*}*/
	return tabView;
}


BView*
ConfigManager::MakeViewFor(const char* groupName, GMessage& list)
{
	// printf("Making for %s\n", (const char*)config["key"]);
	// Create and add the setting views
	BGroupView *view = new BGroupView(groupName, B_HORIZONTAL,
		B_USE_HALF_ITEM_SPACING);
	BGroupLayout *layout = view->GroupLayout();
	layout->SetInsets(B_USE_HALF_ITEM_INSETS);

	BGroupView *settingView = new BGroupView(groupName, B_VERTICAL,
		B_USE_HALF_ITEM_SPACING);
	BGroupLayout *settingLayout = settingView->GroupLayout();
	settingLayout->SetInsets(B_USE_HALF_ITEM_INSETS);

	GMessage msg;
	int i=0;
	while(list.FindMessage("config", i++, &msg) == B_OK)  {

		BView *parameterView = MakeSelfHostingViewFor(msg);
		if (parameterView == NULL)
			return nullptr;

		settingLayout->AddView(parameterView);
		if (dynamic_cast<BControl*>(parameterView)) {
			//dynamic_cast<BControl*>(parameterView)->SetTarget(this);
		}

	}
		settingLayout->AddItem(BSpaceLayoutItem::CreateHorizontalStrut(10));
		layout->AddView(settingView);



	// Add the sub-group views
/*	for (int32 i = 0; i < group.size(); i++) {
		SettingGroup *subGroup = group.at(i);
		if (subGroup == NULL)
			continue;

		BView *groupView = MakeViewFor(*subGroup);
		if (groupView == NULL)
			continue;

		if (i > 0)
			layout->AddView(new SeparatorView(B_VERTICAL));

		layout->AddView(groupView);
	}*/

	layout->AddItem(BSpaceLayoutItem::CreateGlue());
	return view;
}


BView*
ConfigManager::MakeSelfHostingViewFor(GMessage& config)
{
	/*if (parameter.Flags() & B_HIDDEN_PARAMETER
		|| parameter_should_be_hidden(parameter))
		return NULL;
*/
	BView *view = MakeViewFor(config);
	if (view == NULL) {
		// The MakeViewFor() method above returns a BControl - which we
		// don't need for a null parameter; that's why it returns NULL.
		// But we want to see something anyway, so we add a string view
		// here.
		/*if (parameter.Type() == Setting::B_NULL_PARAMETER) {
			if (parameter.Group()->ParameterAt(0) == &parameter) {
				// this is the first parameter in this group, so
				// let's use a nice title view
				return new TitleView(parameter.Name());
			}
			BStringView *stringView = new BStringView(parameter.Name(),
				parameter.Name());
			stringView->SetAlignment(B_ALIGN_CENTER);

			return stringView;
		}*/

		return NULL;
	}

	/*MessageFilter *filter = MessageFilter::FilterFor(view, setting);
	if (filter != NULL)
		view->AddFilter(filter);*/

	return view;
}

#include <OptionPopUp.h>

BView*
ConfigManager::MakeViewFor(GMessage& config)
{
	type_code type = config["type_code"];
	switch (type) {
		case B_BOOL_TYPE:
		{
			BCheckBox* cb = new GControl<BCheckBox, bool>(config, (*this)[config["key"]], this);
			return cb;
		}
		case B_INT32_TYPE:
		{
			if (config.Has("mode")) {
				if (BString((const char*)config["mode"]).Compare("options") == 0){

					BOptionPopUp* popUp = new GControl<BOptionPopUp, int32>(config, (*this)[config["key"]], this);
					int32 c=1;
					while(true) {
						BString key("option_");
						key << c;
						if (!config.Has(key.String()))
							break;
						popUp->AddOption(config[key.String()]["label"], config[key.String()]["value"]);
						c++;
					}
					return popUp;
				}
			} else {
				BSpinner* sp = new GControl<BSpinner, int32>(config, (*this)[config["key"]], this);
				sp->SetValue((*this)[config["key"]]);
				if (config.Has("max"))
					sp->SetMaxValue(config["max"]);
				if (config.Has("min"))
					sp->SetMinValue(config["min"]);
				return sp;
			}
		}

		case B_STRING_TYPE:
		{
			//TODO: handle the 'mode'
			return new GControl<BTextControl, const char*>(config, (*this)[config["key"]], this);
		}
/*
		default:
		{
			BString errorString;
			errorString.SetToFormat("Setting: Don't know setting type: 0x%x\n", setting->Type());
			debugger(errorString.String());
		}*/
	}
	return NULL;
}
