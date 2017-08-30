/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_

#include <cstddef>

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>


#include "omicron/api/API.hpp"

namespace omi
{

// TODO: DOC
// TODO: DOC immutable -> mutable && immutable -> mutable
class Attribute
    : private arc::lang::Nonmovable
{
private:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend OMI_API_GLOBAL arc::str::UTF8String& operator<<(
            arc::str::UTF8String&,
            const Attribute&);

public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Integral type which defines the type identifier of an attribute.
     */
    typedef arc::int32 Type;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for null attributes.
     */
    OMI_API_GLOBAL static Type kTypeNull;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    // TODO: DOC
    class Storage
        : private arc::lang::Noncopyable
        , private arc::lang::Nonmovable
        , private arc::lang::Noncomparable
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        // TODO: DOC
        std::size_t m_ref_count;
        // TODO: DOC
        std::size_t m_immutable_ref_count;

        //------------------------C O N S T R U C T O R-------------------------

        // TODO: DOC
        OMI_API_GLOBAL Storage();

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_GLOBAL virtual ~Storage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        // TODO: DOC
        virtual bool equals(const Storage* other) = 0;

        // TODO: DOC
        virtual Storage* copy_for_overwrite(bool soft) = 0;

        // TODO: DOC
        virtual void string_repr(arc::str::UTF8String& s) const = 0;
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new null attribute.
     *
     * \note This attribute is immutable by definition.
     */
    OMI_API_GLOBAL Attribute();

    /*!
     * \brief Constructs a new reference count of the given attribute.
     */
    OMI_API_GLOBAL Attribute(const Attribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~Attribute();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     */
    OMI_API_GLOBAL Attribute& operator=(const Attribute& other);

    /*!
     * \brief Equality operator.
     */
    OMI_API_GLOBAL bool operator==(const Attribute& other);

    /*!
     * \brief Inequality operator.
     */
    OMI_API_GLOBAL bool operator!=(const Attribute& other);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: doc
    OMI_API_GLOBAL Type get_type() const;

    // TODO: doc (whether the attribute is valid for its current type)
    OMI_API_GLOBAL bool is_valid() const;

    // TODO: doc
    OMI_API_GLOBAL bool is_immutable() const;

    /*!
     * \brief Attempts to a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid attribute type to copy from this
     * will construct an invalid attribute.
     */
    OMI_API_GLOBAL void assign(const Attribute& other);

    // TODO: doc
    OMI_API_GLOBAL Attribute as_immutable() const;

    // TODO: doc
    OMI_API_GLOBAL Attribute as_mutable() const;

protected:

    //--------------------------------------------------------------------------
    //                                 DEFINITION
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal definition object of attributes - contains the
     *        copy-on-write storage object and manages reference counting.
     */
    class Definition;

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    // TODO: this doesn't have to take the definition - just need a null
    //       constructor
    /*!
     * \brief Super constructor which assigns the internal definition to the
     *        given object **without** reference counting it.
     */
    OMI_API_GLOBAL Attribute(Definition* def);

    // TODO: DOC
    OMI_API_GLOBAL Attribute(Type type, bool immutable, Storage* storage);

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Checks whether the given type is valid for this attribute.
     */
    OMI_API_GLOBAL virtual bool check_type(Type type) const;

    /*!
     * \brief Increases the reference count this attribute's internal
     *        definition.
     */
    OMI_API_GLOBAL void increase_ref();

    /*!
     * \brief Decreases the reference count of this attribute's internal
     *        definition, if the reference count reaches 0, the definition is
     *        deleted.
     */
    OMI_API_GLOBAL void decrease_ref();

    /*!
     * \brief Returns the storage object of this attribute's definition casted
     *        as a pointer to the given type.
     */
    template<typename T_StorageType>
    T_StorageType* get_storage() const
    {
        return static_cast<T_StorageType*>(get_untyped_storage());
    }

    // TODO: DOC: throws IllegalActionError
    OMI_API_GLOBAL void prepare_modifcation(bool soft = false);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // The internal definition of attribute
    Definition* m_def;

    //--------------------------------------------------------------------------
    //                             PRIVATE FUNCTIONS
    //--------------------------------------------------------------------------

    // Returns the storage of this attribute as is
    OMI_API_GLOBAL Storage* get_untyped_storage() const;
};

//------------------------------------------------------------------------------
//                               EXTERNAL OPERATORS
//------------------------------------------------------------------------------

/*!
 * \brief Appends a string representation of the Attribute to the given
 *        UTF8String.
 */
OMI_API_GLOBAL arc::str::UTF8String& operator<<(
        arc::str::UTF8String& s,
        const omi::Attribute& a);

/*!
 * \brief Appends a string representation of the Attribute to the given stream.
 */
OMI_API_GLOBAL std::ostream& operator<<(
        std::ostream& s,
        const omi::Attribute& a);

} // namespace omi

#endif
