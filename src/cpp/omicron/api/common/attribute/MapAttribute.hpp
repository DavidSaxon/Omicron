/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_MAPATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_MAPATTRIBUTE_HPP_

#include <unordered_map>

#include "omicron/api/common/attribute/Attribute.hpp"


namespace omi
{

/*!
 * \brief A MapAttribute holds zero or more key value pairs of names
 *        (arc::str::UTF8String) and attributes.
 *
 * MapAttributes are unordered but can be used to build attribute hierarchies by
 * nested MapAttributes.
 *
 * Any function that expects a name, also supports nested name, e.g. using the
 * name ```"group.attr1"``` for the MapAttribute::has() function will
 * check if the MapAttribute has another MapAttribute under the name
 * ```"group"``` and that this MapAttribute has an attribute under the name
 * ```"attr1"```.
 *
 * \note Immutable MapAttributes only guarantee that that structure of the map
 *       is immutable. If a map has mutable child attributes they can still be
 *       modified. In order to have a pure immutable MapAttribute, itself and
 *       all of its ancestors must be immutable.
 */
class MapAttribute : public Attribute
{
public:

    //--------------------------------------------------------------------------
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief The type used to store MapAttribute's data.
     */
    typedef std::unordered_map<arc::str::UTF8String, Attribute> DataType;

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The type identifier for MapAttributes.
     */
    OMI_API_EXPORT static Type kTypeMap;

    //--------------------------------------------------------------------------
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The internal storage type of MapAttributes.
     */
    class MapStorage : public Attribute::Storage
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
        mutable std::unordered_map<arc::str::UTF8String, Hash> m_sub_hashes;

        //-----------------------C O N S T R U C T O R S------------------------

        /*!
         * \brief Creates a new empty MapStorage.
         */
        OMI_API_EXPORT MapStorage();

        /*!
         * \brief Creates new MapStorage using a copy of that data described
         *        by the given iterators as the initial data.
         */
        template<typename T_InputIterator>
        MapStorage(const T_InputIterator& first, const T_InputIterator& last)
            : Attribute::Storage()
            , m_data            (first, last)
        {
        }

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_EXPORT virtual ~MapStorage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        OMI_API_EXPORT virtual bool equals(const Storage* other) const override;

        OMI_API_EXPORT
        virtual bool less_than(const Storage* other) const override;

        OMI_API_EXPORT virtual bool is_data_pure_immutable() const override;

        OMI_API_EXPORT virtual bool is_data_pure_mutable() const override;

        OMI_API_EXPORT virtual Storage* as_pure_immutable() override;

        OMI_API_EXPORT virtual Storage* as_pure_mutable() override;

        OMI_API_EXPORT virtual Hash get_hash(arc::uint64 seed) const override;

        OMI_API_EXPORT virtual void invalidate_hash() override;

        OMI_API_EXPORT virtual Storage* copy_for_overwrite(bool soft) override;

        OMI_API_EXPORT virtual void string_repr(
                std::size_t indentation,
                arc::str::UTF8String& s) const override;
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a new empty MapAttribute.
     *
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_EXPORT MapAttribute(bool immutable = true);

    /*!
     * \brief Constructs a new MapAttribute using a copy of the data described
     *        by the given iterators as the initial contents of the attribute.
     *
     * \param first The starting iterator of the MapAttribute's data.
     * \param last The iterator one-past-the-end of the MapAttribute's data.
     * \param immutable Whether this attribute is immutable or not.
     */
    template<typename T_InputIterator>
    MapAttribute(
            const T_InputIterator& first,
            const T_InputIterator& last,
            bool immutable = true)
        : Attribute(kTypeMap, immutable, new MapStorage(first, last))
    {
    }

    /*!
     * \brief Constructs a new MapAttribute using a copy of the data from the
     *        given map.
     *
     * \param data The data to copy the contents of this attribute's data from.
     * \param immutable Whether this attribute is immutable or not.
     */
    OMI_API_EXPORT MapAttribute(const DataType& data, bool immutable = true);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid map attribute this will construct
     * a null attribute and the reference count will not be increased.
     */
    OMI_API_EXPORT MapAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is invalid this will construct a null attribute
     * and the reference count will not be increased.
     */
    OMI_API_EXPORT MapAttribute(const MapAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~MapAttribute();

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::KeyError If there is not attribute under the given name
     *                           in this MapAttribute.
     */
    OMI_API_EXPORT const Attribute& operator[](
            const arc::str::UTF8String& name) const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::KeyError If there is not attribute under the given name
     *                           in this MapAttribute.
     */
    OMI_API_EXPORT Attribute& operator[](const arc::str::UTF8String& name);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the number of entries in this MapAttribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT std::size_t get_size() const;

    /*!
     * \brief Returns whether this MapAttribute has no entries.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT bool is_empty() const;

    /*!
     * \brief Returns the internal map structure of this MapAttribute.
     *
     * \note Using this structure is this fastest way to iterate over the
     *       contents of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT const DataType& get_values() const;

    /*!
     * \brief Returns the names (keys) of the entries in this MapAttribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT std::vector<arc::str::UTF8String> get_names() const;

    /*!
     * \brief Returns the attributes (values) of the entries in this
     *        MapAttribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT std::vector<Attribute> get_attributes() const;

    /*!
     * \brief Returns whether there is an entry in the map with the given name.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT bool has(const arc::str::UTF8String& name) const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::KeyError If there is not attribute under the given name
     *                           in this MapAttribute.
     */
    OMI_API_EXPORT const Attribute& get(const arc::str::UTF8String& name) const;

    /*!
     * \brief Returns the attribute in this MapAttribute under the given name.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::KeyError If there is not attribute under the given name
     *                           in this MapAttribute.
     */
    OMI_API_EXPORT Attribute& get(const arc::str::UTF8String& name);

    /*!
     * \brief Inserts the given attribute into the map under the provided name.
     *
     * \note If an attribute already exists under the name, it will be
     *       overridden.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_EXPORT void insert(
            const arc::str::UTF8String& name,
            const Attribute& attrribute);

    /*!
     * \brief Removes the attribute with the given name from this MapAttribute.
     *
     * \note Supports nested naming syntax.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throws arc::ex::KeyError If there is not attribute under the given name
     *                           in this MapAttribute.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_EXPORT void erase(const arc::str::UTF8String& name);

    /*!
     * \brief Replaces the current data of this MapAttribute with the given
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
        get_storage<MapStorage>()->m_data = DataType(first, last);
    }

    /*!
     * \brief Replaces the current data of this MapAttribute with the given
     *        data.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_EXPORT void set_values(const DataType& data);

    /*!
     * \brief Clears the contents of this MapAttribute - effectively replacing
     *        the current data with empty data.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_EXPORT void clear();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual bool check_type(Type type) const override;
};

} // namespace omi

#endif
