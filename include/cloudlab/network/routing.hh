#ifndef CLOUDLAB_ROUTING_HH
#define CLOUDLAB_ROUTING_HH

#include "cloudlab/kvs.hh"
#include "cloudlab/network/address.hh"

#include <algorithm>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cloudlab {

/**
 * Routing class will be useful in later tasks.
 */
class Routing {
 public:
  explicit Routing(const std::string& backend_address)
      : backend_address{SocketAddress{backend_address}} {
  }

  auto get_backend_address() -> SocketAddress {
    return backend_address;
  }

 private:
  // API requests are forwarded to this address
  const SocketAddress backend_address;
};

}  // namespace cloudlab

#endif  // CLOUDLAB_ROUTING_HH
