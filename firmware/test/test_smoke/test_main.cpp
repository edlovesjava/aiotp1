// Smoke test for the native Unity environment. Proves the host test
// toolchain works; real module tests live in test/test_<module>/.

#include <unity.h>

namespace {

int add(int a, int b) { return a + b; }

}  // namespace

void setUp() {}
void tearDown() {}

void test_add_positive() { TEST_ASSERT_EQUAL_INT(5, add(2, 3)); }

void test_add_negative() { TEST_ASSERT_EQUAL_INT(-1, add(2, -3)); }

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_add_positive);
    RUN_TEST(test_add_negative);
    return UNITY_END();
}
