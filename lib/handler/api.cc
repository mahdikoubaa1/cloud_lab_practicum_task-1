#include "cloud.pb.h"
#include "cloudlab/flags.hh"

#include "cloudlab/handler/api.hh"

#include "fmt/core.h"

namespace cloudlab {

void APIHandler::handle_connection(Connection& con) {
  cloud::CloudMessage request{}, response{};

  if (!con.receive(request)) {
    return;
  }

  if (request.type() != cloud::CloudMessage_Type_REQUEST) {
    throw std::runtime_error("expected a request");
  }

  auto backend_address = routing.get_backend_address();

  Connection backend{backend_address};

  switch (request.operation()) {
    case cloud::CloudMessage_Operation_PUT:
    case cloud::CloudMessage_Operation_GET:
    case cloud::CloudMessage_Operation_DELETE: {
      backend.send(request);
      backend.receive(response);
      break;
    }
    default:
      response.set_type(cloud::CloudMessage_Type_RESPONSE);
      response.set_operation(request.operation());
      response.set_success(false);
      response.set_message("Operation not supported");
      break;
  }

  con.send(response);
}

}  // namespace cloudlab