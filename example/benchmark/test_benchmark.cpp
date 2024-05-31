#include "benchmark/benchmark.h"

static void BM_StringCreation(benchmark::State& state) {
    for (auto _ : state)
        std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    state.counters["bytes_per_second"] = benchmark::Counter(state.bytes_processed(), benchmark::Counter::kIsRate);
    for (auto _ : state) {
        state.PauseTiming(); //暂停计时
        std::string copy(x);
        state.ResumeTiming(); //恢复计时
    }

}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
