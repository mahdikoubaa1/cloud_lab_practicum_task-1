#include "cloudlab/kvs.hh"
#include "cloudlab/flags.hh"

#include "fmt/core.h"
#include "rocksdb/db.h"

#include <algorithm>
#include <random>

namespace cloudlab {

auto KVS::open() -> bool {
  // only open db if it was not opened yet
  if (!db) {
    // TODO(you)
  }

  return true;
}

auto KVS::get(const std::string& key, std::string& result) -> bool {
  // TODO(you)
  return {};
}

/*
 * [NOTE] - If a PUT request is issued and a DB does not yet exist
 * the server should open a new DB and insert the key-value pair
 */
auto KVS::put(const std::string& key, const std::string& value) -> bool {
  // TODO(you)
  return {};
}

/***
 * Completly removes the DB
 */
auto KVS::remove(const std::string& key) -> bool {
  // TODO(you)
  return {};
}

auto KVS::clear() -> bool {
  // TODO(you)
  return {};
}

auto KVS::begin() -> KVS::Iterator {
  // TODO(you)
  return {};
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto KVS::end() const -> KVS::Sentinel {
  return {};
}

auto KVS::Iterator::operator*()
    -> std::pair<std::string_view, std::string_view> {
  // TODO(you)(optional for task1, may be needed for other tasks)
  return {};
}

auto KVS::Iterator::operator++() -> KVS::Iterator& {
  // TODO(you)(optional for task1, may be needed for other tasks)
  return *this;
}

auto operator==(const KVS::Iterator& it, const KVS::Sentinel&) -> bool {
  // TODO(you)(optional for task1, may be needed for other tasks)
  return {};
}

auto operator!=(const KVS::Iterator& lhs, const KVS::Sentinel& rhs) -> bool {
  return !(lhs == rhs);
}

KVS::~KVS() {
  // TODO(you)
}

KVS::Iterator::~Iterator() {
  // TODO(you)
}

}  // namespace cloudlab