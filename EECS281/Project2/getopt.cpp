#include <iostream>
#include <getopt.h>
#include <string>

using namespace std;

int main(int argc, char **argv){
	/*
	this program takes in two arguments
	--optionA or -a does not have a required option
	--optionB or -b DOES have a required option.
	*/
	struct option longOpts[] = {
		{"optionA", no_argument, NULL, 'a'},
		{"optionB", required_argument, NULL, 'b'}
	};

	int opt = 0, index = 0;
	while((opt = getopt_long (argc, argv, "ab:", longOpts, &index)) != -1){
		switch(opt) {
			case 'a':
				cout << "you have triggered option A\n";
				break;
			case 'b':
				cout << "you have triggered option B with option " << optarg << "\n";
				//optarg is defined in getopt.h
				break;
			case '?':
				cout << "I didn't recognize one of your flags\n";
				break;
		}
	}
	return 0;
}