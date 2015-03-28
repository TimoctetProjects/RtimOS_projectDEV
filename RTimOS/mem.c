/**
  ******************************************************************************
  * @file      mem.c
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
#include "mem.h"
#include "RtimOS_Config.h"

/**
   ******************************************************************************
 * Private macro
 *
 */
/* @brief Macro for 4 bytes access */
#define __WriteValueToAdress_u32(aDRESS) \
	(*((volatile unsigned long*)(aDRESS)))

/* @brief Macros for 1 byte access */
#define __WriteValueToAdress_u8(aDRESS) \
	(*((volatile unsigned char*)(aDRESS)))

/**
   ******************************************************************************
 * Private variables
 *
 */
unsigned char heap[HEAPSIZE_bytes];

/**
   ******************************************************************************
 * Private function prototypes
 *
 */
static void* _mem_allocationinit(unsigned int _size);

void* (*mem_allocation)(unsigned int _size) = _mem_allocationinit;

/**
   ******************************************************************************
 * Exported function
 *
 */
/**
  * @brief  Clear to 0 a certain number a bytes
  * @param	pStartAdress	Adress of the begining of the zone to clear
  * 		length_byte		Length of the zone (expressed in bytes)
  */
inline void
mem_ClearZone(	void* const		_pStartAdress,
				unsigned long 	length_byte)
{
	unsigned long pStartAdress = (unsigned int) _pStartAdress;

	for(; pStartAdress < (unsigned long)_pStartAdress + length_byte; pStartAdress++) {
		__WriteValueToAdress_u8(pStartAdress) = 0;
	}
}

/**
  * @brief  Set to _value a certain number a bytes
  * @param	_pStartAdress	Adress of the begining of the zone to clear
  * 		_value			Value to set to these bytes
  * 		_length_byte	Length of the zone (expressed in bytes)
  */
inline void
mem_SetZone(	void* const 	_pStartAdress,
				unsigned long	value,
				unsigned long 	length_byte)
{
	unsigned int pStartAdress = (unsigned int) _pStartAdress;
	// TODO: Ca marche pas idiot ! T'ecrit un unsigned long avec une macro u8
	// Faut que tu sizeof et void* et decoupe en paquet d'octet
	for(; pStartAdress < (unsigned int)_pStartAdress + length_byte; pStartAdress++) {
		__WriteValueToAdress_u8(pStartAdress) = value;
	}
}

/**
  * @brief  Memory allocation of a certain number of bytes
  * @param	_size	Size to allocate
  * @retval	Pointer to the begining of the allocated area
  */
static void*
_mem_allocation(unsigned int _size)
{
	//TODO
	void* p = 0;
	return p;
}


/**
  * @brief  Initialize the memory allocation unit
  * @param	_size	Size to allocate
  * @retval	Pointer to the begining of the allocated area
  */
static void*
_mem_allocationinit(unsigned int _size)
{
	//TODO
	mem_allocation = _mem_allocation;
	return _mem_allocation(_size);
}
