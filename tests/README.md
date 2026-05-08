# 수동 테스트 시나리오 — SampleOrderSystem

콘솔 조작으로 검증하는 수동 테스트 시나리오 모음입니다.

---

## 테스트 환경

### 실행 전 준비

1. `SampleOrderSystem.exe` 를 실행한다.
2. 메인 메뉴에서 **1. SampleOrderSystem** 을 선택한다.
   - 메인 메뉴 → DummyData 생성이 필요하면 **2** 선택 후 더미 데이터 생성 가능

### Seed 데이터 사용 (권장)

`data/` 디렉토리에 아래 파일이 포함된 Seed 데이터가 준비되어 있다.

**시료 ID 참조표**

| ID | 이름 | 재고 | 수요 | 상태 |
|----|------|------|------|------|
| 1 | 플레이트-A형 | 50 | 10 | 여유 |
| 2 | 플레이트-B형 | 3 | 23 | 부족 |
| 3 | 칩-X형 | 0 | 5 | 고갈 |
| 4 | 칩-Y형 | 20 | 8 | 여유 |
| 5 | 웨이퍼-M형 | 5 | 20 | 부족 |
| 6 | 웨이퍼-N형 | 0 | 3 | 고갈 |
| 7 | 다이오드-Q형 | 100 | 0 | 여유 |
| 8 | 트랜지스터-R형 | 0 | 0 | 고갈 |

**주문 ID 참조표**

| ID | 시료ID | 고객사 | 수량 | 상태 |
|----|--------|--------|------|------|
| 1 | 1 | 삼성전자 | 10 | RESERVED |
| 2 | 3 | SK하이닉스 | 5 | RESERVED |
| 3 | 2 | TSMC | 8 | RESERVED |
| 4 | 5 | 인텔 | 20 | PRODUCING |
| 5 | 6 | AMD | 3 | PRODUCING |
| 6 | 2 | 퀄컴 | 15 | PRODUCING |
| 7 | 4 | 마이크론 | 5 | CONFIRMED |
| 8 | 7 | 퀄컴 | 30 | CONFIRMED |
| 9 | 7 | 삼성전자 | 15 | RELEASED |
| 10 | 4 | 인텔 | 3 | RELEASED |
| 11 | 8 | 마이크론 | 50 | REJECTED |
| 12 | 4 | AMD | 8 | RESERVED |

**생산 라인 ID 참조표**

| ID | 주문ID | 라인명 | 상태 | 진행률 |
|----|--------|--------|------|--------|
| 1 | 4 | LINE-004 | RUNNING | 45% |
| 2 | 5 | LINE-005 | IDLE | 0% |
| 3 | 6 | LINE-006 | PAUSED | 30% |
| 4 | 9 | LINE-009 | DONE | 100% |

### 메뉴 경로 표기 규칙

`메인 > 2. 주문 접수 > 1. 주문 등록` 형식으로 표기한다.

---

## TC-01 시료 등록 테스트

### TC-01-1 시료 정상 등록

**목적**: 모든 필드를 입력하면 시료가 등록되는지 확인

**경로**: 메인 > 1. 시료 관리 > 1. 시료 등록

**절차**
1. 이름: `테스트-A형` 입력
2. 설명: `테스트용 반도체 시료` 입력
3. 평균 생산 시간: `7` 입력
4. 수율(%): `85.0` 입력
5. 초기 재고: `10` 입력

**기대 결과**
- "시료가 등록되었습니다." 메시지 표시
- 시료 목록 조회 시 테스트-A형이 새 ID로 존재
- `data/samples.json` 에 레코드 추가, nextId 증가

---

### TC-01-2 중복 이름 등록 거부

**목적**: 동일한 이름의 시료를 중복 등록할 수 없는지 확인

**절차**
1. 이름: `플레이트-A형` 입력 (Seed ID=1과 동일)
2. 나머지 임의 값 입력

