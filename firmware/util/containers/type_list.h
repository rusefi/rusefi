#pragma once

#include <type_traits>

/*
 * Indicates that a member of type_list should be able to be replaced in a unit test.
 */
template<typename base_t>
struct Mockable;

template<typename... Ts>
struct type_list;

/*
 * Instantiates each type, allowing you to fetch by type.
 *
 * This is basically std::tuple, std::get, and std::apply, but with the API oriented more toward
 * our needs.
 *
 * The primary use of this is to provide a list of engine modules in the giant Engine structure.
 *
 * The main feature is that objects are mockable.  If you pass in Mockable<T> instead of T, and
 * typedef T::interface_t, then you can call set to change what get returns.
 *
 * For a normal type T,
 * T & get<T>();
 * T & unmock<T>();
 *
 * For Mockable<T>,
 * T::interface_t & get<T>();
 * T & unmock<T>();
 */
template<typename base_t, typename... tail_t>
struct type_list<base_t, tail_t...> {
    type_list<base_t> first;
    type_list<tail_t...> others;

#if EFI_UNIT_TEST
    virtual ~type_list() = default;
#endif

    static_assert(!decltype(others)::template has<base_t>(), "Each type can only be listed once.");

    template<typename count_t>
    static consteval size_t count() {
        return decltype(first)::template count<count_t>() +
               decltype(others)::template count<count_t>();
    }

    static consteval size_t count() {
        return 1 + decltype(others)::count();
    }

    /*
     * Returns whether has_t exists in the type list
     *
     * has_t should not be Mockable, it should be the actual type.
     */
    template<typename has_t>
    static consteval bool has() {
        return decltype(first)::template has<has_t>() ||
               decltype(others)::template has<has_t>();
    }

    /*
     * Call the given function on the unmocked version of each type.
     *
     * It is probably best (and maybe only possible?) to call this with a generic lambda, as:
     * tl.apply_all([](auto & m) { m.WhateverFuncIWant(); });
     */
    template<typename func_t>
    void constexpr apply_all(func_t const & f) {
        first.apply_all(f);
        others.apply_all(f);
    }

    template<typename func_t>
    void constexpr apply_all(func_t const & f) const {
        first.apply_all(f);
        others.apply_all(f);
    }

    // Applies an accumulator function over the sequence of elements.
    // The specified seed value is used as the initial accumulator value,
    // and the specified function is used to select the result value.
    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) {
        return others.aggregate(accumulator, first.aggregate(accumulator, seed));
    }

    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) const {
        return others.aggregate(accumulator, first.aggregate(accumulator, seed));
    }

    /*
     * Return the container object for type get_t.
     *
     * get_t should not be Mockable, it should be the actual type.
     * The return object will have the methods unmock(), operator->, operator*, and if
     * Mockable, set().
     *
     * The return type is type_list<get_t> or type_list<Mockable<get_t>>
     */
    template<typename get_t>
    constexpr decltype(auto) get() {
        static_assert(has<get_t>(), "Type not found in type_list");
        if constexpr (decltype(first)::template has<get_t>()) {
            return first.template get<get_t>();
        } else {
            return others.template get<get_t>();
        }
    }

    template<typename get_t>
    constexpr decltype(auto) get() const {
        static_assert(has<get_t>(), "Type not found in type_list");
        if constexpr (decltype(first)::template has<get_t>()) {
            return first.template get<get_t>();
        } else {
            return others.template get<get_t>();
        }
    }
};

/*
 * Specialization of type_list for a single base_t (that is not Mockable<>).
 */
template<typename base_t>
struct type_list<base_t> {
private:
    base_t me;

public:
    template<typename count_t>
    static consteval size_t count() {
        return std::is_same_v<base_t, count_t> ? 1 : 0;
    }

    static consteval size_t count() {
        return 1;
    }

    template<typename func_t>
    void constexpr apply_all(func_t const & f) {
        f(me);
    }

    template<typename func_t>
    void constexpr apply_all(func_t const & f) const {
        f(me);
    }

    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) {
        return accumulator(me, seed);
    }

    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) const {
        return accumulator(me, seed);
    }

    template<typename has_t>
    static consteval bool has() {
        return std::is_same_v<has_t, base_t>;
    }

    template<typename get_t, typename = std::enable_if_t<has<get_t>()>>
    constexpr auto& get() {
        return *this;
    }

    template<typename get_t, typename = std::enable_if_t<has<get_t>()>>
    constexpr auto const& get() const {
        return *this;
    }

    constexpr auto& unmock() {
        return me;
    }

    constexpr auto const& unmock() const {
        return me;
    }

    constexpr auto operator->() {
        return &me;
    }

    constexpr auto operator->() const {
        return &me;
    }

    constexpr auto& operator*() {
        return me;
    }

    constexpr auto const& operator*() const {
        return me;
    }
};

#if EFI_PROD_CODE

/*
 * Production specialization of type_list for a single Mockable<base_t>.
 *
 * Performs exactly as base_t.
 */
template<typename base_t>
struct type_list<Mockable<base_t>> : type_list<base_t> {
};

#else // if not EFI_PROD_CODE:

#include <memory>

/*
 * Unit test/simulator specialization of type_list for a single Mockable<base_t>.
 */
template<typename base_t>
struct type_list<Mockable<base_t>> {
private:
    // Dynamically allocate so that ASAN can detect overflows for us
    std::unique_ptr<base_t> me = std::make_unique<base_t>();
    using interface_t = typename base_t::interface_t;
    interface_t* cur = me.get();

public:
    template<typename count_t>
    static consteval size_t count() {
        return std::is_same_v<base_t, count_t> ? 1 : 0;
    }

    static consteval size_t count() {
        return 1;
    }

    template<typename func_t>
    void constexpr apply_all(func_t const & f) {
        f(*me);
    }

    template<typename func_t>
    void constexpr apply_all(func_t const & f) const {
        f(*me);
    }

    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) {
        return accumulator(*me, seed);
    }

    template<typename return_t, typename func_t>
    decltype(auto) aggregate(func_t const& accumulator, return_t seed) const {
        return accumulator(*me, seed);
    }

    template<typename has_t>
    static consteval bool has() {
        return std::is_same_v<has_t, base_t>;
    }

    template<typename get_t, typename = std::enable_if_t<has<get_t>()>>
    constexpr auto& get() {
        return *this;
    }

    template<typename get_t, typename = std::enable_if_t<has<get_t>()>>
    constexpr auto const& get() const {
        return *this;
    }

    auto& unmock() {
        return *me;
    }

    auto const& unmock() const {
        return *me;
    }

    void constexpr set(interface_t* ptr) {
        if (ptr) {
            cur = ptr;
        } else {
            cur = me.get();
        }
    }

    constexpr interface_t* operator->() {
        return cur;
    }

    constexpr interface_t const* operator->() const {
        return cur;
    }

    constexpr interface_t& operator*() {
        return *cur;
    }

    constexpr interface_t const& operator*() const {
        return *cur;
    }
};

#endif // EFI_UNIT_TEST
