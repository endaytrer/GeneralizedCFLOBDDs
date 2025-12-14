#include "gcflobdd_bdd_node.h"
#include "sectionT.h"

namespace G_CFL_OBDD {


// Default constructor
template <typename Handle>
SectionT<Handle>::SectionT()
{
}

// // Copy constructor
// template <typename Handle>
// SectionT<Handle>::SectionT(const SectionT<Handle> &C)
//     : entryPointHandle(new BDDNodeHandle(*(C.entryPointHandle))), returnMapHandle(C.returnMapHandle)
// {
// }

// Constructor
template <typename Handle>
SectionT<Handle>::SectionT(BDDNodeHandle &entryPointHandle, Handle &returnMapHandle)
	: entryPointHandle(new BDDNodeHandle(entryPointHandle)), returnMapHandle(returnMapHandle)
{
}

// Constructor
template <typename Handle>
SectionT<Handle>::SectionT(BDDNode *entryPoint, Handle &returnMapHandle)
	: entryPointHandle(new BDDNodeHandle(entryPoint)), returnMapHandle(returnMapHandle)
{
}

// Copy constructor
template <typename Handle>
SectionT<Handle>::SectionT(const SectionT<Handle> &C)
{
	entryPointHandle = new BDDNodeHandle(*(C.entryPointHandle));
	returnMapHandle = C.returnMapHandle;
}

// Destructor
template <typename Handle>
SectionT<Handle>::~SectionT()
{
	delete entryPointHandle;
	entryPointHandle = NULL;
}

// Hash
template <typename Handle>
unsigned int SectionT<Handle>::Hash(unsigned int modsize) const
{
	unsigned int hvalue = 0;
	hvalue = (997 * returnMapHandle.Hash(modsize) + entryPointHandle->Hash(modsize)) % modsize;
	return hvalue;
}

// Overloaded =
template <typename Handle>
SectionT<Handle>& SectionT<Handle>::operator= (const SectionT<Handle>& C)
{
	if (this != &C)      // don't assign to self!
	{
		entryPointHandle = new BDDNodeHandle(*(C.entryPointHandle));
		returnMapHandle = C.returnMapHandle;
	}
	return *this;
}

// Overloaded !=
template <typename Handle>
bool SectionT<Handle>::operator!= (const SectionT<Handle> & C) const
{
	return (returnMapHandle != C.returnMapHandle) || ((*entryPointHandle) != (*C.entryPointHandle));
}

// Overloaded ==
template <typename Handle>
bool SectionT<Handle>::operator== (const SectionT<Handle> & C) const
{
	return (returnMapHandle == C.returnMapHandle) && ((*entryPointHandle) == (*C.entryPointHandle));
}

// print
template <typename Handle>
std::ostream& SectionT<Handle>::print(std::ostream & out) const
{
	out << (*entryPointHandle);
	out << returnMapHandle;
	return out;
}

template <typename Handle>	
std::ostream& operator<< (std::ostream & out, const SectionT<Handle> &c)
	{
		c.print(out);
		return(out);
	}
} // namespace G_CFL_OBDD
