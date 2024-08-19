#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace tgr {

template <typename T>
struct tmap_node {
  using type = T;
  T value;

  template <typename U>
  constexpr tmap_node(U&& value) : value(std::forward<U>(value)) {}
};

template <typename... Ts>
struct tmap : public tmap_node<Ts>... {
  template <typename... Us>
  constexpr tmap(Us&&... us) : tmap_node<Ts>(std::forward<Us>(us))... {}
};

template <typename... Us>
tmap(Us&&...) -> tmap<Us...>;

template <typename... Us>
constexpr auto make_tmap(Us&&... us) -> tmap<Us...> {
  return tmap<Us...>(std::forward<Us>(us)...);
}

template <typename T, typename... Ts>
constexpr auto tmap_get(const tmap<Ts...>& map) -> const T& {
  static_assert(std::is_base_of_v<tmap_node<T>, tmap<Ts...>>,
                "T must be a base type of tmap<Ts...>");
  return static_cast<const tmap_node<T>&>(map).value;
}

template <typename T, typename... Ts>
constexpr auto tmap_get(tmap<Ts...>& map) -> T& {
  static_assert(std::is_base_of_v<tmap_node<T>, tmap<Ts...>>,
                "T must be a base type of tmap<Ts...>");
  return static_cast<tmap_node<T>&>(map).value;
}

template <typename... Ts, typename... Us>
constexpr auto tmap_cat(const tmap<Ts...>& m1,
                        const tmap<Us...>& m2) -> tmap<Ts..., Us...> {
  return tmap<Ts..., Us...>(tmap_get<Ts>(m1)..., tmap_get<Us>(m2)...);
}

template <typename M1, typename M2, typename... Mn>
constexpr auto tmap_cat(const M1& m1, const M2& m2, const Mn&... mn) {
  return tmap_cat(tmap_cat(m1, m2), mn...);
}

template <typename Func, typename... Ts>
constexpr auto for_each(const tmap<Ts...>& map, Func&& func) {
  static_assert((std::is_invocable_v<Func, const Ts&> && ...),
                "Function must be invocable with all values in the tmap");
  (std::invoke(func, static_cast<tmap_node<const Ts>&>(map).value), ...);
}

template <typename Func, typename... Ts>
constexpr auto for_each(tmap<Ts...>& map, Func&& func) -> void {
  static_assert((std::is_invocable_v<Func, Ts&> && ...),
                "Function must be invocable with all values in the tmap");
  (std::invoke(func, static_cast<tmap_node<Ts>&>(map).value), ...);
}

template <typename... Ts>
constexpr auto size(const tmap<Ts...>& map) -> std::size_t {
  return sizeof...(Ts);
}

}  // namespace tgr
