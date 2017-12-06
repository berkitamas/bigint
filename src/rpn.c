/**
 * @file 		rpn.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#include "utils.h"
#include "queue.h"
#include "stack.h"
#include "ops.h"
#include "rpn.h"

bool is_number(char c)
{
	return (c >= '0' && c <= '9');
}

bool is_operator(char c)
{
	op_t *ops = get_ops(); //ops.c
	int i;
	for(i = 0; ops[i].type != UNKNOWN; ++i)
		if (ops[i].ch == c) return true;
	return false;
}

bool is_right_bracket(char c)
{
	return (c == ')');
}

//https://en.wikipedia.org/wiki/Shunting-yard_algorithm
//http://wcipeg.com/wiki/Shunting_yard_algorithm
void read_number(FILE *stream, char **str)
{
	char *buf;
	MY_MALLOC(buf, sizeof(char));
	uint64_t counter = 0;
	char c;
	bool otz = false; //other than zero
	do {
		c = getc(stream);
		if(!isspace(c) && is_number(c)) {
			if(c != '0' || otz) { //Megszabadítjuk a számot az értéktelen 0-áktól
				MY_REALLOC(buf, (++counter) * sizeof(char));
				buf[counter - 1] = c;
				otz = true;
			}
		}
	} while ((isspace(c) && c != '\n' && c != EOF) || is_number(c));

	//Ha valami nagyon nagy gáz van
	if(counter == 0) {
		MY_REALLOC(buf, (++counter) * sizeof(char));
		buf[counter - 1] = '0';
	}
	MY_REALLOC(buf, (++counter) * sizeof(char));
	buf[counter - 1] = '\0';
	*str = buf;
	ungetc(c, stream);
}

my_queue_t parse_infix(FILE *stream)
{
	op_t * ops = get_ops(); //ops.c

	my_stack_t op = init_stack();
	my_queue_t out = init_queue();
	char c;

	p_elem prev_elem = NULL;
	p_elem elem = NULL;

	do {
		c = getc(stream);
		elem = NULL;
		/*
		A Shunting-yard algoritmus röviden:
		- Van egy várólistánk, abba akarjuk RPN szerint belerakni az értékeket
		- Az operandusokat gondolkodás nélkül belerakjuk
		- Operátoroknál viccesebb, azokat először egy várólistába rakjuk egy rendszer szerint:
			- Kivesszük az összes elemet a várólistából addig:
				- Amíg van mit kivenni
				- Amíg nem érünk el egy nyitó zárójelig
				- Amíg egy kisebb sorrendiségűt nem találunk
			- Nyitó zárójeleket gondolkodás nélkül belepakoljuk a verembe
			- Csukó zárójeleknél mindent kipakolunk, ami a zárójelen belül van a várólistában
				- A nyitó zárójelet nem (hisz ez a célunk)
			- Ha vége az inputnak, akkor a verem összes elemét belepakoljuk a várólistába
			- Készen vagyunk, átadjuk a kiértékelő függvénynek a várólistát
		- Nehézségek
			- Input validálás
			- Mikor unáris mínusz az unáris mínusz
				- Egy karakter szimbolizál két műveletet
		*/
		if (!isspace(c)) { //A szóközökkel már beolvasásnál nem foglalkozunk
			if (is_number(c)) {
				MY_MALLOC(elem, sizeof(elem_t));
				ungetc(c, stream);
				char *str;
				read_number(stream, &str);
				elem->type = OPERAND;
				elem->operand.sign = false;
				elem->operand.num = str;
				enqueue(out, elem);
				prev_elem = elem;
			} else if (is_operator(c)) {
				MY_MALLOC(elem, sizeof(elem_t));
				/*
				Ez egy cseles dolog, nem olyan evidens, hogy mikor unáris mínusz a mínusz:
				- Ha ezzel kezdődik az egész formula
				- Ha nyitó zárójel után van
				- Ha bináris operátor után van
				- Ha egy másik unáris mínusz vagy plusz után van
				*/
				if	( (c == '-' || c == '+') &&
					( prev_elem == NULL || (
							prev_elem->type == OPERATOR && (
				   				prev_elem->operator.type == LEFT_BRACKET ||
								prev_elem->operator.type == BINARY ||
								prev_elem->operator.ch == '-' ||
								prev_elem->operator.ch == '+')))) {
					//Ez az igaz ág
					int i;
					for (i = 0; ops[i].type != UNKNOWN; ++i) {
						if(ops[i].ch == c && ops[i].type == UNARY) {
							elem->type = OPERATOR;
							elem->operator.type = UNARY;
							elem->operator.prc = ops[i].prc;
							elem->operator.ch = ops[i].ch;
							elem->operator.op_unary = ops[i].op_unary;
							push(op, elem);
							prev_elem = elem;
							break;
						}
					}
				} else {
					//Minden bináris operátort infixként értelmezünk
					if (prev_elem == NULL && c != '(') error("NOTOPERAND", "You must write a number");
					int i;
					for (i = 0; ops[i].prc; ++i) {
						if(ops[i].ch == c ) {
							//Az unáris mínuszt fentebb kifejtettem
							if (!((c == '+' || c == '-') && ops[i].type == UNARY)) {
								//Ha egy szám áll a zárójel előtt, akkor azt úgy veszem, mintha a zárójeles
								//érték együtthatója lenne
								if(ops[i].type == LEFT_BRACKET && prev_elem && prev_elem->type == OPERAND) {
									elem->type = OPERATOR;
									elem->operator.type = BINARY;
									elem->operator.prc = 3;
									elem->operator.ch = '*';
									elem->operator.op_binary = multiply;
									push(op, elem);
									MY_MALLOC(elem, sizeof(elem_t));
								}
								p_elem tos = (p_elem) peek(op);
								//Így találta ki Dijkstra, ha a veremben lévő operátor sorrendisége nem kisebb az aktuális operátornál, akkor kiveszi a veremből
								//A nyitó zárójel különleges
								while (tos && tos->type == OPERATOR && ops[i].type != LEFT_BRACKET && tos->operator.prc >= ops[i].prc) {
									p_elem op_elem = (p_elem) pop(op);
									enqueue(out, op_elem);
									tos = (p_elem) peek(op);
								}
								elem->type = OPERATOR;
								elem->operator.type = ops[i].type;
								elem->operator.prc = ops[i].prc;
								elem->operator.ch = ops[i].ch;
								if (ops[i].type == UNARY)
									elem->operator.op_unary = ops[i].op_unary;
								else
									elem->operator.op_binary = ops[i].op_binary;
								push(op, elem);
								prev_elem = elem;
								break;
							}
						}
					}
				}
			} else if (is_right_bracket(c)) {
				//Egyszerű: mindent kiszed a nyitó zárójeléig
				p_elem tos = (p_elem) peek(op);
				bool valid = false;
				while (tos && tos->operator.type != LEFT_BRACKET) {
					p_elem elem = (p_elem) pop(op);
					//Ha nincs nyitó zárójelünk, azaz az input elejére értünk
					//akkor a felhasználó elfelejtette kinyitni
					if (elem == NULL) error("BRACKET", "Misssing parenthesis");
					enqueue(out, elem);
					tos = (p_elem) peek(op);
				}

				valid = (tos && tos->operator.type == LEFT_BRACKET);
				if (valid) {
					free(pop(op));
					free(elem);
				} else error("BRACKET", "Missing parenthesis");
			} else {
				error("UNKNOWN", "Unknown operator");
			}
		}
	} while (c != '\n' && c != EOF);

	if(!head(out)) error("NOOPERAND", "You must write a number");

	//A maradék operátort is belepakolja a várólistába
	while((elem = (p_elem) pop(op))) {
		enqueue(out, elem);
	}

	fclose(stream);
	free_stack(op);

	return out;
}