**기대 결과**
- "[오류] 이미 존재하는 시료 이름입니다." 메시지 표시
- 시료 추가 없음

---

### TC-01-3 수율 범위 초과 거부

**목적**: 수율 입력 범위(0 초과 ~ 100 이하) 벗어난 값 거부 확인

**절차 A** (0 이하)
1. 수율 입력 단계에서 `0` 입력

**절차 B** (100 초과)
1. 수율 입력 단계에서 `101` 입력

**기대 결과**
- 두 경우 모두 재입력 요청 메시지 표시
- 유효 범위(0.1~100.0) 값 입력 전까지 진행 불가

---

### TC-01-4 평균 생산 시간 0 이하 거부

**목적**: 생산 시간 최솟값(1분) 미만 입력 거부 확인

**절차**
1. 평균 생산 시간 단계에서 `0` 입력

**기대 결과**
- 재입력 요청 메시지 표시

---

### TC-01-5 시료 검색

**목적**: 이름 키워드로 시료를 검색할 수 있는지 확인

**경로**: 메인 > 1. 시료 관리 > 3. 시료 검색

**절차**
1. 검색어: `웨이퍼` 입력

**기대 결과**
- 웨이퍼-M형(ID=5), 웨이퍼-N형(ID=6) 2건만 표시
- 다른 시료는 표시되지 않음

---

### TC-01-6 시료 수정

**목적**: 시료 정보를 수정하면 파일에 반영되는지 확인

**경로**: 메인 > 1. 시료 관리 > 4. 시료 수정

**절차**
1. ID: `4` (칩-Y형) 입력
2. 이름: `칩-Y형` (동일 유지)
3. 수율: `90.0` 으로 변경

**기대 결과**
- "시료가 수정되었습니다." 메시지 표시
- 시료 목록에서 ID=4의 수율이 90.0으로 변경됨
- `data/samples.json` 반영

---

## TC-02 주문 등록 테스트

### TC-02-1 정상 주문 등록

**목적**: 존재하는 시료에 주문을 등록하면 RESERVED 상태로 저장되는지 확인

**경로**: 메인 > 2. 주문 접수 > 1. 주문 등록

**절차**
1. 시료 ID: `1` 입력 (플레이트-A형)
2. 고객사명: `테스트주문고객` 입력
3. 수량: `5` 입력

**기대 결과**
- "주문이 등록되었습니다. (상태: RESERVED)" 메시지 표시
- 주문 목록에 새 주문 존재, status=RESERVED
- productName = 플레이트-A형 (자동 조회)
- `data/orders.json` 에 레코드 추가

---

### TC-02-2 존재하지 않는 시료 ID 거부

**목적**: 없는 시료 ID로 주문 시 오류 처리 확인

**절차**
1. 시료 ID: `999` 입력 (존재하지 않는 ID)

**기대 결과**
- "[오류] 존재하지 않는 시료 ID입니다: 999" 메시지 표시
- 주문 등록 없음

---

### TC-02-3 수량 0 이하 거부

**목적**: 주문 수량 최솟값(1개) 미만 입력 거부 확인

**절차**
1. 시료 ID: `1`, 고객사: `테스트`
2. 수량: `0` 입력

**기대 결과**
- 재입력 요청 메시지 표시

---

### TC-02-4 주문 목록 조회

**목적**: 전체 주문 목록이 모든 상태를 포함하여 표시되는지 확인

**경로**: 메인 > 2. 주문 접수 > 2. 주문 목록 조회

**기대 결과**
- Seed 주문 12건이 ID/시료ID/제품명/고객사/수량/상태 형식으로 표시
- RESERVED, PRODUCING, CONFIRMED, RELEASED, REJECTED 모두 포함

---

## TC-03 주문 승인 테스트

### TC-03-1 재고 충분 → CONFIRMED

**목적**: 재고 ≥ 주문 수량인 경우 CONFIRMED로 변경되는지 확인

**Seed 조건**: 주문 ID=1 (플레이트-A형 stock=50, qty=10 → 재고 충분)

