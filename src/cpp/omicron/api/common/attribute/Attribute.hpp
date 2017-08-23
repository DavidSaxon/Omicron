/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_

#include <cstddef>

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{

// TODO: DOC
// TODO: DOC immutable -> mutable && immutable -> mutable
class Attribute
{
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
        OMI_API_GLOBAL virtual Storage* copy_for_overwrite(bool soft);
    };

    //--------------------------------------------------------------------------
    //                                 DEFINITION
    //--------------------------------------------------------------------------

    // TODO: THIS THING CAN BE PRIVATE?
    class Definition
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        std::size_t m_ref_count;
        Type m_type;
        bool m_immutable;
        mutable Storage* m_storage;

        //------------------------C O N S T R U C T O R-------------------------

        // TODO: DOC
        Definition(
                Type type,
                bool immutable,
                Storage* storage)
            : m_ref_count(1)
            , m_type     (type)
            , m_immutable(immutable)
            , m_storage  (storage)
        {
            // increase immutable reference count of storage?
            if(m_immutable && m_storage != nullptr)
            {
                ++m_storage->m_immutable_ref_count;
            }
        }

        //-------------------------D E S T R U C T O R--------------------------

        // TODO: DOC
        OMI_API_GLOBAL virtual ~Definition()
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
                    // decrease immutable reference count?
                    if(m_immutable)
                    {
                        --m_storage->m_immutable_ref_count;
                    }
                }
            }
        }

    protected:

        //--------P R O T E C T E D    M E M B E R    F U N C T I O N S---------

        /*!
         * \brief Sets the storage object of this definition (and handles
         *        special reference counting for managing copy-on-write).
         */
        OMI_API_GLOBAL void set_storage(Storage* storage);
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
        return static_cast<T_StorageType*>(m_def->m_storage);
    }

    // TODO: DOC: throws IllegalActionError
    OMI_API_GLOBAL void prepare_modifcation(bool soft = false);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // The internal definition of attribute
    Definition* m_def;
};


} // namespace omi

#endif
