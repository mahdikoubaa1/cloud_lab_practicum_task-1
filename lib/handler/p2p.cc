#include "cloudlab/handler/p2p.hh"
#include "cloudlab/flags.hh"

#include "fmt/core.h"

#include "cloud.pb.h"

namespace cloudlab {

P2PHandler::P2PHandler(Routing& routing) : kvs{"/tmp/kvs"}, routing{routing} {
}

auto P2PHandler::handle_connection(Connection& con) -> void {
  cloud::CloudMessage request{}, response{};

  if (!con.receive(request)) {
    return;
  }

  if (request.type() != cloud::CloudMessage_Type_REQUEST) {
    throw std::runtime_error("expected a request");
  }

  switch (request.operation()) {
    case cloud::CloudMessage_Operation_PUT: {
      handle_put(con, request);
      break;
    }
    case cloud::CloudMessage_Operation_GET: {
      handle_get(con, request);
      break;
    }
    case cloud::CloudMessage_Operation_DELETE: {
      handle_delete(con, request);
      break;
    }
    default:
      response.set_type(cloud::CloudMessage_Type_RESPONSE);
      response.set_operation(request.operation());
      response.set_success(false);
      response.set_message("Operation not (yet) supported");

      con.send(response);

      break;
  }
}

auto P2PHandler::handle_put(Connection& con, const cloud::CloudMessage& msg)
    -> void {
  cloud::CloudMessage response{};

  // TODO(you)

  con.send(response);
}

auto P2PHandler::handle_get(Connection& con, const cloud::CloudMessage& msg)
    -> void {
  cloud::CloudMessage response{};

  // TODO(you)

  con.send(response);
}

auto P2PHandler::handle_delete(Connection& con, const cloud::CloudMessage& msg)
    -> void {
  cloud::CloudMessage response{};

  // TODO(you)

  con.send(response);
}

}  // namespace cloudlab