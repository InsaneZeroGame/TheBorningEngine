#pragma once
#include "TbeWindow.h"
#include "TbeModule.h"

namespace TBE
{
	class Renderer : public Module
	{
	public:
		Renderer();
		virtual ~Renderer();
		virtual void InitGpuResource() {};
		virtual void InitFrameDependentResource() {};
		virtual void SetTargetWindow(const BaseWindow* p_window);
	
		// Inherited via Module
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnDestory() override;

	protected:
		const BaseWindow* m_targetWindow;

		
	};
}