#include <benchmark/benchmark.h>

#include "array.h"
#include "doubly_list.h"
#include "forward_list.h"
#include "hash_table.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"

static void BM_ArrayPushBack(benchmark::State& state) {
    for (auto _ : state) {
        Array arr("Array");
        for (int i = 0; i < state.range(0); ++i) {
            arr.push_back("value");
        }
        benchmark::DoNotOptimize(arr.length());
    }
}
BENCHMARK(BM_ArrayPushBack)->Range(8, 1 << 12);


static void BM_DoublyListPushTailAndFind(benchmark::State& state) {
    for (auto _ : state) {
        DoublyList dl("DoublyList");
        for (int i = 0; i < state.range(0); ++i) {
            dl.push_tail("value");
        }
        benchmark::DoNotOptimize(dl.find("value"));
    }
}
BENCHMARK(BM_DoublyListPushTailAndFind)->Range(8, 1 << 12);


static void BM_ForwardListPushTailAndFind(benchmark::State& state) {
    for (auto _ : state) {
        Forward_List fl("ForwardList");
        for (int i = 0; i < state.range(0); ++i) {
            fl.push_tail("value");
        }
        benchmark::DoNotOptimize(fl.find("value"));
    }
}
BENCHMARK(BM_ForwardListPushTailAndFind)->Range(8, 1 << 12);


static void BM_QueuePushPop(benchmark::State& state) {
    for (auto _ : state) {
        Queue q("Queue");
        for (int i = 0; i < state.range(0); ++i) {
            q.push("value");
        }
        while (!q.is_empty()) {
            benchmark::DoNotOptimize(q.pop());
        }
    }
}
BENCHMARK(BM_QueuePushPop)->Range(8, 1 << 12);


static void BM_StackPushPop(benchmark::State& state) {
    for (auto _ : state) {
        Stack st("Stack");
        for (int i = 0; i < state.range(0); ++i) {
            st.push("value");
        }
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(st.pop());
        }
    }
}
BENCHMARK(BM_StackPushPop)->Range(8, 1 << 12);


static void BM_HashTableInsertGet(benchmark::State& state) {
    for (auto _ : state) {
        HashTable ht(state.range(0));
        for (int i = 0; i < state.range(0); ++i) {
            ht.insert(i, i);
        }
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(ht.get(i));
        }
    }
}
BENCHMARK(BM_HashTableInsertGet)->Range(8, 1 << 12);


static void BM_CuckooHashTableInsertGet(benchmark::State& state) {
    for (auto _ : state) {
        CuckooHashTable ht(state.range(0));
        for (int i = 0; i < state.range(0); ++i) {
            ht.insert(i, i);
        }
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(ht.get(i));
        }
    }
}
BENCHMARK(BM_CuckooHashTableInsertGet)->Range(8, 1 << 12);


static void BM_TreeInsertFind(benchmark::State& state) {
    for (auto _ : state) {
        Tree t("benchmark_tree");
        for (int i = 0; i < state.range(0); ++i) {
            t.insert(i);
        }
        benchmark::DoNotOptimize(t.find(state.range(0) / 2));
    }
}
BENCHMARK(BM_TreeInsertFind)->Range(8, 1 << 12);

BENCHMARK_MAIN();
