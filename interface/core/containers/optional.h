#ifndef HD_INC_CORE_OPTIONAL_H
#define HD_INC_CORE_OPTIONAL_H
#include "aligned_buffer.h"
#include "../memory.h"
#include "../templates/forward.h"
#include "../templates/move.h"
#include "../tag_in_place.h"
#include "../templates/swap.h"
#include "../traits/is_constructible.h"
#include "../traits/is_copy_constructible.h"
#include "../traits/is_move_constructible.h"
#include "../traits/is_assignable.h"
#include "../traits/is_trivially_destructible.h"
#include "../traits/is_trivially_copy_constructible.h"
#include "../traits/is_trivially_move_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/is_comparable_with_equal.h"
#include "../traits/is_comparable_with_not_equal.h"
#include "../traits/is_comparable_with_less.h"
#include "../traits/is_comparable_with_greater.h"
#include "../traits/is_comparable_with_less_equal.h"
#include "../traits/is_comparable_with_greater_equal.h"
#include "../traits/is_not_same.h"
#include "../traits/is_scalar.h"
#include "../traits/is_object.h"
#include "../traits/remove_cv_ref.h"
#include "../traits/decay.h"
#include "../traits/conjunction.h"
#include "../traits/negation.h"

namespace hud
{

    namespace details
    {

        /**
         * Contains the tagged union of the possible value.
         * @tparam base_t The optional_destructible_base that inherit this class.
         * @tparam value_t The type to store
         */
        template<typename base_t, typename value_t>
        struct optional_storage
        {

            /** Retrieves this as a optional_destructible_base */
            constexpr base_t *this_as_base_t() noexcept
            {
                return static_cast<base_t *>(this);
            }

            /** Retrieves this as a optional_destructible_base */
            constexpr const base_t *this_as_base_t() const noexcept
            {
                return static_cast<const base_t *>(this);
            }

            /** Call destructor on the contained value and mark the storage without value */
            constexpr void reset() noexcept
            {
                if (this_as_base_t()->some)
                {
                    hud::memory::destroy_object(&this_as_base_t()->some_value);
                    this_as_base_t()->some = false;
                }
            }

            /** Checks if it contains a value */
            [[nodiscard]] constexpr bool has_value() const noexcept
            {
                return this_as_base_t()->some;
            }

            /** Retrivies the contained value */
            [[nodiscard]] constexpr value_t &value() & noexcept
            {
                check(has_value());
                return this_as_base_t()->some_value;
            }

            /** Retrivies the contained value */
            [[nodiscard]] constexpr const value_t &value() const & noexcept
            {
                check(has_value());
                return this_as_base_t()->some_value;
            }

            /** Retrivies the contained value */
            [[nodiscard]] constexpr const value_t &&value() const && noexcept
            {
                check(has_value());
                return hud::move(this_as_base_t()->some_value);
            }

            /** Retrivies the contained value */
            [[nodiscard]] constexpr value_t &&value() && noexcept
            {
                check(has_value());
                return hud::move(this_as_base_t()->some_value);
            }

            /**
             * Construct the value with the given parameter.
             * @tparam ...u_type_t Types of ...args
             * @param ...args Parameters forwarded to the value constructor
             */
            template<typename... u_type_t>
            constexpr void construct_in_place(u_type_t &&...args) noexcept
            {
                hud::memory::construct_object_at(&this_as_base_t()->some_value, hud::forward<u_type_t>(args)...);
                this_as_base_t()->some = true;
            }

            /**
             * Assign another optional to this.
             * If *this and other does not contain a value, do nothing.
             * If *this and other contain a value, assign the value by forwarding the value of other in *this value.
             * If *this contains a value, but other does, the contained value is destroyed, and the optional does not contains a value after this call.
             * If *this does not contain a value, but other do, the value is constructed by forwarding the value of other in *this value.
             * @tparam u_type_t Type of other
             * @param other The optional to assign
             */
            template<typename u_type_t>
            constexpr void assign(u_type_t &&other) noexcept
            {
                if (has_value() == other.has_value())
                {
                    if (has_value())
                    {
                        value() = hud::forward<u_type_t>(other).value();
                    }
                }
                else
                {
                    if (has_value())
                    {
                        reset();
                    }
                    else
                    {
                        construct_in_place(hud::forward<u_type_t>(other).value());
                    }
                }
            }
        };

