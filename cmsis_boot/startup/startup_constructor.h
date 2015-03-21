/***************************************************************************************************
 *
 * @author	Duclos Timothe
 * @date	28/01/2015
 * @file	startup_constructor.h
 * @brief	Gestion des constructeurs
 *
 ***************************************************************************************************/

#ifndef STARTUP_CONSTRUCTOR_H
#define STARTUP_CONSTRUCTOR_H

/***************************************************************************************************
 * Includes
 */
#include <sys/types.h>

/***************************************************************************************************
 * Exported macros
 */
#define Constructor(pRIORITY)	__attribute__((constructor(pRIORITY)))

/***************************************************************************************************
 * Exported defines
 */
#define CONSTRUCTOR_PRIORITY_1	101
#define CONSTRUCTOR_PRIORITY_2	102
#define CONSTRUCTOR_PRIORITY_3	103

/***************************************************************************************************
 * Exported variables
 */
extern void (*__preinit_array_start 	[]) (void) __attribute__((weak));
extern void (*__preinit_array_end 	[]) (void) __attribute__((weak));
extern void (*__init_array_start 	[]) (void) __attribute__((weak));
extern void (*__init_array_end 		[]) (void) __attribute__((weak));

/***************************************************************************************************
 * Exported function prototype
 */
void __libc_init_array();

#endif /** STARTUP_CONSTRUCTOR_H */
