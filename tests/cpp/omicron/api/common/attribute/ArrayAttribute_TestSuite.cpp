#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.ArrayAttribute)

#include <cassert>

#include <omicron/api/common/attribute/ArrayAttribute.hpp>
#include <omicron/api/common/attribute/BoolAttribute.hpp>
#include <omicron/api/common/attribute/ByteAttribute.hpp>
#include <omicron/api/common/attribute/DoubleAttribute.hpp>
#include <omicron/api/common/attribute/FloatAttribute.hpp>
#include <omicron/api/common/attribute/Int16Attribute.hpp>
#include <omicron/api/common/attribute/Int32Attribute.hpp>
#include <omicron/api/common/attribute/Int64Attribute.hpp>
#include <omicron/api/common/attribute/MapAttribute.hpp>
#include <omicron/api/common/attribute/PathAttribute.hpp>
#include <omicron/api/common/attribute/StringAttribute.hpp>

namespace
{

//------------------------------------------------------------------------------
//                              DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(default_constructor)
{
    omi::ArrayAttribute a;
    ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
    ARC_CHECK_TRUE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
    ARC_CHECK_EQUAL(a.get_size(), 0);
    ARC_CHECK_TRUE(a.is_empty());
    ARC_CHECK_ITER_EQUAL(a.get_values(), omi::ArrayAttribute::DataType());
    ARC_CHECK_THROW(
        a.get(0),
        arc::ex::IndexOutOfBoundsError
    );
    ARC_CHECK_THROW(
        a.front(),
        arc::ex::IndexOutOfBoundsError
    );
    ARC_CHECK_THROW(
        a.back(),
        arc::ex::IndexOutOfBoundsError
    );
    ARC_CHECK_THROW(
        a.set(0, omi::Int32Attribute(12)),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.push_back(omi::StringAttribute("Hello world!")),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.insert(0, omi::Int32Attribute(12)),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.erase(0),
        arc::ex::IllegalActionError
    );
    std::vector<omi::Attribute> test_data = {
        omi::ByteAttribute({'x', 'y'}, 1),
        omi::Int16Attribute(55)
    };
    ARC_CHECK_THROW(
        a.set_values(test_data.begin(), test_data.end()),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.set_values(test_data),
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

ARC_TEST_UNIT(iterator_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute a(data.begin(), data.end(), true);

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
        ARC_CHECK_EQUAL(a.get(0), data[0]);
        ARC_CHECK_EQUAL(a[1], data[1]);
        ARC_CHECK_EQUAL(a.get(2), data[2]);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.front(), data[0]);
        ARC_CHECK_EQUAL(a.back(), data[3]);
        ARC_CHECK_THROW(
            a.set(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.push_back(omi::StringAttribute("Hello world!")),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.insert(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::IllegalActionError
        );
        std::vector<omi::Attribute> test_data = {
            omi::ByteAttribute({'x', 'y', 'z'}, 3),
            omi::Int64Attribute(435435)
        };
        ARC_CHECK_THROW(
            a.set_values(test_data.begin(), test_data.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(test_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute a(data.begin(), data.end(), false);

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
        ARC_CHECK_EQUAL(a.get(0), data[0]);
        ARC_CHECK_EQUAL(a[1], data[1]);
        ARC_CHECK_EQUAL(a.get(2), data[2]);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.front(), data[0]);
        ARC_CHECK_EQUAL(a.back(), data[3]);

        omi::Int32Attribute int_attr(7);
        a.set(2, int_attr);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get(2), int_attr);

        omi::DoubleAttribute double_attr(-4.0);
        a.push_back(double_attr);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.get(4), double_attr);

        omi::FloatAttribute float_attr({3.0F, 0.0F}, 2);
        a.insert(1, float_attr);
        ARC_CHECK_EQUAL(a.get_size(), 6);
        ARC_CHECK_EQUAL(a.get(2), data[1]);
        ARC_CHECK_EQUAL(a[1], float_attr);

        a.erase(3);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[2], data[1]);
        ARC_CHECK_EQUAL(a[3], data[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);

        a.set_values(data.begin(), data.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
    }
}

//------------------------------------------------------------------------------
//                                COPY CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(copy_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute b(data, true);
        omi::ArrayAttribute a(b);

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
        ARC_CHECK_EQUAL(a.get(0), data[0]);
        ARC_CHECK_EQUAL(a[1], data[1]);
        ARC_CHECK_EQUAL(a.get(2), data[2]);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.front(), data[0]);
        ARC_CHECK_EQUAL(a.back(), data[3]);
        ARC_CHECK_THROW(
            a.set(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.push_back(omi::StringAttribute("Hello world!")),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.insert(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::IllegalActionError
        );
        std::vector<omi::Attribute> test_data = {
            omi::ByteAttribute({'x', 'y', 'z'}, 3),
            omi::Int64Attribute(435435)
        };
        ARC_CHECK_THROW(
            a.set_values(test_data.begin(), test_data.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(test_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute b(data.begin(), data.end(), false);
        omi::ArrayAttribute a(b);

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
        ARC_CHECK_EQUAL(a.get(0), data[0]);
        ARC_CHECK_EQUAL(a[1], data[1]);
        ARC_CHECK_EQUAL(a.get(2), data[2]);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.front(), data[0]);
        ARC_CHECK_EQUAL(a.back(), data[3]);

        omi::Int32Attribute int_attr(7);
        a.set(2, int_attr);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get(2), int_attr);

        omi::DoubleAttribute double_attr(-4.0);
        a.push_back(double_attr);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.get(4), double_attr);

        omi::FloatAttribute float_attr({3.0F, 0.0F}, 2);
        a.insert(1, float_attr);
        ARC_CHECK_EQUAL(a.get_size(), 6);
        ARC_CHECK_EQUAL(a.get(2), data[1]);
        ARC_CHECK_EQUAL(a[1], float_attr);

        a.erase(3);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[2], data[1]);
        ARC_CHECK_EQUAL(a[3], data[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);

        a.set_values(data.begin(), data.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
    }
}

//------------------------------------------------------------------------------
//                              ASSIGNMENT OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(assignment_operator)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute b(data, true);
        omi::ArrayAttribute a;

        a = b;

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data);
        ARC_CHECK_EQUAL(a.get(0), data[0]);
        ARC_CHECK_EQUAL(a[1], data[1]);
        ARC_CHECK_EQUAL(a.get(2), data[2]);
        ARC_CHECK_EQUAL(a[3], data[3]);
        ARC_CHECK_EQUAL(a.front(), data[0]);
        ARC_CHECK_EQUAL(a.back(), data[3]);
        ARC_CHECK_THROW(
            a.set(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.push_back(omi::StringAttribute("Hello world!")),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.insert(0, omi::Int32Attribute(12)),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::IllegalActionError
        );
        std::vector<omi::Attribute> test_data = {
            omi::ByteAttribute({'x', 'y', 'z'}, 3),
            omi::Int64Attribute(435435)
        };
        ARC_CHECK_THROW(
            a.set_values(test_data.begin(), test_data.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(test_data),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true),
            omi::Int64Attribute(65654)
        };


        omi::ArrayAttribute b(data1.begin(), data1.end(), false);
        omi::ArrayAttribute a(data2, true);
        a = b;

        ARC_CHECK_EQUAL(a.get_type(), omi::ArrayAttribute::kTypeArray);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data1);
        ARC_CHECK_EQUAL(a.get(0), data1[0]);
        ARC_CHECK_EQUAL(a[1], data1[1]);
        ARC_CHECK_EQUAL(a.get(2), data1[2]);
        ARC_CHECK_EQUAL(a[3], data1[3]);
        ARC_CHECK_EQUAL(a.front(), data1[0]);
        ARC_CHECK_EQUAL(a.back(), data1[3]);

        omi::Int32Attribute int_attr(7);
        a.set(2, int_attr);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get(2), int_attr);

        omi::DoubleAttribute double_attr(-4.0);
        a.push_back(double_attr);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[3], data1[3]);
        ARC_CHECK_EQUAL(a.get(4), double_attr);

