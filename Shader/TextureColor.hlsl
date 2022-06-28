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
	float4 TextColor;  // �ٲ� Color��
	int    Path;       // switch�� ������ Tag
	float  Time;
	float  dummy1;
	float  dummy2;
}


Texture2D sourc_texture[2] : register(t0); // �׽����ڿ��̶�� �ǹ� : 128��
Texture2D sourc_texture2 : register(t1); // �׽����ڿ��̶�� �ǹ�

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
    float4 color = sourc_texture[0].Sample(samp, input.uv); // ���� �ؽ����� �ȼ���.
	float4 color2 = sourc_texture[1].Sample(samp, input.uv);
	float4 returnColor = color;
	float4 alpha  = float4(color.rgb, 0);
	float4 alpha2 = color;
	// TextColor �� ���� memcpy �ذ��� �޸𸮿� �������� �ȼ���.
	[forcecase]
	
	switch (Path)
	{
		case  0:
			returnColor = color;
			break;
		case  1:
			if(color.a < 0.1f)
                discard;
			returnColor = float4(TextColor.r, TextColor.g, TextColor.b, color.a);
			break;
		case  2:
		    // �������� :  return start + ( end - start ) * amount;
            returnColor = lerp(color, alpha, Time); // ���� Alpha ~ 0  -1 ~ 1 ����
            break;
        case 3:
            if (input.position.y < dummy1)	// �Է¹��� Pixel Input�� y position �� (min) ���� ������ ����
                discard;
            if (input.position.y > dummy2)	// �Է¹��� Pixel Input�� y position �� (max) ���� ũ�� ����
                discard;
            break;
        case 4:
            if (color.a < 0.1f)	// ������ �ȼ��� �����ع�����.
				discard;
			returnColor = float4(TextColor.r, TextColor.g, TextColor.b, TextColor.a);
			break;
		case 5:
		    // �������� :  return start + ( end - start ) * amount;
			alpha = float4(TextColor.rgb, alpha2.a);
		
			returnColor = lerp(alpha, alpha2, Time); // ���� Alpha ~ 0 
			break;
		case 10:	// ��Ƽ �ؽ���
			returnColor = color2 * 0.5 + color * 0.5;
			returnColor = saturate(returnColor);
			returnColor.a = color.a;
			break;
		case 11:	// ���İ��� ����Ǵ� ���̴� ( texture ���� �� �ִ� TextColor�� ���� ���Ѵ� )
			if(color.a < 0.1f)
                discard;
            returnColor.a = TextColor.a;
            break;
    }
	return returnColor;
}