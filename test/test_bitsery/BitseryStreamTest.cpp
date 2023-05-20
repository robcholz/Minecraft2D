//
// Created by robcholz on 5/13/23.
//

#include <gtest/gtest.h>
#include <fstream>
#include "bitsery/traits/vector.h"
#include <bitsery/adapter/stream.h>
#include "bitsery/bitsery.h"

enum class MyEnum : uint16_t { V1, V2, V3 };
struct MyStruct {
	uint32_t i;
	MyEnum e;
	std::vector<float> fs;
};

template<typename S>
void serialize(S &s, MyStruct &o) {
	s.value4b(o.i);
	s.value2b(o.e);
	s.container4b(o.fs, 10);
}

using Buffer = std::vector<uint8_t>;

TEST(BitseryStreamTest, BasicAssertion) {
	MyStruct data{8941, MyEnum::V2, {15.0f, -8.5f, 0.045f}};
	MyStruct res{};

	// open file stream for writing and reading
	auto fileName = "../test/test_bitsery/test.mca";
	std::fstream s{ fileName, s.binary | s.trunc | s.out };
	if (!s.is_open()) {
		std::cout << "cannot open " << fileName << " for writing\n";
	}

	// we cannot use quick serialization function, because streams cannot use
	// writtenBytesCount method
	bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> ser{ s };
	ser.object(data);
	// flush to writer
	ser.adapter().flush();
	s.close();
	// reopen for reading

	s.open(fileName, s.binary | s.in);
	if (!s.is_open()) {
		std::cout << "cannot open " << fileName << " for reading\n";
	}

	// same as serialization, but returns deserialization state as a pair
	// first = error code, second = is buffer was successfully read from begin to
	// the end.
	auto state =bitsery::quickDeserialization<bitsery::InputStreamAdapter>(s, res);

	ASSERT_TRUE(state.first == bitsery::ReaderError::NoError && state.second);
	ASSERT_TRUE(data.i == res.i && data.e == res.e && data.fs == res.fs);
}