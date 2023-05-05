#ifndef GUI_DATA_OBSERVER_
#define GUI_DATA_OBSERVER_

#include "./util/json.h"
// getjson
// update
/**
 * @brief A class to represent the observer with a GUI
 *
 */
class GUIDataObserver {
 public:
  /**
   * @brief Destroy the GUIDataObserver object
   *
   */
  ~GUIDataObserver();
  /**
   * @brief Returns the current JsonObject object
   *
   * @return JsonObject
   */
  JsonObject GetJSON();
  /**
   * @brief Get the Instance object
   *
   * @return GUIDataObserver*
   */
  static GUIDataObserver* GetInstance();
  /**
   * @brief Set the current JsonObject
   *
   */
  void SetJSON(JsonObject);

 private:
  /**
   * @brief Construct a new GUIDataObserver object
   *
   */
  GUIDataObserver();
  JsonObject obj;
  static GUIDataObserver* instancePtr;
};
#endif