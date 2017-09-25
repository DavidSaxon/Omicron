#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.MapAttribute)

#include <cassert>

#include <omicron/api/common/attribute/ByteAttribute.hpp>
#include <omicron/api/common/attribute/DoubleAttribute.hpp>
#include <omicron/api/common/attribute/FloatAttribute.hpp>
#include <omicron/api/common/attribute/Int16Attribute.hpp>
#include <omicron/api/common/attribute/Int32Attribute.hpp>
#include <omicron/api/common/attribute/Int64Attribute.hpp>
#include <omicron/api/common/attribute/PathAttribute.hpp>
#include <omicron/api/common/attribute/StringAttribute.hpp>
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

    // convenience function to check whether the vectors contain the same data
    // (but does not enforce the ordering to be the same)
    template<typename T_DataType>
    bool compare_unordered(
            const std::vector<T_DataType>& a,
            const std::vector<T_DataType>& b)
    {
        // check sizes
        if(a.size() != b.size())
        {
            return false;
        }
        // build sets
        std::set<T_DataType> a_set(a.begin(), a.end());
        std::set<T_DataType> b_set(b.begin(), b.end());
        for(T_DataType v : a_set)
        {
            auto f_v = b_set.find(v);
            if(f_v == b_set.end())
            {
                return false;
            }
        }
        return true;
    }

    // convenience function build a map from a names vector and an attributes
    // vector
    omi::MapAttribute::DataType zip(
            const std::vector<arc::str::UTF8String>& names,
            const std::vector<omi::Attribute>& attributes)
    {
        assert(names.size() == attributes.size());
        omi::MapAttribute::DataType ret;
        for(std::size_t i = 0; i < names.size(); ++i)
        {
            ret.insert({names[i], attributes[i]});
        }
        return ret;
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
    ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), fixture->empty_data));
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
        a.set_values(fixture->empty_data),
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
        std::vector<arc::str::UTF8String> names =
            {"int_key", "byte_key", "float_key"};
        std::vector<omi::Attribute> attributes =
        {
            omi::Int32Attribute(4),
            omi::ByteAttribute('z'),
            omi::FloatAttribute(3.14F)
        };
        omi::MapAttribute::DataType data = fixture->zip(names, attributes);

        omi::MapAttribute a(data.begin(), data.end(), true);
        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("int_key"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("float_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("int_key"),   attributes[0]);
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes[1]);
        ARC_CHECK_EQUAL(a.get("float_key"), attributes[2]);
        ARC_CHECK_THROW(
            a.insert("test", omi::Int32Attribute()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(fixture->empty_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<arc::str::UTF8String> names1 =
            {"byte_key", "int64_key"};
        std::vector<omi::Attribute> attributes1 =
        {
            omi::ByteAttribute('x'),
            omi::Int64Attribute(4758257078353)
        };
        omi::MapAttribute::DataType data1 = fixture->zip(names1, attributes1);

        std::vector<arc::str::UTF8String> names2 =
            {"string_key", "int16_key", "path_key", "double_key"};
        arc::io::sys::Path test_path;
        test_path << "path" << "to" << "file";
        std::vector<omi::Attribute> attributes2 =
        {
            omi::StringAttribute("Hello World!"),
            omi::Int16Attribute(5),
            omi::PathAttribute(test_path),
            omi::DoubleAttribute(3.14)
        };
        omi::MapAttribute::DataType data2 = fixture->zip(names2, attributes2);

        omi::MapAttribute a(data1, false);
        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("int64_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes1[0]);
        ARC_CHECK_EQUAL(a.get("int64_key"), attributes1[1]);

        omi::ByteAttribute extra('g');
        a.insert("test", extra);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_FALSE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_FALSE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_FALSE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_TRUE(a.has("test"));
        ARC_CHECK_EQUAL(a.get("test"), extra);
        a.erase("byte_key");
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );

        a.set_values(data2.begin(), data2.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data2));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names2));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes2)
        );
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_FALSE(a.has("int64_key"));
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("string_key"));
        ARC_CHECK_TRUE(a.has("int16_key"));
        ARC_CHECK_TRUE(a.has("path_key"));
        ARC_CHECK_TRUE(a.has("double_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("int64_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("string_key"), attributes2[0]);
        ARC_CHECK_EQUAL(a.get("int16_key"),  attributes2[1]);
        ARC_CHECK_EQUAL(a.get("path_key"),   attributes2[2]);
        ARC_CHECK_EQUAL(a.get("double_key"), attributes2[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), fixture->empty_data));
        ARC_CHECK_ITER_EQUAL(a.get_names(), fixture->empty_names);
        ARC_CHECK_ITER_EQUAL(a.get_attributes(), fixture->empty_attributes);
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
    }
}

