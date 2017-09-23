#include "omicron/api/report/stats/StatsQuery.hpp"

#include <list>

#include <arcanecore/io/sys/FileReader.hpp>


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL StatsQuery::StatsQuery()
{
}

OMI_API_GLOBAL StatsQuery::StatsQuery(const arc::io::sys::Path& path)
{
    // open a reader to the file
    arc::io::sys::FileReader reader(path);
    // read in to a std::list first for efficiency
    std::list<arc::str::UTF8String> lines;
    while(!reader.eof())
    {
        arc::str::UTF8String line;
        reader.read_line(line);
        if(!line.is_empty() && !line.starts_with("//"))
        {
            lines.push_back(line);
        }
    }
    // assign to the match strings
    m_strings = MatchStrings(lines.begin(), lines.end());
}

OMI_API_GLOBAL StatsQuery::StatsQuery(const StatsQuery& other)
    : m_strings(other.m_strings)
    , m_result (other.m_result)
{
}

OMI_API_GLOBAL StatsQuery::StatsQuery(StatsQuery&& other)
    : m_strings(other.m_strings)
    , m_result (other.m_result)
{
    other.m_strings = MatchStrings();
    other.m_result  = Result();
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL StatsQuery::~StatsQuery()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL StatsQuery& StatsQuery::operator=(const StatsQuery& other)
{
    m_strings = other.m_strings;
    m_result  = other.m_result;
    return *this;
}

OMI_API_GLOBAL StatsQuery& StatsQuery::operator=(StatsQuery&& other)
{
    m_strings = other.m_strings;
    m_result  = other.m_result;
    other.m_strings = MatchStrings();
    other.m_result  = Result();
    return *this;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL const StatsQuery::MatchStrings& StatsQuery::get_strings() const
{
    return m_strings;
}

OMI_API_GLOBAL void StatsQuery::add(const arc::str::UTF8String& s)
{
    m_strings.push_back(s);
}

OMI_API_GLOBAL const StatsQuery::Result& StatsQuery::get_result() const
{
    return m_result;
}

OMI_API_GLOBAL void StatsQuery::clear()
{
    m_strings.clear();
    m_result.clear();
}

} // namespace report
} // namespace omi
