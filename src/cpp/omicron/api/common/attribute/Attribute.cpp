#include "omicron/api/common/attribute/Attribute.hpp"

#include <arcanecore/base/Exceptions.hpp>

namespace omi
{

//------------------------------------------------------------------------------
//                            PUBLIC STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Attribute::kTypeNull = 0;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T R U C T O R-----------------------------

OMI_API_GLOBAL Attribute::Storage::Storage()
    : m_ref_count          (1)
    , m_immutable_ref_count(0)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_GLOBAL Attribute::Storage::~Storage()
{
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_GLOBAL Attribute::Storage* Attribute::Storage::copy_for_overwrite(
        bool soft)
{
    // no storage at this level
    return nullptr;
}

//------------------------------------------------------------------------------
//                                   DEFINITION
//------------------------------------------------------------------------------

//----------------------------C O N S T R U C T O R-----------------------------

// OMI_API_GLOBAL Attribute::Definition::Definition(Type type, bool immutable)
//     : m_ref_count(1)
//     , m_type     (type)
//     , m_immutable(immutable)
//     , m_storage  (nullptr)
// {
// }

// //-----------------------------D E S T R U C T O R------------------------------

// OMI_API_GLOBAL Attribute::Definition::~Definition()
// {
//     // decrease the reference count of the storage
//     if(m_storage != nullptr)
//     {
//         if(m_storage->m_ref_count <= 1)
//         {
//             delete m_storage;
//         }
//         else
//         {
//             --m_storage->m_ref_count;
//             // decrease immutable reference count?
//             if(m_immutable)
//             {
//                 --m_storage->m_immutable_ref_count;
//             }
//         }
//     }
// }

// //---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

// OMI_API_GLOBAL void Attribute::Definition::prepare_modifcation(bool soft)
// {
//     // throw if immutable
//     if(m_immutable)
//     {
//         throw arc::ex::IllegalActionError(
//             "Immutable attribute cannot be modified"
//         );
//     }

//     // is the storage used by any immutable definitions?
//     if(m_storage->m_immutable_ref_count > 0)
//     {
//         // need to copy for rewrite
//         m_storage = m_storage->copy_for_overwrite(soft);
//     }
// }

// //------------P R O T E C T E D    M E M B E R    F U N C T I O N S-------------

// OMI_API_GLOBAL void Attribute::Definition::set_storage(Storage* storage)
// {
//     m_storage = storage;
//     // increase immutable reference count?
//     if(m_immutable)
//     {
//         ++m_storage->m_immutable_ref_count;
//     }
// }

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Attribute()
    : m_def(new Definition(kTypeNull, true, nullptr))
{
}

OMI_API_GLOBAL Attribute::Attribute(const Attribute& other)
    : m_def(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::~Attribute()
{
    decrease_ref();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Attribute::get_type() const
{
    return m_def->m_type;
}

OMI_API_GLOBAL bool Attribute::is_valid() const
{
    return check_type(m_def->m_type);
}

OMI_API_GLOBAL bool Attribute::is_immutable() const
{
    return m_def->m_immutable;
}

OMI_API_GLOBAL void Attribute::assign(const Attribute& other)
{
    // decrease the reference of the existing definition
    if(m_def != nullptr)
    {
        decrease_ref();
    }

    // use the other definition if it's not null and has a valid type
    if(other.m_def != nullptr && check_type(other.m_def->m_type))
    {
        // assign the new definition and increase the reference count
        m_def = other.m_def;
        increase_ref();
    }
    else
    {
        // assign new null definition
        m_def = new Definition(kTypeNull, true, nullptr);
    }
}

OMI_API_GLOBAL Attribute Attribute::as_immutable() const
{
    // no need to do anything if this is already immutable
    if(m_def->m_immutable)
    {
        return *this;
    }

    // decrease the reference count of the current definition
    ++m_def->m_storage->m_ref_count;
    // create a new definition and return it as an attribute
    return Attribute(new Definition(m_def->m_type, true, m_def->m_storage));
}

OMI_API_GLOBAL Attribute Attribute::as_mutable() const
{
    // no need to do anything if this is already mutable
    if(!m_def->m_immutable)
    {
        return *this;
    }

    // decrease the reference count of the current definition
    ++m_def->m_storage->m_ref_count;
    // create a new definition and return it as an attribute
    return Attribute(new Definition(m_def->m_type, false, m_def->m_storage));
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Attribute(Definition* def)
    : m_def(def)
{
}

OMI_API_GLOBAL Attribute::Attribute(
        Type type,
        bool immutable,
        Storage* storage)
    : m_def(new Definition(type, immutable, storage))
{
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool Attribute::check_type(Type type) const
{
    // always valid at this level
    return true;
}

OMI_API_GLOBAL void Attribute::increase_ref()
{
    ++m_def->m_ref_count;
}

OMI_API_GLOBAL void Attribute::decrease_ref()
{
    // delete?
    if(m_def->m_ref_count == 1)
    {
        delete m_def;
        m_def = nullptr;
    }
    else
    {
        --m_def->m_ref_count;
    }
}

OMI_API_GLOBAL void Attribute::prepare_modifcation(bool soft)
{
    // throw if immutable
    if(m_def->m_immutable)
    {
        throw arc::ex::IllegalActionError(
            "Immutable attribute cannot be modified"
        );
    }

    // is the storage used by any immutable definitions?
    if(m_def->m_storage->m_immutable_ref_count > 0)
    {
        // need to copy for rewrite
        m_def->m_storage = m_def->m_storage->copy_for_overwrite(soft);
    }
}

} // namespace omi
