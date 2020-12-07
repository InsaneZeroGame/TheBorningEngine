#include "shader_common.hlsli"


float4 main(PSInput input) : SV_TARGET
{
	return input.position;
}