/**
 * @file 		stack.h
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#ifndef STACK_H
#define STACK_H

/**
 * A verem típusa (típuselrejtés)
 */
typedef void *my_stack_t;

/**
 * @brief      Verem inicializálása
 *
 * @return     Az inicializált verem
 */
my_stack_t init_stack();

/**
 * @brief      Elem hozzáadása a veremhez
 *
 * @param[in]  s     A verem
 * @param      data  Az elem
 */
void push(my_stack_t s, void *data);

/**
 * @brief      Elem kivétele a veremből
 *
 * @param[in]  s     A verem
 *
 * @return     A kivett elem
 */
void * pop(my_stack_t s);

/**
 * @brief      A verem teteje (Top Of the Stack)
 *
 * A vermen nem végzünk módosítást.
 *
 * @param[in]  s     A verem
 *
 * @return     A verem teteje
 */
void * peek(my_stack_t s);

/**
 * @brief      Verem felszabadítása
 *
 * @param[in]  s     A verem
 */
void free_stack(my_stack_t s);

#endif /* STACK_H */