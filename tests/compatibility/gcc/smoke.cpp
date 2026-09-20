#include "csv.hpp"

#include <cmath>
#include <iostream>

#if defined(CSV_COMPAT_EXPECT_FLOAT_OPTOUT) && defined(CLASSIFY_SCALAR_HAS_STD_FLOAT_FROM_CHARS)
#error "The explicit floating-point from_chars opt-out was ignored"
#endif

int main() {
    // Exercise field conversion and linking on old libstdc++ versions without
    // involving reader/chunk behavior, which is covered by the main suite.
    const char* inputs[] = { "1.25", "-42.5", "6.25e2", "1.5e-3" };
    const double expected[] = { 1.25, -42.5, 625.0, 0.0015 };
    for (unsigned i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
        csv::CSVField field(inputs[i]);
        const double actual = field.get<double>();
        if (!field.is_float() || !std::isfinite(actual)
            || std::abs(actual - expected[i]) > 1e-12) {
            std::cerr << "Incorrect floating-point conversion for " << inputs[i]
                      << ": " << actual << '\n';
            return 1;
        }
    }
    return 0;
}
