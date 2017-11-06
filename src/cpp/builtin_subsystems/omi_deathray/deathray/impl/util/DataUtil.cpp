#include "deathray/impl/util/DataUtil.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/str/UTF8String.hpp>


namespace death
{
namespace util
{
namespace data
{

DeathSize data_type_to_bit_width(DeathDataType data_type)
{
    return data_type_to_byte_width(data_type) * 8;
}

DeathSize data_type_to_byte_width(DeathDataType data_type)
{
    switch(data_type)
    {
        case kDeathByte:
            return static_cast<DeathSize>(sizeof(DeathByte));
        case kDeathUByte:
            return static_cast<DeathSize>(sizeof(DeathUByte));
        case kDeathInt16:
            return static_cast<DeathSize>(sizeof(DeathInt16));
        case kDeathUInt16:
            return static_cast<DeathSize>(sizeof(DeathUInt16));
        case kDeathInt32:
            return static_cast<DeathSize>(sizeof(DeathInt32));
        case kDeathUInt32:
            return static_cast<DeathSize>(sizeof(DeathUInt32));
        case kDeathInt64:
            return static_cast<DeathSize>(sizeof(DeathInt64));
        case kDeathUInt64:
            return static_cast<DeathSize>(sizeof(DeathUInt64));
        case kDeathFloat:
            return static_cast<DeathSize>(sizeof(DeathFloat));
        case kDeathDouble:
            return static_cast<DeathSize>(sizeof(DeathDouble));
    }

    arc::str::UTF8String error_message;
    error_message
        << "Unexpected data type: " << static_cast<int>(data_type);
    throw arc::ex::ValueError(error_message);
}

} // namespace data
} // namespace util
} // namespace death
