#include "lora-energy-model-helper.h"
#include "ns3/lora-net-device.h"
//#include "ns3/wifi-tx-current-model.h"

namespace ns3 {

LoraEnergyModelHelper::LoraEnergyModelHelper ()
{
  m_radioEnergy.SetTypeId ("ns3::LoraEnergyModel");
  m_depletionCallback.Nullify ();
  m_rechargedCallback.Nullify ();
}

LoraEnergyModelHelper::~LoraEnergyModelHelper ()
{
}

void
LoraEnergyModelHelper::Set (std::string name, const AttributeValue &v)
{
  m_radioEnergy.Set (name, v);
}

void
LoraEnergyModelHelper::SetDepletionCallback (
  LoraEnergyModel::LoraEnergyDepletionCallback callback)
{
  m_depletionCallback = callback;
}

void
LoraEnergyModelHelper::SetRechargedCallback (
  LoraEnergyModel::LoraEnergyRechargedCallback callback)
{
  m_rechargedCallback = callback;
}

void
LoraEnergyModelHelper::SetTxCurrentModel (std::string name,
                                               std::string n0, const AttributeValue& v0,
                                               std::string n1, const AttributeValue& v1,
                                               std::string n2, const AttributeValue& v2,
                                               std::string n3, const AttributeValue& v3,
                                               std::string n4, const AttributeValue& v4,
                                               std::string n5, const AttributeValue& v5,
                                               std::string n6, const AttributeValue& v6,
                                               std::string n7, const AttributeValue& v7)
{
  ObjectFactory factory;
  factory.SetTypeId (name);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  //m_txCurrentModel = factory;
}


/*
 * Private function starts here.
 */

Ptr<DeviceEnergyModel>
LoraEnergyModelHelper::DoInstall (Ptr<NetDevice> device,
                                       Ptr<EnergySource> source) const
{
  NS_ASSERT (device != NULL);
  NS_ASSERT (source != NULL);
  // check if device is LoraNetDevice
  std::string deviceName = device->GetInstanceTypeId ().GetName ();
  if (deviceName.compare ("ns3::LoraNetDevice") != 0)
    {
      NS_FATAL_ERROR ("NetDevice type is not LoraNetDevice!");
    }
  Ptr<Node> node = device->GetNode ();
  Ptr<LoraEnergyModel> model = m_radioEnergy.Create ()->GetObject<LoraEnergyModel> ();
  NS_ASSERT (model != NULL);
  // set energy source pointer
  model->SetEnergySource (source);
  Ptr<LoraNetDevice> LoraDevice = DynamicCast<LoraNetDevice> (device);
  Ptr<LoraPhy> LoraPhy = LoraDevice->GetPhy ();
  LoraPhy->RegisterListener (model->GetPhyListener ());
  return model;
}

} // namespace ns3
