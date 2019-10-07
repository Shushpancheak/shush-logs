#include "shush-logs.hpp"
#include "gtest/gtest.h"

TEST(CREATE_SOME_LOGS, LOGS) {
  shush::logs::Logger logger("test");
  for (char i = 0; i < 100; ++i) {
    logger.Log("HEY MAN" + static_cast<char>('0' + i));
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}