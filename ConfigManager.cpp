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

BView*
ConfigManager::MakeView()
{
	bool has = configuration.HasMessage("config");
	if (!has)
		return nullptr;

	BTabView *tabView = new BTabView("config");

	GMessage msg;
	int i=0;
	while(configuration.FindMessage("config", i++, &msg) == B_OK)  {

		BView *groupView = MakeViewFor(msg["key"], msg);
		if (groupView == NULL) {
			printf("Skipping for %s\n", (const char*)msg["key"]);
			continue;
		}

		BScrollView *scrollView = new BScrollView(groupView->Name(), groupView, 0,
			true, true, B_NO_BORDER);
		scrollView->SetExplicitMinSize(BSize(B_V_SCROLL_BAR_WIDTH,
			B_H_SCROLL_BAR_HEIGHT));

		tabView->AddTab(scrollView);

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
ConfigManager::MakeViewFor(const char* groupName, GMessage& config)
{
	printf("Making for %s\n", (const char*)config["key"]);
	// Create and add the setting views
	BGroupView *view = new BGroupView(groupName, B_HORIZONTAL,
		B_USE_HALF_ITEM_SPACING);
	BGroupLayout *layout = view->GroupLayout();
	layout->SetInsets(B_USE_HALF_ITEM_INSETS);

	//std::pair<SettingsGroups::iterator, SettingsGroups::iterator> groupIter = fSettingsGroups.equal_range(groupName);
	//SettingsGroups::iterator settingIter;
	if ( true) {
		BGroupView *settingView = new BGroupView(groupName, B_VERTICAL,
			B_USE_HALF_ITEM_SPACING);
		BGroupLayout *settingLayout = settingView->GroupLayout();
		settingLayout->SetInsets(B_USE_HALF_ITEM_INSETS);

		/*for (settingIter = groupIter.first; settingIter != groupIter.second; settingIter++) {
			std::cout << "groupName: " << settingIter->first << ", setting: " << settingIter->second << std::endl;
			Setting *setting = fSettingsMap[settingIter->second];
			if (setting == NULL)
				continue;*/

//			std::cout << "Setting name: " << setting << std::endl;
			BView *parameterView = MakeSelfHostingViewFor(config);
			if (parameterView == NULL)
				return nullptr;

			settingLayout->AddView(parameterView);
		//}
		settingLayout->AddItem(BSpaceLayoutItem::CreateHorizontalStrut(10));
		layout->AddView(settingView);
	}

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

#include <CheckBox.h>
#include <Spinner.h>
#include <TextControl.h>

BView*
ConfigManager::MakeViewFor(GMessage& config)
{
	type_code type = config["type_code"];
	switch (type) {
		case B_BOOL_TYPE:
		{
			BCheckBox* cb = new BCheckBox(config["key"], config["description"], NULL);
			cb->SetValue(storage[config["key"]] ? 1 : 0);
			return cb;

		}

		case B_INT32_TYPE:
		{
			//TODO: handle the 'mode'
			BSpinner* sp = new BSpinner(config["key"], config["description"], NULL);
			sp->SetValue(storage[config["key"]]);
			GMessage extra = config["extra"];
			if (extra.Has("max"))
				sp->SetMaxValue(extra["max"]);
			if (extra.Has("min"))
				sp->SetMinValue(extra["min"]);
			return sp;
		}

		case B_STRING_TYPE:
		{
			//TODO: handle the 'mode'
			return new BTextControl(config["key"], config["description"], storage[config["key"]], NULL);
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
