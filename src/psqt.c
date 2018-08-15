/*
  Ethereal is a UCI chess playing engine authored by Andrew Grant.
  <https://github.com/AndyGrant/Ethereal>     <andrew@grantnet.us>

  Ethereal is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Ethereal is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "assert.h"
#include "bitboards.h"
#include "evaluate.h"
#include "psqt.h"
#include "types.h"

int PSQT[32][SQUARE_NB];

#define S(mg, eg) MakeScore((mg), (eg))

const int PawnPSQT32[32] = {
        S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
        S( -20,   6), S(   9,   3), S(  -7,   6), S(  -5,   1),
        S( -22,   0), S( -12,   0), S(  -4, -12), S(  -2, -16),
        S( -18,   9), S( -13,   9), S(  14, -19), S(  12, -32),
        S(  -3,  18), S(   0,  18), S(  -2,  -2), S(   8, -26),
        S(   4,  32), S(  11,  29), S(  16,   3), S(  29, -23),
        S( -51,   0), S( -54,   3), S(  -6, -22), S(  -1, -38),
        S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
        S( -38, -48), S(  10, -35), S(  -1, -17), S(  16, -12),
        S(  12, -46), S(  16,  -8), S(  21, -23), S(  27,  -8),
        S(  18, -24), S(  36, -22), S(  26,  -1), S(  37,   7),
        S(  19,  -8), S(  26,   6), S(  38,  22), S(  40,  32),
        S(  30,   4), S(  34,  12), S(  47,  32), S(  41,  42),
        S( -32,  14), S(  17,   8), S(  27,  32), S(  39,  33),
        S( -28, -20), S( -33,   9), S(  34, -29), S(  10,  -5),
        S(-179, -35), S(-101, -25), S(-158,  -5), S( -45, -25),
};

const int BishopPSQT32[32] = {
        S(  30, -23), S(  21, -29), S(   3,  -6), S(  24, -11),
        S(  44, -31), S(  40, -32), S(  28, -18), S(  17,  -6),
        S(  30, -14), S(  40, -17), S(  27,   0), S(  22,   2),
        S(  20, -11), S(  17,  -4), S(  18,  11), S(  34,  12),
        S(  -8,   7), S(  33,   2), S(   5,  15), S(  25,  19),
        S(   4,   0), S(   0,   9), S(  19,   7), S(  27,   4),
        S( -74,   7), S( -29,  -1), S(  -7, -14), S( -42,   2),
        S( -48,   2), S( -60,  -4), S(-124,  10), S(-112,  13),
};

const int RookPSQT32[32] = {
        S(   6, -31), S(   3, -27), S(  18, -27), S(  26, -31),
        S( -33, -16), S(   1, -27), S(  12, -21), S(  16, -25),
        S( -12, -12), S(   8, -14), S(   2, -11), S(  15, -21),
        S( -12,   0), S(  -5,   7), S(   0,   2), S(   2,   4),
        S(  -3,  10), S(  -3,  12), S(  21,   1), S(  24,   6),
        S( -20,  12), S(  12,  10), S(   2,  13), S(  21,   9),
        S(   1,   6), S( -17,  16), S(  28,   0), S(  12,   5),
        S(   0,  19), S(   8,  14), S( -24,  23), S(  -6,  21),
};

const int QueenPSQT32[32] = {
        S(   6, -45), S(  -3, -32), S(   7, -27), S(  27, -42),
        S(  18, -43), S(  22, -38), S(  29, -54), S(  25, -10),
        S(  15, -24), S(  31, -16), S(  13,  18), S(  14,   8),
        S(  14,  -6), S(  15,   6), S(   0,  20), S(  -3,  45),
        S( -12,  13), S(  -4,  32), S(  -7,  22), S( -29,  59),
        S( -32,   3), S( -14,  15), S( -16,  15), S( -15,  44),
        S( -10,   8), S( -72,  56), S(  12,   7), S( -35,  58),
        S( -25,  -8), S(   5,  -3), S(  10,   1), S( -19,  10),
};

const int KingPSQT32[32] = {
        S(  85,-101), S(  82, -66), S(  36, -24), S(  19, -35),
        S(  73, -60), S(  52, -43), S(   5,  -8), S( -19,   3),
        S(   7, -42), S(  48, -36), S(  18,  -7), S( -15,  15),
        S( -46, -25), S(  32, -25), S(   1,  12), S( -47,  34),
        S( -17, -12), S(  54,  -5), S(   6,  28), S( -30,  36),
        S(  39, -17), S(  82,  -6), S(  73,  19), S(  10,  22),
        S(  16, -14), S(  51,  -4), S(  34,   1), S(   8,   0),
        S(  28, -81), S(  85, -68), S( -21, -33), S( -15, -34),
};
#undef S

int relativeSquare32(int s, int c) {
    assert(0 <= c && c < COLOUR_NB);
    assert(0 <= s && s < SQUARE_NB);
    static const int edgeDistance[FILE_NB] = {0, 1, 2, 3, 3, 2, 1, 0};
    return 4 * relativeRankOf(c, s) + edgeDistance[fileOf(s)];
}

void initializePSQT() {

    for (int s = 0; s < SQUARE_NB; s++) {
        const int w32 = relativeSquare32(s, WHITE);
        const int b32 = relativeSquare32(s, BLACK);

        PSQT[WHITE_PAWN  ][s] = +MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) +   PawnPSQT32[w32];
        PSQT[WHITE_KNIGHT][s] = +MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) + KnightPSQT32[w32];
        PSQT[WHITE_BISHOP][s] = +MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) + BishopPSQT32[w32];
        PSQT[WHITE_ROOK  ][s] = +MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) +   RookPSQT32[w32];
        PSQT[WHITE_QUEEN ][s] = +MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) +  QueenPSQT32[w32];
        PSQT[WHITE_KING  ][s] = +MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) +   KingPSQT32[w32];

        PSQT[BLACK_PAWN  ][s] = -MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) -   PawnPSQT32[b32];
        PSQT[BLACK_KNIGHT][s] = -MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) - KnightPSQT32[b32];
        PSQT[BLACK_BISHOP][s] = -MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) - BishopPSQT32[b32];
        PSQT[BLACK_ROOK  ][s] = -MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) -   RookPSQT32[b32];
        PSQT[BLACK_QUEEN ][s] = -MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) -  QueenPSQT32[b32];
        PSQT[BLACK_KING  ][s] = -MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) -   KingPSQT32[b32];
    }
}
