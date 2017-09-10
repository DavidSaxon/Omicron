#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.Int32Attribute)

#include <omicron/api/common/attribute/FloatAttribute.hpp>
#include <omicron/api/common/attribute/Int32Attribute.hpp>
#include <omicron/api/common/attribute/MapAttribute.hpp>


namespace
{

//------------------------------------------------------------------------------
//                              DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(default_constructor)
{
    omi::Int32Attribute a;
    ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
    ARC_CHECK_TRUE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
    ARC_CHECK_TRUE(a.is_pure_immutable());
    ARC_CHECK_EQUAL(a.get_size(), 0);
    ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
    ARC_CHECK_THROW(
        a.get_value(),
        arc::ex::IndexOutOfBoundsError
    );
    ARC_CHECK_TRUE(a.get_values().empty());
    ARC_CHECK_THROW(
        a.set_value(8),
        arc::ex::IllegalActionError
    );
    omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
    ARC_CHECK_THROW(
        a.set_values(values.begin(), values.end()),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.set_values(values),
        arc::ex::IllegalActionError
    );
    ARC_CHECK_THROW(
        a.set_tuple_size(4),
        arc::ex::IllegalActionError
    );
}

//------------------------------------------------------------------------------
//                               VALUE CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(value_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute a(17, true);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 17);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{17}
        );
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values.begin(), values.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{17}
        );
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }

    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute a(21, false);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 21);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{21}
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        a.set_values(values.begin(), values.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_values(values);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
                ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_tuple_size(4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 4);
    }
}

//------------------------------------------------------------------------------
//                              ITERATOR CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(iterator_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute a(values.begin(), values.end(), tuple_size, true);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute::ArrayType values = {-7, 4546, 1, 3};
        std::size_t tuple_size = 2;
        omi::Int32Attribute a(values.begin(), values.end(), tuple_size, false);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), -7);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6, 14, -98};
        a.set_values(values2.begin(), values2.end());
        ARC_CHECK_EQUAL(a.get_size(), values2.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values2);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        a.set_values(values2);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values2);
        ARC_CHECK_EQUAL(a.get_size(), values2.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        a.set_tuple_size(12);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 12);
    }
}

//------------------------------------------------------------------------------
//                               VECTOR CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(vector_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute a(values, tuple_size, true);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute::ArrayType values = {-7, 4546, 1, 3};
        std::size_t tuple_size = 2;
        omi::Int32Attribute a(values, tuple_size, false);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), -7);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6, 14, -98};
        a.set_values(values2.begin(), values2.end());
        ARC_CHECK_EQUAL(a.get_size(), values2.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values2);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        a.set_values(values2);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values2);
        ARC_CHECK_EQUAL(a.get_size(), values2.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        a.set_tuple_size(12);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 12);
    }
}

//------------------------------------------------------------------------------
//                                COPY CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(copy_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute b(values, tuple_size, true);
        omi::Int32Attribute a(b);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute b(21, false);
        omi::Int32Attribute a(b);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 21);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{21}
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        a.set_values(values.begin(), values.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_values(values);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
                ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_tuple_size(4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 4);
    }
}

//------------------------------------------------------------------------------
//                             ATTRIBUTE CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(attribute_constructor)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute b(values, tuple_size, true);
        omi::Attribute c(b);
        omi::Int32Attribute a(b);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute b(21, false);
        omi::DataAttribute c(b);
        omi::Int32Attribute a(b);
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 21);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{21}
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        a.set_values(values.begin(), values.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_values(values);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
                ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_tuple_size(4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 4);
    }
}

