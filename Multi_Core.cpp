#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include<cstdlib>
#include<cstring>

#if defined(__linux__)
#include<sched.h>
#elif defined(__QNXNTO__)
#include <sys/syspage.h>
#include <sys/neutrino.h>
#endif

#include "Binomial_Multi.hpp"
#include "Timer.hpp"

#define RED     "\033[1m\033[31m" 
#define MAGENTA "\033[1m\033[35m"

#if defined(__QNXNTO__)
void Set_CPU_Affinity(unsigned ID) {
	int *rsizep, masksize_bytes, size;
	int num_elements;
	void *my_data;
	/* Determine the size of the runmask, in bytes. */
	num_elements = RMSK_SIZE(_syspage_ptr->num_cpu);
	masksize_bytes = num_elements * sizeof(unsigned);
	/* Allocate memory for the data structure that we'll pass
	 * to ThreadCtl(). We need space for an integer (the number
	 * of elements in each mask array) and the two masks
	 * (runmask and inherit mask). */
	
	size = sizeof(int) + 2 * masksize_bytes;
	my_data = malloc(size);
	if (my_data == nullptr) return;

	memset(my_data, 0x00, size);
	
	/* Set up pointers to the "members" of the structure. */
  int *rmaskp, *imaskp;
	rsizep = (int *)my_data;
	rmaskp = rsizep + 1;
	imaskp = rmaskp + num_elements;
	/* Set the size. */
  *rsizep = num_elements;
     
	RMSK_SET(ID, rmaskp);
	RMSK_SET(ID, imaskp);
#if defined(__DEBUG__)
	std::cout << "rmaskp: " << *rmaskp << " imaskp: " << *imaskp << "\n"; 
#endif
	ThreadCtl( _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT, &my_data);
	free(my_data);
}
#endif

void Thread_Entry_Func(Binomial_Mult *P1, unsigned ID) {
	
#if defined(__QNXNTO__)
	Set_CPU_Affinity(ID);
#endif
	if (P1 != nullptr)
		P1->Matrix_Mult();
#if defined(__linux__)
//	std::cout << "Thread: is running on CPU: " << sched_getcpu() << "\n";
#endif
}

void Run_Thread(const std::vector<Binomial_Mult*> Bino, const int Count, const int No_of_CPU) {
	std::vector<std::thread> Thread_Pool(Count);
  {
		
		Timer T2("With Thread-------->");
		int i = 0;
		for (auto& T : Thread_Pool) {
			T = std::thread(Thread_Entry_Func, Bino[i], i%No_of_CPU);
#if defined(__linux__)
			cpu_set_t cpuset;
			CPU_ZERO(&cpuset);
			CPU_SET(i%No_of_CPU, &cpuset);
			int rc = pthread_setaffinity_np(T.native_handle(),
                                sizeof(cpu_set_t), &cpuset);
#elif defined(__QNXNTO__)
#endif
			++i;
		}
		for (auto& T : Thread_Pool) 
			T.join();
	}
}



int main(int argc, char *argv[]) {
	int Count = 0;
	int No_of_CPU = 0;
	unsigned long Iteration;	
	char *Temp;

	if (argc != 2) {
		std::cerr << "Enter Number of Iteration\n";
		exit(1);
	}

	Iteration = strtoul(argv[1], &Temp, 10);
	if (Iteration == 0) {
		std::cerr << "Iteration must be greater than 0\n";
		exit(0);
	}	

#if defined(__linux__)
	cpu_set_t Set;
	if (sched_getaffinity(0, sizeof(Set), &Set) == 0) {
#if 0
		for (auto i = 0; i < CPU_COUNT; i++) {
			if (CPU_ISSETD(i, &Set)) Count++;
		}
#endif
		No_of_CPU = CPU_COUNT(&Set);
	}
#elif defined(__QNXNTO__)
	No_of_CPU = _syspage_ptr->num_cpu;

#endif
	Count = No_of_CPU + 1;
	std::cout << "Number of Cores : " << No_of_CPU << "\n";
	std::cout << "Num of threads  : " << Count << "\n";

	Binomial_Mult B1(Iteration, std::string("matrix1.in"));
	Binomial_Mult B2(Iteration, std::string("matrix2.in"));
	Binomial_Mult B3(Iteration, std::string("matrix3.in"));
	Binomial_Mult B4(Iteration, std::string("matrix4.in"));
	Binomial_Mult B5(Iteration, std::string("matrix5.in"));
	Binomial_Mult B6(Iteration, std::string("matrix6.in"));
	Binomial_Mult B7(Iteration, std::string("matrix7.in"));
	Binomial_Mult B8(Iteration, std::string("matrix8.in"));
	Binomial_Mult B9(Iteration, std::string("matrix8.in"));
	
	std::vector<Binomial_Mult*> Bino;
	Bino.push_back(&B1);
	Bino.push_back(&B2);
	Bino.push_back(&B3);
	Bino.push_back(&B4);
	Bino.push_back(&B5);
	Bino.push_back(&B6);
	Bino.push_back(&B7);
	Bino.push_back(&B8);
	Bino.push_back(&B9);

  {
		Timer T1("With-Out Thread---->");
		for (auto i = 0; i < Count; i++) {
			Bino[i]->Matrix_Mult();
		}
  }

	Run_Thread(Bino, Count, No_of_CPU);		

	return 0;	
}
