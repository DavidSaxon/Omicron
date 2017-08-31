/*!
 * \file
 * \author David Saxon
 */
#ifndef OMCIRON_API_COMMON_ATTRIBUTE_INT16ATTRIBUTE_HPP_
#define OMCIRON_API_COMMON_ATTRIBUTE_INT16ATTRIBUTE_HPP_

#include <vector>

#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{

/*!
 * \brief A DataAttribute that holds signed 16-bit integers.
 */
class Int16Attribute : public DataAttribute
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The data type this attribute is storing.
     */
    typedef arc::int16 DataType;

    /*!
     * \brief The array type that is used to hold this attribute's data.
     */
    typedef std::vector<DataType> ArrayType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for Int16Attributes.
     */
    OMI_API_GLOBAL static Type kTypeInt16;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The storage type used by Int16Attributes.
     */
    typedef DataAttribute::TypedDataStorage<
            Int16Attribute,
            DataType
    > Int16Storage;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty Int16Attribute.
     *
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL Int16Attribute(bool immutable = true);

    /*!
     * \brief Constructs a new Int16Attribute with a single value and a tuple
     *        size of 0.
     *
     * \param value The single value of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL Int16Attribute(DataType value, bool immutable = true);

    /*!
     * \brief Constructs a new Int16Attribute using a copy of the data described
     *        by the given iterators.
     *
     * \param first The starting iterator of the data.
     * \param last The iterator past-the-end of the data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    template<typename T_InputIterator>
    Int16Attribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            std::size_t tuple_size = 0,
            bool immutable = true)
        : DataAttribute(
                kTypeInt16,
                immutable,
                new Int16Storage(first, last, tuple_size)
        )
    {
    }

    /*!
     * \brief Constructs a new Int16Attribute using a copy of the data in the
     *        given array.
     *
     * \param values The data to copy to us as this attribute's data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL Int16Attribute(
            const ArrayType& values,
            std::size_t tuple_size = 0,
            bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid Int16Attribute this will construct
     * a null attribute and the reference count will not be increased.
     */
    OMI_API_GLOBAL Int16Attribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given attribute.
     *
     * If the given attribute is invalid this will construct a null attribute
     * and the reference count will not be increased.
     */
    OMI_API_GLOBAL Int16Attribute(const Int16Attribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~Int16Attribute();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the of this attribute as a string.
     */
    OMI_API_GLOBAL static arc::str::UTF8String get_type_string();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the first integer value of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IndexOutOfBoundsError If this attribute has no values.
     */
    OMI_API_GLOBAL DataType get_value() const;

    /*!
     * \brief Returns the array of values of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_GLOBAL const ArrayType& get_values() const;

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
        // valid?
        check_state("set_values() used on an invalid attribute");

        prepare_modifcation();
        get_storage<Int16Storage>()->m_data =
            std::vector<DataType>(first, last);
    }

    /*!
     * \brief Sets the value of this attribute to be a copy of the data within
     *        the given array.
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
};

} // namespace omi

#endif
