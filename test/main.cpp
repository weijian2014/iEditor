#include <gtest/gtest.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
   (void)argc;
   (void)argv;

   testing::InitGoogleTest(&argc, argv);
   //  testing::FLAGS_gtest_filter = "*.*";

   cout << "Run unit test" << endl;
   return RUN_ALL_TESTS();
}
