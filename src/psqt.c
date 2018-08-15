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
        S( -11,   5), S(  28,   2), S(   8,   1), S(   7,  -3),
        S( -14,   1), S(   8,   1), S(   7,  -9), S(   8, -12),
        S( -10,  12), S(   3,  12), S(  19, -13), S(  18, -24),
        S(   2,  17), S(  12,  17), S(  12,  -3), S(  17, -27),
        S(   5,  17), S(  20,  13), S(  19,  -9), S(  27, -42),
        S( -68,  -9), S( -93,  -3), S( -15, -37), S(  -9, -53),
        S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
        S( -19, -41), S(  40, -35), S(  27, -18), S(  45, -15),
        S(  37, -39), S(  44,  -5), S(  50, -25), S(  58, -12),
        S(  47, -26), S(  64, -24), S(  54,  -7), S(  64,   6),
        S(  45, -20), S(  50,  -2), S(  65,  11), S(  69,  20),
        S(  50,  -7), S(  59,   2), S(  69,  19), S(  67,  29),
        S( -29,  14), S(  38,   1), S(  32,  27), S(  53,  23),
        S(   2, -14), S( -17,  22), S(  56, -18), S(  25,   1),
        S(-168, -21), S( -95,  -7), S(-148,   9), S( -48, -11),
};

const int BishopPSQT32[32] = {
        S(  63, -32), S(  52, -37), S(  37, -16), S(  57, -25),
        S(  75, -41), S(  73, -44), S(  58, -30), S(  49, -18),
        S(  64, -27), S(  70, -29), S(  56, -11), S(  51,  -8),
        S(  49, -33), S(  45, -19), S(  45,  -3), S(  60,  -4),
        S(  12, -12), S(  57, -11), S(  25,   0), S(  47,   3),
        S(  23, -21), S(   9,  -2), S(  30,  -4), S(  41, -11),
        S( -49,   8), S(   0,  -1), S(  17, -20), S( -25,   1),
        S( -18,   4), S( -42,  -8), S(-106,  14), S(-100,  14),
};

const int RookPSQT32[32] = {
        S(  53, -33), S(  49, -30), S(  65, -31), S(  73, -35),
        S(  13, -17), S(  47, -31), S(  57, -26), S(  62, -30),
        S(  32, -14), S(  51, -17), S(  45, -13), S(  59, -25),
        S(  32,  -2), S(  33,   7), S(  40,   0), S(  42,   3),
        S(  37,   9), S(  33,  13), S(  59,   1), S(  62,   6),
        S(  11,  18), S(  36,  19), S(  26,  22), S(  58,  11),
        S(  34,  10), S(   6,  23), S(  43,  11), S(  23,  17),
        S(  22,  25), S(  13,  27), S( -13,  34), S(  -5,  35),
};

const int QueenPSQT32[32] = {
        S(  17, -43), S(   6, -37), S(  17, -37), S(  37, -40),
        S(  31, -33), S(  34, -43), S(  38, -56), S(  32,  -7),
        S(  27, -30), S(  40, -16), S(  19,  28), S(  21,   9),
        S(  25,  -8), S(  24,   6), S(   4,  24), S(   2,  41),
        S(  -8,  22), S(   1,  28), S(  -2,  21), S( -28,  68),
        S( -30,  19), S( -13,  18), S( -13,  14), S( -11,  44),
        S(   0,   9), S( -61,  56), S(   5,  12), S( -49,  58),
        S( -32,  31), S(  10,  23), S(  15,  20), S( -16,  17),
};

const int KingPSQT32[32] = {
        S(  75,-102), S(  72, -77), S(  25, -26), S(   9, -38),
        S(  65, -61), S(  43, -47), S(   1,  -9), S( -23,  -1),
        S(  27, -48), S(  52, -41), S(  30, -11), S( -17,  11),
        S( -27, -24), S(  34, -23), S(  10,  11), S( -49,  32),
        S( -11,  -4), S(  54,  -1), S(   4,  31), S( -26,  33),
        S(  39, -10), S(  79,  -3), S(  73,  19), S(  13,  21),
        S(  15,  -1), S(  48,   0), S(  34,   3), S(   8,  -2),
        S(  28, -81), S(  85, -69), S( -20, -29), S( -15, -31),
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
