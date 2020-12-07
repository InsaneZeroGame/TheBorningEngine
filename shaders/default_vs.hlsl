#include "shader_common.hlsli"

PSInput main(float4 position : POSITION)
{
	PSInput input;
	input.position = position;
	return input;
}