#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip> 
#include <vector>
#include "keno.h"
#include <sstream>
int main(int argc, char **argv){
	/*======================================Validando a abertura do arquivo========================================*/
	/*=============================================================================================================*/
	




	std::ifstream ifs;
	
	if(argc != 2){
		std::cerr << "\n>>> Incorrect amount of input arguments. Try again.\n\n";
		return EXIT_FAILURE;
	}
	
	//Concatenando o nome do arquivo com o caminho.
	std::string file_path = "data/" + std::string(argv[1]);
	ifs.open(file_path);
	
	if(!ifs.is_open()){
		std::cerr << "\n>>> Unfortunately, the file [" << file_path << "] could not be opened. Try again.\n\n";
		return EXIT_FAILURE;
	}

	



	/*=====================================Realizando a leitura do arquivo=========================================*/
	/*=============================================================================================================*/
	



	std::cout << ">>> Preparing to read bet file [" << file_path << "], please wait...\n";
	std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;

	//Criando vector de strings e adcionando valores a partir do fluxo.
	std::vector <std::string> betv; 
	inputFileValues(ifs, betv);

	ifs.close();

	//Verifica se há erro no arquivo.
	bool wage_is_correct = true;
	bool round_is_correct = true;
	bool spots_is_correct = true;
	
	wage_is_correct = check_wage(betv[0]);
	if(wage_is_correct == false){
		std::cerr << "\n>>> Error! The amount or content of wages in the file is incorrect.\n";
		std::cerr << "Remember: you can only provide 1 value greater than 0.\n\n"; 
		return EXIT_FAILURE;
	}
	round_is_correct = check_round(betv[1]);
	if(round_is_correct == false){
		std::cerr << "\n>>> Error! The amount or content of round in the file is incorrect.\n"; 
		std::cerr << "Remember: you can only provide 1 value greater than 0.\n\n"; 
		return EXIT_FAILURE;
	}
	spots_is_correct = check_spots(betv[2]);
	if(spots_is_correct == false){
		std::cerr << "\n>>> Error! There are some errors in the spots or you did not provide any spot.\n";
		std::cerr << "Remember: the spots must be between 1 and 80 inclusive.\n\n";
		return EXIT_FAILURE;
	}

	std::cout << ">>> Bet successfully read!\n";

	




	/*=================================Realizando leitura do arquivo da tabela====================================*/
	/*============================================================================================================*/
	
	



	//Tabela com os fatores que serão muitiplicados com a aposta em dinheiro.
	std::ifstream input;
	input.open("data/tabela.txt");

	if(!input.is_open()){
		std::cerr << "\n>>> Unfortunately, the file [tabela.txt] could not be opened. Try again.\n\n";
		return EXIT_FAILURE;
	}

	//Cria vector de strings.
	std::vector<std::string> string_table;
	inputFileValues(input, string_table);

	std::vector<std::vector<cash_type>> payout_table(15);
	for (int i = 0 ; i < 15 ; i++ ){
   		payout_table[i].resize(16);
	}
	
	//Transforma a tabela de vector de strings para matrix de float.
	createMatrix(string_table, payout_table);
	input.close();






	/*=============================Criação dos vectors do arquivo da aposta=======================================*/
	/*============================================================================================================*/

	

	//Criando um vector que recebe os 20 primeiros elementos de um shuffle de um array de 80 elementos. 
	set_of_numbers_type hits(20);
	generate_hits(hits);

	//Conta a quantidade de valores que existe na string do arquivo.
	number_type repeated_size = countFromArq(betv[2]);
	
	//Converte a string em um vetor com possíveis valores repetidos.
	set_of_numbers_type repeated_vector(repeated_size);
	copyFromArq(betv[2], repeated_vector);

	//Criando um vector, tornando seus valores únicos e ordenado-o.
	set_of_numbers_type my_bet(repeated_size);
	my_bet = unique_ordered_values(repeated_vector);

	if(my_bet.size() > 15){
		my_bet.resize(15);
	}
	
	


	/*==============================================Começo do jogo==================================================*/
	/*==============================================================================================================*/

	std::istringstream iss(betv[0]);
	float IC;
	iss >> IC;
	iss.clear();
	
	iss.str(betv[1]);
	int NR;
	iss >> NR;
	iss.clear();



	KenoBet bet;

	//Adiciona o valor total que será investido. 
	bet.set_wage(IC);

	//Adiciona os numeros da aposta ao objeto.
	for(size_t i = 0; i < my_bet.size(); i++){
		bet.add_number(my_bet[i]);
	}

	
	std::cout << std::setw(4) 
			  << std::setfill(' ') 
			  << "" 
			  << "You are going to wage a total of $" 
			  << IC 
			  << " dollars.\n";
	
	std::cout << std::setw(4) 
			  << std::setfill(' ') 
			  << "" 
			  << "Going for a total of " 
			  << NR  
			  << " rounds, waging $" 
			  << IC/NR 
			  << " per round.\n\n";

	std::cout << std::setw(4)
			  << std::setfill(' ')
			  << ""
			  << "Your bet has "
			  << bet.size()
			  << " members. They are: [";
	print(bet.get_spots());
	std::cout << "]" << std::endl;

	
	//Tabela que mostra os coeficientes.
	printTable(payout_table, bet.size());
	int i = 1;
	char iniciar;
	float lucro = 0;
	float total = bet.get_wage();
	

	std::cout << "\n" << std::setw(9) << std::setfill(' ') << "" << "Enter [d] to start the first round.\n";
	
	while(i <= NR){
		if(i > 1){
			std::cout << "\n" << std::setw(9) << std::setfill(' ') << "" << "Enter [d] to start the next round.\n";
		}

		std::cin >> iniciar;

		if(iniciar == 'd'){
			generate_hits(hits);
			set_of_numbers_type match = bet.get_hits(hits);


			lucro = (IC/NR)*payout_table[bet.size()-1][match.size()];
			total += lucro - (IC/NR);

		
			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << std::setw(50)
					  << std::setfill('-')
					  << ""
					  << std::endl;

			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << "This is round #"
					  << i
					  << " of "
					  << NR
					  << ", and your wage is $"
					  << std::fixed
					  << std::setprecision(2)
					  << IC/NR
					  << ". Good luck!\n";

			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << "The hits are: [";
			print(hits);
			std::cout << "]\n\n";

			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << "You hit the following numbers(s) [";
			print(match);

			if(match.size() == 1){
				std::cout << "], a total of "
						  << match.size()
						  << " hit out of "
					  	  << bet.size()
				  		  << std::endl;
			}else{
				std::cout << "], a total of "
						  << match.size()
						  << " hits out of "
					  	  << bet.size()
				  		  << std::endl;
			}

			if(payout_table[bet.size()-1][match.size()] >= 1){
				std::cout << std::setw(9)
						  << std::setfill(' ')
						  << ""
						  << "Payout rate is "
						  << payout_table[bet.size()-1][match.size()]
						  << ", thus you came out with: $"
						  << lucro
						  << std::endl;
			}else{
				std::cout << std::setw(9)
						  << std::setfill(' ')
						  << ""
						  << "Payout rate is "
						  << payout_table[bet.size()-1][match.size()]
						  << ", thus you came out with: $"
						  << IC/NR*payout_table[bet.size()-1][match.size()] - IC/NR
						  << std::endl;	
			}

			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << "Your net balance so far is: $"
					  << total
					  << " dollars."
					  << std::endl;
		
		}else{
			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << "Wrong key pressed. [d] is the correct key. Try again.\n";
			continue;
		}


		i++;
	}

	std::cout << ">>> End of rounds!\n";

	std::cout << std::setw(50)
			  << std::setfill('-')
			  << ""
			  << std::endl
			  << std::endl;

	std::cout << "===== " << "SUMMARY" << " =====" << std::endl;
	
	std::cout << ">>> You spent in this game a total of $"
			  << bet.get_wage()
			  << " dollars.\n";

	if(total - bet.get_wage() >= 0.009){
		std::cout << ">>> Horray, you won $"
				  << std::fixed
				  << std::setprecision(2)
				  << total - bet.get_wage()
				  << " dollars. See you next time! ;-)"
				  << std::endl;
	}else{
		std::cout << ">>> Horray, you lost $"
				  << std::fixed
				  << std::setprecision(2)
				  << std::fabs(total - bet.get_wage())
				  << " dollars. See you next time! ;-)"
				  << std::endl;
	} 

	std::cout << ">>> You are leaving the Keno table with $"
			  << std::fixed
			  << std::setprecision(2)
			  << total
			  << " dollars."
			  << std::endl;

	return 0;
}