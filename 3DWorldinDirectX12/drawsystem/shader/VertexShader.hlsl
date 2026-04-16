
struct InputVertexData
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct OutputVertexData
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

OutputVertexData main(InputVertexData Input)
{
    OutputVertexData Output;
    Output.pos = float4(Input.pos, 1.0f);
    Output.color = Input.color;
    return Output;
}

float4 ps(OutputVertexData input):SV_Target
{
    return input.color;
}