//------------------------------------------------------------------------------
//                                COPY CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(copy_constructor, MapAttributeFixture)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        std::vector<arc::str::UTF8String> names =
            {"int_key", "byte_key", "float_key"};
        std::vector<omi::Attribute> attributes =
        {
            omi::Int32Attribute(4),
            omi::ByteAttribute('6'),
            omi::FloatAttribute(3.14F)
        };
        omi::MapAttribute::DataType data = fixture->zip(names, attributes);

        omi::MapAttribute b(data.begin(), data.end(), true);
        omi::MapAttribute a(b);

        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("int_key"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("float_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("int_key"),   attributes[0]);
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes[1]);
        ARC_CHECK_EQUAL(a.get("float_key"), attributes[2]);
        ARC_CHECK_THROW(
            a.insert("test", omi::Int32Attribute()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(fixture->empty_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<arc::str::UTF8String> names1 =
            {"byte_key", "int64_key"};
        std::vector<omi::Attribute> attributes1 =
        {
            omi::ByteAttribute('x'),
            omi::Int64Attribute(4758257078353)
        };
        omi::MapAttribute::DataType data1 = fixture->zip(names1, attributes1);

        std::vector<arc::str::UTF8String> names2 =
            {"string_key", "int16_key", "path_key", "double_key"};
        arc::io::sys::Path test_path;
        test_path << "path" << "to" << "file";
        std::vector<omi::Attribute> attributes2 =
        {
            omi::StringAttribute("Hello World!"),
            omi::Int16Attribute(5),
            omi::PathAttribute(test_path),
            omi::DoubleAttribute(3.14)
        };
        omi::MapAttribute::DataType data2 = fixture->zip(names2, attributes2);

        omi::MapAttribute b(data1, false);
        omi::Attribute c(b);
        ARC_CHECK_EQUAL(c.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(c.is_valid());

        omi::MapAttribute a(c);
        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("int64_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes1[0]);
        ARC_CHECK_EQUAL(a.get("int64_key"), attributes1[1]);

        omi::ByteAttribute extra('h');
        a.insert("test", extra);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_FALSE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_FALSE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_FALSE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_TRUE(a.has("test"));
        ARC_CHECK_EQUAL(a.get("test"), extra);
        a.erase("byte_key");
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );

        a.set_values(data2.begin(), data2.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data2));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names2));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes2)
        );
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_FALSE(a.has("int64_key"));
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("string_key"));
        ARC_CHECK_TRUE(a.has("int16_key"));
        ARC_CHECK_TRUE(a.has("path_key"));
        ARC_CHECK_TRUE(a.has("double_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("int64_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("string_key"), attributes2[0]);
        ARC_CHECK_EQUAL(a.get("int16_key"),  attributes2[1]);
        ARC_CHECK_EQUAL(a.get("path_key"),   attributes2[2]);
        ARC_CHECK_EQUAL(a.get("double_key"), attributes2[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), fixture->empty_data));
        ARC_CHECK_ITER_EQUAL(a.get_names(), fixture->empty_names);
        ARC_CHECK_ITER_EQUAL(a.get_attributes(), fixture->empty_attributes);
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
    }
}

