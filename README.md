# SampleOrderSystem

반도체 시료 생산주문관리 콘솔 시스템 (C++17 / JSON 기반 PoC)

---

## 프로젝트 소개

반도체 시료(Sample)의 주문 접수부터 생산, 재고 반영, 출고까지의 전체 업무 흐름을 콘솔 환경에서 검증하는 PoC 시스템입니다.

DB 없이 JSON 파일만으로 데이터를 영속화하며, MVC 구조와 Service 계층을 통해 역할 분리 구조를 검증합니다.

---

## 시스템 개요

| 항목 | 내용 |
|------|------|
| 언어 | C++17 |
| 플랫폼 | Windows (Console) |
| 데이터 저장 | JSON 파일 (`data/` 디렉토리) |
| 아키텍처 | MVC + Repository + Service 계층 |
| 빌드 시스템 | Visual Studio (`.vcxproj`) |

---

## 주요 기능 설명

### 시료(Sample) 관리
- 시료 등록 / 수정 / 삭제 / 목록 조회
- 시료별 재고(stock), 평균 생산 시간(avgProductionTime), 수율(yield) 관리

### 주문(Order) 관리
- 주문 생성 (상태: RESERVED)
- 생산 시작 → 생산 라인 자동 할당 (상태: PRODUCING)
- 주문 취소 (상태: REJECTED)
- 상태별 목록 조회

### 생산 라인(ProductionLine) 관리
- 생산 진행률 갱신 (0~100%)
- 일시중지 / 재개
- 생산 완료 처리 → 재고 자동 증가 + 주문 CONFIRMED 전환

### 출고(Shipment) 처리
- CONFIRMED 주문 출고 처리 → 재고 자동 차감 + 주문 RELEASED 전환
- 출고 이력 조회

### 모니터링(Monitoring)
- 대시보드: 주문 상태별 집계 + 시료 재고 현황
- 생산 현황: 라인별 진행률 실시간 표시

### 더미 데이터 생성(DummyData)
- 시료 / 주문 / 생산 라인 대량 생성 (최대 1,000 / 10,000 / 10,000건)

---

## 실행 방법

### 빌드
Visual Studio에서 `SampleOrderSystem.vcxproj`를 열고 빌드합니다.

### 실행
```
SampleOrderSystem.exe
```

실행 디렉토리 기준으로 `data/` 폴더가 없으면 자동 생성됩니다.

### 초기 데이터 준비
더미 데이터가 없는 경우, 실행 후 메인 메뉴에서 **2. DummyData Generator**를 선택하여 초기 데이터를 생성합니다.

---

## JSON 데이터 구조 설명

모든 데이터는 `data/` 디렉토리의 JSON 파일로 저장됩니다.

### data/samples.json — 시료 마스터
```json
{
  "nextId": 204,
  "items": [
    {
      "id": 1,
      "name": "플레이트-A형",
      "description": "A형 컴포넌트 검사 플레이트",
      "stock": 10,
      "avgProductionTime": 4,
      "yield": 95.5
    }
  ]
}
```
| 필드 | 타입 | 설명 |
|------|------|------|
| id | int | 시료 고유 식별자 |
| name | string | 시료명 |
| description | string | 시료 설명 |
| stock | int | 현재 재고 수량 |
| avgProductionTime | int | 평균 생산 시간 (분) |
| yield | double | 생산 수율 (%) |

---

### data/orders.json — 주문 내역
```json
{
  "nextId": 213,
  "items": [
    {
      "id": 1,
      "sampleId": 1,
      "productName": "플레이트-A형",
      "customerName": "삼성전자",
      "quantity": 3,
      "status": "RESERVED"
    }
  ]
}
```
| 필드 | 타입 | 설명 |
|------|------|------|
| id | int | 주문 고유 식별자 |
| sampleId | int | 시료 ID (samples.json 참조) |
| productName | string | 제품명 |
| customerName | string | 고객사명 |
| quantity | int | 주문 수량 |
| status | string | RESERVED / PRODUCING / CONFIRMED / RELEASED / REJECTED |

---

### data/production_lines.json — 생산 라인
```json
{
  "nextId": 201,
  "items": [
    {
      "id": 1,
      "orderId": 60,
      "lineName": "LINE-Y01",
      "status": "RUNNING",
      "progress": 45
    }
  ]
}
```
| 필드 | 타입 | 설명 |
|------|------|------|
| id | int | 생산 라인 고유 식별자 |
| orderId | int | 주문 ID (orders.json 참조) |
| lineName | string | 라인명 |
| status | string | IDLE / RUNNING / PAUSED / DONE |
| progress | int | 진행률 (0~100) |

---

### data/shipments.json — 출고 이력
```json
{
  "nextId": 1,
  "items": [
    {
      "id": 1,
      "orderId": 5,
      "sampleId": 2,
      "quantity": 3,
      "customerName": "삼성전자",
      "shippedAt": "2026-05-08"
    }
  ]
}
```
| 필드 | 타입 | 설명 |
|------|------|------|
| id | int | 출고 고유 식별자 |
| orderId | int | 출고 처리된 주문 ID |
| sampleId | int | 출고 시료 ID |
| quantity | int | 출고 수량 |
| customerName | string | 수신 고객사 |
| shippedAt | string | 출고 일자 (YYYY-MM-DD) |

---

## 콘솔 화면 설명

### 최상위 메뉴
```
========================================
     SampleOrderSystem
========================================
 1. SampleOrderSystem (업무 시스템)
 2. DummyData Generator (테스트 데이터)
 0. 종료
========================================
```

### SampleOrderSystem 메인 메뉴
```
========================================
     메인 메뉴
========================================
 1. 시료 관리
 2. 주문 관리
 3. 생산 라인 관리
 4. 출고 관리
 5. 모니터링
 0. 뒤로
========================================
```

### 주문 관리 메뉴
```
 1. 주문 생성
 2. 주문 목록 조회
 3. 생산 시작 (RESERVED → PRODUCING)
 4. 주문 취소 (→ REJECTED)
 0. 뒤로
```

### 생산 라인 관리 메뉴
```
 1. 생산 라인 목록
 2. 진행률 갱신
 3. 생산 완료 처리 (→ DONE)
 4. 일시 중지
 5. 재개
 0. 뒤로
```

### 출고 관리 메뉴
```
 1. 출고 처리 (CONFIRMED → RELEASED)
 2. 출고 이력 조회
 0. 뒤로
```

### 모니터링 메뉴
```
 1. 대시보드 (주문 집계 + 재고 현황)
 2. 주문 현황
 3. 시료 재고 현황
 4. 생산 현황
 0. 뒤로
```

---

## MVC 구조 설명

```
View  (사용자 입력/출력)
  │   입력값 전달, 결과 표시
  ▼
Controller  (흐름 제어)
  │   업무 처리 위임
  ▼
Service  (업무 규칙)
  │   데이터 접근 위임
  ▼
Repository  (데이터 CRUD)
  │   파일 읽기/쓰기
  ▼
JsonFileStorage  (JSON 파일 영속화)
```

| 계층 | 책임 | 금지 사항 |
|------|------|----------|
| View | 입력 수집, 결과 표시 | 업무 판단, 직접 데이터 접근 |
| Controller | 메뉴 흐름 제어, View ↔ Service 연결 | 업무 규칙 직접 구현 |
| Service | 상태 전환 규칙, 재고 연동 | 직접 JSON 파일 접근 |
| Repository | CRUD + JSON 직렬화/역직렬화 | 업무 규칙 포함 |
