#pragma once

// This macro defines a type that can detect whether a type has a particular
// member.
#define allow_override_for(member_name) \
	template <typename T>   \
	class has_member_helper_##member_name {										\
		typedef char yes_type;													\
		typedef long no_type;													\
		template <typename U> static yes_type test(decltype(&U::member_name));	\
		template <typename U> static no_type  test(...);						\
	public:																		\
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);	\
	};																			\
	template<typename T>														\
	static constexpr bool has_member_##member_name() { return has_member_helper_##member_name<T>::value;}

// This macro uses the above definitions to check whether a particular 
#define CONFIG_HAS_OVERRIDE(member_name) efi::overrides::has_member_##member_name<ConfigOverrides>()
