#include <gtest/gtest.h>
#include "Singleton.h"
#include <QString>

#define private public
#include "Config.h"

TEST(ConfigTest, test_1) {
   Config* config = common::Singleton<Config>::instancePointer();
   assert(config);

   QString configFile("./config/config.ini");
   EXPECT_TRUE(config->init(configFile));
}
