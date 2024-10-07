#ifndef SEGM1_H_
#define SEGM1_H_

#define SEG_PORT	PORTB
#define SEG_DDR		DDRB

#define V_PORT		PORTA
#define V_DDR		DDRA

#define LN1			1
#define LN2			2
#define LN3			3
#define LN4			4
#define LN5			5
#define LN6			6

const int LNL[6] = {LN6, LN5, LN4, LN3, LN2, LN1};

#define A			0
#define B			1
#define C			2
#define D			3
#define E			4
#define F			5
#define G			6
#define DP			7

#define S9 ((1 << A) | (1 << B) | (1 << C) | (1 << D) | (1 << F) | (1 << G))
#define S8 ((1 << A) | (1 << B) | (1 << C) | (1 << D) | (1 << E) | (1 << F) | (1 << G))
#define S7 ((1 << C) | (1 << B) | (1 << A))
#define S6 ((1 << D) | (1 << C) | (1 << A) | (1 << F) | (1 << G) | (1 << E))
#define S5 ((1 << D) | (1 << C) | (1 << A) | (1 << F) | (1 << G))
#define S4 ((1 << F) | (1 << G) | (1 << B) | (1 << C))
#define S3 ((1 << D) | (1 << C) | (1 << B) | (1 << A) | (1 << G))
#define S2 ((1 << D) | (1 << E) | (1 << B) | (1 << A) | (1 << G))
#define S1 ((1 << C) | (1 << B))
#define S0 ((1 << D) | (1 << C) | (1 << B) | (1 << A) | (1 << F) | (1 << E))



#endif