**경로**: 메인 > 3. 주문 승인/거절 > 1. 주문 승인

**절차**
1. RESERVED 목록 확인
2. ID: `1` 입력

**기대 결과**
- "승인 완료 — 재고 충분 (상태: CONFIRMED)" 메시지 표시
- 주문 ID=1 status = CONFIRMED
- 생산 라인 미생성 (재고 충분이므로)

---

### TC-03-2 재고 고갈 → PRODUCING + Queue 등록

**목적**: 재고 = 0인 경우 PRODUCING 전환 및 생산 라인(IDLE)이 생성되는지 확인

**Seed 조건**: 주문 ID=2 (칩-X형 stock=0, qty=5 → 재고 고갈)

**절차**
1. ID: `2` 입력

**기대 결과**
- "승인 완료 — 재고 부족, 생산 큐 등록 (상태: PRODUCING)" 메시지 표시
- 주문 ID=2 status = PRODUCING
- `data/production_lines.json` 에 orderId=2, status=IDLE 라인 추가

---

### TC-03-3 재고 부족 → PRODUCING + Queue 등록

**목적**: 재고 > 0 이지만 수량 미달인 경우 PRODUCING 전환 확인

**Seed 조건**: 주문 ID=3 (플레이트-B형 stock=3, qty=8 → 재고 부족)

**절차**
1. ID: `3` 입력

**기대 결과**
- "승인 완료 — 재고 부족, 생산 큐 등록 (상태: PRODUCING)" 메시지 표시
- 주문 ID=3 status = PRODUCING
- 생산 라인 IDLE 생성됨

---

### TC-03-4 PRODUCING 상태 주문 승인 시도 거부

**목적**: RESERVED 이외 상태 주문은 승인 불가함을 확인

**Seed 조건**: 주문 ID=4 (status=PRODUCING)

**절차**
1. ID: `4` 입력

**기대 결과**
- "[오류] RESERVED 상태의 주문만 승인할 수 있습니다." 메시지 표시
- 주문 상태 변화 없음

---

## TC-04 주문 거절 테스트

### TC-04-1 RESERVED 주문 정상 거절

**목적**: RESERVED 주문을 거절하면 REJECTED로 변경되는지 확인

**Seed 조건**: 주문 ID=12 (칩-Y형, AMD, qty=8, RESERVED)

**경로**: 메인 > 3. 주문 승인/거절 > 2. 주문 거절

**절차**
1. ID: `12` 입력

**기대 결과**
- "주문이 거절되었습니다. (상태: REJECTED)" 메시지 표시
- 주문 ID=12 status = REJECTED
- `data/orders.json` 반영

---

### TC-04-2 CONFIRMED 주문 거절 시도 거부

**목적**: CONFIRMED 이후 상태는 거절 불가함을 확인

**Seed 조건**: 주문 ID=7 (status=CONFIRMED)

**절차**
1. ID: `7` 입력

**기대 결과**
- "[오류] RESERVED 상태의 주문만 거절할 수 있습니다." 메시지 표시
- 주문 상태 변화 없음

---

### TC-04-3 REJECTED 주문 모니터링 집계 제외 확인

**목적**: 거절된 주문이 Dashboard 집계에 포함되지 않는지 확인

**전제 조건**: TC-04-1 완료 (주문 ID=12 REJECTED)

**절차**
1. 메인 > 5. 모니터링 > 2. 주문 상태별 집계

**기대 결과**
- 주문 ID=12가 RESERVED, PRODUCING, CONFIRMED, RELEASED 어느 항목에도 집계되지 않음
- 합계(total)에서도 제외됨

---

## TC-05 생산라인 Queue 테스트

### TC-05-1 생산 Queue 조회 (IDLE 라인)

**목적**: IDLE 상태 라인만 Queue에 표시되는지 확인

**경로**: 메인 > 4. 생산 라인 > 2. 생산 Queue 조회

