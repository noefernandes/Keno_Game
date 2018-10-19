#include "keno.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <chrono>
#include <random>

/*=========================================Funções para validação e leitura========================================*/
/*=================================================================================================================*/
void inputFileValues(std::ifstream & file, std::vector<std::string> & v){
	std::string line;

	while(getline(file, line)){
		if(line == ""){
			continue;
		}
		v.push_back(line);
	}
}


bool check_wage(std::string string){
	if(string == ""){
		return false;
	}

	std::istringstream iss(string);
	std::string var;
	int cont = 0;
	
	while(iss >> var){
		cont++;
	}

	iss.clear();

	if(cont != 1){
		return false;
	}
	
	iss.str(string);
	cash_type value;
	iss >> value;

	if(value < 0){
		return false;
	}

	if(iss.fail() or !iss.eof()){
		iss.clear();
		return false;
	}

	return true;
}

bool check_round(std::string string){
	if(string == ""){
		return false;
	}

	std::istringstream iss(string);
	std::string var;
	int cont = 0;
	
	while(iss >> var){
		cont++;
	}

	iss.clear();

	if(cont != 1){
		return false;
	}

	std::istringstream iss2(string);
	number_type value;
	iss2 >> value;

	if(value < 0){
		return false;
	}

	if(iss2.fail() or !iss2.eof()){
		iss2.clear();
		return false;
	}

	return true;
}

bool check_spots(std::string string){
	if(string == ""){
		return false;
	}

	std::istringstream iss(string);
	number_type var;
	while(iss >> var){
		if(var < 1 or var > 80){
			iss.clear();
			return false;
		}
	}

	if(!iss.eof()){
		iss.clear();
		return false;
	}

	return true;
}



/*=========================================Organizando os valores da aposta======================================*/
/*===============================================================================================================*/

void sort(set_of_numbers_type & v){
	size_t indiceMenor;

	for(size_t i = 0; i < v.size(); i++){
		indiceMenor = i;
		for(size_t j = i + 1; j < 6; j++){
			if(v[j] < v[indiceMenor]){
				indiceMenor = j;			
			}	 
		}

		std::swap(v[i], v[indiceMenor]);
	}
}



void generate_hits(set_of_numbers_type & v){
	std::array <number_type,80> foo;

  	for(size_t i = 0; i < foo.size(); i++){
  		foo[i] = i+1;
  	}

  	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  	shuffle (foo.begin(), foo.end(), std::default_random_engine(seed));

  	for(size_t i = 0; i < 20; i++){
  		v[i] = foo[i];
  	}

  	sort(v.begin(), v.end());
}

number_type countFromArq(const std::string string){
	std::istringstream iss(string);
	number_type value;
	number_type cont = 0;
	while(iss >> value){
		cont++;
	}

	iss.clear();

	return cont;
}

void copyFromArq(const std::string string, set_of_numbers_type & v){
	std::istringstream iss(string);
	number_type value;
	int cont = 0;
	while(iss >> value){
		v[cont] = value;
		cont++;
	}	

	iss.clear();
}

set_of_numbers_type unique_ordered_values(set_of_numbers_type & repeated){
	sort(repeated.begin(), repeated.end());
	set_of_numbers_type::iterator it;
	it = std::unique(repeated.begin(), repeated.end());
	repeated.resize(std::distance(repeated.begin(), it));

	return repeated;
}

void createMatrix(std::vector<std::string> & string_table, std::vector<std::vector<cash_type>> & payout_table){
	std::istringstream iss;
	cash_type value;

	for(size_t i = 0; i < 15; i++){
		iss.str(string_table[i]);
		for(size_t j = 0; j < 16; j++){
			iss >> value;
			payout_table[i][j] = value; 
		}

		iss.clear();
	}
}


/*==============================================Métodos das classes==================================================*/
/*===================================================================================================================*/
bool KenoBet::add_number (number_type spot_){
    set_of_numbers_type::iterator it = std::find((this->m_spots).begin(), (this->m_spots).end(), spot_);
    if(it == (this->m_spots).end()){
        (this->m_spots).push_back(spot_);
        sort((this->m_spots).begin(), (this->m_spots).end());
        return true;
    }

    return false;
}

bool KenoBet::set_wage(cash_type wage_){
	if(wage_ > 0){
		this->m_wage = wage_;
		return true;
	}

	return false;
}

void KenoBet::reset(void){
	(this->m_spots).resize(0);
}

cash_type KenoBet::get_wage (void) const{
	return this->m_wage;
}

size_t KenoBet::size(void) const{
	return (this->m_spots).size();
}

set_of_numbers_type KenoBet::get_hits(const set_of_numbers_type & hits_) const{
  	set_of_numbers_type v(20);                      
  	set_of_numbers_type::const_iterator it;

  	it = std::set_intersection (hits_.begin(), hits_.end(), m_spots.begin(), m_spots.end(), v.begin());
                                               
  	v.resize(it - v.begin());                      

  	return v;
}

set_of_numbers_type KenoBet::get_spots (void ) const{
	return m_spots;
}


/*=============================================================================================================*/
void print(const set_of_numbers_type & v){
	std::cout << " ";
	for(size_t i = 0; i < v.size(); i++){
		std::cout << v[i] << " ";
	}
}

void printTable(const std::vector<std::vector<cash_type>> & v, size_t size){
	//Primeira linha.
	std::cout << std::setw(9)
			  << std::setfill(' ')
			  << ""
			  << std::setw(7)
			  << std::setfill('-')
			  << ""
			  << "+"
			  << std::setw(9)
			  << std::setfill('-')
			  << "";
			  
	//Segunda linha.
	std::cout << "\n"
			  << std::setw(9)
			  << std::setfill(' ')
			  << ""
			  << "Hits"
			  << std::setw(3)
			  << std::setfill(' ')
			  << ""
			  << "|"
			  << std::setw(1)
			  << std::setfill(' ')
			  << ""
			  << "Payout"
			  << std::setw(2)
			  << std::setfill(' ')
			  << ""
			  << "\n";

	std::cout << std::setw(9)
			  << std::setfill(' ')
			  << ""
			  << std::setw(7)
			  << std::setfill('-')
			  << ""
			  << "+"
			  << std::setw(9)
			  << std::setfill('-')
			  << ""
			  << "\n";

	for(size_t i = 0; i <= size; i++){
		if(i <= 9){
			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << i
					  << std::setw(6)
					  << std::setfill(' ')
					  << ""
					  << "|"
					  << std::setw(1)
					  << std::setfill(' ')
					  << ""
					  << v[size - 1][i]
					  << std::setw(7)
					  << std::setfill(' ')
					  << "";
			std::cout << "\n";
		}else{
			std::cout << std::setw(9)
					  << std::setfill(' ')
					  << ""
					  << i
					  << std::setw(5)
					  << std::setfill(' ')
					  << ""
					  << "|"
					  << std::setw(1)
					  << std::setfill(' ')
					  << ""
					  << v[size - 1][i]
					  << std::setw(7)
					  << std::setfill(' ')
					  << "";
			std::cout << "\n";
		}
	}

}