        omi::FloatAttribute float_attr({3.0F, 0.0F}, 2);
        a.insert(1, float_attr);
        ARC_CHECK_EQUAL(a.get_size(), 6);
        ARC_CHECK_EQUAL(a.get(2), data1[1]);
        ARC_CHECK_EQUAL(a[1], float_attr);

        a.erase(3);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(a[2], data1[1]);
        ARC_CHECK_EQUAL(a[3], data1[3]);

        a.clear();
        ARC_CHECK_EQUAL(a.get_size(), 0);

        a.set_values(data1.begin(), data1.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_FALSE(a.is_empty());
        ARC_CHECK_ITER_EQUAL(a.get_values(), data1);
    }
}

//------------------------------------------------------------------------------
//                               EQUALITY OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(equality_operator)
{
    {
        omi::ArrayAttribute a;
        omi::Attribute b;
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
    }
    {
        omi::ArrayAttribute a;
        omi::ArrayAttribute b;
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
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b(data.begin(), data.end(), false);
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
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        std::vector<omi::Attribute> data2 = {
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute a(data1.begin(), data1.end(), true);
        omi::ArrayAttribute b(data2, true);
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
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, true),
            omi::Int16Attribute(12, false),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::ByteAttribute({'a', 'c'}, 1, true),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, false),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data1.begin(), data1.end(), false);
        omi::ArrayAttribute b(data2, false);
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
//                                     INVALD
//------------------------------------------------------------------------------

