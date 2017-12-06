/**
 * @file 		queue.h
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#ifndef QUEUE_H
#define QUEUE_H

/**
 * Várólista típusa (típuselrejtés)
 */
typedef void *my_queue_t;

/**
 * @brief      Várólista inicializálása
 *
 * @return     Az inicializált várólista
 */
my_queue_t init_queue();

/**
 * @brief      Elem hozzáadása a várólistához
 *
 * @param[in]  s     A várólista
 * @param      data  Az elem (pointere)
 */
void enqueue(my_queue_t q, void *data);

/**
 * @brief      A várólista első elemének kivétele
 *
 * @param[in]  s     A várólista
 *
 * @return     Az első elem (pointere)
 */
void * dequeue(my_queue_t q);

/**
 * @brief      A várólista első eleme
 *
 * A várólistán nem végzünk módosítást.
 *
 * @param[in]  s     A várólista
 *
 * @return     Az első elem (pointere)
 */
void * head(my_queue_t q);

/**
 * @brief      A várólista felszabaítása
 *
 * @param[in]  q     A várólista
 */
void free_queue(my_queue_t q);

#endif /* QUEUE_H */