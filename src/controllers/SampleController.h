#pragma once
#include "IController.h"
#include "../views/SampleView.h"
#include "../services/SampleService.h"

class SampleController : public IController
{
public:
    SampleController(SampleView& view, SampleService& service);
    void run() override;

private:
    SampleView&    m_view;
    SampleService& m_service;

    void handleChoice(int choice);
    void handleCreate();
    void handleList();
    void handleSearch();
    void handleUpdate();
    void handleDelete();
};
