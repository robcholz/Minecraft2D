//
// Created by robcholz on 8/20/23.
//
#include "gtest/gtest.h"
//#include <mempool/ObjectPool.hpp>


TEST(MemPoolTest, BasicAssertion) {
	/*
	class TestBlock {
	private:
		std::string str;
		std::vector<int> vec;
	public:
		TestBlock(std::string str, std::vector<int> vec) : str(std::move(str)), vec(std::move(vec)) {}
		std::string getStr() { return str; }
		std::vector<int>& getVec() { return vec; }
	};
	FixedObjectPool<TestBlock> test_pool(200);
	EXPECT_TRUE(false) << test_pool.calc_stats().num_allocations;
	EXPECT_TRUE(false) << test_pool.calc_stats().num_blocks;
	auto manual_obj = test_pool.new_object("test", std::vector<int>{1, 2, 3});
	test_pool.delete_object(manual_obj);
	EXPECT_TRUE(false) << test_pool.calc_stats().num_allocations;
	EXPECT_TRUE(false) << test_pool.calc_stats().num_blocks;
	test_pool.delete_all();
	EXPECT_TRUE(false) << test_pool.calc_stats().num_allocations;
	EXPECT_TRUE(false) << test_pool.calc_stats().num_blocks;
	 */
}