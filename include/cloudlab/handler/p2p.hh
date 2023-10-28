#ifndef CLOUDLAB_P2P_HH
#define CLOUDLAB_P2P_HH

#include "cloudlab/handler/handler.hh"
#include "cloudlab/kvs.hh"
#include "cloudlab/network/routing.hh"

namespace cloudlab {

/**
 * Handler for P2P requests. Takes care of the messages from peers, cluster
 * metadata / routing tier, and the API.
 */
class P2PHandler : public ServerHandler {
 public:
  explicit P2PHandler(Routing& routing);

  auto handle_connection(Connection& con) -> void override;

 private:
  // clang-format off
  auto handle_put(Connection& con, const cloud::CloudMessage& msg) -> void;
  auto handle_get(Connection& con, const cloud::CloudMessage& msg) -> void;
  auto handle_delete(Connection& con, const cloud::CloudMessage& msg) -> void;
  // clang-format on

  KVS kvs;
  Routing& routing;
};

}  // namespace cloudlab

#endif  // CLOUDLAB_P2P_HH
