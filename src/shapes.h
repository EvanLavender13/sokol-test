#ifndef LAVS_SHAPES_H_
#define LAVS_SHAPES_H_

#include "stdio.h"

#include "sokol_shape.h"

void shapes_box(uint32_t tiles)
{
    sshape_sizes_t size = sshape_box_sizes(tiles);
    printf("box size: vertices [%i] indices [%i]\n", size.vertices.num, size.indices.num);
}

#endif