//------------------------------------------------------------------------------
//                              ASSIGNMENT OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(assignment_operator, MapAttributeFixture)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        std::vector<arc::str::UTF8String> names =
            {"int_key", "byte_key", "float_key"};
        std::vector<omi::Attribute> attributes =
        {
            omi::Int32Attribute(4),
            omi::ByteAttribute('d'),
            omi::FloatAttribute(3.14F)
        };
        omi::MapAttribute::DataType data = fixture->zip(names, attributes);

        omi::MapAttribute b(data.begin(), data.end(), true);
        omi::MapAttribute a;
        a = b;

        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("int_key"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("float_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("int_key"),   attributes[0]);
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes[1]);
        ARC_CHECK_EQUAL(a.get("float_key"), attributes[2]);
        ARC_CHECK_THROW(
            a.insert("test", omi::Int32Attribute()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(fixture->empty_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<arc::str::UTF8String> names1 =
            {"byte_key", "int64_key"};
        std::vector<omi::Attribute> attributes1 =
        {
            omi::ByteAttribute('x'),
            omi::Int64Attribute(4758257078353)
        };
        omi::MapAttribute::DataType data1 = fixture->zip(names1, attributes1);

        std::vector<arc::str::UTF8String> names2 =
            {"string_key", "int16_key", "path_key", "double_key"};
        arc::io::sys::Path test_path;
        test_path << "path" << "to" << "file";
        std::vector<omi::Attribute> attributes2 =
        {
            omi::StringAttribute("Hello World!"),
            omi::Int16Attribute(5),
            omi::PathAttribute(test_path),
            omi::DoubleAttribute(3.14)
        };
        omi::MapAttribute::DataType data2 = fixture->zip(names2, attributes2);

        omi::MapAttribute b(data1, false);
        omi::Attribute c(b);
        ARC_CHECK_EQUAL(c.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(c.is_valid());

        omi::MapAttribute a;
        a = c;
        ARC_CHECK_EQUAL(a.get_type(), omi::MapAttribute::kTypeMap);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("byte_key"));
        ARC_CHECK_TRUE(a.has("int64_key"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("byte_key"),  attributes1[0]);
        ARC_CHECK_EQUAL(a.get("int64_key"), attributes1[1]);

        omi::ByteAttribute extra('s');
        a.insert("test", extra);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_FALSE(fixture->compare_maps(a.get_values(), data1));
        ARC_CHECK_FALSE(fixture->compare_unordered(a.get_names(), names1));
        ARC_CHECK_FALSE(
            fixture->compare_unordered(a.get_attributes(), attributes1)
        );
        ARC_CHECK_TRUE(a.has("test"));
        ARC_CHECK_EQUAL(a.get("test"), extra);
        a.erase("byte_key");
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );

        a.set_values(data2.begin(), data2.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), data2));
        ARC_CHECK_TRUE(fixture->compare_unordered(a.get_names(), names2));
        ARC_CHECK_TRUE(
            fixture->compare_unordered(a.get_attributes(), attributes2)
        );
        ARC_CHECK_FALSE(a.has("byte_key"));
        ARC_CHECK_FALSE(a.has("int64_key"));
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_TRUE(a.has("string_key"));
        ARC_CHECK_TRUE(a.has("int16_key"));
        ARC_CHECK_TRUE(a.has("path_key"));
        ARC_CHECK_TRUE(a.has("double_key"));
        ARC_CHECK_THROW(
            a.get("byte_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("int64_key"),
            arc::ex::KeyError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
        ARC_CHECK_EQUAL(a.get("string_key"), attributes2[0]);
        ARC_CHECK_EQUAL(a.get("int16_key"),  attributes2[1]);
        ARC_CHECK_EQUAL(a.get("path_key"),   attributes2[2]);
        ARC_CHECK_EQUAL(a.get("double_key"), attributes2[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);
        ARC_CHECK_TRUE(fixture->compare_maps(a.get_values(), fixture->empty_data));
        ARC_CHECK_ITER_EQUAL(a.get_names(), fixture->empty_names);
        ARC_CHECK_ITER_EQUAL(a.get_attributes(), fixture->empty_attributes);
        ARC_CHECK_FALSE(a.has("test"));
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::KeyError
        );
    }
}

//------------------------------------------------------------------------------
//                               EQUALITY OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(equality_operator, MapAttributeFixture)
{
    {
        omi::MapAttribute a;
        omi::Attribute b;
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
    }
    {
        omi::MapAttribute a;
        omi::MapAttribute b;
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_TRUE(b == c);
        ARC_CHECK_FALSE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_TRUE(c == d);
        ARC_CHECK_FALSE(c != d);
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data, true);
        omi::MapAttribute b(map_data.begin(), map_data.end(), false);
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_TRUE(b == c);
        ARC_CHECK_FALSE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_TRUE(c == d);
        ARC_CHECK_FALSE(c != d);
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };
        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1.begin(), map_data1.end(), true);
        omi::MapAttribute b(map_data2, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };
        omi::MapAttribute::DataType map_data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"different_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1.begin(), map_data1.end(), false);
        omi::MapAttribute b(map_data2, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };
        omi::MapAttribute::DataType map_data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 0}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1.begin(), map_data1.end(), false);
        omi::MapAttribute b(map_data2, false);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
    }
}

