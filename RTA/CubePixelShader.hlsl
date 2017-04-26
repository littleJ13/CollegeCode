textureCUBE model : register(t0);

SamplerState modelFilter : register(s0);

float4 main(float4 pos : SV_POSITION, float4 uv : UV) : SV_TARGET
{
	return model.Sample(modelFilter, uv);
}
