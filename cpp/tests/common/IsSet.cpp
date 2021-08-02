#include <gtest/gtest.h>

#include "ptk/common/is-set.h"

using namespace ptk;


TEST(is_set_class, emptyInitalisation){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());
}

TEST(is_set_class, valueInitalisation){
	is_set<double> value(4.0);
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 4.0);
}


TEST(is_set_class, assignmentOperator){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());

	value = 1.2;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 1.2);
}

TEST(is_set_class, copyConstructorNotSet){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());

	is_set<double> value1;
	EXPECT_FALSE(value1.isSet());

	value = value1;
	EXPECT_FALSE(value.isSet());
}

TEST(is_set_class, copyConstructorSet){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());

	is_set<double> value1(7.3);
	EXPECT_TRUE(value1.isSet());

	value = value1;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 7.3);
}

TEST(is_set_class, assignmentOperatorCannotOverwrite){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());

	value = 1.2;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 1.2);

	value = 1.4;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 1.2);
}


TEST(is_set_class, copyConstructorCannotOverwrite){
	is_set<double> value;
	EXPECT_FALSE(value.isSet());

	is_set<double> value1(7.3);
	EXPECT_TRUE(value1.isSet());

	value = value1;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 7.3);

	is_set<double> value2(10.3);
	EXPECT_TRUE(value2.isSet());

	value = value2;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 7.3);
}



TEST(is_set_class, assignmentOperatorOverwrite){
	is_set<double> value(true);
	EXPECT_FALSE(value.isSet());

	value = 1.2;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 1.2);

	value = 1.4;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 1.4);
}


TEST(is_set_class, copyConstructorOverwrite){
	is_set<double> value(true);
	EXPECT_FALSE(value.isSet());

	is_set<double> value1(7.3);
	EXPECT_TRUE(value1.isSet());

	value = value1;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 7.3);

	is_set<double> value2(10.3);
	EXPECT_TRUE(value2.isSet());

	value = value2;
	EXPECT_TRUE(value.isSet());
	EXPECT_EQ(value.data(), 10.3);
}

TEST(is_set_class, basicAddT){
	is_set<int> value(4);
	int x = 5;
	is_set<int> result = value + x;


	EXPECT_EQ(value.data(), 4);
	EXPECT_EQ(result.data(), 9);

	int y = 3;
	is_set<int> result1 = y + value;
	EXPECT_EQ(result1.data(), 7);

}

TEST(is_set_class, basicAddIsSet){
	is_set<int> value(4);
	is_set<int> value1(7);
	is_set<int> result = value + value1;

	EXPECT_EQ(value.data(), 4);
	EXPECT_EQ(value1.data(), 7);
	EXPECT_EQ(result.data(), 11);

	is_set<int> value2(9);
	is_set<int> result1 = value2 + value;
	EXPECT_EQ(result1.data(), 13);
	EXPECT_EQ(value2.data(), 9);
}
