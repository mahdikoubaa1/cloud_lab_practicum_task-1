#include "cloudlab/kvs.hh"

#include "cloud.pb.h"
#include "cloudlab/network/connection.hh"
#include <sys/socket.h>

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <thread>
#include <fcntl.h>

const auto create_random_string = [](int fd, size_t size) {
  auto buf = std::make_unique<uint8_t[]>(size);
  read(fd, buf.get(), size);
  return fmt::format("{}-{}", fd,
                     std::string{reinterpret_cast<char*>(buf.get()), size});
};

class Task1 : public ::testing::Test {
 public:
  void TearDown() override {
    kvs.clear();
  }

 protected:
  cloudlab::KVS& db() {
    return kvs;
  }

 private:
  cloudlab::KVS kvs{};
};

TEST_F(Task1, Correctness) {
  std::string out;
  auto fd = open("/dev/random", O_RDONLY | O_CLOEXEC);

  auto& kvs = db();

  for (auto i = 1; i < 1000; i++) {
    std::string key = create_random_string(fd, i);
    std::string val = create_random_string(fd, 1000 - i);

    kvs.get(key, out);
    EXPECT_NE(val, out);

    kvs.put(key, val);

    kvs.get(key, out);
    EXPECT_EQ(val, out);

    kvs.remove(key);
  }

  close(fd);
}

TEST_F(Task1, Multithreaded) {
  std::vector<std::thread> threads;

  auto& kvs = db();

  auto thread_job = [&kvs]() -> void {
    std::string out;
    auto fd = open("/dev/random", O_RDONLY | O_CLOEXEC);

    for (auto i = 1; i < 1000; i++) {
      std::string key = create_random_string(fd, i);
      std::string val = create_random_string(fd, 1000 - i);

      kvs.get(key, out);
      EXPECT_NE(val, out);

      kvs.put(key, val);

      kvs.get(key, out);
      EXPECT_EQ(val, out);

      kvs.remove(key);
    }

    close(fd);
  };

  threads.reserve(16);

  for (auto i = 0; i < 16; i++) {
    threads.emplace_back(std::thread(thread_job));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}

TEST_F(Task1, ConnectionTest) {
  cloud::CloudMessage send_msg{}, receive_msg{};
  int tmp[2];

  socketpair(PF_LOCAL, SOCK_STREAM, 0, tmp);

  auto con_1 = cloudlab::Connection{tmp[0]};
  auto con_2 = cloudlab::Connection{tmp[1]};

  /* PUT msg test */
  send_msg.set_operation(cloud::CloudMessage_Operation_PUT);
  auto* kvp_put = send_msg.add_kvp();
  kvp_put->set_key("123");

  con_1.send(send_msg);

  EXPECT_EQ(receive_msg.kvp_size(), 0);

  con_2.receive(receive_msg);

  EXPECT_EQ(receive_msg.operation(), cloud::CloudMessage_Operation_PUT);
  EXPECT_EQ(receive_msg.kvp(0).key(), "123");

  /* GET msg test */
  send_msg.set_operation(cloud::CloudMessage_Operation_GET);
  auto* kvp_get = send_msg.add_kvp();
  kvp_get->set_key("456");

  con_1.send(send_msg);

  EXPECT_EQ(receive_msg.kvp_size(), 1);

  con_2.receive(receive_msg);

  EXPECT_EQ(receive_msg.operation(), cloud::CloudMessage_Operation_GET);
  EXPECT_EQ(receive_msg.kvp(1).key(), "456");

  /* DELETE msg test */
  send_msg.set_operation(cloud::CloudMessage_Operation_DELETE);
  auto* kvp_delete = send_msg.add_kvp();
  kvp_delete->set_key("789");

  con_1.send(send_msg);

  EXPECT_EQ(receive_msg.kvp_size(), 2);

  con_2.receive(receive_msg);

  EXPECT_EQ(receive_msg.operation(), cloud::CloudMessage_Operation_DELETE);
  EXPECT_EQ(receive_msg.kvp(2).key(), "789");
}