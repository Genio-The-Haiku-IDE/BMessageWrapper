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


	cfg.AddConfig("Editor", "edit_fontsize", B_TRANSLATE("Font size:"), -1);

/*

	status += file.SetInt32("edit_fontsize", Settings.edit_fontsize);
	status += file.SetInt32("syntax_highlight", Settings.syntax_highlight);
	status += file.SetInt32("tab_width", Settings.tab_width);
	status += file.SetInt32("brace_match", Settings.brace_match);
	status += file.SetInt32("save_caret", Settings.save_caret);
	status += file.SetInt32("show_linenumber", Settings.show_linenumber);
	status += file.SetInt32("show_commentmargin", Settings.show_commentmargin);
	status += file.SetInt32("mark_caretline", Settings.mark_caretline);
	status += file.SetInt32("show_edgeline", Settings.show_edgeline);
	status += file.SetString("edgeline_column", Settings.edgeline_column);
	status += file.SetInt32("enable_folding", Settings.enable_folding);
	status += file.SetInt32("enable_notifications", Settings.enable_notifications);
	status += file.SetInt32("wrap_console", Settings.wrap_console);
	status += file.SetInt32("console_banner", Settings.console_banner);
	status += file.SetInt32("build_on_save", Settings.build_on_save);
	status += file.SetInt32("save_on_build", Settings.save_on_build);
	status += file.SetInt32("editor_zoom", Settings.editor_zoom);
	status += file.SetBool("find_wrap", 	  Settings.find_wrap);
	status += file.SetBool("find_whole_word", Settings.find_whole_word);
	status += file.SetBool("find_match_case", Settings.find_match_case);
	status += file.SetInt32("log_destination", Settings.log_destination);
	status += file.SetInt32("log_level", Settings.log_level);
	status += file.SetInt32("trim_trailing_whitespace", Settings.trim_trailing_whitespace);
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