        /**
         * Destructor base of the optional. If the inner type is trivially destructible, the optional is trivially destructible.
         * @tparam type_t Inner type
         */
        template<typename type_t, bool = hud::is_trivially_destructible_v<type_t>>
        struct optional_destructible_base
            : optional_storage<optional_destructible_base<type_t>, type_t>
        {
            /** By default optional do not contains value */
            constexpr optional_destructible_base() noexcept
                : none_value()
                , some(false)
            {
            }

            /**
             * Construct an optional that contains a value.
             * The Value is construct in place.
             * @tparam ...args_t Types of args
             * @param tag_in_place tag used to select this constructor
             * @param args List of args forwarded to the constructed value
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<type_t, args_t...>)
            constexpr explicit optional_destructible_base(tag_in_place, args_t &&...args) noexcept
                : some_value(hud::forward<args_t>(args)...)
                , some(true)
            {
            }

            /** Aligned value in union that contains the value or nothing (empty struct)*/
            struct empty
            {
            };

            union alignas(alignof(type_t))
            {
                empty none_value;
                type_t some_value;
            };

            /**  True if storage contains a value, false otherwise */
            bool some = false;
        };

        /**
         * Destructor base of the optional. If the inner type is trivially destructible, the optional is trivially destructible.
         * @tparam type_t Inner type
         */
        template<typename type_t>
        struct optional_destructible_base<type_t, false>
            : optional_storage<optional_destructible_base<type_t>, type_t>
        {
            /** By default optional do not contains value */
            constexpr optional_destructible_base() noexcept
                : none_value()
                , some(false)
            {
            }

            /**
             * Construct an optional that contains a value.
             * The Value is construct in place.
             * @tparam ...args_t Types of args
             * @param tag_in_place tag used to select this constructor
             * @param args List of args forwarded to the constructed value
             */
            template<typename... args_t>
            requires(hud::is_constructible_v<type_t, args_t...>)
            constexpr explicit optional_destructible_base(tag_in_place, args_t &&...args) noexcept
                : some_value(hud::forward<args_t>(args)...)
                , some(true)
            {
            }

            /** Call the destructor of inner value if this has value */
            constexpr ~optional_destructible_base() noexcept
            {
                if (some)
                {
                    hud::memory::destroy_object(&some_value);
                }
            }

            /** Aligned value in union that contains the value or nothing (empty struct)*/
            struct empty
            {
            };

            union alignas(alignof(type_t))
            {
                empty none_value;
                type_t some_value;
            };

            /**  True if storage contains a value, false otherwise */
            bool some = false;
        };

        /**
         * Copy constructor base of the optional. If the inner type is trivially copy constructible, the optional is trivially copy constructible.
         * @tparam type_t Inner type
         */
        template<typename type_t, bool = is_trivially_copy_constructible_v<type_t>>
        struct optional_copy_base
            : optional_destructible_base<type_t>
        {
            using super_type = optional_destructible_base<type_t>;
            using super_type::super_type;
        };

        template<typename type_t>
        struct optional_copy_base<type_t, false>
            : optional_destructible_base<type_t>
        {

            using super_type = optional_destructible_base<type_t>;
            using super_type::super_type;

            constexpr optional_copy_base() = default;

            constexpr optional_copy_base(const optional_copy_base &other) noexcept
            {
                if (other.some)
                {
                    this->construct_in_place(other.some_value);
                }
            }

            constexpr optional_copy_base(optional_copy_base &&) = default;
            constexpr optional_copy_base &operator=(const optional_copy_base &) = default;
            constexpr optional_copy_base &operator=(optional_copy_base &&) = default;
        };

        /**
         * Move constructor base of the optional. If the inner type is trivially move constructible, the optional is trivially move constructible.
         * @tparam type_t Inner type
         */
        template<typename type_t, bool = is_trivially_move_constructible_v<type_t>>
        struct optional_move_base
            : optional_copy_base<type_t>
        {
            using super_type = optional_copy_base<type_t>;
            using super_type::super_type;
        };

