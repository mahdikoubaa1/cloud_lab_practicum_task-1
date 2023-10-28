#include "cloudlab/kvs.hh"
#include "cloudlab/flags.hh"

#include "fmt/core.h"
#include "rocksdb/db.h"

#include <algorithm>
#include <random>

namespace cloudlab {

auto KVS::open() -> bool {
  // only open db if it was not opened yet
  mtx.lock();
  if (!db) {
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, path.string(), &db);
    mtx.unlock();
    return status.ok();
  }
  mtx.unlock();
  return true;
}

auto KVS::get(const std::string& key, std::string& result) -> bool {

  rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, &result);
  return s.ok();
}

/*
 * [NOTE] - If a PUT request is issued and a DB does not yet exist
 * the server should open a new DB and insert the key-value pair
 */
auto KVS::put(const std::string& key, const std::string& value) -> bool {
  open();
  rocksdb::Status status = db->Put(rocksdb::WriteOptions(), key, value);
  return status.ok();
}

auto KVS::remove(const std::string& key) -> bool {
  rocksdb::Status s = db->Delete(rocksdb::WriteOptions(), key);
  return s.ok();
}
/***
 * Completly removes the DB
 */
auto KVS::clear() -> bool {
  auto* it = db->NewIterator(rocksdb::ReadOptions());
  rocksdb::Status s;
  bool h{true};
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    s = db->Delete(rocksdb::WriteOptions(), it->key());
    if (!s.ok()) {
      h = false;
      break;
    }
  }
  if (!it->status().ok()) h = false;

  delete it;
  return h;
}

auto KVS::begin() -> KVS::Iterator {

  auto* it = db->NewIterator(rocksdb::ReadOptions());
  std::deque<rocksdb::Iterator*> h;
  h.push_back(it);
  KVS::Iterator iterator{h};
  return iterator;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto KVS::end() const -> KVS::Sentinel {
  return {};
}

auto KVS::Iterator::operator*()
    -> std::pair<std::string_view, std::string_view> {
  for (auto its = this->iterators.begin(); its != this->iterators.end();) {
    if (*its == nullptr || !(*its)->Valid()) {
      delete *its;
      its = this->iterators.erase(its);
    } else {
      return {(*its)->key().ToStringView(), (*its)->value().ToStringView()};
    }
  }
  return {};
}

auto KVS::Iterator::operator++() -> KVS::Iterator& {
  bool i{};
  for (auto its = this->iterators.begin(); its != this->iterators.end();) {
    if (*its == nullptr || !(*its)->Valid()) {
      i = true;
      delete *its;
      its = this->iterators.erase(its);
    } else {
      if (!i) {
        (*its)->Next();
        continue;
      }
      break;
    }
  }
  return *this;
}

auto operator==(const KVS::Iterator& it, const KVS::Sentinel&) -> bool {
  if (it.iterators.empty()) return true;
  for (auto* its : it.iterators) {
    if (its != nullptr && its->Valid()) return true;
  }
  return false;
}

auto operator!=(const KVS::Iterator& lhs, const KVS::Sentinel& rhs) -> bool {
  return !(lhs == rhs);
}

KVS::~KVS() {
  if (db != nullptr) db->Close();
  db= nullptr;
}

KVS::Iterator::~Iterator() {
  for (auto its = this->iterators.begin(); its != this->iterators.end();) {
    delete *its;
    its = this->iterators.erase(its);
  }
}

}  // namespace cloudlab