ARC_TEST_UNIT(invalid)
{
    {
        omi::Attribute b;
        omi::ArrayAttribute a(b);
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
            a.is_empty(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a[0],
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.front(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.back(),
            arc::ex::StateError
        );
        omi::Int32Attribute int_attr(12);
        ARC_CHECK_THROW(
            a.set(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.push_back(int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(omi::ArrayAttribute::DataType()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
    {
        omi::Int32Attribute b({1, 5, -8, 4}, 2, false);
        omi::ArrayAttribute a(b);

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
            a.is_empty(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a[0],
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.front(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.back(),
            arc::ex::StateError
        );
        omi::Int32Attribute int_attr(12);
        ARC_CHECK_THROW(
            a.set(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.push_back(int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(omi::ArrayAttribute::DataType()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
    {
        omi::MapAttribute::DataType map_data = {
            {"byte_key", omi::ByteAttribute('x')},
            {"int64_key", omi::Int64Attribute(4758257078353)}
        };

        omi::MapAttribute b(map_data);
        omi::ArrayAttribute a(b);

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
            a.is_empty(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a[0],
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.front(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.back(),
            arc::ex::StateError
        );
        omi::Int32Attribute int_attr(12);
        ARC_CHECK_THROW(
            a.set(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.push_back(int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(omi::ArrayAttribute::DataType()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::StateError
        );
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute b(data);
        omi::DataAttribute c(b);

        omi::ArrayAttribute a(c);

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
            a.is_empty(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a[0],
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.front(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.back(),
            arc::ex::StateError
        );
        omi::Int32Attribute int_attr(12);
        ARC_CHECK_THROW(
            a.set(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.push_back(int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.insert(0, int_attr),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.erase(0),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(omi::ArrayAttribute::DataType()),
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
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        omi::ArrayAttribute a;
        ARC_CHECK_EQUAL(a.get_hash(), a.get_hash());
        omi::ArrayAttribute b;
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::ArrayAttribute c = a.as_mutable();
        ARC_CHECK_EQUAL(a.get_hash(), c.get_hash());
        c.set_values(data);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), c.get_hash());
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true),
            omi::Int64Attribute(65654)
        };

        omi::ArrayAttribute a(data1, true);
        omi::ArrayAttribute b(data1, false);

        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(data2);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_values(data1);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::Int32Attribute int_attr(-4);
        b.push_back(int_attr);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        omi::ArrayAttribute c = a.as_mutable();
        ARC_CHECK_NOT_EQUAL(b.get_hash(), c.get_hash());
        c.set_values(data1);
        c.insert(data1.size(), int_attr);
        ARC_CHECK_EQUAL(b.get_hash(), c.get_hash());
        b.clear();
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        ARC_CHECK_NOT_EQUAL(b.get_hash(), c.get_hash());
    }

    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true),
            omi::Int64Attribute(65654),
            omi::ArrayAttribute(data1, false)
        };

        omi::ArrayAttribute a(data2, false);
        omi::ArrayAttribute b(data2, true);

        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::Attribute::Hash hash1 = a.get_hash();
        omi::Int32Attribute int_attr(-45);
        a.push_back(int_attr);
        omi::Attribute::Hash hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        omi::ArrayAttribute sub_array = a.get(2);
        ARC_CHECK_TRUE(sub_array.is_valid());
        sub_array.erase(1);
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        omi::StringAttribute str_attr = sub_array.get(2);
        str_attr.set_values({"Hello", "world"});
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        omi::ByteAttribute byte_attr = sub_array.get(0);
        byte_attr.set_tuple_size(2);
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        byte_attr.set_tuple_size(2);
        hash2 = a.get_hash();
        ARC_CHECK_EQUAL(hash1, hash2);

        sub_array.clear();
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
    }
    {
        std::vector<omi::Attribute> data0 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12),
            omi::FloatAttribute(3.0F),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::MapAttribute::DataType data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"array_key", omi::ArrayAttribute(data1, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, true)}
        };

        std::vector<omi::Attribute> data3 = {
            omi::BoolAttribute(true),
            omi::Int64Attribute(65654),
            omi::MapAttribute(data2, false)
        };

        omi::ArrayAttribute a(data3, false);
        omi::Attribute::Hash hash1 = a.get_hash();
        omi::Attribute::Hash hash2 = a.get_hash();
        ARC_CHECK_EQUAL(hash1, hash2);

        a.set(0, omi::BoolAttribute(false));
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        omi::MapAttribute sub_map = a.get(2);
        ARC_CHECK_TRUE(sub_map.is_valid());
        omi::ArrayAttribute sub_array = sub_map.get("array_key");
        ARC_CHECK_TRUE(sub_array.is_valid());

        omi::ByteAttribute byte_attr = sub_map.get("byte_key");
        byte_attr.set_values({'a', 'b'});
        hash1 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);

        omi::StringAttribute str_attr = sub_array.get(3);
        str_attr.set_tuple_size(0);
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
        str_attr.set_tuple_size(0);
        hash1 = a.get_hash();
        ARC_CHECK_EQUAL(hash1, hash2);

        sub_map.insert("array_key", omi::ArrayAttribute(data0, true));
        hash2 = a.get_hash();
        ARC_CHECK_NOT_EQUAL(hash1, hash2);
    }
}

//------------------------------------------------------------------------------
//                                 PURE IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(pure_immutable)
{
    {
        omi::ArrayAttribute a;
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::ArrayAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_immutable());
        omi::ArrayAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
        ARC_CHECK_TRUE(b[0].is_immutable());
        ARC_CHECK_TRUE(b[1].is_immutable());
        ARC_CHECK_TRUE(b[2].is_immutable());
        ARC_CHECK_TRUE(b[3].is_immutable());
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, true),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, true)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true),
            omi::Int64Attribute(65654, true),
            omi::ArrayAttribute(data1, true)
        };

        omi::ArrayAttribute a(data2, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::ArrayAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());

        omi::ArrayAttribute c(data2, false);
        ARC_CHECK_FALSE(c.is_immutable());
        ARC_CHECK_FALSE(c.is_pure_immutable());
        omi::ArrayAttribute d = c.as_pure_immutable();
        ARC_CHECK_TRUE(d.is_immutable());
        ARC_CHECK_TRUE(d.is_pure_immutable());
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, false),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::MapAttribute::DataType data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"array_key", omi::ArrayAttribute(data1, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)}
        };

        std::vector<omi::Attribute> data3 = {
            omi::BoolAttribute(true, true),
            omi::Int64Attribute(65654, true),
            omi::MapAttribute(data2, true)
        };

        omi::ArrayAttribute a(data3, true);
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_immutable());
        omi::ArrayAttribute b = a.as_pure_immutable();
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_immutable());
    }
}

