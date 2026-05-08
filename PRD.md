# PRD — SampleOrderSystem

반도체 시료 생산주문관리 시스템 제품 요구사항 문서

---

## 기능 요구사항

### FR-01 시료(Sample) 관리

| ID | 요구사항 |
|----|----------|
| FR-01-1 | 시료를 등록할 수 있다. (name, description, stock, avgProductionTime, yield) |
| FR-01-2 | 시료 목록을 전체 조회할 수 있다. |
| FR-01-3 | 시료 정보를 수정할 수 있다. |
| FR-01-4 | 시료를 삭제할 수 있다. |
| FR-01-5 | 시료별 재고(stock) 수량을 확인할 수 있다. |

---

### FR-02 주문(Order) 관리

| ID | 요구사항 |
|----|----------|
| FR-02-1 | 고객사명, 시료 ID, 수량을 입력하여 주문을 생성할 수 있다. 생성 시 상태는 RESERVED. |
| FR-02-2 | 주문 목록을 전체 조회할 수 있다. |
| FR-02-3 | 주문 상태별 필터 조회를 할 수 있다. |
| FR-02-4 | RESERVED 상태 주문에 생산 라인을 배정하여 생산을 시작할 수 있다. 성공 시 상태는 PRODUCING. |
| FR-02-5 | RESERVED 또는 PRODUCING 상태 주문을 취소할 수 있다. 취소 시 상태는 REJECTED. |
| FR-02-6 | 유효하지 않은 상태 전환 시도는 거부되고 오류 메시지가 표시된다. |

---

### FR-03 생산 라인(ProductionLine) 관리

| ID | 요구사항 |
|----|----------|
| FR-03-1 | 생산 라인 목록(라인명, 연결 주문 ID, 상태, 진행률)을 조회할 수 있다. |
| FR-03-2 | 생산 라인의 진행률(0~100)을 수동으로 갱신할 수 있다. |
| FR-03-3 | RUNNING 상태 생산 라인을 일시중지(PAUSED)할 수 있다. |
| FR-03-4 | PAUSED 상태 생산 라인을 재개(RUNNING)할 수 있다. |
| FR-03-5 | 생산 완료 처리를 수행할 수 있다. 완료 시: 진행률 100, 상태 DONE, 재고 증가, 연결 주문 CONFIRMED 전환이 순서대로 처리된다. |
| FR-03-6 | 생산 완료 시 재고 증가량은 `order.quantity × (sample.yield / 100.0)`의 정수 내림값으로 계산한다. |

---

### FR-04 출고(Shipment) 처리

| ID | 요구사항 |
|----|----------|
| FR-04-1 | CONFIRMED 상태 주문에 대해 출고를 처리할 수 있다. |
| FR-04-2 | 출고 처리 시: 재고 차감, 주문 상태 RELEASED 전환, Shipment 레코드 생성이 순서대로 처리된다. |
| FR-04-3 | 재고가 주문 수량보다 부족한 경우 출고는 거부되고 오류 메시지가 표시된다. |
| FR-04-4 | 출고 이력(출고 ID, 주문 ID, 시료명, 수량, 고객사, 출고일자)을 조회할 수 있다. |
| FR-04-5 | CONFIRMED 이외의 상태 주문에 대한 출고 시도는 거부된다. |

---

### FR-05 모니터링(Monitoring)

| ID | 요구사항 |
|----|----------|
| FR-05-1 | 대시보드에서 주문 상태별 건수(RESERVED, PRODUCING, CONFIRMED, RELEASED, REJECTED, 합계)를 확인할 수 있다. |
| FR-05-2 | 시료별 재고 현황(재고 수량, 수요 건수, 재고 상태)을 확인할 수 있다. |
| FR-05-3 | 재고 상태는 SURPLUS(여유), SHORTAGE(부족), DEPLETED(고갈) 세 단계로 표시된다. |
| FR-05-4 | 생산 현황에서 활성 생산 라인(RUNNING, PAUSED)의 진행률을 확인할 수 있다. |

---

### FR-06 더미 데이터 생성(DummyData Generator)

| ID | 요구사항 |
|----|----------|
| FR-06-1 | 시료 데이터를 최대 1,000건까지 생성할 수 있다. |
| FR-06-2 | 주문 데이터를 최대 10,000건까지 생성할 수 있다. |
| FR-06-3 | 생산 라인 데이터를 최대 10,000건까지 생성할 수 있다. |
| FR-06-4 | 덮어쓰기(overwrite) 또는 추가(append) 모드를 선택할 수 있다. |
| FR-06-5 | 시료 / 주문 / 생산 라인 / 전체를 일괄 생성할 수 있다. |

---

## 비기능 요구사항

| ID | 항목 | 요구사항 |
|----|------|----------|
| NFR-01 | 데이터 저장 | DB 사용 금지. JSON 파일(`data/` 디렉토리)만 사용. |
| NFR-02 | 언어 | C++17 표준 준수. |
| NFR-03 | 플랫폼 | Windows 콘솔 환경에서 동작. |
| NFR-04 | 의존성 | 외부 라이브러리 사용 금지. 표준 라이브러리만 사용. |
| NFR-05 | 데이터 정합성 | JSON 파일 저장은 각 변경 직후 즉시 수행(persist). 메모리 상태와 파일 상태는 항상 동기화. |
| NFR-06 | ID 채번 | nextId는 파일에 함께 저장되며 단조 증가. 삭제 시 ID 재사용 없음. |
| NFR-07 | 아키텍처 | MVC + Repository + Service 계층 구조 유지. 계층 간 역방향 의존 금지. |
| NFR-08 | 코드 스타일 | 각 클래스의 책임은 단일 원칙(SRP) 준수. |

