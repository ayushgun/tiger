# Tiger

Tiger is an opinionated C++ library offering highly specialized containers and algorithms, such as compile-time heterogeneous type containers or SIMD short strings.

### Type Map

```cpp
#include <iostream>
#include <tiger/type_map.hpp>

int main() {
  tgr::tmap<int, double, char> map(1, 1.0, 'a');

  // Access value by type
  std::cout << tgr::tmap_get<char>(map) << '\n';

  // Iterate over all values
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

  // Check for type existence
  std::cout << tgr::tset_contains<int>(set) << '\n';

  // Iterate over all types
  tgr::for_each(set, [](auto node) {
    using T = typename decltype(node)::type;
    if (std::is_arithmetic_v<T>) {
      std::cout << "Arithmetic type found\n";
    }
  });
}
```

### Fast String

```cpp
#include <iostream>
#include <tiger/string/fast_string.hpp>

int main() {
  tgr::fstring str("Hello, Tiger!");
  str.push_back('!');

  std::cout << str << '\n';
}
```
