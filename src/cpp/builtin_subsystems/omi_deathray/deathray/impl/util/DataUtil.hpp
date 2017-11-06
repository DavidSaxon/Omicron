/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 * \brief Utility operations for interacting with data.
 */
#ifndef DEATHRAY_IMPL_UTIL_DATAUTIL_HPP_
#define DEATHRAY_IMPL_UTIL_DATAUTIL_HPP_

#include "deathray/api/API.h"


namespace death
{
namespace util
{
/*!
 * \brief Utility operations for interacting with data.
 */
namespace data
{

/*!
 * \brief Returns the width in bits for the given data type.
 */
DeathSize data_type_to_bit_width(DeathDataType data_type);

/*!
 * \brief Returns the width in bytes for the given data type.
 */
DeathSize data_type_to_byte_width(DeathDataType data_type);

} // namespace data
} // namespace util
} // namespace death

#endif
