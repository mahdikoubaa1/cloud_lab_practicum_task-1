#include "cloudlab/network/connection.hh"
#include "cloudlab/flags.hh"
#include "cloudlab/network/address.hh"

#include "fmt/core.h"

#include "cloud.pb.h"
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <arpa/inet.h>
#include <event2/event.h>
#include <netdb.h>
#include <unistd.h>

namespace cloudlab {

Connection::Connection(const SocketAddress& address) {
  addrinfo hints{}, *req = nullptr;
  memset(&hints, 0, sizeof(addrinfo));

  if (address.is_ipv4()) {
    hints.ai_family = AF_INET;
    hints.ai_addrlen = sizeof(struct sockaddr_in);
  } else {
    hints.ai_family = AF_INET6;
    hints.ai_addrlen = sizeof(struct sockaddr_in6);
  }

  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = IPPROTO_TCP;

  if (getaddrinfo(address.get_ip_address().c_str(),
                  std::to_string(address.get_port()).c_str(), &hints,
                  &req) != 0) {
    throw std::runtime_error("getaddrinfo() failed");
  }
  fd = socket(req->ai_family, req->ai_socktype, req->ai_protocol);
  if (fd == -1) {
    throw std::runtime_error("socket() failed");
  }

  // TODO(you): perform the socket connection.
  // see: setsockopt and connect
  const int enable = 1;

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    throw std::runtime_error("setsockopt() failed");

  if (connect(fd, (req->ai_addr), sizeof(*req->ai_addr)) < 0)
    throw std::runtime_error("connect() failed");

  freeaddrinfo(req);
}

Connection::Connection(const std::string& address)
    : Connection(SocketAddress{address}) {
}

Connection::~Connection() {
  close(fd);
}

/***
 * The implementation of sending and receiving messages is flexible. The message
 * syntax specified on cloud.proto and the respective api from protobuff shoud
 * be used. See https://developers.google.com/protocol-buffers/docs/cpptutorial
 */
auto Connection::receive(cloud::CloudMessage& msg) const -> bool {
  char a[max_message_size] = {};
  if (fd == -1) {
    if (bufferevent_read(static_cast<bufferevent*>(bev), a, max_message_size) <
        0)
      return false;
  }

  else if (read(fd, a, max_message_size) < 0)
    return false;
  return msg.ParseFromString(std::string(a));
}

auto Connection::send(const cloud::CloudMessage& msg) const -> bool {
  std::string s;
  if (!msg.SerializePartialToString(&s)) return false;
  if (fd == -1) {
    return bufferevent_write(static_cast<bufferevent*>(bev), s.c_str(),
                             s.length()) >= 0;
  }

  return  (write(fd, s.c_str(), s.length()) < 0);
}

}  // namespace cloudlab