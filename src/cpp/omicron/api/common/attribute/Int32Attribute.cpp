#include "omicron/api/common/attribute/Int32Attribute.hpp"

#include <typeinfo>

#include <arcanecore/base/Exceptions.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Int32Attribute::kTypeInt32 =
    DataAttribute::kTypeDataBits & (typeid(arc::int32).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::Int32Attribute(bool immutable)
    : DataAttribute(new Int32Definition(), immutable)
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(DataType value, bool immutable)
    : DataAttribute(new Int32Definition(&value, (&value) + 1, 0), immutable)
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(
        const std::vector<DataType>& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        new Int32Definition(values.begin(), values.end(), tuple_size),
        immutable
    )
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(const Int32Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::~Int32Attribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::DataType Int32Attribute::get_value(
        bool throw_on_error,
        DataType default_value) const
{
    // get the storage
    Int32Storage* storage = get_storage(throw_on_error);
    if(storage == nullptr)
    {
        return default_value;
    }

    // non-empty?
    if(storage->m_data.empty())
    {
        if(throw_on_error)
        {
            throw arc::ex::IndexOutOfBoundsError(
                "Failed to get single value from empty Int32Attribute"
            );
        }
        else
        {
            return default_value;
        }
    }

    // return
    return storage->m_data.front();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool Int32Attribute::check_type(Type type) const
{
    return type == kTypeInt32;
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

Int32Attribute::Int32Storage* Int32Attribute::get_storage(
        bool throw_on_error) const
{
    // valid?
    if(!is_valid())
    {
        if(throw_on_error)
        {
            throw arc::ex::ValueError("Invalid Int32Attribute");
        }
        else
        {
            return nullptr;
        }
    }

    // cast and return
    return static_cast<Int32Storage*>(m_def->m_storage);
}

} // namespace omi
