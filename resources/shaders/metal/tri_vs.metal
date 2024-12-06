float3 pos[[attribute(0)]];

float4 vs_main(){
  return float4(pos.x, pos.y, pos.z, 1.0f)
}
