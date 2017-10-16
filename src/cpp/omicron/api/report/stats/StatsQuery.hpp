/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_REPORT_STATS_STATSQUERY_HPP_
#define OMICRON_API_REPORT_STATS_STATSQUERY_HPP_

#include <unordered_map>
#include <vector>

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{
namespace report
{

class StatsDatabase;

/*!
 * \brief An object that is used make a query into Omicron's StatsDatabase.
 *
 * Queries are effectively a array of fnmatch patterns that are matched against
 * the names of entries in the StatsDatabase. All of the entries that match
 * any of the patterns are returned via this object.
 */
class StatsQuery
    : private arc::lang::Noncomparable
{
private:

    //--------------------------------------------------------------------------
    //                               FRIEND CLASSES
    //--------------------------------------------------------------------------

    friend class StatsDatabase;

public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Defines the array of strings that are used for making matches
     *        during the query.
     */
    typedef std::vector<arc::str::UTF8String> PatternArray;

    /*!
     * \brief Defines the mapping from statistic's names to their attribute
     *        values which are retrieved as a result of this query.
     */
    typedef std::unordered_map<arc::str::UTF8String, omi::DataAttribute> Result;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new empty StatsQuery.
     */
    OMI_API_EXPORT StatsQuery();

    /*!
     * \brief Creates a StatsQuery using the patterns from the data described by
     *        the given iterators.
     *
     * \tparam T_InputIterator The type of the iterator being used by this
     *                         constructor, this must be an iterator over a
     *                         container of arc::str::UTF8String objects.
     */
    template<typename T_InputIterator>
    StatsQuery(const T_InputIterator& first, const T_InputIterator& last)
        : m_patterns(first, last)
    {
    }

    /*!
     * \brief Creates a StatsQuery reading the data from the given .query file.
     *
     * \throw arc::ex::IOError If the file path cannot be opened.
     */
    OMI_API_EXPORT StatsQuery(const arc::io::sys::Path& path);

    /*!
     * \brief Copy constructor.
     */
    OMI_API_EXPORT StatsQuery(const StatsQuery& other);

    /*!
     * \brief Move constructor.
     */
    OMI_API_EXPORT StatsQuery(StatsQuery&& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~StatsQuery();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Copy assignment operator.
     */
    OMI_API_EXPORT StatsQuery& operator=(const StatsQuery& other);

    /*!
     * \brief Move assignment operator.
     */
    OMI_API_EXPORT StatsQuery& operator=(StatsQuery&& other);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the list of patterns within this query.
     */
    OMI_API_EXPORT const PatternArray& get_patterns() const;

    /*!
     * \brief Adds a new pattern to the list match strings within this query.
     */
    OMI_API_EXPORT void add_pattern(const arc::str::UTF8String& s);

    /*!
     * \brief Returns the result of this query.
     */
    OMI_API_EXPORT const Result& get_result() const;

    /*!
     * \brief Clears the matches patterns of this query and any current results.
     */
    OMI_API_EXPORT void clear();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    PatternArray m_patterns;
    Result m_result;
};

} // namespace report
} // namespace omi

#endif
