#ifndef _CONNECTION_LIST_T_H
#define _CONNECTION_LIST_T_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "connectionT.h"

namespace G_CFL_OBDD {

	template <typename Handle>
	class ConnectionListT
	{
	public:
		ConnectionListT();                                  // Default constructor
		ConnectionListT(const ConnectionListT &C);              // Copy constructor
		ConnectionListT(unsigned int capacity);
		~ConnectionListT();                                 // Destructor

		unsigned int Hash(unsigned int modsize) const;
		void Reserve(unsigned int capacity);
		ConnectionListT& operator= (const ConnectionListT &C);   // Overloaded =
		bool operator!= (const ConnectionListT & C) const;        // Overloaded !=
		bool operator== (const ConnectionListT & C) const;        // Overloaded ==
		void AddConnection(ConnectionT<Handle>& y);
		ConnectionT<Handle> Lookup(int x);
		long int LookupInv(ConnectionT<Handle>& y);
		unsigned int Size();
		ConnectionT<Handle>& operator[](unsigned int i);                       // Overloaded []

		// std::vector<ConnectionT<Handle>> connections;
		ConnectionT<Handle>* connections;
    	unsigned int numConnections;
		unsigned int currentConnectionIndex;

	public:
		std::ostream& print(std::ostream & out = std::cout) const;

		struct ConnectionListHash {
		public:
			size_t operator()(const ConnectionListT<Handle>& c) const {
				return c.Hash(997);
			}
		};

		struct ConnectionListEqual {
		public:
			bool operator()(const ConnectionListT<Handle>& a, const ConnectionListT<Handle>& b) const {
				return (a == b);
			}
		};
	};


	template <typename Handle>
	std::ostream& operator<< (std::ostream & out, const ConnectionListT<Handle> &c);

}

#endif

