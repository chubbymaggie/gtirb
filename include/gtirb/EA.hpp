#pragma once

#include <cstdint>
#include <gtirb/Constants.hpp>
#include <gtirb/Export.hpp>
#include <limits>
#include <string>

namespace gtirb
{
    ///
    /// \class EA
    /// \author John E. Farrier
    ///
    /// A special class to store an Effective Address. This is initialized to
    /// `gtirb::constants::BadAddress`. It is compatible with a uint64_t for 64-bit address storage.
    /// An EA cannot store a relative address as it cannot contain a negative number.
    ///
    class GTIRB_GTIRB_EXPORT_API EA
    {
    public:
        ///
        /// Default constructor.
        /// The default implementation provided by the compiler is used.
        ///
        EA() = default;

        ///
        /// Allow explicit conversion from a 'uint64_t' to an gtirb::EA object type.
        /// Do not allow other types of integers to be automatically converted.
        ///
        explicit EA(uint64_t x);

        ///
        /// Explicitly set the value of the EA from a 'uint64_t'.
        ///
        void set(uint64_t x);

        ///
        /// Get the value of the EA as a 'uint64_t'.
        ///
        uint64_t get() const;

        ///
        /// Allow for static conversion of the EA to a 'uint64_t'.
        ///
        operator uint64_t() const;

        ///
        /// Assignment operator overload
        ///
        EA operator=(EA);

        ///
        /// Allow testing for equality with a 'uint64_t'.
        ///
        bool operator==(uint64_t x) const;

        bool operator==(EA x) const;
        bool operator!=(EA x) const;
        bool operator>(EA x) const;
        bool operator<(EA x) const;

        EA operator+(EA x) const;
        EA operator+=(EA x);
        EA operator-(EA x) const;
        EA operator-=(EA x);

        ///
        /// Provide for static casting to a 'std::string'.
        ///
        operator std::string() const;

    private:
        ///
        /// Prevent automatic type conversions.
        /// This template is not implemented as to provide compiler protection against automatic
        /// type conversions.
        ///
        template <typename T>
        void set(T);

        ///
        /// Prevent automatic type conversions.
        /// This template is not implemented as to provide compiler protection against automatic
        /// type conversions.
        ///
        template <typename T>
        EA operator+(T);

        ///
        /// Prevent automatic type conversions.
        /// This template is not implemented as to provide compiler protection against automatic
        /// type conversions.
        ///
        template <typename T>
        EA operator+=(T);

        ///
        /// Prevent automatic type conversions.
        /// This template is not implemented as to provide compiler protection against automatic
        /// type conversions.
        ///
        template <typename T>
        EA operator-(T);

        ///
        /// Prevent automatic type conversions.
        /// This template is not implemented as to provide compiler protection against automatic
        /// type conversions.
        ///
        template <typename T>
        EA operator-=(T);

        ///
        /// Internal storage for the effective address (EA).
        /// It is initialized with the constant gtirb::constants::BadAddress.
        ///
        uint64_t ea{gtirb::constants::BadAddress};
    };
} // namespace gtirb

///
/// Provide for testing equality between a gtirb::EA and a uint64_t.
///
inline bool operator==(const uint64_t rhs, const gtirb::EA lhs)
{
    return gtirb::EA(rhs) == lhs;
}