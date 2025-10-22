#ifndef _CONNECTION_T_H
#define _CONNECTION_T_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "connectionT.h"

namespace G_CFL_OBDD {

	
	class G_CFLOBDDNode;
	class G_CFLOBDDInternalNode;   //  : public G_CFLOBDDNode
	class G_CFLOBDDLeafNode;       //  : public G_CFLOBDDNode
	class G_CFLOBDDForkNode;       //  : public G_CFLOBDDLeafNode
	class G_CFLOBDDDontCareNode;   //  : public G_CFLOBDDLeafNode
	class G_CFLOBDDNodeHandle;
}


namespace G_CFL_OBDD {

	template <typename Handle>
	class ConnectionT
	{
	public:
		ConnectionT();                                  // Default constructor
		ConnectionT(const ConnectionT &C) = default;              // Copy constructor
		ConnectionT(G_CFLOBDDNode *entryPoint, Handle &returnMapHandle);
		ConnectionT(G_CFLOBDDNodeHandle &entryPointHandle, Handle &returnMapHandle);
		~ConnectionT() = default;                                 // Destructor

		unsigned int Hash(unsigned int modsize) const;
		ConnectionT& operator= (const ConnectionT &C);   // Overloaded =
		bool operator!= (const ConnectionT & C) const;        // Overloaded !=
		bool operator== (const ConnectionT & C) const;        // Overloaded ==

		G_CFLOBDDNodeHandle* entryPointHandle = NULL;
		Handle returnMapHandle;

	public:
		std::ostream& print(std::ostream & out = std::cout) const;
	};


	template <typename Handle>
	std::ostream& operator<< (std::ostream & out, const ConnectionT<Handle> &c);

}

#endif

