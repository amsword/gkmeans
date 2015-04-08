#pragma once


template <class T1, class T2, class T3>
class Triplet
{
public:
	T1 first;
	T2 second;
	T3 third;

public:
	Triplet()
	{
	}

	Triplet(T1 t1, T2 t2, T3 t3) : first(t1), second(t2), third(t3)
	{
	}
};

template <class T1, class T2, class T3>
class GreatTripletSecond
{
public:
	bool operator ()(const Triplet<T1, T2, T3> &t1, 
		const Triplet<T1, T2, T3> &t2)
	{
		return t1.second > t2.second;
	}
};

template <class T1, class T2, class T3>
class GreatTripletThird
{
public:
	bool operator () (const Triplet<T1, T2, T3> &t1, 
		const Triplet<T1, T2, T3> &t2)
	{
		return t1.third > t2.third;
	}
};

template <class T1, class T2, class T3>
class LessTripletThird
{
public:
	bool operator () (const Triplet<T1, T2, T3> &t1, 
		const Triplet<T1, T2, T3> &t2)
	{
		return t1.third < t2.third;
	}
};
