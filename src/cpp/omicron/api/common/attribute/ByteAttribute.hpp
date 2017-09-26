/*!
 * \file
 * \author David Saxon
 */
#ifndef OMCIRON_API_COMMON_ATTRIBUTE_BYTEATTRIBUTE_HPP_
#define OMCIRON_API_COMMON_ATTRIBUTE_BYTEATTRIBUTE_HPP_

#include <vector>

#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{

/*!
 * \brief A DataAttribute that holds bytes (a.k.a char).
 */
class ByteAttribute : public DataAttribute
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The data type this attribute is storing.
     */
    typedef char DataType;

    /*!
     * \brief The array type that is used to hold this attribute's data.
     */
    typedef std::vector<DataType> ArrayType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for ByteAttributes.
     */
    OMI_API_GLOBAL static Type kTypeByte;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The storage type used by ByteAttributes.
     */
    typedef DataAttribute::TypedDataStorage<DataType> ByteStorage;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty immutable ByteAttribute with a tuple size
     *        of 0.
     */
    OMI_API_GLOBAL ByteAttribute();

    /*!
     * \brief Constructs a new ByteAttribute with a single value and a tuple
     *        size of 0.
     *
     * \param value The single value of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL ByteAttribute(DataType value, bool immutable = true);

    /*!
     * \brief Constructs a new ByteAttribute using a copy of the data described
     *        by the given iterators.
     *
     * \param first The starting iterator of the data.
     * \param last The iterator past-the-end of the data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    template<typename T_InputIterator>
    ByteAttribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            std::size_t tuple_size = 0,
            bool immutable = true)
        : DataAttribute(
                kTypeByte,
                immutable,
                new ByteStorage(first, last, tuple_size)
        )
    {
    }

    /*!
     * \brief Constructs a new ByteAttribute using a copy of the data in the
     *        given array.
     *
     * \param values The data to copy to us as this attribute's data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL ByteAttribute(
            const ArrayType& values,
            std::size_t tuple_size = 0,
            bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid Int64Attribute this will construct
     * a null attribute and the reference count will not be increased.
     */
    OMI_API_GLOBAL ByteAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given attribute.
     *
     * If the given attribute is invalid this will construct a null attribute
     * and the reference count will not be increased.
     */
    OMI_API_GLOBAL ByteAttribute(const ByteAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~ByteAttribute();

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
     * \brief Returns the first value of this attribute.
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
     * \brief Returns the single value at the given index.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IndexOutOfBoundsError If the index is out of this
     *                                       attribute's bounds.
     */
    OMI_API_GLOBAL const DataType& at(std::size_t index) const;

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
        get_storage<ByteStorage>()->m_data =
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

    /*!
     * \brief Sets the single value at the given index.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     * \throw arc::ex::IndexOutOfBoundsError If the index is out of this
     *                                       attribute's bounds.
     */
    OMI_API_GLOBAL void set_at(std::size_t index, DataType value);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual bool check_type(Type type) const override;
};

} // namespace omi

#endif