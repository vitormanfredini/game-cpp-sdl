#include <array>
#include <cmath>

constexpr float TWO_PI = 6.28318530718f;

template <size_t N>
class SineLookupTable {
public:
    static std::array<float, N> generateSineLookup() {
        std::array<float, N> lookupTable = {};
        for (size_t i = 0; i < N; ++i) {
            lookupTable[i] = std::sin(TWO_PI * i / N);
        }
        return lookupTable;
    }

    static inline std::array<float, N> table = generateSineLookup();
};
