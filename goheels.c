/*
 * goheels.c
 *
 *  Created on: Sep 29, 2016
 *      Author: kayleellewellyn
 */
#include <stdio.h>

int main(){

	int i;
	char *tarheels[1024];

	tarheels[0] = ".----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.\n";
	tarheels[1] = "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |\n";
	tarheels[2] = "| |  _________   | || |      __      | || |  _______     | || |  ____  ____  | || |  _________   | || |  _________   | || |   _____      | |\n";
	tarheels[3] = "| | |  _   _  |  | || |     /  \\     | || | |_   __ \\    | || | |_   ||   _| | || | |_   ___  |  | || | |_   ___  |  | || |  |_   _|     | |\n";
	tarheels[4] = "| | |_/ | | \\_|  | || |    / /\\ \\    | || |   | |__) |   | || |   | |__| |   | || |   | |_  \\_|  | || |   | |_  \\_|  | || |    | |       | |\n";
	tarheels[5] = "| |     | |      | || |   / ____ \\   | || |   |  __ /    | || |   |  __  |   | || |   |  _|  _   | || |   |  _|  _   | || |    | |   _   | |\n";
	tarheels[6] = "| |    _| |_     | || | _/ /    \\ \\_ | || |  _| |  \\ \\_  | || |  _| |  | |_  | || |  _| |___/ |  | || |  _| |___/ |  | || |   _| |__/ |  | |\n";
	tarheels[7] = "| |   |_____|    | || ||____|  |____|| || | |____| |___| | || | |____||____| | || | |_________|  | || | |_________|  | || |  |________|  | |\n";
	tarheels[8] = "| |              | || |              | || |              | || |              | || |              | || |              | || |              | |\n";
	tarheels[9] = "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |\n";
	tarheels[10] = " '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' \n";

	for(i = 0; i < 11; i++){
		printf("%s",tarheels[i]);
	}



	return 0;

}