#include "pch.h"

#include "func_chain.h"

struct AddOne final : public SensorConverter {
    SensorResult convert(float input) const {
        return input + 1;
    }
};

struct SubOne final : public SensorConverter {
    SensorResult convert(float input) const {
        return input - 1;
    }
};

struct Doubler final : public SensorConverter {
    SensorResult convert(float input) const {
        return input * 2;
    }
};

TEST(FunctionChain, TestSingle)
{
    FuncChain<AddOne> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 6);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 11);
    }
}

TEST(FunctionChain, TestDouble)
{
    // This computes fc(x) = (x + 1) * 2
    FuncChain<AddOne, Doubler> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 12);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 22);
    }
}

TEST(FunctionChain, TestTriple)
{
    // This computes fc(x) = ((x + 1) * 2) - 1
    FuncChain<AddOne, Doubler, SubOne> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 11);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 21);
    }
}

TEST(FunctionChain, TestGet)
{
    // No logic here - the test is that it compiles
    FuncChain<AddOne, Doubler, SubOne> fc;

    fc.get<AddOne>();
    fc.get<Doubler>();
    fc.get<SubOne>();
}