**기대 결과**
- Seed 기준: LINE-005(ID=2, orderId=5, IDLE) 1건 표시
- RUNNING/PAUSED/DONE 라인은 표시되지 않음

---

### TC-05-2 생산 현황 조회 (RUNNING/PAUSED)

**목적**: 활성 생산 라인이 현황에 표시되는지 확인

**경로**: 메인 > 4. 생산 라인 > 1. 생산 현황 조회

**기대 결과**
- LINE-004(ID=1, RUNNING 45%), LINE-006(ID=3, PAUSED 30%) 2건 표시
- IDLE, DONE 라인은 표시되지 않음

---

### TC-05-3 FIFO Queue 시작 (최저 ID IDLE 자동 선택)

**목적**: 생산 시작 시 IDLE 라인 중 가장 낮은 ID가 선택되는지 확인

**Seed 조건**: IDLE 라인 = LINE-005(ID=2, orderId=5)

**경로**: 메인 > 4. 생산 라인 > 3. 생산 시작

**절차**
1. Queue 목록 확인 후 생산 시작 진행

**기대 결과**
- LINE-005 정보 표시 (라인명, 주문ID, 실 생산량, 예상 시간)
- LINE-005 status = RUNNING, progress = 0
- 생산량 계산 검증:
  - 주문 ID=5: qty=3, 시료 ID=6 (웨이퍼-N형 stock=0, yield=70.0%)
  - shortfall = 3 - 0 = 3
  - effectiveYield = 0.70 × 0.9 = 0.630
  - actualQty = ceil(3 / 0.630) = ceil(4.76) = **5**
  - totalTime = 15 × 5 = **75분**

---

### TC-05-4 Queue 비어있을 때 시작 시도

**목적**: 대기 항목이 없으면 안내 메시지가 표시되는지 확인

**전제 조건**: 모든 IDLE 라인이 RUNNING 또는 DONE으로 전환된 상태

**기대 결과**
- "생산 대기 중인 항목이 없습니다." 메시지 표시

---

## TC-06 생산 완료 테스트

### TC-06-1 생산 완료 처리 및 공식 검증

**목적**: 생산 완료 시 공식에 따른 재고 증가와 주문 CONFIRMED 전환 확인

**Seed 조건**:
- 라인 ID=1 (LINE-004, RUNNING, orderId=4)
- 주문 ID=4: qty=20, 시료 ID=5 (웨이퍼-M형 stock=5, avgTime=10, yield=78.0%)

**경로**: 메인 > 4. 생산 라인 > 4. 생산 완료 처리

**절차**
1. 완료 전 시료 ID=5의 stock 값 기록 (5)
2. 현황 목록에서 라인 ID=1 입력

**기대 결과**
- "생산 완료 처리되었습니다." 메시지 표시
- **실 생산량 계산 검증**:
  - shortfall = 20 - 5 = 15
  - effectiveYield = 0.78 × 0.9 = 0.702
  - actualQty = ceil(15 / 0.702) = ceil(21.37) = **22**
  - totalTime = 10 × 22 = **220분**
- 시료 ID=5 stock: 5 + 22 = **27**
- 주문 ID=4 status = **CONFIRMED**
- 라인 ID=1 status = **DONE**, progress = **100**
- `data/samples.json`, `data/orders.json`, `data/production_lines.json` 모두 반영

---

### TC-06-2 RUNNING 아닌 라인 완료 시도 거부

**목적**: PAUSED/IDLE/DONE 상태 라인의 완료 처리가 거부되는지 확인

**Seed 조건**: 라인 ID=3 (LINE-006, PAUSED)

**절차**
1. ID: `3` 입력

**기대 결과**
- "[오류] RUNNING 상태의 라인만 완료 처리할 수 있습니다." 메시지 표시
- 라인 상태 변화 없음

---

## TC-07 모니터링 Dashboard 테스트

### TC-07-1 전체 Dashboard 3섹션 출력

