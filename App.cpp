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
App::MessageReceived(BMessage* msg) {
	//debugger("App");
	BApplication::MessageReceived(msg);
}


#define B_TRANSLATE(X) X

void
PrepareConfig(ConfigManager& cfg) {

	cfg.AddConfig("General", "projects_directory", B_TRANSLATE("Projects folder:"), "/boot/home/workspace");
	cfg.AddConfig("General", "fullpath_title", B_TRANSLATE("Show full path in window title"), true);

	cfg.AddConfig("Startup", "reopen_projects", B_TRANSLATE("Reload projects"), true);
	cfg.AddConfig("Startup", "reopen_files", B_TRANSLATE("Reload files"), true);
	cfg.AddConfig("Startup", "show_projects", B_TRANSLATE("Show projects pane"), true);
	cfg.AddConfig("Startup", "show_output", B_TRANSLATE("Show output pane"), true);
	cfg.AddConfig("Startup", "show_toolbar", B_TRANSLATE("Show toolbar"), true);

	GMessage sizes;
	sizes = {{ {"mode","options"} }};
	sizes["option_1"]["value"] = -1;
	sizes["option_1"]["label"] = B_TRANSLATE("Default size");
	int32 c = 2;
	for (int32 i = 10; i <= 18; i++) {
		BString key("option_");
		key << c;
		BString text;
		text << i;
		sizes[key.String()]["value"] = i;
		sizes[key.String()]["label"] = text.String();
		c++;
	}

	cfg.AddConfig("Editor", "edit_fontsize", B_TRANSLATE("Font size:"), -1, &sizes);
	cfg.AddConfig("Editor", "syntax_highlight", B_TRANSLATE("Enable syntax highlighting"), true);
	cfg.AddConfig("Editor", "brace_match", B_TRANSLATE("Enable brace matching"), true);
	cfg.AddConfig("Editor", "save_caret", B_TRANSLATE("Save caret position"), true);
	cfg.AddConfig("Editor", "trim_trailing_whitespace", B_TRANSLATE("Trim trailing whitespace on save"), false);
	GMessage tabs = {{ {"min",1},{"max",8} }};
	cfg.AddConfig("Editor", "tab_width", B_TRANSLATE("Tab width:  "), 4, &tabs);

	cfg.AddConfig("Visual", "show_linenumber", B_TRANSLATE("Show line number"), true);
	cfg.AddConfig("Visual", "show_commentmargin", B_TRANSLATE("Show comment margin"), true);
	cfg.AddConfig("Visual", "mark_caretline", B_TRANSLATE("Mark caret line"), true);
	cfg.AddConfig("Visual", "show_edgeline", B_TRANSLATE("Show edge line"), true);
	cfg.AddConfig("Visual", "enable_folding", B_TRANSLATE("Enable folding"), true);
	GMessage limits = {{ {"min", 0}, {"max", 500} }};
	cfg.AddConfig("Visual", "edgeline_column", B_TRANSLATE("Show edge line"), 80, &limits);

	cfg.AddConfig("Notifications", "enable_notifications", B_TRANSLATE("Enable notifications"), true);

	cfg.AddConfig("Build", "wrap_console",   B_TRANSLATE("Wrap console"), false);
	cfg.AddConfig("Build", "console_banner", B_TRANSLATE("Console banner"), true);
	cfg.AddConfig("Build", "build_on_save",  B_TRANSLATE("Build target on resource save"), false);
	cfg.AddConfig("Build", "save_on_build",  B_TRANSLATE("Save changed files on build"), false);

/*
	status += file.SetInt32("editor_zoom", Settings.editor_zoom);
	status += file.SetBool("find_wrap", 	  Settings.find_wrap);
	status += file.SetBool("find_whole_word", Settings.find_whole_word);
	status += file.SetBool("find_match_case", Settings.find_match_case);
	status += file.SetInt32("log_destination", Settings.log_destination);
	status += file.SetInt32("log_level", Settings.log_level);
*/
}

ConfigManager cfg;

void
App::ReadyToRun()
 {
	//	Initialization
	//ConfigManager* cfg = new ConfigManager();
	PrepareConfig(cfg);

	cfg.ResetToDefault(); //!

	cfg.Print();

	//printf("Test access: %s\n", (const char*)cfg["projects_directory"]);

//	cfg->Run();

	//Invalid access
	//cfg["save_on_build"] = false;

	 BWindow* window = new BWindow(BRect(100, 100, 500, 500), "test", B_TITLED_WINDOW,
		 B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_QUIT_ON_WINDOW_CLOSE);
	 window->SetLayout(new BGroupLayout(B_HORIZONTAL));
	 window->AddChild(cfg.MakeView());
	 window->Show();
}
