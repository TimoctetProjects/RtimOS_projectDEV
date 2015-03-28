/**
  ******************************************************************************
  * @file      RtimOS.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      07/03/2015
  * @brief     Gestion de l'OS
  ******************************************************************************
*/

#ifndef RTIMOS_H
#define RTIMOS_H

// TODO: 	Fichier de portage cortex M4 avec des macros et fonctions ASM notement pour le critical
//			Le yielding, ...


// TODO: 	Fichier de type avec les typedef et autres pour l'exercice


// TODO: 	A la fin de cette partie du dev (Timer et suspension de tache OK)
//			Proprifier le code


// TODO:	Faire un systeme d'assert param sur liaison serie


// TODO:	Développer la config

// TODO:	Pour les taches, faire une seconde liste de tache en waiting

// TODO:	Forcer le premier chargement de contexte
//			En jetant ce que le main avait consommé

// TODO:	Fonction de retour des taches (si for(;;) interrompu)
//			Qui possed une assert et un for(;;)

// TODO:	Mettre un for(;;) apres le main

/**
   ******************************************************************************
 * Include
 *
 */

/**
   ******************************************************************************
 * Exported Types
 *
 */
typedef enum {
	FAIL = 0,
	PASS = !FAIL
}ResultTest_e;

typedef enum {
	STATUS_FINIS = 0,
	STATUS_ENCOURS = !STATUS_FINIS
}Status_e;

/**
   ******************************************************************************
 * Exported Maccro
 *
 */
/**
* @brief This section define NULL
*/
#ifdef NULL
	#undef NULL
#endif /** NULL */
#ifndef NULL
	#define NULL	((void *)0)
#endif /** NULL */

/**
* @brief Read the offset in byte of a struct's field
*/
#define __GetOffset_byte(type, field)	\
	((unsigned long)&(((type*)0)->field))

/**
   ******************************************************************************
 * This section handles validations
 *
 */
/** @brief Validating ALL */
#define VALDIATION					0


/** @brief Linked list */
#define VALIDATION_LIST				0 | VALDIATION
#define VALIDATION_LIST_LINEAIRE	0 | VALIDATION_LIST
#define VALIDATION_LIST_CIRULAIR	0 | VALIDATION_LIST



#define IS_VALIDATION	\
	VALIDATION_LIST_CIRULAIR | VALIDATION_LIST_LINEAIRE



#endif /** RTIMOS_H */

/***********************************END OF FILE*******************************/
