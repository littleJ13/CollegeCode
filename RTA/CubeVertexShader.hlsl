#pragma pack_matrix(row_major)
// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model[100];
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 uv : UV;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos.xyz, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, model[0]);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	// Pass the color through without modification.
	output.uv = float4(input.pos.xyz, 1.0f);

	return output;
}
