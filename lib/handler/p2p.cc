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
  response.set_type(cloud::CloudMessage_Type_RESPONSE);
  response.set_operation(msg.operation());
  for (const auto& kvp : msg.kvp()) {
    if (!kvs.put(kvp.key(), kvp.value())) {
      response.set_success(false);
      response.set_message("Could not put!!");
      con.send(response);
      return;
    }
    auto* tmp = response.add_kvp();
    tmp->set_value(kvp.value());
    tmp->set_key(kvp.key());
  }
  response.set_success(true);
  response.set_message("Success!");

  con.send(response);
}

auto P2PHandler::handle_get(Connection& con, const cloud::CloudMessage& msg)
    -> void {
  cloud::CloudMessage response{};
  response.set_type(cloud::CloudMessage_Type_RESPONSE);
  response.set_operation(msg.operation());
  for (const auto& kvp : msg.kvp()) {
    auto* tmp = response.add_kvp();
    auto res = tmp->value();
    tmp->set_key(kvp.key());
    if (!kvs.get(kvp.key(), res)) {
      response.set_success(false);
      response.set_message("Could not get!!");
      con.send(response);
      return;
    }
    tmp->set_value(res);
  }
  response.set_success(true);
  response.set_message("Success!");

  con.send(response);
}

auto P2PHandler::handle_delete(Connection& con, const cloud::CloudMessage& msg)
    -> void {
  cloud::CloudMessage response{};

  response.set_type(cloud::CloudMessage_Type_RESPONSE);
  response.set_operation(msg.operation());
  for (const auto& kvp : msg.kvp()) {
    auto* tmp = response.add_kvp();
    auto res = tmp->value();
    tmp->set_key(kvp.key());
    if (!kvs.get(kvp.key(), res)) {
      response.set_success(false);
      response.set_message("could not delete!!");
      con.send(response);
      return;
    }
    tmp->set_value(res);
    if (!kvs.remove(kvp.key())){
      response.set_success(false);
      response.set_message("");
      con.send(response);
      return;
    }
  }
  response.set_success(true);
  response.set_message("Success!");
  con.send(response);
}

}  // namespace cloudlab