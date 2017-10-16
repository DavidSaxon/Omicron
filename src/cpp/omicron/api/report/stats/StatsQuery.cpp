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

OMI_API_EXPORT StatsQuery::StatsQuery()
{
}

OMI_API_EXPORT StatsQuery::StatsQuery(const arc::io::sys::Path& path)
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
    m_patterns = PatternArray(lines.begin(), lines.end());
}

OMI_API_EXPORT StatsQuery::StatsQuery(const StatsQuery& other)
    : m_patterns(other.m_patterns)
    , m_result  (other.m_result)
{
}

OMI_API_EXPORT StatsQuery::StatsQuery(StatsQuery&& other)
    : m_patterns(other.m_patterns)
    , m_result  (other.m_result)
{
    other.m_patterns = PatternArray();
    other.m_result   = Result();
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT StatsQuery::~StatsQuery()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_EXPORT StatsQuery& StatsQuery::operator=(const StatsQuery& other)
{
    m_patterns = other.m_patterns;
    m_result   = other.m_result;
    return *this;
}

OMI_API_EXPORT StatsQuery& StatsQuery::operator=(StatsQuery&& other)
{
    m_patterns = other.m_patterns;
    m_result   = other.m_result;
    other.m_patterns = PatternArray();
    other.m_result   = Result();
    return *this;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT const StatsQuery::PatternArray& StatsQuery::get_patterns() const
{
    return m_patterns;
}

OMI_API_EXPORT void StatsQuery::add_pattern(const arc::str::UTF8String& s)
{
    m_patterns.push_back(s);
}

OMI_API_EXPORT const StatsQuery::Result& StatsQuery::get_result() const
{
    return m_result;
}

OMI_API_EXPORT void StatsQuery::clear()
{
    m_patterns.clear();
    m_result.clear();
}

} // namespace report
} // namespace omi
