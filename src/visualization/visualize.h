#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "../ops/gcflobdd_int.h"
#include <graphviz/gvc.h>

namespace Visualization {

    class ForkGrouping {
        public:
            Agnode_t* entry;
            Agnode_t* exit1, *exit2;
    };

    extern void visualizeGCFLOBDD(const G_CFL_OBDD::G_CFLOBDD& gcflobdd);


} // namespace Visualization

#endif