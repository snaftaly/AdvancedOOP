#include "_305220980_A.h"

Direction _305220980_A::step(){
	std::vector<Direction> possibleMoves {Direction::North,Direction::South,Direction::West,Direction::East,Direction::Stay};
	return getStep(possibleMoves);
}
