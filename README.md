# GDIProject

> C++과 GDI+만으로 만든 헥사곤 턴제 전략 게임

![gameplay](https://github.com/user-attachments/assets/8bcd35a3-23e3-47fb-8e72-7e7d5b41a01e)

---

## 스크린샷

| 게임 시작 | 인게임 | 인게임 | 게임오버 |
|:---:|:---:|:---:|:---:|
| <img src="https://github.com/user-attachments/assets/8f814996-cd5d-432c-9200-ace061474116" width="420"/> | <img src="https://github.com/user-attachments/assets/f8e2e782-ef65-4e05-b5e5-fcce68dab260" width="420"/> | <img src="https://github.com/user-attachments/assets/4d651d76-eb2a-47fb-a0d4-99f4370f8c0b" width="420"/> | <img src="https://github.com/user-attachments/assets/45d409b8-0ace-4bb5-9afe-e3f5a3a288db" width="420"/> |

---

## 소개

외부 라이브러리나 상용 엔진 없이 **C++과 GDI+만으로** 게임 구조를 처음부터 설계했습니다.

헥사곤 타일 맵 위에서 유닛을 생산하고, BFS로 이동 가능 범위를 계산하며, AI와 턴을 교대하는 문명류 전략 게임입니다.
렌더러·입력·씬 전환 등 엔진 계층을 직접 구현하며 게임 프레임워크 구조를 학습하는 것이 목표였습니다.

---

## 주요 기능

- **헥사곤 타일 맵** — 10×15 그리드, 6가지 지형 (사막 / 초원 / 구릉 / 평원 / 산 / 수도)
- **유닛 3종** — 정착자(Settler) / 전사(Warrior) / 궁수(Archer), 지형별 이동 비용 반영
- **BFS 이동 범위** — 선택한 유닛의 이동 가능 타일을 실시간 하이라이트
- **도시 생산** — 도시(City)에서 유닛을 소환해 전장에 투입
- **AI 자동 행동** — 정착자는 좋은 지형을 탐색, 전투 유닛은 가장 가까운 적을 향해 이동·공격
- **카메라 줌** — 마우스 휠로 0.3×~4.0× 자유롭게 조절
- **승패 판정** — 30턴 내 적 섬멸 시 승리, 초과 시 패배

---

## 조작법

| 입력 | 동작 |
|---|---|
| 좌클릭 (유닛 위) | 유닛 선택 (이동 가능 범위 하이라이트) |
| 좌클릭 (타일 위) | 선택한 유닛을 해당 타일로 이동 |
| 마우스 휠 위/아래 | 카메라 줌 인 / 줌 아웃 |
| 유닛 생산 버튼 | 정착자 · 전사 · 궁수 소환 |
| 턴 종료 버튼 | 플레이어 턴 종료 → AI 자동 행동 시작 |
| 턴 스킵 버튼 | 선택 유닛의 이번 턴 행동 건너뜀 |

---

## 빌드 방법

```
요구 환경: Visual Studio 2022, Windows 10/11 x64
```

1. `GDIProject.sln` 을 Visual Studio 2022로 열기
2. 솔루션 구성을 `Release` / `x64` 로 설정
3. **Build → Build Solution** (단축키 `Ctrl+Shift+B`)
4. 빌드 완료 후 `x64/Release/Games.exe` 실행

> GDI+는 Windows에 기본 내장되어 있어 별도 설치가 필요 없습니다.

---

## 기술 스택

| 항목 | 내용 |
|---|---|
| 언어 | C++ |
| 렌더링 | GDI+ (Gdiplus) |
| 플랫폼 | Windows (Win32 API) |
| 빌드 | Visual Studio 2022, MSBuild |
| 외부 라이브러리 | 없음 |

---

## 프로젝트 구조

```
GDIProject/
├── Engine/           # 공통 엔진 계층
│   ├── Runtime/
│   │   ├── Renderer/     # GDI+ 렌더러 (이미지·텍스트·UI·애니메이션)
│   │   └── Animation/    # 스프라이트 시트 기반 프레임 애니메이션
│   ├── Classes/
│   │   ├── Object / Actor / Character / Pawn  # 오브젝트 계층
│   │   ├── Scene/        # 씬 베이스 클래스
│   │   └── Components/   # UI, 텍스트, 버튼 컴포넌트
│   ├── Input/            # Win32 키·마우스 입력
│   ├── Math/             # FVector2 등 수학 유틸리티
│   └── Experiment/       # QuadTree, AABB 충돌, SmartCast
│
└── Games/            # 게임 콘텐츠 계층
    ├── Scene/            # MenuScene / PlayScene / EndScene / WinScene
    ├── Character/        # PlayerCharacter, EnemyCharacter
    ├── Tile/             # ATile (헥사곤 좌표, BFS), ACity
    ├── Player/           # APlayerController, AAIPlayer, TurnGameState
    ├── Manager/          # TurnManager (플레이어↔AI 턴 교대)
    └── UI/               # PlayScene_Widget (버튼·텍스트 UI)
```

---

## 기술적 이슈와 해결 과정

**1. 헥사곤 좌표 변환 오류**

짝수 행과 홀수 행의 이웃 타일 오프셋이 달라, 인접 타일 탐색 로직에서 잘못된 좌표를 계산하는 문제가 있었습니다.
`even_dy/dx`, `odd_dy/dx` 방향 배열을 분리 선언하고, 픽셀↔인덱스 변환을 `ATile`의 정적 함수로 묶어 한 곳에서만 관리하도록 했습니다.
변환 로직이 분산되어 있을 때는 버그 수정 시 여러 파일을 고쳐야 했지만, 통합 후에는 한 함수만 수정하면 됐습니다.

**2. AI 턴 중 플레이어 입력 처리 혼재**

AI가 유닛을 자동으로 행동시키는 동안 플레이어가 마우스를 클릭하면 의도치 않은 유닛이 선택됐습니다.
`TurnManager`에 `ETurnState::PlayerTurn / AITurn` 열거형 상태를 두고, AI 턴일 때는 `PlayerController::HandleInput()`을 호출하지 않도록 분기를 명확히 했습니다.

**3. 전체 타일 순회로 인한 이동 범위 계산 부담**

유닛을 선택할 때마다 전체 타일을 순회하며 이동 가능 여부를 판단하면, 맵이 커질수록 연산량이 늘었습니다.
BFS 기반 `ATile::GetReachableTiles()`로 교체해, 지형별 이동 비용(`GetTileMoveCost`)을 누적하며 도달 가능한 타일만 탐색하도록 했습니다.

**4. 렌더러와 게임 로직 혼재**

초기에는 렌더링 코드와 게임 로직이 같은 파일에 섞여 있어, 한쪽을 수정하면 다른 쪽까지 영향이 퍼졌습니다.
Engine 프로젝트(렌더러·입력·수학·시간)와 Games 프로젝트(씬·유닛·타일·UI)를 별도 모듈로 분리했습니다.
`NewObject<T>()` / `Cast<T>()`로 오브젝트 생성과 다운캐스트를 Engine 계층에서 통일해, Games 계층은 내부 구현 없이 동일한 방법을 사용합니다.
