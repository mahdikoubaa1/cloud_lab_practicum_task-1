#ifndef CLOUDLAB_KVS_HH
#define CLOUDLAB_KVS_HH

#include <deque>
#include <filesystem>
#include <set>
#include <shared_mutex>
#include <vector>

namespace rocksdb {
class DB;
class Iterator;
}  // namespace rocksdb

namespace cloudlab {

/**
 * The key-value store. We use rocksdb for the actual key-value operations.
 */
class KVS {
 public:
  struct Sentinel {};

  struct Iterator {
    std::deque<rocksdb::Iterator*> iterators;

    ~Iterator();

    friend auto operator==(const Iterator& it, const Sentinel&) -> bool;
    friend auto operator!=(const Iterator& lhs, const Sentinel& rhs) -> bool;

    auto operator*() -> std::pair<std::string_view, std::string_view>;
    auto operator++() -> Iterator&;
  };

  /***
   * open - [boolean] Indicates if the KVS should be newly opened or the method
   * should only return the path.
   */
  explicit KVS(const std::string& path = {}, bool open = false) : path{path} {
    // TODO(you)
  }

  ~KVS();

  // delete copy constructor and copy assignment
  KVS(const KVS&) = delete;
  auto operator=(const KVS&) -> KVS& = delete;

  [[nodiscard]] auto begin() -> Iterator;

  [[nodiscard]] auto end() const -> Sentinel;

  auto open() -> bool;

  auto get(const std::string& key, std::string& result) -> bool;

  auto put(const std::string& key, const std::string& value) -> bool;

  auto remove(const std::string& key) -> bool;

  auto clear() -> bool;

 private:
  std::filesystem::path path;
  rocksdb::DB* db{};
  std::mutex mtx;
};

}  // namespace cloudlab

#endif  // CLOUDLAB_KVS_HH