        template<typename type_t>
        struct optional_move_base<type_t, false>
            : optional_copy_base<type_t>
        {
            using super_type = optional_copy_base<type_t>;
            using super_type::super_type;

            constexpr optional_move_base() = default;

            constexpr optional_move_base(optional_move_base &&other) noexcept
            {
                if (other.some)
                {
                    hud::memory::construct_object_at(&(this->some_value), hud::forward<type_t>(other.some_value));
                    this->some = true;
                }
            }

            constexpr optional_move_base(const optional_move_base &) = default;
            constexpr optional_move_base &operator=(const optional_move_base &) = default;
            constexpr optional_move_base &operator=(optional_move_base &&) = default;
        };

        /**
         * Copy assign operator base of the optional. If the inner type is trivially copy assignable, the optional is trivially copy assignable.
         * If the inner type is neither copy assignable, the optional is explictly not copy assignable.
         * @tparam type_t Inner type
         */
        template<typename type_t, bool IsCopyAssignTrivial = is_trivially_copy_constructible_v<type_t> && is_trivially_copy_assignable_v<type_t> && hud::is_trivially_destructible_v<type_t>, bool IsCopyAssignDeleted = hud::is_copy_constructible_v<type_t> && hud::is_copy_assignable_v<type_t>>
        struct optional_copy_assign_base
            : optional_move_base<type_t>
        {
            using super_type = optional_move_base<type_t>;
            using super_type::super_type;
        };

        template<typename type_t>
        struct optional_copy_assign_base<type_t, false, true> // Neither trivial or deleted
            : optional_move_base<type_t>
        {
            using super_type = optional_move_base<type_t>;
            using super_type::super_type;
            constexpr optional_copy_assign_base() = default;
            constexpr optional_copy_assign_base(const optional_copy_assign_base &) = default;
            constexpr optional_copy_assign_base(optional_copy_assign_base &&) = default;

            constexpr optional_copy_assign_base &operator=(const optional_copy_assign_base &other) noexcept
            {
                this->assign(other);
                return *this;
            };

            constexpr optional_copy_assign_base &operator=(optional_copy_assign_base &&) = default;
        };

        template<typename type_t, bool IsCopyAssignTrivial>
        struct optional_copy_assign_base<type_t, IsCopyAssignTrivial, false> // deleted
            : optional_move_base<type_t>
        {
            using super_type = optional_move_base<type_t>;
            using super_type::super_type;
            constexpr optional_copy_assign_base() = default;
            constexpr optional_copy_assign_base(const optional_copy_assign_base &) = default;
            constexpr optional_copy_assign_base(optional_copy_assign_base &&) = default;
            constexpr optional_copy_assign_base &operator=(const optional_copy_assign_base &) = delete;
            constexpr optional_copy_assign_base &operator=(optional_copy_assign_base &&) = default;
        };

        /**
         * Move assign operator base of the optional. If the inner type is trivially move assignable, the optional is trivially move assignable.
         * If the inner type is neither move assignable, the optional is explictly not move assignable.
         * @tparam type_t Inner type
         */
        template<typename type_t, bool IsMoveAssignTrivial = is_trivially_move_assignable_v<type_t> && is_trivially_move_constructible_v<type_t> && hud::is_trivially_destructible_v<type_t>, bool IsMoveAssignDeleted = hud::is_move_constructible_v<type_t> && hud::is_move_assignable_v<type_t>>
        struct optional_move_assign_base
            : optional_copy_assign_base<type_t>
        {
            using super_type = optional_copy_assign_base<type_t>;
            using super_type::super_type;
        };