//------------------------------------------------------------------------------
//                                  PURE MUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(pure_mutable)
{
    {
        omi::ArrayAttribute a;
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        omi::ArrayAttribute b = a.as_pure_mutable();
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_mutable());
        omi::ArrayAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_mutable());
        ARC_CHECK_FALSE(b[0].is_immutable());
        ARC_CHECK_FALSE(b[1].is_immutable());
        ARC_CHECK_FALSE(b[2].is_immutable());
        ARC_CHECK_FALSE(b[3].is_immutable());
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, false),
            omi::FloatAttribute(3.0F, false),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true, false),
            omi::Int64Attribute(65654, false),
            omi::ArrayAttribute(data1, false)
        };

        omi::ArrayAttribute a(data2, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());
        omi::ArrayAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());

        omi::ArrayAttribute c(data2, true);
        ARC_CHECK_TRUE(c.is_immutable());
        ARC_CHECK_FALSE(c.is_pure_mutable());
        omi::ArrayAttribute d = c.as_pure_mutable();
        ARC_CHECK_FALSE(d.is_immutable());
        ARC_CHECK_TRUE(d.is_pure_mutable());
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, false),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::MapAttribute::DataType data2 = {
            {"int32_key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)},
            {"byte_key", omi::ByteAttribute({'x', 'y'}, 0, false)},
            {"array_key", omi::ArrayAttribute(data1, false)},
            {"int16_key", omi::Int16Attribute({1, 5, -8, 4}, 4, false)}
        };

        std::vector<omi::Attribute> data3 = {
            omi::BoolAttribute(true, false),
            omi::Int64Attribute(65654, false),
            omi::MapAttribute(data2, false)
        };

        omi::ArrayAttribute a(data3, false);
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_FALSE(a.is_pure_mutable());
        omi::ArrayAttribute b = a.as_pure_mutable();
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(b.is_pure_mutable());
    }
}

