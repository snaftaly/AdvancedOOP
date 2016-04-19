#include "_305220980_A.h"

Direction _305220980_A::step(){
	// TODO: do some shuffling to not get stuck
	std::vector<Direction> possibleMoves {Direction::North,Direction::West,Direction::South,Direction::East,Direction::Stay};
	return getStep(possibleMoves);
}
