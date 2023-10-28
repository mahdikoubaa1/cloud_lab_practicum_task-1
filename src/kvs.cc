#include "cloudlab/flags.hh"
#include "cloudlab/handler/api.hh"
#include "cloudlab/handler/p2p.hh"
#include "cloudlab/network/server.hh"

#include "argh.hh"
#include <fmt/core.h>

using namespace cloudlab;

auto main(int argc, char* argv[]) -> int {
  argh::parser cmdl({"-a", "--api", "-p", "--p2p"});
  cmdl.parse(argc, argv);

  std::string api_address, p2p_address;
  cmdl({"-a", "--api"}, "127.0.0.1:31000") >> api_address;
  cmdl({"-p", "--p2p"}, "127.0.0.1:32000") >> p2p_address;

  auto routing = Routing(p2p_address);

  auto api_handler = APIHandler(routing);
  auto api_server = Server(api_address, api_handler);
  auto api_thread = api_server.run();

  auto p2p_handler = P2PHandler(routing);
  auto p2p_server = Server(p2p_address, p2p_handler);
  auto p2p_thread = p2p_server.run();

  fmt::print("KVS up and running ...\n");

  api_thread.join();
  p2p_thread.join();
}