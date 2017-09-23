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

// TODO: DOC
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
    OMI_API_GLOBAL static StatsDatabase* instance();

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
     *
     * \throw arc::ex::KeyError If there is already a statistic with the given
     *                          name.
     * \throw arc::ex::ValueError If the attribute is not a valid mutable
     *                            attribute.
     */
    OMI_API_GLOBAL void define_entry(
            const arc::str::UTF8String& name,
            omi::DataAttribute attr);

    /*!
     * \brief Returns the attribute for the statistic with the given name.
     *
     * Since attributes are reference counted the returned attribute can be
     * held onto and modified to update the statistic.
     *
     * \throw arc::ex::KeyError If there is no entry for the given name.
     */
    OMI_API_GLOBAL const omi::DataAttribute& get_entry(
            const arc::str::UTF8String& name) const;

    // TOOD: get names

    /*!
     * \brief Executes the given query on the StatsDatabase.
     */
    OMI_API_GLOBAL void execute_query(StatsQuery& query) const;


    // TODO: text dump

    // TODO:

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
