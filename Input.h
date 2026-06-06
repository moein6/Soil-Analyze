#pragma once

#include "Sieve.h"
#include <optional>
#include <limits>

class Input
{
public:
	Input();


	bool Run();

	System get_system()const noexcept;

	std::shared_ptr<SieveVector> Return_Pointer() const;

private:
	void Reset_cin()const;
	bool SelectSystem();
	bool CollectUserInput();
	bool ReadValueFor(Sieve&);
	std::optional<SieveVector> InitializeData();

private:
	SieveVector m_data;
	System m_system;

};
