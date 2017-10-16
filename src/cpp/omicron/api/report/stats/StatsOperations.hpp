/*!
 * \file
 * \author David Saxon
 * \brief Utility operations relating to Omicron's statistics system.
 */
#ifndef OMICRON_API_REPORT_STATSOPERATIONS_HPP_
#define OMICRON_API_REPORT_STATSOPERATIONS_HPP_

#include <iosfwd>

#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class StatsQuery;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief A formatted version of the results of the query to the given output
 *        stream.
 *
 * \param title Option parameter that can be used to print a title for the
 *              query results.
 */
OMI_API_EXPORT void print_stats_query(
        const StatsQuery& query,
        std::ostream& out_stream,
        const arc::str::UTF8String& title = "");

} // namespace report
} // namespace omi

#endif
