/*
Effect.hlsl

//  Vertex에 대한 구조체를 define 해 놓음.
//  Vertex Input typedefline
//  float4 : x,y,z,w(동차)를 가지는 shader에서 사용되는 변수 
//  POSITION, COLOR : GPU에 이 변수를 사용한다. 라고 정의를 해주는 것(SemanticName)
*/
struct VertexInput
{
    float4 Position : POSITION0; //시멘틱 네임. GPU한테 이 변수가 POSITION0으로 쓰인다고 알려 줌
    // 0 ~ 16까지 있음.
//   float4 Position2 : POSITION1;   
    float4 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; // SV? 좀 더 중요한 의미...next
    float4 Color : COLOR0;
};
/*
vertex Shader 출력 프로그램
- 1.0f ~ 1.0f 사이 계산
이 계산을 화면출력위해선, 변환작업 필요.
*/
PixelInput VS(VertexInput input)
{
    PixelInput output;
    //next: Matrix변환 
    output = input;
//    output.Position = input.Position;
//    output.Color = input.Color;
    return output;
}
/*
Pixel Shader 출력 프로그램
*/
float4 PS(PixelInput input) : SV_Target0
{
    return input.Color;
}