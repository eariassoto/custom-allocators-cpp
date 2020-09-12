// Copyright (c) 2020 Emmanuel Arias
#include <catch.hpp>

#include "pool_allocator.h"

TEST_CASE("Pool allocator has the proper available blocks after creation",
          "pool allocator") {
    const size_t BLOCK_SIZE_IN_BYTES = 64;
    const size_t POOL_SIZE = 100;
    PoolAllocator allocator{BLOCK_SIZE_IN_BYTES, POOL_SIZE};

    REQUIRE(allocator.GetAvailableBlocks() == 100);
}

TEST_CASE("Pool allocator can allocate and free elements", "pool allocator") {
    struct Person {
        std::string name;
        unsigned int age;
    };
    const size_t BLOCK_SIZE_IN_BYTES = sizeof(Person);
    const size_t POOL_SIZE = 100;
    PoolAllocator allocator{BLOCK_SIZE_IN_BYTES, POOL_SIZE};

    REQUIRE(allocator.GetAvailableBlocks() == 100);

    Person* p1 = new (allocator.AllocateBlock()) Person();
    p1->age = 42;
    p1->name = "John Smith";

    REQUIRE(allocator.GetAvailableBlocks() == 99);

    void* ptr1 = allocator.AllocateBlock();
    void* ptr2 = allocator.AllocateBlock();

    REQUIRE(allocator.GetAvailableBlocks() == 97);
    REQUIRE(p1 != ptr1);
    REQUIRE(ptr1 != ptr2);

    REQUIRE(p1->age == 42);
    REQUIRE(p1->name == "John Smith");

    allocator.FreeBlock(ptr1);
    allocator.FreeBlock(ptr2);

    REQUIRE(allocator.GetAvailableBlocks() == 99);

    p1->~Person();
    allocator.FreeBlock(reinterpret_cast<void*>(p1));

    REQUIRE(allocator.GetAvailableBlocks() == 100);
}
