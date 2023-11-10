/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GMessage2_H
#define GMessage2_H
#include "GMessage.h"
#include <map>
#include <string>
#include <variant>
#include <memory>

#define KV(T) kv_pair(const std::string &k, T v) : pair(k, std::make_shared<mapped_type>(v)) {}

#define SUPPORTED_1		int32,bool, const char*, GMessage2
#define SUPPORTED_2		KV(int32); KV(bool); KV(const char*); KV(GMessage2);


class GMessage2 : public GMessage
{
public:

  using mapped_type = std::variant<SUPPORTED_1>;
  struct kv_pair : public std::pair<std::string, std::shared_ptr<mapped_type>>
  { SUPPORTED_2 };

  GMessage2(){};
  GMessage2(std::initializer_list<kv_pair> entries);

protected:
  std::map<std::string, mapped_type> _entries;
};

MESSAGE_VALUE_REF(Message, GMessage2, B_MESSAGE_TYPE);

GMessage2::GMessage2(std::initializer_list<kv_pair> entries)
  {
    for (auto &[k, v]: entries)
    {
		std::visit([&] (const auto& z) {
			(*((GMessage*)this))[k.c_str()] = z;
		}, *v);
    }
  }

int main2(void)
{
  GMessage2 _ = {
    { "level1_1", true },
    { "level1_2", 1    },
    { "level2", {
        { "level2_1", 2     },
        { "level2_2", false },
		{ "peperone", "rosso"}
      },
    },
  };

  GMessage2* msg = new GMessage2({{"name", "Genio"}});
  _.PrintToStream();
  msg->PrintToStream();
  GMessage msg2 = _["level2"];//.PrintToStream();
  msg2.PrintToStream();
  return 0;
}
#endif // GMessage2_H
