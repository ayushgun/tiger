# Tiger 

A library of heterogeneous compile-time containers for types.

## Getting Started

These are minimal examples written to showcase the purpose of tiger's containers. This is only the surface of what is possible; to see the comprehensive API, see the header files.

### Type Map

```cpp
#include <iostream>
#include <tiger/type_map.hpp>

int main() {
  tgr::tmap<int, double, char> map(1, 1.0, 'a');

  // Access an individual value via the type key -- O(1)
  std::cout << tgr::tmap_get<char>(map) << '\n';

  // Iterate over all values in the map -- O(n)
  tgr::for_each(map, [](const auto& value) { std::cout << value << '\n'; });
}
```

### Type Set

```cpp
#include <iostream>
#include <tiger/type_set.hpp>
#include <type_traits>

int main() {
  tgr::tset<int, double, char*> set;

  // Check for existence of a type -- O(1)
  std::cout << tgr::tset_contains<int>(set) << '\n';

  // Iterate over all types in the set -- O(n)
  tgr::for_each(set, [](auto node) {
    using T = typename decltype(node)::type;

    if (std::is_arithmetic_v<T>) {
      std::cout << "Arithmetic type found\n";
    }
  });
}
```