**목적**: Dashboard가 주문현황 + 시료재고 + 생산현황 3섹션을 모두 표시하는지 확인

**경로**: 메인 > 5. 모니터링 > 1. 전체 Dashboard

**기대 결과** (Seed 기준)
- **[주문 현황]**: RESERVED=4, PRODUCING=3, CONFIRMED=2, RELEASED=2, 합계=11 (REJECTED 제외)
- **[시료 재고 현황]**: 8개 시료 목록 (재고/수요/상태 포함)
- **[생산 현황]**: RUNNING 1건, PAUSED 1건

---

### TC-07-2 시료 재고 현황 - 3가지 상태 모두 표시

**경로**: 메인 > 5. 모니터링 > 3. 시료 재고 현황

**기대 결과**
- **[여유]**: ID=1(stock=50>demand=10), ID=4(stock=20>demand=8), ID=7(stock=100>demand=0) → 3건
- **[부족]**: ID=2(stock=3≤demand=23), ID=5(stock=5≤demand=20) → 2건
- **[고갈]**: ID=3(stock=0), ID=6(stock=0), ID=8(stock=0) → 3건
- 하단 요약: 여유 3개 / 부족 2개 / 고갈 3개

---

### TC-07-3 REJECTED 주문 집계 제외 확인

**Seed 조건**: 주문 ID=11 (REJECTED)

**절차**
1. Dashboard 또는 주문 상태별 집계 화면 확인

**기대 결과**
- 총 주문 12건 중 REJECTED(ID=11) 1건이 합계에 포함되지 않음
- 합계 = 11건

---

### TC-07-4 생산 현황 조회

**경로**: 메인 > 5. 모니터링 > 4. 생산 현황

**기대 결과**
- LINE-004(RUNNING, 45%), LINE-006(PAUSED, 30%) 표시
- 라인 ID, 주문 ID, 진행률, 상태, 라인명 포함

---

## TC-08 출고 처리 테스트

### TC-08-1 정상 출고 처리

**목적**: CONFIRMED 주문을 출고하면 재고 차감과 RELEASED 전환이 발생하는지 확인

**Seed 조건**: 주문 ID=7 (칩-Y형 stock=20, qty=5, CONFIRMED)

**경로**: 메인 > 6. 출고 처리 > 1. 출고 처리

**절차**
1. CONFIRMED 목록에서 주문 ID=7 확인
2. ID: `7` 입력

**기대 결과**
- "출고 처리 완료되었습니다. (상태: RELEASED)" 메시지 표시
- 주문 ID=7 status = RELEASED
- 시료 ID=4 (칩-Y형) stock: 20 - 5 = **15**
- `data/shipments.json` 에 새 출고 레코드 생성 (shippedAt = 오늘 날짜)
- `data/orders.json`, `data/samples.json` 반영

---

### TC-08-2 재고 부족 출고 거부

**목적**: 재고가 주문 수량보다 적으면 출고가 거부되는지 확인

**전제 조건**: TC-08-1 완료 후 시료 ID=4 stock=15인 상태에서
- 주문 ID=8 (다이오드-Q형 stock=100, qty=30) 출고 → 정상 (다른 시료)

실제 거부 시나리오:
1. TC-06-1 미실행 상태에서 주문 ID=4 (웨이퍼-M형 stock=5, qty=20, PRODUCING) 직접 상태 불일치 확인

**절차**
1. CONFIRMED 목록에서 주문 ID를 잘못 입력하거나, 재고를 시료 수정으로 0으로 만든 후 시도

**기대 결과**
- "[오류] 재고가 부족하여 출고할 수 없습니다." 메시지 표시
- 주문 상태 변화 없음 (CONFIRMED 유지)
- 재고 변화 없음

---

### TC-08-3 CONFIRMED 이외 상태 출고 거부

**목적**: RESERVED, PRODUCING 등의 상태 주문은 출고 불가함을 확인

