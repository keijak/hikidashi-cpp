#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>

template <class Key, class T>
using rb_tree_map =
    __gnu_pbds::tree<Key, T, std::less<Key>, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

template <class Key>
using rb_tree_set =
    __gnu_pbds::tree<Key, __gnu_pbds::null_type, std::less<Key>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

// Enables `__gnu_pbds::gp_hash_table::resize()` similar to
// `std::unordered_map::reserve()`.
#include <ext/pb_ds/assoc_container.hpp>

// https://codeforces.com/blog/entry/62393
struct custom_hash {
  // splitmix64
  // http://xorshift.di.unimi.it/splitmix64.c
  size_t operator()(std::uint64_t x) const {
    static const std::uint64_t FIXED_RANDOM =
        std::chrono::steady_clock::now().time_since_epoch().count();
    x += FIXED_RANDOM;
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
};

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

template <class K>
using gp_hash_set = __gnu_pbds::gp_hash_table<
    K, __gnu_pbds::null_type, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;
