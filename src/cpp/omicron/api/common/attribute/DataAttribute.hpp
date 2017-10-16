/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_

#include <vector>

#include "omicron/api/common/attribute/Attribute.hpp"

// TODO: REMOVE ME
#include <iostream>


namespace omi
{

/*!
 * \brief The DataAttribute class is the base class that all data attribute
 *        types inherit from.
 *
 * A DataAttribute is a generic container for zero or more instances of an
 * specific type.
 * DataAttribute's also hold a meta-data field: tuple size which can be used to
 * define the size that values should be grouped into. This field is not
 * enforced however so isn't required to match the number of values in the
 * Attribute. A tuple size of 0 denotes the data is not grouped in any way.
 */
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
    //                                  STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief The Abstract base class of internal storage type of
     *        DataAttributes.
     */
    class DataStorage : public Attribute::Storage
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        /*!
         * \brief The tuple size of the data.
         */
        std::size_t m_tuple_size;

        /*!
         * \brief The currently cached hash of this storage - if the hash is
         *        (0, 0) then there is not currently cached hash.
         */
        mutable Hash m_cached_hash;

        //-----------------------C O N S T R U C T O R S------------------------

        /*!
         * \brief Creates new empty DataStorage with the given tuple size.
         */
        OMI_API_EXPORT DataStorage(std::size_t tuple_size);

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_EXPORT virtual ~DataStorage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        OMI_API_EXPORT virtual bool is_data_pure_immutable() const override;

        OMI_API_EXPORT virtual bool is_data_pure_mutable() const override;

        OMI_API_EXPORT virtual Storage* as_pure_immutable() override;

        OMI_API_EXPORT virtual Storage* as_pure_mutable() override;

        /*!
         * \brief Returns the number of values in this DataStorage.
         */
        virtual std::size_t get_size() const = 0;

        /*!
         * \brief Computes the hash of this storage.
         */
        OMI_API_EXPORT void compute_hash(
                const void* data,
                std::size_t length,
                arc::uint64 seed,
                Hash& out_hash) const;
    };

    //--------------------------------------------------------------------------
    //                               TYPED STORAGE
    //--------------------------------------------------------------------------

    /*!
     * \brief Type specific implementation of DataStorage.
     *
     * This implementation should be used by classes that inherit from
     * DataAttribute.
     *
     * \tparam T_DataType The data type of the attribute and hence the data
     *                    type this storage will hold.
     */
    template<typename T_DataType>
    class TypedDataStorage : public DataStorage
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        /*!
         * \brief The internal data of this storage.
         */
        std::vector<T_DataType> m_data;

        //-----------------------C O N S T R U C T O R S------------------------

        /*!
         * \brief Creates new empty TypedDataStorage with the given tuple size
         */
        TypedDataStorage(std::size_t tuple_size)
            : DataStorage(tuple_size)
        {
        }

        /*!
         * \brief Creates new TypedDataStorage using a copy of the data
         *        described by the given iterators.
         *
         * \param first The starting iterator of the data.
         * \param last The iterator one-past-the-end of the data.
         * \param tuple_size The tuple size of the data.
         */
        template<typename T_InputIterator>
        TypedDataStorage(
                const T_InputIterator& first,
                const T_InputIterator& last,
                std::size_t tuple_size)
            : DataStorage(tuple_size)
            , m_data     (first, last)
        {
        }

        //-------------------------D E S T R U C T O R--------------------------

        virtual ~TypedDataStorage()
        {
        }

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------

        virtual bool equals(const Attribute::Storage* other) const override
        {
            // cast
            const TypedDataStorage<T_DataType>* casted =
                dynamic_cast<const TypedDataStorage<T_DataType>*>(
                    other
                );
            if(casted == nullptr)
            {
                return false;
            }

            // check tuple size
            if(m_tuple_size != casted->m_tuple_size)
            {
                return false;
            }

            // check data size
            if(m_data.size() != casted->m_data.size())
            {
                return false;
            }

            // check data
            for(std::size_t i = 0; i < m_data.size(); ++i)
            {
                if(m_data[i] != casted->m_data[i])
                {
                    return false;
                }
            }

            return true;
        }

        virtual bool less_than(const Storage* other) const override
        {
            // cast
            const TypedDataStorage<T_DataType>* casted =
                dynamic_cast<const TypedDataStorage<T_DataType>*>(
                    other
                );
            if(casted == nullptr)
            {
                return true;
            }

            return m_data < casted->m_data;
        }

        virtual Hash get_hash(arc::uint64 seed) const override
        {
            // hash need recomputing?
            if(m_cached_hash.part1 == 0 && m_cached_hash.part2 == 0)
            {
                // is there actually anything to hash?
                if(!m_data.empty())
                {
                    const T_DataType& m = m_data[0];
                    compute_hash(
                        static_cast<const void*>(&m),
                        m_data.size() * sizeof(T_DataType),
                        seed,
                        m_cached_hash
                    );
                }
                else
                {
                    // use the seed and tuple size since there's nothing to hash
                    m_cached_hash.part1 = seed;
                    m_cached_hash.part2 = m_tuple_size;
                }
            }
            return m_cached_hash;
        }

        virtual void invalidate_hash() override
        {
            m_cached_hash.part1 = 0;
            m_cached_hash.part2 = 0;
        }

        virtual Storage* copy_for_overwrite(bool soft) override
        {
            if(soft)
            {
                // soft overwrite - so copy everything
                return new TypedDataStorage<T_DataType>(
                    m_data.begin(),
                    m_data.end(),
                    m_tuple_size
                );
            }

            // just copy the tuple size
            return new TypedDataStorage<T_DataType>(m_tuple_size);
        }

        virtual void string_repr(
                std::size_t indentation,
                arc::str::UTF8String& s) const override
        {
            // indentation?
            if(indentation > 0)
            {
                s << (arc::str::UTF8String(" ") * indentation);
            }
            s << "[";
            // values
            for(std::size_t i = 0; i < m_data.size(); ++i)
            {
                s << m_data[i];
                if(i < m_data.size() - 1)
                {
                    s << ", ";
                }
            }
            s << "]";
        }

        virtual std::size_t get_size() const override
        {
            return m_data.size();
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
    OMI_API_EXPORT DataAttribute();

    /*!
     * \brief Constructs a new reference count of the given Attribute.
     *
     * If the given attribute is not a valid data attribute this construct an
     * invalid DataAttribute.
     */
    OMI_API_EXPORT DataAttribute(const Attribute& other);

    /*!
     * \brief Constructs a new reference count of the given DataAttribute.
     */
    OMI_API_EXPORT DataAttribute(const DataAttribute& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT ~DataAttribute();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the number of values this attribute holds.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT std::size_t get_size() const;

    /*!
     * \brief Returns the tuple size of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     */
    OMI_API_EXPORT std::size_t get_tuple_size() const;

    /*!
     * \brief Sets the tuple size of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_EXPORT void set_tuple_size(std::size_t tuple_size);

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    // override
    OMI_API_EXPORT DataAttribute(Definition* def);

    // override
    OMI_API_EXPORT DataAttribute(Type type, bool immutable, Storage* storage);

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual bool check_type(Type type) const override;
};

} // namespace omi

#endif