        template<typename type_t>
        struct optional_move_assign_base<type_t, false, true> // Neither trivial or deleted
            : optional_copy_assign_base<type_t>
        {
            using super_type = optional_copy_assign_base<type_t>;
            using super_type::super_type;
            constexpr optional_move_assign_base() = default;
            constexpr optional_move_assign_base(const optional_move_assign_base &) = default;
            constexpr optional_move_assign_base(optional_move_assign_base &&) = default;
            constexpr optional_move_assign_base &operator=(const optional_move_assign_base &) = default;

            constexpr optional_move_assign_base &operator=(optional_move_assign_base &&other) noexcept
            {
                this->assign(hud::forward<optional_move_assign_base>(other));
                return *this;
            };
        };

        template<typename type_t, bool IsMoveAssignTrivial>
        struct optional_move_assign_base<type_t, IsMoveAssignTrivial, false> // deleted
            : optional_copy_assign_base<type_t>
        {
            using super_type = optional_copy_assign_base<type_t>;
            using super_type::super_type;
            constexpr optional_move_assign_base() = default;
            constexpr optional_move_assign_base(const optional_move_assign_base &) = default;
            constexpr optional_move_assign_base(optional_move_assign_base &&) = default;
            constexpr optional_move_assign_base &operator=(const optional_move_assign_base &) = default;
            constexpr optional_move_assign_base &operator=(optional_move_assign_base &&) = delete;
        };

        /** Hidden implementation of the optional. */
        template<typename type_t>
        struct optional_impl : optional_move_assign_base<type_t>
        {
            using super_type = optional_move_assign_base<type_t>;
            using super_type::super_type;

            constexpr optional_impl() = default;
            constexpr optional_impl(const optional_impl &) = default;
            constexpr optional_impl(optional_impl &&) = default;
            constexpr optional_impl &operator=(const optional_impl &) = default;
            constexpr optional_impl &operator=(optional_impl &&) = default;
        };

    } // namespace details

    /** Empty class type used to indicate optional type with uninitialized state */
    struct nullopt_t
    {
        struct tag
        {
        };

        explicit constexpr nullopt_t(tag)
        {
        }
    };

    /** Constant used to indicate optional type with uninitialized state. */
    inline constexpr nullopt_t nullopt {nullopt_t::tag {}};

