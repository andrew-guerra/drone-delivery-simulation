#include "../include/GUIDataObserver.h"

GUIDataObserver* GUIDataObserver::instancePtr = nullptr;

GUIDataObserver::GUIDataObserver() { this->obj = JsonObject(); }

GUIDataObserver* GUIDataObserver::GetInstance() {
  if (GUIDataObserver::instancePtr == nullptr) {
    GUIDataObserver::instancePtr = new GUIDataObserver();
  }
  return GUIDataObserver::instancePtr;
}

GUIDataObserver::~GUIDataObserver() {}

JsonObject GUIDataObserver::GetJSON() { return this->obj; }

void GUIDataObserver::Update(JsonObject newObj) { this->obj = newObj; }