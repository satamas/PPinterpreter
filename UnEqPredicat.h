/*
 * UnEqPredicat.h
 *
 *  Created on: 04.01.2014
 *      Author: atamas
 */

#ifndef UNEQPREDICAT_H_
#define UNEQPREDICAT_H_


template <typename T>
struct UnEqPredicat{
	UnEqPredicat(T const charToCompare){
		a = charToCompare;
	}
	bool operator ()(T const b) const{
		return (a == b);
	}
private:
	T a;
};

#endif /* UNEQPREDICAT_H_ */
