 /**
 * @file 		main.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdint.h>

#include "utils.h"
#include "stack.h"
#include "queue.h"
#include "ops.h"
#include "rpn.h"

int main()
{
	//Beolvassuk, majd feldolgozzuk a STREAM_IN-ből bejövő inputot.
	my_queue_t rpn = parse_infix(STREAM_IN);
	operand_t result =  rpn_result(rpn);

#ifdef DEBUG
	//Debug módban megkülönböztetjük az eredményt a műveletektől (jobban olvasható)
	fprintf(STREAM_OUT, "Result:\n");
#endif /* DEBUG */

#ifndef NOFORMAT
	//Formázzuk az eredményt (ezres tagolással)
	uint64_t i, three = (strlen_long(result.num)+2) % 3 , counter = 0;
	char *result_formatted;
	MY_MALLOC(result_formatted, sizeof(char));

	for (i = 0; i < strlen_long(result.num); ++i) {
		MY_REALLOC(result_formatted, (++counter) * sizeof(char));
		result_formatted[counter - 1] = result.num[i];
		if(i % 3 == three) {
			MY_REALLOC(result_formatted, (++counter) * sizeof(char));
			result_formatted[counter - 1] = ' ';
		}
	}
	MY_REALLOC(result_formatted, (++counter) * sizeof(char));
	result_formatted[counter - 1] = '\0';

	fprintf(STREAM_OUT, "%s%s\n", result.sign?"-":"", result_formatted);
	free(result_formatted);
#else /* NOFORMAT */
	//"Csupasz" mód, nem változtatunk a szövegen semmit
	fprintf(STREAM_OUT, "%s%s\n", result.sign?"-":"", result.num);
#endif /* NOFORMAT */
#ifdef DEBUG
	printf("Number length: %llu\n", strlen_long(result.num));
#endif /* DEBUG */
	free(result.num);
	fclose(STREAM_OUT);
	return EXIT_SUCCESS;
}