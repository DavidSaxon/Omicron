#include "omicron/api/report/stats/StatsOperations.hpp"

#include <algorithm>
#include <iostream>

#include "omicron/api/report/stats/StatsQuery.hpp"


namespace omi
{
namespace report
{

namespace
{

// structure for sorting the names alphabetically and hierarchically
struct StatOrderer
{
    std::unordered_map<arc::str::UTF8String, StatOrderer*> children;

    ~StatOrderer()
    {
        for(auto child : children)
        {
            delete child.second;
        }
    }
};


// TODO: DOC

// Recursively sorts the list of hierarchal statistic names into a hierarchal
// data structure
static void sort_stats_hierarchically(
        const std::vector<arc::str::UTF8String>& components,
        std::size_t offset,
        std::unordered_map<arc::str::UTF8String, StatOrderer*>& out_sorted,
        std::size_t& max_indent)
{
    // get the component to sort
    arc::str::UTF8String name = components[offset];
    // update max indent?
    std::size_t indent = (offset * 4) + name.get_length() + 4;
    if(indent < 78 && indent > max_indent)
    {
        max_indent = indent;
    }
    // is it in the sorted map already?
    auto f_sorted = out_sorted.find(name);
    if(f_sorted == out_sorted.end())
    {
        // StatOrderer
        out_sorted.insert(std::make_pair(name, new StatOrderer()));
        f_sorted = out_sorted.find(name);
    }
    // recurse?
    ++offset;
    if(offset < components.size())
    {
        sort_stats_hierarchically(
            components,
            offset,
            f_sorted->second->children,
            max_indent
        );
    }
}

// Recursively writes the given hierarchal data structure to a string
static void hierarchy_to_string(
        const StatsQuery& query,
        const std::unordered_map<arc::str::UTF8String, StatOrderer*>& roots,
        std::size_t indent,
        std::size_t value_indent,
        const arc::str::UTF8String& hierarchy_name,
        arc::str::UTF8String& out_str)
{
    // alphabetically order the current names
    std::vector<arc::str::UTF8String> sorted;
    sorted.reserve(roots.size());
    for(auto entry : roots)
    {
        sorted.push_back(entry.first);
    }
    std::sort(sorted.begin(), sorted.end());

    // create the indentation string
    arc::str::UTF8String indent_str = " ";
    indent_str *= indent;

    // write to the string and recurse
    for(const arc::str::UTF8String& name : sorted)
    {
        // build the hierarchy name
        arc::str::UTF8String next_name = hierarchy_name;
        if(!next_name.is_empty())
        {
            next_name += ".";
        }
        next_name += name;

        // print the stat name
        out_str << "\t" << indent_str;
        StatOrderer* p = roots.at(name);
        if(p->children.empty())
        {
            out_str << "-";
        }
        else
        {
            out_str << ">";
        }
        out_str << " " << name;

        // print the stat value
        auto f_stat = query.get_result().find(next_name);
        if(f_stat != query.get_result().end())
        {
            // how much to indent by
            std::size_t current_indent = indent + 3 + name.get_length();
            std::size_t indent_by = 1;
            if(current_indent < value_indent)
            {
                indent_by = value_indent - current_indent;
            }
            // build the string
            arc::str::UTF8String value_indent_str = " ";
            value_indent_str *= indent_by;
            // print the value
            out_str << ":" << value_indent_str << f_stat->second;
        }
        out_str << "\n";
        // print children?
        if(!p->children.empty())
        {
            hierarchy_to_string(
                query,
                p->children,
                indent + 4,
                value_indent,
                next_name,
                out_str
            );
        }
    }
}

} // namespace anonymous

OMI_API_EXPORT void print_stats_query(
        const StatsQuery& query,
        std::ostream& out_stream,
        const arc::str::UTF8String& title)
{
    std::size_t value_indent = 0;
    // sort the stats hierarchically
    std::unordered_map<arc::str::UTF8String, StatOrderer*> roots;
    for(auto stat : query.get_result())
    {
        std::vector<arc::str::UTF8String> components =
            stat.first.split(".");
        // TOOD: get largest indent here
        sort_stats_hierarchically(components, 0, roots, value_indent);
    }

    // recurse the hierarchy and print in alphabetical order
    arc::str::UTF8String content;
    hierarchy_to_string(query, roots, 0, value_indent, "", content);

    // clean up
    for(auto root : roots)
    {
        delete root.second;
    }
    roots.clear();

    // build the header string
    arc::str::UTF8String header = "-";
    header *= 80;

    // build the string to print
    arc::str::UTF8String s;
    s << "\n\t" << header << "\n";
    if(!title.is_empty())
    {
        s << "\t" << title << "\n\t" << header << "\n";
    }
    s << content;
    s << "\t" << header;

    out_stream << s << std::endl;
}

} // namespace report
} // namespace omi
