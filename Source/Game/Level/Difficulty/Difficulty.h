
#pragma once

enum Difficulty_t { EASY, MEDIUM, HARD, CRAZY };

class cDifficulty {
	private:
		int iDiff = EASY;
	
	public:
		bool Set( int NewDiff ) { iDiff = NewDiff; return ( NewDiff >= EASY && NewDiff <= CRAZY ) ?  true : false;  };
		int Get() { return iDiff; };
	
};