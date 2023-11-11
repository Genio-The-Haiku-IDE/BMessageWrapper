/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GMessage.h"

void GMessage::_HandleVariantList(variant_list& list) {
	MakeEmpty();
    for (auto &[k, v]: list)
    {
		std::visit([&] (const auto& z) {
			(*this)[k.c_str()] = z;
		}, *v);
    }
}

auto GMessage::operator[](const char* key) -> GMessageReturn { return GMessageReturn(this, key); }


GMessage* set_what(uint32 what, GMessage* gms) { gms->what = what; return gms; }