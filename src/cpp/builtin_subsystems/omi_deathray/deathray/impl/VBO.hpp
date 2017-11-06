/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_VBO_HPP_
#define DEATHRAY_IMPL_VBO_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "deathray/api/API.h"
#include "deathray/api/VBO.h"


namespace death
{

// TODO: DOC
class VBO
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    VBO(DeathVBOHandle handle);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~VBO();

    //--------------------------------------------------------------------------
    //                          PUBLIC: MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the number of values in this VBO
     */
    DeathSize get_size() const;

    /*!
     * \brief Returns the tuple size of this VBO.
     */
    DeathSize get_tuple_size() const;

    /*!
     * \brief Returns the data of this VBO.
     */
    const DeathFloat* get_data() const;

    /*!
     * \brief Implementation of the death_vbo_set_data function.
     */
    DeathError set_data(
            DeathDataType data_type,
            DeathSize size,
            DeathSize tuple_size,
            const void* data);

    /*!
     * \brief Releases any existing data held by this VBO.
     */
    void release();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class VBOImpl;
    VBOImpl* m_impl;
};

} // namespace death

//------------------------------------------------------------------------------
//                             API STRUCT DEFINITION
//------------------------------------------------------------------------------

struct DeathVBO
{
    death::VBO* impl;
    DeathVBO() : impl(nullptr) {};
};

#endif
