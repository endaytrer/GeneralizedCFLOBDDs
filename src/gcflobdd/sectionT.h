#ifndef _SECTION_T_H
#define _SECTION_T_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "gcflobdd_bdd_node.h"

namespace G_CFL_OBDD {
	class G_CFLOBDDBDDNode;
	class BDDNode;
	class BDDNodeHandle;
}


namespace G_CFL_OBDD {

	template <typename Handle>
	class SectionT
	{
	public:
		SectionT();                                  // Default constructor
		SectionT(const SectionT &C);              // Copy constructor
		SectionT(BDDNode *entryPoint, Handle &returnMapHandle);
		SectionT(BDDNodeHandle &entryPointHandle, Handle &returnMapHandle);
		~SectionT();                                 // Destructor

		unsigned int Hash(unsigned int modsize) const;
		SectionT& operator= (const SectionT &C);   // Overloaded =
		bool operator!= (const SectionT & C) const;        // Overloaded !=
		bool operator== (const SectionT & C) const;        // Overloaded ==

		BDDNodeHandle* entryPointHandle = NULL;
		Handle returnMapHandle;

	public:
		std::ostream& print(std::ostream & out = std::cout) const;

		struct SectionHash {
		public:
			size_t operator()(const SectionT<Handle>& c) const {
				return c.Hash(997);;
			}
		};

		struct SectionEqual {
		public:
			bool operator()(const SectionT<Handle>& a, const SectionT<Handle>& b) const {
				return (a == b);
			}
		};
	};


	template <typename Handle>
	std::ostream& operator<< (std::ostream & out, const SectionT<Handle> &c);

}

#endif

