//
// Created by rafa on 1/24/21.
//

#ifndef PAIRCOUNTER_COUNTER_H
#define PAIRCOUNTER_COUNTER_H \

#include "define.h"

long long *pair_count(Catalog d1, Catalog d2, Grid *nb1, Grid *nb2, double l_box, int nside, double minsep, double maxsep,
                      int nbins);

#endif //PAIRCOUNTER_COUNTER_H
