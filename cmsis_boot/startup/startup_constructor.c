/***************************************************************************************************
 *
 * @author	Duclos Timothe
 * @date	28/01/2015
 * @file	startup_constructor.h
 * @brief	Gestion des constructeurs
 *
 ***************************************************************************************************/

/***************************************************************************************************
 * Includes
 */
#include "startup_constructor.h"

/***************************************************************************************************
 * Exported function definition
 */
void
__libc_init_array()
{
  size_t count;
  size_t i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}
