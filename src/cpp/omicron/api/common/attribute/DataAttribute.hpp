/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_

#include "omicron/api/common/attribute/Attribute.hpp"


namespace omi
{

// TODO: DOC
class DataAttribute : public Attribute
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief In order for an derived type to be considered a valid data
     *        attribute the first byte of its type id must be these bits.
     */
    static Type kTypeDataBits;

    //--------------------------------------------------------------------------
    //                               PUBLIC STRUCTS
    //--------------------------------------------------------------------------

    // TODO: DOC
    struct DataDefinition : public Attribute::Definition
    {
        std::size_t m_size;
        std::size_t m_tuple_size;

        DataDefinition(Type type, std::size_t tuple_size)
            : Attribute::Definition(type)
            , m_size               (0)
            , m_tuple_size         (tuple_size)
        {
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
    OMI_API_GLOBAL DataAttribute();

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid data attribute this construct an
     * invalid DataAttribute.
     */
    OMI_API_GLOBAL DataAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given DataAttribute.
     */
    OMI_API_GLOBAL DataAttribute(const DataAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL ~DataAttribute();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: doc
    OMI_API_GLOBAL std::size_t get_size() const;

    // TODO: doc
    OMI_API_GLOBAL std::size_t get_tuple_size() const;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL DataAttribute(Definition* def, bool immutable = true);

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL virtual bool check_type(Type type) const;
};

} // namespace omi

#endif
