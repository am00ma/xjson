#include "test.h"

int main(int argc, char* argv[])
{
    TEST_SUITE("consume");

    TEST_CASE("primitives") {}
    TEST_CASE("array") {}
    TEST_CASE("object") {}
    TEST_CASE("nested") {}

    TEST_RESULTS();
}
