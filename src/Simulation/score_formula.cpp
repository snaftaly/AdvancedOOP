#include <map>
#include <string>

using namespace std;

extern "C" {
	int calc_score(const map<string, int>& score_params){
		int actual_position_in_competition;
		int winner_num_steps;
		int this_num_steps;
		int sum_dirt_in_house;
		int dirt_collected;
		int is_back_in_docking;

		int position_in_competition;
		int score;

		// check if one of the params don't exist
		if (score_params.find("actual_position_in_competition") == score_params.end() ||
			score_params.find("winner_num_steps") == score_params.end() ||
			score_params.find("this_num_steps") == score_params.end() ||
			score_params.find("sum_dirt_in_house") == score_params.end() ||
			score_params.find("dirt_collected") == score_params.end() ||
			score_params.find("is_back_in_docking") == score_params.end())
		{
			return -1;
		}

		actual_position_in_competition = score_params.find("actual_position_in_competition")->second;
		winner_num_steps = score_params.find("winner_num_steps")->second;
		this_num_steps = score_params.find("this_num_steps")->second;
		sum_dirt_in_house = score_params.find("sum_dirt_in_house")->second;
		dirt_collected = score_params.find("dirt_collected")->second;
		is_back_in_docking = score_params.find("is_back_in_docking")->second;

		if (sum_dirt_in_house == dirt_collected && is_back_in_docking){
			position_in_competition = min(4, actual_position_in_competition);
		}
		else{
			position_in_competition = 10;
		}

		score = max(0,
				2000
				- (position_in_competition - 1)*50
				+ (winner_num_steps - this_num_steps)*10
				- (sum_dirt_in_house - dirt_collected)*3
				+ (is_back_in_docking ? 50 : -200)
				);
		return score;
	}
}
