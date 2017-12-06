/**
 * @file 		rpn.h
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#ifndef RPN_H
#define RPN_H

#include <stdbool.h>
#include "queue.h"

/**
 * A tokenek típusai
 */
typedef enum token_type {
	OPERAND, ///< Operandus, azaz szám
	OPERATOR ///< Operátor, azaz művelet
} token_type;

/**
 * A műveletek típusai
 */
typedef enum operator_type {
	UNKNOWN, ///< Ismeretlen, ezzel zárjuk a tömböt
	UNARY, ///< Unáris (egy operandusú) művelet (pl. negálás)
	BINARY, ///< Bináris (két operandusú) művelet (pl. összeadás)
	LEFT_BRACKET ///< Nyitó zárójel (algoritmus miatt szükséges megkülönböztetni)
} operator_type;

/**
 * Az operandust tároló adatstruktúra
 */
typedef struct operand_t {
	bool sign; ///< Előjel (false - pozitív, true - negatív)
	char *num; ///< A szám stringként eltárolva
} operand_t, *p_operand;

/**
 * Az operátort tároló adatstruktúra
 */
typedef struct operator_t {
	operator_type type; ///< Operátor típusa
	int prc; ///< Operátor sorrendisége (melyiket végezzük el először)
	char ch; ///< Az operátort szimbolizáló karakter
	/**
	 * Unáris és bináris operátorok esetében mások lesznek a bemeneti értékek mennyisége.
	 */
	union {
		operand_t(*op_unary)(operand_t); ///< Unáris operátor művelete
		operand_t(*op_binary)(operand_t,operand_t); ///< Bináris operátor művelete
	};

} operator_t;

/**
 * Az operátort és az operandust "összefogó" adatstruktúra
 *
 * Azért kell, mert az algoritmus egy részében egy helyen tároljuk az operátort és az operandust
 * egy előre nem ismert sorrendben. Viszont ahhoz, hogy ne legyen problémás az adat kinyerése, egy
 * egységes adatstruktúrát hoztam létre, amivel egyszerűen meg lehet vizsgálni, hogy a jelenlegi
 * elem operátor vagy operandus-e.
 */
typedef struct elem_t {
	token_type type; ///< A token típusa
	/*
		Operátor vagy operandus adatstruktúrája
	*/
	union {
		operator_t operator; ///< Operátor adatstruktúrája
		operand_t operand; ///< Operandus adatstruktúrája
	};
} elem_t, *p_elem;

/**
 * @brief      A megadott karakter számjegy-e?
 *
 * @param[in]  c      A karakter
 *
 * @return     Számjegy-e?
 */
bool is_number(char c);

/**
 * @brief      A megadott karakter operátor (múveleti jel)-e?
 *
 * @param[in]  c     A karakter
 *
 * @return     Operátor-e?
 */
bool is_operator(char c);

/**
 * @brief      A megadott karakter csukó zárójel-e?
 *
 * @param[in]  c     A karakter
 *
 * @return     Csukó zárójel-e?
 */
bool is_right_bracket(char c);

/**
 * @brief      A streamből kiolvasott karakterek feldolgozása.
 *
 * Feltételezhetjük, hogy a streamben a matematikai formulát infix jelöléssel
 * (az operátor az operandusok között van) adjuk meg, ezt a függvény átalakítja postfix
 * (fordított lengyel jelölés - RPN)-re, és ezt egy olyan adatstruktúrában tárolja el,
 * amivel a jövőben egyszerűen tudunk dolgozni.
 *
 * @param      stream  A stream
 *
 * @return     A tároló adatstruktúra (várólista)
 */
my_queue_t parse_infix(FILE *stream);

/**
 * @brief      Egy bizonyos adatstruktúrában meghatározott matematikai formula kiértékelése
 *
 * Ez a függvény parse_infix() által létrehozott adatstruktúrával dolgozik.
 *
 * @param[in]  exp   A matematikai kifejezés
 *
 * @return     Az eredmény
 */
operand_t rpn_result(my_queue_t exp);

#endif /* RPN_H */