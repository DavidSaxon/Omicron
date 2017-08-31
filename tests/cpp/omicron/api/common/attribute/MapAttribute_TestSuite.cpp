#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.MapAttribute)

#include <omicron/api/common/attribute/Int32Attribute.hpp>
#include <omicron/api/common/attribute/MapAttribute.hpp>


namespace
{

//------------------------------------------------------------------------------
//                                    FIXTURE
//------------------------------------------------------------------------------

class MapAttributeFixture : public arc::test::Fixture
{
public:

    //--------------------P U B L I C    A T T R I B U T E S--------------------

    const omi::MapAttribute::DataType empty_data;
    const std::vector<arc::str::UTF8String> empty_names;
    const std::vector<omi::Attribute> empty_attributes;

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    // convenience function to check whether to MapAttribute's data is equal
    bool compare_maps(
            const omi::MapAttribute::DataType& a,
            const omi::MapAttribute::DataType& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }
        for(auto entry : a)
        {
            auto f_entry = b.find(entry.first);
            if(f_entry == b.end())
            {
                return false;
            }
            if(entry.second != f_entry->second)
            {
                return false;
            }
        }
        return true;
    }
};

//------------------------------------------------------------------------------
//                              DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(default_constructor, MapAttributeFixture)
{
    omi::MapAttribute a;
    ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
    ARC_CHECK_TRUE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
    ARC_CHECK_EQUAL(a.get_size(), 0);
    ARC_CHECK_TRUE(fixture->compare_maps(a.get_data(), fixture->empty_data));
    ARC_CHECK_ITER_EQUAL(a.get_names(), fixture->empty_names);
    ARC_CHECK_ITER_EQUAL(a.get_attributes(), fixture->empty_attributes);
    ARC_CHECK_FALSE(a.has("test"));
    ARC_CHECK_THROW(
        a.get("test"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.insert("test", omi::Int32Attribute()),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.erase("test"),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.set_data(fixture->empty_data),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.clear(),
        arc::ex::IllegalActionError
    );
}

//------------------------------------------------------------------------------
//                              ITERATOR CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(iterator_constructor, MapAttributeFixture)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
    }
}

} // namespace anonymous
