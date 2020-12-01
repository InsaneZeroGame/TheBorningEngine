#pragma once

namespace TBE
{
	class Module
	{
	public:
		Module() {};
		virtual ~Module() {};

		virtual void OnInit() = 0;

		virtual void OnUpdate() = 0;

		virtual void OnDestory() = 0;

	private:

	};
}