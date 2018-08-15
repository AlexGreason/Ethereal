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
        S(   1,   9), S(  47,   2), S(  27,  -6), S(  24,   3),
        S(   0,   1), S(  30,   1), S(  28, -14), S(  27, -16),
        S(   0,  14), S(  19,  15), S(  39, -14), S(  39, -28),
        S(  14,  14), S(  29,  14), S(  27,  -6), S(  35, -32),
        S(  15,  20), S(  16,  18), S(   5,  -9), S(  14, -35),
        S( -51,   3), S( -35,   7), S(  -6, -21), S(   0, -38),
        S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
        S( -33, -46), S(   3, -26), S(  -8, -11), S(  10,  -1),
        S(   1, -46), S(  11,  -2), S(  11, -14), S(  21,   7),
        S(   8, -15), S(  31, -12), S(  21,  -3), S(  34,  15),
        S(   7,  -6), S(  13,   7), S(  30,  25), S(  36,  38),
        S(  21,   1), S(  26,  12), S(  45,  30), S(  33,  36),
        S( -33,  12), S(  23,   4), S(  36,  30), S(  36,  33),
        S( -24, -14), S( -35,  13), S(  40, -26), S(  13,  -3),
        S(-171, -33), S(-100, -28), S(-157,  -5), S( -40, -23),
};

const int BishopPSQT32[32] = {
        S(  31, -21), S(  30, -22), S(  -2,  -7), S(  18,  -8),
        S(  36, -29), S(  38, -35), S(  21, -16), S(  12,  -6),
        S(  25, -10), S(  36, -11), S(  20,   0), S(  17,   0),
        S(  18, -16), S(  11,  -9), S(   6,  14), S(  27,  12),
        S( -24,   7), S(  23,   6), S(  -4,   6), S(  11,  11),
        S(   0,  -1), S(  -5,   4), S(  18,   0), S(  17,   1),
        S( -62,   8), S(  -8,   1), S(  -5,  -8), S( -37,   6),
        S( -44,   2), S( -56,   0), S(-121,   9), S(-111,  12),
};

const int RookPSQT32[32] = {
        S(  14, -23), S(   6, -24), S(  26, -32), S(  35, -36),
        S( -32, -17), S(   4, -27), S(  16, -22), S(  16, -32),
        S( -10,  -8), S(   7,  -9), S(   0, -12), S(  19, -16),
        S(  -9,   1), S(  -5,  12), S(  -3,  10), S(   0,   6),
        S(  -4,  13), S( -11,  17), S(  16,   5), S(  28,   8),
        S( -20,  13), S(   9,   6), S(   5,   9), S(  19,  13),
        S(   1,   3), S( -15,  18), S(  29,   6), S(  12,   5),
        S(   2,  17), S(  12,  19), S( -24,  19), S(   0,  19),
};

const int QueenPSQT32[32] = {
        S(   6, -43), S(  -8, -26), S(   0, -23), S(  23, -39),
        S(  15, -45), S(  15, -36), S(  28, -57), S(  19, -10),
        S(  14, -22), S(  29, -14), S(  10,  10), S(   5,   5),
        S(   9,   0), S(  16,   7), S(  -6,  17), S(  -2,  42),
        S( -13,  11), S( -16,  34), S(  -4,  22), S( -41,  52),
        S( -28,   1), S( -10,  15), S( -11,  17), S( -20,  42),
        S(   0,  11), S( -75,  50), S(  13,   6), S( -27,  61),
        S( -19, -20), S(   2, -13), S(   8,  -3), S( -20,  10),
};

const int KingPSQT32[32] = {
        S(  90,-110), S(  86, -87), S(  27, -17), S(  23, -41),
        S(  72, -63), S(  50, -48), S(  -5,  -7), S( -25,   5),
        S(  10, -38), S(  46, -34), S(  17,  -3), S( -15,  14),
        S( -46, -19), S(  32, -21), S(   5,  19), S( -45,  41),
        S( -16, -12), S(  57,   5), S(   8,  31), S( -29,  38),
        S(  40, -17), S(  83,  -8), S(  74,  18), S(  11,  23),
        S(  16, -16), S(  51,  -5), S(  33,  -3), S(   9,   0),
        S(  29, -81), S(  85, -67), S( -21, -35), S( -16, -37),
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
