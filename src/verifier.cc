#include "cloudlab/flags.hh"
#include "cloudlab/network/connection.hh"

#include "cloud.pb.h"

#include "argh.hh"
#include <fmt/core.h>

using namespace cloudlab;

auto main(int argc, char *argv[]) -> int {
  argh::parser cmdl({"-s", "-m"});
  cmdl.parse(argc, argv);

  std::string api_address;
  cmdl({"-s"}, "127.0.0.1:31000") >> api_address;
  int messages;
  cmdl({"-m"}, "10000") >> messages;
#if VERBOSE
  fmt::print("-s {} -m {}\n", api_address, messages);
#endif

  for (int i = 0; i < messages; i++) {
    Connection con{api_address};
    cloud::CloudMessage msg{};
    msg.set_type(cloud::CloudMessage_Type_REQUEST);
    msg.set_operation(cloud::CloudMessage_Operation_GET);
    auto *tmp = msg.add_kvp();
    tmp->set_key(std::to_string(i));
    con.send(msg);

    con.receive(msg);
    if (!msg.success()) {
#if VERBOSE
      fmt::print("{}\n", msg.message());
#endif
      return 1;
    } else {
      for (const auto &kvp : msg.kvp()) {
        if (kvp.value().compare("ERROR") == 0) {
#if VERBOSE
          fmt::print("Key:\t{}\nValue:\t{}\n", kvp.key(), kvp.value());
#endif
          return 1;
        } else if (kvp.value().compare(std::to_string(i)) != 0) {
#if VERBOSE
          fmt::print("Mismatch: Key:\t{}\nValue:\t{}\n", kvp.key(),
                     kvp.value());
#endif
          return 1;
        }
      }
    }
  }
  return 0;
}
