//
// Created by rafa on 1/25/21.
//

#include "pac.h"
#include "define.h"
#include "grid.h"
#include "counter.h"
#include "stdlib.h"


long long *DD(long long npart, double *d1, double boxsize, int nside, double minsep, double maxsep, int nbins) {
    // Build Catalog struct from array
    Catalog catalog;
    catalog.np = (long long) npart;
    catalog.pos = (double *) malloc(3 * catalog.np * sizeof(double));
    for (long long i = 0; i < 3*catalog.np; i++) {
        catalog.pos[i] = d1[i];
    }

    // Build Grid struct from Catalog
    Grid *boxes = catalog_to_boxes(boxsize, nside, catalog);

    // Do pair counting
    long long *DD = pair_count(catalog, catalog, boxes, boxes, boxsize, nside, minsep, maxsep, nbins);

    return DD;
}