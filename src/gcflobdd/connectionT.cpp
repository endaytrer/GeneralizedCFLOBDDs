#include "gcflobdd_node.h"
#include "connectionT.h"

namespace G_CFL_OBDD {


// Default constructor
template <typename Handle>
ConnectionT<Handle>::ConnectionT()
{
}

// // Copy constructor
// template <typename Handle>
// ConnectionT<Handle>::ConnectionT(const ConnectionT<Handle> &C)
//     : entryPointHandle(new G_CFLOBDDNodeHandle(*(C.entryPointHandle))), returnMapHandle(C.returnMapHandle)
// {
// }

// Constructor
template <typename Handle>
ConnectionT<Handle>::ConnectionT(G_CFLOBDDNodeHandle &entryPointHandle, Handle &returnMapHandle)
	: entryPointHandle(new G_CFLOBDDNodeHandle(entryPointHandle)), returnMapHandle(returnMapHandle)
{
}

// Constructor
template <typename Handle>
ConnectionT<Handle>::ConnectionT(G_CFLOBDDNode *entryPoint, Handle &returnMapHandle)
	: entryPointHandle(new G_CFLOBDDNodeHandle(entryPoint)), returnMapHandle(returnMapHandle)
{
}

// template <typename Handle>
// ConnectionT<Handle>::~ConnectionT()
// {
// }

// Hash
template <typename Handle>
unsigned int ConnectionT<Handle>::Hash(unsigned int modsize) const
{
	unsigned int hvalue = 0;
	hvalue = (997 * returnMapHandle.Hash(modsize) + entryPointHandle->Hash(modsize)) % modsize;
	return hvalue;
}

// Overloaded =
template <typename Handle>
ConnectionT<Handle>& ConnectionT<Handle>::operator= (const ConnectionT<Handle>& C)
{
	if (this != &C)      // don't assign to self!
	{
		entryPointHandle = C.entryPointHandle;
		returnMapHandle = C.returnMapHandle;
	}
	return *this;
}

// Overloaded !=
template <typename Handle>
bool ConnectionT<Handle>::operator!= (const ConnectionT<Handle> & C) const
{
	return (returnMapHandle != C.returnMapHandle) || ((*entryPointHandle) != (*C.entryPointHandle));
}

// Overloaded ==
template <typename Handle>
bool ConnectionT<Handle>::operator== (const ConnectionT<Handle> & C) const
{
	return (returnMapHandle == C.returnMapHandle) && ((*entryPointHandle) == (*C.entryPointHandle));
}

// print
template <typename Handle>
std::ostream& ConnectionT<Handle>::print(std::ostream & out) const
{
	for (unsigned int i = entryPointHandle->handleContents->level; i < 5; i++) {  // Indentation
	    out << "  ";
	}
	out << (*entryPointHandle);
	for (unsigned int i = entryPointHandle->handleContents->level; i < 5; i++) {  // Indentation
	    out << "  ";
	}
	out << returnMapHandle;
	return out;
}

template <typename Handle>	
std::ostream& operator<< (std::ostream & out, const ConnectionT<Handle> &c)
	{
		c.print(out);
		return(out);
	}
} // namespace G_CFL_OBDD
