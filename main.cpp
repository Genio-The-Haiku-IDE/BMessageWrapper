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
 msg2["details"] = {{ {"id", 442}, {"cost", 78} }};
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
 msgC["complex"]["nested"]["structure"] = {{ {"min", 0}, {"max", 100} }};
 msgC.PrintToStream();
  msgC["complex"]["nested"]["structure"]["max"] = 9999;
  msgC.PrintToStream();

 GMessage msg13({{{ {"what", 12}, {"total", 1}, {"unread", 1}, {"new", 1} }}});
 auto msg14 = new GMessage({{{ {"what", 12}, {"total", 1}, {"unread", 1}, {"new", 1} }}});
}

#include "GMessage2.h"
int
main(int argc, char **argv)
{
	//testGMessage();
	//testConfigManager();
	//testApp(0, nullptr);

	main2();
	return 0;
}

