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
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The data type this attribute is storing.
     */
    typedef arc::int32 DataType;
    /*!
     * \brief The array type that is used to return weak references to this
     *        attribute's data.
     */
    typedef std::vector<DataType> ArrayType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for int32 attributes.
     */
    OMI_API_GLOBAL static Type kTypeInt32;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    typedef DataAttribute::TypedDataStorage<
            Int32Attribute,
            DataType
    > Int32Storage;

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
    Int32Attribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            std::size_t tuple_size = 0,
            bool immutable = true)
        : DataAttribute(
                kTypeInt32,
                immutable,
                new Int32Storage(first, last, tuple_size)
        )
    {
    }

    // TODO: doc
    OMI_API_GLOBAL Int32Attribute(
            const ArrayType& values,
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
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: DOC
    OMI_API_GLOBAL static arc::str::UTF8String get_type_string();

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
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IndexOutOfBoundsError If this attribute has no values.
     */
    OMI_API_GLOBAL DataType get_value(
            bool throw_on_error = false,
            DataType default_value = 0) const;

    /*!
     * \brief Returns the array of values of this attribute.
     *
     * \param throw_on_error Whether an exception will be thrown if the value
     *                       cannot be retrieved.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_GLOBAL const ArrayType& get_values(bool throw_on_error = false);

    /*!
     * \brief Sets the value of this attribute to be a size 1 array holding the
     *        given value.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void set_value(DataType value);

    /*!
     * \brief Sets the value of this attribute to be a copy of the array
     *        specified by the first and last (one past the end) iterators.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    template<typename T_InputIterator>
    void set_values(const T_InputIterator& first, const T_InputIterator& last)
    {
        check_state();
        prepare_modifcation();
        get_storage<Int32Storage>()->m_data =
            std::vector<DataType>(first, last);
    }

    /*!
     * \brief Sets the value of this attribute to be a copy of the data within
     *        the given vector.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void set_values(const ArrayType& values);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL virtual bool check_type(Type type) const;

    /*!
     * \brief Convenience function which throws a arc::ex::StateError if this
     *        attribute is not valid.
     */
    OMI_API_GLOBAL void check_state() const;
};

} // namespace omi

#endif
