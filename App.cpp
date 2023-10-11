/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "ConfigManager.h"

App::App(): BApplication("application/x-vnd.message-config")
{
}



void
App::ReadyToRun()
 {
	//	Initialization
	ConfigManager cfg;

	GMessage powerExtra = {{ {"min", 0}, {"max", 100} }};
	cfg.AddConfig("power", "Energy to use", 75, &powerExtra);

	GMessage compilerExtra = {{ {"mode", "choise"} }};
	compilerExtra["choice_1"]["value"] = "gcc";
	compilerExtra["choice_1"]["description"] = "gcc";
	compilerExtra["choice_2"]["value"] = "clang";
	compilerExtra["choice_2"]["description"] = "CLang Compiler";

	cfg.AddConfig("compiler", "Compiler to use", "gcc", &compilerExtra);

	cfg.AddConfig("active", "Use new engine", true);
	cfg.ResetToDefault();

	cfg.Print();



	// Settings settings;
	// settings.AddBoolSetting("test bool", "Bool 1", true, "first");
	// settings.AddIntSetting("test int", "Int 1", 99, "first");
	// settings.AddStringSetting("test string", "String", "foobar", "first");
	//
	// settings.AddBoolSetting("test bool 2", "Boolean 2", false, "second");
	// settings.AddIntSetting("test int 2", "INT 2", 99, "second");
	// settings.AddStringSetting("test string 2", "Test String 2", "foobar", "second");
	// status_t status = settings.Initialize();
	// if (status != B_OK) {
		// std::cerr << "Failed to initialize settings: " << ::strerror(status) << std::endl;
		// exit(-1);
	// }
	//
	//Load from file
	// settings.Load("/home/settings_test");
	//
	//Usage
	// bool test = settings.GetBool("test bool");
	// assert(test == true);
	//
	// settings.SetBool("test bool", false);
	// test = settings.GetBool("test bool");
	// assert(test == false);
	//
	// int32 intValue = settings.GetInt("test int");
	// assert(intValue == 99);
	//
	// BString stringValue = settings.GetString("test string");
	// assert(strcmp(stringValue.String(), "foobar") == 0);

	 BWindow* window = new BWindow(BRect(100, 100, 400, 400), "test", B_TITLED_WINDOW,
		 B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_QUIT_ON_WINDOW_CLOSE);
	 window->SetLayout(new BGroupLayout(B_HORIZONTAL));
	 window->AddChild(cfg.MakeView());
	 window->Show();
}
