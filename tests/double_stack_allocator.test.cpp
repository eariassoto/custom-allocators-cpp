// Copyright (c) 2020 Emmanuel Arias
#include <catch.hpp>

#include "DoubleStackAllocator.h"

using Marker = DoubleStackAllocator::Marker;

TEST_CASE("Lower stack can allocate memory, upper stack not used",
          "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    SECTION("allocating zero bytes should do nothing") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateLower(0);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address == nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);
    }

    SECTION("allocating less than the allocator's size is a valid operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateLower(16);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 16);
        REQUIRE(upperMarker == 64);
    }

    SECTION(
        "allocating as much memory as the allocator's size is a valid "
        "operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateLower(64);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 64);
        REQUIRE(upperMarker == 64);
    }

    SECTION(
        "two continous allocations smaller than the allocator's size are valid "
        "operations") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateLower(16);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 16);
        REQUIRE(upperMarker == 64);

        address = allocator.AllocateLower(32);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 48);
        REQUIRE(upperMarker == 64);
    }
}

TEST_CASE("Upper stack can allocate memory", "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    SECTION("allocating zero bytes should do nothing") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateUpper(0);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address == nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);
    }

    SECTION("allocating less than the allocator's size is a valid operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateUpper(16);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 48);
    }

    SECTION(
        "allocating as much memory as the allocator's size is a valid "
        "operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateUpper(64);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 0);
    }

    SECTION(
        "two continous allocations smaller than the allocator's size are valid "
        "operations") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* address = allocator.AllocateUpper(16);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 48);

        address = allocator.AllocateUpper(32);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(address != nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 16);
    }
}

TEST_CASE("Both stacks can allocate memory", "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    SECTION("allocating zero bytes should do nothing") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* lowerAddress = allocator.AllocateLower(0);
        void* upperAddress = allocator.AllocateUpper(0);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(lowerAddress == nullptr);
        REQUIRE(upperAddress == nullptr);
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);
    }

    SECTION("allocating less than the allocator's size is a valid operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* lowerAddress = allocator.AllocateLower(16);
        void* upperAddress = allocator.AllocateUpper(32);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(lowerAddress != nullptr);
        REQUIRE(upperAddress != nullptr);
        REQUIRE(lowerMarker == 16);
        REQUIRE(upperMarker == 32);
    }

    SECTION(
        "allocating as much memory as the allocator's size is a valid "
        "operation") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* lowerAddress = allocator.AllocateLower(32);
        void* upperAddress = allocator.AllocateUpper(32);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(lowerAddress != nullptr);
        REQUIRE(upperAddress != nullptr);
        REQUIRE(lowerMarker == 32);
        REQUIRE(upperMarker == 32);
    }

    SECTION(
        "two continous allocations smaller than the allocator's size are valid "
        "operations") {
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();
        REQUIRE(lowerMarker == 0);
        REQUIRE(upperMarker == 64);

        void* lowerAddress = allocator.AllocateLower(8);
        void* upperAddress = allocator.AllocateUpper(8);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(lowerAddress != nullptr);
        REQUIRE(upperAddress != nullptr);
        REQUIRE(lowerMarker == 8);
        REQUIRE(upperMarker == 56);

        lowerAddress = allocator.AllocateLower(16);
        upperAddress = allocator.AllocateUpper(16);
        lowerMarker = allocator.GetLowerMarker();
        upperMarker = allocator.GetUpperMarker();

        REQUIRE(lowerAddress != nullptr);
        REQUIRE(upperAddress != nullptr);
        REQUIRE(lowerMarker == 24);
        REQUIRE(upperMarker == 40);
    }
}


TEST_CASE("stacks cannot allocate memory crossing the other stack's marker", "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};
    allocator.AllocateLower(20);
    allocator.AllocateUpper(20);
    REQUIRE(allocator.GetLowerMarker() == 20);
    REQUIRE(allocator.GetUpperMarker() == 44);

    SECTION("trying to allocate memory in the lower stack gives nothing") {
        void* address = allocator.AllocateLower(32);
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();

        REQUIRE(address == nullptr);
        REQUIRE(lowerMarker == 20);
        REQUIRE(upperMarker == 44);
    }

    SECTION("trying to allocate memory in the upper stack gives nothing") {
        void* address = allocator.AllocateUpper(32);
        Marker lowerMarker = allocator.GetLowerMarker();
        Marker upperMarker = allocator.GetUpperMarker();

        REQUIRE(address == nullptr);
        REQUIRE(lowerMarker == 20);
        REQUIRE(upperMarker == 44);
    }
}

TEST_CASE("Double stack can free to a valid marker", "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    allocator.AllocateLower(16);
    allocator.AllocateUpper(8);
    REQUIRE(allocator.GetLowerMarker() == 16);
    REQUIRE(allocator.GetUpperMarker() == 56);

    allocator.AllocateLower(16);
    allocator.AllocateUpper(16);
    REQUIRE(allocator.GetLowerMarker() == 32);
    REQUIRE(allocator.GetUpperMarker() == 40);

    SECTION("freeing to the same lower stack marker does nothing") {
        Marker currentMarker = allocator.GetLowerMarker();
        allocator.FreeToLowerMarker(currentMarker);
        REQUIRE(allocator.GetLowerMarker() == currentMarker);
    }

    SECTION("freeing to the same upper stack marker does nothing") {
        Marker currentMarker = allocator.GetUpperMarker();
        allocator.FreeToUpperMarker(currentMarker);
        REQUIRE(allocator.GetUpperMarker() == currentMarker);
    }

    SECTION("freeing to a valid lower stack marker is a valid operation") {
        allocator.FreeToLowerMarker(16);
        REQUIRE(allocator.GetLowerMarker() == 16);
    }

    SECTION("freeing to a valid upper stack marker is a valid operation") {
        allocator.FreeToUpperMarker(56);
        REQUIRE(allocator.GetUpperMarker() == 56);
    }

    SECTION("freeing to a not valid lower stack marker does nothing") {
        allocator.FreeToLowerMarker(128);
        REQUIRE(allocator.GetLowerMarker() == 32);
    }

    SECTION("freeing to a not valid upper stack marker does nothing") {
        allocator.FreeToLowerMarker(128);
        REQUIRE(allocator.GetUpperMarker() == 40);
    }
}

TEST_CASE("Double stack can clear memory", "double stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    DoubleStackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    allocator.AllocateLower(16);
    allocator.AllocateUpper(16);
    REQUIRE(allocator.GetLowerMarker() == 16);
    REQUIRE(allocator.GetUpperMarker() == 48);

    SECTION("Clear lower stack stack works properly") {
        allocator.ClearLower();
        REQUIRE(allocator.GetLowerMarker() == 0);
        REQUIRE(allocator.GetUpperMarker() == 48);
    }

    SECTION("Clear upper stack stack works properly") {
        allocator.ClearUpper();
        REQUIRE(allocator.GetLowerMarker() == 16);
        REQUIRE(allocator.GetUpperMarker() == 64);
    }

    SECTION("ClearAll works properly") {
        allocator.ClearAll();
        REQUIRE(allocator.GetLowerMarker() == 0);
        REQUIRE(allocator.GetUpperMarker() == 64);
    }
    
}
