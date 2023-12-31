/*
 * Copyright 2018, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <stdio.h>
#include "GMessage.h"

void testGMessage() {
	GMessage msg;
	msg["name"] = "Andrea";
	msg["points"] = 1412;
	msg["active"] = true;

	printf("Points: %d\n", (int32)msg["points"]);

	msg.PrintToStream();

	GMessage msg1 = {{ {"name", "Andrea"}, {"points", 1412}, {"active", true} }};

	msg1.PrintToStream();

	GMessage msg2 = msg1;
	msg2["details"] = { {"id", 442}, {"cost", 78} };
	GMessage x = msg2["details"];
	int i = msg2["details"]["id"];
	printf("qui---\n");
	msg2["details"]["ixx"] = true;
	msg2["details"]["ixx"]["subxx"] = true;

	auto ret =  msg2["details"]["ixx"];
	ret.Print();
	ret = true;
	ret.Print();
	printf("ret = %d --\n", i );
	msg2.PrintToStream();

	GMessage msgC = {{ {"key", "build_on_save"} }};
	msgC["complex"]["nested"]["structure"] = { {"min", 0}, {"max", 100} };
	msgC.PrintToStream();
	msgC["complex"]["nested"]["structure"]["max"] = 9999;
	msgC.PrintToStream();

	GMessage msg13({ {"what", 12}, {"total", 1}, {"unread", 1}, {"new", 1} });
	auto msg14 = new GMessage({ {"what", 12}, {"total", 1}, {"unread", 1}, {"new", 1} });

	//Real case example (from Genio)
	GMessage levels = { {"mode", "options"} };
	levels["option_1"]["value"] = (int32)0;
	levels["option_1"]["label"] = "Off";
	levels["option_2"]["value"] = (int32)1;
	levels["option_2"]["label"] = "Error";
	levels["option_3"]["value"] = (int32)2;
	levels["option_3"]["label"] = "Info";
	levels["option_4"]["value"] = (int32)3;
	levels["option_4"]["label"] = "Debug";
	levels["option_5"]["value"] = (int32)4;
	levels["option_5"]["label"] = "Trace";
	levels.PrintToStream();

	GMessage levels2 = {
		{"mode", "options"},
		{"option_1", {
			{"value", (int32)0 },
			{"label", "Off" }}},
		{"option_2", {
			{"value", (int32)1 },
			{"label", "Error"}}},
		{"option_3", {
			{"value", (int32)2 },
			{"label", "Info"}}},
		{"option_4", {
			{"value", (int32)4 },
			{"label", "Debug"}}},
		{"option_5", {
			{"value", (int32)5 },
			{"label", "Trace"}}}
	};
	levels2.PrintToStream();

	GMessage levels3 = {
		{"what", 'HOLA' },
		{"mode", "options"},
		{"option_1", {
			{"what", 'HERE' },
			{"value", (int32)0 },
			{"label", "Off" }}}
	};
	levels3.PrintToStream();
}

#include <vector>

void
testPointer()
{
	std::vector<int>* v = new std::vector<int>();
	v->push_back(2023);
	GMessage point;
	point["pointer"] = (void*)v;


	printf("Vector: %d (%x)\n", v->front(), v);
	v = nullptr;
	//point.PrintToStream();

	std::vector<int>* x = (std::vector<int>*)(void*)point["pointer"];
	printf("Vector: %d (%x)\n", x->front(), x);

}
#include <Path.h>
void testRef()
{
	entry_ref r;
	BEntry p("/boot/home");
	p.GetRef(&r);
	GMessage g {{"ref", r}};
	g["path"] = r;

	g.PrintToStream();
}

#include <String.h>

#include "magic.h"
int
main(int argc, char **argv)
{
/*
	BMessage test;
	BString msg = "Test Value";

	printf("Before: [%s]\n", msg.String());

	test.AddData("result", B_ANY_TYPE, &msg, sizeof(BString));

	test.PrintToStream();

	BString* dead = nullptr;
	ssize_t len;
	test.FindData("result", B_ANY_TYPE, (const void**)&dead, &len);

	printf("After: [%s]\n", dead->String());

	testGMessage();
	*/
	testPointer();
	testRef();
	//magic();
	return 0;
}