    /**
     * Manages an optional contained value, i.e. a value that may or may not be present.
     * If an optional<type_t> contains a value, the value is guaranteed to be allocated as part of the optional object footprint, i.e. no dynamic memory allocation ever takes place.
     * The optional object contains a value in the following conditions:
     *  - The object is initialized with/assigned from a value of type type_t or another optional that contains a value.
     * The object does not contain a value in the following conditions:
     *  - The object is default-initialized.
     *  - The object is initialized with/assigned from a value of type hud::nullopt_t or an optional object that does not contain a value.
     *  - The member function reset() is called.
     */
    template<typename type_t>
    class optional
        : details::optional_impl<type_t>
    {
        using super_type = details::optional_impl<type_t>;

        static_assert(!is_reference_v<type_t>, "optional<type_t&> is ill-formed");
        static_assert(is_destructible_v<type_t>, "optional<type_t> must meet the requirements of destructible type (t.~type_t() should be valid.");

    public:
        /** Default construct an optional that does not contain a value */
        constexpr optional() noexcept = default;

        /** Copy construct the optional. Copy the inner value if presents. */
        constexpr optional(const optional &) noexcept = default;

        /** Move construct the optional. Move the inner value if presents. Does not make other empty. */
        constexpr optional(optional &&) noexcept = default;

        /**
         * Construct an optional that does not contain a value
         * @param nullopt_t tag that indicate optional type with uninitialized state
         */
        constexpr optional(hud::nullopt_t) noexcept {};

        /**
         * Construct an optional that contain a in-place constructed type_t value
         * @param tag_in_place tag used to select this constructor
         * @tparam args_t The type_t constructor arguments
         * @param args Arguments to forward to the constructor of the value
         */
        template<typename... args_t>
        requires(hud::is_constructible_v<type_t, args_t...>)
        constexpr explicit optional(tag_in_place, args_t &&...args) noexcept
            : super_type(tag_in_place {}, hud::forward<args_t>(args)...)
        {
        }

        /**
         * Constructs an optional object that contains a value
         * @tparam u_type_t Type of value
         * @param value The value to move
         */
        template<typename u_type_t = type_t>
        requires(hud::conjunction_v<
                 hud::is_constructible<type_t, u_type_t &&>,
                 is_not_same<remove_cv_ref_t<u_type_t>, tag_in_place>,
                 is_not_same<u_type_t, optional<type_t>>>)
        explicit(!hud::is_convertible_v<u_type_t &&, type_t>) constexpr optional(u_type_t &&value) noexcept
            : super_type(tag_in_place {}, hud::move(value))
        {
        }

        /**
         * If the optional contains a value, destroy that value.
         * Otherwise do nothing.
         * This does not contain a value after this call.
         * @param nullopt_t The hud::nullopt_t tag
         */
        constexpr optional &operator=(hud::nullopt_t) noexcept
        {
            reset();
            return *this;
        }

        /**
         * If the optional contains a value, copy assign of the contained value.
         * Otherwise do nothing.
         */
        constexpr optional &operator=(const optional &) noexcept = default;

        /**
         * If the optional contains a value, move assign of the contained value.
         * Otherwise do nothing.
         * Does not make other empty.
         */
        constexpr optional &operator=(optional &&) noexcept = default;

        /*
         * Depending on whether *this contains a value before the call, the contained value is either direct-initialized from hud::forward<u_type_t>(value) or assigned from hud::forward<u_type_t>(value).
         * @param value The value to perfect-forward assign
         * @return *this
         */
        template<typename u_type_t = type_t>
        requires(hud::conjunction_v<
                 is_not_same<
                     remove_cv_ref_t<u_type_t>,
                     optional<type_t>>,
                 is_constructible<type_t, u_type_t>,
                 is_assignable<type_t &, u_type_t>,
                 hud::negation<
                     hud::conjunction<
                         is_scalar<type_t>,
                         hud::is_same<hud::decay_t<u_type_t>, type_t>>>>)
        constexpr optional &operator=(u_type_t &&value) noexcept
        {
            if (has_value())
            {
                this->some_value = hud::forward<u_type_t>(value);
            }
            else
            {
                this->construct_in_place(hud::forward<u_type_t>(value));
            }
            return *this;
        }

        /**
         * Copy assign another optional to this.
         * If *this and other does not contain a value, do nothing.
         * If *this and other contain a value, assign *this value with other values.
         * If *this contains a value, but other does, the contained value is destroyed, and the optional does not contains a value after this call.
         * If *this does not contain a value, but other do, the value is constructed *this value with other values.
         * @tparam u_type_t Type of other
         * @param other The optional to assign
         */
        template<typename u_type_t>
        requires(hud::conjunction_v<
                 hud::is_copy_constructible<type_t, u_type_t>,
                 hud::is_copy_assignable<type_t, u_type_t>,
                 hud::negation<
                     hud::disjunction<
                         hud::is_constructible<type_t, optional<u_type_t> &>,
                         hud::is_constructible<type_t, const optional<u_type_t> &>,
                         hud::is_constructible<type_t, optional<u_type_t> &&>,
                         hud::is_constructible<type_t, const optional<u_type_t> &&>,
                         hud::is_convertible<optional<u_type_t> &, type_t>,
                         hud::is_convertible<const optional<u_type_t> &, type_t>,
                         hud::is_convertible<optional<u_type_t> &&, type_t>,
                         hud::is_convertible<const optional<u_type_t> &&, type_t>,
                         hud::is_assignable<type_t &, optional<u_type_t> &>,
                         hud::is_assignable<type_t &, const optional<u_type_t> &>,
                         hud::is_assignable<type_t &, optional<u_type_t> &&>,
                         hud::is_assignable<type_t &, const optional<u_type_t> &&>>>>)
        constexpr optional &operator=(const optional<u_type_t> &other) noexcept
        {
            this->assign(other);
            return *this;
        }

        /**
         * Move sassign another optional to this.
         * If *this and other does not contain a value, do nothing.
         * If *this and other contain a value, assign the value by forwarding the value of other in *this value.
         * If *this contains a value, but other does, the contained value is destroyed, and the optional does not contains a value after this call.
         * If *this does not contain a value, but other do, the value is constructed by forwarding the value of other in *this value.
         * @tparam u_type_t Type of other
         * @param other The optional to assign
         */
        template<typename u_type_t>
        requires(hud::conjunction_v<
                 is_constructible<type_t, u_type_t>,
                 is_assignable<type_t &, u_type_t>,
                 hud::negation<hud::disjunction<
                     is_constructible<type_t, optional<u_type_t> &>,
                     is_constructible<type_t, const optional<u_type_t> &>,
                     is_constructible<type_t, optional<u_type_t> &&>,
                     is_constructible<type_t, const optional<u_type_t> &&>,
                     hud::is_convertible<optional<u_type_t> &, type_t>,
                     hud::is_convertible<const optional<u_type_t> &, type_t>,
                     hud::is_convertible<optional<u_type_t> &&, type_t>,
                     hud::is_convertible<const optional<u_type_t> &&, type_t>,
                     is_assignable<type_t &, optional<u_type_t> &>,
                     is_assignable<type_t &, const optional<u_type_t> &>,
                     is_assignable<type_t &, optional<u_type_t> &&>,
                     is_assignable<type_t &, const optional<u_type_t> &&>>>>)
        constexpr optional &operator=(optional<u_type_t> &&other) noexcept
        {
            this->assign(hud::forward<optional<u_type_t>>(other));
            return *this;
        }

        /** Checks whether the optional contains a value or not */
        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return super_type::has_value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]] constexpr type_t &value() & noexcept
        {
            return super_type::value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]] constexpr const type_t &value() const & noexcept
        {
            return super_type::value();
        }

        /** Retrieves a const l-value reference to the contained value */
        [[nodiscard]] constexpr const type_t &&value() const && noexcept
        {
            return hud::move(super_type::value());
        }

        /** Retrieves a l-value reference to the contained value */
        [[nodiscard]] constexpr type_t &&value() && noexcept
        {
            return hud::move(super_type::value());
        }

        /**
         * If the optional contains a value, destroy that value.
         * Otherwise do nothing.
         * This does not contain a value after this call.
         */
        constexpr void reset() noexcept
        {
            super_type::reset();
        }

        /**
        Construct the contained value in-place.
        If this contain a value the contained value is destroyed by calling its destructor before constructing the value.
        @tparam ...args_t Types of args
        @param args Arguments to forward to the constructor of the value
        */
        template<typename... args_t>
        constexpr type_t &emplace(args_t &&...args) noexcept
        {
            reset();
            this->construct_in_place(hud::forward<args_t>(args)...);
            return value();
        }

        /** Checks whether the optional contains a value or not */
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        /** Retrieves a reference to the contained value if optional contains a value, return the default_value otherwise */
        template<typename u_type_t>
        [[nodiscard]] constexpr type_t value_or(u_type_t &&default_value) const & noexcept
        {
            static_assert(hud::is_copy_constructible_v<type_t>, "optional<type_t>::value_or: type_t must be copy constructible");
            static_assert(hud::is_convertible_v<u_type_t, type_t>, "optional<type_t>::value_or: u_type_t must be convertible to type_t");
            return has_value() ? value() : static_cast<type_t>(hud::forward<u_type_t>(default_value));
        }

        /** Retrieve a reference to the contained value if optional contains a value, return the default_value otherwise */
        template<typename u_type_t>
        [[nodiscard]] constexpr type_t value_or(u_type_t &&default_value) && noexcept
        {
            static_assert(hud::is_copy_constructible_v<type_t>, "optional<type_t>::value_or: type_t must be copy constructible");
            static_assert(hud::is_convertible_v<u_type_t, type_t>, "optional<type_t>::value_or: u_type_t must be convertible to type_t");
            return has_value() ? hud::move(value()) : static_cast<type_t>(hud::forward<u_type_t>(default_value));
        }

        /** Retrieves a pointer to the contained value */
        [[nodiscard]] constexpr type_t *operator->() noexcept
        {
            check(has_value());
            return &value();
        }

        /** Retrieves a pointer to the contained value */
        [[nodiscard]] constexpr const type_t *operator->() const noexcept
        {
            check(has_value());
            return &value();
        }

        /** Retrieves a reference to the contained value */
        [[nodiscard]] constexpr type_t &operator*() noexcept
        {
            check(has_value());
            return value();
        }

        /** Retrieves a reference to the contained value */
        [[nodiscard]] constexpr const type_t &operator*() const noexcept
        {
            check(has_value());
            return value();
        }

        /** Swap with another optional */
        constexpr void swap(optional &other) noexcept
        {
            if (has_value() == other.has_value())
            {
                if (has_value())
                {
                    hud::swap(value(), other.value());
                }
            }
            else
            {
                const bool this_has_value = has_value();
                // Move the optional that contains value to the optional that do not contains value then destroy the first one
                optional &empty = this_has_value ? other : *this;
                optional &non_empty = this_has_value ? *this : other;
                empty.construct_in_place(hud::move(non_empty.value()));
                non_empty.reset();
            }
        }
    };

    /**
     * Swap first optional with the second optional
     * Same as first.swap(second)
     * @tparam type_t The element type of both optional
     * @param first The first optional to swap
     * @param second The second optional to swap
     */
    template<typename type_t>
    constexpr void swap(optional<type_t> &first, optional<type_t> &second) noexcept
    {
        first.swap(second);
    }

    /**
     * Checks whether right and left optional are equal.
     * Optionals are equal if both contained or not a value and if contained values are equal.
     * Value types must be comparable with operator==()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right and left optional are equal, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(hud::is_comparable_with_equal_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator==(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        const bool left_has_value = left.has_value();
        return left_has_value == right.has_value() && (!left_has_value || left.value() == right.value());
    }

    /**
     * Checks whether right and left optional are not equals.
     * Optionals are not equals if at least one optional don't contains a value or if both contains a value, checks whether both values are not equals
     * Value types must be comparable with operator!=()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right and left optional are not equals, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(hud::is_comparable_with_not_equal_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator!=(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        const bool left_has_value = left.has_value();
        return left_has_value != right.has_value() || (left_has_value && left.value() != right.value());
    }

    /**
     * Checks whether right is less than left optional.
     * An right is less than left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is less than left value.
     * Value types must be comparable with operator<()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right is less than left optional, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(is_comparable_with_less_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator<(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        return right.has_value() && (!left.has_value() || left.value() < right.value());
    }

    /**
     * Checks whether right is greater than left optional.
     * An right is greater than left if :
     *     - right contains a value while left don't, or,
     *     - both contains a value and right value is greater than left value.
     * Value types must be comparable with operator>()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right is greater than left optional, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(is_comparable_with_greater_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator>(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        return left.has_value() && (!right.has_value() || left.value() > right.value());
    }

    /**
     * Checks whether right is less or equal than left optional.
     * An right is less or equal than left if :
     *     - right don't contains a value while left do, or,
     *     - both contains a value and right value is less or equal than left value.
     * Value types must be comparable with operator<=()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right is less or equal than left optional, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(is_comparable_with_less_equal_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator<=(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        return !left.has_value() || (right.has_value() && left.value() <= right.value());
    }

    /**
     * Checks whether right is greater or equal than left optional.
     * An right is greater or equal than left if :
     *     - right contains a value while left don't, or,
     *     - both contains a value and right value is greater or equal than left value.
     * Value types must be comparable with operator>=()
     * @tparam left_t Value type of the left optional
     * @tparam right_t Value type of the right optional
     * @param left The left optional to compare
     * @param right The right optional to compare
     * @param true if right is greater or equal than left optional, false otherwise
     */
    template<typename left_t, typename right_t>
    requires(is_comparable_with_greater_equal_v<left_t, right_t>)
    [[nodiscard]] constexpr bool operator>=(const optional<left_t> &left, const optional<right_t> &right) noexcept
    {
        return !right.has_value() || (left.has_value() && left.value() >= right.value());
    }

    /**
     * Checks whether an optional value is equal to a value
     * optional is equal to a value if :
     *     - the optional contain a value and the contained value is equal the compared value.
     * Value types must be comparable with operator==()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(hud::is_comparable_with_equal_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator==(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() == value : false;
    }

    /**
     * Checks whether a value is equal to an optional
     * A value is equal to a optional if :
     *     - the optional contain a value and the value is equal the optional value.
     * Value types must be comparable with operator==()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if a value is equal to an optional, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(hud::is_comparable_with_equal_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator==(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value == option.value() : false;
    }

    /**
     * Checks whether an optional value is not equal to a value
     * optional is not equal to a value if:
     *     - the optional do not contain a value and the optional value is not equal the compared value, or,
     *     - the optional do not have a value
     * Value types must be comparable with operator!=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is not equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(hud::is_comparable_with_not_equal_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator!=(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() != value : true;
    }

    /**
     * Checks whether a value is not equal an optional
     * A value is not equal to an optional if:
     *     - the optional do not contain a value and the optional value is not equal the compared value, or,
     *     - the optional do not have a value
     * Value types must be comparable with operator!=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if a value is not equal an optional, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(hud::is_comparable_with_not_equal_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator!=(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value != option.value() : true;
    }

    /**
     * Checks whether an optional value is less than a value
     * optional is less to a value if:
     *     - the optional contains a value and the optional value is less than the compared value, or,
     *     - the optional do not contain a value
     * Value types must be comparable with operator<()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is less than a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_less_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator<(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() < value : true;
    }

    /**
     * Checks whether a value is less than an optional
     * A value is less than an optional if:
     *     - the optional contain a value and the value is less than the optional value
     * Value types must be comparable with operator<()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is less than a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_less_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator<(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value < option.value() : false;
    }

    /**
     * Checks whether an optional value is greater than a value
     * optional is greater than a value if:
     *     - the optional contain a value and the contained value is greater than the compared value
     * Value types must be comparable with operator>()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is greater than a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_greater_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator>(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() > value : false;
    }

    /**
     * Checks whether a value is greater than an optional
     * A value is greater than an optional if:
     *     - the optional contain a value and the contained value is greater than the compared value, or,
     *     - the optional do not contain a value
     * Value types must be comparable with operator>()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if a value is greater than an optional, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_greater_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator>(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value > option.value() : true;
    }

    /**
     * Checks whether an optional value is less or equal a value
     * optional is less or equal to a value if:
     *     - the optional contains a value and the optional value is less or equal the compared value, or,
     *     - the optional do not contain a value
     * Value types must be comparable with operator<=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is less or equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_less_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator<=(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() <= value : true;
    }

    /**
     * Checks whether a value is less or equal an optional
     * A value is less or equal than an optional if:
     *     - the optional contain a value and the value is less or equal the optional value
     * Value types must be comparable with operator<=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is less or equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_less_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator<=(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value <= option.value() : false;
    }

    /**
     * Checks whether an optional value is greater or equal a value
     * optional is greater or equal to a value if:
     *     - the optional contains a value and the optional value is greater or equal the compared value
     * Value types must be comparable with operator>=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is greater or equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_greater_equal_v<type_t, value_t>)
    [[nodiscard]] constexpr bool operator>=(const optional<type_t> &option, const value_t &value) noexcept
    {
        return option.has_value() ? option.value() >= value : false;
    }

    /**
     * Checks whether a value is greater or equal an optional
     * A value is less than an optional if:
     *     - the optional contain a value and the value is greater or equal the optional value, or,
     *     - the optional do not contain a value
     * Value types must be comparable with operator>=()
     * @tparam type_t Value type of the optional
     * @tparam value_t Type of the value
     * @param option The optional to compare
     * @param value The value to compare
     * @param true if an optional value is greater or equal to a value, false otherwise
     */
    template<typename type_t, typename value_t>
    requires(is_comparable_with_greater_equal_v<value_t, type_t>)
    [[nodiscard]] constexpr bool operator>=(const value_t &value, const optional<type_t> &option) noexcept
    {
        return option.has_value() ? value >= option.value() : true;
    }

} // namespace hud

#endif // HD_INC_CORE_OPTIONAL_H
