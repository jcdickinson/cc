#pragma once
#include <stdint.h>

namespace fx
{
	class IRenderOperation
	{
	public:
		virtual ~IRenderOperation() {}
		virtual void Render() = 0;
	};

	class CommandBuffer
	{
	public:
		enum { MaxOperations = 32768 };

	private:
		IRenderOperation* _operations[];
	};
}