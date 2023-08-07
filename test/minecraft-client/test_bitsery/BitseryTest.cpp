//
// Created by robcholz on 5/13/23.
//

#include <gtest/gtest.h>
#include "bitsery/traits/vector.h"
#include "bitsery/adapter/buffer.h"
#include "bitsery/bitsery.h"

enum class MyEnum:uint16_t { V1,V2,V3 };
struct MyStruct {
	uint32_t i;
	MyEnum e;
	std::vector<float> fs;
};

template <typename S>
void serialize(S& s, MyStruct& o) {
	s.value4b(o.i);
	s.value2b(o.e);
	s.container4b(o.fs, 10);
}

using Buffer = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

TEST(BitseryTest,BasicAssertion){
	MyStruct data{8941, MyEnum::V2, {15.0f, -8.5f, 0.045f}};
	MyStruct res{};

	Buffer buffer;

	auto writtenSize = bitsery::quickSerialization<OutputAdapter>(buffer, data);
	auto state = bitsery::quickDeserialization<InputAdapter>({buffer.begin(), writtenSize}, res);
	ASSERT_TRUE(state.first == bitsery::ReaderError::NoError && state.second);
	ASSERT_TRUE(data.fs == res.fs && data.i == res.i && data.e == res.e);
}