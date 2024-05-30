#pragma once

class Brain
{
public:
	Brain();
	virtual ~Brain();

	void Init();
	void Run();

protected:
	shared_ptr<class BTSequence> root;
};
