#include "shader_common.hlsli"


#define DefaultRS "RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT ) " 


[RootSignature(DefaultRS)]
PSInput main(float4 position : POSITION)
{
	PSInput input;
	input.position = position;
	return input;
}