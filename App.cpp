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
//fix me:
struct Logger {
	enum LOGGER_DEST {
		LOGGER_DEST_STDOUT = 0,
		LOGGER_DEST_STDERR = 1,
		LOGGER_DEST_SYSLOG = 2,
		LOGGER_DEST_BEDC   = 3
	};
};

typedef enum log_level {
	LOG_LEVEL_UNSET		= -1,
	LOG_LEVEL_OFF		= 1,
	LOG_LEVEL_ERROR		= 2,
	LOG_LEVEL_INFO		= 3,
	LOG_LEVEL_DEBUG		= 4,
	LOG_LEVEL_TRACE		= 5
} log_level;

void
PrepareConfig(ConfigManager& cfg) {

	cfg.AddConfig("General", "projects_directory", B_TRANSLATE("Projects folder:"), "/boot/home/workspace");
	cfg.AddConfig("General", "fullpath_title", B_TRANSLATE("Show full path in window title"), true);
	GMessage loggers = {{ {"mode", "options"} }};
	loggers["option_1"]["value"] = (int32)Logger::LOGGER_DEST_STDOUT;
	loggers["option_1"]["label"] = "Stdout";
	loggers["option_2"]["value"] = (int32)Logger::LOGGER_DEST_STDERR;
	loggers["option_2"]["label"] = "Stderr";
	loggers["option_3"]["value"] = (int32)Logger::LOGGER_DEST_SYSLOG;
	loggers["option_3"]["label"] = "Syslog";
	loggers["option_4"]["value"] = (int32)Logger::LOGGER_DEST_BEDC;
	loggers["option_4"]["label"] = "BeDC";
	cfg.AddConfig("General", "log_destination", B_TRANSLATE("Log destination:"), (int32)Logger::LOGGER_DEST_STDOUT, &loggers);

	GMessage levels = {{ {"mode", "options"} }};
	levels["option_1"]["value"] = (int32)LOG_LEVEL_OFF;
	levels["option_1"]["label"] = "Off";
	levels["option_2"]["value"] = (int32)LOG_LEVEL_ERROR;
	levels["option_2"]["label"] = "Error";
	levels["option_3"]["value"] = (int32)LOG_LEVEL_INFO;
	levels["option_3"]["label"] = "Info";
	levels["option_4"]["value"] = (int32)LOG_LEVEL_DEBUG;
	levels["option_4"]["label"] = "Debug";
	levels["option_5"]["value"] = (int32)LOG_LEVEL_TRACE;
	levels["option_5"]["label"] = "Trace";
	cfg.AddConfig("General", "log_level", B_TRANSLATE("Log level:"), (int32)LOG_LEVEL_ERROR, &levels);

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
	GMessage zooms = {{ {"min", -9}, {"max", 19} }};
	cfg.AddConfig("Editor", "editor_zoom", B_TRANSLATE("Editor zoom:"), 0, &zooms);

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

	//New config, in Genio currently without a UI
	cfg.AddConfig("Find", "find_wrap", "FIXME: Wrap", false);
	cfg.AddConfig("Find", "find_whole_word", "FIXME: Whole word", false);
	cfg.AddConfig("Find", "find_match_case", "FIXME: Match case", false);
}

ConfigManager cfg;

void
App::ReadyToRun()
 {
	PrepareConfig(cfg);
	cfg.ResetToDefault(); // !important

	cfg.Print();

	 BWindow* window = new BWindow(BRect(100, 100, 700, 500), "test", B_TITLED_WINDOW,
		 B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_QUIT_ON_WINDOW_CLOSE);
	 window->SetLayout(new BGroupLayout(B_HORIZONTAL));
	 window->AddChild(cfg.MakeView());
	 window->Show();


}
