/*!
 * \file
 * \author David Saxon
 */
#ifndef OMCIRON_API_COMMON_ATTRIBUTE_INT32ATTRIBUTE_HPP_
#define OMCIRON_API_COMMON_ATTRIBUTE_INT32ATTRIBUTE_HPP_

#include <vector>

#include "omicron/api/common/attribute/DataAttribute.hpp"

namespace omi
{

// TODO: DOC
class Int32Attribute : public DataAttribute
{
    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The data type this attribute is storing.
     */
    typedef arc::int32 DataType;
    // TODO: ArrayType

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for int32 attributes.
     */
    OMI_API_GLOBAL static Type kTypeInt32;

    //--------------------------------------------------------------------------
    //                               PUBLIC STRUCTS
    //--------------------------------------------------------------------------

    // TODO: Doc
    struct Int32Storage : public Attribute::Storage
    {
        std::vector<DataType> m_data;

        Int32Storage()
        {
        }

        template<typename T_InputIterator>
        Int32Storage(
                const T_InputIterator& first,
                const T_InputIterator& last)
            : m_data(first, last)
        {
        }
    };

    // TODO: DOC
    struct Int32Definition : public DataAttribute::DataDefinition
    {
        Int32Definition()
            : DataAttribute::DataDefinition(kTypeInt32, 0)
        {
            m_storage = new Int32Storage();
        }

        template<typename T_InputIterator>
        Int32Definition(
                const T_InputIterator& first,
                const T_InputIterator& last,
                std::size_t tuple_size)
            : DataAttribute::DataDefinition(kTypeInt32, tuple_size)
        {
            m_storage = new Int32Storage(first, last);
        }
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty int32 attribute.
     *
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL Int32Attribute(bool immutable = true);

    // TODO: doc
    OMI_API_GLOBAL Int32Attribute(DataType value, bool immutable = true);

    // TODO: doc
    template<typename T_InputIterator>
    OMI_API_GLOBAL Int32Attribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            std::size_t tuple_size = 0,
            bool immutable = true)
        : DataAttribute(new Int32Definition(first, last, tuple_size), immutable)
    {
    }

    // TODO: doc
    OMI_API_GLOBAL Int32Attribute(
            const std::vector<DataType>& values,
            std::size_t tuple_size = 0,
            bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid int32 attribute this will construct
     * an invalid Int32Attribute.
     */
    OMI_API_GLOBAL Int32Attribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given attribute.
     */
    OMI_API_GLOBAL Int32Attribute(const Int32Attribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~Int32Attribute();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the first integer value of this attribute.
     *
     * \param throw_on_error Whether an exception will be thrown if the value
     *                       cannot be retrieved.
     * \param default_value The value which will be returned if it cannot be
     *                      retrieved from this attribute and throw_on_error is
     *                      ```false```.
     *
     * \throw arc::ex::ValueError If this attribute is not valid.
     * \throw arc::ex::IndexOutOfBoundsError If this attribute has no values.
     */
    OMI_API_GLOBAL DataType get_value(
            bool throw_on_error = false,
            DataType default_value = 0) const;

    // TODO: get values

    // TODO: set value

    // TODO: set values

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL virtual bool check_type(Type type) const;

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the internal storage of this attribute casted to an
     *        Int32Storage tyoe.
     *
     * \throws arc::ex::ValueError If this attribute is not valid.
     */
    Int32Storage* get_storage(bool throw_on_error) const;
};

} // namespace omi

#endif
