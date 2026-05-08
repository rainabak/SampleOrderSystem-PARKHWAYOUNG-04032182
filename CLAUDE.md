# CLAUDE.md — SampleOrderSystem 작업 규칙

Claude가 이 프로젝트에서 코드를 작성하거나 수정할 때 반드시 따라야 하는 규칙입니다.

---

## Claude 작업 규칙

### 기본 원칙

- 요청받은 범위만 구현한다. 요청하지 않은 리팩터링, 기능 추가, 파일 생성을 하지 않는다.
- 코드 작성 전 반드시 관련 헤더 파일을 먼저 읽어 기존 설계를 파악한다.
- 변경 전 현재 파일 상태를 확인하지 않고 Edit/Write를 사용하지 않는다.
- 구현 단계가 지정된 경우(Commit 계획 등) 해당 순서를 지킨다.
- 설계 문서(`PRD.md`, `README.md`)와 충돌하는 구현을 진행하지 않는다. 충돌 시 사용자에게 먼저 확인한다.

### 언어 및 스타일

- C++17 표준만 사용한다. C++20 이상 기능 사용 금지.
- 외부 라이브러리 추가 금지. 표준 라이브러리만 사용.
- 주석은 WHY가 명확히 필요한 경우에만 한 줄 작성한다. WHAT 설명 주석 금지.
- 멤버 변수명은 `m_` 접두사를 사용한다. (기존 코드 컨벤션 유지)
- 클래스 멤버 순서: public → private. 각 섹션 내에서 메서드 → 멤버 변수 순.

---

## MVC 역할 분리 규칙

### View 규칙

- View는 화면 출력과 사용자 입력 수집만 담당한다.
- View는 업무 판단을 하지 않는다. (`if (stock > 0)` 같은 재고 판단 금지)
- View는 Repository, Service, 다른 Controller에 직접 의존하지 않는다.
- View의 메서드는 `show*()`, `get*()`, `prompt*()`, `read*()` 형태로 명명한다.

### Controller 규칙

- Controller는 View와 Service(또는 Repository) 사이의 흐름 제어만 담당한다.
- Controller는 업무 규칙을 직접 구현하지 않는다. (상태 전환 판단, 재고 계산 등 Service에 위임)
- Controller는 JSON 파일에 직접 접근하지 않는다.
- Controller는 `IController` 인터페이스를 구현하며 `run()` 메서드를 진입점으로 사용한다.
- Controller 생성자는 필요한 View와 Service/Repository를 참조로 받는다. (소유하지 않음)

### Service 규칙 (별도 섹션 참조)

### Repository 규칙

- Repository는 단일 엔티티의 CRUD와 JSON 직렬화/역직렬화만 담당한다.
- Repository는 다른 Repository를 직접 의존하지 않는다. (Service를 통해 조합)
- Repository는 인터페이스(`I*Repository`)를 구현한다. Controller와 Service는 인터페이스에만 의존한다.
- `persist()` 는 private 메서드로 구현하며 add/update/remove 호출 직후 즉시 호출한다.

---

## JSON 접근 규칙

- JSON 파일 읽기/쓰기는 `JsonFileStorage` 를 통해서만 수행한다.
- `JsonFileStorage` 는 Repository 계층에서만 사용한다. Service, Controller에서 직접 사용 금지.
- `JsonFileWriter` 는 Generator/DummyData 생성 전용이다. 일반 업무 로직에서 사용 금지.
- JSON 파싱은 `JsonUtil` 네임스페이스 함수를 사용한다. 직접 문자열 파싱 구현 금지.
- `nextId` 는 파일에 저장된 값을 기준으로 채번하며 삭제 시 재사용하지 않는다.
- JSON 파일 경로는 `main.cpp`에서 `JsonFileStorage` 생성 시 지정한다. Repository 내부에 하드코딩 금지.

---

## Service 책임 규칙

### OrderService

- 주문 상태 전환의 유일한 관문이다. Controller가 직접 `Order.status`를 변경하지 않는다.
- 허용되지 않은 상태 전환 시 `false`를 반환하고 상태를 변경하지 않는다.
- `confirmOrder()` 는 `ProductionService.completeLine()` 내부에서만 호출한다. 외부에서 직접 호출 금지.

