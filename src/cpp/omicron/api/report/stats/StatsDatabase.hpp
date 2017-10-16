/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_REPORT_STATS_STATSDATABASE_HPP_
#define OMICRON_API_REPORT_STATS_STATSDATABASE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class StatsQuery;

/*!
 * \brief Singleton object responsible for holding all of Omicron's various
 *        runtime statistics and providing methods for querying the stat of
 *        statistics.
 */
class StatsDatabase
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the StatsDatabase.
     */
    OMI_API_EXPORT static StatsDatabase* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Defines a new entry in the StatsDatabase.
     *
     * \param name The name of this statistic entry, hierarchical names
     *             separated by a . are recommended.
     * \param attr The attribute used to store the value of the statistic, this
     *             must be a valid mutable DataAttribute. Since attributes are
     *             reference count the original attribute can be held onto, and
     *             modified in order to update the statistic within the
     *             database.
     * \param description An optional description about what this statistic is
     *                    measuring.
     *
     * \throw arc::ex::KeyError If there is already a statistic with the given
     *                          name.
     * \throw arc::ex::ValueError If the attribute is not a valid mutable
     *                            attribute.
     */
    OMI_API_EXPORT void define_entry(
            const arc::str::UTF8String& name,
            omi::DataAttribute attr,
            const arc::str::UTF8String& description = "");

    /*!
     * \brief Returns the attribute for the statistic with the given name.
     *
     * Since attributes are reference counted the returned attribute can be
     * held onto and modified to update the statistic.
     *
     * \throw arc::ex::KeyError If there is no entry for the given name.
     */
    OMI_API_EXPORT const omi::DataAttribute& get_entry(
            const arc::str::UTF8String& name) const;

    /*!
     * \brief Returns the description of the statistic with the given name.
     *
     * If there is no description for the name, an empty string will be
     * returned.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_description(
            const arc::str::UTF8String& name) const;

    /*!
     * \brief Returns an array of all the names of the entries in the
     *        StatsDatabase.
     */
    OMI_API_EXPORT std::vector<arc::str::UTF8String> get_names() const;

    /*!
     * \brief Executes the given query on the StatsDatabase.
     */
    OMI_API_EXPORT void execute_query(StatsQuery& query) const;

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    StatsDatabase();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~StatsDatabase();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class StatsDatabaseImpl;
    StatsDatabaseImpl* m_impl;
};

} // namespace report
} // namespace omi

#endif
