/*
Effect.hlsl

//  Vertex�� ���� ����ü�� define �� ����.
//  Vertex Input typedefline
//  float4 : x,y,z,w(����)�� ������ shader���� ���Ǵ� ���� 
//  POSITION, COLOR : GPU�� �� ������ ����Ѵ�. ��� ���Ǹ� ���ִ� ��(SemanticName)
*/
struct VertexInput
{
    float4 Position : POSITION0; //�ø�ƽ ����. GPU���� �� ������ POSITION0���� ���δٰ� �˷� ��
    // 0 ~ 16���� ����.
//   float4 Position2 : POSITION1;   
    float4 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; // SV? �� �� �߿��� �ǹ�...next
    float4 Color : COLOR0;
};
/*
vertex Shader ��� ���α׷�
- 1.0f ~ 1.0f ���� ���
�� ����� ȭ��������ؼ�, ��ȯ�۾� �ʿ�.
*/
PixelInput VS(VertexInput input)
{
    PixelInput output;
    //next: Matrix��ȯ 
    output = input;
//    output.Position = input.Position;
//    output.Color = input.Color;
    return output;
}
/*
Pixel Shader ��� ���α׷�
*/
float4 PS(PixelInput input) : SV_Target0
{
    return input.Color;
}