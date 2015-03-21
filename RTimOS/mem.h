/**
  ******************************************************************************
  * @file      mem.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      08/03/2015
  * @brief     Gestion de la memoire de l'OS
  ******************************************************************************
*/

/**
   ******************************************************************************
 * Include
 *
 */

/**
   ******************************************************************************
 * Exported functions
 *
 */
void mem_ClearZone(void*const _pStartAdress, unsigned long length_byte);
void mem_SetZone(void*const _pStartAdress, unsigned long _value, unsigned long length_byte);


/***********************************END OF FILE*******************************/
