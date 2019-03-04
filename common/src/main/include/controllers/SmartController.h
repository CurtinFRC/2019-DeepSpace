#pragma once

// #include <functional>
#include <map>
#include <utility>
#include <vector>

#include "controllers/Controller.h"
#include "controllers/Inputs.h"
#include "Toggle.h"

namespace curtinfrc {
  namespace controllers {
    struct tInput {
      int cont, id;

      bool operator== (const tInput comp) {
        return this->cont == comp.cont && this->id == comp.id;
      };

      bool operator!= (const tInput comp) {
        return !(*this == comp);
      };

      tInput(int contIn, int idIn) : cont(contIn), id(idIn) {};
    };

    struct tAxis : public tInput { tAxis(int cont, int id) : tInput(cont, id) {}; };
    const tAxis noAxis(-1, -1);
    struct tButton : public tInput { tButton(int cont, int id) : tInput(cont, id) {}; };
    const tButton noButton(-1, -1);
    struct tPOV : public tInput { tPOV(int cont, int id) : tInput(cont, id) {}; };
    const tPOV noPOV(-1, -1);


    struct SmartControllerConfig {
      const int nAxi;
      const int nButtons;
      const int nPOVs;
    };

    class SmartController : public Controller {
     public:
      SmartController(frc::GenericHID *cont, SmartControllerConfig contConfig) : Controller(cont) {
        for (int i = 0; i < contConfig.nAxi; i++) _axi[i] = new inputs::Axis(*this, i);
        for (int i = 0; i < contConfig.nButtons; i++) _buttons[i] = new inputs::Button(*this, i);
        for (int i = 0; i < contConfig.nPOVs; i++) _POVs[i] = new inputs::POV(*this, i);
      };

      // 'value' determines whether checking for existance or absence
      bool Exists(tAxis axis, bool value = true);
      bool Exists(tButton button, bool value = true);
      bool Exists(tPOV pov, bool value = true);

      bool Exists(std::vector<tAxis> axis, bool value = true);
      bool Exists(std::vector<tButton> button, bool value = true);
      bool Exists(std::vector<tPOV> pov, bool value = true);


      void Map(tAxis map_axis, tButton virt_button, double threshold = 0.3);
      void Map(tAxis map_axis, std::vector<tButton> virt_buttons);
      void PairAxis(tAxis primary_axis, tAxis secondary_axis, bool squared = false);

      void Map(std::pair<tButton, tButton> map_buttons, std::vector<tButton> virt_buttons);

      void Map(tPOV map_POV, std::vector<tButton> virt_buttons);


      // Controller::Get overrides
      virtual double GetAxis(int axis = 1) override { return Get(tAxis(-1, axis)); };
      virtual bool GetButton(int button = 1) override { return Get(tButton(-1, button)); };
      virtual int GetPOVAngle(int pov = 0) override { return (int)Get(tPOV(-1, pov)); };
      virtual POVPos GetPOV(int pov = 0) override { return Get(tPOV(-1, pov)); };

      virtual double Get(tAxis axis);
      virtual bool Get(tButton button);
      virtual POVPos Get(tPOV pov);


     protected:
      inputs::ContAxis *GetAxisObj(int axis) { return GetObj(tAxis(-1, axis)); };
      inputs::ContButton *GetButtonObj(int button) { return GetObj(tButton(-1, button)); };
      inputs::ContPOV *GetPOVObj(int pov) { return GetObj(tPOV(-1, pov)); };

      inputs::ContAxis *GetObj(tAxis axis);
      inputs::ContButton *GetObj(tButton button);
      inputs::ContPOV *GetObj(tPOV pov);

     private:
      std::map<int, inputs::ContAxis*> _axi;
      std::map<int, inputs::ContButton*> _buttons;
      std::map<int, inputs::ContPOV*> _POVs;
    };
  } // ns controllers
} // ns curtinfrc
