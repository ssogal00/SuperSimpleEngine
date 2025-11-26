// 스레드 그룹 당 스레드 수 (1000개를 커버하기 위해 1024 사용)
#define BLOCK_SIZE 1024

// 입력: 읽기 전용 구조체 버퍼
StructuredBuffer<float3> InputBuffer : register(t0);

// 출력: 쓰기 가능한 구조체 버퍼 (결과값 1개만 저장)
RWStructuredBuffer<float3> OutputBuffer : register(u0);

// 그룹 내 스레드들이 공유하는 메모리 (고속)
groupshared float3 sharedMem[BLOCK_SIZE];

[numthreads(BLOCK_SIZE, 1, 1)]
void CSMain(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID)
{
    // 1. 글로벌 메모리에서 공유 메모리로 데이터 로드
    // 데이터가 1000개이므로 인덱스가 넘어가는 경우 0으로 초기화
    if (DTid.x < 1000)
    {
        sharedMem[GTid.x] = InputBuffer[DTid.x];
    }
    else
    {
        sharedMem[GTid.x] = float3(0, 0, 0);
    }

    // 모든 스레드가 로드를 마칠 때까지 대기
    GroupMemoryBarrierWithGroupSync();

    // 2. 병렬 리덕션 (Parallel Reduction) 수행
    // 절반씩 범위를 줄여가며 합산 (1024 -> 512 -> 256 ... -> 1)
    [unroll]
    for (uint s = BLOCK_SIZE / 2; s > 0; s >>= 1)
    {
        if (GTid.x < s)
        {
            sharedMem[GTid.x] += sharedMem[GTid.x + s];
        }
        // 각 단계마다 동기화 필요
        GroupMemoryBarrierWithGroupSync();
    }

    // 3. 최종 결과 쓰기
    // 0번 스레드가 최종 합계 값을 가지고 있음
    if (GTid.x == 0)
    {
        OutputBuffer[0] = sharedMem[0];
    }
}