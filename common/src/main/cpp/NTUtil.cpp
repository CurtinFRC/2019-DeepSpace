#include "NTUtil.h"

using namespace curtinfrc;

NTBoundDouble::NTBoundDouble(std::shared_ptr<nt::NetworkTable> table, std::string name, double *value) : _table(table), _name(name), _val(value) {
  _entry = table->GetEntry(name);
  _entry.ForceSetDouble(*value);
  _listener = _entry.AddListener([=](const nt::EntryNotification &evt) {
    *_val = evt.value->GetDouble();
  }, NT_NOTIFY_UPDATE);
}

NTBoundDouble::~NTBoundDouble() {
  _table->RemoveEntryListener(_listener);
}

void NTBoundDouble::Override(double newVal) {
  _entry.ForceSetDouble(newVal); 
  *_val = newVal; 
}