### ProductionService

- `completeLine()` 은 반드시 다음 순서로 처리한다:
  1. ProductionLine 상태 갱신 (DONE, progress=100) 및 persist
  2. `InventoryService.increase()` 호출
  3. `OrderService.confirmOrder()` 호출
- 순서가 보장되지 않으면 데이터 정합성이 깨진다.

### InventoryService

- `Sample.stock` 변경의 유일한 관문이다. Service나 Repository가 직접 `stock` 필드를 수정하지 않는다.
- `decrease()` 는 재고가 부족하면 아무 변경 없이 `false`를 반환한다.
- `increase()` 에서 수율 계산: `floor(quantity × yield / 100.0)`, 최솟값 1.

### ShipmentService

- 출고 처리 전 반드시 `Order.status == CONFIRMED`를 확인한다.
- 출고 처리 전 반드시 `InventoryService.canFulfill()`을 확인한다.
- 두 조건 중 하나라도 실패하면 전체 처리를 중단하고 `false`를 반환한다.

---

## Clean Code 규칙

### 함수/메서드

- 함수는 한 가지 일만 한다.
- 함수 길이는 40줄을 초과하지 않는 것을 권장한다. 초과 시 분리를 검토한다.
- 불필요한 else 분기를 피한다. early return을 사용한다.
- 매직 넘버 사용 금지. 의미 있는 이름의 상수(`constexpr`, `const`)로 정의한다.

### 네이밍

- 클래스명: PascalCase (`OrderService`, `ProductionLine`)
- 메서드명: camelCase (`createOrder`, `updateProgress`)
- 멤버 변수: `m_camelCase` (`m_orderRepo`, `m_status`)
- 상수: `kPascalCase` (`kMaxSample`, `kDefaultStock`)
- 파일명: 클래스명과 동일 (`OrderService.h`, `OrderService.cpp`)

### 의존성

- 의존성은 생성자 주입(Constructor Injection)으로만 전달한다. setter 주입, 전역 상태 사용 금지.
- 인터페이스에 의존한다. 구현 클래스에 직접 의존하지 않는다.
- 순환 의존 금지. 의존 방향: Controller → Service → Repository → Persistence.

### 헤더 파일

- 헤더 파일에는 `#pragma once`를 사용한다.
- 헤더 파일에서 `using namespace std;` 금지.
- 전방 선언(forward declaration)을 적극 활용하여 불필요한 `#include` 줄인다.

---

## Commit 규칙

### 커밋 메시지 형식

```
<type>: <subject>

Co-Authored-By: Claude Sonnet 4.6 (1M context) <noreply@anthropic.com>
```

### type 목록

| type | 사용 시점 |
|------|----------|
| `feat` | 새로운 기능 추가 |
| `refactor` | 기능 변경 없는 코드 구조 개선 |
| `fix` | 버그 수정 |
| `docs` | 문서 변경 |
| `chore` | 빌드 설정, `.gitignore` 등 |

### 커밋 단위 규칙

- 하나의 커밋은 하나의 논리적 변경 단위다.
- 모델, Repository, Service, Controller, View를 하나의 커밋에 섞지 않는다.
- 빌드가 깨지는 상태로 커밋하지 않는다.
- 이번 구현의 커밋 순서 (PRD.md의 Commit 계획 준수):

| 순서 | 대상 | type |
|------|------|------|
| C1 | Shipment 모델 추가 | feat |
| C2 | ProductionLineRepository | feat |
| C3 | ShipmentRepository | feat |
| C4 | InventoryService | feat |
| C5 | OrderService | feat |
| C6 | ProductionService | feat |
| C7 | ShipmentService + MonitoringService 확장 | feat |
| C8 | OrderController, ProductionLineController 연결 | refactor |
| C9 | ShipmentController, MonitoringController 연결 | refactor |
| C10 | View 확장 (메뉴 추가) | feat |
| C11 | main.cpp DI 통합 | refactor |

### Push 규칙

- `git push`는 사용자가 명시적으로 요청한 경우에만 수행한다.
- `main` 또는 `master` 브랜치에 force push 금지.
