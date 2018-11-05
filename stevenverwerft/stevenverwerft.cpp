#include "HW5.cpp"
#include <iostream>
#include <math.h>
#include <algorithm>  // std::max, std::sort
#include <limits.h>  // CHAR_BIT
#include <vector>
#include <array>
#include <numeric>  // accumulate

void init(void) {
	int aa, ab, l, x = 0;
	for (int i = 4; i <= 13; i++) {
		aa = 13 - i;
		for (int j = (aa + 2) / 3; j <= i && j <= aa; j++) {
			ab = aa - j;
			for (int k = (ab + 1) / 2; k <= j && k <= ab; k++) {
				adj[x] = i + j - 8;
				l = ab - k; code[x++] = (((i - 4) * 7) + j) * 5 + k;
				//				printf("%d %d %d %d: %d\n", i, j, k, l, (((i-4)*7)+j)*5+k);
			}
		}
	}
	return;
}


void init_set(void) {
	int x[52], y, a;
	for (int i = 0; i < 52; i++) x[i] = i;
	srand(1);
	
	int *lower_bound_ahp = std::min_element(ahpl, ahpl+39);
	std::array<int, 17> current_hands; // static 1D array to store the hands for the four players

	for (int i = 0; i < CRD; i++) {
		current_hands.fill(0);  // re-initialize the current hands array with zeros each iteration
		
		for (int j = 52; --j >= 0;) {
			
			if (j >= 2) {
				y = rand() % j;
				a = x[y]; x[y] = x[j]; x[j] = a; 
			}
			
			int player = (j / 13);
			int card = x[j] / 13;
		
			char remainder = x[j] % 13;
			if (remainder < 4) {
				hp[i][player] += (4 - remainder);  // task purpose 1
			}

			current_hands[(player*4) + card]++;
		}

		int value = 0;  // value calculated as (((i-4)*7)+j)*5 + k
		int distribution = 0;  // the distribution index for a hand-distribution
		short int player_hand = 0;  // stores the number of each cardcolor in the current hand of a player

		for (int player = 0; player <= 3; player++) {

			player_hand = (current_hands[player*4]) 
				+ (current_hands[player*4 + 1] << 4) 
				+ (current_hands[player*4 + 2] << 8) 
				+ (current_hands[player*4 + 3] << 12);
			
			/*	
				codes: Spades = 0, Diamonds = 1, Hearts = 2, Clubs = 3
				=> shift each card by its code number*4

				e.g.:
					 S  H  D  C
					[3, 3, 5, 2]

					        C    D    H    S
				hand =>  [0010 0101 0011 0011] (= 9523)

				Number of Hearts (H = 1)?

				hand >> 1*4:
				hand => [0000 0010 0101 0011] (= 595)
					  & [0000 0000 0000 1111] (= 15)
						--------------------
						[0000 0000 0000 0011] (= 3)
			*/

			// inline sort of the player's hand => to find the distribution
			std::sort(&current_hands[player*4], &current_hands[player*4+4], [](int a, int b) {return a > b; });

			value = ((   // value calculated as (((i - 4) * 7) + j) * 5 + k
				(current_hands[player*4] - 4) * 7) 
				+ current_hands[player*4 + 1]) * 5 
				+ current_hands[player*4 + 2];
			
			distribution = std::find(code, code + 39, value) - code;  // find index of value in code[x]
			
			dis[i][player] = distribution;  // task purpose 2
			ahp[i][player] = std::max(hp[i][player] + adj[distribution], 0);

			// check the rules
			bool rule_applied = false;
			if (ahp[i][player] >= *lower_bound_ahp) {
				for (int rule = 0; rule < 41; rule++) {
					if ((distribution >= distl[rule]) && (distribution <= distu[rule]) && (ahp[i][player] >= ahpl[rule]) && (ahp[i][player] <= ahpu[rule]))
					{
						// check for extra restrictions
						int restrictions_met = 0;
						for (int extra_res = 0; extra_res < res[rule]; extra_res++) {

							int num_cards = ((player_hand >> (color[rule][extra_res] * 4)) & 15);
							if ((num_cards >= nrl[rule][extra_res]) && (num_cards <= nru[rule][extra_res])) {
								restrictions_met += 1;
								continue;
							}
							else { break; }
						}
						if (restrictions_met == res[rule]) {
							cnt[rule[bid]][player]++;  // task purpose 3
							rule_applied = true;
							break;
						}
					}
				}
			}
			if (!rule_applied){
				cnt[0][player]++;
			}
		}
	}

	return;
}

