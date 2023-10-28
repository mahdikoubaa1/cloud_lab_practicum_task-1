#include "cloudlab/flags.hh"
#include "cloudlab/network/connection.hh"

#include "cloud.pb.h"

#include "argh.hh"
#include <thread>
#include <fmt/core.h>

using namespace cloudlab;
int putfromto(int from, int amount, const std::string &a) {
  for (int i = from; i < from+amount; i++) {
    Connection con{a};
    cloud::CloudMessage msg{};
    msg.set_type(cloud::CloudMessage_Type_REQUEST);
    msg.set_operation(cloud::CloudMessage_Operation_PUT);
    auto *tmp = msg.add_kvp();
    tmp->set_key(std::to_string(i));
    tmp->set_value(std::to_string(i));
    con.send(msg);
    con.receive(msg);
    if (!msg.success()) {
#if VERBOSE
      fmt::print("{}\n", msg.message());
#endif
      throw std::runtime_error("could not put key-value");

    }
  }
}
auto main(int argc, char *argv[]) -> int {
  argh::parser cmdl({"-s", "-m","-c"});
  cmdl.parse(argc, argv);

  std::string api_address;
  cmdl({"-s"}, "127.0.0.1:31000") >> api_address;
  int messages;
  cmdl({"-m"}, "10000") >> messages;
  int clients;
  cmdl({"-c"}, "1") >> clients;

  std::array<int, 2> h{messages / clients, messages % clients};
  std::vector<std::thread> retvalues (clients);
  std::vector<std::thread> clientthreads(clients);
  for (int i = 0; i < clients; i++) {
    if (i < h[1])
      clientthreads[i] =
          std::thread(putfromto, i * (h[0] + 1), h[0] + 1, api_address);
    else
      clientthreads[i] = std::thread(
          putfromto, h[1] * (h[0] + 1) + (i - h[1]) * h[0], h[0], api_address);
  }
  for (int i=0; i<clients;i++){
    clientthreads[i].join();
    }
}
