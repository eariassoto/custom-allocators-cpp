// Copyright (c) 2020 Emmanuel Arias
#include <catch.hpp>
#include "StackAllocator.h"

using Marker = StackAllocator::Marker;

TEST_CASE("Stack can allocate memory", "stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    StackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    SECTION("allocating zero bytes should do nothing") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 0);

        void* address = allocator.Allocate(0);
        marker = allocator.GetMarker();

        REQUIRE(address == nullptr);
        REQUIRE(marker == 0);
    }

    SECTION("allocating less than the allocator's size is a valid operation") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 0);

        void* address = allocator.Allocate(16);
        marker = allocator.GetMarker();

        REQUIRE(address != nullptr);
        REQUIRE(marker == 16);
    }

    SECTION(
        "allocating as much memory as the allocator's size is a valid "
        "operation") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 0);

        void* address = allocator.Allocate(64);
        marker = allocator.GetMarker();

        REQUIRE(address != nullptr);
        REQUIRE(marker == 64);
    }

    SECTION(
        "two continous allocations smaller than the allocator's size are valid "
        "operations") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 0);

        void* address = allocator.Allocate(16);
        marker = allocator.GetMarker();

        REQUIRE(address != nullptr);
        REQUIRE(marker == 16);

		address = allocator.Allocate(32);
        marker = allocator.GetMarker();

        REQUIRE(address != nullptr);
        REQUIRE(marker == 48);
    }
}

TEST_CASE("Full stack cannot allocate memory", "stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    StackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};
    allocator.Allocate(64);
    REQUIRE(allocator.GetMarker() == 64);

    SECTION("allocating zero bytes should do nothing") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 64);

        void* address = allocator.Allocate(0);
        marker = allocator.GetMarker();

        REQUIRE(address == nullptr);
        REQUIRE(marker == 64);
    }

    SECTION("trying to allocate memory gives nothing") {
        Marker marker = allocator.GetMarker();
        REQUIRE(marker == 64);

        void* address = allocator.Allocate(16);
        marker = allocator.GetMarker();

        REQUIRE(address == nullptr);
        REQUIRE(marker == 64);
    }
}

TEST_CASE("Stack can free to a valid marker", "stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    StackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    allocator.Allocate(16);
    REQUIRE(allocator.GetMarker() == 16);

    allocator.Allocate(32);
    REQUIRE(allocator.GetMarker() == 48);

    SECTION("freeing to the same marker does nothing") {
        Marker currentMarker = allocator.GetMarker();
        allocator.FreeToMarker(currentMarker);
        REQUIRE(allocator.GetMarker() == currentMarker);
    }

    SECTION("freeing to a valid marker is a valid operation") {
        allocator.FreeToMarker(16);
        REQUIRE(allocator.GetMarker() == 16);
    }

    SECTION("freeing to a not valid marker does nothing") {
        allocator.FreeToMarker(128);
        REQUIRE(allocator.GetMarker() == 48);
    }
}

TEST_CASE("Stack can clear memory", "stack allocator") {
    const size_t ALLOCATOR_SIZE_IN_BYTES = 64;
    StackAllocator allocator{ALLOCATOR_SIZE_IN_BYTES};

    allocator.Allocate(16);
    REQUIRE(allocator.GetMarker() == 16);

    allocator.Allocate(32);
    REQUIRE(allocator.GetMarker() == 48);

    allocator.Clear();
    REQUIRE(allocator.GetMarker() == 0);
}
