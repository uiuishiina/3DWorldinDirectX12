
struct InputVertexData
{
    float3 pos : POSITION;
};

struct OutputVertexData
{
    float4 pos : SV_POSITION;
};

OutputVertexData main(InputVertexData Input)
{
    OutputVertexData Output;
    Output.pos = float4(Input.pos, 1.0f);
    return Output;
}