//------------------------------------------------------------------------------
//                              ASSIGNMENT OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(assignment_operator)
{
    ARC_TEST_MESSAGE("Checking immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute b(values, tuple_size, true);
        omi::Int32Attribute a;
        a = b;
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking mutable");
    {
        omi::Int32Attribute b(21, false);
        omi::Int32Attribute a(33, false);
        a = b;
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 21);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{21}
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        a.set_values(values.begin(), values.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_values(values);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
                ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_tuple_size(4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 4);
    }
    ARC_TEST_MESSAGE("Checking attribute immutable");
    {
        omi::Int32Attribute::ArrayType values = {0, 1, 4, -2, 6, 8};
        std::size_t tuple_size = 3;
        omi::Int32Attribute b(values, tuple_size, true);
        omi::Attribute c(b);
        omi::Int32Attribute a({2, 3, 4}, 1, true);
        a = c;
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_TRUE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), values.size());
        ARC_CHECK_EQUAL(a.get_tuple_size(), tuple_size);
        ARC_CHECK_EQUAL(a.get_value(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        omi::Int32Attribute::ArrayType values2 = {3, 8, 1, 6};
        ARC_CHECK_THROW(
            a.set_values(values2.begin(), values2.end()),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_THROW(
            a.set_values(values2),
            arc::ex::IllegalActionError
        );
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::IllegalActionError
        );
    }
    ARC_TEST_MESSAGE("Checking attribute mutable");
    {
        omi::Int32Attribute b(21, false);
        omi::DataAttribute c(b);
        omi::Int32Attribute a;
        a = c;
        ARC_CHECK_EQUAL(a.get_type(), omi::Int32Attribute::kTypeInt32);
        ARC_CHECK_TRUE(a.is_valid());
        ARC_CHECK_FALSE(a.is_immutable());
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_EQUAL(a.get_value(), 21);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{21}
        );
        omi::Int32Attribute::ArrayType values = {3, 8, 1, 6};
        a.set_values(values.begin(), values.end());
        ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
        a.set_value(8);
        ARC_CHECK_ITER_EQUAL(
            a.get_values(),
            omi::Int32Attribute::ArrayType{8}
        );
        ARC_CHECK_EQUAL(a.get_size(), 1);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_values(values);
        ARC_CHECK_ITER_EQUAL(a.get_values(), values);
                ARC_CHECK_EQUAL(a.get_size(), 4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
        a.set_tuple_size(4);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 4);
    }
}

