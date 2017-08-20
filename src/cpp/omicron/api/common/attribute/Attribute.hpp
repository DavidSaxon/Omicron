/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_ATTRIBUTE_HPP_

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
    //                               PUBLIC STRUCT
    //--------------------------------------------------------------------------

    // TODO: Doc
    struct Storage
        : private arc::lang::Noncopyable
        , private arc::lang::Nonmovable
        , private arc::lang::Noncomparable
    {
        std::size_t m_ref_count;

        Storage()
            : m_ref_count(1)
        {
        }

        virtual ~Storage()
        {
        }

        // TODO: new copy
    };

    // TODO: DOC
    struct Definition
    {
    public:

        std::size_t m_ref_count;
        Type m_type;
        Storage* m_storage;

        Definition(Type type)
            : m_ref_count(1)
            , m_type     (type)
            , m_storage  (nullptr)
        {
        }

        virtual ~Definition()
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

        virtual bool prepare_write()
        {
            return false;
        }
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
    OMI_API_GLOBAL Attribute as_mutable() const;

    // TODO: doc
    OMI_API_GLOBAL Attribute as_immutable() const;

protected:

    //--------------------------------------------------------------------------
    //                            PROTECTED ATTRIBUTES
    //--------------------------------------------------------------------------

    // The internal definition of attribute
    Definition* m_def;
    // Whether this attribute immutable
    bool m_immutable;

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor which assigns the internal definition to the
     *        given object **without** reference counting it.
     *
     * \param immutable Whether this attribute will be immutable or not.
     */
    OMI_API_GLOBAL Attribute(Definition* def, bool immutable = true);

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
};


} // namespace omi

#endif
