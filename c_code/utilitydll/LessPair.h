#pragma once

#include <iostream>

using namespace std;

template <class T>
class LessPair
{
public:
	int index;
	T distance;
	friend bool operator < (const LessPair<T> &n1, const LessPair<T> &n2)
	{
		if (n1.distance == n2.distance) return n1.index < n2.index;
		return n1.distance < n2.distance;
	}
	friend ostream& operator << (ostream &os, const LessPair<T> &p)
	{
		os << p.index << "\t" << p.distance << "\t";
		return os;
	}

	LessPair() {}
	LessPair(int idx, T dist) {index = idx; distance = dist;}
};


template <class T>
class GreatPair
{
public:
	int index;
	T distance;
	friend bool operator < (const GreatPair<T> &n1, const GreatPair<T> &n2)
	{
		if (n1.distance == n2.distance) return n1.index < n2.index;
		return n1.distance > n2.distance;
	}
	friend ostream& operator << (ostream &os, const GreatPair<T> &p)
	{
		os << p.index << "\t" << p.distance << "\t";
		return os;
	}

	GreatPair() {}
	GreatPair(int idx, T dist) {index = idx; distance = dist;}
};



