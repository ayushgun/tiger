# Tiger

 A library of highly specialized containers and algorithms, such as compile-time heterogenous type containers or x86 AVX2 register strings. 

### Type Map

```cpp
#include <iostream>
#include <tiger/types/type_map.hpp>

int main() {
  tgr::tmap<int, double, char> map(1, 1.0, 'a');

  // Access value by type -- O(1)
  std::cout << tgr::tmap_get<char>(map) << '\n';

  // Iterate over all values -- O(n)
  tgr::for_each(map, [](const auto& value) { std::cout << value << '\n'; });
}
```

### Type Set

```cpp
#include <iostream>
#include <tiger/types/type_set.hpp>
#include <type_traits>

int main() {
  tgr::tset<int, double, char*> set;

  // Check for type existence -- O(1)
  std::cout << tgr::tset_contains<int>(set) << '\n';

  // Iterate over all types -- O(n)
  tgr::for_each(set, [](auto node) {
    using T = typename decltype(node)::type;
    if (std::is_arithmetic_v<T>) {
      std::cout << "Arithmetic type found\n";
    }
  });
}
```

### Short String

```cpp
#include <algorithm>
#include <iostream>
#include <tiger/simd/short_string.hpp>

int main() {
  tgr::short_string s = "Hello World";

  // Interoperability with STL APIs
  std::sort(s.begin(), s.end());
  std::cout << s << '\n';
  std::cout << s.size() << '\n';
}
```
