// 1
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")

// 2
#pragma GCC optimize("unroll-loops")
#pragma GCC target( \
    "popcnt,bmi2,fma,fma4,avx512f,avx512dq,avx512cd,avx512bw,avx512vl")
