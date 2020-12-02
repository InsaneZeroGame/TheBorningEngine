#pragma once

namespace TBE
{
	class NoCopy
	{
	public:
		NoCopy() {};
		virtual ~NoCopy() {};
		NoCopy(const NoCopy&) = delete;
		NoCopy(const NoCopy&&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;

	private:

	};


	class Module : NoCopy
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