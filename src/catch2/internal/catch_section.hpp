
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_SECTION_HPP_INCLUDED
#define CATCH_SECTION_HPP_INCLUDED

#include <catch2/internal/catch_compiler_capabilities.hpp>
#include <catch2/internal/catch_config_static_analysis_support.hpp>
#include <catch2/internal/catch_noncopyable.hpp>
#include <catch2/catch_section_info.hpp>
#include <catch2/catch_timer.hpp>
#include <catch2/catch_totals.hpp>
#include <catch2/internal/catch_unique_name.hpp>

namespace Catch {

    class Section : Detail::NonCopyable {
    public:
        Section( SectionInfo&& info );
        Section( SourceLineInfo const& _lineInfo,
                 StringRef _name,
                 const char* const = nullptr );
        ~Section();

        // This indicates whether the section should be executed or not
        explicit operator bool() const;

    private:
        SectionInfo m_info;

        Counts m_assertions;
        bool m_sectionIncluded;
        Timer m_timer;
    };

} // end namespace Catch

#if !defined(CATCH_CONFIG_EXPERIMENTAL_STATIC_ANALYSIS_SUPPORT)
#    define INTERNAL_CATCH_SECTION( ... )                                 \
        CATCH_INTERNAL_START_WARNINGS_SUPPRESSION                         \
        CATCH_INTERNAL_SUPPRESS_UNUSED_VARIABLE_WARNINGS                  \
        if ( Catch::Section const& INTERNAL_CATCH_UNIQUE_NAME(            \
                 catch_internal_Section ) =                               \
                 Catch::Section( CATCH_INTERNAL_LINEINFO, __VA_ARGS__ ) ) \
        CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION

#    define INTERNAL_CATCH_DYNAMIC_SECTION( ... )                     \
        CATCH_INTERNAL_START_WARNINGS_SUPPRESSION                     \
        CATCH_INTERNAL_SUPPRESS_UNUSED_VARIABLE_WARNINGS              \
        if ( Catch::Section const& INTERNAL_CATCH_UNIQUE_NAME(        \
                 catch_internal_Section ) =                           \
                 Catch::SectionInfo(                                  \
                     CATCH_INTERNAL_LINEINFO,                         \
                     ( Catch::ReusableStringStream() << __VA_ARGS__ ) \
                         .str() ) )                                   \
        CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION

#else

// These section definitions imply that at most one section at one
// level will be intered (because only one section's __LINE__ can be
// equal to the dummy `sectionHint` variable from `TEST_CASE`).

namespace Catch {
namespace Detail {
// Intentionally without linkage, as it should only be used as a dummy
// symbol for static analysis.
int GetSectionHint();
}
}

#    define INTERNAL_CATCH_SECTION( ... )                            \
        if ( [[maybe_unused]] int previousSectionHint = sectionHint, \
             sectionHint = Catch::Detail::GetNewSectionHint();       \
             previousSectionHint == __LINE__ )

#    define INTERNAL_CATCH_DYNAMIC_SECTION( ... )                    \
        if ( [[maybe_unused]] int previousSectionHint = sectionHint, \
             sectionHint = Catch::Detail::GetNewSectionHint();       \
             previousSectionHint == __LINE__ )

#endif


#endif // CATCH_SECTION_HPP_INCLUDED
