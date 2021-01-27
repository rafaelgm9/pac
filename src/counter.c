//
// Created by rafa on 1/24/21.
//

#include "counter.h"
#include "define.h"
#include <stdlib.h>
#include <math.h>
#include <omp.h>

long long *
pair_count(Catalog d1, Catalog d2, Grid *nb1, Grid *nb2, double l_box, int nside, double minsep, double maxsep,
           int nbins) {
    double rbins[nbins + 1], rbins2[nbins + 1];
    double agrid = l_box / nside;
    double r2_max = maxsep * maxsep;
    double r2_min = minsep * minsep;
    int index_max = (int) (maxsep / agrid) + 1;
    long long *DD;

    DD = (long long *) malloc(nbins * sizeof(long long));

    for (int i = 0; i <= nbins; i++) {
        rbins[i] = (double) (pow(10, i * (log10(maxsep) - log10(minsep)) / 10. + log10(minsep)));
//        rbins[i] = (double) (minsep + i * (maxsep - minsep) / 10.);
        rbins2[i] = rbins[i] * rbins[i];
    }

    for (int i = 0; i < nbins; i++) {
        DD[i] = 0;
    }

    omp_set_num_threads(6);
#pragma omp parallel default(none) shared(index_max, nside, d1, nb2, DD, l_box, rbins2, nbins, r2_max, r2_min)
    {
        long long dd[nbins];

        for (int i = 0; i < nbins; i++) {
            dd[i] = 0;
        }

        long long i, j;
        int i1, j1, k1;
        double x1, y1, z1;
        double dx, dy, dz;
        double x11, y11, z11;
        int iwrapx, iwrapy, iwrapz;
        int i2, j2, k2;
        int s;
        double r2;

#pragma omp for nowait schedule(dynamic)
        for (i = 0; i < d1.np; i++) {
            x1 = d1.pos[3 * i];
            y1 = d1.pos[3 * i + 1];
            z1 = d1.pos[3 * i + 2];

            i1 = (int) (x1 / l_box * nside);
            j1 = (int) (y1 / l_box * nside);
            k1 = (int) (z1 / l_box * nside);

            if (i1 == nside) {
                i1 -= 1;
            }
            if (j1 == nside) {
                j1 -= 1;
            }
            if (k1 == nside) {
                k1 -= 1;
            }

            for (int idx = -index_max; idx <= index_max; idx++) {

                iwrapx = 0;
                i2 = i1 + idx;

                if (i2 < 0) {
                    i2 += nside;
                    iwrapx = -1;
                } else if (i2 >= nside) {
                    i2 -= nside;
                    iwrapx = 1;
                }

                for (int idy = -index_max; idy <= index_max; idy++) {

                    iwrapy = 0;
                    j2 = j1 + idy;

                    if (j2 < 0) {
                        j2 += nside;
                        iwrapy = -1;
                    } else if (j2 >= nside) {
                        j2 -= nside;
                        iwrapy = 1;
                    }

                    for (int idz = -index_max; idz <= index_max; idz++) {

                        iwrapz = 0;
                        k2 = k1 + idz;

                        if (k2 < 0) {
                            k2 += nside;
                            iwrapz = -1;
                        } else if (k2 >= nside) {
                            k2 -= nside;
                            iwrapz = 1;
                        }

                        x11 = x1 - l_box * iwrapx;
                        y11 = y1 - l_box * iwrapy;
                        z11 = z1 - l_box * iwrapz;

                        s = nside * nside * k2 + nside * j2 + i2;

                        for (j = 0; j < nb2[s].np; j++) {

                            dx = x11 - nb2[s].pos[3 * j];
                            dy = y11 - nb2[s].pos[3 * j + 1];
                            dz = z11 - nb2[s].pos[3 * j + 2];

                            r2 = dx * dx + dy * dy + dz * dz;

                            if (r2 > r2_max || r2 < r2_min) continue;
                            for (int k = nbins - 1; k >= 0; k--) {
                                if (r2 >= rbins2[k]) {
                                    (dd[k])++;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

#pragma omp critical
        {
            for (int i = 0; i < nbins; i++)
                DD[i] += dd[i];
        }
    }

    return DD;
}