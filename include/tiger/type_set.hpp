#pragma once

#include <functional>
#include <type_traits>

namespace tgr {

template <typename T>
struct tset_node {
  using type = T;
};

template <typename... Ts>
struct tset : public tset_node<Ts>... {};

template <typename T, typename... Ts>
constexpr bool tset_contains_impl =
    std::is_base_of_v<tset_node<T>, tset<Ts...>>;

template <typename... Ts>
constexpr auto make_tset() -> tset<Ts...> {
  return tset<Ts...>();
}

template <typename T, typename... Ts>
constexpr auto tset_contains(const tset<Ts...>& set) -> bool {
  return tset_contains_impl<T, Ts...>;
}

template <typename... Ts, typename... Us>
constexpr auto tset_cat(const tset<Ts...>& s1,
                        const tset<Us...>& s2) -> tset<Ts..., Us...> {
  return tset<Ts..., Us...>();
}

template <typename S1, typename S2, typename... Sn>
constexpr auto tset_cat(const S1& s1, const S2& s2, const Sn&... sn) {
  return tset_cat(tset_cat(s1, s2), sn...);
}

template <typename... Ts, typename... Us>
constexpr auto tset_intersect(const tset<Ts...>& s1,
                              const tset<Us...>& s2) -> decltype(auto) {
  return tset_cat(
      std::conditional_t<tset_contains_impl<Ts, Us...>, tset<Ts>, tset<>>()...);
}

template <typename Func, typename... Ts>
constexpr auto for_each(const tset<Ts...>&, Func&& func) -> void {
  static_assert(
      (std::is_invocable_v<Func, tset_node<Ts>> && ...),
      "Function must be invocable with all type identities in the tset");
  (std::invoke(func, tset_node<Ts>{}), ...);
}

template <typename... Ts>
constexpr auto size(const tset<Ts...>& set) -> std::size_t {
  return sizeof...(Ts);
}

}  // namespace tgr
