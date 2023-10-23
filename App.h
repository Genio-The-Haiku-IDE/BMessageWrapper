/*
 * Copyright 2018, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Application.h>
#include <GroupLayout.h>
#include <String.h>
#include <Window.h>


class App: public BApplication {
public:
	App();
	virtual void ReadyToRun();
	virtual void MessageReceived(BMessage* msg);
};