//------------------------------------------------------------------------------
//                                    INVALID
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(invalid, MapAttributeFixture)
{
    {
        omi::Attribute b;
        omi::MapAttribute a(b);
        ARC_CHECK_FALSE(a.is_valid());
        ARC_CHECK_THROW(
            a.as_immutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.as_mutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_names(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_attributes(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.has("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert("test", omi::ByteAttribute('j')),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::StateError
        );

        omi::MapAttribute::DataType new_data = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };
        ARC_CHECK_THROW(
            a.set_values(new_data.begin(), new_data.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_data),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
    {
        omi::Int32Attribute b({1, 5, -8, 4}, 2, false);
        omi::MapAttribute a(b);
        ARC_CHECK_FALSE(a.is_valid());
        ARC_CHECK_THROW(
            a.as_immutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.as_mutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_names(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_attributes(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.has("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert("test", omi::ByteAttribute('j')),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::StateError
        );

        omi::MapAttribute::DataType new_data = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };
        ARC_CHECK_THROW(
            a.set_values(new_data.begin(), new_data.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_data),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };
        omi::MapAttribute b(map_data, false);
        omi::DataAttribute c(b);

        omi::MapAttribute a(c);
        ARC_CHECK_FALSE(a.is_valid());
        ARC_CHECK_THROW(
            a.as_immutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.as_mutable(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_names(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_attributes(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.has("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get("test"),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert("test", omi::ByteAttribute('y')),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase("test"),
            arc::ex::StateError
        );

        omi::MapAttribute::DataType new_data = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };
        ARC_CHECK_THROW(
            a.set_values(new_data.begin(), new_data.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_data),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
}

//------------------------------------------------------------------------------
//                                      HASH
//------------------------------------------------------------------------------

ARC_TEST_UNIT(hash)
{
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a;
        ARC_CHECK_EQUAL(a.get_hash(), a.get_hash());
        omi::MapAttribute b;
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::MapAttribute c = a.as_mutable();
        ARC_CHECK_EQUAL(a.get_hash(), c.get_hash());
        c.set_values(map_data1);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), c.get_hash());
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", true)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute a(map_data1, true);
        omi::MapAttribute b(map_data1, false);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(map_data2);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(map_data1);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        b.erase("byte_key");
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(map_data1);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        b.insert("byte_key", omi::ByteAttribute('x'));
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(map_data1);
        b.insert("string_key", omi::StringAttribute("Hello world", false));
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::StringAttribute str_attr = b["string_key"];
        str_attr.set_value("different!");
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
    }
    {
        omi::MapAttribute::DataType map_data0 = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x', false)},
            {"int64_key", omi::Int64Attribute(4758257078353)},
            {"map_key", omi::MapAttribute(map_data1, false)}
        };

        omi::MapAttribute::DataType map_data3 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"map_key", omi::MapAttribute(map_data2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
        };

        omi::MapAttribute a(map_data3, false);
        omi::MapAttribute b(map_data3, true);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::Attribute::Hash hash1 = a.get_hash();
        a.insert("string_key", omi::StringAttribute({"hello", "world"}, 0));
        omi::Attribute::Hash hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        a.erase("map_key.int64_key");
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        omi::ByteAttribute byte_attr = a["map_key.byte_key"];
        byte_attr.set_values({'x', 'y'});
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        omi::StringAttribute str_attr = a["map_key.map_key.string_key"];
        str_attr.set_tuple_size(2);
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        str_attr.set_tuple_size(2);
        hash2 = a.get_hash();
        ARC_CHECK_EQUAL(hash1, hash2);
        omi::MapAttribute sub_map = a["map_key.map_key"];
        sub_map.set_values(map_data0);
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        a.insert("map_key.map_key", omi::MapAttribute(map_data1, false));
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        sub_map = a["map_key.map_key"];
        sub_map.clear();
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        hash2 = a.get_hash();
        ARC_CHECK_EQUAL(hash1, hash2);
    }
}

//------------------------------------------------------------------------------
//                                 PURE IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(pure_immutable, MapAttributeFixture)
{
    {
        omi::MapAttribute a;
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::MapAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };
        omi::MapAttribute a(map_data, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_immutable());
        omi::MapAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
        ARC_CHECK_TRUE(b["int32_key"].is_immutable());
        ARC_CHECK_TRUE(b["byte_key"].is_immutable());
        ARC_CHECK_TRUE(b["int16_key"].is_immutable());
        ARC_CHECK_TRUE(b["string_key"].is_immutable());
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", true)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x', true)},
            {"int64_key", omi::Int64Attribute(4758257078353)},
            {"map_key", omi::MapAttribute(map_data1, true)}
        };

        omi::MapAttribute::DataType map_data3 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"map_key", omi::MapAttribute(map_data2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
        };

        omi::MapAttribute a(map_data3, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::MapAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());

        omi::MapAttribute c(map_data3, false);
        ARC_CHECK_FALSE(c.is_immutable());
        ARC_CHECK_FALSE(c.is_pure_immutable());
        omi::MapAttribute d = c.as_pure_immutable();
        ARC_CHECK_TRUE(d.is_immutable());
        ARC_CHECK_TRUE(d.is_pure_immutable());
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x', true)},
            {"int64_key", omi::Int64Attribute(4758257078353, false)},
            {"map_key", omi::MapAttribute(map_data1, true)}
        };

        omi::MapAttribute::DataType map_data3 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, false)},
            {"map_key", omi::MapAttribute(map_data2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)}
        };

        omi::MapAttribute a(map_data3, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_immutable());
        omi::MapAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
        ARC_CHECK_TRUE(b["int32_key"].is_immutable());
        ARC_CHECK_TRUE(b["map_key"].is_immutable());
        ARC_CHECK_TRUE(b["map_key.int64_key"].is_immutable());
        ARC_CHECK_TRUE(b["map_key.map_key"].is_immutable());
        ARC_CHECK_TRUE(b["map_key.map_key.byte_key"].is_immutable());
        ARC_CHECK_TRUE(b["map_key.map_key.string_key"].is_immutable());

    }
}

//------------------------------------------------------------------------------
//                                  PURE MUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(pure_mutable, MapAttributeFixture)
{
    {
        omi::MapAttribute a;
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::MapAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_mutable());
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)},
            {"string_key", omi::StringAttribute("Hello world", true)}
        };
        omi::MapAttribute a(map_data, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_mutable());
        omi::MapAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_mutable());
        ARC_CHECK_FALSE(b["int32_key"].is_immutable());
        ARC_CHECK_FALSE(b["byte_key"].is_immutable());
        ARC_CHECK_FALSE(b["int16_key"].is_immutable());
        ARC_CHECK_FALSE(b["string_key"].is_immutable());
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x', false)},
            {"int64_key", omi::Int64Attribute(4758257078353, false)},
            {"map_key", omi::MapAttribute(map_data1, false)}
        };

        omi::MapAttribute::DataType map_data3 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, false)},
            {"map_key", omi::MapAttribute(map_data2, false)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)}
        };

        omi::MapAttribute a(map_data3, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());
        omi::MapAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());

        omi::MapAttribute c(map_data3, true);
        ARC_CHECK_TRUE(c.is_immutable());
        ARC_CHECK_FALSE(c.is_pure_mutable());
        omi::MapAttribute d = c.as_pure_mutable();
        ARC_CHECK_FALSE(d.is_immutable());
        ARC_CHECK_TRUE(d.is_pure_mutable());
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", true)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"byte_key", omi::ByteAttribute('x', false)},
            {"int64_key", omi::Int64Attribute(4758257078353, true)},
            {"map_key", omi::MapAttribute(map_data1, false)}
        };

        omi::MapAttribute::DataType map_data3 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"map_key", omi::MapAttribute(map_data2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
        };

        omi::MapAttribute a(map_data3, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_mutable());
        omi::MapAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_mutable());
        ARC_CHECK_FALSE(b["int32_key"].is_immutable());
        ARC_CHECK_FALSE(b["map_key"].is_immutable());
        ARC_CHECK_FALSE(b["map_key.int64_key"].is_immutable());
        ARC_CHECK_FALSE(b["map_key.map_key"].is_immutable());
        ARC_CHECK_FALSE(b["map_key.map_key.byte_key"].is_immutable());
        ARC_CHECK_FALSE(b["map_key.map_key.string_key"].is_immutable());
    }
}

