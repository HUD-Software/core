#include <core/templates/default_deleter.h>

namespace hud_test {
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
        ASSERT_EQ(hud_test::A_state, hud_test::state::uninit);
        hud_test::a* a_ptr = new hud_test::a;
        ASSERT_EQ(hud_test::A_state, hud_test::state::construct);
        hud::default_deleter<hud_test::a> deleter_a;
        deleter_a(a_ptr);
        ASSERT_EQ(hud_test::A_state, hud_test::state::destroyed);


        for (usize cur = 0; cur < hud_test::array_size; cur++) {
            ASSERT_EQ(hud_test::B_state[cur], hud_test::state::uninit);
        }
    }

    /** Test DefaultDeleter Array specialization */
    {
        hud_test::b* b_ptr = new hud_test::b[hud_test::array_size];
        for (usize cur = 0; cur < hud_test::array_size; cur++) {
            b_ptr[cur].set_index(cur);
            ASSERT_EQ(hud_test::B_state[cur], hud_test::state::construct);
        }

        hud::default_deleter<hud_test::b[]> deleter_b;
        deleter_b(b_ptr);

        for (usize cur = 0; cur < hud_test::array_size; cur++) {
            ASSERT_EQ(hud_test::B_state[cur], hud_test::state::destroyed);
        }
    }

    /** Test default free */
    {
        i32* c = static_cast<i32*>(hud::memory::allocate(sizeof(i32)));
        hud::default_free freer_c;
        freer_c(c);
    }
}