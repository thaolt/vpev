#include <cstring>
#include <iostream>
#include <bitset>
#include <thread>
#include <future>

#include "phevaluator/phevaluator.h"

int payout[] = {
    0,
    800,
    40,
    20,
    9,
    6,
    5,
    3,
    2,
    1,
    0
};


double eval(int *cards, int n) {
    if (n == 5) {
        int rank = evaluate_5cards(cards[0],cards[1],cards[2],cards[3],cards[4]);
        int category = 10;
        if (rank == 1) {
            category = 1;
        } else if (rank <= 10) {
            category = 2;
        } else if (rank <= 166) {
            category = 3;
        } else if (rank <= 322) {
            category = 4;
        } else if (rank <= 1599) {
            category = 5;
        } else if (rank <= 1609) {
            category = 6;
        } else if (rank <= 2467) {
            category = 7;
        } else if (rank <= 3325) {
            category = 8;
        } else if (rank <= 4205) {
            category = 9;
        }
        return payout[ category ];
    } else {
        int deck_len = 52-n;
        int deck[deck_len];
        int di = 0;
        for (int i = 0; i < 52; ++i) {
            bool skip = false;
            for (int j = 0; j < n; ++j) {
                if (i == cards[j]) {skip = true; break;}
            }
            if (!skip) {
                deck[di] = i;
                di++;
            }
        }
        double r = 0;

        for (int c = 0; c < deck_len; ++c) {
            int pcards[5];
            std::memcpy(pcards, cards, sizeof(int) * 5);
            pcards[n] = deck[c];
            if (n == 0) {
                r += 0.339453 / (deck_len);
            } else {
                r += eval(pcards, n+1) / (deck_len);
            }
        }
        return r;
    }
}


int str2cnum(const char* str)
{
    int r = 0; int s = 0;
    char * hstr = (char*) calloc(std::strlen(str), sizeof(char));
    for (int x=0; x<std::strlen(str); x++)
        hstr[x] = (std::toupper(str[x]));

    switch (hstr[0]) {
    case '2': r = 0; break; case '3': r = 1; break; case '4': r = 2; break;
    case '5': r = 3; break; case '6': r = 4; break; case '7': r = 5; break;
    case '8': r = 6; break; case '9': r = 7; break; case 'T': r = 8; break;
    case 'J': r = 9; break; case 'Q': r = 10; break; case 'K': r = 11; break;
    case 'A': r = 12; break;
    }
    switch (hstr[1]) {
    case 'C': s = 0; break; case 'D': s = 1; break; case 'H': s = 2; break; case 'S': s = 3; break;
    }
    free(hstr);
    return r * 4 + s;
}

void print_hand(int *h, int n) {
    for (int i = 0; i < n; ++i) {
        char cstr[3] = {0};
        int cr = h[i] / 4;
        int cs = h[i] % 4;
        switch (cr) {
        case 0: cstr[0] = '2'; break; case 1: cstr[0] = '3'; break; case 2: cstr[0] = '4'; break;
        case 3: cstr[0] = '5'; break; case 4: cstr[0] = '6'; break; case 5: cstr[0] = '7'; break;
        case 6: cstr[0] = '8'; break; case 7: cstr[0] = '9'; break; case 8: cstr[0] = 'T'; break;
        case 9: cstr[0] = 'J'; break; case 10: cstr[0] = 'Q'; break; case 11: cstr[0] = 'K'; break;
        case 12: cstr[0] = 'A'; break;
        }
        switch (cs) {
        case 0: cstr[1] = 'c'; break; case 1: cstr[1] = 'd'; break; case 2: cstr[1] = 'h'; break; case 3: cstr[1] = 's'; break;
        }
        std::cout << cstr << " ";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 6) return 1;
    int starting_hand[5] = {
        str2cnum(argv[1]),str2cnum(argv[2]),str2cnum(argv[3]),
        str2cnum(argv[4]),str2cnum(argv[5])
    };
    double maxEV = 0;
    std::bitset<5> maxMask(0);

    for (int i = 0; i < 32; ++i) {
        int holds[5] = {0};
        int cc = 0;
        std::bitset<5> bits(i);
        for (int b = 0; b < 5; ++b) {
            if (bits.test(b)) {
                holds[cc] = starting_hand[b];
                cc++;
            }
        }
        double ev = eval(holds, cc);
//        std::cout << bits.to_string() << " ev: " << ev << std::endl;
        if (ev > maxEV) {
            maxEV = ev;
            maxMask = bits;
        }
    }

    std::string output = maxMask.to_string();
    reverse(output.begin(), output.end());
    std::cout << output << " ev: " << maxEV << std::endl;


    return 0;
}
