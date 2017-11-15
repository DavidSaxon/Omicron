/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_COMMON_HASH_HPP_
#define OMICRON_API_COMMON_HASH_HPP_


namespace omi
{

/*!
 * \brief Simple data structure that stores the values of a 128-bit hash.
 *
 * \note This object does not provide functionality for hashing data, it is
 *       purely used for storing and checking the results of hashing
 *       algorithms.
 */
class Hash
{
public:

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The first 64-bits of the hash.
     */
    arc::uint64 part1;
    /*!
     * \brief The second 64-bits of the hash.
     */
    arc::uint64 part2;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new zero'd hash.
     */
    Hash()
        : part1(0)
        , part2(0)
    {
    }

    /*!
     * \brief Creates a new 128-bit hash from the 2 64-bit values.
     */
    Hash(arc::uint64 in_part1, arc::uint64 in_part2)
        : part1(in_part1)
        , part2(in_part2)
    {
    }

    /*!
     * \brief Copy constructor.
     */
    Hash(const Hash& other)
        : part1(other.part1)
        , part2(other.part2)
    {
    }

    /*!
     * \brief Move constructor.
     */
    Hash(Hash&& other)
        : part1(other.part1)
        , part2(other.part2)
    {
        other.part1 = 0;
        other.part2 = 0;
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Hash()
    {
    }

    //--------------------------------------------------------------------------
    //                                 OPERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Assignment operator.
     */
    Hash& operator=(const Hash& other)
    {
        part1 = other.part1;
        part2 = other.part2;
        return *this;
    }

    /*!
     * \brief Move assignment operator.
     */
    Hash& operator=(Hash&& other)
    {
        part1 = other.part1;
        part2 = other.part2;
        other.part1 = 0;
        other.part2 = 0;
        return *this;
    }

    /*!
     * \brief Equality operator.
     */
    bool operator==(const Hash& other) const
    {
        return part1 == other.part1 && part2 == other.part2;
    }

    /*!
     * \brief Inequality operator.
     */
    bool operator!=(const Hash& other) const
    {
        return !((*this) == other);
    }
};

} // namespace omi

#endif
