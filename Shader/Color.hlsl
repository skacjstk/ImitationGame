/*
Color.hlsl

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
// Effect.hlsl ��� �߰��� ���� 
//Matrix : ��� Buffer �� ���� ������ ����
cbuffer Transformbuffer : register(b0) //0~13 ������ ���� (4096*13)
{
    matrix world; // float4   �ڱ� �ڽ�
    matrix view; // camera :  ���� ���� ���⿡ ����
    matrix proj; // �����翵 : projector -> ����� ��ǥ 
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
PixelInput VS(VertexInput input)        //ColorShader.cpp ���� CreateShader �κп� �ø�ƽ
{
    PixelInput output;
    
    // Matrix��ȯ         // mul�� hlsl ���� �Լ�
    output.Position = mul(input.Position, world); // ���� ��ȯ
    output.Position = mul(output.Position, view); // �� ��ȯ
    output.Position = mul(output.Position, proj); // �����翵 ��ȯ     
    output.Color = input.Color;
    return output; // �� Output �����Ͱ� float4 PS�� 
}
/*
Pixel Shader ��� ���α׷�
*/
float4 PS(PixelInput input) : SV_Target0
{
    return input.Color;
}
