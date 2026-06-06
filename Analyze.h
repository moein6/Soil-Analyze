#pragma once

//	this class is going to analyze the data system -> astm , aashto , bscs , ...

#include "Sieve.h"
 

class Analyze : public Sieve
{
public:
	Analyze();

	System AnalyzeData(const SieveVector&);

	void Print(const System&);


private:
	
	bool Analyze_ASTM(const SieveVector&);
	
	bool Analyze_BSCS(const SieveVector&);

	bool Analyze_MIT(const SieveVector&);
	
	bool Analyze_AASHTO(const SieveVector&);

private:

	bool Detected;
	double m_pASTMno, m_pBSCSno , m_pASTMsize, m_pBSCSsize;

};



