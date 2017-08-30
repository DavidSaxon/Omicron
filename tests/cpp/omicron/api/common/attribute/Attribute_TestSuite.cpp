#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.Attribute)

#include <omicron/api/common/attribute/Attribute.hpp>


namespace
{

//------------------------------------------------------------------------------
//                              DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(default_constructor)
{
    omi::Attribute a;
    ARC_CHECK_EQUAL(a.get_type(), omi::Attribute::kTypeNull);
    ARC_CHECK_FALSE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
}

//------------------------------------------------------------------------------
//                                COPY CONSTRUCTOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(copy_constructor)
{
    omi::Attribute b;
    omi::Attribute a(b);
    ARC_CHECK_EQUAL(a.get_type(), omi::Attribute::kTypeNull);
    ARC_CHECK_FALSE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
}

//------------------------------------------------------------------------------
//                              ASSIGNMENT OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(assignment_operator)
{
    omi::Attribute b;
    omi::Attribute a;
    a = b;
    ARC_CHECK_EQUAL(a.get_type(), omi::Attribute::kTypeNull);
    ARC_CHECK_FALSE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
}

//------------------------------------------------------------------------------
//                               EQUALITY OPERATOR
//------------------------------------------------------------------------------

ARC_TEST_UNIT(equality_operator)
{
    {
        omi::Attribute a;
        omi::Attribute b;
        ARC_CHECK_TRUE(a == b);
        ARC_CHECK_FALSE(a != b);
    }
}

//------------------------------------------------------------------------------
//                                  AS IMMUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_immutable)
{
    omi::Attribute a;
    ARC_CHECK_THROW(
        a.as_immutable(),
        arc::ex::IllegalActionError
    );
}

//------------------------------------------------------------------------------
//                                   AS MUTABLE
//------------------------------------------------------------------------------

ARC_TEST_UNIT(as_mutable)
{
    omi::Attribute a;
    ARC_CHECK_THROW(
        a.as_mutable(),
        arc::ex::IllegalActionError
    );
}

//------------------------------------------------------------------------------
//                             STRING REPRESENTATION
//------------------------------------------------------------------------------

ARC_TEST_UNIT(string_representation)
{
    omi::Attribute a;
    arc::str::UTF8String s;
    arc::str::UTF8String r("NullAttribute");
    s << a;
    ARC_CHECK_EQUAL(s, r);
}

} // namespace