//------------------------------------------------------------------------------
//                                      HAS
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(has, MapAttributeFixture)
{
    omi::MapAttribute::DataType map_data1 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
        {"string_key", omi::StringAttribute("Hello world", false)}
    };

    omi::MapAttribute::DataType map_data2 = {
        {"byte_key", omi::ByteAttribute('x')},
        {"int64_key", omi::Int64Attribute(4758257078353)},
        {"map_key", omi::MapAttribute(map_data1, false)}
    };

    omi::MapAttribute::DataType map_data3 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"map_key", omi::MapAttribute(map_data2)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
    };

    omi::MapAttribute a(map_data3, false);

    ARC_CHECK_TRUE(a.has("int32_key"));
    ARC_CHECK_TRUE(a.has("map_key"));
    ARC_CHECK_FALSE(a.has("int64_key"));
    ARC_CHECK_FALSE(a.has("int32_key.byte_key"));
    ARC_CHECK_TRUE(a.has("map_key.byte_key"));
    ARC_CHECK_TRUE(a.has("map_key.int64_key"));
    ARC_CHECK_TRUE(a.has("map_key.map_key"));
    ARC_CHECK_FALSE(a.has("map_key.int32_key"));
    ARC_CHECK_FALSE(a.has("map_key.string_key"));
    ARC_CHECK_FALSE(a.has("map_key.byte_key.int32_key"));
    ARC_CHECK_TRUE(a.has("map_key.map_key.int16_key"));
    ARC_CHECK_TRUE(a.has("map_key.map_key.string_key"));
    ARC_CHECK_FALSE(a.has("map_key.map_key.int64_key"));
    ARC_CHECK_FALSE(a.has("map_key.map_key.int32_key.byte_key"));
}

