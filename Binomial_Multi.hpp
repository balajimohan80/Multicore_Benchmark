#ifndef __BINOMIAL_MULTIPLICATION__
#define __BINOMIAL_MULTIPLICATION__

#include<iostream>
#include<string>
#include<vector>

class Binomial_Mult {
private:
	const unsigned int No_Of_Iterations;
	const std::string Matrix_Data_File;

	int Size;
	std::vector<std::vector<int>> SQ_Matrix;
	std::vector<std::vector<int>> Res_Matrix;
	int Fill_Matrix_Data();

public:
	Binomial_Mult(unsigned int, const std::string &);
	void Display_Matrix_Data();
	void Matrix_Mult();
};

#endif