//------------------------------------------------------------------------------
//                               EQUALITY OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(equality_operator)
{
    {
        omi::Int32Attribute a;
        omi::Attribute b;
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
    }
    {
        omi::Int32Attribute a;
        omi::Int32Attribute b;
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_TRUE(b == c);
        ARC_CHECK_FALSE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_TRUE(c == d);
        ARC_CHECK_FALSE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_TRUE(d == e);
        ARC_CHECK_FALSE(d != e);
    }
    {
        omi::Int32Attribute a(77);
        omi::Int32Attribute b(77);
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_TRUE(b == c);
        ARC_CHECK_FALSE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_TRUE(c == d);
        ARC_CHECK_FALSE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_TRUE(d == e);
        ARC_CHECK_FALSE(d != e);
        omi::Int32Attribute f = c;
        ARC_CHECK_TRUE(f == e);
        ARC_CHECK_FALSE(f != e);
    }
    {
        omi::Int32Attribute a(12);
        omi::Int32Attribute b(11);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
    {
        omi::Int32Attribute a({1, 5, -8, 4}, 2, true);
        omi::Int32Attribute b({1, 5, -8, 4}, 2, false);
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_TRUE(b == c);
        ARC_CHECK_FALSE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_TRUE(c == d);
        ARC_CHECK_FALSE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_TRUE(d == e);
        ARC_CHECK_FALSE(d != e);
        omi::Int32Attribute f = c;
        ARC_CHECK_TRUE(f == e);
        ARC_CHECK_FALSE(f != e);
    }
    {
        omi::Int32Attribute a({1, 5, -8, 4}, 2, true);
        omi::Int32Attribute b({1, 5, -8, 4}, 4, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
    {
        omi::Int32Attribute a({1, 5}, 2, true);
        omi::Int32Attribute b({1, 5, -8, 4}, 2, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
    {
        omi::Int32Attribute a({1, 5, -8, 4}, 2, true);
        omi::Int32Attribute b({1, 5, -8, 0}, 2, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(c);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
    {
        omi::Int32Attribute a({1, 5, -8, 4}, 2, true);
        omi::FloatAttribute b({1.0F, 5.0F, -8.0F, 4.0F}, 2, true);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::Attribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::DataAttribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(d);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
    {
        omi::MapAttribute::DataType map_data =
            {{"key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)}};

        omi::Int32Attribute a({1, 5, -8, 4}, 2, true);
        omi::MapAttribute b(map_data, false);
        ARC_CHECK_FALSE(a == b);
        ARC_CHECK_TRUE(a != b);
        omi::DataAttribute c(a);
        ARC_CHECK_FALSE(b == c);
        ARC_CHECK_TRUE(b != c);
        omi::Attribute d(b);
        ARC_CHECK_FALSE(c == d);
        ARC_CHECK_TRUE(c != d);
        omi::Int32Attribute e(d);
        ARC_CHECK_FALSE(d == e);
        ARC_CHECK_TRUE(d != e);
    }
}

//------------------------------------------------------------------------------
//                               INVALID ATTRIBUTE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(invalid)
{
    {
        omi::Attribute b;
        omi::Int32Attribute a(b);
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
            a.get_tuple_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_value(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_value(4),
            arc::ex::StateError
        );
        std::vector<omi::Int32Attribute::DataType> new_values = {1, 2, 3, 4};
        ARC_CHECK_THROW(
            a.set_values(new_values.begin(), new_values.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_values),
            arc::ex::StateError
        );
    }
    {
        omi::FloatAttribute b(3.14F);
        omi::Int32Attribute a(b);
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
            a.get_tuple_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_value(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_value(4),
            arc::ex::StateError
        );
        std::vector<omi::Int32Attribute::DataType> new_values = {1, 2, 3, 4};
        ARC_CHECK_THROW(
            a.set_values(new_values.begin(), new_values.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_values),
            arc::ex::StateError
        );
    }
    {
        omi::Int32Attribute b(3);
        omi::FloatAttribute c(b);
        omi::Int32Attribute a(c);
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
            a.get_tuple_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_value(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_value(4),
            arc::ex::StateError
        );
        std::vector<omi::Int32Attribute::DataType> new_values = {1, 2, 3, 4};
        ARC_CHECK_THROW(
            a.set_values(new_values.begin(), new_values.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_values),
            arc::ex::StateError
        );
    }
    {
        omi::MapAttribute::DataType map_data =
            {{"key", omi::Int32Attribute({1, 5, -8, 4}, 2, true)}};

        omi::MapAttribute b(map_data, false);
        omi::Int32Attribute a(b);
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
            a.get_tuple_size(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_tuple_size(4),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_value(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.get_values(),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_value(4),
            arc::ex::StateError
        );
        std::vector<omi::Int32Attribute::DataType> new_values = {1, 2, 3, 4};
        ARC_CHECK_THROW(
            a.set_values(new_values.begin(), new_values.end()),
            arc::ex::StateError
        );
        ARC_CHECK_THROW(
            a.set_values(new_values),
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
        omi::Int32Attribute a;
        ARC_CHECK_EQUAL(a.get_hash(), a.get_hash());
        omi::Int32Attribute b;
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        omi::Int32Attribute c = a.as_mutable();
        c.set_tuple_size(2);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), c.get_hash());
    }
    {
        omi::Int32Attribute a(12, true);
        omi::Int32Attribute b(12, false);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        b.set_value(8);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        omi::Int32Attribute c = a.as_mutable();
        ARC_CHECK_NOT_EQUAL(b.get_hash(), c.get_hash());
        c.set_value(8);
        ARC_CHECK_EQUAL(b.get_hash(), c.get_hash());
    }
    {
        omi::Int32Attribute a;
        omi::Int32Attribute b(12);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        omi::Int32Attribute c = a.as_mutable();
        c.set_value(12);
        ARC_CHECK_EQUAL(b.get_hash(), c.get_hash());
    }
    {
        omi::Int32Attribute a(45, false);
        omi::Int32Attribute b(0, false);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_value(45);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        a.set_tuple_size(2);
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_tuple_size(2);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
    }
    {
        omi::Int32Attribute a({4, -8, 1, 3}, 2, false);
        omi::Int32Attribute b({4, -8, 1, 3}, 2, false);
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
        a.set_values({1, 4, 7, 8});
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        b.set_values({1, 4, 7});
        ARC_CHECK_NOT_EQUAL(a.get_hash(), b.get_hash());
        a.set_values({1, 4, 7});
        ARC_CHECK_EQUAL(a.get_hash(), b.get_hash());
    }
}

//------------------------------------------------------------------------------
//                             STRING REPRESENTATION
//------------------------------------------------------------------------------

ARC_TEST_UNIT(string_representation)
{
    {
        omi::Int32Attribute a;
        arc::str::UTF8String s;
        arc::str::UTF8String r("Int32Attribute(0): []");
        s << a;
        ARC_CHECK_EQUAL(s, r);
        omi::DataAttribute b(a);
        s = "";
        s << b;
        ARC_CHECK_EQUAL(s, r);
        omi::Attribute c(a);
        s = "";
        s << c;
        ARC_CHECK_EQUAL(s, r);
    }
    {
        omi::Int32Attribute a(17, true);
        arc::str::UTF8String s;
        arc::str::UTF8String r("Int32Attribute(0): [17]");
        s << a;
        ARC_CHECK_EQUAL(s, r);
        omi::DataAttribute b(a);
        s = "";
        s << b;
        ARC_CHECK_EQUAL(s, r);
        omi::Attribute c(a);
        s = "";
        s << c;
        ARC_CHECK_EQUAL(s, r);
    }
    {
        omi::Int32Attribute a(-5, false);
        arc::str::UTF8String s;
        arc::str::UTF8String r("Int32Attribute(0): [-5]");
        s << a;
        ARC_CHECK_EQUAL(s, r);
        omi::DataAttribute b(a);
        s = "";
        s << b;
        ARC_CHECK_EQUAL(s, r);
        omi::Attribute c(a);
        s = "";
        s << c;
        ARC_CHECK_EQUAL(s, r);
    }
    {
        omi::Int32Attribute a({4, -1, 0, 546}, 2, true);
        arc::str::UTF8String s;
        arc::str::UTF8String r("Int32Attribute(2): [4, -1, 0, 546]");
        s << a;
        ARC_CHECK_EQUAL(s, r);
        omi::DataAttribute b(a);
        s = "";
        s << b;
        ARC_CHECK_EQUAL(s, r);
        omi::Attribute c(a);
        s = "";
        s << c;
        ARC_CHECK_EQUAL(s, r);
    }
    {
        omi::Int32Attribute a({0, 22, -12, 44, 94, 8}, 3, false);
        arc::str::UTF8String s;
        arc::str::UTF8String r("Int32Attribute(3): [0, 22, -12, 44, 94, 8]");
        s << a;
        ARC_CHECK_EQUAL(s, r);
        omi::DataAttribute b(a);
        s = "";
        s << b;
        ARC_CHECK_EQUAL(s, r);
        omi::Attribute c(a);
        s = "";
        s << c;
        ARC_CHECK_EQUAL(s, r);
    }
}

//------------------------------------------------------------------------------
//                                 SET TUPLE SIZE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(set_tuple_size)
{
    {
        omi::Int32Attribute a({1, 2, 3, 4}, 2, true);
        ARC_CHECK_THROW(
            a.set_tuple_size(0),
            arc::ex::IllegalActionError
        );
    }
    {
        omi::Int32Attribute a({1, 2, 3, 4}, 4, false);
        a.set_tuple_size(2);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 2);
        a.set_tuple_size(0);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 0);
    }
}

//------------------------------------------------------------------------------
//                                   SET VALUES
//------------------------------------------------------------------------------

ARC_TEST_UNIT(set_values)
{
    {
        omi::Int32Attribute a({1, 2, 3, 4}, 2, true);
        ARC_CHECK_THROW(
            a.set_values({4, 3, 2, 1}),
            arc::ex::IllegalActionError
        );
    }
    {
        omi::Int32Attribute a({1, 2, 3, 4}, 2, false);
        std::vector<omi::Int32Attribute::DataType> new_values = {4, 3, 2, 1};
        a.set_values(new_values);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 2);
        ARC_CHECK_ITER_EQUAL(a.get_values(), new_values);
        new_values = {8};
        a.set_value(8);
        ARC_CHECK_EQUAL(a.get_tuple_size(), 2);
        ARC_CHECK_ITER_EQUAL(a.get_values(), new_values);
    }
}

//------------------------------------------------------------------------------
//                                  AS IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_immutable)
{
    {
        omi::Int32Attribute a(12, true);
        omi::Int32Attribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_immutable());
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
    }
    {
        omi::Int32Attribute a(12, false);
        omi::Int32Attribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_value(8),
            arc::ex::IllegalActionError
        );
        a.set_tuple_size(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a(12, false);
        omi::Int32Attribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_value(8),
            arc::ex::IllegalActionError
        );
        a.set_value(6);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a({5, -4, 0, 12}, 2, false);
        omi::Int32Attribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_value(8),
            arc::ex::IllegalActionError
        );
        a.set_tuple_size(1);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a({1, 44, -6, 0}, 2, false);
        omi::Int32Attribute b = a.as_immutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_TRUE(b.is_immutable());
        ARC_CHECK_THROW(
            b.set_value(8),
            arc::ex::IllegalActionError
        );
        a.set_values({4, 5, 6, 7});
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
    }
}

//------------------------------------------------------------------------------
//                                   AS MUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_mutable)
{
    {
        omi::Int32Attribute a(12, false);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_TRUE(a.is_pure_mutable());
        b.set_tuple_size(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
        a.set_tuple_size(2);
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
        a.set_tuple_size(0);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a(12, false);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        b.set_value(6);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
        a.set_value(6);
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
        a.set_value(-2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a(12, true);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        b.set_tuple_size(2);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a(12, true);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        b.set_value(6);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a({5, -4, 0, 12}, 2, true);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        b.set_tuple_size(1);
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_NOT_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_EQUAL(a.get_values(), b.get_values());
    }
    {
        omi::Int32Attribute a({1, 44, -6, 0}, 2, true);
        omi::Int32Attribute b = a.as_mutable();
        ARC_CHECK_EQUAL(a, b);
        ARC_CHECK_FALSE(b.is_immutable());
        ARC_CHECK_THROW(
            a.set_value(8),
            arc::ex::IllegalActionError
        );
        b.set_values({4, 5, 6, 7});
        ARC_CHECK_NOT_EQUAL(a, b);
        ARC_CHECK_EQUAL(a.get_tuple_size(), b.get_tuple_size());
        ARC_CHECK_ITER_NOT_EQUAL(a.get_values(), b.get_values());
    }
}

} // namespace
