/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_ARRAYATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_ARRAYATTRIBUTE_HPP_

#include <vector>

#include "omicron/api/common/attribute/Attribute.hpp"


namespace omi
{

/*!
 * \brief An ArrayAttribute holds zero or more ordered child attributes.
 *
 * \note Immutable ArrayAttribute only guarantee that that structure of the
 *       array is immutable. If an array has mutable child attributes they can
 *       still be modified. In order to have a pure immutable ArrayAttribute,
 *       itself and all of its ancestors must be immutable.
 */
class ArrayAttribute : public Attribute
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    typedef std::vector<Attribute> DataType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for ArrayAttributes.
     */
    OMI_API_GLOBAL static Type kTypeArray;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal storage type of ArrayAttributes.
     */
    class ArrayStorage : public Attribute::Storage
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        /*!
         * \brief The map data.
         */
        DataType m_data;

        /*!
         * \brief The currently cached hash of this storage - if the hash is
         *        (0, 0) then there is not currently cached hash.
         */
        mutable Hash m_cached_hash;
        /*!
         * \brief The last checked hashes of the sub-attributes, these are
         *        checked to see if the hash is out of date.
         */
        mutable std::vector<Hash> m_sub_hashes;

        //-----------------------C O N S T R U C T O R S------------------------

        /*!
         * \brief Creates a new empty ArrayStorage.
         */
        OMI_API_GLOBAL ArrayStorage();

        /*!
         * \brief Creates new ArrayStorage using a copy of that data described
         *        by the given iterators as the initial data.
         */
        template<typename T_InputIterator>
        ArrayStorage(const T_InputIterator& first, const T_InputIterator& last)
            : Attribute::Storage()
            , m_data            (first, last)
        {
        }

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_GLOBAL virtual ~ArrayStorage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        OMI_API_GLOBAL virtual bool equals(const Storage* other) const override;

        OMI_API_GLOBAL
        virtual bool less_than(const Storage* other) const override;

        OMI_API_GLOBAL virtual bool is_data_pure_immutable() const override;

        OMI_API_GLOBAL virtual bool is_data_pure_mutable() const override;

        OMI_API_GLOBAL virtual Storage* as_pure_immutable() override;

        OMI_API_GLOBAL virtual Storage* as_pure_mutable() override;

        OMI_API_GLOBAL virtual Hash get_hash(arc::uint64 seed) const override;

        OMI_API_GLOBAL virtual void invalidate_hash() override;

        OMI_API_GLOBAL virtual Storage* copy_for_overwrite(bool soft) override;

        OMI_API_GLOBAL virtual void string_repr(
                std::size_t indentation,
                arc::str::UTF8String& s) const override;
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty ArrayAttribute.
     *
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL ArrayAttribute(bool immutable = true);

    /*!
     * \brief Constructs a new ArrayAttribute using a copy of the data described
     *        by the given iterators as the initial contents of the attribute.
     *
     * \param first The starting iterator of the ArrayAttribute's data.
     * \param last The iterator one-past-the-end of the ArrayAttribute's data.
     * \param immutable Whether this attribute is immutable or not.
     */
    template<typename T_InputIterator>
    ArrayAttribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            bool immutable = true)
        : Attribute(kTypeArray, immutable, new ArrayStorage(first, last))
    {
    }

    /*!
     * \brief Constructs a new ArrayAttribute using a copy of the data from the
     *        given map.
     *
     * \param data The data to copy the contents of this attribute's data from.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_GLOBAL ArrayAttribute(const DataType& data, bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid map attribute this will construct
     * a null attribute and the reference count will not be increased.
     */
    OMI_API_GLOBAL ArrayAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is invalid this will construct a null attribute
     * and the reference count will not be increased.
     */
    OMI_API_GLOBAL ArrayAttribute(const ArrayAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual ~ArrayAttribute();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     */
    OMI_API_GLOBAL const Attribute& operator[](std::size_t index) const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     */
    OMI_API_GLOBAL Attribute& operator[](std::size_t index);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the number of entries in this ArrayAttribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_GLOBAL std::size_t get_size() const;

    /*!
     * \brief Returns whether this ArrayAttribute has no elements.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_GLOBAL bool is_empty() const;

    /*!
     * \brief Returns the internal array structure of this ArrayAttribute.
     *
     * \note Using this structure is this fastest way to iterate over the
     *       contents of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_GLOBAL const DataType& get_values() const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     */
    OMI_API_GLOBAL const Attribute& get(std::size_t index) const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     */
    OMI_API_GLOBAL Attribute& get(std::size_t index);

    /*!
     * \brief Returns the first attribute in this array.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the array is empty.
     */
    OMI_API_GLOBAL const Attribute& front() const;

    /*!
     * \brief Returns the first attribute in this array.
     *
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the array is empty.
     */
    OMI_API_GLOBAL Attribute& front();

    /*!
     * \brief Returns the last attribute in this array.
     *
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the array is empty.
     */
    OMI_API_GLOBAL const Attribute& back() const;

    /*!
     * \brief Returns the last attribute in this array.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the array is empty.
     */
    OMI_API_GLOBAL Attribute& back();

    /*!
     * \brief Sets the attribute at the given index.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void set(std::size_t index, const Attribute& attribute);

    /*!
     * \brief Extends the length of this array by 1 and inserts the given
     *        attribute into the new last position.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
     OMI_API_GLOBAL void push_back(const Attribute& attribute);

    /*!
     * \brief Inserts the given attribute into the array at the provided index
     *        and shifts all elements at the index and beyond it 1 step along.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range + 1 of this array.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void insert(std::size_t index, const Attribute& attrribute);

    /*!
     * \brief Removes the attribute at this given index from this ArrayAttribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::IndexOutOfBoundsError If the given index is outside of
     *                                        the range of this array.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void erase(std::size_t index);

    /*!
     * \brief Replaces the current data of this ArrayAttribute with the given
     *        data.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    template<typename T_InputIterator>
    void set_values(
            const T_InputIterator& first,
            const T_InputIterator& last)
    {
        // valid?
        check_state("set_values() used on an invalid attribute");
        prepare_modifcation();
        get_storage<ArrayStorage>()->m_data = DataType(first, last);
    }

    /*!
     * \brief Replaces the current data of this ArrayAttribute with the given
     *        data.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void set_values(const DataType& data);

    /*!
     * \brief Clears the contents of this ArrayAttribute - effectively replacing
     *        the current data with empty data.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void clear();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_GLOBAL virtual bool check_type(Type type) const override;
};

} // namespace omi

#endif
