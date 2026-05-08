#include <windows.h>

// ── Persistence ──────────────────────────────────────────────────
#include "persistence/JsonFileStorage.h"

// ── Repositories ─────────────────────────────────────────────────
#include "repositories/SampleRepository.h"
#include "repositories/OrderRepository.h"
#include "repositories/ProductionLineRepository.h"
#include "repositories/ShipmentRepository.h"

// ── Services ─────────────────────────────────────────────────────
#include "services/SampleService.h"
#include "services/InventoryService.h"
#include "services/OrderService.h"
#include "services/ProductionService.h"
#include "services/ShipmentService.h"
#include "services/MonitoringService.h"

// ── Views ─────────────────────────────────────────────────────────
#include "views/MainMenuView.h"
#include "views/SampleView.h"
#include "views/OrderView.h"
#include "views/ProductionLineView.h"
#include "views/MonitoringView.h"
#include "views/ShipmentView.h"

// ── Controllers ───────────────────────────────────────────────────
#include "controllers/MainController.h"
#include "controllers/SampleController.h"
#include "controllers/OrderController.h"
#include "controllers/ProductionLineController.h"
#include "controllers/MonitoringController.h"
#include "controllers/ShipmentController.h"

// ── DummyData Generator ───────────────────────────────────────────
#include "utils/RandomUtil.h"
#include "generators/SampleGenerator.h"
#include "generators/OrderGenerator.h"
#include "generators/ProductionLineGenerator.h"
#include "persistence/JsonFileWriter.h"
#include "services/GeneratorService.h"
#include "views/ConsoleView.h"
#include "controllers/DummyDataController.h"

int main()
{
    // ── Persistence ───────────────────────────────────────────────
    JsonFileStorage sampleStorage         ("data/samples.json");
    JsonFileStorage orderStorage          ("data/orders.json");
    JsonFileStorage productionLineStorage ("data/production_lines.json");
    JsonFileStorage shipmentStorage       ("data/shipments.json");

    // ── Repositories ──────────────────────────────────────────────
    SampleRepository         sampleRepo(sampleStorage);
    OrderRepository          orderRepo(orderStorage);
    ProductionLineRepository productionLineRepo(productionLineStorage);
    ShipmentRepository       shipmentRepo(shipmentStorage);

    // ── Services (의존성 순서 준수) ───────────────────────────────
    SampleService     sampleService    (sampleRepo);
    InventoryService  inventoryService (sampleRepo);
    OrderService      orderService     (orderRepo, productionLineRepo, sampleRepo, inventoryService);
    ProductionService productionService(productionLineRepo, orderRepo, sampleRepo,
                                        inventoryService, orderService);
    ShipmentService   shipmentService  (orderRepo, shipmentRepo, inventoryService);
    MonitoringService monitoringService(orderRepo, sampleRepo, productionLineRepo);

    // ── DummyData Generator ───────────────────────────────────────
    RandomUtil              random;
    JsonFileWriter          writer("data");
    SampleGenerator         sampleGen(random);
    OrderGenerator          orderGen(random);
    ProductionLineGenerator productionLineGen(random);
    GeneratorService        generatorService(writer, sampleGen, orderGen, productionLineGen);
    ConsoleView             consoleView;
    DummyDataController     dummyCtrl(consoleView, generatorService);

    // ── Views ─────────────────────────────────────────────────────
    MainMenuView       mainView;
    SampleView         sampleView;
    OrderView          orderView;
    ProductionLineView productionLineView;
    MonitoringView     monitoringView;
    ShipmentView       shipmentView;

    // ── Controllers ───────────────────────────────────────────────
    SampleController         sampleCtrl        (sampleView,         sampleService);
    OrderController          orderCtrl         (orderView,          orderService);
    ProductionLineController productionLineCtrl(productionLineView, productionService);
    ShipmentController       shipmentCtrl      (shipmentView,       shipmentService);
    MonitoringController     monitoringCtrl    (monitoringView,     monitoringService);

    MainController controller(mainView,
                              sampleCtrl,
                              orderCtrl,
                              productionLineCtrl,
                              monitoringCtrl,
                              shipmentCtrl);

    // ── 최상위 메뉴 ──────────────────────────────────────────────
    while (true)
    {
        consoleView.showMainMenu();
        const int mode = consoleView.readMainMenuChoice();
        if      (mode == 0) break;
        else if (mode == 1) controller.run();
        else if (mode == 2) dummyCtrl.run();
    }

    return 0;
}
