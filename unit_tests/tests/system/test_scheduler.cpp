#include "pch.h"
#include "scheduler.h"

namespace {
	struct A{int a;};
	struct B{int b;};

	void foo(uintptr_t p){ }
	void bar(char c){ }

	void a_func(A* a){ a-> a = 64;}
	void b_func(B* b){ b-> b = 32;}
}

TEST(scheduler, action_s) {

	A a{};
	B b{};

	uintptr_t uptr{};
	char c{};

	auto const foo_act{ action_s::make<foo>(uptr) };
	auto const bar_act{ action_s::make<bar>(c) };

	foo_act.execute();
	bar_act.execute();

	auto const a_act{ action_s::make<a_func>(&a) };
	auto const b_act{ action_s::make<b_func>(&b) };

	a_act.execute();
	b_act.execute();

	ASSERT_EQ(a.a, 64);
	ASSERT_EQ(b.b, 32);
}