//------------------------------------------------------------------------------
//                                  AS IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_immutable)
{
    {
        omi::ArrayAttribute a;
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            b.set(2, int_attr),
            arc::ex::IllegalActionError
        );
        a.set(2, int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            b.push_back(int_attr),
            arc::ex::IllegalActionError
        );
        a.push_back(int_attr);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(b.get_size(), 4);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            b.insert(1, int_attr),
            arc::ex::IllegalActionError
        );
        a.insert(1, int_attr);
        ARC_CHECK_EQUAL(a.get_size(), 5);
        ARC_CHECK_EQUAL(b.get_size(), 4);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.erase(2),
            arc::ex::IllegalActionError
        );
        a.erase(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_EQUAL(b.get_size(), 4);
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true, false),
            omi::Int64Attribute(65654, false),
            omi::ArrayAttribute(data1, false)
        };

        omi::ArrayAttribute a(data1, false);
        omi::ArrayAttribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_values(data2),
            arc::ex::IllegalActionError
        );
        a.set_values(data2.begin(), data2.end());
        ARC_CHECK_NOT_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_immutable();
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
    }
}

//------------------------------------------------------------------------------
//                                   AS MUTABLE
//------------------------------------------------------------------------------


ARC_TEST_UNIT(as_mutable)
{
    {
        omi::ArrayAttribute a;
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
        b.clear();
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        a.set(2, int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
        b.set(2, int_attr);
        ARC_CHECK_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            a.set(2, int_attr),
            arc::ex::IllegalActionError
        );
        b.set(2, int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            a.push_back(int_attr),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_EQUAL(a, b);
        b.push_back(int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 5);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        b.push_back(int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 5);
        a.push_back(int_attr);
        ARC_CHECK_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        ARC_CHECK_THROW(
            a.insert(1, int_attr),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_EQUAL(a, b);
        b.insert(1, int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 5);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        omi::Int32Attribute int_attr({77, -9}, 2);
        b.insert(1, int_attr);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 5);
        a.insert(1, int_attr);
        ARC_CHECK_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.erase(2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_EQUAL(a, b);
        b.erase(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 3);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.erase(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 3);
        a.erase(2);
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 3);
        ARC_CHECK_EQUAL(b.get_size(), 3);
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true, false),
            omi::Int64Attribute(65654, false),
            omi::ArrayAttribute(data1, false)
        };

        omi::ArrayAttribute a(data1, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.set_values(data2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_EQUAL(a, b);
        b.set_values(data2.begin(), data2.end());
        ARC_CHECK_NOT_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data1 = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        std::vector<omi::Attribute> data2 = {
            omi::BoolAttribute(true, false),
            omi::Int64Attribute(65654, false),
            omi::ArrayAttribute(data1, false)
        };

        omi::ArrayAttribute a(data1, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.set_values(data2.begin(), data2.end());
        ARC_CHECK_NOT_EQUAL(a, b);
        a.set_values(data2.begin(), data2.end());
        ARC_CHECK_EQUAL(a, b);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, true);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.clear(),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_EQUAL(a, b);
        b.clear();
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 0);
    }
    {
        std::vector<omi::Attribute> data = {
            omi::ByteAttribute({'a', 'b'}, 1, false),
            omi::Int16Attribute(12, true),
            omi::FloatAttribute(3.0F, true),
            omi::StringAttribute({"hello", "world"}, 2, false)
        };

        omi::ArrayAttribute a(data, false);
        omi::ArrayAttribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.clear();
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(b.get_size(), 0);
        a.clear();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_size(), 0);
        ARC_CHECK_EQUAL(b.get_size(), 0);
    }
}

} // namespace anonymous
