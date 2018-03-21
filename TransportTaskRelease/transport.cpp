#include "Pot.h"

void main()
{
	Transport l_Transport("InputFines.txt");
	l_Transport.NWCorner();
	l_Transport.PotentialSolution();
	l_Transport.Print("Output.txt");
}