////////////////////////////////////////////////////
//  Color.hlsl
//  HLSL : High Level Shader Lanange
//         --> 과거에는 Assember
//         --> C하고 유사
//  * 사용되는 function명은 동일이름으로 쉐이더생성(프로그램)
//     해주어야 한다
//  * 전역변수 : Matrix등
///////////////////////////////////////////////////
// 정점
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
// 상수Buffer를 받을 세이더 작성
cbuffer Transformbuffer : register(b0) // 0~13 슬롯에 등록 (4096*16바이트 이하 16배수)
{
	matrix world;     // float4x4 나자신에 대한 위치,스케일, 회전
	matrix view;      // 눈이 보는 방향에 대한
	matrix proj;      // Projectetor를 생각하자  직교사영
}
cbuffer ColorBuffer : register(b1)
{
	float4 TextColor;  // 바뀔 Color값
	int    Path;       // switch문 사용시의 Tag
	float  Time;
	float  dummy1;
	float  dummy2;
}


Texture2D sourc_texture1 : register(t0); // 테스쳐자원이라는 의미 : 128개
Texture2D sourc_texture2 : register(t1); // 테스쳐자원이라는 의미

SamplerState samp : register(s0);  //샘플링을 할 상태 정보 0-15


//////////////////////////////////////////////////////////
// Vertex  프로그램
// multiply --> mul (곱셈)
//////////////////////////////////////////////////////////
PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.position = mul(input.position, world); // 월드변환
	output.position = mul(output.position, view); // 뷰변환
	output.position = mul(output.position, proj); // 프로젝션변환
	output.uv = input.uv;
	return output;
}
//////////////////////////////////////////////////////////
// Pixel  프로그램
//////////////////////////////////////////////////////////
float4 PS(PixelInput input) : SV_Target // 현재세팅된 메인렌더 타깃을 사용하라고 알려준다
{
	float4 color = sourc_texture1.Sample(samp, input.uv);
	float4 returnColor = color;
	float4 alpha  = float4(color.rgb, 0);
	float4 alpha2 = color;
	
	[forcecase]
	
	switch (Path)
	{
		case  0:
			returnColor = color;
			break;
		case  1:
			returnColor = float4(TextColor.r, TextColor.g, TextColor.b, color.a);
			break;
		case  2:
		    // 선형보간 :  return start + ( end - start ) * amount;
            returnColor = lerp(color, alpha, Time); // 기존 Alpha ~ 0  -1 ~ 1 까지
            break;
        case 3:
            if (input.position.y < dummy1)	// 입력받은 Pixel Input의 y position 이 (min) 보다 작으면 무시
                discard;
            if (input.position.y > dummy2)	// 입력받은 Pixel Input의 y position 이 (max) 보다 크면 무시
                discard;
            break;
        case 4:
            if (color.a < 0.1f)	// 투명한 픽셀을 무시해버린다.
				discard;
			returnColor = float4(TextColor.r, TextColor.g, TextColor.b, TextColor.a);
			break;
		case 5:
		    // 선형보간 :  return start + ( end - start ) * amount;
			alpha = float4(TextColor.rgb, alpha2.a);
		
			returnColor = lerp(alpha, alpha2, Time); // 기존 Alpha ~ 0 
			break;
		
	}
	return returnColor;
}