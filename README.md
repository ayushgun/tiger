# Tiger

 A library of highly specialized STL-compliant containers and algorithms, such as compile-time heterogenous type containers, x86 AVX2 register strings, or stack memory allocators. 

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

### Stack Allocator

```cpp
#include <vector>
#include <tiger/stack/stack_allocator.hpp>

int main() {
  // Interoperability with STL containers
  std::vector<int, tgr::stack_allocator<int, 1024>> v;

  // Use existing container APIs without modification
  v.push_back(-1);
  std::cout << v.at(0) << '\n';
  std::cout << v.size() << '\n';
}
```