---

## 주문 상태 흐름

```
createOrder()
     │
     ▼
[RESERVED] ──── cancelOrder() ──► [REJECTED]
     │
startProduction()
     │
     ▼
[PRODUCING] ──── cancelOrder() ──► [REJECTED]
     │
completeLine() 트리거
     │
     ▼
[CONFIRMED]
     │
ship()
     │
     ▼
[RELEASED]
```

### 전이 허용 표

| 현재 상태 \ 목표 상태 | RESERVED | PRODUCING | CONFIRMED | RELEASED | REJECTED |
|----------------------|----------|-----------|-----------|----------|----------|
| RESERVED | — | ✅ startProduction | ❌ | ❌ | ✅ cancel |
| PRODUCING | ❌ | — | ✅ completeLine | ❌ | ✅ cancel |
| CONFIRMED | ❌ | ❌ | — | ✅ ship | ❌ |
| RELEASED | ❌ | ❌ | ❌ | — | ❌ |
| REJECTED | ❌ | ❌ | ❌ | ❌ | — |

---

## 재고 처리 규칙

### 재고 증가 (생산 완료 시)

- 트리거: `ProductionService.completeLine()` 호출
- 공식: `증가량 = floor(order.quantity × sample.yield / 100.0)`
- 최솟값: 1 (수율이 낮더라도 1개 이상 생산된 것으로 처리)
- 처리 순서:
  1. ProductionLine.status → DONE, progress → 100
  2. `InventoryService.increase(sampleId, 증가량)`
  3. `OrderService.confirmOrder(orderId)` → Order.status → CONFIRMED

### 재고 감소 (출고 시)

- 트리거: `ShipmentService.ship()` 호출
- 공식: `감소량 = order.quantity`
- 사전 조건: `sample.stock >= order.quantity` (부족 시 출고 거부)
- 처리 순서:
  1. `InventoryService.canFulfill(sampleId, quantity)` 확인
  2. `InventoryService.decrease(sampleId, quantity)`
  3. Order.status → RELEASED
  4. Shipment 레코드 생성 및 저장

### 재고 상태 기준 (SampleStatus.stockStatus)

| 상태 | 조건 | 설명 |
|------|------|------|
| SURPLUS | stock > demand | 재고 여유 |
| SHORTAGE | 0 < stock ≤ demand | 재고 부족 |
| DEPLETED | stock == 0 | 재고 고갈 |

- demand: 해당 시료를 참조하는 RESERVED + PRODUCING 상태 주문의 수량 합계

---

## 생산 라인 처리 규칙

### 상태 전이 규칙

| 현재 상태 | 허용 전이 | 거부 전이 |
|----------|-----------|----------|
| IDLE | → RUNNING | → PAUSED, → DONE |
| RUNNING | → PAUSED, → DONE | → IDLE |
| PAUSED | → RUNNING | → IDLE, → DONE |
| DONE | (종단) | 모두 |

### 진행률 규칙

- IDLE 생성 시 progress = 0
- RUNNING 상태에서만 진행률 수동 갱신 가능 (1~99)
- PAUSED 상태에서는 진행률 값 유지 (갱신 불가)
- completeLine() 호출 시 progress 자동으로 100 설정
- DONE 이후 진행률 변경 불가

### 주문과의 연동 규칙

- 1 Order : 1 ProductionLine (이번 PoC 범위)
- 이미 ProductionLine이 존재하는 Order에 중복 생산 시작 불가
- ProductionLine 삭제 시 연결 Order 상태는 변경하지 않음 (참조만 해제)

---

## 완료 조건

이번 PoC 구현이 완료된 것으로 판단하는 기준입니다.

| # | 완료 기준 |
|---|-----------|
| 1 | 주문 생성 → 생산 시작 → 진행률 갱신 → 생산 완료 → 출고 처리의 전체 흐름이 오류 없이 동작한다. |
| 2 | 생산 완료 시 재고가 yield 공식에 따라 정확히 증가한다. |
| 3 | 출고 처리 시 재고가 정확히 차감되고 Shipment 레코드가 파일에 저장된다. |
| 4 | 재고 부족 상태에서 출고 시도 시 거부 메시지가 표시된다. |
| 5 | 유효하지 않은 상태 전환 시도 시 거부 메시지가 표시된다. |
| 6 | 프로그램 재시작 후 데이터가 JSON 파일에서 정상 복원된다. |
| 7 | 모니터링 대시보드에서 실시간 주문 집계 및 재고 현황이 정확히 표시된다. |
| 8 | Controller가 직접 JSON 파일에 접근하지 않는다. |
| 9 | Service가 View 클래스에 의존하지 않는다. |

---

## 제외 범위

이번 PoC에서 의도적으로 구현하지 않는 항목입니다.

| 항목 | 제외 이유 |
|------|----------|
| 사용자 인증/권한 분리 | 역할 기반 접근 제어는 PoC 검증 범위 외 |
| 네트워크/API | 콘솔 단일 프로세스 검증이 목적 |
| 실시간 자동 진행률 갱신 | 수동 입력 방식으로 흐름 검증에 충분 |
| 1 Order : N ProductionLine | 단순화하여 1:1 관계만 검증 |
| 데이터 백업/복구 자동화 | JsonFileStorage 수준의 단순 파일 I/O로 충분 |
| 날짜/시간 기반 스케줄링 | 수동 메뉴 조작으로 흐름 검증 |
| UI 프레임워크 | Win32 API 포함 GUI 일체 제외 |
| 단위 테스트 자동화 | 수동 시나리오 테스트로 대체 |
