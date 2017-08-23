#include "arcanecore/test/ArcTest.hpp"

ARC_TEST_MODULE(omi.api.common.Attribute)

#include <omicron/api/common/attribute/Attribute.hpp>


namespace
{

ARC_TEST_UNIT(default_constructor)
{
    omi::Attribute a;
    ARC_CHECK_EQUAL(a.get_type(), omi::Attribute::kTypeNull);
    ARC_CHECK_TRUE(a.is_valid());
    ARC_CHECK_TRUE(a.is_immutable());
}

// TODO: what should as immutable do?

// TODO: what should as mutable do?

} // namespace
