#include "omicron/api/asset/resource/OBJResource.hpp"


namespace omi
{
namespace asset
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class OBJResource::OBJResourceImpl
{
public:

    //-------------------------C O N S T R U C T O R S--------------------------

    OBJResourceImpl()
    {
    }

    //--------------------------D E S T R U C T O R S---------------------------

    ~OBJResourceImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void load(arc::io::sys::FileReader* reader)
    {
        // TODO:
    }

    void release()
    {
        // TODO:
    }
};

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

//---------------------------C O N S T R U C T O R S----------------------------

OMI_API_GLOBAL OBJResource::OBJResource()
    : m_impl(new OBJResourceImpl())
{
}

//-----------------------------D E S T R U C T O R------------------------------

OBJResource::~OBJResource()
{
    delete m_impl;
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_GLOBAL void OBJResource::load(arc::io::sys::FileReader* reader)
{
    m_impl->load(reader);
}

OMI_API_GLOBAL void OBJResource::release()
{
    m_impl->release();
}

} // namespace asset
} // namespace omi
