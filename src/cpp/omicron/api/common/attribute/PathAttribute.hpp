/*!
 * \file
 * \author David Saxon
 */
#ifndef OMCIRON_API_COMMON_ATTRIBUTE_PATHATTRIBUTE_HPP_
#define OMCIRON_API_COMMON_ATTRIBUTE_PATHATTRIBUTE_HPP_

#include <vector>

#include <arcanecore/io/sys/Path.hpp>

#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{

/*!
 * \brief A DataAttribute that holds signed 32-bit integers.
 */
class PathAttribute : public DataAttribute
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The data type this attribute is storing.
     */
    typedef arc::io::sys::Path DataType;

    /*!
     * \brief The array type that is used to hold this attribute's data.
     */
    typedef std::vector<DataType> ArrayType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for PathAttributes.
     */
    OMI_API_GLOBAL static Type kTypePath;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The storage type used by PathAttributes.
     */
    typedef DataAttribute::TypedDataStorage<
            PathAttribute,
            DataType
    > PathStorage;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty PathAttribute.
     *
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL PathAttribute(bool immutable = true);

    /*!
     * \brief Constructs a new PathAttribute with a single value and a tuple
     *        size of 0.
     *
     * \param value The single value of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL PathAttribute(DataType value, bool immutable = true);

    /*!
     * \brief Constructs a new PathAttribute using a copy of the data described
     *        by the given iterators.
     *
     * \param first The starting iterator of the data.
     * \param last The iterator past-the-end of the data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    template<typename T_InputIterator>
    PathAttribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            std::size_t tuple_size = 0,
            bool immutable = true)
        : DataAttribute(
                kTypePath,
                immutable,
                new PathStorage(first, last, tuple_size)
        )
    {
    }

    /*!
     * \brief Constructs a new PathAttribute using a copy of the data in the
     *        given array.
     *
     * \param values The data to copy to us as this attribute's data.
     * \param tuple_size The tuple size of this attribute.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL PathAttribute(
            const ArrayType& values,
            std::size_t tuple_size = 0,
            bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid PathAttribute this will construct
     * a null attribute and the reference count will not be increased.
     */
    OMI_API_GLOBAL PathAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given attribute.
     *
     * If the given attribute is invalid this will construct a null attribute
     * and the reference count will not be increased.
     */
    OMI_API_GLOBAL PathAttribute(const PathAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~PathAttribute();

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
        get_storage<PathStorage>()->m_data =
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
