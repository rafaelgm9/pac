//
// Created by rafa on 9/28/20.
//

#include "grid.h"
#include "define.h"
#include <stdlib.h>


Grid *catalog_to_boxes(double l_box, int nside, Catalog cat) {
    Grid *boxes;

    boxes = (Grid *) malloc(nside * nside * nside * sizeof(Grid));

    for (int i = 0; i < nside*nside*nside; i++) {
        boxes[i].np = 0;
    }

    for (long long ii = 0; ii < cat.np; ii++) {
        int i, j, k;
        int s;

        i = (int) (cat.pos[3*ii] / l_box * nside);
        j = (int) (cat.pos[3*ii+1] / l_box * nside);
        k = (int) (cat.pos[3*ii+2] / l_box * nside);

        if (i == nside) {
            i -= 1;
        }
        if (j == nside) {
            j -= 1;
        }
        if (k == nside) {
            k -= 1;
        }

        s = nside*nside*k + nside*j + i;

        (boxes[s].np)++;
    }

    for (int i = 0; i < nside*nside*nside; i++) {
        if (boxes[i].np > 0) {
            boxes[i].pos = (double *) malloc(3 * boxes[i].np * sizeof(double));
        }
        boxes[i].np = 0;
    }

    for (long long ii = 0; ii < cat.np; ii++) {
        int i, j, k;
        int s;
        long long offset;

        i = (int) (cat.pos[3*ii] / l_box * nside);
        j = (int) (cat.pos[3*ii+1] / l_box * nside);
        k = (int) (cat.pos[3*ii+2] / l_box * nside);

        if (i == nside) {
            i -= 1;
        }
        if (j == nside) {
            j -= 1;
        }
        if (k == nside) {
            k -= 1;
        }

        s = nside*nside*k + nside*j + i;

        offset = 3*boxes[s].np;
        boxes[s].pos[offset] = cat.pos[3*ii];
        boxes[s].pos[offset+1] = cat.pos[3*ii+1];
        boxes[s].pos[offset+2] = cat.pos[3*ii+2];
        boxes[s].np++;
    }

    return boxes;
}