//------------------------------------------------------------------------------
//                                      GET
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(get, MapAttributeFixture)
{
    omi::MapAttribute::DataType map_data1 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
        {"string_key", omi::StringAttribute("Hello world", false)}
    };

    omi::MapAttribute::DataType map_data2 = {
        {"byte_key", omi::ByteAttribute('x')},
        {"int64_key", omi::Int64Attribute(4758257078353)},
        {"map_key", omi::MapAttribute(map_data1, false)}
    };

    omi::MapAttribute::DataType map_data3 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"map_key", omi::MapAttribute(map_data2)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
    };

    omi::MapAttribute a(map_data3, true);

    ARC_CHECK_EQUAL(a.get("int32_key"), map_data3["int32_key"]);
    ARC_CHECK_EQUAL(a.get("map_key"), map_data3["map_key"]);
    ARC_CHECK_THROW(
        a.get("int64_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.get("int64_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.get("int32_key.byte_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_EQUAL(a.get("map_key.byte_key"), map_data2["byte_key"]);
    ARC_CHECK_EQUAL(a.get("map_key.int64_key"), map_data2["int64_key"]);
    ARC_CHECK_EQUAL(a.get("map_key.map_key"), map_data2["map_key"]);
    ARC_CHECK_THROW(
        a.get("map_key.int32_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.get("map_key.string_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.get("map_key.byte_key.int32_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_EQUAL(a.get("map_key.map_key.int16_key"), map_data1["int16_key"]);
    ARC_CHECK_EQUAL(a.get("map_key.map_key.int16_key"), map_data1["int16_key"]);
    ARC_CHECK_EQUAL(
        a.get("map_key.map_key.string_key"),
        map_data1["string_key"]
    );
    ARC_CHECK_THROW(
        a.get("map_key.map_key.int64_key"),
        arc::ex::KeyError
    );
    ARC_CHECK_THROW(
        a.get("map_key.map_key.int32_key.byte_key"),
        arc::ex::KeyError
    );
}

//------------------------------------------------------------------------------
//                                     INSERT
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(insert, MapAttributeFixture)
{
    omi::MapAttribute::DataType map_data1 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
    };

    omi::MapAttribute::DataType map_data2 = {
        {"byte_key", omi::ByteAttribute('x')},
        {"int64_key", omi::Int64Attribute(4758257078353)}
    };

    omi::MapAttribute::DataType map_data3 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"map_key", omi::MapAttribute(map_data2, false)},
        {"map_imm_key", omi::MapAttribute(map_data2, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
    };

    omi::MapAttribute a(map_data3, false);

    ARC_CHECK_FALSE(a.has("byte_key"));
    omi::ByteAttribute byte_attr({'x', 'y'}, 0, true);
    a.insert("byte_key", byte_attr);
    ARC_CHECK_TRUE(a.has("byte_key"));
    ARC_CHECK_EQUAL(a.get("byte_key"), byte_attr);

    ARC_CHECK_THROW(
        a.insert("map_imm_key.byte_key", byte_attr),
        arc::ex::IllegalActionError
    )

    ARC_CHECK_TRUE(a.has("map_key.byte_key"));
    ARC_CHECK_NOT_EQUAL(a.get("map_key.byte_key"), byte_attr);
    a.insert("map_key.byte_key", byte_attr);
    ARC_CHECK_TRUE(a.has("map_key.byte_key"));
    ARC_CHECK_EQUAL(a.get("map_key.byte_key"), byte_attr);

    omi::MapAttribute sub_map(map_data1, true);
    ARC_CHECK_FALSE(a.has("map_key.map_imm_key"));
    a.insert("map_key.map_imm_key", sub_map);
    ARC_CHECK_TRUE(a.has("map_key.map_imm_key"));
    ARC_CHECK_EQUAL(a.get("map_key.map_imm_key"), sub_map);

    ARC_CHECK_FALSE(a.has("map_key.map_key"));
    a.insert("map_key.map_key", sub_map.as_mutable());
    ARC_CHECK_TRUE(a.has("map_key.map_key"));
    ARC_CHECK_EQUAL(a.get("map_key.map_key"), sub_map);
    ARC_CHECK_FALSE(a.get("map_key.map_key").is_immutable());

    omi::StringAttribute str_attr("Hello world", false);
    ARC_CHECK_THROW(
        a.insert("map_key.map_imm_key.string_key", str_attr),
        arc::ex::IllegalActionError
    )

    ARC_CHECK_FALSE(a.has("map_key.map_key.string_key"));
    a.insert("map_key.map_key.string_key", str_attr);
    ARC_CHECK_TRUE(a.has("map_key.map_key.string_key"));
    ARC_CHECK_EQUAL(a.get("map_key.map_key.string_key"), str_attr);
}

//------------------------------------------------------------------------------
//                                     ERASE
//------------------------------------------------------------------------------

ARC_TEST_UNIT_FIXTURE(erase, MapAttributeFixture)
{
    omi::MapAttribute::DataType map_data1 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
        {"string_key", omi::StringAttribute("Hello world", false)}
    };

    omi::MapAttribute::DataType map_data2 = {
        {"byte_key", omi::ByteAttribute('x')},
        {"int64_key", omi::Int64Attribute(4758257078353)},
        {"map_key", omi::MapAttribute(map_data1, false)},
        {"map_imm_key", omi::MapAttribute(map_data1, true)}
    };

    omi::MapAttribute::DataType map_data3 = {
        {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
        {"map_key", omi::MapAttribute(map_data2, false)},
        {"map_imm_key", omi::MapAttribute(map_data2, true)},
        {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
        {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
    };

    omi::MapAttribute a(map_data3, false);

    ARC_CHECK_THROW(
        a.erase("string_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_TRUE(a.has("int32_key"));
    a.erase("int32_key");
    ARC_CHECK_FALSE(a.has("int32_key"));
    ARC_CHECK_THROW(
        a.get("int32_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_THROW(
        a.erase("map_imm_key.byte_key"),
        arc::ex::IllegalActionError
    );

    ARC_CHECK_THROW(
        a.erase("map_key.int32_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_TRUE(a.has("map_key.byte_key"));
    a.erase("map_key.byte_key");
    ARC_CHECK_FALSE(a.has("map_key.byte_key"));
    ARC_CHECK_THROW(
        a.get("map_key.byte_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_THROW(
        a.erase("map_key.map_imm_key.int16_key"),
        arc::ex::IllegalActionError
    );

    ARC_CHECK_THROW(
        a.erase("map_key.map_key.map_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_TRUE(a.has("map_key.map_key.int16_key"));
    a.erase("map_key.map_key.int16_key");
    ARC_CHECK_FALSE(a.has("map_key.map_key.int16_key"));
    ARC_CHECK_THROW(
        a.get("map_key.map_key.int16_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_TRUE(a.has("map_key.map_imm_key"));
    a.erase("map_key.map_imm_key");
    ARC_CHECK_FALSE(a.has("map_key.map_imm_key"));
    ARC_CHECK_THROW(
        a.get("map_key.map_imm_key"),
        arc::ex::KeyError
    );

    ARC_CHECK_TRUE(a.has("map_key.map_key"));
    a.erase("map_key.map_key");
    ARC_CHECK_FALSE(a.has("map_key.map_key"));
    ARC_CHECK_THROW(
        a.get("map_key.map_key"),
        arc::ex::KeyError
    );
}

//------------------------------------------------------------------------------
//                                  AS IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_immutable)
{
    {
        omi::MapAttribute a;
        omi::MapAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data, false);
        omi::MapAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.erase("string_key"),
            arc::ex::IllegalActionError
        );
        a.erase("string_key");
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_EQUAL(b.get_size(), 4);
        ARC_CHECK_FALSE(a.has("string_key"));
        ARC_CHECK_TRUE(b.has("string_key"));
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1, false);
        omi::MapAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_values(map_data2),
            arc::ex::IllegalActionError
        );
        a.set_values(map_data2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 2);
        ARC_CHECK_FALSE(a.has("int64_key"));
        ARC_CHECK_TRUE(b.has("int64_key"));
        ARC_CHECK_TRUE(a.has("int16_key"));
        ARC_CHECK_FALSE(b.has("int16_key"));
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data, false);
        omi::MapAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.clear(),
            arc::ex::IllegalActionError
        );
        a.clear();
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 0);
        ARC_CHECK_EQUAL(b.get_size(), 4);
        ARC_CHECK_FALSE(a.has("string_key"));
        ARC_CHECK_TRUE(b.has("string_key"));
    }
}

//------------------------------------------------------------------------------
//                                   AS MUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_mutable)
{
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data, false);
        omi::MapAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.erase("string_key");
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 3);
        ARC_CHECK_TRUE(a.has("string_key"));
        ARC_CHECK_FALSE(b.has("string_key"));
        a.erase("int32_key");
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_EQUAL(b.get_size(), 3);
        ARC_CHECK_FALSE(a.has("int32_key"));
        ARC_CHECK_TRUE(b.has("int32_key"));
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data, true);
        omi::MapAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.erase("string_key");
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 3);
        ARC_CHECK_TRUE(a.has("string_key"));
        ARC_CHECK_FALSE(b.has("string_key"));
        ARC_CHECK_THROW(
            a.erase("int32_key"),
            arc::ex::IllegalActionError
        );
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1, false);
        omi::MapAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.set_values(map_data2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_EQUAL(b.get_size(), 4);
        a.set_values(map_data2);
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 4);
    }
    {
        omi::MapAttribute::DataType map_data1 = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute::DataType map_data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, true)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)},
            {"string_key", omi::StringAttribute("Hello world", false)}
        };

        omi::MapAttribute a(map_data1, true);
        omi::MapAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.set_values(map_data2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 2);
        ARC_CHECK_EQUAL(b.get_size(), 4);
        ARC_CHECK_THROW(
            a.set_values(map_data2),
            arc::ex::IllegalActionError
        );
    }
}

} // namespace anonymous
