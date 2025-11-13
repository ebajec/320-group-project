#include "roulette.hpp"
#include <vector>
#include <stdlib.h>


static constexpr int WINS[] = {
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
    36, 36, 18, 7, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,9, 18, 18,  18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    18, 18, 18, 18,18, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2

};

static_assert(sizeof(WINS) / sizeof(WINS[0]) == ROULETTE_BET_COUNT, "WINS payout table mismatch");

int spin(const int bets[], int *rolledNumber) {
    int cashout(0);
    std::vector<int> paid;

    int roll (rand() % 38);
    if (rolledNumber)
        *rolledNumber = roll;
    

    switch (roll) {
        case 0:
            paid.insert(paid.end(), {
                STRAIGHT0, TOPLINE, ROW
            });
            break;
        case 1:
            paid.insert(paid.end(), {
                    STRAIGHT1, TOPLINE, STREET1, DOUBLESTREET1, SPLIT1,
                    SPLIT3, CORNER1, COLUMN1, ODD, RD, HALF1, DOZEN1
            });
            break;
        case 2:
            paid.insert(paid.end(), {
                    STRAIGHT2, TOPLINE, STREET1, DOUBLESTREET1, SPLIT1,
                    SPLIT2, SPLIT4, CORNER1, CORNER2, COLUMN2, EVEN, BLK, HALF1, DOZEN1
            });
            break;
        case 3:
            paid.insert(paid.end(), {
                    STRAIGHT3, TOPLINE, STREET1, DOUBLESTREET1, SPLIT2,
                    SPLIT5, CORNER2, COLUMN3, ODD, RD, HALF1, DOZEN1
            });
            break;
        case 4:
            paid.insert(paid.end(), {
                    STRAIGHT4, STREET2, DOUBLESTREET1, DOUBLESTREET2, SPLIT3,
                    SPLIT6, SPLIT8, CORNER1, CORNER3, COLUMN1, EVEN, BLK, HALF1, DOZEN1
            });
            break;
        case 5:
            paid.insert(paid.end(), {
                    STRAIGHT5, STREET2, DOUBLESTREET1, DOUBLESTREET2, SPLIT4,
                    SPLIT6, SPLIT7, SPLIT9, CORNER1, CORNER2, CORNER3, CORNER4, COLUMN2, ODD, RD, HALF1, DOZEN1
            });
            break;
        case 6:
            paid.insert(paid.end(), {
                    STRAIGHT6, STREET2, DOUBLESTREET1, DOUBLESTREET2, SPLIT5,
                    SPLIT7, SPLIT10, CORNER2, CORNER4, COLUMN3, EVEN, BLK, HALF1, DOZEN1
            });
            break;
        case 7:
            paid.insert(paid.end(), {
                    STRAIGHT7, STREET3, DOUBLESTREET2, DOUBLESTREET3, SPLIT8,
                    SPLIT11, SPLIT13, CORNER3, CORNER5, COLUMN1, ODD, RD, HALF1, DOZEN1
            });
            break;
        case 8:
            paid.insert(paid.end(), {
                    STRAIGHT8, STREET3, DOUBLESTREET3, DOUBLESTREET2, SPLIT9,
                    SPLIT11, SPLIT12, SPLIT14, CORNER5, CORNER6, CORNER3, CORNER4, COLUMN2, EVEN, BLK, HALF1, DOZEN1
            });
            break;
        case 9:
            paid.insert(paid.end(), {
                    STRAIGHT9, STREET3, DOUBLESTREET3, DOUBLESTREET2, SPLIT10,
                    SPLIT12, SPLIT15, CORNER6, CORNER4, COLUMN3, ODD, RD, HALF1, DOZEN1
            });
            break;
        case 10:
            paid.insert(paid.end(), {
                    STRAIGHT10, STREET4, DOUBLESTREET4, DOUBLESTREET3, SPLIT13,
                    SPLIT16, SPLIT18, CORNER7, CORNER5, COLUMN1, EVEN, BLK, HALF1, DOZEN1
            });
            break;
        case 11:
            paid.insert(paid.end(), {
                    STRAIGHT11, STREET4, DOUBLESTREET3, DOUBLESTREET4, SPLIT14,
                    SPLIT16, SPLIT17, SPLIT19, CORNER5, CORNER6, CORNER7, CORNER8, COLUMN2, ODD, BLK, HALF1, DOZEN1
            });
            break;
        case 12:
            paid.insert(paid.end(), {
                    STRAIGHT12, STREET4, DOUBLESTREET3, DOUBLESTREET4, SPLIT15,
                    SPLIT17, SPLIT20, CORNER6, CORNER8, COLUMN3, EVEN, RD, HALF1, DOZEN1
            });
            break;
        case 13:
            paid.insert(paid.end(), {
                    STRAIGHT13, STREET5, DOUBLESTREET4, DOUBLESTREET5, SPLIT18,
                    SPLIT21, SPLIT23, CORNER7, CORNER9, COLUMN1, ODD, BLK, HALF1, DOZEN2
            });
            break;
        case 14:
            paid.insert(paid.end(), {
                    STRAIGHT14, STREET5, DOUBLESTREET5, DOUBLESTREET4, SPLIT19,
                    SPLIT21, SPLIT22, SPLIT24, CORNER9, CORNER10, CORNER7, CORNER8, COLUMN2, EVEN, RD, HALF1, DOZEN2
            });
            break;
        case 15:
            paid.insert(paid.end(), {
                    STRAIGHT15, STREET5, DOUBLESTREET5, DOUBLESTREET4, SPLIT20,
                    SPLIT22, SPLIT25, CORNER10, CORNER8, COLUMN3, ODD, BLK, HALF1, DOZEN2
            });
            break;
        case 16:
            paid.insert(paid.end(), {
                    STRAIGHT16, STREET6, DOUBLESTREET6, DOUBLESTREET5, SPLIT23,
                    SPLIT26, SPLIT28, CORNER11, CORNER9, COLUMN1, EVEN, RD, HALF1, DOZEN2
            });
            break;
        case 17:
            paid.insert(paid.end(), {
                    STRAIGHT17, STREET6, DOUBLESTREET6, DOUBLESTREET5, SPLIT24,
                    SPLIT26, SPLIT27, SPLIT29, CORNER9, CORNER10, CORNER11, CORNER12, COLUMN2, ODD, BLK, HALF1, DOZEN2
            });
            break;
        case 18:
            paid.insert(paid.end(), {
                    STRAIGHT18, STREET6, DOUBLESTREET5, DOUBLESTREET6, SPLIT25,
                    SPLIT27, SPLIT30, CORNER10, CORNER12, COLUMN3, EVEN, RD, HALF1, DOZEN2
            });
            break;
        case 19:
            paid.insert(paid.end(), {
                    STRAIGHT19, STREET7, DOUBLESTREET6, DOUBLESTREET7, SPLIT28,
                    SPLIT31, SPLIT33, CORNER11, CORNER9, COLUMN1, EVEN, RD, HALF2, DOZEN2
            });
            break;
        case 20:
            paid.insert(paid.end(), {
                    STRAIGHT20, STREET7, DOUBLESTREET6, DOUBLESTREET7, SPLIT29,
                    SPLIT31, SPLIT32, SPLIT34, CORNER13, CORNER14, CORNER11, CORNER12, COLUMN2, EVEN, BLK, HALF2, DOZEN2
            });
            break;
        case 21:
            paid.insert(paid.end(), {
                    STRAIGHT21, STREET7, DOUBLESTREET7, DOUBLESTREET6, SPLIT30,
                    SPLIT32, SPLIT35, CORNER14, CORNER12, COLUMN3, ODD, RD, HALF2, DOZEN2
            });
            break;
        case 22:
            paid.insert(paid.end(), {
                    STRAIGHT22, STREET8, DOUBLESTREET8, DOUBLESTREET7, SPLIT33,
                    SPLIT36, SPLIT38, CORNER11, CORNER13, COLUMN1, EVEN, BLK, HALF2, DOZEN2
            });
            break;
        case 23:
            paid.insert(paid.end(), {
                    STRAIGHT23, STREET8, DOUBLESTREET8, DOUBLESTREET7, SPLIT34,
                    SPLIT36, SPLIT37, SPLIT39, CORNER13, CORNER14, CORNER15, CORNER16, COLUMN2, ODD, RD, HALF2, DOZEN2
            });
            break;
        case 24:
            paid.insert(paid.end(), {
                    STRAIGHT24, STREET8, DOUBLESTREET8, DOUBLESTREET7, SPLIT35,
                    SPLIT37, SPLIT40, CORNER14, CORNER16, COLUMN3, EVEN, BLK, HALF2, DOZEN2
            });
            break;
        case 25:
            paid.insert(paid.end(), {
                    STRAIGHT25, STREET9, DOUBLESTREET8, DOUBLESTREET9, SPLIT38,
                    SPLIT41, SPLIT43, CORNER15, CORNER13, COLUMN1, ODD, RD, HALF2, DOZEN3
            });
            break;
        case 26:
            paid.insert(paid.end(), {
                    STRAIGHT26, STREET9, DOUBLESTREET8, DOUBLESTREET9, SPLIT39,
                    SPLIT41, SPLIT42, SPLIT44, CORNER17, CORNER18, CORNER15, CORNER16, COLUMN2, EVEN, BLK, HALF2, DOZEN3
            });
            break;
        case 27:
            paid.insert(paid.end(), {
                    STRAIGHT27, STREET9, DOUBLESTREET8, DOUBLESTREET9, SPLIT40,
                    SPLIT42, SPLIT45, CORNER18, CORNER16, COLUMN3, ODD, RD, HALF2, DOZEN3
            });
            break;
        case 28:
            paid.insert(paid.end(), {
                    STRAIGHT28, STREET10, DOUBLESTREET10, DOUBLESTREET9, SPLIT43,
                    SPLIT46, SPLIT48, CORNER15, CORNER17, COLUMN1, EVEN, BLK, HALF2, DOZEN3
            });
            break;
        case 29:
            paid.insert(paid.end(), {
                    STRAIGHT29, STREET10, DOUBLESTREET10, DOUBLESTREET9, SPLIT44,
                    SPLIT46, SPLIT47, SPLIT49, CORNER17, CORNER18, CORNER19, CORNER20, COLUMN2, ODD, BLK, HALF2, DOZEN3
            });
            break;
        case 30:
            paid.insert(paid.end(), {
                    STRAIGHT30, STREET10, DOUBLESTREET10, DOUBLESTREET9, SPLIT45,
                    SPLIT47, SPLIT50, CORNER18, CORNER20, COLUMN3, EVEN, RD, HALF2, DOZEN3
            });
            break;
        case 31:
            paid.insert(paid.end(), {
                    STRAIGHT31, STREET11, DOUBLESTREET10, DOUBLESTREET11, SPLIT48,
                    SPLIT51, SPLIT53, CORNER19, CORNER17, COLUMN1, ODD, BLK, HALF2, DOZEN3
            });
            break;
        case 32:
            paid.insert(paid.end(), {
                    STRAIGHT32, STREET11, DOUBLESTREET10, DOUBLESTREET11, SPLIT49,
                    SPLIT51, SPLIT52, SPLIT54, CORNER21, CORNER22, CORNER19, CORNER20, COLUMN2, EVEN, RD, HALF2, DOZEN3
            });
            break;
        case 33:
            paid.insert(paid.end(), {
                    STRAIGHT33, STREET11, DOUBLESTREET10, DOUBLESTREET11, SPLIT50,
                    SPLIT52, SPLIT55, CORNER22, CORNER20, COLUMN3, ODD, BLK, HALF2, DOZEN3
            });
            break;
        case 34:
            paid.insert(paid.end(), {
                    STRAIGHT34, STREET12, DOUBLESTREET11, SPLIT53,
                    SPLIT56, CORNER19, COLUMN1, EVEN, RD, HALF2, DOZEN3
            });
            break;
        case 35:
            paid.insert(paid.end(), {
                    STRAIGHT35, STREET12, DOUBLESTREET11, SPLIT54,
                    SPLIT56, SPLIT57, CORNER21, CORNER22, COLUMN2, ODD, BLK, HALF2, DOZEN3
            });
            break;
        case 36:
            paid.insert(paid.end(), {
                    STRAIGHT36, STREET12, DOUBLESTREET11, SPLIT55,
                    SPLIT57, CORNER22, COLUMN3, EVEN, RD, HALF2, DOZEN3
            });
            break;
        case 37:
            paid.insert(paid.end(), {
                    STRAIGHT00, TOPLINE, ROW
            });
            break;
        default:
            break;

    }

    for (int i: paid) {
        cashout += bets[i] * WINS[i];
    }

    return cashout;
}
