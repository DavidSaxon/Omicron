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

/*!
 * \brief The is the base class for all Omicron Attributes.
 *
 * Attributes are data containers that can be passed around generically.
 * Attributes can be efficiently "casted" as any other Attribute type that they
 * derive from and then "re-casted" back to their original type with no data
 * loss. In-fact Attributes can be casted to any other attribute type but unless
 * is a cast to a derived type of the original type or the original type the
 * result will be an invalid null attribute. Casting attributes is efficient
 * because no actual copying of data is performed - attributes use a
 * copy-on-write model which means data is purely referenced counted between
 * attributes until it's modified.
 *
 * Attributes can be group into four main categories:
 *  - NullAttribute: Attributes that are invalid or are constructed from the
 *                   base attribute type omi::Attribute, these attributes are
 *                   are a null type but always valid.
 *  - DataAttribute: The most common attribute type which contains zero or more
 *                   values of specific type, e.g. FloatAttribute,
 *                   Int32Attribute, etc.
 *  - ArrayAttribute: An attribute that contains an ordered array of zero or
 *                    or more attributes.
 *  - MapAttribute: An attribute which contains key values pairs of names and
 *                  attributes. MapAttributes can be used to encode hierarchies
 *                  by nesting MapAttributes.
 *
 * An Attribute can be checked to see if it's valid using the is_valid()
 * function.
 *
 * For example:
 *
 * \code
 * omi::Int32Attribute a(12);
 * omi::Attribute b(a);
 * omi::Int32Attribute c(b);
 * c.is_valid();
 * \endcode
 *
 * Would return ```true``` since the original Int32Attribute has been casted to
 * a type it was derived from then and then casted back to its original type.
 *
 * However:
 *
 * \code
 * omi::Int32Attribute a(12);
 * omi::FloatAttribute b(a);
 * b.is_valid();
 * \endcode
 *
 * Would return ```false``` since the original Int32Attribute has been casted to
 * an orthogonal type.
 */
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

    /*!
     * \brief The internal storage class defines how the contents of the
     *        attribute will be stored.
     *
     * Implementation of any sub-attribute types need to implement a derived
     * storage class.
     */
    class Storage
        : private arc::lang::Noncopyable
        , private arc::lang::Nonmovable
        , private arc::lang::Noncomparable
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        /*!
         * \brief The current reference count of the storage.
         */
        std::size_t m_ref_count;

        //------------------------C O N S T R U C T O R-------------------------

        /*!
         * \brief Storage super constructor.
         */
        OMI_API_GLOBAL Storage();

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_GLOBAL virtual ~Storage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        /*!
         * \brief Compares whether this storage has equality with the other
         *        given storage pointer.
         */
        virtual bool equals(const Storage* other) = 0;

        /*!
         * \brief Makes a copy of this storage with the intention that the
         *        copy's values will be overwritten.
         *
         * This means that heavy data does not need to be copied since it will
         * be overridden. The exception to this case is if the soft parameter is
         * ```true``` which means the copy is being made for a soft overwrite,
         * for example a DataAttribute's tuple size is being modified. When
         * the soft parameter is ```true``` the full data of the storage should
         * be copied.
         */
        virtual Storage* copy_for_overwrite(bool soft) = 0;

        /*!
         * \brief Appends a string representation of this storage to the given
         *        string.
         *
         * \param indentation The amount of indentation (in spaces) that each
         *                    line of this attribute should be proceeded by.
         * \param s The string to append to.
         */
        virtual void string_repr(
                std::size_t indentation,
                arc::str::UTF8String& s) const = 0;
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

    /*!
     * \brief Returns the type identifier of this attribute.
     *
     * Each attribute type has a unique type attribute, which means checking
     * the type of a base class attribute can be to check what the original type
     * of the attribute is.
     */
    OMI_API_GLOBAL Type get_type() const;

    /*!
     * \brief Returns whether this attribute is valid.
     *
     * Base attributes are always valid, but derived types will be invalid if
     * they were casted from an incompatible type.
     *
     * Invalid attributes will always return their type as
     * omi::Attribute::kTypeNull.
     */
    OMI_API_GLOBAL bool is_valid() const;

    /*!
     * \brief Returns whether this attribute is immutable or not.
     *
     * Immutable attributes will never copy storage and mutable attributes will
     * only copy storage when they're modified.
     */
    OMI_API_GLOBAL bool is_immutable() const;

    /*!
     * \brief Attempts to a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid attribute type to copy from this
     * will construct an invalid attribute.
     */
    OMI_API_GLOBAL void assign(const Attribute& other);

    /*!
     * \brief Returns an immutable version of this attribute.
     *
     * If this attribute is already immutable itself will be returned.
     * If this attribute is mutable a copy of the storage will not be made for
     * the new immutable attribute, instead the next time this mutable
     * attribute is modified it will take a new copy of the storage.
     *
     * \throws arc::ex::StateError If this attribute is invalid.
     */
    OMI_API_GLOBAL Attribute as_immutable() const;

    /*!
     * \brief Returns an mutable version of this attribute.
     *
     * If this attribute is already mutable itself will be returned.
     * If this attribute is immutable a copy of the storage will not be made for
     * the new mutable attribute, instead the next time the mutable attribute is
     * modified it will take a new copy of the storage.
     *
     * \throws arc::ex::StateError If this attribute is invalid.
     */
    OMI_API_GLOBAL Attribute as_mutable() const;

    /*!
     * \brief Appends the string representation of this attribute (using the
     *        given indentation amount) to the provided string.
     *
     * \param s The string to append this string representation.
     * \param indentation The amount of indentation (in spaces) this attribute
     *                    should be indented by.
     */
    OMI_API_GLOBAL void string_repr(
            arc::str::UTF8String& s,
            std::size_t indentation = 0) const;

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

    /*!
     * \brief Super constructor which assigns the internal definition to the
     *        given object **without** reference counting it.
     */
    OMI_API_GLOBAL Attribute(Definition* def);

    /*!
     * \brief Super constructor for derived types.
     *
     * \param type The type identifier of this attribute.
     * \param immutable Whether this attribute is mutable or not.
     * \param storage The internal storage object this attribute will use (can
     *                be null).
     */
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

    /*!
     * \brief Convenience function that checks whether this attribute is valid
     *        and if it is not throws an arc::ex::StateError with the given
     *        message.
     */
    OMI_API_GLOBAL void check_state(const arc::str::UTF8String& message) const;

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
