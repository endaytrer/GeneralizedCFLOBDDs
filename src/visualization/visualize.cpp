#include "visualize.h"
#include <graphviz/gvc.h>
using namespace G_CFL_OBDD;

namespace Visualization {

    ForkGrouping createForkVisualization(unsigned int x_offset, unsigned int y_offset, Agraph_t* g) {
        // --- Create oval2 (subgraph/cluster) ---
        Agraph_t *oval = agsubg(g, const_cast<char*>("cluster_fork"), 1);
        agsafeset(oval, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));  // no label
        agsafeset(oval, const_cast<char*>("style"), const_cast<char*>("filled,rounded"), const_cast<char*>(""));
        agsafeset(oval, const_cast<char*>("fillcolor"), const_cast<char*>("white"), const_cast<char*>(""));
        agsafeset(oval, const_cast<char*>("margin"), const_cast<char*>("2,2"), const_cast<char*>("")); // wider → oval
        agsafeset(oval, const_cast<char*>("penwidth"), const_cast<char*>("2"), const_cast<char*>(""));   // thicker border

        auto entry = agnode(oval, NULL, 1);
        agsafeset(entry, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));
        agsafeset(entry, const_cast<char*>("shape"), const_cast<char*>("point"), const_cast<char*>(""));
        agsafeset(entry, const_cast<char*>("width"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(entry, const_cast<char*>("height"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(entry, const_cast<char*>("fixedsize"), const_cast<char*>("true"), const_cast<char*>(""));
        agsafeset(entry, const_cast<char*>("pin"), const_cast<char*>("true"), const_cast<char*>(""));
        std::string entry_pos;
        entry_pos = std::to_string(x_offset) + "," + std::to_string(y_offset) + "!";
        agsafeset(entry, const_cast<char*>("pos"), const_cast<char*>(entry_pos.c_str()), const_cast<char*>(""));


        auto exit1 = agnode(oval, NULL, 1);
        agsafeset(exit1, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));
        agsafeset(exit1, const_cast<char*>("shape"), const_cast<char*>("point"), const_cast<char*>(""));
        agsafeset(exit1, const_cast<char*>("width"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(exit1, const_cast<char*>("height"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(exit1, const_cast<char*>("fixedsize"), const_cast<char*>("true"), const_cast<char*>(""));
        agsafeset(exit1, const_cast<char*>("pin"), const_cast<char*>("true"), const_cast<char*>(""));
        std::string exit1_pos;
        exit1_pos = std::to_string(x_offset - 1) + "," + std::to_string(y_offset - 1) + "!";
        agsafeset(exit1, const_cast<char*>("pos"), const_cast<char*>(exit1_pos.c_str()), const_cast<char*>(""));

        auto exit2 = agnode(oval, NULL, 1);
        agsafeset(exit2, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));
        agsafeset(exit2, const_cast<char*>("shape"), const_cast<char*>("point"), const_cast<char*>(""));
        agsafeset(exit2, const_cast<char*>("width"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(exit2, const_cast<char*>("height"), const_cast<char*>("0.2"), const_cast<char*>(""));
        agsafeset(exit2, const_cast<char*>("fixedsize"), const_cast<char*>("true"), const_cast<char*>(""));
        agsafeset(exit2, const_cast<char*>("pin"), const_cast<char*>("true"), const_cast<char*>(""));
        std::string exit2_pos;
        exit2_pos = std::to_string(x_offset + 1) + "," + std::to_string(y_offset - 1) + "!";
        agsafeset(exit2, const_cast<char*>("pos"), const_cast<char*>(exit2_pos.c_str()), const_cast<char*>(""));

        class ForkGrouping forkGroup;
        forkGroup.entry = entry;
        forkGroup.exit1 = exit1;
        forkGroup.exit2 = exit2;

        // Fork
        agedge(g, forkGroup.entry, forkGroup.exit1, nullptr, 1);
        agedge(g, forkGroup.entry, forkGroup.exit2, nullptr, 1);

        return forkGroup;
    }

    void visualizeGCFLOBDDAtLevel(const G_CFLOBDD& gcflobdd, int level, const std::shared_ptr<GrammarNode>& grammar, GVC_t* gvc, Agraph_t* g) {
        
            

    }

    void visualizeGCFLOBDD(const G_CFLOBDD& gcflobdd) {

        // Create a Graphviz context
        GVC_t *gvc = gvContext();

        Agraph_t *g = agopen(const_cast<char*>("G"), Agdirected, NULL);
        agsafeset(g, const_cast<char*>("layout"), const_cast<char*>("neato"), const_cast<char*>(""));
        agsafeset(g, const_cast<char*>("overlap"), const_cast<char*>("false"), const_cast<char*>(""));
        agsafeset(g, const_cast<char*>("splines"), const_cast<char*>("true"), const_cast<char*>(""));

        // // --- Create oval1 (subgraph/cluster) ---
        // Agraph_t *oval1 = agsubg(g, const_cast<char*>("cluster_0"), 1);
        // agsafeset(oval1, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));
        // agsafeset(oval1, const_cast<char*>("style"), const_cast<char*>("filled"), const_cast<char*>(""));
        // agsafeset(oval1, const_cast<char*>("fillcolor"), const_cast<char*>(""), const_cast<char*>(""));
        // agsafeset(oval1, const_cast<char*>("width"), const_cast<char*>("3"), const_cast<char*>(""));
        // agsafeset(oval1, const_cast<char*>("height"), const_cast<char*>("2.5"), const_cast<char*>(""));

        // std::vector<Agnode_t*> nodesOval1;
        // for (int i = 0; i < 3; i++) {
        //     auto n = agnode(oval1, NULL, 1);

        //     agsafeset(n, const_cast<char*>("label"), const_cast<char*>(""), const_cast<char*>(""));
        //     agsafeset(n, const_cast<char*>("shape"), const_cast<char*>("point"), const_cast<char*>(""));
        //     agsafeset(n, const_cast<char*>("width"), const_cast<char*>("0.2"), const_cast<char*>(""));
        //     agsafeset(n, const_cast<char*>("height"), const_cast<char*>("0.2"), const_cast<char*>(""));
        //     agsafeset(n, const_cast<char*>("fixedsize"), const_cast<char*>("true"), const_cast<char*>(""));

        //     std::string pos;
        //     if (i == 0) pos = "2,2!";
        //     else if (i == 1) pos = "1,1!";
        //     else pos = "3,1!";
        //     agsafeset(n, const_cast<char*>("pos"), const_cast<char*>(pos.c_str()), const_cast<char*>(""));
        //     agsafeset(n, const_cast<char*>("pin"), const_cast<char*>("true"), const_cast<char*>(""));

        //     nodesOval1.push_back(n);
        // }


        ForkGrouping forkGroup = createForkVisualization(4, 4, g);

        // // Calls
        // agedge(g, nodesOval1[0], forkGroup.exit1, nullptr, 1);
        // agedge(g, nodesOval1[1], forkGroup.exit1, nullptr, 1);
        // agedge(g, nodesOval1[2], forkGroup.exit2, nullptr, 1);

        // // Return
        // agedge(g, forkGroup.exit1, nodesOval1[1], nullptr, 1);
        // agedge(g, forkGroup.exit2, nodesOval1[2], nullptr, 1);

        // Layout with dot engine
        gvLayout(gvc, g, "dot");

        // Render to file
        gvRenderFilename(gvc, g, "png", "output.png");

        // Free memory
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);

    }

} // namespace Visualization