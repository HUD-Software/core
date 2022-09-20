#include <core/templates/default_deleter.h>

namespace {
    enum class state {
        uninit,
        construct,
        destroyed
    };

    state A_state = state::uninit;
    struct a {
        a() { A_state = state::construct; }
        ~a() { A_state = state::destroyed; }
    };

    constexpr usize array_size = 2;
    state B_state[array_size] = { state::uninit };
    struct b {
        b() {}
        void set_index(usize idx) {
            mIndex = idx;
            B_state[mIndex] = state::construct;
        }
        ~b() { B_state[mIndex] = state::destroyed; }
        usize mIndex;
    };
}

TEST(templates, default_deleter) {


    /** Test DefaultDeleter */
    {
        ASSERT_EQ(A_state, state::uninit);
        a* a_ptr = new a;
        ASSERT_EQ(A_state, state::construct);
        hud::default_deleter<a> deleter_a;
        deleter_a(a_ptr);
        ASSERT_EQ(A_state, state::destroyed);


        for (usize cur = 0; cur < array_size; cur++) {
            ASSERT_EQ(B_state[cur], state::uninit);
        }
    }

    /** Test DefaultDeleter Array specialization */
    {
        b* b_ptr = new b[array_size];
        for (usize cur = 0; cur < array_size; cur++) {
            b_ptr[cur].set_index(cur);
            ASSERT_EQ(B_state[cur], state::construct);
        }

        hud::default_deleter<b[]> deleter_b;
        deleter_b(b_ptr);

        for (usize cur = 0; cur < array_size; cur++) {
            ASSERT_EQ(B_state[cur], state::destroyed);
        }
    }

    /** Test default free */
    {
        i32* c = static_cast<i32*>(hud::memory::allocate(sizeof(i32)));
        hud::default_free freer_c;
        freer_c(c);
    }
}