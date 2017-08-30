/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_
#define OMICRON_API_COMMON_ATTRIBUTE_DATAATTRIBUTE_HPP_

#include <vector>

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
    //                                  STORAGE
    //--------------------------------------------------------------------------

    // TODO: Doc
    class DataStorage : public Attribute::Storage
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        std::size_t m_tuple_size;

        //-----------------------C O N S T R U C T O R S------------------------

        // TODO: DOC
        OMI_API_GLOBAL DataStorage(std::size_t tuple_size);

        //-------------------------D E S T R U C T O R--------------------------

        OMI_API_GLOBAL virtual ~DataStorage();

        //-----------P U B L I C    M E M B E R    F U N C T I O N S------------
        // TODO: DOC
        virtual std::size_t get_size() const = 0;

    };

    //--------------------------------------------------------------------------
    //                               TYPED STORAGE
    //--------------------------------------------------------------------------

    // TODO: DOC
    template<typename T_BaseType, typename T_DataType>
    class TypedDataStorage : public DataStorage
    {
    public:

        //------------------P U B L I C    A T T R I B U T E S------------------

        std::vector<T_DataType> m_data;

        //-----------------------C O N S T R U C T O R S------------------------

        // TODO: DOC
        TypedDataStorage(std::size_t tuple_size)
            : DataStorage(tuple_size)
        {
        }

        // TODO: DOC
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

        // TODO: DOC
        virtual Storage* copy_for_overwrite(bool soft)
        {
            if(soft)
            {
                // soft overwrite - so copy everything
                return new TypedDataStorage<T_BaseType, T_DataType>(
                    m_data.begin(),
                    m_data.end(),
                    m_tuple_size
                );
            }

            // just copy the tuple size
            return new TypedDataStorage<T_BaseType, T_DataType>(m_tuple_size);
        }

        // override
        virtual bool equals(const Attribute::Storage* other)
        {
            // cast
            const TypedDataStorage<T_BaseType, T_DataType>* casted =
                dynamic_cast<const TypedDataStorage<T_BaseType, T_DataType>*>(
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

        // override
        virtual void string_repr(arc::str::UTF8String& s) const
        {
            // get name
            s << T_BaseType::get_type_string();
            // tuple size
            s << "(" << m_tuple_size << "): [";
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

        // override
        virtual std::size_t get_size() const
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

    /*!
     * \brief Sets the tuple size of this attribute.
     *
     * \throw arc::ex::StateError If this attribute is not valid.
     * \throw arc::ex::IllegalActionError If this attribute is immutable.
     */
    OMI_API_GLOBAL void set_tuple_size(std::size_t tuple_size);

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTORS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL DataAttribute(Definition* def);

    // override
    OMI_API_GLOBAL DataAttribute(Type type, bool immutable, Storage* storage);

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_GLOBAL virtual bool check_type(Type type) const;
};

} // namespace omi

#endif
