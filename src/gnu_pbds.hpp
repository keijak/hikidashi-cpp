#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>
template <class Key>
using rb_tree_set =
    __gnu_pbds::tree<Key, __gnu_pbds::null_type, std::less<Key>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;
template <class Key, class T>
using rb_tree_map =
    __gnu_pbds::tree<Key, T, std::less<Key>, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

// #include <ext/pb_ds/assoc_container.hpp>
using __gnu_pbds::gp_hash_table;
