#include "omicron/api/common/attribute/Attribute.hpp"

#include <arcanecore/base/Exceptions.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                            PUBLIC STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Type Attribute::kTypeNull = 0;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T R U C T O R-----------------------------

OMI_API_EXPORT Attribute::Storage::Storage()
    : m_ref_count(1)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_EXPORT Attribute::Storage::~Storage()
{
}

//------------------------------------------------------------------------------
//                                   DEFINITION
//------------------------------------------------------------------------------

class Attribute::Definition
{
public:

    //--------------------P U B L I C    A T T R I B U T E S--------------------

    std::size_t m_ref_count;
    Type m_type;
    bool m_immutable;
    mutable Storage* m_storage;

    //--------------------------C O N S T R U C T O R---------------------------

    Definition(
            Type type,
            bool immutable,
            Storage* storage)
        : m_ref_count(1)
        , m_type     (type)
        , m_immutable(immutable)
        , m_storage  (storage)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~Definition()
    {
        // decrease the reference count of the storage
        if(m_storage != nullptr)
        {
            if(m_storage->m_ref_count <= 1)
            {
                delete m_storage;
            }
            else
            {
                --m_storage->m_ref_count;
            }
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Attribute()
    : m_def(new Definition(kTypeNull, true, nullptr))
{
}

OMI_API_EXPORT Attribute::Attribute(const Attribute& other)
    : m_def(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::~Attribute()
{
    decrease_ref();
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute& Attribute::operator=(const Attribute& other)
{
    assign(other);
    return *this;
}

OMI_API_EXPORT bool Attribute::operator==(const Attribute& other) const
{
    // fast fail if types aren't the same
    if(get_type() != other.get_type())
    {
        return false;
    }
    // fast pass if both have null storage
    if(m_def->m_storage == nullptr && other.m_def->m_storage == nullptr)
    {
        return true;
    }
    // fast fail if one storage is null but the other isn't
    if(m_def->m_storage == nullptr || other.m_def->m_storage == nullptr)
    {
        return false;
    }
    // use storage comparison
    return m_def->m_storage->equals(other.m_def->m_storage);
}

OMI_API_EXPORT bool Attribute::operator!=(const Attribute& other) const
{
    return !((*this) == other);
}

OMI_API_EXPORT bool Attribute::operator<(const Attribute& other) const
{
    // check types first
    if(get_type() != other.get_type())
    {
        return get_type() < other.get_type();
    }
    // both have null storage?
    if(m_def->m_storage == nullptr && other.m_def->m_storage == nullptr)
    {
        return true;
    }
    // this has null storage
    if(m_def->m_storage == nullptr)
    {
        return true;
    }
    // other has null storage
    if(other.m_def->m_storage == nullptr)
    {
        return false;
    }
    // use storage less than
    return m_def->m_storage->less_than(other.m_def->m_storage);
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Type Attribute::get_type() const
{
    return m_def->m_type;
}

OMI_API_EXPORT bool Attribute::is_valid() const
{
    return check_type(m_def->m_type);
}

OMI_API_EXPORT bool Attribute::is_immutable() const
{
    // invalid attributes are always immutable
    return m_def->m_immutable || !is_valid();
}

OMI_API_EXPORT bool Attribute::is_pure_immutable() const
{
    // check mutability first
    if(!is_immutable())
    {
        return false;
    }
    // null storage?
    if(m_def->m_storage == nullptr)
    {
        return true;
    }
    // check data
    return m_def->m_storage->is_data_pure_immutable();
}

OMI_API_EXPORT bool Attribute::is_pure_mutable() const
{
    // check mutability first
    if(is_immutable())
    {
        return false;
    }
    // null storage?
    if(m_def->m_storage == nullptr)
    {
        return false;
    }
    // check data
    return m_def->m_storage->is_data_pure_mutable();
}

OMI_API_EXPORT Attribute::Hash Attribute::get_hash() const
{
    // return null hash?
    if(m_def == nullptr || m_def->m_storage == nullptr)
    {
        return Hash();
    }
    // return from storage
    return m_def->m_storage->get_hash(static_cast<arc::uint64>(m_def->m_type));
}

OMI_API_EXPORT void Attribute::assign(const Attribute& other)
{
    // decrease the reference of the existing definition
    if(m_def != nullptr)
    {
        decrease_ref();
    }

    // use the other definition if it's not null, and has a valid type
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

OMI_API_EXPORT Attribute Attribute::as_immutable() const
{
    // error on invalid attributes
    check_state("as_immutable() used on an invalid attribute");

    // no need to do anything if this is already immutable
    if(m_def->m_immutable)
    {
        return *this;
    }

    // increase the reference count of the current storage
    if(m_def->m_storage != nullptr)
    {
        ++m_def->m_storage->m_ref_count;
    }
    // create a new definition and return it as an attribute
    return Attribute(new Definition(m_def->m_type, true, m_def->m_storage));
}

OMI_API_EXPORT Attribute Attribute::as_mutable() const
{
    // error on invalid attributes
    check_state("as_mutable() used on an invalid attribute");

    // increase the reference count of the current storage
    if(m_def->m_storage != nullptr)
    {
        ++m_def->m_storage->m_ref_count;
    };
    // create a new definition and return it as an attribute
    return Attribute(new Definition(m_def->m_type, false, m_def->m_storage));
}

OMI_API_EXPORT Attribute Attribute::as_pure_immutable() const
{
    // error on invalid attributes
    check_state("as_pure_immutable() used on an invalid attribute");

    // already pure immutable?
    if(is_pure_immutable())
    {
        return *this;
    }
    // get an immutable version of this attribute - if that's pure then return
    // it
    Attribute a0 = as_immutable();
    if(a0.is_pure_immutable())
    {
        return a0;
    }
    // return using immutable storage
    return Attribute(new Definition(
        m_def->m_type,
        true,
        m_def->m_storage->as_pure_immutable()
    ));
}

OMI_API_EXPORT Attribute Attribute::as_pure_mutable() const
{
    // error on invalid attributes
    check_state("as_pure_mutable() used on an invalid attribute");

    // get an mutable version of this attribute - if that's pure then return it
    Attribute a0 = as_mutable();
    if(a0.is_pure_mutable())
    {
        return a0;
    }
    // return using immutable storage
    return Attribute(new Definition(
        m_def->m_type,
        false,
        m_def->m_storage->as_pure_mutable()
    ));
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Attribute(Definition* def)
    : m_def(def)
{
}

OMI_API_EXPORT Attribute::Attribute(
        Type type,
        bool immutable,
        Storage* storage)
    : m_def(new Definition(type, immutable, storage))
{
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool Attribute::check_type(Type type) const
{
    // valid for anything that is not null
    return type != kTypeNull;
}

OMI_API_EXPORT void Attribute::increase_ref()
{
    ++m_def->m_ref_count;
}

OMI_API_EXPORT void Attribute::decrease_ref()
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

OMI_API_EXPORT void Attribute::prepare_modifcation(bool soft)
{
    // throw if immutable
    if(m_def->m_immutable)
    {
        throw arc::ex::IllegalActionError(
            "Immutable attribute cannot be modified"
        );
    }

    // throw if null
    if(m_def->m_storage == nullptr)
    {
        throw arc::ex::IllegalActionError(
            "Null attribute cannot be modified"
        );
    }

    // is the storage used by any other definitions?
    if(m_def->m_storage->m_ref_count > 1)
    {
        // decrease the current reference count
        --m_def->m_storage->m_ref_count;
        // need to copy for rewrite
        m_def->m_storage = m_def->m_storage->copy_for_overwrite(soft);
    }

    // invalidate the current hash
    m_def->m_storage->invalidate_hash();
}

OMI_API_EXPORT void Attribute::check_state(
        const arc::str::UTF8String& message) const
{
    if(!is_valid())
    {
        throw arc::ex::StateError(message);
    }
}

/*!
 * \brief Appends the string representation of this attribute (using the
 *        given indentation amount) to the provided string.
 */
OMI_API_EXPORT void Attribute::string_repr(
        arc::str::UTF8String& s,
        std::size_t indentation) const
{
    // null storage?
    if(m_def->m_storage == nullptr)
    {
        // indent?
        if(indentation > 0)
        {
            s += (arc::str::UTF8String(" ") * indentation);
        }
        s += "NullAttribute";
    }
    else
    {
        // get from storage
        m_def->m_storage->string_repr(indentation, s);
    }
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Storage* Attribute::get_untyped_storage() const
{
    return m_def->m_storage;
}

//------------------------------------------------------------------------------
//                               EXTERNAL OPERATORS
//------------------------------------------------------------------------------

OMI_API_EXPORT arc::str::UTF8String& operator<<(
        arc::str::UTF8String& s,
        const omi::Attribute& a)
{
    a.string_repr(s, 0);
    return s;
}

OMI_API_EXPORT std::ostream& operator<<(
        std::ostream& s,
        const omi::Attribute& a)
{
    arc::str::UTF8String us;
    us << a;
    s << us;
    return s;
}

OMI_API_EXPORT arc::str::UTF8String& operator<<(
        arc::str::UTF8String& s,
        const omi::Attribute::Hash& h)
{
    s << h.part1 << "::" << h.part2;
    return s;
}

OMI_API_EXPORT std::ostream& operator<<(
        std::ostream& s,
        const omi::Attribute::Hash& h)
{
    s << h.part1 << "::" << h.part2;
    return s;
}

} // namespace omi