**절차**
1. 출고 처리 메뉴 진입 시 CONFIRMED 목록만 표시됨을 확인
2. 출고 ID 입력 창에서 RESERVED 주문 ID=1 입력

**기대 결과**
- "[오류] CONFIRMED 상태의 주문만 출고할 수 있습니다." 메시지 표시

---

### TC-08-4 출고 이력 조회

**목적**: 출고 처리 후 이력이 정확히 기록되는지 확인

**전제 조건**: TC-08-1 완료

**경로**: 메인 > 6. 출고 처리 > 2. 출고 이력 조회

**기대 결과**
- ID, 주문ID, 시료ID, 수량, 출고일자, 고객사 테이블 표시
- TC-08-1에서 처리한 주문 ID=7 레코드가 shippedAt = 오늘 날짜로 존재

---

## TC-09 JSON Persistence 테스트

### TC-09-1 각 JSON 파일 구조 확인

**목적**: 저장된 JSON이 파서가 읽을 수 있는 구조인지 확인

**방법**: 텍스트 편집기로 `data/` 내 4개 파일 직접 확인

**기대 결과**

| 파일 | 최상위 필드 | 필수 항목 |
|------|-----------|----------|
| samples.json | nextId, items | id, name, description, stock, avgProductionTime, yield |
| orders.json | nextId, items | id, sampleId, productName, customerName, quantity, status |
| production_lines.json | nextId, items | id, orderId, lineName, status, progress |
| shipments.json | nextId, items | id, orderId, sampleId, quantity, customerName, shippedAt |

---

### TC-09-2 nextId 단조 증가 확인

**목적**: 레코드 거절(상태만 변경) 후 새 항목 추가 시 ID가 재사용되지 않는지 확인

**절차**
1. 주문 목록에서 현재 최대 ID (N=12) 확인
2. 새 주문 등록 (시료 ID=1, 고객사: 테스트, 수량=1)

**기대 결과**
- 새 주문 ID = 13 (N+1)
- `data/orders.json` nextId = 14

---

### TC-09-3 파일 동기화 즉시성 확인

**목적**: 조작 직후 파일에 변경이 반영되는지 확인

**절차**
1. 시료 수정 (stock 임의 변경)
2. 프로그램 종료 없이 텍스트 편집기로 `data/samples.json` 확인

**기대 결과**
- 수정 직후 파일에 변경된 stock 값이 존재 (메모리-파일 동기화)

---

## TC-10 재실행 후 데이터 유지 테스트

### TC-10-1 주문 상태 재시작 후 유지

**목적**: 프로그램을 종료하고 재시작해도 주문 상태가 보존되는지 확인

**절차**
1. TC-03-1 완료 (주문 ID=1 CONFIRMED 상태)
2. 주문 목록에서 ID=1 상태(CONFIRMED) 메모
3. 프로그램 종료 (`0. 종료`)
4. 프로그램 재시작
5. 메인 > 2. 주문 접수 > 2. 주문 목록 조회

**기대 결과**
- 주문 ID=1 status = CONFIRMED (종료 전과 동일)
- 전체 주문 건수 동일

---

### TC-10-2 재고 변경 재시작 후 유지

**목적**: 생산 완료로 변경된 재고가 재시작 후에도 유지되는지 확인

**전제 조건**: TC-06-1 완료 (시료 ID=5 stock=27로 증가)

**절차**
1. 시료 stock=27 확인 메모
2. 프로그램 종료 후 재시작
3. 메인 > 1. 시료 관리 > 2. 시료 목록 조회 또는 모니터링

**기대 결과**
- 시료 ID=5 (웨이퍼-M형) stock = 27 유지

---

### TC-10-3 출고 이력 재시작 후 유지

**전제 조건**: TC-08-1 완료 (shipments.json 생성)

**절차**
1. 프로그램 종료 후 재시작
2. 메인 > 6. 출고 처리 > 2. 출고 이력 조회

**기대 결과**
- 이전 세션의 출고 레코드가 그대로 표시됨

