/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_TYPES_GEOMETRY_HPP_
#define OMICRON_API_ASSET_TYPES_GEOMETRY_HPP_

#include "omicron/api/asset/types/AbstractAsset.hpp"


namespace omi
{
namespace asset
{

// TODO: DOC
class Geometry : public AbstractAsset
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    OMI_API_EXPORT Geometry();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Geometry();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    OMI_API_EXPORT virtual void init(const omi::MapAttribute& data) override;

    // override
    OMI_API_EXPORT virtual void release() override;

    /*!
     * \brief Returns the number of vertices in this geometry.
     *
     * \note This number can be different from the size of the position data
     *        returned by get_positions() since the data may be indexed.
     */
    OMI_API_EXPORT std::size_t get_vertex_size() const;

    /*!
     * \brief Returns a reference to this geometry's position data for read-only
     *        purposes.
     */
    OMI_API_EXPORT const std::vector<float>& get_positions() const;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class GeometryImpl;
    GeometryImpl* m_impl;
};

} // namespace asset
} // namespace omi


#endif
