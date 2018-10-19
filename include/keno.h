/*! \file keno.h
	\author Noé Fernandes
	\date October, 9th
*/
#ifndef KENO_H
#define KENO_H

#include <vector>
#include <fstream>
#include <iostream>

using number_type = unsigned short int ; //<! data type for a keno hit .
using cash_type = float; //<! Defines the wage type in this application .
using set_of_numbers_type = std::vector <number_type>;

class KenoBet {
	public :
		//! Creates an empty Keno bet .
		KenoBet(): m_wage (0)
		{ /* empty */ };
		/*! Adds a number to the spots only if the number is not already there .
		@param spot_ The number we wish to include in the bet .
		@return T if number chosen is successfully inserted ; F otherwise . */
		bool add_number (number_type spot_);
		/*! Sets the amount of money the player is betting .
		@param wage_ The wage .
		@return True if we have a wage above zero ; false otherwise . */
		bool set_wage (cash_type wage_);
		//! Resets a bet to an empty state .
		void reset(void);
		/*! Retrieves the player’s wage on this bet .
		@return The wage value . */
		cash_type get_wage (void) const ;
		/*! Returns to the current number of spots in the player’s bet .
		@return Number of spots present in the bet . */
		size_t size (void) const ;
		/*! Determine how many spots match the hits passed as argument .
		@param hits_ List of hits randomly chosen by the computer .
		@return An vector with the list of hits . */
		set_of_numbers_type get_hits (const set_of_numbers_type & hits_) const;
		/*! Return a vector < spot_type > with the spots the player has picked so far .
		@return The vector < spot_type > with the player’s spots picked so far . */
		set_of_numbers_type get_spots (void ) const;

	private :
		set_of_numbers_type m_spots;
		cash_type m_wage;
		//<! The player’s bet .
		//<! The player’s wage .
};

/*! Stores each line of the file in a vector of strings .
@param file The file .
@param v Vector .*/
void inputFileValues(std::ifstream & file, std::vector<std::string> & v);
/*! Checks if the string representing the money to be invested in the file is valid .
@param string String to be checked .
@return True, if is valid ; false otherwise . */
bool check_wage(const std::string string);
/*! Checks if the string representing the number of rounds in the file is valid .
@param string String to be checked .
@return True, if is valid ; false otherwise . */
bool check_round(const std::string string);
/*! Check if the string representing the spots numbers in the file is valid .
@param string String to be checked .
@return True, if is valid ; false otherwise . */
bool check_spots(const std::string string);
/*! Sort a vector .
@param v Vector .
*/
void sort(set_of_numbers_type & v);
/*! It raffles the winning values .
@param v Vector .*/
void generate_hits(set_of_numbers_type & v);
/*! Copies string values to a vector.
@param string String .
@param v Vector of number type .*/
void copyFromArq(const std::string string, set_of_numbers_type & v);
/*! Count the number of elements in the string .
@param string String .
@return Number of elements in the string .*/ 
number_type countFromArq(const std::string string);
/*! Makes unique and sorts the values of a vector .
@param repeated Vector .
@return Ordered vector with unique values .*/
set_of_numbers_type unique_ordered_values(set_of_numbers_type & repeated);
/*! Transforms the vector of strings to float array .
@param string_table Vector .
@param payout_table Matrix .
*/ 
void createMatrix(std::vector<std::string> & string_table, std::vector<std::vector<cash_type>> & payout_table);
/*! Prints the vector on the screen .
@param v Vector to be printed .*/
void print(const set_of_numbers_type & v);
/*! Prints the table with the coefficients on the screen .
@param v Float Matrix .
@param size Size of the bet vector stored in the object .*/
void printTable(const std::vector<std::vector<cash_type>> & v, size_t size);


#endif