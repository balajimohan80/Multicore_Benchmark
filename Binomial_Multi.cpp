#include<Binomial_Multi.hpp>

Binomial_Mult::Binomial_Mult(unsigned int Iteration, const std::string &F_Name):
	No_Of_Iterations(Iteration),Matrix_Data_File(F_Name) {
	Fill_Matrix_Data();
}

int Binomial_Mult::Fill_Matrix_Data() {
	FILE *F = fopen(static_cast<const char*>(Matrix_Data_File.c_str()), "r+");
	if (F == nullptr) {
		std::cerr << "Not able to open " << Matrix_Data_File << "\n";
		return -1;
	}
	fscanf(F, "%d", &Size); 
	
	SQ_Matrix.resize(Size);
	
	for (auto i = 0; i < Size; i++) {
		for (auto j = 0; j < Size; j++) {
			int Temp;
			fscanf(F, "%d",&Temp);
			SQ_Matrix[i].push_back(Temp);
		}
	}

	//Set Res matrix size based on Input Matrix size
	Res_Matrix.resize(Size);
	for (auto i = 0; i < Size; i++) {
		Res_Matrix[i].resize(SQ_Matrix[i].size(), 0);
	}
	fclose(F);
}

void Binomial_Mult::Display_Matrix_Data() {
	for (auto i = 0; i < SQ_Matrix.size(); i++) {
		for (auto j : SQ_Matrix[i]) {
			std::cout << j << " ";
		}
		std::cout << "\n";
	}
}


void Binomial_Mult::Matrix_Mult() {
	for (auto l = 0; l < No_Of_Iterations; l++) {
		for (auto i = 0; i < Size; i++) {
			for (auto j = 0; j < Size; j++) {
				Res_Matrix[i][j] = 0;
				for (auto k = 0; k < Size; k++) {
					Res_Matrix[i][j] +=
						(SQ_Matrix[i][k] * SQ_Matrix[k][j]);
				}
			}		
		}

		for (auto i = 0; i < Size; i++)
			for (auto j = 0; j < Size; j++)
				SQ_Matrix[i][j] = Res_Matrix[i][j];	
	}	
}


