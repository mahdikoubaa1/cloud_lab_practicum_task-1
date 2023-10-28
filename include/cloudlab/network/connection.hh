#ifndef CLOUDLAB_CONNECTION_HH
#define CLOUDLAB_CONNECTION_HH

#include "cloudlab/network/address.hh"

namespace cloud {
class CloudMessage;
}

namespace cloudlab {

const auto max_message_size = 16384;

/**
 * Representation of a (TCP) network connection.
 */
class Connection {
 public:
  explicit Connection(const SocketAddress& address);

  explicit Connection(const std::string& address);

  explicit Connection(int fd) : fd{fd} {};

  explicit Connection(void* bev) : bev{bev} {};

  ~Connection();

  auto receive(cloud::CloudMessage& msg) const -> bool;

  // NOLINTNEXTLINE(modernize-use-nodiscard)
  auto send(const cloud::CloudMessage& msg) const -> bool;

 private:
  int fd{-1};
  void* bev{nullptr};
};

}  // namespace cloudlab

#endif  // CLOUDLAB_CONNECTION_HH
