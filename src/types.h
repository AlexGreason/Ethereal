#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

//#include "search.h"
#define MaxDepth	(128)
#define MaxMoves	(256)

/* Structure Definitions */

typedef uint32_t move_t;
//	bits	00-07: From Square
//	bits	08-15: To Square 
//	bits	16-23: Capture Type
//	bits	24-27: Move Type
//	bits	28-31: Castle Changes

typedef struct transposition_t {
	int node_count;
	
} transposition_t;

typedef struct board_t {
	int positions[256];
	int squares[256];
	
	int piece_locations[2][32];
	int piece_counts[2];
	
	int pawn_locations[2][16];
	int pawn_counts[2];
	
	int castle_rights;
	
	int depth;
	move_t ep_history[MaxDepth];
	
	int turn;
	
} board_t;

typedef struct principle_variation_t {
	int length;
	move_t line[MaxDepth];
	
} principle_variation_t;


typedef struct search_tree_t {
	int ply;
	int raw_nodes;
	int alpha_beta_nodes;
	int quiescence_nodes;
	
	board_t board;
	
	move_t killer_moves[MaxDepth][3];
	principle_variation_t principle_variation;
	
} search_tree_t;

#endif