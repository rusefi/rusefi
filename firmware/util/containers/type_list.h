#pragma once

template<typename base_t, typename... tail_t>
struct type_list {
	type_list<base_t> mine;
	type_list<tail_t...> others;

	static_assert(!decltype(others)::template has<base_t>(),
		      "Each type can only be listed once.");

	template<typename func_t>
	void apply_all(func_t const & f) {
		mine.apply_all(f);
		others.apply_all(f);
	}

	template<typename get_t>
	auto get() -> typename std::enable_if<std::is_same<get_t, base_t>::value, get_t &>::type {
		return mine.template get<get_t>();
	}

	template<typename get_t>
	auto get() -> typename std::enable_if<!std::is_same<get_t, base_t>::value, get_t &>::type {
		return others.template get<get_t>();
	}

	template<typename has_t>
	static constexpr bool has() {
		return decltype(mine)::template has<has_t>() ||
			decltype(others)::template has<has_t>();
	}
};

template<typename base_t>
struct type_list<base_t> {
	base_t me;

	template<typename func_t>
	void apply_all(func_t const & f) {
		f(me);
	}

	template<typename get_t>
	auto get() -> typename std::enable_if<std::is_same<get_t, base_t>::value, get_t &>::type {
		return me;
	}

	template<typename has_t>
	static constexpr bool has() {
		return std::is_same<has_t, base_t>::value;
	}
};
