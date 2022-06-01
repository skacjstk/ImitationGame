////////////////////////////////////////////////////
//  Color.hlsl
//  HLSL : High Level Shader Lanange
//         --> ���ſ��� Assember
//         --> C�ϰ� ����
//  * ���Ǵ� function���� �����̸����� ���̴�����(���α׷�)
//     ���־�� �Ѵ�
//  * �������� : Matrix��
///////////////////////////////////////////////////
// ����
struct VertexInput
{
	float4 position : POSITION0; // Semantic Name
	float2 uv       : TEXCOORD0; // Texture coordinate
};
struct PixelInput
{
	float4 position : SV_POSITION0; // System  Value
	float2 uv       : TEXCOORD0;
};
// ���Buffer�� ���� ���̴� �ۼ�
cbuffer Transformbuffer : register(b0) // 0~13 ���Կ� ��� (4096*16����Ʈ ���� 16���)
{
	matrix world;     // float4x4 ���ڽſ� ���� ��ġ,������, ȸ��
	matrix view;      // ���� ���� ���⿡ ����
	matrix proj;      // Projectetor�� ��������  �����翵
}
cbuffer ColorBuffer : register(b1)
{
    float4 TextColor; // �ٲ� Color��
    int Path; // switch�� ������ Tag
    float Time;
    float dummy1;
    float dummy2;
}
Texture2D sourc_texture1[2] : register(t0); // �׽����ڿ��̶�� �ǹ� : 128��
Texture2D sourc_texture2[2] : register(t1); // �׽����ڿ��̶�� �ǹ�

SamplerState samp : register(s0);  //���ø��� �� ���� ���� 0-15


//////////////////////////////////////////////////////////
// Vertex  ���α׷�
// multiply --> mul (����)
//////////////////////////////////////////////////////////
PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.position = mul(input.position, world); // ���庯ȯ
	output.position = mul(output.position, view); // �亯ȯ
	output.position = mul(output.position, proj); // �������Ǻ�ȯ
	output.uv = input.uv;
	return output;
}
//////////////////////////////////////////////////////////
// Pixel  ���α׷�
//////////////////////////////////////////////////////////
float4 PS(PixelInput input) : SV_Target // ���缼�õ� ���η��� Ÿ���� ����϶�� �˷��ش�
{
	float4 color = sourc_texture1[0].Sample(samp, input.uv);
	
	return color;
}