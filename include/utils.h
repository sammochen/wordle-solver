#include <vector>

namespace utils {
template <typename T>
std::vector<T> concat(std::vector<T> A, const std::vector<T> &&B) {
  A.insert(A.end(), B.begin(), B.end());
  return A;
}
} // namespace utils