operand_t rpn_result(my_queue_t exp)
{
	my_stack_t operands = init_stack();
	p_elem elem;

#ifdef DEBUG
	//Debug módban kiírjuk a múveletek elvégzését elemenként
	fprintf(STREAM_OUT, "Parse:\n");
#endif

	//Addig megy, míg van elem
	while((elem = (p_elem) dequeue(exp))) {
		/*
			Az RPN kiértékelése röviden:
			- Van egy várólistánk, amiben vegyesen vannak operátorok és operandusok
			- Az operandusokat kérdés nélkül belepakoljuk egy verembe
			- Operátor esetén kiveszünk annyi elemet az operandusok verméből, amennyi kell
			- Az eredményt visszapakoljuk a verembe
			- A legvégén egy elem fog maradni abban az esetben, ha jó az input
		*/
		if (elem->type == OPERATOR) {
			p_elem left = NULL, right = NULL;
			operand_t l = number(false, "0"); //Warning elkerülése
			//Nagyon egyszerű a kiértékelés
			if(elem->operator.type == UNARY) {
				left = (p_elem) pop(operands);
				if(!left) error("PARSE", "Parse error");
				l = elem->operator.op_unary(left->operand);
			} else if(elem->operator.type == BINARY) {
				//Az első elem mindig a bináris operátor jobb oldali operandusa
				//Nem kommutatív műveletetknél (pl. kivonás) van jelentősége
				right = (p_elem) pop(operands);
				left = (p_elem) pop(operands);
				if(!left || !right) error("PARSE", "Parse error");
				l = elem->operator.op_binary(left->operand, right->operand);
			} else if(elem->operator.type == LEFT_BRACKET) {
				//Ha valami isteni csoda folytán marad nyitó zárójel
				//akkor baj van, a felhasználó elfelejtette bezárni
				error("BRACKET", "Missing parenthesis");
			}
			p_elem num = NULL;
			MY_MALLOC(num, sizeof(elem_t));
			num->type = OPERAND;
			num->operand = l;
#ifdef DEBUG
			//Debug módban kiírunk minden egyes műveletet
			if (elem->operator.type == UNARY && left) {
				fprintf(STREAM_OUT, "(%c)%s%s = %s%s\n", elem->operator.ch, (left->operand.sign)?"-":"", left->operand.num, (l.sign)?"-":"", l.num);
			} else if (elem->operator.type == BINARY && left && right){
				fprintf(STREAM_OUT, "%s%s %c %s%s = %s%s\n", (left->operand.sign)?"-":"", left->operand.num, elem->operator.ch, (right->operand.sign)?"-":"", right->operand.num, (l.sign)?"-":"", l.num);
			}
#endif
			//Mindent felszabaítunk, amit kell
			if (left && left->type == OPERAND)
				free(left->operand.num);
			free(left);
			if (right && right->type == OPERAND)
				free(right->operand.num);
			free(right);
			free(elem);

			push(operands, num);
		} else if (elem->type == OPERAND) {
			push(operands, elem);
		}
	}
	p_elem result = pop(operands);

	//Ha marad a várólistában elem, akkor hibás volt az input
	if (pop(operands)) error("PARSE", "Parse error");

	operand_t ret = result->operand;
	free_stack(operands);
	free(result);
	free_queue(exp);
	return ret;
}