---

### TC-10-4 생산 라인 상태 재시작 후 유지

**전제 조건**: TC-05-3 완료 (LINE-005 RUNNING으로 변경)

**절차**
1. 프로그램 종료 후 재시작
2. 메인 > 4. 생산 라인 > 1. 생산 현황 조회

**기대 결과**
- LINE-005 status = RUNNING (재시작 후 상태 유지)

---

## TC-11 MVC 역할 분리 검증

코드 리뷰로 확인하는 정적 구조 검증 항목입니다.

### TC-11-1 Controller → JSON 직접 접근 없음

**확인 방법**: 모든 `*Controller.cpp` 에서 `JsonFileStorage`, `JsonFileWriter` import 또는 직접 생성 코드 검색

```
Grep 대상: #include.*JsonFileStorage, #include.*JsonFileWriter
대상 파일: src/controllers/*.cpp
```

**기대 결과**: 검색 결과 없음 (Controller 계층에서 Persistence 직접 접근 없음)

---

### TC-11-2 View → 업무 조건 판단 없음

**확인 방법**: 모든 `*View.cpp` 에서 status 비교, stock 비교, quantity 비교 코드 검색

```
Grep 대상: status ==, stock >, stock <, quantity >
대상 파일: src/views/*.cpp
```

**기대 결과**: View 파일에서 업무 규칙 판단 로직 없음

---

### TC-11-3 Service → View 의존 없음

**확인 방법**: 모든 `*Service.h` 의 `#include` 목록에서 View 헤더 참조 검색

```
Grep 대상: #include.*View.h
대상 파일: src/services/*.h
```

**기대 결과**: Service 헤더에서 View 포함 없음

---

### TC-11-4 재고(stock) 변경 — InventoryService 단일 관문

**확인 방법**: Repository 외부에서 `stock` 필드를 직접 수정하는 코드 검색

```
Grep 대상: \.stock\s*=, \.stock\s*\+=, \.stock\s*-=
제외 파일: src/repositories/*.cpp (Repository 내부는 허용)
대상 파일: src/services/*.cpp, src/controllers/*.cpp
```

**기대 결과**: `InventoryService.cpp` 외 파일에서 stock 직접 수정 없음

---

### TC-11-5 주문 상태(status) 변경 — 서비스 계층 경유

**확인 방법**: Repository 외부에서 `order.status` 또는 `updated.status` 를 직접 변경하는 코드가 있다면, 반드시 Service(`OrderService`, `ShipmentService`) 내부임을 확인

```
Grep 대상: \.status\s*=
대상 파일: src/services/*.cpp
```

**기대 결과**: Controller에서 order.status 직접 수정 없음

---

### TC-11-6 의존성 방향 단방향 확인

**확인 방법**: 하위 계층이 상위 계층을 역방향 참조하지 않는지 확인

```
Repository 헤더가 Service 헤더를 include 하지 않음
Service 헤더가 Controller 헤더를 include 하지 않음
```

**기대 결과**: 계층 역전 없음

---

## TC-12 Clean Code 점검 항목

코드 품질을 정적 확인으로 검증하는 항목입니다.

### TC-12-1 함수 길이

**기준**: 단일 함수 40줄 이하 권장

**확인 방법**: 가장 긴 메서드가 포함된 파일을 식별하여 줄 수 확인

**점검 대상**: `completeLine()`, `ship()`, `approveOrder()`, `handleCreate()` 등 복잡한 메서드

**기대 결과**: 각 메서드가 명확한 단일 책임을 갖고 40줄 내외로 작성됨

---

### TC-12-2 매직 넘버 사용 없음

**확인 방법**: 의미 불명 숫자 리터럴 검색

```
Grep 대상: [^0-9][2-9][0-9]+[^0-9]  (다자리 숫자)
주의 확인: 100 (yield 기준값), 0.9 (안전 계수) 등은 상수 또는 주석으로 명시
```

