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
        S(   7,   1), S(  54,  -5), S(  34, -13), S(  30,  -7),
        S(   5,  -5), S(  37,  -6), S(  35, -22), S(  34, -25),
        S(   6,   7), S(  26,   6), S(  46, -23), S(  46, -37),
        S(  20,   6), S(  36,   5), S(  33, -14), S(  41, -41),
        S(  28,   4), S(  35,  -1), S(  10, -24), S(  24, -51),
        S( -70,  -9), S( -39,  -6), S( -15, -37), S(  -3, -58),
        S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
        S( -13, -38), S(   3, -12), S(  -8,  -1), S(   9,   8),
        S(   0, -31), S(   7,  19), S(  11,  -1), S(  22,  12),
        S(   9,  -5), S(  31,  -6), S(  22,   0), S(  36,  17),
        S(   9, -13), S(   9,  14), S(  32,  26), S(  37,  41),
        S(  21,   1), S(  27,  16), S(  48,  28), S(  34,  38),
        S( -45,  25), S(  21,   7), S(  35,  28), S(  24,  41),
        S(  -7,   3), S( -42,  33), S(  31, -13), S(  15,  -2),
        S(-181, -25), S( -94, -21), S(-155,   3), S( -36, -12),
};

const int BishopPSQT32[32] = {
        S(  37, -24), S(  38, -19), S(   0,  -6), S(  19,  -6),
        S(  39, -30), S(  41, -36), S(  23, -16), S(  16,  -7),
        S(  28, -10), S(  40, -11), S(  23,   0), S(  21,   0),
        S(  24, -31), S(  15, -13), S(   9,  14), S(  31,  11),
        S( -25,   8), S(  26,   7), S(  -4,   5), S(  11,  13),
        S(   4,  -9), S(  -9,   7), S(  10,   2), S(  17,   0),
        S( -58,  14), S( -21,  11), S(  -3,  -3), S( -30,  15),
        S( -32,   5), S( -39,   5), S(-102,  15), S(-108,  14),
};

const int RookPSQT32[32] = {
        S(  32, -31), S(  24, -33), S(  44, -40), S(  54, -44),
        S( -15, -19), S(  26, -35), S(  37, -33), S(  37, -41),
        S(   6,  -7), S(  26, -12), S(  18, -14), S(  40, -24),
        S(   9,  -2), S(   9,  15), S(  10,  13), S(  16,   4),
        S(  14,   8), S(   0,  20), S(  31,   3), S(  47,   3),
        S( -15,  14), S(   9,   9), S(   6,  12), S(  30,  12),
        S(  23,  -3), S( -17,  23), S(  20,  13), S(   6,  10),
        S(  16,  11), S(  14,  25), S( -20,  17), S(  -2,  17),
};

const int QueenPSQT32[32] = {
        S(  15, -35), S(  -3, -17), S(   5, -30), S(  27, -37),
        S(  24, -37), S(  20, -34), S(  35, -65), S(  23,  -7),
        S(  22, -24), S(  34, -10), S(  15,  15), S(   9,   7),
        S(  13,  11), S(  20,  13), S(  -2,  22), S(   3,  35),
        S(  -9,  13), S( -13,  37), S(   0,  19), S( -45,  62),
        S( -36,   5), S( -11,   8), S( -21,  15), S( -31,  42),
        S(   7,   7), S( -75,  40), S(  -3,   2), S( -38,  51),
        S( -13, -13), S(   3, -10), S(  10,   3), S( -23,  15),
};

const int KingPSQT32[32] = {
        S(  82,-111), S(  77, -84), S(  17, -13), S(  14, -39),
        S(  63, -62), S(  41, -45), S( -15,  -4), S( -35,   7),
        S(  29, -44), S(  48, -35), S(  20,  -4), S( -16,  13),
        S( -35, -14), S(  33, -21), S(  15,  17), S( -42,  40),
        S( -12,  -4), S(  59,   8), S(  10,  31), S( -22,  37),
        S(  41, -17), S(  84, -18), S(  75,  16), S(  19,  28),
        S(  14, -13), S(  49,  -7), S(  30, -11), S(   9,  -1),
        S(  29, -81), S(  85, -67), S( -19, -35), S( -18, -44),
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
