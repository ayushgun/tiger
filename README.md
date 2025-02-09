# Tiger

 A library of highly specialized STL-compliant containers and algorithms, such as compile-time heterogenous type containers, ARM Neon register strings, or stack memory allocators. 

### Short String

```cpp
#include <algorithm>
#include <iostream>
#include <tiger/short_string.hpp>

int main() {
  tgr::short_string s = "Hello World";

  // Interoperability with STL APIs
  std::sort(s.begin(), s.end());
  std::cout << s << '\n';
  std::cout << s.size() << '\n';
}
```

### Service Thread

```cpp
#include <cmath>
#include <sys/qos.h>
#include <tiger/service_thread.hpp>

int kernel(int n) {
  return std::atan(std::tan(n));
}

int main() {
  // Launch thread which may be pinned to P core
  tgr::performance_thread p(kernel, 10);
  p.join();

  // Launch thread which may be scheduled on E core
  tgr::performance_thread e(kernel, 10);
  e.join();

  // Launch thread with custom QoS class
  tgr::service_thread<QOS_CLASS_UTILITY> u(kernel, 10);
  u.join();
}
```

### Type Map

```cpp
#include <iostream>
#include <tiger/type_map.hpp>

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
#include <tiger/type_set.hpp>
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
#include <iostream>
#include <vector>
#include <tiger/stack_allocator.hpp>

int main() {
  // Interoperability with STL containers
  std::vector<int, tgr::stack_allocator<int, 1024>> v;

  // Use existing container APIs without modification
  v.push_back(-1);
  std::cout << v.at(0) << '\n';
  std::cout << v.size() << '\n';
}
```