**기대 결과**
- 하드코딩된 의미 불명의 숫자 없음
- `kMaxSample = 1000` 등 constexpr/const 로 정의됨
- 생산 공식 내 `0.9` (안전 계수) 사용 위치가 명확함

---

### TC-12-3 함수 네이밍 일관성

**기준**: 동사+명사 형태, camelCase

**확인 방법**: 각 계층별 네이밍 패턴 검토

| 계층 | 패턴 | 예시 |
|------|------|------|
| View | show*, prompt*, get* | showMenu, promptOrderId |
| Controller | handle* | handleCreate, handleApprove |
| Service | 동사+명사 | createOrder, completeLine |
| Repository | find*, add, update, remove | findById, findAll |

**기대 결과**: 각 계층에서 일관된 네이밍 패턴 사용

---

### TC-12-4 주석 최소화 원칙

**기준**: WHY 주석만 허용 (WHAT 주석 금지)

**확인 방법**: `// ──` 구분자, 단계 표기, 알고리즘 이유 주석 검토

**기대 결과**
- 코드 자체가 설명적인 경우 주석 없음
- 생산 공식(`ceil(부족분 / (수율 * 0.9))`) 등 비즈니스 규칙은 주석 또는 상수명으로 명시

---

### TC-12-5 const 정확성

**확인 방법**: 데이터를 변경하지 않는 메서드에 `const` 한정자 적용 여부 확인

```
Grep 대상: const; (메서드 const 선언)
대상: findAll, getOrderSummary, getActiveLines, showMenu 등 읽기 전용 메서드
```

**기대 결과**: 읽기 전용 메서드에 `const` 적용됨

---

### TC-12-6 단일 책임 원칙 (SRP) 확인

**확인 항목**

| 클래스 | 단일 책임 |
|--------|----------|
| `SampleService` | 시료 마스터 관리만 담당 |
| `InventoryService` | 재고 수량 변경만 담당 |
| `OrderService` | 주문 상태 전환만 담당 |
| `ProductionService` | 생산 라인 생명주기만 담당 |
| `ShipmentService` | 출고 처리만 담당 |
| `MonitoringService` | 현황 조회만 담당 (변경 없음) |

**기대 결과**: 각 Service가 위 책임 이외의 역할을 수행하지 않음

---

## 전체 흐름 통합 테스트

### FULL-01 완전한 업무 흐름 (Seed 데이터 기준)

Seed 데이터를 사용해 전체 흐름을 순서대로 완주한다.

| 단계 | 메뉴 경로 | 입력 | 확인 사항 |
|------|----------|------|----------|
| 1 | 모니터링 > Dashboard | - | RESERVED=4, PRODUCING=3, CONFIRMED=2, 합계=11 |
| 2 | 주문 승인/거절 > 승인 | ID=1 | CONFIRMED, 재고 충분 메시지 |
| 3 | 주문 승인/거절 > 승인 | ID=2 | PRODUCING, 재고 고갈 → Queue 등록 |
| 4 | 생산 라인 > Queue 조회 | - | LINE-005(ID=2) IDLE 존재 |
| 5 | 생산 라인 > 생산 시작 | - | LINE-005 RUNNING 전환, 실생산량/시간 표시 |
| 6 | 생산 라인 > 생산 완료 | ID=1 | LINE-004 DONE, 웨이퍼-M형 stock+22, 주문ID=4 CONFIRMED |
| 7 | 출고 처리 > 출고 처리 | ID=7 | 칩-Y형 stock-5, 주문ID=7 RELEASED |
| 8 | 출고 처리 > 출고 이력 | - | 방금 출고 레코드 확인 |
| 9 | 모니터링 > Dashboard | - | 변경된 집계 확인 |
| 10 | 종료 후 재시작 | - | 모든 변경 사항 복원 확인 |

**통과 기준**: 10단계 모두 기대 결과와 일치하면 SampleOrderSystem PoC 완료 조건 충족
